#include "sbioinfo/variant.h"
#include "sbioinfo/gff.h"
#include "sbioinfo/vcf.h"
slib::sbio::SVar::SVar() : type(0), qual(0.0) { read[0] = 0; read[1] = 0; }
slib::sbio::SVar::SVar(const SVar& v) :SVar() {
	type = v.type;
	sforin(i, 0, 2) {
		pos[i] = v.pos[i];
		read[i] = v.read[i];
	}
	alt = v.alt;
	qual = v.qual;
}
slib::sbio::SVar::~SVar() {}
slib::sbio::SVar& slib::sbio::SVar::operator = (const SVar& v) {
	type = v.type; 
	sforin(i, 0, 2) {
		pos[i] = v.pos[i];
		read[i] = v.read[i];
	}
	alt = v.alt; 
	qual = v.qual;
	return (*this);
}
slib::sbio::SVar& slib::sbio::SVar::operator += (const SVar& v) {
	read[0] += v.read[0];
	read[1] += v.read[1];
	qual = qual * v.qual;
	return (*this);
}

int slib::sbio::SVar::length() const {
	switch (type & 0xFF) {
	case DELETION:
		return pos[1].begin - pos[0].end - 1;
	case DUPLICATION:
		return pos[0].end - pos[1].begin + 1;
	case INSERTION:
		return pos[1].begin - pos[0].end - 1;
	case INVERSION:
		return (pos[0].dir? pos[1].begin - pos[0].begin : pos[1].end - pos[0].end) - 1;
	default:
		return 0;
	}
}
void slib::sbio::SVar::categorize() {
	subyte t = 0;
	if (pos[0].idx == pos[1].idx) {
		if (pos[0].dir == pos[1].dir) {
			if (pos[1].begin <= pos[0].end) t = slib::sbio::DUPLICATION;
			else {
				int diff = pos[1].begin - pos[0].end - 1;
				if (alt.size() < diff) t = slib::sbio::DELETION;
				else if (alt.size()) t = slib::sbio::INSERTION;
			}
		}
		else {
			t = slib::sbio::INVERSION;
			if (pos[1].begin < pos[0].begin) complement();
		}
	}
	else {
		t = slib::sbio::TRANSLOCATION;
		if (pos[0].dir != pos[1].dir) {
			t |= slib::sbio::INVERSION;
			if (pos[1].idx < pos[0].idx) complement();
		}
	}
	type = (type & 0xFF00) | t;
}
/*
void slib::sbio::SVar::shaping() {
	auto t = type & 0xFF;
	if (t == INSERTION) {


	}
	else if (t & INSERTION) {


	}
	else {
		if (t == DELETION || t == TRANSLOCATION) {
			pos[0].begin = pos[0].end;
			pos[1].end = pos[1].begin;
		}
		else if (t == DUPLICATION) {
			pos[0].begin = pos[0].end;
			pos[1].end = pos[1].begin;
		}
		else if (t & INVERSION) {
			if (pos[0].dir) {
				pos[0].end = pos[0].begin;
				pos[1].end = pos[1].begin;
			}
			else {
				pos[0].begin = pos[0].end;
				pos[1].begin = pos[1].end;
			}
		}
	}
}
*/
/*
	if (var.type & INVERSION &&
		(var.pos[1].idx < var.pos[0].idx ||
			(var.pos[0].idx == var.pos[1].idx && var.pos[1].begin < var.pos[0].begin))) {
		auto tmp = var.pos[0];
		var.pos[0] = var.pos[1];
		var.pos[1] = tmp;
		var.pos[0].dir = !var.pos[0].dir;
		var.pos[1].dir = !var.pos[1].dir;
		auto tmpr = var.read[0];
		var.read[0] = var.read[1];
		var.read[1] = tmpr;
		if (var.alt.length()) sseq::dcomp(var.alt);
		vidx = var.pos[0].idx * 5 + (var.type & TRANSLOCATION ? 4 : 2);
	}
	return vidx;
}
*/
bool slib::sbio::SVar::isStrict() const { return (type >> 8) == 0; }
bool slib::sbio::SVar::isComplex() const {
	auto t = (type & 0xFF);
	return (t & INSERTION) && t != INSERTION;
}
void slib::sbio::SVar::complement() {
	Memory<RefPos>::swap(&pos[0], &pos[1]);
	pos[0].dir = !pos[0].dir; pos[1].dir = !pos[1].dir;
	Memory<sushort>::swap(&read[0], &read[1]);
	if (alt.size()) sna::toComplement(alt);
}
int slib::sbio::SVar::total() const { return read[0] + read[1]; }
double slib::sbio::SVar::bias() const { return sbio::sutil::readBias(read); }
bool slib::sbio::SVar::comparable(const SVar& v) const {
	return (type & 0xFF) == (v.type & 0xFF) &&
		pos[0].idx == v.pos[0].idx && pos[1].idx == v.pos[1].idx &&
		pos[0].dir == v.pos[0].dir && pos[1].dir == v.pos[1].dir;
}
bool slib::sbio::SVar::lt(const SVar& var, SVParam* par) const {
	switch (type & 0xFF) {
	case DELETION:
		return pos[1].begin < var.pos[0].end;
	case DUPLICATION:
		return pos[0].end < var.pos[1].begin;
	case INSERTION:
		return pos[0].end + par->break_site_len < var.pos[0].end;
	case INVERSION:
		return pos[0].dir ? pos[1].begin < var.pos[0].begin : pos[1].end < var.pos[0].end;
	case TRANSLOCATION:
		return pos[0].end + par->break_site_len < var.pos[0].end;
	case TRANSLOCATION | INVERSION:
		return pos[0].dir ? pos[0].begin + par->break_site_len < var.pos[0].begin : pos[0].end + par->break_site_len < var.pos[0].end;
	default:
		return false;
	}
}
bool _checkBreakSite(const slib::sbio::SVar* v1, const slib::sbio::SVar* v2, slib::sbio::SVParam* par) {
	bool eq1 = false, eq2 = false;
	auto t1 = (v1->type >> 8) & slib::sbio::UNCLEAR_TAIL,
		t2 = (v2->type >> 8) & slib::sbio::UNCLEAR_TAIL;
	int dif = 0;
	if (t1 || t2) {
		if (t1) {
			// both has unclear tail
			if (t2) dif = std::abs(v1->pos[0].dir ? (v1->pos[0].begin - v2->pos[0].begin) : (v1->pos[0].end - v2->pos[0].end));
			// only v2 has strict tail
			else dif = v1->pos[0].dir ? (v1->pos[0].begin - v2->pos[0].begin) : (v2->pos[0].end - v1->pos[0].end);
		}
		// only v1 has strict tail
		else if (t2) dif = v1->pos[0].dir ? (v2->pos[0].begin - v1->pos[0].begin) : (v1->pos[0].end - v2->pos[0].end); 
		eq1 = -par->break_site_len <= dif && dif <= par->max_gap;
	}
	else {
		dif = v1->pos[0].dir ? (v2->pos[0].begin - v1->pos[0].begin) : (v1->pos[0].end - v2->pos[0].end);
		auto adif = (int)(v1->alt.match("|") ? v1->alt.find("|") : v1->alt.size()) - 
			(int)(v2->alt.match("|") ? v2->alt.find("|") : v2->alt.size());
		eq1 = std::abs(dif) <= par->break_site_len || std::abs(dif + adif) <= par->break_site_len;
	}
	//
	t1 = (v1->type >> 8) & slib::sbio::UNCLEAR_HEAD;
	t2 = (v2->type >> 8) & slib::sbio::UNCLEAR_HEAD;
	if (t1 || t2) {
		if (t1) {
			// both has unclear head
			if (t2) dif = dif = std::abs(v1->pos[1].dir ? (v1->pos[1].end - v2->pos[1].end) : (v1->pos[1].begin - v2->pos[1].begin));
			// only v2 has strict head
			else dif = v1->pos[1].dir ? (v2->pos[1].end - v1->pos[1].end) : (v1->pos[1].begin - v2->pos[1].begin);
		}
		// only v1 has strict head
		else if (t2) dif = v1->pos[0].dir ? (v1->pos[1].end - v2->pos[1].end) : (v2->pos[1].begin - v1->pos[1].begin);
		eq2 = -par->break_site_len <= dif && dif <= par->max_gap;
	}
	else {
		dif = v1->pos[1].dir ? (v1->pos[1].end - v2->pos[1].end) : (v2->pos[1].begin - v1->pos[1].begin);
		auto adif = (int)(v1->alt.size() - (v1->alt.match("|") ? v1->alt.find("|") : 0)) -
			(int)(v2->alt.size() - (v2->alt.match("|") ? v2->alt.find("|") : 0));
		eq2 = std::abs(dif) <= par->break_site_len || std::abs(dif + adif) <= par->break_site_len;
	}
	return eq1 && eq2;
}
bool _checkBreakSeq(const slib::sbio::SVar* v1, const slib::sbio::SVar* v2, slib::sbio::SeqList* ref, slib::sbio::SVParam* par) {
	// break site check
	auto t1 = (v1->type >> 8), t2 = (v2->type >> 8);
	bool strict[2] = { true, true };
	if ((t1 & slib::sbio::UNCLEAR_TAIL) || (t2 & slib::sbio::UNCLEAR_TAIL)) strict[0] = false;
	if ((t1 & slib::sbio::UNCLEAR_HEAD) || (t2 & slib::sbio::UNCLEAR_HEAD)) strict[1] = false;
	if (!strict[0] && !strict[1]) return true;
	//
	slib::String buffer[2];
	
	// insertion check
	bool confins[2] = { true, true };
	slib::String altbuf[2][2];
	if (v1->alt.match("|")) {
		confins[0] = false;
		auto div = v1->alt.find("|");
		altbuf[0][0] = v1->alt.substring(0, div);
		altbuf[0][1] = v1->alt.substring(div + 1);
	}
	else {
		altbuf[0][0] = v1->alt;
		altbuf[0][1] = v1->alt;
	}
	if (v2->alt.match("|")) {
		confins[1] = false;
		auto div = v2->alt.find("|");
		altbuf[1][0] = v2->alt.substring(0, div);
		altbuf[1][1] = v2->alt.substring(div + 1);
	}
	else {
		altbuf[1][0] = v2->alt;
		altbuf[1][1] = v2->alt;
	}

	/*
	int diff1, diff2;
	// diff. of break pos
	if (v1->pos[0].dir) diff1 = v1->pos[0].begin - v2->pos[0].begin;
	else diff1 = v1->pos[0].end - v2->pos[0].end;
	if (v1->pos[1].dir) diff2 = v1->pos[1].end - v2->pos[1].end;
	else diff2 = v1->pos[1].begin - v2->pos[1].begin;
	*/

	int len[2] = { 0, 0 };
	if (strict[0] && strict[1]) {
		//
		if (v1->pos[0].dir) {
			auto end = (v1->pos[0].begin < v2->pos[0].begin ? v1->pos[0].begin : v2->pos[0].begin) + par->break_site_len - 1;
			len[0] = end - v1->pos[0].begin + 1; len[1] = end - v2->pos[0].begin + 1;
			if (len[0] <= 0) len[0] = 0;
			else buffer[0] = ref->at(v1->pos[0].idx).raw(v1->pos[0].begin, len[0], true);
			if (len[1] <= 0) len[1] = 0;
			else buffer[1] = ref->at(v2->pos[0].idx).raw(v2->pos[0].begin, len[1], true);
		}
		//
		else {
			auto beg = (v1->pos[0].end < v2->pos[0].end ? v2->pos[0].end : v1->pos[0].end) - par->break_site_len + 1;
			len[0] = v1->pos[0].end - beg + 1; len[1] = v2->pos[0].end - beg + 1;
			if (len[0] <= 0) len[0] = 0;
			else buffer[0] = ref->at(v1->pos[0].idx).raw(beg, len[0]);
			if (len[1] <= 0) len[1] = 0;
			else buffer[1] = ref->at(v2->pos[0].idx).raw(beg, len[1]);
		}
		//
		len[0] = par->break_site_len - len[0];
		len[1] = par->break_site_len - len[1];
		//
		if (confins[0] && len[0] && altbuf[0][0].size()) {
			if (len[0] < altbuf[0][0].size()) buffer[0] << altbuf[0][0].substring(0, len[0]);
			else buffer[0] << altbuf[0][0];
		}
		//
		if (confins[1] && len[1] && altbuf[1][0].size()) {
			if (len[1] < altbuf[1][0].size()) buffer[1] << altbuf[1][0].substring(0, len[1]);
			else buffer[1] << altbuf[1][0];
		}
		//
		len[0] = par->break_site_len * 2 - buffer[0].size();
		len[1] = par->break_site_len * 2 - buffer[1].size();
		if (v1->pos[1].dir) {
			buffer[0] << ref->at(v1->pos[1].idx).raw(v1->pos[1].end - len[0] + 1, len[0], true);
			buffer[1] << ref->at(v2->pos[1].idx).raw(v2->pos[1].end - len[1] + 1, len[1], true);
		}
		else {
			buffer[0] << ref->at(v1->pos[1].idx).raw(v1->pos[1].begin, len[0]);
			buffer[1] << ref->at(v2->pos[1].idx).raw(v2->pos[1].begin, len[1]);
		}
		auto dist = slib::smath::levenshtein(buffer[0].cstr(), buffer[0].size(), buffer[1].cstr(), buffer[1].size());
		//SPrint(" >>> ", buffer[0], slib::NL, " >>> ", buffer[1], slib::NL, "Dist. = ", dist);
		return dist <= par->max_dist;
	}
	else {
		// Left
		if (strict[0]) {
			//
			if (v1->pos[0].dir) {
				auto end = (v1->pos[0].begin < v2->pos[0].begin ? v1->pos[0].begin : v2->pos[0].begin) + par->break_site_len - 1;
				len[0] = end - v1->pos[0].begin + 1; len[1] = end - v2->pos[0].begin + 1;
				if (len[0] <= 0) len[0] = 0;
				else buffer[0] = ref->at(v1->pos[0].idx).raw(v1->pos[0].begin, len[0], true);
				if (len[1] <= 0) len[1] = 0;
				else buffer[1] = ref->at(v2->pos[0].idx).raw(v2->pos[0].begin, len[1], true);
			}
			//
			else {
				auto beg = (v1->pos[0].end < v2->pos[0].end ? v2->pos[0].end : v1->pos[0].end) - par->break_site_len + 1;
				len[0] = v1->pos[0].end - beg + 1; len[1] = v2->pos[0].end - beg + 1;
				if (len[0] <= 0) len[0] = 0;
				else buffer[0] = ref->at(v1->pos[0].idx).raw(beg, len[0]);
				if (len[1] <= 0) len[1] = 0;
				else buffer[1] = ref->at(v2->pos[0].idx).raw(beg, len[1]);
			}
			//
			len[0] = par->break_site_len * 2 - len[0];
			len[1] = par->break_site_len * 2 - len[1];
			//
			if (len[0] && altbuf[0][0].size()) {
				if (len[0] < altbuf[0][0].size()) buffer[0] << altbuf[0][0].substring(0, len[0]);
				else buffer[0] << altbuf[0][0];
			}
			//
			if (len[1] && altbuf[1][0].size()) {
				if (len[1] < altbuf[1][0].size()) buffer[1] << altbuf[1][0].substring(0, len[1]);
				else buffer[1] << altbuf[1][0];
			}
		}
		//Right
		else if (strict[1]) {
			//
			if (v1->pos[1].dir) {
				auto beg = (v1->pos[1].end < v2->pos[1].end ? v2->pos[1].end : v1->pos[1].end) - par->break_site_len + 1;
				len[0] = v1->pos[1].end - beg + 1; len[1] = v2->pos[1].end - beg + 1;
				if (len[0] <= 0) len[0] = 0;
				else buffer[0] = ref->at(v1->pos[1].idx).raw(beg, len[0], true);
				if (len[1] <= 0) len[1] = 0;
				else buffer[1] = ref->at(v2->pos[1].idx).raw(beg, len[1], true);
			}
			//
			else {
				auto end = (v1->pos[1].begin < v2->pos[1].begin ? v1->pos[1].begin : v2->pos[1].begin) + par->break_site_len - 1;
				len[0] = end - v1->pos[1].begin + 1; len[1] = end - v2->pos[1].begin + 1;
				if (len[0] <= 0) len[0] = 0;
				else buffer[0] = ref->at(v1->pos[1].idx).raw(v1->pos[1].begin, len[0]);
				if (len[1] <= 0) len[1] = 0;
				else buffer[1] = ref->at(v2->pos[1].idx).raw(v2->pos[1].begin, len[1]);
			}
			//
			len[0] = par->break_site_len * 2 - len[0];
			len[1] = par->break_site_len * 2 - len[1];
			//
			if (len[0] && altbuf[0][1].size()) {
				if (len[0] < altbuf[0][1].size()) buffer[0] = altbuf[0][1].substring(altbuf[0][1].size() - len[0]) + buffer[0];
				else buffer[0] = altbuf[0][1] + buffer[0];
			}
			//
			if (len[1] && altbuf[1][1].size()) {
				if (len[1] < altbuf[1][1].size()) buffer[1] = altbuf[1][1].substring(altbuf[1][1].size() - len[1]) + buffer[1];
				else buffer[1] = altbuf[1][1] + buffer[1];
			}
		}
		auto dist = slib::smath::levenshtein(buffer[0].cstr(), buffer[0].size(), buffer[1].cstr(), buffer[1].size());
		//SPrint(" >>> ", buffer[0], slib::NL, " >>> ", buffer[1], slib::NL, "Dist. = ",dist);
		return dist <= par->max_dist;
	}
}
bool slib::sbio::SVar::equal(const SVar& var, SeqList* ref, SVParam *par) const {
	if (!comparable(var)) return false;
	auto t = (type & 0xFF);
	switch (t) {
	case DELETION:
	{
		auto range1 = srange(pos[0].end + 1, pos[1].begin - 1),
			range2 = srange(var.pos[0].end + 1, var.pos[1].begin - 1);
		return range1.overlap(range2) &&
			std::abs(range1.length() - range2.length()) <= par->max_gap &&
			_checkBreakSite(this, &var, par) &&
			_checkBreakSeq(this, &var, ref, par);
	}
	case DUPLICATION:
	{
		auto range1 = srange(pos[1].begin, pos[0].end),
			range2 = srange(var.pos[1].begin, var.pos[0].end);
		return range1.overlap(range2) &&
			std::abs(range1.length() - range2.length()) <= par->max_gap &&
			_checkBreakSite(this, &var, par) &&
			_checkBreakSeq(this, &var, ref, par);
	}
	case INSERTION:
	{
		auto range1 = srange(pos[0].end + 1, pos[1].begin - 1),
			range2 = srange(var.pos[0].end + 1, var.pos[1].begin - 1);
		return range1.overlap(range2) &&
			std::abs((int)alt.size() - (int)var.alt.size()) <= par->max_gap &&
			_checkBreakSite(this, &var, par) &&
			_checkBreakSeq(this, &var, ref, par);
	}
	case INVERSION:
	{
		auto range1 = srange(pos[0].dir ? pos[0].begin - 1 : pos[0].end + 1,
			pos[0].dir ? pos[1].begin - 1 : pos[1].end + 1),
			range2 = srange(var.pos[0].dir ? var.pos[0].begin - 1 : var.pos[0].end + 1,
				var.pos[0].dir ? var.pos[1].begin - 1 : var.pos[1].end + 1);
		return range1.overlap(range2) &&
			std::abs(range1.length() - range2.length()) <= par->max_gap &&
			_checkBreakSite(this, &var, par) &&
			_checkBreakSeq(this, &var, ref, par);
	}
	default:
	{
		return _checkBreakSite(this, &var, par) &&
			_checkBreakSeq(this, &var, ref, par);
	}
	}
}
void slib::sbio::SVar::merge(SVar& var, SVParam* par) {
	auto v1 = this, v2 = &var;
	String alt;
	//
	auto t1 = (v1->type >> 8) & slib::sbio::UNCLEAR_TAIL,
		t2 = (v2->type >> 8) & slib::sbio::UNCLEAR_TAIL;
	auto as1 = v1->alt.match("|") ? v1->alt.substring(0, v1->alt.find("|")) : v1->alt,
		as2 = v2->alt.match("|") ? v2->alt.substring(0, v2->alt.find("|")) : v2->alt;
	//
	if (t1 || t2) {
		if (t1) {
			// both has unclear tail
			if (t2) {
				if (v1->pos[0].dir) {
					if (v2->pos[0].begin < v1->pos[0].begin) {
						v1->pos[0].begin = v2->pos[0].begin;
						alt = as2;
					}
					else alt = as1;
				}
				else {
					if (v1->pos[0].end < v2->pos[0].end) {
						v1->pos[0].end = v2->pos[0].end;
						alt = as2;
					}
					else alt = as1;
				}
			}
			// only v2 has strict tail
			else {
				if (v1->pos[0].dir) v1->pos[0].begin = v2->pos[0].begin;
				else v1->pos[0].end = v2->pos[0].end;
				v1->type -= (slib::sbio::UNCLEAR_TAIL << 8);
				alt = as2;
			}
		}
		// only v1 has strict tail
		else if (t2) alt = as1;
	}
	// both has strict tail
	else { 
		if (as2.size() < as1.size()) {
			v1 = &var;
			v2 = this;
			alt = as2;
		}
		else alt = as1;
	}
	
	//
	t1 = (v1->type >> 8) & slib::sbio::UNCLEAR_HEAD;
	t2 = (v2->type >> 8) & slib::sbio::UNCLEAR_HEAD;
	as1 = v1->alt.match("|") ? v1->alt.substring(v1->alt.find("|") + 1) : "";
	as2 = v2->alt.match("|") ? v2->alt.substring(v2->alt.find("|") + 1) : "";

	if (t1 || t2) {
		if (t1) {
			// both has unclear head
			if (t2) {
				if (v1->pos[1].dir) {
					if (v1->pos[1].end < v2->pos[1].end) {
						v1->pos[1].end = v2->pos[1].end;
						alt << (as2.empty() ? "" : ("|" + as2));
					}
					else alt << (as1.empty() ? "" : ("|" + as1));
				}
				else {
					if (v2->pos[1].begin < v1->pos[1].begin) {
						v1->pos[1].begin = v2->pos[1].begin;
						alt << (as2.empty() ? "" : ("|" + as2));
					}
					else alt << (as1.empty() ? "" : ("|" + as1));
				}
			}
			// only v2 has strict tail
			else {
				if (v1->pos[1].dir) v1->pos[1].end = v2->pos[1].end;
				else v1->pos[1].begin = v2->pos[1].begin;
				v1->type -= (slib::sbio::UNCLEAR_HEAD << 8);
				alt << (as2.empty() ? "" : ("|" + as2));
			}
		}
		// only v1 has strict tail
		else if (t2) alt << (as1.empty() ? "" : ("|" + as1));
	}
	// both has strict tail
	else alt << (as1.empty() ? "" : ("|" + as1));
	// merge
	(*v1) += (*v2);
	v1->alt = alt;
	v2->type = 0;
}
slib::String slib::sbio::SVar::toString(SeqList* ref) const {
	String str;
	str << sbio::sutil::varTypeStr(type) << TAB <<
		(ref ? ref->at(pos[0].idx).name : String(pos[0].idx)) << ":" <<
		pos[0].begin << "-" << pos[0].end <<
		((type >> 8) & UNCLEAR_TAIL ? "? " : " ") <<
		(pos[0].dir ? "(-)" : "(+)") << TAB <<
		(ref ? ref->at(pos[1].idx).name : String(pos[1].idx)) << ":" <<
		pos[1].begin << "-" << pos[1].end <<
		((type >> 8) & UNCLEAR_HEAD ? "? " : " ") << (pos[1].dir ? "(-)" : "(+)") << TAB <<
		alt << TAB << read[0] << "/" << read[1] << TAB << sbio::sutil::phredVal(qual);
	return str;
}
void slib::sbio::SVar::clear() {
	type = 0; 
	pos[0].init(); 
	pos[1].init(); 
	qual = 0.0; 
	read[0] = 0; 
	read[1] = 0; 
	alt.clear();
}
bool slib::sbio::SVar::operator < (const SVar& v) const {
	if (pos[0].idx != v.pos[0].idx) return pos[0].idx < v.pos[0].idx;
	if (pos[1].idx != v.pos[1].idx) return pos[1].idx < v.pos[1].idx;
	if (pos[0].dir != v.pos[0].dir) return pos[0].dir < v.pos[0].dir;
	if (pos[0].begin != v.pos[0].begin) return pos[0].begin < v.pos[0].begin;
	if (pos[1].begin != v.pos[1].begin) return pos[1].begin < v.pos[1].begin;
	if (pos[0].end != v.pos[0].end) return pos[0].end < v.pos[0].end;
	if (pos[1].end != v.pos[1].end) return pos[1].end < v.pos[1].end;
	if (type != v.type) return type < v.type;
	if (!(alt == v.alt)) return alt < v.alt;
	return false;
}
bool slib::sbio::SVar::operator ==(const SVar& v) const {
	return pos[0] == v.pos[0] && pos[1] == v.pos[1] && type == v.type && alt == v.alt;
}

slib::sbio::CNData::CNData() {
	depth[0] = 0.f; depth[1] = 0.f; ndepth[0] = 0.f; ndepth[1] = 0.f; copy = 1.f;
}
slib::sbio::CNData::CNData(const slib::sbio::CNData& cn) {
	depth[0] = cn.depth[0]; depth[1] = cn.depth[1]; ndepth[0] = cn.ndepth[0]; ndepth[1] = cn.ndepth[1]; copy = cn.copy;
}
slib::sbio::CNData::~CNData() {}
slib::sbio::CNData& slib::sbio::CNData::operator=(const slib::sbio::CNData& cn) {
	depth[0] = cn.depth[0]; depth[1] = cn.depth[1]; ndepth[0] = cn.ndepth[0]; ndepth[1] = cn.ndepth[1]; copy = cn.copy; return *this;
}

slib::sbio::CNVar::CNVar() : type(0), qual(0.0) {}
/*
CNVar::CNVar(CNV_TYPE t, sint p, float* s, float* b, sint bin) {
	type = t; pos = srange(p * bin, (p + 1) * bin - 1); copy[0] = (*s) * bin; copy[1] = (b ? (*b) : 1.0) * bin;
}
CNVar::CNVar(float* s, float* b, double* r, double* border) {
	type = CNVar::classify(s, b, r, border);
}
CNVar::CNVar(const CNVar& cnv) {
	type = cnv.type; pos = cnv.pos; copy[0] = cnv.copy[0]; copy[1] = cnv.copy[1]; prob = cnv.prob;
}
*/
slib::sbio::CNVar::~CNVar() {}
/*
slib::sbio::CNVar& slib::sbio::CNVar::operator=(const CNVar& cnv) {
	type = cnv.type; pos = cnv.pos; copy = cnv.copy; qual = cnv.qual;
	return  *this;
}
*/
/*
CNV_TYPE slib::sbio::CNVar::classify(float* sdp, float* bdp, double* ratio, double* border) {
	double copy = (*sdp) / (bdp ? (*bdp) : 1.0) * (*ratio);
	if (copy < border[0]) return HOMO_DEL_CNV;
	else if (copy < border[1]) return HETERO_DEL_CNV;
	else if (copy < border[2]) return NON_CNV;
	else if (copy < border[3]) return HETERO_DUP_CNV;
	else if (copy < border[4]) return HOMO_DUP_CNV;
	else return MULTI_CNV;
}
*/

slib::sbio::Variant::Variant() { clear(); }
slib::sbio::Variant::Variant(const SVar& var) : Variant() {
	type = var.type & 0xFF;
	alt = var.alt;
	read[0] = var.read[0];
	read[1] = var.read[1];
	qual = sbio::sutil::phredVal(var.qual);
}
slib::sbio::Variant::Variant(const SVar& var1, const SVar& var2, int rev) : Variant() {
	alt = var1.alt + "/" + var2.alt;
	
	switch (rev) {
	case 0:
	{
		read[0] = var1.read[0] + var2.read[0];
		read[1] = var1.read[1] + var2.read[1];
		break;
	}
	case 1:
	{
		read[0] = var1.read[0] + var2.read[1];
		read[1] = var1.read[1] + var2.read[0];
		break;
	}
	case 2:
	{
		read[0] = var1.read[1] + var2.read[0];
		read[1] = var1.read[0] + var2.read[1];
		break;
	}
	default:
		break;
	}
	qual = sbio::sutil::phredVal(1.0 - ((1.0 - var1.qual) * (1.0 - var2.qual)));
}
slib::sbio::Variant::Variant(const Variant& var) :Variant() {
	type = var.type; 
	flag = var.flag;
	genotype = var.genotype;
	varid = var.varid;
	alt = var.alt;
	frequency = var.frequency;
	qual = var.qual;
	gqual = var.gqual;
	sforin(i, 0, 2) {
		pos[i] = var.pos[i];
		read[i] = var.read[i];
		copy[i] = var.copy[i];
		sforin(j, 0, 2) depth[i][j] = var.depth[i][j];
 	}
	annotation = var.annotation;
	attribute = var.attribute;
}
slib::sbio::Variant::Variant(const slib::sbio::GffData& gff, slib::sbio::SeqList* ref) : Variant() {
	type = sbio::sutil::varType(gff.type);
	pos[0] = gff;
	qual = gff.score;
	attribute = gff.attribute;
}
slib::sbio::Variant::Variant(const SDictionary& obj) : Variant() { set(obj); }
slib::sbio::Variant::~Variant() {}
slib::sbio::Variant& slib::sbio::Variant::operator=(const slib::sbio::SVar& var) {
	/*
	type = var.type; alt = var.alt; qual = var.qual;
	memcpy(pos, var.pos, sizeof(sbpos) * 2); memcpy(read, var.read, sizeof(sushort) * 2);
	*/
	return *this;
}
slib::sbio::Variant& slib::sbio::Variant::operator=(const slib::sbio::Variant& var) {
	type = var.type;
	flag = var.flag;
	genotype = var.genotype;
	varid = var.varid;
	alt = var.alt;
	frequency = var.frequency;
	qual = var.qual;
	gqual = var.gqual;
	sforin(i, 0, 2) {
		pos[i] = var.pos[i];
		read[i] = var.read[i];
		copy[i] = var.copy[i];
		sforin(j, 0, 2) depth[i][j] = var.depth[i][j];
	}
	annotation = var.annotation;
	attribute = var.attribute;
	return *this;
}
bool slib::sbio::Variant::comparable(const slib::sbio::Variant& var) const {
	return type == var.type &&
		pos[0].idx == var.pos[0].idx && pos[1].idx == var.pos[1].idx &&
		pos[0].dir == var.pos[0].dir && pos[1].dir == var.pos[1].dir;
}
bool slib::sbio::Variant::lt(const slib::sbio::Variant& var, slib::sbio::VarParam* par) const {
	if (pos[0].idx != var.pos[0].idx) return pos[0].idx < var.pos[0].idx;
	if (pos[0].dir != var.pos[0].dir) return pos[0].dir < var.pos[0].dir;
	if (pos[0].begin != var.pos[0].begin) return pos[0].begin < var.pos[0].begin;
	if (pos[0].end != var.pos[0].end) return pos[0].end < var.pos[0].end;
	if (pos[1].idx != var.pos[1].idx) return pos[1].idx < var.pos[1].idx;
	if (pos[1].dir != var.pos[1].dir) return pos[1].dir < var.pos[1].dir;
	if (pos[1].begin != var.pos[1].begin) return pos[1].begin < var.pos[1].begin;
	if (pos[1].end != var.pos[1].end) return pos[1].end < var.pos[1].end;
	return false;
}
bool slib::sbio::Variant::equal(const slib::sbio::Variant& var, slib::sbio::SeqList* ref, slib::sbio::VarParam* par) const {
	if (!comparable(var)) return false;
	if ((flag & SMALL_VARIANT) && (var.flag & SMALL_VARIANT)) 
		return pos[0] == var.pos[0] && alt == var.alt;
	else {

		return pos[0].begin == var.pos[0].begin && pos[0].end == var.pos[0].end && alt == var.alt;
	}
}
void slib::sbio::Variant::set(const SDictionary& obj) {
	varid = obj["vid"];
	type = obj["type"];
	genotype = obj["gtype"];
	flag = obj["flag"];
	pos[0] = sbpos(obj["pos"][0][0], obj["pos"][0][1], obj["pos"][0][2], obj["pos"][0][3]);
	pos[1] = sbpos(obj["pos"][1][0], obj["pos"][1][1], obj["pos"][1][2], obj["pos"][1][3]);
	read[0] = obj["read"][0]; 
	read[1] = obj["read"][1];
	alt = obj["alt"];
	copy[0] = obj["copy"][0];
	copy[1] = obj["copy"][1];
	depth[0][0] = obj["depth"][0];
	depth[0][1] = obj["depth"][1];
	depth[1][0] = obj["depth"][2];
	depth[1][1] = obj["depth"][3];
	frequency = obj["freq"];
	qual = obj["qual"];
	gqual = obj["gqual"];
	const SArray &annotInfo = obj["annotation"];
	annotation.resize(annotInfo.size());
	sfor2(annotation, annotInfo) {
		$_1.type = (GENE_TYPE)$_2["type"].intValue();
		$_1.gid = $_2["gid"];
		$_1.name = $_2["name"];
		sforeach(transcript, $_2["transcripts"]) {
			TranscriptAnnotData tdat;
			tdat.type = (TRANSCRIPT_TYPE)transcript["type"].intValue();
			tdat.pos = srange(transcript["pos"][0], transcript["pos"][1]);
			tdat.name = transcript["name"];
			tdat.site = transcript["site"];
			tdat.mutation = transcript["mutation"];
			tdat.score = transcript["score"];
			tdat.effect = (VARIANT_EFFECT)transcript["effect"].intValue();
			const SDictionary& subst = transcript["substitution"];
			tdat.substitution.prefix = subst["prefix"].empty() ? '\0' : subst["prefix"].toString()[0];
			tdat.substitution.pos = srange(subst["pos"][0], subst["pos"][1]);
			tdat.substitution.ori = subst["ori"];
			tdat.substitution.alt = subst["alt"];
			$_1.transcripts.add(tdat);
		}
	}
	attribute = obj["attribute"];
}
void slib::sbio::Variant::clear() {
	type = 0;
	flag = 0;
	genotype = 0;
	pos[0].init();
	pos[1].init();

	varid.clear();
	alt.clear();

	read[0] = 0;
	read[1] = 0;

	depth[0][0] = 0.f;
	depth[0][1] = 0.f;
	depth[1][0] = 0.f;
	depth[1][1] = 0.f;

	copy[0] = 0.f;
	copy[1] = 0.f;
	frequency = 0.f;
	qual = 0.f;
	gqual = 0.f;

	attribute.clear();
}
slib::String slib::sbio::Variant::toString(const char* form) const {
	String str, f(form);
	if (flag & NOT_USE_FLAG || flag & UNAVAILABLE_FLAG) str << "<NG>";
	else str << varid;
	str << TAB << sbio::sutil::varTypeStr(type) << TAB;
	if (flag & SMALL_VARIANT) {
		switch (type) {
		case sbio::SNV:
			str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end << TAB <<
				(attribute.hasKey("_ref_") ? attribute["_ref_"] : "N") << "/" << alt;
			break;
		case sbio::MNV:
			str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end << TAB <<
				(attribute.hasKey("_ref_") ? attribute["_ref_"].string() : S("N") * alt.size()) << "/" << alt;
			break;
		case sbio::DELETION:
			str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end << TAB <<
				(attribute.hasKey("_ref_") ? attribute["_ref_"].string() : S("N") * alt.size()) << "/-";
			break;
		case sbio::DUPLICATION:
			str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end << TAB << "-/" << alt;
			break;
		case sbio::INSERTION:
			str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end << TAB << "-/" << alt;
			break;
		default:
			break;
		}
	}
	else if (flag & CN_VARIANT) str << pos[0].idx << ":" << pos[0].begin << "-" << pos[0].end;
	else {
		if (type & INSERTION) {
			if (pos[1].idx == -1) str << pos[0].idx << ":" << pos[0].begin << "-" << alt << "-" << pos[0].end;
			else {
				str << pos[0].idx << ":" << pos[0].begin << "-[" <<
					pos[1].idx << ":" << pos[1].begin << "-" << pos[1].end << "(" << (pos[1].dir ? "-" : "+") <<
					")]-" << pos[0].end;
			}
		}
		else if (pos[1].idx != -1) {
			if (type & TRANSLOCATION) {
				if (type & INVERSION) {
					str << pos[0].idx << ":" << pos[0].begin << "=" << pos[1].idx << ":" << pos[1].begin << "(-)" << NL <<
						pos[1].idx << ":" << pos[1].end << "(-)=" << pos[0].idx << ":" << pos[0].end;
				}
				else {
					str << pos[0].idx << ":" << pos[0].begin << "=" << pos[1].idx << ":" << pos[1].end << NL <<
						pos[1].idx << ":" << pos[1].begin << "=" << pos[0].idx << ":" << pos[0].end;
				}
			}
			else str << pos[0].idx << ":" << pos[0].begin << "<" << pos[1].end << "-" << pos[1].begin << ">" << pos[0].end;
		}
		else str << pos[0].idx << ":" << pos[0].begin << "-" << (alt.size() ? alt + "-" : "") << pos[0].end;
	}
	return str;
}
slib::SObjPtr slib::sbio::Variant::toObj() const {
	sobj annotInfo = SArray();
	sforeach(gene, annotation) {
		sobj ginfo = {
			D_("type", (int)gene.type),
			D_("gid", gene.gid),
			D_("name", gene.name),
			D_("transcripts", SArray())
		};
		SArray& transcripts = ginfo["transcripts"];
		sforeach(transcript, gene.transcripts) {
			sobj tinfo = {
				D_("type", (int)transcript.type),
				D_("pos", sobj({transcript.pos.begin, transcript.pos.end})),
				D_("name", transcript.name),
				D_("site", transcript.site),
				D_("mutation", transcript.mutation),
				D_("score", transcript.score),
				D_("effect", (int)transcript.effect),
				D_("substitution", sobj({
					D_("prefix", S({transcript.substitution.prefix})),
					D_("pos", sobj({transcript.substitution.pos.begin, transcript.substitution.pos.end})),
					D_("ori", transcript.substitution.ori),
					D_("alt", transcript.substitution.alt)
					}))
			};
			transcripts.add(tinfo);
		}
		annotInfo.add(ginfo);
	}
	sobj obj = SDictionary({
		D_("vid", varid),
		D_("type", type),
		D_("gtype", genotype),
		D_("flag", flag),
		D_("pos", sobj({
			sobj({ pos[0].idx, pos[0].begin, pos[0].end, pos[0].dir }),
			sobj({ pos[1].idx, pos[1].begin, pos[1].end, pos[1].dir })
			})),
		D_("read", sobj({read[0], read[1]})),
		D_("alt", alt),
		D_("depth", sobj({depth[0][0], depth[0][1], depth[1][0], depth[1][1]})),
		D_("copy", sobj({copy[0], copy[1]})),
		D_("freq", frequency),
		D_("qual", qual),
		D_("gqual", gqual),
		D_("annotation", annotInfo),
		D_("attribute", attribute),
		});
	return obj;
}
bool slib::sbio::Variant::operator <(const Variant& var) const {
	if (pos[0].idx != var.pos[0].idx) return pos[0].idx < var.pos[0].idx;
	if (pos[0].dir != var.pos[0].dir) return pos[0].dir < var.pos[0].dir;
	if (pos[1].idx != var.pos[1].idx) return pos[1].idx < var.pos[1].idx;
	if (pos[1].dir != var.pos[1].dir) return pos[1].dir < var.pos[1].dir;
	if (flag & SMALL_VARIANT) {
		if (pos[0].begin != var.pos[0].begin) return pos[0].begin < var.pos[0].begin;
		if (pos[0].end != var.pos[0].end) return pos[0].end < var.pos[0].end;
		if (pos[1].begin != var.pos[1].begin) return pos[1].begin < var.pos[1].begin;
		if (pos[1].end != var.pos[1].end) return pos[1].end < var.pos[1].end;
	}
	if (type != var.type) return type < var.type;
	return false;
}
bool slib::sbio::Variant::operator ==(const Variant& var) const {
	return type == var.type && pos[0] == var.pos[0] && pos[1] == var.pos[1] && alt == var.alt;
}
/*
bool slib::sbio::Variant::comparable(const SVariant *var) const {
    return pos[0].idx == var->pos[0].idx && pos[0].dir == var->pos[0].dir &&
    pos[1].idx == var->pos[1].idx && pos[1].dir == var->pos[1].dir;
}
bool SVariant::lt(const SVariant *var, size_t dist) const {
    if (flag == SMALL_VARIANT) return svar_data::lt(*var, 0);
    return svar_data::lt(*var, dist);
}
bool SVariant::equal(const SVariant *var, size_t dist) const {
	if (flag & SMALL_VARIANT) return *this == *var;
    return svar_data::equal(*var, dist);
}
sushort slib::sbio::Variant::annotatedSite() const {
	if (genes.empty()) return 0;
	sushort site = 0;
	sforeach(genes) {
		sforeach_(tit, E_.transcripts) site |= tit->site;
	}
	return site;
}



*/