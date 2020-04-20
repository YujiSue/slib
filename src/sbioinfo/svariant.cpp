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
//svar_data &svar_data::dat() { return *this; }
//const svar_data &svar_data::dat() const { return *this; }

void svar_data::comp() {
    sbpos tmp = pos[0]; pos[0] = pos[1]; pos[1] = tmp;
    pos[0].dir = !pos[0].dir; pos[1].dir = !pos[1].dir;
    if (alt.size()) sseq::dcomp(&alt[0]);
}
sint svar_data::total() const { return read[0] + read[1]; }
double svar_data::bias() const { return SVarUtil::readBias(read); }
double svar_data::phred() const { return SBIUtil::phredVal(qual); }
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
	memset(depth, 0, sizeof(float) * 2); memset(ndepth, 0, sizeof(float) * 2);
	memset(bgdepth, 0, sizeof(float) * 2); memset(bgndepth, 0, sizeof(float) * 2);
	memset(ratio, 0, sizeof(float) * 2);
}
scn_data::scn_data(const scn_data& cn) {
	memcpy(depth, cn.depth, sizeof(float) * 2); memcpy(ndepth, cn.ndepth, sizeof(float) * 2);
	memcpy(bgdepth, cn.bgdepth, sizeof(float) * 2); memcpy(bgndepth, cn.bgndepth, sizeof(float) * 2);
	memcpy(ratio, cn.ratio, sizeof(float) * 2); frequency = cn.frequency;
}
scn_data::~scn_data() {}
scn_data& scn_data::operator=(const scn_data& cn) {
	memcpy(depth, cn.depth, sizeof(float) * 2); memcpy(ndepth, cn.ndepth, sizeof(float) * 2);
	memcpy(bgdepth, cn.bgdepth, sizeof(float) * 2); memcpy(bgndepth, cn.bgndepth, sizeof(float) * 2);
	memcpy(ratio, cn.ratio, sizeof(float) * 2); frequency = cn.frequency; return *this;
}
SVariant::SVariant() : flag(0), homo(false), svar_data() {}
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
void SVariant::set(sobj& obj) {}
sobj SVariant::toObj() { return snull; }
bool SVariant::operator <(const SVariant &var) const {
    return *((const svar_data *)this) < *((const svar_data *)&var);
}
bool SVariant::operator ==(const SVariant &var) const {
    return *((const svar_data *)this) == *((const svar_data *)&var);
}
