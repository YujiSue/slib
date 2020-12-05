#include "sbioinfo/sbioinfoutil.h"

using namespace slib;
using namespace slib::sbio;

SBioInfoException::SBioInfoException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
			: SException(f, l, func, e, target, note) {
            prefix = "sbioinfo";

}
SBioInfoException::~SBioInfoException() {}

suint slib::sbio::sbiutil::countBin(Map<sint, suint>& order, srange range) {
	auto cap = range.length() >> 13;
    if (cap < 8) cap = 8;
    order.reserve(cap);
	suint count = 1; order[0] = 0;
    auto beg = range.begin>>26, end = (range.end>>26)+1;
    sforin(i, beg, end) { order[1+i] = count; ++count; }
    beg = range.begin>>23, end = (range.end>>23)+1;
    sforin(i, beg, end) { order[9+i] = count; ++count; }
    beg = range.begin>>20, end = (range.end>>20)+1;
    sforin(i, beg, end) { order[73+i] = count; ++count; }
    beg = range.begin>>17, end = (range.end>>17)+1;
    sforin(i, beg, end) { order[585+i] = count; ++count; }
    beg = range.begin>>14, end = (range.end>>14)+1;
    sforin(i, beg, end) { order[4681+i] = count; ++count; }
    return count;
}
sushort sbiutil::getBin(srange range) {
	range.begin >>= 14; range.end >>= 14;
	if (range.begin == range.end) return 4681 + range.begin;
	range.begin >>= 3; range.end >>= 3;
	if (range.begin == range.end) return 585 + range.begin;
	range.begin >>= 3; range.end >>= 3;
	if (range.begin == range.end) return 73 + range.begin;
	range.begin >>= 3; range.end >>= 3;
	if (range.begin == range.end) return 9 + range.begin;
	range.begin >>= 3; range.end >>= 3;
	if (range.begin == range.end) return 1 + range.begin;
    return 0;
}
void sbiutil::getBins(ushortarray &bins, srange range) {
    bins.add(0);
	sint last = (range.end >> 26) + 1;
	sforin(i, (range.begin >> 26), last) bins.add(1 + i);
	last = (range.end >> 23) + 1; sforin(i, (range.begin >> 23), last) bins.add(9 + i);
	last = (range.end >> 20) + 1; sforin(i, (range.begin >> 20), last) bins.add(73 + i);
	last = (range.end >> 17) + 1; sforin(i, (range.begin >> 17), last) bins.add(585 + i);
	last = (range.end >> 14) + 1; sforin(i, (range.begin >> 14), last) bins.add(4681 + i);
}
void sbiutil::getBins(ushortarray&bins, const sregion &region) {
    sforeach(region) getBins(bins, E_);
    bins.sort();
    auto size = bins.size();
    sforeach(bins) {
        if (E_ == -1) continue;
        if (E_ == E_NEXT) {
			auto it_ = it + 1;
            while (it_ < bins.end() && E_ == *it_) { *it_ = -1; ++it_; --size; }
        }
    }
    bins.sort();
    bins.resize(size);
}
double sbiutil::scoreVal(double v) { return exp(v * logf(10.0) / -10.0); }
double sbiutil::phredVal(double v) { return v < smath::D_EPS?INFINITY:-10.0*(log(v)/log(10.0)); }
sbpos::sbpos() : idx(-1), srange(), dir(false) {}
sbpos::sbpos(sint i, sint b, sint e, bool d) : idx(i), srange(b, e), dir(d) {}
sbpos::sbpos(const char *s, const sindex *namei) : sbpos() { set(s, namei); }
sbpos::sbpos(const sbpos &p) : idx(p.idx), srange(p), dir(p.dir) {}
sbpos::~sbpos() {}
sbpos &sbpos::operator = (const sbpos &p) {
    idx = p.idx; begin = p.begin; end = p.end; dir = p.dir; return *this;
}
void sbpos::set(const char *s, const sindex *namei) {
    String str(s), sep;
    if (str.contain(":")) {
        sep = ":";
        auto off = str.find(sep);
        auto s_ = str.substring(0, off);
		idx = namei?namei->at(s_):s_.intValue();
        str.clip(off+sep.length());
        if (str.contain("-")) sep = "-";
        else if (str.contain("_")) sep = "_";
        else if (str.contain("..")) sep = "..";
        else sep = " ";
        
        off = str.find(sep);
        begin = str.substring(0, off).intValue();
        str.clip(off+sep.length());
        if (str.contain("(")) {
            off = str.find("(");
            end = str.substring(0, off).intValue();
            str.clip(off+sep.length());
            dir = str[0]=='-';
        }
        else if (str.length()) end = str.intValue();
        else end = begin;
    }
    else {
        if (str.contain(TAB)) sep = TAB;
        if (str.contain(",")) sep = ",";
        auto val = str.split(sep);
        idx = namei?namei->at(val[0]):val[0].intValue();
        if (1 < val.size()) {
            begin = val[1].intValue();
            if (2 < val.size()) {
                end = val[2].intValue();
                if (3 < val.size())
                    dir = val[3]=="-";
            }
            else end = begin;
        }
    }
}
void sbpos::set(double d, const intarray* length) {
	sinteger total = 0;
	sforeach(*length) total += E_;
	total *= d;
	sforeachi(*length) {
		if (total < length->at(i)) {
			idx = i; begin = total; end = begin; dir = false; break;
		}
		else total -= length->at(i);
	}
}
double sbpos::absolute(const intarray* length) {
	sinteger total = 0, tmp = 0;
	sforeachi(*length) {
		if (idx == i) tmp += begin;
		else if (i < idx) tmp += length->at(i);
		total += length->at(i);
	}
	return (double)tmp / total;
}
String sbpos::toString(stringarray *names) const {
	return names ? names->at(idx) : String(idx) << "," << begin << "," << end << "," << (dir ? "-" : "+");
}
void sbpos::init() { idx = -1; begin = 0; end = 0; dir = false; }
bool sbpos::operator < (const sbpos &p) const {
    if (idx != p.idx) return idx < p.idx;
    if (begin != p.begin) return begin < p.begin;
    if (end != p.end) return end < p.end;
    return  dir < p.dir;
}
bool sbpos::operator == (const sbpos &p) const {
    return idx == p.idx && begin == p.begin && end == p.end && dir == p.dir;
}
bool sbpos::operator != (const sbpos &p) const {
    return !(*this == p);
}