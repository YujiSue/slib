#include "sbioinfo/sbsequtil.h"

using namespace slib;
using namespace slib::sbio;
using namespace slib::smath;

sbseq_annotation::sbseq_annotation() : type(0) {}
sbseq_annotation::sbseq_annotation(suint t, const srange& p, bool d) : type(0), pos(p), dir(d) {}
sbseq_annotation::sbseq_annotation(const sbseq_annotation& a) {
	type = a.type; pos = a.pos;
	dir = a.dir; attribute = a.attribute;
}
sbseq_annotation::~sbseq_annotation() {}
sbseq_annotation& sbseq_annotation::operator=(const sbseq_annotation& a) { 
	type = a.type; pos = a.pos;
	dir = a.dir; attribute = a.attribute;
	return *this; 
}
bool sbseq_annotation::operator<(const sbseq_annotation& a) const { return pos < a.pos; }
bool sbseq_annotation::operator==(const sbseq_annotation& a) const {
	return type == a.type && pos == a.pos;
}

void sseq::seqForm(String& str) {
	str.replace(REG(/[^a-zA-Z]+/g), "");
}
subyte sseq::seqType(String& str) {
	if (str.match(REG(/[QEILFPJZX]+/i))) return AA_SEQ;
	else if (str.match(REG(/U/i))) return RNA_SEQ;
	else return DNA_SEQ;
}
subyte sseq::maskByte(sushort type) {
	auto t = type & 0x0F;
	if (t == DNA_SEQ) {
		auto c = (type >> 4) & 0x0F;
		if (c == 4) return 4;
		else return 0;
	}
	else if (t == RNA_SEQ) return 4;
	else if (t == AA_SEQ) return 23;
	return 0;
}
char sseq::maskChar(sushort type) {
	auto t = type & 0x0F;
	if (t == DNA_SEQ || t == RNA_SEQ) return 'N';
	else if (t == AA_SEQ) return 'X';
	return '*';
}
bool sseq::isATGC(const char &s) {
    return s == 'a' || s == 'A' || s == 't' || s == 'T' ||
    s == 'g' || s == 'G' || s == 'c' || s == 'C';
}
bool sseq::isATGCi(const subyte &b) {
    return b == 1 || b == 2 || b == 4 || b == 8;
}
inline bool sseq::isGC(const char &s) {
    return s == 'g' || s == 'G' || s == 'c' || s == 'C';
}
inline bool sseq::isGCi(const subyte &b) {
    return b == 2 || b == 4;
}
size_t sseq::gcCount(const char *s) {
    size_t count = 0, size = strlen(s);
    sforin(i, 0, size) { if(isGC(*s)) ++count; ++s; }
    return count;
}
size_t sseq::gcCounti(const ubytearray &s) {
    size_t count = 0, size = s.size();
    auto p = s.ptr();
    sforin(i, 0, size) { if(isGCi(*p)) ++count; ++p; }
    return count;
}
bool sseq::containBase(const char &c, const char *s, size_t l) {
    sforin(i, 0, l) { if(*s == c) return true; else ++s; }
    return false;
}
void slib::sbio::rawcopy(const subyte *ori, size_t pos, size_t len, subyte *seq) {
    memcpy(seq, &ori[pos], len);
}
Map<char, subyte> slib::sbio::DNA_BASE16_INDEX = {
    cu('N',0), cu('A',1), cu('C',2), cu('M',3), cu('n',0), cu('a',1), cu('c',2), cu('m',3),
    cu('G',4), cu('R',5), cu('S',6), cu('V',7), cu('g',4), cu('r',5), cu('s',6), cu('v',7),
    cu('T',8), cu('W',9), cu('Y',10), cu('H',11), cu('t',8), cu('w',9), cu('y',10), cu('h',11),
    cu('K',12), cu('D',13), cu('B',14), cu('k',12), cu('d',13), cu('b',14)
};
Map<char, subyte> slib::sbio::DNA_BASE4_INDEX =
{ cu('A',0), cu('C',1), cu('G',2), cu('T',3), cu('a',0), cu('c',1), cu('g',2), cu('t',3) };
Map<char, char> slib::sbio::DNA_COMPLEMENT_CHAR = {
    cc('a','t'), cc('A','T'), cc('t','a'), cc('T','A'), cc('g','c'), cc('G','C'), cc('c','g'), cc('C','G'),
    cc('r','y'), cc('R','Y'), cc('y','r'), cc('Y','R'), cc('m','k'), cc('M','K'), cc('k','m'), cc('K','M'),
    cc('v','b'), cc('V','B'), cc('b','v'), cc('B','V'), cc('h','d'), cc('H','D'), cc('d','h'), cc('D','H'),
    cc('W','W'), cc('w','w'), cc('S','S'), cc('s','s'), cc('n','n'), cc('N','N'), cc('-', '-')
};
bytearray slib::sbio::DNA_COMPLEMENT_IDX =
{ 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
smat<SEQ_CONVERTER> slib::sbio::DNA_CONVERTER(5, 5,
{
    rawcopy, sseq::dencode1, sseq::dencode2, rawcopy, sseq::dencode4,
    sseq::ddecode1, rawcopy, sseq::dcompress2, rawcopy, sseq::dcompress4,
    sseq::ddecode2, sseq::dexpand2, sseq::drecode22, rawcopy, sseq::drecode24,
    rawcopy, rawcopy, rawcopy, rawcopy, rawcopy,
    sseq::ddecode4, sseq::dexpand4, sseq::drecode42, rawcopy, sseq::drecode44
});
void sseq::draw(const subyte &b, char *s) { s[0] = b; }
void sseq::ddec10(const subyte &b, char *s) { s[0] = DNA_BASE16[b]; }
void sseq::ddec20(const subyte &b, char *s) {
    s[1] = DNA_BASE16[b&0x0F]; s[0] = DNA_BASE16[(b>>4)&0x0F];
}
void sseq::ddec40(const subyte &b, char *s) {
    s[3] = DNA_BASE4[b&0x03]; s[2] = DNA_BASE4[(b>>2)&0x03];
    s[1] = DNA_BASE4[(b>>4)&0x03]; s[0] = DNA_BASE4[(b>>6)&0x03];
}
void sseq::denc02(subyte &b, const char *s) {
    b = (DNA_BASE16_INDEX[s[0]]<<4)|DNA_BASE16_INDEX[s[1]];
}
void sseq::denc04(subyte &b, const char *s) {
    b = (DNA_BASE4_INDEX[s[0]])<<6|(DNA_BASE4_INDEX[s[1]])<<4|
    (DNA_BASE4_INDEX[s[2]])<<2|DNA_BASE4_INDEX[s[3]];
}
subyte sseq::b24(subyte s) {
    int8_t c = -1; while (0 < s) { s>>=1; ++c; } return c<0?0:c;
}
subyte sseq::b42(subyte s) { return 1<<s; }
void sseq::ddec21(const subyte &b, subyte *s) {
    s[1] = b&0x0F; s[0] = (b>>4)&0x0F;
}
void sseq::ddec41(const subyte &b, subyte *s) {
    s[3] = sseq::b42(b&0x03); s[2] = sseq::b42((b>>2)&0x03);
    s[1] = sseq::b42((b>>4)&0x03); s[0] = sseq::b42((b>>6)&0x03);
}
void sseq::denc12(subyte &b, const subyte *s) {
    b = (s[0]<<4)|s[1];
}
void sseq::denc14(subyte &b, const subyte *s) {
    b = (sseq::b24(s[0])<<6)|(sseq::b24(s[1])<<4)|(sseq::b24(s[2])<<2)|sseq::b24(s[3]);
}
void sseq::ddecode1(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    ori += pos;
    sforin(i, 0, length) { *seq = DNA_BASE16[*ori]; ++seq; ++ori; }
}
void sseq::ddecode2(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    auto tmp = seq;
    ori += (pos+length)/2; seq += length;
    if((pos+length)%2) { --seq; *seq = DNA_BASE16[((*ori)>>4)&0x0F]; --length; }
    if(1 < length) { do { --ori; seq -= 2; length-=2; ddec20(*ori, (char *)seq); } while (1 < length); }
    if(length) { --ori; --seq; *seq = DNA_BASE16[(*ori)&0x0F]; }
}
void sseq::ddecode4(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    char s[4];
    size_t off = (pos+length)/4, shift = (pos+length)%4;
    if(shift) {
        ddec40(ori[off], s);
        if (shift <= length) { length-=shift; memcpy(&seq[length], s, shift); }
        else { memcpy(&seq[0], &s[pos-off*4], length); length = 0; }
    }
    if(3 < length) { do { --off; length-=4; ddec40(ori[off], (char *)&seq[length]); } while(3 < length); }
    if(length) { ddec40(ori[--off], s); memcpy(&seq[0], &s[4-length], length); }
}
void sseq::dencode1(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    ori += pos;
    sforin(i, 0, length) { *seq = DNA_BASE16_INDEX[(char)(*ori)]; ++seq; ++ori; }
}
void sseq::dencode2(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    size_t size = length/2;
    sforin(i, 0, size) { denc02(seq[i], (char *)&ori[pos]); pos+=2; }
    if(2*size < length) seq[size] = DNA_BASE16_INDEX[ori[pos]]<<4;
}
void sseq::dencode4(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    size_t size = length/4, rest = length-4*size;
    sforin(i, 0, size) { denc04(seq[i], (char *)&ori[pos]); pos+=4; }
    if(rest) {
        seq[size] = 0;
        sforin(r, 0, rest) { seq[size] |= (DNA_BASE4_INDEX[(char)ori[pos]]<<(6-2*r)); ++pos; }
    }
}
void sseq::dexpand2(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    size_t off = (pos+length)/2;
    if((pos+length)%2) seq[--length] = (ori[off]>>4)&0x0F;
    if(1 < length) { do { --off; length-=2; ddec21(ori[off], &seq[length]); } while (1 < length); }
    if(length) seq[0] = ori[--off]&0x0F;
}
void sseq::dexpand4(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    subyte s[4];
    size_t off = (pos+length)/4, shift = (pos+length)%4;
    if(shift) {
        ddec41(ori[off], s);
        if (shift <= length) { length-=shift; memcpy(&seq[length], s, shift); }
        else { memcpy(&seq[0], &s[pos-off*4], length); length = 0; }
    }
    if(3 < length) { do { --off; length-=4; ddec41(ori[off], &seq[length]); } while(3 < length); }
    if(length) { ddec41(ori[--off], s); memcpy(&seq[0], &s[4-length], length); }
}
void sseq::dcompress2(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    auto off = 0; ori += pos;
    while (off < length-2) { denc12(*seq, ori); ++seq; ori+=2; off+=2; }
    if (off < length) *seq = (*ori)<<4;
}
void sseq::dcompress4(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    auto off = 0; ori += pos;
    while (off < length-4) { denc14(*seq, ori); ++seq; ori+=4; off+=4; }
    if (off < length) {
        *seq = 0; sforin(r, 0, length-off) { (*seq)|=b24(*ori)<<(6-2*r); ++ori; }
    }
}
void sseq::drecode22(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    svec<subyte> tmp(length);
    dexpand2(ori, pos, length, tmp.ptr());
    dcompress2(tmp.ptr(), 0, length, seq);
}
void sseq::drecode24(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    svec<subyte> tmp(length);
    dexpand2(ori, pos, length, tmp.ptr());
    dcompress4(tmp.ptr(), 0, length, seq);
}
void sseq::drecode42(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    svec<subyte> tmp(length);
    dexpand4(ori, pos, length, tmp.ptr());
    dcompress2(tmp.ptr(), 0, length, seq);
}
void sseq::drecode44(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    svec<subyte> tmp(length);
    dexpand4(ori, pos, length, tmp.ptr());
    dcompress4(tmp.ptr(), 0, length, seq);
}
void sseq::dcomp(char *seq, size_t s) {
    auto size = s==-1?strlen(seq):s;
    auto beg = seq, end = seq+size-1;
    while (beg < end) {
        auto tmp = *beg;
        *beg = DNA_COMPLEMENT_CHAR[*end];
        *end = DNA_COMPLEMENT_CHAR[tmp];
        ++beg; --end;
    }
}
void sseq::dcomp(String &seq) {
    auto size = seq.size();
    auto beg = seq.ptr(), end = &seq[-1];
    while (beg < end) {
        auto tmp = *beg;
        *beg = DNA_COMPLEMENT_CHAR[*end];
        *end = DNA_COMPLEMENT_CHAR[tmp];
        ++beg; --end;
    }
}
void sseq::dcompi(ubytearray &seq) {
    auto size = seq.size();
    auto beg = seq.ptr(), end = &seq[-1];
    while (beg < end) {
        auto tmp = *beg;
        *beg = DNA_COMPLEMENT_IDX[*end];
        *end = DNA_COMPLEMENT_IDX[tmp];
        ++beg; --end;
    }
}
void sseq::dcpycomp(const char *seq, char *cseq) {
    auto size = strlen(seq);
    auto s = seq+size-1; auto cs = cseq;
    sforin(i, 0, size) { *cs = DNA_COMPLEMENT_CHAR[*s]; --s; ++cs; }
}
String sseq::dcompseq(const char *seq) {
    String cseq(strlen(seq), 0);
    sseq::dcpycomp(seq, &cseq[0]);
    return cseq;
}
void sseq::dcpycompi(ubytearray &seq, ubytearray &cseq) {
    size_t size = seq.size(); cseq.resize(size, 0);
    auto s = &seq[-1], cs = cseq.ptr();
    sforin(i, 0, size) { *cs = DNA_COMPLEMENT_IDX[*s]; --s; ++cs; }
}
ubytearray sseq::dcompseqi(ubytearray &seq) {
    ubytearray cseq(seq.size());
    sseq::dcpycompi(seq, cseq);
    return cseq;
}
Map<char, subyte> slib::sbio::RNA_BASE_INDEX = {
    cu('A',0), cu('a',0), cu('C',1), cu('c',1),
    cu('G',2), cu('g',2), cu('T',3), cu('t',3),
    cu('N',4), cu('n',4)
};
Map<char, char> slib::sbio::RNA_COMPLEMENT_CHAR = {
    cc('a','u'), cc('A','U'), cc('u','a'), cc('U','A'),
    cc('g','c'), cc('G','C'), cc('c','g'), cc('C','G'),
    cc('N','N'), cc('n','n')
};
smat<SEQ_CONVERTER> slib::sbio::RNA_CONVERTER =
smat<SEQ_CONVERTER>(2, 2, {
    rawcopy, sseq::rencode,
    sseq::rdecode, rawcopy
});
void sseq::rdecode(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) { *seq = RNA_BASE[*ori]; ++ori; ++seq; }
}
void sseq::rencode(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) { *seq = RNA_BASE_INDEX[*ori]; ++ori; ++seq; }
}
void sseq::rcomp(char *seq, size_t s) {
    auto beg = seq, end = seq+(s==-1?strlen(seq):s)-1;
    while (beg < end) {
        char tmp = *beg;
        *beg = RNA_COMPLEMENT_CHAR[*end];
        *end = RNA_COMPLEMENT_CHAR[tmp];
        ++beg; --end;
    }
}
void sseq::rcomp(String &seq) {
    auto beg = &seq[0], end = &seq[seq.length()-1];
    while (beg < end) {
        char tmp = *beg;
        *beg = RNA_COMPLEMENT_CHAR[*end];
        *end = RNA_COMPLEMENT_CHAR[tmp];
        ++beg; --end;
    }
}
void sseq::rcompi(ubytearray &seq) {
    auto beg = &seq[0], end = &seq[-1];
    while (beg < end) {
        char tmp = *beg;
        *beg = (*end)^0x03;
        *end = tmp^0x03;
        ++beg; --end;
    }
}
void sseq::rcpycomp(const char *seq, char *cseq) {
    size_t size = strlen(seq);
    auto s = seq+size-1; auto cs = cseq;
    sforin(i, 0, size) { *cs = RNA_COMPLEMENT_CHAR[*s]; ++cs; --s; }
}
String sseq::rcompseq(const char *seq) {
    String cseq(strlen(seq), 0);
    rcpycomp(seq, &cseq[0]);
    return cseq;
}
void sseq::rcpycompi(ubytearray &seq, ubytearray &cseq) {
    size_t size = seq.size(); cseq.resize(size, 0);
    auto s = &seq[-1], cs = cseq.ptr();
    sforin(i, 0, size) { *cs = (*s)^0x03; ++cs; --s; }
}
ubytearray sseq::rcompseqi(ubytearray &seq) {
    ubytearray cseq(seq.size());
    rcpycompi(seq, cseq);
    return cseq;
}
void sseq::dtrans(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) {
        if (*ori == 'T') *seq = 'U';
        else if(*ori == 't') *seq = 'u';
        else *seq = *ori;
        ++ori; ++seq;
    }
}
void sseq::dtransi(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    ori += pos;
    sforin(i, 0, length) { *seq = b24(*ori); ++seq; ++ori; }
}
void sseq::rtrans(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) {
        if (*ori == 'U') *seq = 'T';
        else if (*ori == 'u') *seq = 't';
        else *seq = *ori;
        ++seq; ++ori;
    }
}
void sseq::rtransi(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    ori += pos;
    sforin(i, 0, length) { *seq = b42(*ori); ++seq; ++ori; }
}
Map<char, subyte> slib::sbio::AMINO_ACID_INDEX = {
    cu('A',0), cu('R',1), cu('N',2), cu('D',3), cu('a',0), cu('r', 1), cu('n',2), cu('d',3),
    cu('C',4), cu('Q',5), cu('E',6), cu('G',7), cu('c',4), cu('q',5), cu('e',6), cu('g',7),
    cu('H',8), cu('I',9), cu('L',10), cu('K',11), cu('h',8), cu('i',9), cu('l',10), cu('k',11),
    cu('M',12), cu('F',13), cu('P',14), cu('S',15), cu('m',12), cu('f',13), cu('p',14), cu('s',15),
    cu('T',16), cu('W',17), cu('Y',18), cu('V',19), cu('t',16), cu('w',17), cu('y',18), cu('v',19),
    cu('B',20), cu('J',21), cu('Z',22), cu('X',23), cu('b',20), cu('j',21), cu('z',22), cu('x',23),
    cu('*', 24)
};
smat<SEQ_CONVERTER> slib::sbio::AA_CONVERTER =
smat<SEQ_CONVERTER>(2, 2, {
    rawcopy, sseq::aencode,
    sseq::adecode, rawcopy
});
CODON_TABLE slib::sbio::DEFAULT_CODON =
CODON_TABLE(4, 4, {
    { 11, 2,11, 2 }, { 16,16,16,16 }, {  1,15, 1,15 }, {  9, 9,12, 9 },
    {  5, 8, 5, 8 }, { 14,14,14,14 }, {  1, 1, 1, 1 }, { 10,10,10,10 },
    {  6, 3, 6, 3 }, {  0, 0, 0, 0 }, {  7, 7, 7, 7 }, { 19,19,19,19 },
    { 24,18,24,18 }, { 15,15,15,15 }, { 24, 4,17, 4 }, { 10,13,10,13 } });
void sseq::adecode(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) { *seq = AMINO_ACID[*ori]; ++seq; ++ori; }
}
void sseq::aencode(const subyte *ori, size_t pos, size_t length, subyte *seq) {
    if(!length) return;
    ori += pos;
    sforin(i, 0, length) { *seq = AMINO_ACID_INDEX[*(char *)ori]; ++seq; ++ori; }
}
void sseq::atrans(const subyte *ori, size_t pos, size_t length, subyte *seq, const CODON_TABLE &codon) {
    if(!length) return;
    auto size = length/3;
    ori += pos;
    sforin(i, 0, size) { *seq = codon[*ori][*(ori+1)][*(ori+2)]; ++seq; ori += 3; }
}