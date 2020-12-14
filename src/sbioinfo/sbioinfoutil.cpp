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
sushort slib::sbio::sbiutil::getBin(srange range) {
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
void slib::sbio::sbiutil::getBins(ushortarray &bins, srange range) {
    bins.add(0);
	sint last = (range.end >> 26) + 1;
	sforin(i, (range.begin >> 26), last) bins.add(1 + i);
	last = (range.end >> 23) + 1; sforin(i, (range.begin >> 23), last) bins.add(9 + i);
	last = (range.end >> 20) + 1; sforin(i, (range.begin >> 20), last) bins.add(73 + i);
	last = (range.end >> 17) + 1; sforin(i, (range.begin >> 17), last) bins.add(585 + i);
	last = (range.end >> 14) + 1; sforin(i, (range.begin >> 14), last) bins.add(4681 + i);
}
void slib::sbio::sbiutil::getBins(ushortarray&bins, const sregion &region) {
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
double slib::sbio::sbiutil::scoreVal(double v) { return exp(v * logf(10.0) / -10.0); }
double slib::sbio::sbiutil::phredVal(double v) { return v < smath::D_EPS?INFINITY:-10.0*(log(v)/log(10.0)); }

String slib::sbio::sbiutil::geneType(sushort i) {
	if (i & PROTEIN_CODING) return "Protein coding";
	else if (i & NON_CODING) {
		if (i == sbio::T_RNA) return "tRNA";
		else if (i == sbio::R_RNA) return "rRNA";
		else if (i == sbio::LINC_RNA) return "lincRNA";
		else if (i == sbio::AS_RNA) return "antisense RNA";
		else if (i == sbio::MI_RNA) return "miRNA";
		else if (i == sbio::PI_RNA) return "piRNA";
		else if (i == sbio::SN_RNA) return "snRNA";
		else if (i == sbio::SNO_RNA) return "snoRNA";
		else if (i == sbio::SC_RNA) return "scRNA";
	}
	else if (i & PSEUDOGENE) return "Pseudogene";
	else if (i & TRANSPOSON) return "Transposon";
	return "Unknown";
}
sushort slib::sbio::sbiutil::geneTypeIdx(const char* s) {
	String str = String::lower(s);
	if (str.beginWith("protein")) return sbio::PROTEIN_CODING;
	else if (str.beginWith("pseudo")) return sbio::PSEUDOGENE;
	else if (str.beginWith("transposon")) return sbio::TRANSPOSON;
	else if (str.beginWith("nc")) return sbio::NON_CODING;
	else if (str == "trna") return sbio::T_RNA;
	else if (str == "rrna") return sbio::R_RNA;
	else if (str.beginWith("linc")) return sbio::LINC_RNA;
	else if (str.beginWith("antisense") || str == "asrna") return sbio::AS_RNA;
	else if (str == "mirna") return sbio::MI_RNA;
	else if (str == "pirna") return sbio::PI_RNA;
	else if (str == "snrna") return sbio::SN_RNA;
	else if (str == "snorna") return sbio::SNO_RNA;
	else if (str == "scrna") return sbio::SC_RNA;
	return 0;
}
sobj slib::sbio::sbiutil::decodeGeneType(sushort i) { return geneType(i); }
sushort slib::sbio::sbiutil::encodeGeneType(sobj obj) { return geneTypeIdx(obj); }
String slib::sbio::sbiutil::transcriptType(sushort i) {
	if (i & sbio::M_RNA) return "mRNA";
	else if (i & NON_CODING) {
		if (i == sbio::T_RNA) return "tRNA";
		else if (i == sbio::R_RNA) return "rRNA";
		else if (i == sbio::LINC_RNA) return "lincRNA";
		else if (i == sbio::AS_RNA) return "antisense RNA";
		else if (i == sbio::MI_RNA) return "miRNA";
		else if (i == sbio::PI_RNA) return "piRNA";
		else if (i == sbio::SN_RNA) return "snRNA";
		else if (i == sbio::SNO_RNA) return "snoRNA";
		else if (i == sbio::SC_RNA) return "scRNA";
	}
	return "Unknown";
}
sushort slib::sbio::sbiutil::transcriptTypeIdx(const char* s) {
	String str = String::lower(s);
	if (str == "mrna") return sbio::M_RNA;
	else if (str.beginWith("nc")) return sbio::NON_CODING;
	else if (str == "trna") return sbio::T_RNA;
	else if (str == "rrna") return sbio::R_RNA;
	else if (str.beginWith("linc")) return sbio::LINC_RNA;
	else if (str.beginWith("antisense") || str == "asrna") return sbio::AS_RNA;
	else if (str == "mirna") return sbio::MI_RNA;
	else if (str == "pirna") return sbio::PI_RNA;
	else if (str == "snrna") return sbio::SN_RNA;
	else if (str == "snorna") return sbio::SNO_RNA;
	else if (str == "scrna") return sbio::SC_RNA;
	return 0;
}
sobj slib::sbio::sbiutil::decodeTranscriptType(sushort i) { return sbiutil::transcriptType(i); }
sushort slib::sbio::sbiutil::encodeTranscriptType(sobj obj) { return sbiutil::transcriptTypeIdx(obj); }
stringarray slib::sbio::sbiutil::geneSite(sushort i) {
	if (!i) return { "intergenic" };
	stringarray strs;
	if (i & sbio::CDS) strs.add("CDS");
	if (i & sbio::UTR) {
		if (i & 0x0010)  strs.add("5'UTR");
		else if (i & 0x0020)  strs.add("3'UTR");
		strs.add("UTR");
	}
	if (i & sbio::EXON) strs.add("exon");
	if (i & sbio::INTRON) {
		if (i & 0x0040)  strs.add("splice site");
		strs.add("intron");
	}
	if (i & sbio::PROCESSED)  strs.add("RNA");
	return strs;
}
sushort slib::sbio::sbiutil::geneSiteIdx(const char* s) {
	auto str = String::lower(s);
	if (str == "intergenic") return 0;
	if (str == "cds") return sbio::CDS;
	if (str == "exon") return sbio::EXON;
	if (str == "utr") return sbio::UTR;
	if (str == "5'utr") return sbio::UTR5;
	if (str == "3'utr") return sbio::UTR3;
	if (str == "intron") return sbio::INTRON;
	if (str == "splice site") return sbio::SPLICE_SITE;
	if (str.contain("rna")) return sbio::PROCESSED;
	return 0;
}
sushort slib::sbio::sbiutil::geneSiteIdx(const stringarray& s) {
	sushort i = 0;
	sforeach(s) i |= sbiutil::geneSiteIdx(E_);
	return i;
}
sobj slib::sbio::sbiutil::decodeGeneSite(sushort i) {
	if (!i) return { "intergenic" };
	sarray array;
	if (i & sbio::CDS) array.add("CDS");
	if (i & sbio::EXON) array.add("exon");
	if (i & sbio::UTR) {
		if (i & 0x0010)  array.add("5'UTR");
		else if (i & 0x0020)  array.add("3'UTR");
		array.add("UTR");
	}
	if (i & sbio::PROCESSED)  array.add("RNA");
	if (i & sbio::INTRON) {
		if (i & 0x0040)  array.add("splice site");
		array.add("intron");
	}
	return array;
}
sushort slib::sbio::sbiutil::encodeGeneSite(sobj obj) {
	if (obj.isArray()) {
		sushort idx = 0;
		sforeach(obj) idx |= sbiutil::geneSiteIdx(E_);
		return idx;
	}
	else return sbiutil::varTypeIdx(obj.split(","));
}
String slib::sbio::sbiutil::varType(sushort i) {
	switch (i)
	{
	case sbio::SNV:
		return "SNV";
	case sbio::MNV:
		return "MNV";
	case sbio::DELETION:
		return "DEL";
	case sbio::INSERTION:
		return "INS";
	case sbio::DUPLICATION:
		return "DUP";
	case sbio::MULTIPLICATION:
		return "MUL";
	case sbio::INVERSION:
		return "INV";
	case sbio::TRANSLOCATION:
		return "TRS";
	default:
		return "";
	}
}
stringarray slib::sbio::sbiutil::varTypes(sushort i) {
	stringarray strs;
	if (i & sbio::SNV) strs.add("SNV");
	if (i & sbio::MNV) strs.add("MNV");
	if (i & sbio::DELETION) strs.add("DEL");
	if (i & sbio::INSERTION) strs.add("INS");
	if (i & sbio::DUPLICATION) strs.add("DUP");
	if (i & sbio::MULTIPLICATION) strs.add("MUL");
	if (i & sbio::INVERSION) strs.add("INV");
	if (i & sbio::TRANSLOCATION) strs.add("TRS");
	return strs;
}
sushort slib::sbio::sbiutil::varTypeIdx(const char* s) {
	String str = String::lower(s);
	if (str == "snv" || str.beginWith("single")) return sbio::SNV;
	if (str == "mnv") return sbio::MNV;
	if (str.beginWith("del")) return sbio::DELETION;
	if (str.beginWith("ins")) return sbio::INSERTION;
	if (str.beginWith("dup")) return sbio::DUPLICATION;
	if (str.beginWith("mul")) {
		if (str.contain("nucleotide")) return sbio::MNV;
		else return sbio::MULTIPLICATION;
	}
	if (str.beginWith("inv")) return sbio::INVERSION;
	if (str.contain("trs") || str.contain("trans")) return sbio::TRANSLOCATION;
	return 0;
}
sushort slib::sbio::sbiutil::varTypeIdx(const stringarray& s) {
	sushort idx = 0;
	sforeach(s) idx |= sbiutil::varTypeIdx(E_);
	return idx;
}
sobj slib::sbio::sbiutil::decodeVarType(sushort i) {
	sarray array;
	if (i & sbio::SNV) array.add("SNV");
	if (i & sbio::MNV) array.add("MNV");
	if (i & sbio::DELETION) array.add("DEL");
	if (i & sbio::INSERTION) array.add("INS");
	if (i & sbio::DUPLICATION) array.add("DUP");
	if (i & sbio::MULTIPLICATION) array.add("MUL");
	if (i & sbio::INVERSION) array.add("INV");
	if (i & sbio::TRANSLOCATION) array.add("TRS");
	return array;
}
sushort slib::sbio::sbiutil::encodeVarType(sobj obj) {
	if (obj.isArray()) {
		sushort idx = 0;
		sforeach(obj) idx |= sbiutil::varTypeIdx(E_);
		return idx;
	}
	else return sbiutil::varTypeIdx(obj.split("+"));
}
stringarray slib::sbio::sbiutil::mutType(sushort i) {
	stringarray strs;
	if (i & sbio::SUBSTITUTION) {
		if ((i & sbio::MISSENSE) == sbio::MISSENSE) strs.add("Missense");
		else if ((i & sbio::NONSENSE) == sbio::NONSENSE) strs.add("Nonsense");
		else strs.add("Substitution");
	}
	else if (i & sbio::INDEL_MUT) {
		if ((i & 0xFF00) == sbio::IN_FRAME) strs.add("In frame");
		else if ((i & sbio::FRAME_SHIFT) == sbio::FRAME_SHIFT) strs.add("Frame shift");
		else if ((i & sbio::HEAD_LESION) == sbio::HEAD_LESION) strs.add("Head lesion");
		else if ((i & sbio::TAIL_LESION) == sbio::TAIL_LESION) strs.add("Tail lesion");
		else strs.add("InDel");
	}
	else if (i & sbio::COPYNUM_MUT) {
		if ((i & sbio::NULL_MUT) == sbio::NULL_MUT) strs.add("Null");
		else if ((i & sbio::MULTI_COPY) == sbio::MULTI_COPY) strs.add("Multiple copies");
		else if ((i & sbio::TRIPLET_REPEAT) == sbio::TRIPLET_REPEAT) strs.add("Triplet repeats");
		else strs.add("Copy number mutation");
	}
	else if (i & sbio::REARRANGE_MUT) {
		if ((i & sbio::SPLIT_MUT) == sbio::SPLIT_MUT) strs.add("Gene split");
		else if ((i & sbio::SELF_REARRANGEMENT) == sbio::SELF_REARRANGEMENT) strs.add("Self-rearrangement");
		else if ((i & sbio::ECTOPIC_MUT) == sbio::ECTOPIC_MUT) strs.add("Ectopic gene");
		else strs.add("Rearranged mutation");
	}
	else if (!i) strs.add("Silent");
	return strs;
}
sushort slib::sbio::sbiutil::mutTypeIdx(const char* s) {
	auto str = String::lower(s);
	if (str == "silent") return sbio::SILENT_MUT;
	if (str == "nonsense") return sbio::NONSENSE;
	if (str == ("missense")) return sbio::MISSENSE;
	if (str == ("substitution")) return sbio::SUBSTITUTION;
	if (str.beginWith("indel")) return sbio::INDEL_MUT;
	if (str == ("in frame")) return sbio::IN_FRAME;
	if (str == ("frame shift")) return sbio::FRAME_SHIFT;
	if (str == ("head lesion")) return sbio::HEAD_LESION;
	if (str == ("tail lesion")) return sbio::TAIL_LESION;
	if (str == ("null")) return sbio::NULL_MUT;
	if (str == ("multiple copies")) return sbio::MULTI_COPY;
	if (str.beginWith("triplet")) return sbio::TRIPLET_REPEAT;
	if (str.beginWith("copy number")) return sbio::COPYNUM_MUT;
	if (str == ("gene split")) return sbio::SPLIT_MUT;
	if (str.beginWith("self-rearrange")) return sbio::SELF_REARRANGEMENT;
	if (str.beginWith("ectopic")) return sbio::ECTOPIC_MUT;
	if (str.beginWith("rearrange")) return sbio::REARRANGE_MUT;
	return 0;
}
sushort slib::sbio::sbiutil::mutTypeIdx(const stringarray& s) {
	sushort idx = 0;
	sforeach(s) idx |= sbiutil::mutTypeIdx(E_);
	return idx;
}
sobj slib::sbio::sbiutil::decodeMutType(sushort i) {
	sarray array;
	if (i & sbio::SUBSTITUTION) {
		if ((i & sbio::MISSENSE) == sbio::MISSENSE) array.add("Missense");
		else if ((i & sbio::NONSENSE) == sbio::NONSENSE) array.add("Nonsense");
		else array.add("Substitution");
	}
	else if (i & sbio::INDEL_MUT) {
		if ((i & 0xFF00) == sbio::IN_FRAME) array.add("In frame");
		else if ((i & sbio::FRAME_SHIFT) == sbio::FRAME_SHIFT) array.add("Frame shift");
		else if ((i & sbio::HEAD_LESION) == sbio::HEAD_LESION) array.add("Head lesion");
		else if ((i & sbio::TAIL_LESION) == sbio::TAIL_LESION) array.add("Tail lesion");
		else array.add("InDel");
	}
	else if (i & sbio::COPYNUM_MUT) {
		if ((i & sbio::NULL_MUT) == sbio::NULL_MUT) array.add("Null");
		else if ((i & sbio::MULTI_COPY) == sbio::MULTI_COPY) array.add("Multiple copies");
		else if ((i & sbio::TRIPLET_REPEAT) == sbio::TRIPLET_REPEAT) array.add("Triplet repeats");
		else array.add("Copy number mutation");
	}
	else if (i & sbio::REARRANGE_MUT) {
		if ((i & sbio::SPLIT_MUT) == sbio::SPLIT_MUT) array.add("Gene split");
		else if ((i & sbio::SELF_REARRANGEMENT) == sbio::SELF_REARRANGEMENT) array.add("Self-rearrangement");
		else if ((i & sbio::ECTOPIC_MUT) == sbio::ECTOPIC_MUT) array.add("Ectopic gene");
		else array.add("Rearranged mutation");
	}
	else if (!i) array.add("Silent");
	return array;
}
sushort slib::sbio::sbiutil::encodeMutType(sobj obj) {
	sushort idx = 0;
	sforeach(obj) idx |= sbiutil::mutTypeIdx(E_);
	return idx;
}

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