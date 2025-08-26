#include "sutil/sjson.h"
#include "sbioinfo/variant.h"
#include "sbioinfo/vcf.h"
slib::sbio::VCParam::VCParam() {
	sforin(i, 0, 3) {
		min_depth[i] = DEFAULT_VC_DP;
		min_vdepth[i] = DEFAULT_VC_VDP;
		min_freq[i] = DEFAULT_VC_FREQ;
		homo_freq[i] = DEFAULT_VC_HOMO_FREQ;
		min_qual[i] = DEFAULT_VC_QUAL;
	}
}
slib::sbio::VCParam::VCParam(const VCParam& par) {
	sforin(i, 0, 3) {
		min_depth[i] = par.min_depth[i];
		min_vdepth[i] = par.min_vdepth[i];
		min_freq[i] = par.min_freq[i];
		homo_freq[i] = par.homo_freq[i];
		min_qual[i] = par.min_qual[i];
	}
}
slib::sbio::VCParam::~VCParam() {}
slib::sbio::VCParam& slib::sbio::VCParam::operator=(const slib::sbio::VCParam& par) {
	sforin(i, 0, 3) {
		min_depth[i] = par.min_depth[i];
		min_vdepth[i] = par.min_vdepth[i];
		min_freq[i] = par.min_freq[i];
		homo_freq[i] = par.homo_freq[i];
		min_qual[i] = par.min_qual[i];
	}
	return *this;
}
void slib::sbio::VCParam::set(const sobj& obj) {
	if (obj["snv"]) {
		auto snvdic = obj["snv"];
		if (!snvdic["dp"].isNull()) min_depth[0] = snvdic["dp"];
		if (!snvdic["vdp"].isNull()) min_vdepth[0] = snvdic["vdp"];
		if (!snvdic["freq"].isNull()) min_freq[0] = snvdic["freq"];
		if (!snvdic["border"].isNull()) homo_freq[0] = snvdic["border"];
		if (!snvdic["qual"].isNull()) min_qual[0] = snvdic["qual"];
	}
	if (obj["del"]) {
		auto deldic = obj["del"];
		if (!deldic["dp"].isNull()) min_depth[1] = deldic["dp"];
		if (!deldic["vdp"].isNull()) min_vdepth[1] = deldic["vdp"];
		if (!deldic["freq"].isNull()) min_freq[1] = deldic["freq"];
		if (!deldic["border"].isNull()) homo_freq[1] = deldic["border"];
		if (!deldic["qual"].isNull()) min_qual[1] = deldic["qual"];
	}
	if (obj["ins"]) {
		auto insdic = obj["ins"];
		if (!insdic["dp"].isNull()) min_depth[2] = insdic["dp"];
		if (!insdic["vdp"].isNull()) min_vdepth[2] = insdic["vdp"];
		if (!insdic["freq"].isNull()) min_freq[2] = insdic["freq"];
		if (!insdic["border"].isNull()) homo_freq[2] = insdic["border"];
		if (!insdic["qual"].isNull()) min_qual[2] = insdic["qual"];
	}
}
sobj slib::sbio::VCParam::toObj() {
	auto snvdic = { 
		D_("dp", min_depth[0]), 
		D_("vdp", min_vdepth[0]), 
		D_("freq", min_freq[0]),
		D_("border", homo_freq[0]), 
		D_("dp", min_qual[0]) 
	};
	auto deldic = { 
		D_("dp", min_depth[1]), 
		D_("vdp", min_vdepth[1]), 
		D_("freq", min_freq[1]),
		D_("border", homo_freq[1]), 
		D_("dp", min_qual[1]) 
	};
	auto insdic = { 
		D_("dp", min_depth[2]), 
		D_("vdp", min_vdepth[2]), 
		D_("freq", min_freq[2]),
		D_("border", homo_freq[2]), 
		D_("dp", min_qual[2]) 
	};
	return { D_("snv", snvdic), D_("del", deldic), D_("ins", insdic) };
}

slib::sbio::CNVParam::CNVParam() {
	method = CN_METHOD::RAW;

	min_length = DEFAULT_MIN_CVLEN;
	min_bg = DEFAULT_BG_DEPTH;
	border[0] = DEFAULT_HOMO_DEL;
	border[1] = DEFAULT_DELCP;
	border[2] = DEFAULT_DUPCP;
	border[3] = DEFAULT_HOMO_DUPCP;
	border[4] = DEFAULT_MULCP;
	min_qual = DEFAULT_MIN_QUAL;
	emission = smatd(6, 6, 
		{
			1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 0.0, 1.0
		});
	transition = smatd(6, 6,
		{
			0.5, 0.0, 0.5, 0.0, 0.0, 0.0,
			0.0, 0.5, 0.5, 0.0, 0.0, 0.0,
			1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0, 1.0 / 6.0,
			0.0, 0.0, 0.5, 0.5, 0.0, 0.0,
			0.0, 0.0, 0.5, 0.0, 0.5, 0.0,
			0.0, 0.0, 0.5, 0.0, 0.0, 0.5
		});
}
slib::sbio::CNVParam::CNVParam(const slib::sbio::CNVParam& par) {
	method = par.method;


	min_length = par.min_length;
	min_bg = par.min_bg;
	memcpy(border, par.border, sizeof(double) * 5);
	min_qual = par.min_qual;
	emission = par.emission;
	transition = par.transition;
}
slib::sbio::CNVParam::~CNVParam() {}
slib::sbio::CNVParam& slib::sbio::CNVParam::operator=(const slib::sbio::CNVParam& par) {
	method = par.method;

	use_ml = false;
	gc_correct = false;

	min_bg = par.min_bg;

	border[0] = 0.25f;
	border[1] = 0.75f;
	border[2] = 1.5f;
	border[3] = 1.75f;
	border[4] = 3.5f;


	min_length = par.min_length;
	min_qual = par.min_qual;
	//emission = par.emission;
	//transition = par.transition;
	return *this;
}
slib::sushort slib::sbio::CNVParam::evaluate(float cp) {
	if (use_ml) {

		return 0;
	}
	else {
		if (cp < border[0]) return (HOMO_VAR << 8) | DELETION;
		else if (cp < border[1]) return (HETERO_VAR << 8) | DELETION;
		else if (border[4] < cp) return (HOMO_VAR << 8) | MULTIPLICATION;
		else if (border[3] < cp) return (HOMO_VAR << 8) | DUPLICATION;
		else if (border[2] < cp) return (HETERO_VAR << 8) | DUPLICATION;
		else return 0;
	}
}
void slib::sbio::CNVParam::set(const sobj& obj) {
	if (obj["method"]) method = (CN_METHOD)(obj["method"].intValue());


	if (obj["len"]) min_length = obj["len"];
	if (obj["bg"]) min_bg = obj["bg"];
	if (obj["border"]) {
		auto& array = obj["border"].array();
		sforin(i, 0, 5) border[i] = array[i];
	}
	if (obj["qual"]) min_qual = obj["qual"];
	/*
	if (obj["emit"]) {
		auto pit = obj["emit"].array().begin();
		sfor(emission) { $_ = *pit; ++pit; }
	}
	if (obj["trans"]) {
		auto pit = obj["trans"].array().begin();
		sfor(transition) { $_ = *pit; ++pit; }
	}
	*/
}
sobj slib::sbio::CNVParam::toObj() {


	sobj cnvborder, cnvprob, cnvtrans;
	sforin(i, 0, 5) cnvborder.add(border[i]);
	sfor(emission) cnvprob.add($_);
	sfor(transition) cnvtrans.add($_);

	return { 
		D_("method", (int)method),

		D_("len", min_length), D_("bg", min_bg), D_("border", cnvborder),
		D_("qual", min_qual)
		//D_("emit", cnvprob), 
		//D_("trans", cnvtrans)
	};
}

slib::sbio::SVParam::SVParam() {
	min_read = DEFAULT_SR_READ;
	comp_min_read = 2 * DEFAULT_SR_READ;

	sforin(i, 0, 3) size_range[i] = srange(1, 0x7FFFFFFF);

	min_length[0] = DEFAULT_MIN_DEL_SIZE;
	min_length[1] = DEFAULT_MIN_INS_SIZE;
	min_length[2] = DEFAULT_MIN_INV_SIZE;
	freq_bin = DEFAULT_FREQ_BIN;
	min_freq = DEFAULT_MIN_FREQ;
	
	max_dist = 5;
	break_site_len = 20;
	max_gap = 200;

	read_bias = DEFAULT_FR_BIAS;
	comb_bias = DEFAULT_COMP_BIAS;

	homo_freq = DEFAULT_HOMO_FREQ;

	min_qual = 1.0;
}
slib::sbio::SVParam::SVParam(const SVParam& par) {
	min_read = par.min_read;
	comp_min_read = par.comp_min_read;

	memcpy(min_length, par.min_length, sizeof(sint) * 3);
	freq_bin = par.freq_bin;
	min_freq = par.min_freq;

	max_dist = par.max_dist;
	break_site_len = par.break_site_len;
	max_gap = par.max_gap;


	read_bias = par.read_bias;
	comb_bias = par.comb_bias;
	homo_freq = par.homo_freq;
	min_qual = par.min_qual;
}
slib::sbio::SVParam::~SVParam() {}
slib::sbio::SVParam& slib::sbio::SVParam::operator=(const SVParam& par) {
	min_read = par.min_read;
	comp_min_read = par.comp_min_read;
	
	memcpy(min_length, par.min_length, sizeof(sint) * 3);
	freq_bin = par.freq_bin;
	min_freq = par.min_freq;
	read_bias = par.read_bias;
	comb_bias = par.comb_bias;
	homo_freq = par.homo_freq;
	min_qual = par.min_qual;
	return *this;
}
void slib::sbio::SVParam::set(const sobj& obj) {
	if (obj.hasKey("read")) min_read = obj["read"];
	if (obj.hasKey("cread")) comp_min_read = obj["cread"];


	if (obj["bin"]) freq_bin = obj["bin"];
	if (obj["freq"]) min_freq = obj["freq"];
	if (obj["fbias"]) read_bias = obj["fbias"];
	if (obj["cbias"]) comb_bias = obj["cbias"];
	if (obj["homo"]) homo_freq = obj["homo"];
	if (obj["qual"]) min_qual = obj["qual"];
}
sobj slib::sbio::SVParam::toObj() {
	return {
		D_("read", min_read),
		D_("cread", comp_min_read),
		D_("length", sobj({ min_length[0], min_length[1], min_length[2] })),
		D_("freq", min_freq), D_("fbias", read_bias), D_("cbias", comb_bias),
		D_("bin", freq_bin), D_("homo", homo_freq), D_("qual", min_qual)
	};
}

slib::sbio::VarParam::VarParam() {
	homo_select = false;
	annot = 0x01FF;
	splice_reg = 2;
	codon = DEFAULT_CODON;
	mtcodon = DEFAULT_MT_CODON;
}
slib::sbio::VarParam::VarParam(const VarParam& par) {
	homo_select = par.homo_select;
	annot = par.annot;
}
slib::sbio::VarParam::~VarParam() {}
slib::sbio::VarParam& slib::sbio::VarParam::operator=(const slib::sbio::VarParam& par) {
	homo_select = par.homo_select;
	annot = par.annot;
	return *this;
}
void slib::sbio::VarParam::set(const sobj& obj) {
	if (obj["homo"]) homo_select = obj["homo"];
	if (obj["annot"]) annot = obj["annot"];
	if (obj["cnvpar"]) cnvp.set(obj["cnvpar"]);
	if (obj["svpar"]) svp.set(obj["svpar"]);
	if (obj["vcpar"]) vcp.set(obj["vcpar"]);
}
sobj slib::sbio::VarParam::toObj() {
	return {
		D_("homo", homo_select), D_("annot", annot), 
		D_("cnvpar", cnvp.toObj()), D_("svpar", svp.toObj()), D_("vcpar", vcp.toObj())
	};
}
float slib::sbio::sutil::readBias(const sushort *r) {
	return abs((float)(r[0] - r[1]) / (r[0] + r[1]));
}
float slib::sbio::sutil::combBias(sushort *r1, sushort *r2) {
	return abs((float)((r1[0] + r1[1]) - (r2[0] + r2[1])) / (r1[0] + r1[1] + r2[0] + r2[1]));
}
/*
bool slib::sbio::sutil::checkSV(slib::sbio::SVar* var, SVParam* par) {
	if (par->read_bias < var->bias() || 
		sbio::sutil::phredVal(var->qual) < par->min_qual) return false;
	switch (var->type)
	{
	case DELETION:
	{
		return par->min_read[0] <= var->total() &&
			par->min_length[0] <= (var->pos[1].begin - var->pos[0].end - 1);
	}
	case DUPLICATION:
	{
		return par->min_read[1] <= var->total() &&
			par->min_length[1] <= (var->pos[0].end - var->pos[1].begin - 1);
	}
	case INSERTION:
	{
		return par->min_read[2] <= var->total() &&
			par->min_length[2] <= var->alt.size();
	}
	default:
		return false;
	}
}
bool slib::sbio::sutil::checkSV(SVar* var1, SVar* var2, SVParam* par) {
	if (par->read_bias < var1->bias() || 
		par->read_bias < var2->bias() ||
		par->comb_bias < sbio::sutil::combBias(var1->read, var2->read)) return false;
	switch (var1->type)
	{
	case DELETION:
		if (var1->total() < par->min_sr[0] || var2->total() < par->min_sr[1] ||
			var1->total() + var2->total() < par->min_comp_sr[0]) return false;
		return var1->pos[1].begin < var2->pos[0].begin&& var1->pos[1].begin < var2->pos[0].begin&&
			var1->pos[0].begin - par->min_length[0] < var2->pos[1].begin;
	case DUPLICATION:
		if (var1->total() < par->min_sr[1] || var2->total() < par->min_sr[0] ||
			var1->total() + var2->total() < par->min_comp_sr[0]) return false;
		return var1->pos[1].begin < var2->pos[0].begin&& var2->pos[0].begin < var1->pos[0].begin&&
			var1->pos[0].begin - par->min_length[1] < var2->pos[1].end;
	case INVERSION:
		break;

	default:
		break;
	}
}
*/
slib::sushort slib::sbio::sutil::varType(const char* s, const char *sep) {
	slib::sushort type;
	auto strs = sstr::toLower(s).split(sep);
	sfor(strs) {
		if ($_ == "snv") type |= slib::sbio::SNV;
		else if ($_ == "mnv") type |= slib::sbio::MNV;
		else if ($_.match("ins")) type |= slib::sbio::INSERTION;
		else if ($_.match("del")) type |= slib::sbio::DELETION;
		else if ($_.match("dup")) type |= slib::sbio::DUPLICATION;
		else if ($_.match("mul")) type |= slib::sbio::MULTIPLICATION;
		else if ($_.match("inv")) type |= slib::sbio::INVERSION;
		else if ($_.match("tr")) type |= slib::sbio::TRANSLOCATION;
	}
	return type;
}
slib::String slib::sbio::sutil::varTypeStr(const sushort t, const char *sep) {
	slib::String str;
	if (t & slib::sbio::SNV) str << "SNV" << sep;
	if (t & slib::sbio::MNV) str << "MNV" << sep;
	if (t & slib::sbio::INSERTION) str << "INS" << sep;
	if (t & slib::sbio::DELETION) str << "DEL" << sep;
	if (t & slib::sbio::DUPLICATION) str << "DUP" << sep;
	if (t & slib::sbio::MULTIPLICATION) str << "MUL" << sep;
	if (t & slib::sbio::INVERSION) str << "INV" << sep;
	if (t & slib::sbio::TRANSLOCATION) str << "TRS" << sep;
	if (str.size()) str.resize(str.size() - strlen(sep));
	return str;
}
slib::String slib::sbio::sutil::varTypeDesc(const sushort type) {
	auto t = type & 0xFF;
	if (t & slib::sbio::SNV) return "snv";
	else if (t & slib::sbio::MNV) return "mnv";
	else if (t & slib::sbio::INSERTION) {

		if (t == slib::sbio::INSERTION) return "insertion";
		else if (t & slib::sbio::DELETION) {

			return "complex deletion and insertion";
		}
		else if (t & slib::sbio::TRANSLOCATION) {
			return "translocational inversion";
		}

		String str("insertion");
		if (t != slib::sbio::INSERTION) {
			if (t & slib::sbio::INVERSION) str << " of an inverted fragment";
			else str << " of a fragment";
			if (t & slib::sbio::TRANSLOCATION) str << " from a different chromosome";
			else str << " from a distant site on the same chromosome";
			if (t & slib::sbio::DELETION) str << " into a deleted region";
		}
		return str;
	}
	else if (t & slib::sbio::TRANSLOCATION) {
		if (t & slib::sbio::INVERSION) {
			if (t & slib::sbio::DELETION) return "inverted translocation and deletion";
			else return "inverted translocation";
		}
		else {
			if (t & slib::sbio::DELETION) return "translocation and deletion";
			else return "translocation";
		}
	}
	else if (t & slib::sbio::INVERSION) {
		if (t & slib::sbio::DELETION) return "inversion and deletion";
		else return "inversion";
	}
	else if (t & slib::sbio::DELETION) return "deletion";
	else if (t & slib::sbio::DUPLICATION) return "duplication";
	else if (t & slib::sbio::MULTIPLICATION) return "multiplication";
	return "no variant";
}

slib::sbio::SVREAD_TYPE slib::sbio::sutil::toSVIndex(const sushort type) {
	auto t = type & 0xFF;
	if (t & INSERTION) return slib::sbio::SVREAD_TYPE::INS;
	if (t == DELETION) return slib::sbio::SVREAD_TYPE::DEL;
	else if (t == DUPLICATION) return slib::sbio::SVREAD_TYPE::DUP;
	else if (t == INVERSION) return slib::sbio::SVREAD_TYPE::INV;
	else if (t == TRANSLOCATION) return slib::sbio::SVREAD_TYPE::TRS;
	else return slib::sbio::SVREAD_TYPE::TRINV;
}

slib::Range<int> slib::sbio::sutil::transcriptSite(const srange& rng, TranscriptInfo* trs, bool rev) {
	int len = 0;
	srange trange;
	auto exons = trs->exons();
	if (rev) {
		srfor(exons) {
			if (rng.end < $_.begin) trange.begin = len + $_.length(true);
			if ($_.end < rng.begin) trange.end = len;
			if ($_.include(rng.end)) trange.begin = len + $_.end - rng.end + 1;
			if ($_.include(rng.begin)) trange.end = len + $_.end - rng.begin + 1;
			len += $_.length(true);
		}
		if (rng.end < exons[0].begin) { trange = srange(-1, -1); }
		else if (rng.begin < exons[0].begin) trange.end = len;

	}
	else {
		sfor(exons) {
			if ($_.end < rng.begin) trange.begin = len + $_.length(true);
			if (rng.end < $_.begin) trange.end = len;
			if ($_.include(rng.begin)) trange.begin = len + rng.begin - $_.begin + 1;
			if ($_.include(rng.end)) trange.end = len + rng.end - $_.begin + 1;
			len += $_.length(true);
		}
		if (exons[-1].end < rng.begin) { trange = srange(-1, -1); }
		else if (exons[-1].end < rng.end) trange.end = len;
	}
	return trange;
}
slib::Range<int> slib::sbio::sutil::codingSite(const srange& rng, TranscriptInfo* trs, bool rev) {
	int len = 0;
	srange crange;
	auto cds = trs->coding();
	if (rev) {
		srfor(cds) {
			if (rng.end < $_.begin) crange.begin = len + $_.length(true);
			if ($_.end < rng.begin) crange.end = len;
			if ($_.include(rng.end)) crange.begin = len + $_.end - rng.end + 1;
			if ($_.include(rng.begin)) crange.end = len + $_.end - rng.begin + 1;
			len += $_.length(true);
		}
		if (rng.end < cds[0].begin) { crange = srange(-1, -1); }
		else if (rng.begin < cds[0].begin) crange.end = len;

	}
	else {
		sfor(cds) {
			if ($_.end < rng.begin) crange.begin = len + $_.length(true);
			if (rng.end < $_.begin) crange.end = len;
			if ($_.include(rng.begin)) crange.begin = len + rng.begin - $_.begin + 1;
			if ($_.include(rng.end)) crange.end = len + rng.end - $_.begin + 1;
			len += $_.length(true);
		}
		if (cds[-1].end < rng.begin) { crange = srange(-1, -1); }
		else if (cds[-1].end < rng.end) crange.end = len;
	}

	SPrint(crange.begin, " | ", crange.end);

	return crange;
}
slib::Range<slib::Pair<int, int>> slib::sbio::sutil::proteinSite(const srange& rng, TranscriptInfo* trs, bool rev) {
	auto crange = codingSite(rng, trs, rev);
	return slib::Range<slib::Pair<int, int>>(slib::Pair<int, int>(crange.begin / 3, crange.begin % 3), slib::Pair<int, int>(crange.end / 3, crange.end % 3));
}

void slib::sbio::sio::writeRefInfo(const slib::Array<slib::Pair<slib::String, int>>& ref, slib::IOStream& strm) {
	sfor(ref) {
		strm << "#LG" << slib::TAB << $_.first << slib::TAB << $_.second << " bp" << slib::NL;
	}
	strm.flush();
}
void slib::sbio::sio::writeHead(const stringarray& cols, slib::IOStream& strm) {
	strm << toString(cols, slib::TAB) << NL; strm.flush();
}
void slib::sbio::sio::writeVariant(const slib::sbio::Variant& var, const stringarray &cols, const slib::Array<slib::Pair<slib::String, int>>& ref, slib::IOStream& strm) {
	sfor(cols) {
		auto endl = ($ == (cols.end() - 1));
		if ($_ == "VarID" || $_ == "ID")  strm << var.varid << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Sample")  strm << (var.attribute.hasKey("sample") ? var.attribute["sample"] : "") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Tag")  strm << sbio::sutil::varTypeStr(var.type) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Type")  strm << sbio::sutil::varTypeDesc(var.type) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Category") {
			if (var.flag & SMALL_VARIANT) strm << "VC" << (endl ? slib::LF : slib::TAB);
			else if (var.flag & CN_VARIANT) strm << "CNV" << (endl ? slib::LF : slib::TAB);
			else if (var.flag & SR_VARIANT) strm << "SV" << (endl ? slib::LF : slib::TAB);
			else strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Chr" || $_ == "Chr1") strm << ref[var.pos[0].idx].first << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Chr2") strm << (var.pos[1].idx == -1 ? "-" : ref[var.pos[1].idx].first) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Pos" || $_ == "Pos1") {
			// DEL, DEL+INS+(INV|TRS)
			// SNV|MNV, DUP, INS, INV, TRS(+INV)
			if (var.type & DELETION) strm << S(var.pos[0].begin + 1) << (endl ? slib::LF : slib::TAB);
			else strm << S(var.pos[0].begin) << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Pos2") strm << (var.pos[1].idx == -1 ? "-" : S(var.pos[1].begin)) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Len" || $_ == "Len1") {
			// DEL, DEL+INS+(INV|TRS)	...del length
			// INS | INS+(INV|TRS)		...insertion size or "-"
			// INV, TRS(+INV)			...gap size
			// SNV|MNV, DUP				...length+1
			if (var.type & DELETION) strm << S(var.pos[0].length(true)) << (endl ? slib::LF : slib::TAB);
			else if (var.type & INSERTION) strm << (var.pos[1].idx == -1 ? S(var.alt.size()) : "-") << (endl ? slib::LF : slib::TAB);
			else if (-1 < var.pos[1].idx) strm << S(var.pos[0].length() - 1) << (endl ? slib::LF : slib::TAB);
			else strm << S(var.pos[0].length(true)) << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Len2") {
			if (var.pos[1].idx == -1) strm << "-" << (endl ? slib::LF : slib::TAB);
			else {
				if (var.type & INSERTION) strm << (var.pos[1].idx == -1 ? "-" : S(var.pos[1].length(true))) << (endl ? slib::LF : slib::TAB);
				else strm << (var.pos[1].idx == -1 ? "-" : S(var.pos[1].length() - 1)) << (endl ? slib::LF : slib::TAB);
			}
		}
		else if ($_ == "Ref")  strm << (var.attribute.hasKey("_ref_") ? var.attribute["_ref_"] : "-") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Alt") strm << (var.flag & SMALL_VARIANT ? var.alt : "-") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Ins") strm << (var.flag & SR_VARIANT ? var.alt : "-") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Alt/Ins") strm << var.alt << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Qual")  strm << slib::numToString(var.qual, "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Freq")  strm << slib::numToString(var.frequency, "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Genotype") strm << (var.genotype & sbio::HOMO_VAR ? "Homo": "Hetero") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Dp" || $_ == "Dp1" || $_ == "Cov" || $_ == "Cov1")  strm << slib::numToString(var.depth[0][0], "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Dp2" || $_ == "Cov2")  strm << (var.pos[1].idx == -1 ? "-" : slib::numToString(var.depth[1][0], "%.2f")) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "ADp" || $_ == "Allele Cov")  strm << S((int)var.read[0] + (int)var.read[1]) << (endl ? slib::LF : slib::TAB);
		else if ($_ == "CDp" || $_ == "Control Cov")  strm << slib::numToString(var.depth[0][1], "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "VarRead")  strm << (int)var.read[0] << "|" << (int)var.read[1] << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Copy" || $_ == "Copy1")  strm << slib::numToString(var.copy[0], "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Copy2")  strm << slib::numToString(var.copy[1], "%.2f") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Gene" || $_ == "GeneSymbol") {
			writeAnnotGenes(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "GeneID") {
			writeAnnotGeneIDs(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "GeneType") {
			writeAnnotGeneTypes(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Site") {
			writeVariantSites(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Mutation") {
			writeMutationType(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Substitution") {
			writeSubstitution(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Mutant") 
			strm << (var.attribute.hasKey("mutant") ? var.attribute["mutant"].toString("csv") : "") << (endl ? slib::LF : slib::TAB);
		else if ($_ == "Effect") {
			writeVarEffect(var, strm); strm << (endl ? slib::LF : slib::TAB);
		}
		else if ($_ == "Filter")  strm << (var.attribute.hasKey("filter") ? S(var.attribute["filter"] == "." ? "PASS" : var.attribute["filter"]) :
			S((var.flag & NOT_USE_FLAG || var.flag & UNAVAILABLE_FLAG) ? "NG" : "PASS")) << (endl ? slib::LF : slib::TAB);
	}
	strm.flush();
}
void slib::sbio::sio::writeAnnotGenes(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	strm << var.annotation[0].name;
	auto min = sstat::getMin(5, (int)var.annotation.size());
	sforin(i, 1, min) strm << "," << var.annotation[i].name;
	if (5 < var.annotation.size()) strm << ", ...";
}
void slib::sbio::sio::writeAnnotGeneIDs(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	strm << var.annotation[0].gid;
	auto min = sstat::getMin(5, (int)var.annotation.size());
	sforin(i, 1, min) strm << "," << var.annotation[i].gid;
	if (5 < var.annotation.size()) strm << ", ...";
}
void slib::sbio::sio::writeAnnotGeneTypes(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	strm << var.annotation[0].gtype();
	auto min = sstat::getMin(5, (int)var.annotation.size());
	sforin(i, 1, min) strm << "," << var.annotation[i].gtype();
	if (5 < var.annotation.size()) strm << ", ...";
}
void slib::sbio::sio::writeVariantSites(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	strm << var.annotation[0].varSite();
	auto min = sstat::getMin(5, (int)var.annotation.size());
	sforin(i, 1, min) {
		strm << "," << var.annotation[i].varSite();
	}
	if (5 < var.annotation.size()) strm << ", ...";
}
void slib::sbio::sio::writeMutationType(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	if (var.annotation[0].transcripts.size()) {
		auto rna = var.annotation[0].transcripts.begin();
		strm << rna->name << ":" << rna->mutType(); ++rna;
		while(rna < var.annotation[0].transcripts.end()) { 
			strm << "|" << rna->name << ":" << rna->mutType(); ++rna;
		}
		auto min = sstat::getMin(5, (int)var.annotation.size());
		sforin(i, 1, min) {
			strm << ",";
			if (var.annotation[i].transcripts.size()) {
				rna = var.annotation[i].transcripts.begin();
				strm << rna->name << ":" << rna->mutType(); ++rna;
				while (rna < var.annotation[i].transcripts.end()) {
					strm << "|" << rna->name << ":" << rna->mutType(); ++rna;
				}
			}
		}
		if (5 < var.annotation.size()) strm << ", ...";
	}	
}
void slib::sbio::sio::writeSubstitution(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	String subst, s;
	auto min = sstat::getMin(5, (int)var.annotation.size());
	sforin(i, 0, min) {
		sfor(var.annotation[i].transcripts) {
			s = $_.mutFormat();
			if (s.empty()) continue;
			else subst << $_.name << ":" << s << "|";
		}
		if (subst.size()) subst[-1] = ',';
	}
	if (subst.size()) subst.resize(subst.size() - 1);
	strm << subst;
	if (5 < var.annotation.size()) strm << ", ...";
}
void slib::sbio::sio::writeVarEffect(const slib::sbio::Variant& var, slib::IOStream& strm) {
	if (var.annotation.empty()) return;
	auto min = sstat::getMin(5, (int)var.annotation.size());
	String ef, subef;
	sforin(i, 0, min) {
		sfor(var.annotation[i].transcripts) {
			if ($_.effect == sbio::VARIANT_EFFECT::DELETERIOUS || $_.effect == sbio::VARIANT_EFFECT::HIGH_IMPACT) subef = "Deleterious";
			else if ($_.effect == sbio::VARIANT_EFFECT::MODERATE || $_.effect == sbio::VARIANT_EFFECT::TOLERATED) subef = "Moderate or none";
			else if ($_.effect == sbio::VARIANT_EFFECT::UNKNOWN) subef = "Unknown";
			else if ($_.effect == sbio::VARIANT_EFFECT::PATHOGENIC) subef = "Pathogenic";
			else if ($_.effect == sbio::VARIANT_EFFECT::BENIGN) subef = "Benign";
			if (0.f <= $_.score) subef << " (" << $_.score << ")";
			ef << subef << "|";
		}
		if (ef.size()) ef[-1] = ',';
	}
	if (ef.size()) ef.resize(ef.size() - 1);
	strm << ef;
	if (5 < var.annotation.size()) strm << ", ...";
}

/*
inline void _readVCFData(slib::String& row, VarList* list, Array<slib::sbio::Variant>& variants, const sattribute* converter, bool format) {
	stringarray data = row.split("\t");
	if (format && data.size() < 10) throw SBioInfoException();// (ERR_INFO);
	else if (data.size() < 8) throw SBioInfoException();//(ERR_INFO);
	stringarray alts = data[4].split(",");
	auto vcount = alts.size();
	sforin(i, 0, vcount) { 
		if (alts[i] == "<NON_REF>") --vcount;
	}
	variants.resize(vcount);
	if (vcount == 0) return;
	sforin(i, 0, vcount) _VCFToVar(variants[i], alts[i], data, list->refname);
	_setVCFInfo(variants, data[7], list->attribute["INFO"]);
	if (format) _setVCFFormat(variants, data[8], data[9], list->attribute["FORMAT"]);
	if (converter) {
		sfor(*converter) {
			sforeach(var, variants) {
				if (var.pos[0].idx < 0 || !(var.attribute.hasKey($_.key()))) continue;
				auto& val = var.attribute[$_.key()];
				if ($_.value() == "name") var.varid = (const char*)val;
				if ($_.value() == "copy") var.copy.depth[0][0] = val;
				if ($_.value() == "pread") var.read[0] = val;
				if ($_.value() == "nread") var.read[1] = val;
				if ($_.value() == "freq") var.copy.frequency = val;
				if ($_.value() == "alt") var.alt = (const char*)val;
				if ($_.value() == "qual") var.qual = val;
			}
		}
	}
	sfor(variants) {
		if ($_.flag != UNAVAILABLE_FLAG) list->add($_);
	}
}
void sbio::sio::loadVCF(SFile& file, VarList* list, SeqList* ref, const sattribute* converter) {
	list->clearAll();
	list->filetype = "vcf";
	bool format = false;
	if (!file.isOpened()) file.open(nullptr, slib::sio::READ);
	list->name = file.fileName(false);
	Array<slib::sbio::Variant> variants;
	String row;
	while (!file.eof()) {
		file.readLine(row);
		if (row.empty()) continue;
		if (row.beginWith("##")) _readVCFHeader(row, list);
		else if (row[0] == '#') _readVCFTitle(row, list, format);
		else {
			_readVCFData(row, list, variants, converter, format);
			variants.clear();
		}
	}
	list->refnum = (sint)list->refname.size();
	list->sort();
	file.close();
}

/*
inline void parseVCFToVar(slib::sbio::Variant& var, String& alt, stringarray& data, const SArray& reflist) {
	var.pos[0].idx = reflist.findWithKey(data[0], "name");
	var.pos[0].begin = data[1].intValue();
	var.varid = data[2];
	if (alt == "<CNV>") vcfToCNV(var, data);
	else if (sstr::isEnclosed(alt, "<>") || alt.match("[") || alt.match("]")) vcfToSV(var, alt, data);
	else if(alt.equal(REG_(/[A,G,C,T,N]+/i))) {
		if (data[3].length() == alt.length()) {
			if (data[3].length() == 1) vcfToSNV(var, alt, data);
			else vcfToMNV(var, alt, data);
		}
		else if (data[3].length() < alt.length()) vcfToINS(var, alt, data);
		else vcfToDEL(var, alt, data);
	}
	var.qual = data[5].doubleValue();
	var.attribute["filter"] = data[6];
}
inline void parseVCFInfo(Array<slib::sbio::Variant>& variants, String& data, sobj info_attr) {
	sattribute infos = data.parse(";", "=");
	sfor(infos) {
		if (info_attr[$_.key()]["number"] == "A") {
			stringarray vals = $_.value().split(",");
			if (vals.size() < variants.size()) {
				std::cout << "err vcfinfo" << std::endl;
				throw SBioInfoException();// (ERR_INFO);
			}
			sforin(v, 0, variants.size()) variants[v].attribute[$_.key()] = vals[v];
		}
		else { sforeach(var, variants) var.attribute[$_.key()] = $_.value(); }
	}
}
inline void parseVCFFormat(Array<slib::sbio::Variant>& variants, String& keydat, String& formdat, sobj& form_attr) {
	stringarray fmkeys = keydat.split(":"), formats = formdat.split(":");
	sforin(i, 0, fmkeys.size()) {
		auto& key = fmkeys[i]; auto& format = formats[i];
		if (key == "GT") {
			stringarray genotype = format.match("/") ?
				format.split("/") : format.split("|");
			if (variants.size() == 1) {
				if (genotype.size() != 2) {
					std::cout << "genotype error" << std::endl;
					throw SBioInfoException();// (ERR_INFO);
				}
				
				
				//if (genotype[0] == genotype[1]) variants[0].homo = true;
				//else variants[0].homo = false;
				

				variants[0].attribute[key] = format;
			}
			else {
				if (genotype.size() < 2) {
					std::cout << "genotype error" << std::endl;
					throw SBioInfoException();// (ERR_INFO);
				}
				sforin(v, 0, variants.size()) {
					size_t idx;
					if ((idx = genotype.indexOf(String(v + 1))) == NOT_FOUND) variants[v].flag = UNAVAILABLE_FLAG;
					else {
						if (genotype[0] == "0") {
							//variants[v].homo = false;
							variants[v].attribute[key] = "0/1";
						}
						else {
							if (genotype.indexOf(String(v + 1), idx + 1) == NOT_FOUND) {
								//variants[v].homo = false;
								variants[v].attribute[key] = "1/2";
							}
							else {
								//variants[v].homo = true;
								variants[v].attribute[key] = "1/1";
							}
						}
					}
				}
			}
		}
		else {
			if (form_attr[key]["number"] == "A") {
				stringarray vals = format.split(",");
				sforin(v, 0, variants.size()) variants[v].attribute[key] = vals[v];
			}
			else { sforeach(var, variants) var.attribute[key] = format; }
		}
	}
}

inline void readVCFData(String& row, VarList& vl, Array<slib::sbio::Variant>& variants, const sattribute* converter, bool format) {
	stringarray data = row.split("\t");
	if (format && data.size() < 10) throw SBioInfoException();// (ERR_INFO);
	else if (data.size() < 8) throw SBioInfoException();// (ERR_INFO);
	stringarray alts = data[4].split(",");
	auto vcount = alts.size();
	sforin(i, 0, vcount) {
		if (alts[i] == "<NON_REF>") --vcount;
	}
	variants.resize(vcount);
	if (vcount == 0) return;
	sforin(i, 0, vcount) parseVCFToVar(variants[i], alts[i], data, vl.attribute["reference"]["list"]);
	parseVCFInfo(variants, data[7], vl.attribute["INFO"]);
	if (format) parseVCFFormat(variants, data[8], data[9], vl.attribute["format"]);
	
	
	if (converter) {
		sfor(*converter) {
			sfor_(vit, variants) {
				if (vit->pos[0].idx < 0 || !(vit->attribute.hasKey($_.key))) continue;
				auto& val = vit->attribute[$_.key];
				if ($_.value == "name") vit->name = (const char*)val;
				if ($_.value == "copy") vit->copy.depth[0][0] = val;
				if ($_.value == "pread") vit->read[0] = val;
				if ($_.value == "nread") vit->read[1] = val;
				if ($_.value == "freq") vit->copy.frequency = val;
				if ($_.value == "alt") vit->alt = (const char*)val;
				if ($_.value == "qual") vit->qual = val;
			}
		}
	}
	
	
	sfor(variants) {
		if ($_.flag != UNAVAILABLE_FLAG) vl.add(E_);
	}
}





*/

std::function<bool(const slib::sbio::SVar&, const slib::sbio::SVar&)> slib::sbio::sutil::svsorter(const subyte type, bool counter) {
	switch (type) {
	case DELETION:
		return counter ? delsorterc : delsorter;
	case DUPLICATION:
		return dupsorter;
	case INSERTION:
		return inssorter;
	case INVERSION:
		return invsorter;
	case TRANSLOCATION:
		return trssorter;
	case TRANSLOCATION|INVERSION:
		return trinvsorter;
	default:
		return [](const SVar& v1, const SVar& v2) { return false; };
	}
}
bool slib::sbio::sutil::delsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[0].end == v2.pos[0].end) 
		return v1.pos[1].begin < v2.pos[1].begin;
	return v1.pos[0].end < v2.pos[0].end;
}
bool slib::sbio::sutil::delsorterc(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[1].begin == v2.pos[1].begin)
		return v1.pos[0].end < v2.pos[0].end;
	return v1.pos[1].begin < v2.pos[1].begin;
}
bool slib::sbio::sutil::dupsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[1].begin == v2.pos[1].begin)
		return v1.pos[0].end < v2.pos[0].end;
	return v1.pos[1].begin < v2.pos[1].begin;
}
bool slib::sbio::sutil::inssorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[0].end == v2.pos[0].end)
		return v1.pos[1].begin < v2.pos[1].begin;
	return v1.pos[0].end < v2.pos[0].end;
}
bool slib::sbio::sutil::invsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[0].dir == v2.pos[0].dir) {
		if (v1.pos[0].dir) {
			if (v1.pos[0].begin == v2.pos[0].begin)
				return v1.pos[1].begin < v2.pos[1].begin;
			else return v1.pos[0].begin < v2.pos[0].begin;
		}
		else {
			if (v1.pos[0].end == v2.pos[0].end)
				return v1.pos[1].end < v2.pos[1].end;
			else return v1.pos[0].end < v2.pos[0].end;
		}
	}
	return v1.pos[0].dir < v2.pos[0].dir;
}
bool slib::sbio::sutil::trssorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[0].idx == v2.pos[0].idx) {
		if (v1.pos[1].idx == v2.pos[1].idx) {
			if (v1.pos[0].end == v2.pos[0].end)
				return v1.pos[1].begin < v2.pos[1].begin; 
			else return v1.pos[0].end < v2.pos[0].end;
		}
		return v1.pos[1].idx < v2.pos[1].idx;
	}
  return v1.pos[0].idx < v2.pos[0].idx;
}
bool slib::sbio::sutil::trinvsorter(const slib::sbio::SVar& v1, const slib::sbio::SVar& v2) {
	if (!v1.type) return false;
	if (!v2.type) return true;
	if (v1.pos[0].idx == v2.pos[0].idx) {
		if (v1.pos[1].idx == v2.pos[1].idx) {
			if (v1.pos[0].dir == v2.pos[0].dir) {
				if (v1.pos[0].dir) {
					if (v1.pos[0].begin == v2.pos[0].begin)
						return v1.pos[1].begin < v2.pos[1].begin;
					else return v1.pos[0].begin < v2.pos[0].begin;
				}
				else {
					if (v1.pos[0].end == v2.pos[0].end)
						return v1.pos[1].end < v2.pos[1].end;
					else return v1.pos[0].end < v2.pos[0].end;
				}
			}
			return v1.pos[0].dir ? false : true;
		}
		return v1.pos[1].idx < v2.pos[1].idx;
	}
	return v1.pos[0].idx < v2.pos[0].idx;
}