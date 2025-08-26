#include "sio/stream.h"
#include "sbioinfo/biutil.h"
#include "sbioinfo/seq.h"
#include "sbioinfo/annotation.h"
slib::sbio::SBioInfoException::SBioInfoException() : Exception() { prefix = "sbioinfo"; }
slib::sbio::SBioInfoException::SBioInfoException(int c, const char* msg, const char* desc) : Exception(c, msg, desc) { prefix = "sbioinfo"; }
slib::sbio::SBioInfoException::~SBioInfoException() {}

slib::String slib::sbio::refMisErrorText(const char* s1, int n1, int l1, const char* s2, int n2, int l2) {
	slib::String str;
	str << "The reference sequence in '"<< s1 << "' (Count:" << n1 << ", Total:" << l1 << " bp)" <<
		" and the reference sequence in '"<< s2 << "' (Count:" << n2 << ", Total:" << l2 << " bp)" <<
		" are mismatched in size";
	return str;
}
#ifdef _WINDLL
/*
slib::String slib::SP = { ' ' };
slib::String slib::CR = { '\r' };
slib::String slib::LF = { '\n' };
slib::String slib::CRLF = { '\r', '\n' };
slib::String slib::TAB = { '\t' };
slib::String slib::DEL = { '\b' };
slib::String slib::NL = slib::CRLF;
*/
slib::IOStream slib::DEFAULT_ISTREAM = slib::IOStream(std::cin);
slib::IOStream slib::DEFAULT_OSTREAM = slib::IOStream(std::cout);
#endif

void slib::sbio::sutil::reverse(srange& range, const size_t sz) { 
	if (!sz) return;
	auto tmp = range.begin;
	range.begin = (int)(sz - range.end - 1); 
	range.end = (int)(sz - tmp - 1);
}
double slib::sbio::sutil::rawVal(double v, double lim) {
	if (v == INFINITY) return lim;
	else {
		auto raw = std::exp(-v * std::log(10.0) / 10.0);
		return raw < lim ? lim : raw;
	}
}
double slib::sbio::sutil::phredVal(double v, double lim) { 
	return -10.0 * (std::log(v < lim ? lim : v) / std::log(10.0));
}

slib::sbio::DirRange::DirRange() : srange(), dir(false) {}
slib::sbio::DirRange::DirRange(sint b, sint e, bool d) : srange(b, e), dir(d) {}
slib::sbio::DirRange::DirRange(const slib::sbio::DirRange& r): srange(r), dir(r.dir) {}
slib::sbio::DirRange::~DirRange() {}
slib::sbio::DirRange& slib::sbio::DirRange::operator = (const slib::sbio::DirRange& r) {
	srange::begin = r.begin; srange::end = r.end; dir = r.dir; return *this;
}
void slib::sbio::DirRange::init() {
	srange::begin = 0; srange::end = 0; dir = false;
}
bool slib::sbio::DirRange::operator < (const slib::sbio::DirRange& r) const {
	if (srange::begin != r.begin) return srange::begin < r.begin;
	else if (srange::end != r.end) return srange::end < r.end;
	else if (dir != r.dir) return dir < r.dir;
	return false;
}
bool slib::sbio::DirRange::operator == (const slib::sbio::DirRange& r) const {
	return srange::begin == r.begin && srange::end == r.end && dir == r.dir;
}
bool slib::sbio::DirRange::operator != (const slib::sbio::DirRange& r) const { return !(*this == r); }
slib::sbio::RefPos::RefPos() : idx(-1), slib::sbio::DirRange() {}
slib::sbio::RefPos::RefPos(sint i, sint b, sint e, bool d) : idx(i), slib::sbio::DirRange(b, e, d) {}
slib::sbio::RefPos::RefPos(const RefPos& p) : idx(p.idx), slib::sbio::DirRange(p) {}
slib::sbio::RefPos::~RefPos() {}
slib::sbio::RefPos& slib::sbio::RefPos::operator = (const slib::sbio::RefPos& p) {
	idx = p.idx; begin = p.begin; end = p.end; dir = p.dir; return *this;
}
slib::String slib::sbio::RefPos::toString(const slib::sbio::SeqList& ref) const {
	return ref[idx].name + ":" + S(begin) + "-" + S(end) + "(" << (dir ? "-" : "+") << ")";
}
slib::String slib::sbio::RefPos::toString(const slib::Array<slib::sbio::AnnotInfo>& ref) const {
	return ref[idx].name + ":" + S(begin) + "-" + S(end) + "(" << (dir ? "-" : "+") << ")";
}
void slib::sbio::RefPos::init() { idx = -1; begin = 0; end = 0; dir = false; }
bool slib::sbio::RefPos::operator < (const slib::sbio::RefPos& p) const {
	if (idx != p.idx) return idx < p.idx;
	if (begin != p.begin) return begin < p.begin;
	if (end != p.end) return end < p.end;
	return  dir < p.dir;
}
bool slib::sbio::RefPos::operator == (const slib::sbio::RefPos& p) const {
	return idx == p.idx && begin == p.begin && end == p.end && dir == p.dir;
}
bool slib::sbio::RefPos::operator != (const slib::sbio::RefPos& p) const {
	return !(*this == p);
}
slib::sbio::RefPos slib::sbio::RefPos::toPos(const char* s, const sindex& refindex, bool zerobase) {
	slib::sbio::RefPos p;
	slib::String str(s), sep = " ";
	if (str.match(":")) {
		sep = ":";
		auto off = str.find(sep);
		auto s_ = str.substring(0, off);
		p.idx = refindex.hasKey(s_) ? (int)refindex[s_] : -1;
		str.clip(off + sep.length());
		if (str.match("-")) sep = "-";
		else if (str.match("_")) sep = "_";
		else if (str.match("..")) sep = "..";
		else sep = " ";
		off = str.find(sep);
		p.begin = str.substring(0, off).intValue();
		str.clip(off + sep.length());
		if (str.match("(")) {
			off = str.find("(");
			p.end = str.substring(0, off).intValue();
			str.clip(off + sep.length());
			p.dir = str[0] == '-';
		}
		else if (str.length()) p.end = str.intValue();
		else p.end = p.begin;
	}
	else {
		if (str.match(TAB)) sep = TAB;
		if (str.match(",")) sep = ",";
		auto val = str.split(sep);
		p.idx = refindex.hasKey(val[0]) ? refindex[val[0]] : (sstr::isNumeric(val[0]) ? val[0].intValue() : -1);
		if (1 < val.size()) {
			p.begin = val[1].intValue();
			if (2 < val.size()) {
				p.end = val[2].intValue();
				if (3 < val.size())
					p.dir = val[3] == "-";
			}
			else p.end = p.begin;
		}
	}
	if (!zerobase) p.shift(-1);
	return p;
}

slib::String slib::sbio::sutil::geneType(sushort i) {
	if (i & (subyte)GENE_TYPE::PROTEIN_CODING) return "Protein coding";
	else if (i & (subyte)GENE_TYPE::NON_CODING) return "Non-coding";
	else if (i & (subyte)GENE_TYPE::PSEUDO_GENE) return "Pseudogene";
	else if (i & (subyte)GENE_TYPE::TRANSPOSON) return "Transposon";
	return "Unknown";
}
slib::String slib::sbio::sutil::transcriptType(sushort i) {
	if (i & (sushort)TRANSCRIPT_TYPE::M_RNA) return "mRNA";
	else if (i & (sushort)TRANSCRIPT_TYPE::T_RNA) return "tRNA";
	else if (i & (sushort)TRANSCRIPT_TYPE::R_RNA) return "rRNA";
	else if (i & (sushort)TRANSCRIPT_TYPE::NC_RNA) return "ncRNA";
	return "Unknown";
}

/*

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


*/

slib::sbio::DirRegion::DirRegion() : Array<DirRange>() {}
slib::sbio::DirRegion::DirRegion(const slib::sbio::DirRegion& reg) : Array<DirRange>(reg) {}
slib::sbio::DirRegion::~DirRegion() {}
slib::sbio::DirRegion& slib::sbio::DirRegion::operator = (const slib::sbio::DirRegion& reg) {
	resize(reg.size());
	sfor2(*this, reg) $_1 = $_2;
	return *this;
}
void slib::sbio::DirRegion::init() { clear(); }

slib::sbio::CODON_TABLE slib::sbio::DEFAULT_CODON = {
	slib::svec4b({11, 2, 11, 2}), slib::svec4b({16, 16, 16, 16}), slib::svec4b({1, 15, 1, 15}), slib::svec4b({9, 9, 12, 9}),
	slib::svec4b({5, 8, 5, 8}), slib::svec4b({14, 14, 14, 14}), slib::svec4b({1, 1, 1, 1}), slib::svec4b({10, 10, 10, 10}),
	slib::svec4b({6, 3, 6, 3}), slib::svec4b({0, 0, 0, 0}), slib::svec4b({7, 7, 7, 7}), slib::svec4b({19, 19, 19, 19}),
	slib::svec4b({24, 18, 24, 18}), slib::svec4b({15, 15, 15, 15}), slib::svec4b({24, 4, 17, 4}), slib::svec4b({10, 13, 10, 13})
};
slib::sbio::CODON_TABLE slib::sbio::DEFAULT_MT_CODON = {
	slib::svec4b({11, 2, 11, 2}), slib::svec4b({16, 16, 16, 16}), slib::svec4b({15, 15, 15, 15}), slib::svec4b({12, 9, 12, 9}),
	slib::svec4b({5, 8, 5, 8}), slib::svec4b({14, 14, 14, 14}), slib::svec4b({1, 1, 1, 1}), slib::svec4b({10, 10, 10, 10}),
	slib::svec4b({6, 3, 6, 3}), slib::svec4b({0, 0, 0, 0}), slib::svec4b({7, 7, 7, 7}), slib::svec4b({19, 19, 19, 19}),
	slib::svec4b({24, 18, 24, 18}), slib::svec4b({15, 15, 15, 15}), slib::svec4b({17, 4, 17, 4}), slib::svec4b({10, 13, 10, 13})
};
