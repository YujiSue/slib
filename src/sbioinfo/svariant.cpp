#include "sbioinfo/svariant.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::sbio;

svar_data::svar_data() : type(0), qual(0.0) { memset(read, 0, sizeof(sushort) * 2); }
svar_data::svar_data(const svar_data &v) : type(v.type), alt(v.alt), qual(v.qual) {
	memcpy(pos, v.pos, sizeof(sbpos) * 2); memcpy(read, v.read, sizeof(sushort) * 2);
}
svar_data::~svar_data() {}
svar_data &svar_data::operator = (const svar_data &v) {
    type = v.type; alt = v.alt; qual = v.qual;
	memcpy(pos, v.pos, sizeof(sbpos) * 2); memcpy(read, v.read, sizeof(sushort) * 2);
    return (*this);
}
svar_data &svar_data::operator += (const svar_data &v) {
	read[0] += v.read[0];  read[1] += v.read[1]; qual *= v.qual; return (*this);
}
void svar_data::comp() {
    sbpos tmp = pos[0]; pos[0] = pos[1]; pos[1] = tmp;
    pos[0].dir = !pos[0].dir; pos[1].dir = !pos[1].dir;
	auto rtmp = read[0]; read[0] = read[1]; read[1] = rtmp;
    if (alt.size()) sseq::dcomp(&alt[0]);
}
sint svar_data::total() const { return read[0] + read[1]; }
double svar_data::bias() const { return SVarUtil::readBias(read); }
double svar_data::phred() const { return sbiutil::phredVal(qual); }
bool svar_data::comparable(const svar_data& v) const {
	return type == v.type && pos[0].idx == v.pos[0].idx && pos[1].idx == v.pos[1].idx &&
            pos[0].dir == v.pos[0].dir && pos[1].dir == v.pos[1].dir;
}
bool svar_data::lt(const svar_data &var, size_t dist) const {
    if (comparable(var)) return pos[0].begin <= var.pos[0].begin+dist;
    return *this < var;
}
bool svar_data::equal(const svar_data &var, size_t dist) const {
    return comparable(var) &&
     abs(pos[0].begin-var.pos[0].begin) <= dist &&
     abs(pos[0].end-var.pos[0].end) <= dist &&
     abs(pos[1].begin-var.pos[1].begin) <= dist &&
     abs(pos[1].end-var.pos[1].end) <= dist;
}
String svar_data::toString(SBSeqList *ref) const {
	String str;
	str << slib::toString(sbiutil::varTypes(type), "+") << TAB <<
		(ref ? ref->at(pos[0].idx)->name : String(pos[0].idx)) << ":" <<
		pos[0].begin + 1 << ".." << pos[0].end + 1 << TAB <<
		(ref ? ref->at(pos[1].idx)->name : String(pos[1].idx)) << ":" <<
		pos[1].begin + 1 << ".." << pos[1].end + 1 << TAB <<
		alt << TAB << read[0] <<"/" << read[1] << TAB << phred();
	return str;
}
bool svar_data::operator < (const svar_data &v) const {
    if(pos[0].idx != v.pos[0].idx) return pos[0].idx < v.pos[0].idx;
    if(pos[1].idx != v.pos[1].idx) return pos[1].idx < v.pos[1].idx;
    if(pos[0].dir != v.pos[0].dir) return pos[0].dir < v.pos[0].dir;
    if(pos[0].begin != v.pos[0].begin) return pos[0].begin < v.pos[0].begin;
    if(pos[1].begin != v.pos[1].begin) return pos[1].begin < v.pos[1].begin;
    if(pos[0].end != v.pos[0].end) return pos[0].end < v.pos[0].end;
    if(pos[1].end != v.pos[1].end) return pos[1].end < v.pos[1].end;
    if(type != v.type) return type < v.type;
    if(!(alt==v.alt)) return alt < v.alt;
    return false;
}
bool svar_data::operator ==(const svar_data &v) const {
    return pos[0] == v.pos[0] && pos[1] == v.pos[1] && type == v.type && alt == v.alt;
}
scn_data::scn_data() : frequency(0) { 
	memset(depth[0], 0, sizeof(float) * 2); memset(depth[1], 0, sizeof(float) * 2);
	memset(ndepth[0], 0, sizeof(float) * 2); memset(ndepth[1], 0, sizeof(float) * 2);
	memset(ratio, 0, sizeof(float) * 2);
}
scn_data::scn_data(const scn_data& cn) {
	memcpy(depth[0], cn.depth[0], sizeof(float) * 2); memcpy(depth[1], cn.depth[1], sizeof(float) * 2);
	memcpy(ndepth[0], cn.ndepth[0], sizeof(float) * 2); memcpy(ndepth[1], cn.ndepth[1], sizeof(float) * 2);
	memcpy(ratio, cn.ratio, sizeof(float) * 2); frequency = cn.frequency;
}
scn_data::~scn_data() {}
scn_data& scn_data::operator=(const scn_data& cn) {
	memcpy(depth[0], cn.depth[0], sizeof(float) * 2); memcpy(depth[1], cn.depth[1], sizeof(float) * 2);
	memcpy(ndepth[0], cn.ndepth[0], sizeof(float) * 2); memcpy(ndepth[1], cn.ndepth[1], sizeof(float) * 2);
	memcpy(ratio, cn.ratio, sizeof(float) * 2); frequency = cn.frequency; return *this;
}
cnvariant::cnvariant() : type(NON_CNV), prob(0.0) { memset(copy, 0, 2 * sizeof(double)); }
cnvariant::cnvariant(CNV_TYPE t, sint p, float* s, float* b, sint bin) {
	type = t; pos = srange(p * bin, (p + 1) * bin - 1); copy[0] = (*s) * bin; copy[1] = (b ? (*b) : 1.0) * bin;
}
cnvariant::cnvariant(float* s, float* b, double* r, double* border) {
	type = cnvariant::classify(s, b, r, border);
}
cnvariant::cnvariant(const cnvariant& cnv) {
	type = cnv.type; pos = cnv.pos; copy[0] = cnv.copy[0]; copy[1] = cnv.copy[1]; prob = cnv.prob;
}
cnvariant::~cnvariant() {}

cnvariant& cnvariant::operator=(const cnvariant& cnv) {
	type = cnv.type; pos = cnv.pos; copy[0] = cnv.copy[0]; copy[1] = cnv.copy[1]; prob = cnv.prob;
	return  *this;
}
CNV_TYPE cnvariant::classify(float* sdp, float* bdp, double* ratio, double* border) {
	double copy = (*sdp) / (bdp ? (*bdp) : 1.0) * (*ratio);
	if (copy < border[0]) return HOMO_DEL_CNV;
	else if (copy < border[1]) return HETERO_DEL_CNV;
	else if (copy < border[2]) return NON_CNV;
	else if (copy < border[3]) return HETERO_DUP_CNV;
	else if (copy < border[4]) return HOMO_DUP_CNV;
	else return MULTI_CNV;
}

SVariant::SVariant() : flag(0), homo(false), svar_data() {}
SVariant::SVariant(sobj obj) : SVariant() { set(obj); }
SVariant::SVariant(const svar_data& v) : flag(0), homo(false), svar_data(v) {}
SVariant::SVariant(sushort f, const svar_data &v) : flag(f), homo(false), svar_data(v) {}
SVariant::SVariant(const SVariant &var) : svar_data(var) {
    flag = var.flag; name = var.name;
	ref[0] = var.ref[0]; ref[1] = var.ref[1];
	copy = var.copy; homo = var.homo;
    genes = var.genes; mutants = var.mutants;
    attribute = var.attribute;
}
SVariant::~SVariant() {}

SVariant& SVariant::operator=(const svar_data &var) {
	type = var.type; alt = var.alt; qual = var.qual;
	memcpy(pos, var.pos, sizeof(sbpos) * 2); memcpy(read, var.read, sizeof(sushort) * 2);
	return *this;
}
SVariant &SVariant::operator=(const SVariant &var) {
	(*this) = svar_data(var);
	flag = var.flag; name = var.name;
	ref[0] = var.ref[0]; ref[1] = var.ref[1];
	copy = var.copy; homo = var.homo;
	genes = var.genes; mutants = var.mutants;
	attribute = var.attribute; return *this;
}

bool SVariant::comparable(const SVariant *var) const {
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
sushort SVariant::annotatedSite() const {
	if (genes.empty()) return 0;
	sushort site = 0;
	sforeach(genes) {
		sforeach_(tit, E_.transcripts) site |= tit->site;
	}
	return site;
}
void SVariant::set(sobj obj) {
	flag = obj["flag"];
	name = obj["name"];
	ref[0] = obj["ref1"];
	ref[1] = obj["ref2"];
	auto cns = obj["copy"];
	copy.depth[0][0] = cns[0];
	copy.depth[0][1] = cns[1];
	copy.depth[1][0] = cns[2];
	copy.depth[1][1] = cns[3];
	copy.ndepth[0][0] = cns[4];
	copy.ndepth[0][1] = cns[5];
	copy.ndepth[1][0] = cns[6];
	copy.ndepth[1][1] = cns[7];
	copy.ratio[0] = cns[8];
	copy.ratio[1] = cns[9];
	copy.frequency = cns[10];
	if (obj["homo"]) homo = true;
	if (obj["genes"]) { sforeach(obj["genes"]) genes.add(E_); }
	if (obj["mut"]) sforeach(obj["mut"]) mutants.add(E_.string());
	if (obj["attr"]) attribute = obj["attr"];
}
sobj SVariant::toObj() { 
	sarray copies = { 
		copy.depth[0][0], copy.depth[0][1], copy.depth[1][0], copy.depth[1][1] ,
		copy.ndepth[0][0], copy.ndepth[0][1], copy.ndepth[1][0], copy.ndepth[1][1],
		copy.ratio[0], copy.ratio[1], copy.frequency
	}, garray, marray;
	if (!genes.empty()) sforeach(genes) genes.add(E_.toObj());
	if (!mutants.empty()) sforeach(mutants) marray.add(E_);
	return {
		kv("flag", flag), kv("name", name), kv("type", sbiutil::encodeVarType(type)),
		kv("pos1", V({ kv("ref", ref[0]), kv("idx", pos[0].idx), kv("begin", pos[0].begin), kv("end", pos[0].end), kv("dir", pos[0].dir) })),
		kv("pos2", V({ kv("ref", ref[1]), kv("idx", pos[1].idx), kv("begin", pos[1].begin), kv("end", pos[1].end), kv("dir", pos[1].dir) })),
		kv("alt", alt), kv("read", V({ kv("forward", read[0]), kv("reverse", read[1]) })), kv("qual", qual),
		kv("copy", copies), kv("homo", homo), kv("genes", garray), kv("mut", marray), kv("attr", attribute)
	};
}
bool SVariant::operator <(const SVariant& var) const {
	return *((const svar_data*)this) < *((const svar_data*)&var);
}
bool SVariant::operator ==(const SVariant &var) const {
    return *((const svar_data *)this) == *((const svar_data *)&var);
}
