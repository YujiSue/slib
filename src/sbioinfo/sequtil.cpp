#include "sbioinfo/sequtil.h"
#include "sbioinfo/seq.h"
#include "sbioinfo/annotation.h"
slib::String slib::sbio::sutil::seqTypeName(const slib::subyte type) {
    switch (type) {
    case slib::sbio::sseq::DNA:
        return "DNA";
    case slib::sbio::sseq::RNA:
        return "RNA";
    case slib::sbio::sseq::AA:
        return "Amino acid";
    default:
        return "MISC";
    }
}
slib::String slib::sbio::sutil::seqTypeUnit(const slib::subyte type) {
    if (type == slib::sbio::sseq::AA) return "aa";
    else return "bp";
}

slib::String slib::sbio::sutil::gbkKey(const slib::sbio::SeqNote& note) {
    slib::String key;
    switch (note.category) {
    case (subyte)ANNOT_CATEGORY::GENE:
        return "gene";
    case (subyte)ANNOT_CATEGORY::TRANSCRIPT:
    {
        if (note.type == 0) return "misc_RNA";
        else if (note.type == (suint)TRANSCRIPT_TYPE::M_RNA) return "mRNA";
        else if (note.type == (suint)TRANSCRIPT_TYPE::R_RNA) return "rRNA";
        else if (note.type == (suint)TRANSCRIPT_TYPE::T_RNA) return "tRNA";
        else return "ncRNA";
    }
    case (subyte)ANNOT_CATEGORY::STRUCTURE:
    {
        if (note.type == CDS) return "CDS";
        else if (note.type == EXON) return "exon";
        else if (note.type == INTRON) return "intron";
        else if (note.type == UTR3) return "3'UTR";
        else if (note.type == UTR5) return "5'UTR";
        else return "misc_structure";
    }
    case (subyte)ANNOT_CATEGORY::MUTATION:
        return "variation";
    case (subyte)ANNOT_CATEGORY::FEATURE:
    {
        /*
        */
        return "misc_feature";
    }
    default:
        return "misc_feature";
    }
}
slib::String slib::sbio::sutil::gbkPos(const srange& rng, const size_t sz, bool dir) {
    slib::String str;
    if (dir) str << "complement(";
    str << (rng.begin < 0 ? "<1" : S(rng.begin + 1)) << ".." << (sz == -1 || rng.end < sz ? S(rng.end + 1) : ">" + S(sz));
    if (dir) str << ")";
    return str;
}
slib::String slib::sbio::sutil::gbkPos(const sregion& reg, const size_t sz, bool dir) {
    slib::String str;
    if (dir) str << "complement(";
    str << "join(";
    sfor(reg) {
        str << ($_.begin < 0 ? "<1" : S($_.begin + 1)) << ".." << (sz == -1 || $_.end < sz ? S($_.end + 1) : ">" + S(sz)) << ",";
    }
    if (reg.size()) str[-1] = ')';
    if (dir) str << ")";
    return str;
}
slib::String slib::sbio::sutil::gbkPos(const slib::sbio::SeqNote& note, const size_t sz) {
    if (note.count() == 1) return gbkPos(note.range(), sz, note.dir); 
    else return gbkPos(note.region(), sz, note.dir);
}
slib::String slib::sbio::sutil::toGBK(const slib::sbio::SeqNote& note, const size_t sz) {
    slib::String str, key = gbkKey(note);
    str << SP * 5 <<
        sstr::rfill(key, ' ', 16) <<
        gbkPos(note, sz) << LF;
    str << SP * 21 << "/label=" << sstr::dquote(note.name);
    if (key == "gene") str << LF << SP * 21 << "/gene=" << sstr::dquote(note.name);
    if (note.note.size()) str << LF << SP * 21 << "/note=" << sstr::dquote(note.note);
    return str;
}

void slib::sbio::sseq::format(String& seq) { 
#if defined(__GNUC__) && (__GNUC__ < 5)
    String tmp;
    sfor(seq) {
        int c = (int)$_;
        if ((0x40 < c && c < 0x5b) || (0x60 < c && c < 0x7b)) tmp << $_;
    }
    seq.swap(tmp);
#else
    seq.replace(REG("/[^a-zA-Z]+/g"), ""); 
#endif
}
slib::subyte slib::sbio::sseq::checkType(String& seq) {
#if defined(__GNUC__) && (__GNUC__ < 5)
    bool rna = false;
    sfor(seq) {
        if ($_ == 'q' || $_ == 'Q' ||
            $_ == 'e' || $_ == 'E' ||
            $_ == 'i' || $_ == 'I' ||
            $_ == 'l' || $_ == 'L' ||
            $_ == 'f' || $_ == 'F' ||
            $_ == 'p' || $_ == 'P' ||
            $_ == 'j' || $_ == 'J' ||
            $_ == 'z' || $_ == 'Z' ||
            $_ == 'x' || $_ == 'X') return sbio::sseq::AA;
        else if ($_ == 'u' || $_ == 'U') rna = true;
    }
    if (rna) return sbio::sseq::RNA;
    else return sbio::sseq::DNA;

#else
    if (seq.match(REG("/[QEILFPJZX]+/i"))) return sbio::sseq::AA;
    else if (seq.match(REG("/U+/i"))) return sbio::sseq::RNA;
    else if (seq.match(REG("/[ACGTN]+/i"))) return sbio::sseq::DNA;
    return sbio::sseq::MISC;
#endif
}
char slib::sbio::sseq::maskChar(const subyte type) {
    if (type && sseq::DNA || type && sseq::RNA) return sna::mask;
    else if (type && sseq::AA) return saa::mask;
    else return '*';
}
void slib::sbio::sseq::rawcopy(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    Memory<subyte>::copy(seq, &ori[pos], length);
}
void slib::sbio::sseq::reverse(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto end = &ori[pos + length - 1];
    sforin(i, 0, length) { *seq = *end; ++seq; --end; }
}
void slib::sbio::sseq::transcribe(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = sdna::b24(*ori); ++seq; ++ori; }
}
void slib::sbio::sseq::rtranscribe(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = sdna::b42(*ori); ++seq; ++ori; }
}
void slib::sbio::sseq::translate(const subyte* ori, const size_t pos, const size_t length, subyte* seq, const CODON_TABLE& codon) {
    auto size = length / 3; ori += pos;
    sforin(i, 0, size) { *seq = codon[*ori][*(ori + 1)][*(ori + 2)]; ++seq; ori += 3; }
}

size_t slib::sbio::sna::gcCount(const char* seq, const size_t length) {
    size_t count = 0;
    sforin(i, 0, length) {
        if ((*seq) == 'c' || (*seq) == 'C' || (*seq) == 'g' || (*seq) == 'G') ++count;
        ++seq;
    }
    return count;
}
size_t slib::sbio::sna::gcCount(const String& seq) { return gcCount(seq.cstr(), seq.size()); }
size_t slib::sbio::sna::gcCount(const subyte* seq, const size_t length) {
    size_t count = 0;
    sforin(i, 0, length) {
        if ((*seq) == 0x02 || (*seq) == 0x04) ++count;
        ++seq;
    }
    return count;
}
size_t slib::sbio::sna::gcCount(const ubytearray& seq) { return gcCount(seq.data(), seq.size()); }

slib::subyte slib::sbio::sna::encBase4(const char c) {
    switch (c) {
    case 'A':
        return 0;
    case 'a':
        return 0;
    case 'C':
        return 1;
    case 'c':
        return 1;
    case 'G':
        return 2;
    case 'g':
        return 2;
    case 'T':
        return 3;
    case 't':
        return 3;
    case 'U':
        return 3;
    case 'u':
        return 3;
    case 'N':
        return 4;
    case 'n':
        return 4;
    default:
        return 0;
    }
}
slib::subyte slib::sbio::sna::encBase16(const char c) {
    switch (c) {
    case '=':
        return 0;
    case 'A':
        return 1;
    case 'a':
        return 1;
    case 'C':
        return 2;
    case 'c':
        return 2;
    case 'M':
        return 3;
    case 'm':
        return 3;
    case 'G':
        return 4;
    case 'g':
        return 4;
    case 'R':
        return 5;
    case 'r':
        return 5;
    case 'S':
        return 6;
    case 's':
        return 6;
    case 'V':
        return 7;
    case 'v':
        return 7;
    case 'T':
        return 8;
    case 't':
        return 8;
    case 'U':
        return 8;
    case 'u':
        return 8;
    case 'W':
        return 9;
    case 'w':
        return 9;
    case 'Y':
        return 10;
    case 'y':
        return 10;
    case 'H':
        return 11;
    case 'h':
        return 11;
    case 'K':
        return 12;
    case 'k':
        return 12;
    case 'D':
        return 13;
    case 'd':
        return 13;
    case 'B':
        return 14;
    case 'b':
        return 14;
    default:
        return 15;
    }
}
char slib::sbio::sna::compBase(const char b) {
    switch (b) {
    case 'a':
        return 't';
    case 'A':
        return 'T';
    case 't':
        return 'a';
    case 'T':
        return 'A';
    case 'u':
        return 'a';
    case 'U':
        return 'A';
    case 'g':
        return 'c';
    case 'G':
        return 'C';
    case 'c':
        return 'g';
    case 'C':
        return 'G';
    case 'r':
        return 'y';
    case 'R':
        return 'Y';
    case 'y':
        return 'r';
    case 'Y':
        return 'R';
    case 'm':
        return 'k';
    case 'M':
        return 'K';
    case 'k':
        return 'm';
    case 'K':
        return 'M';
    case 'v':
        return 'b';
    case 'V':
        return 'B';
    case 'b':
        return 'v';
    case 'B':
        return 'V';
    case 'h':
        return 'd';
    case 'H':
        return 'D';
    case 'd':
        return 'h';
    case 'D':
        return 'H';
    case 'w':
        return 'w';
    case 'W':
        return 'W';
    case 's':
        return 's';
    case 'S':
        return 'S';
    case 'n':
        return 'n';
    case 'N':
        return 'N';
    default:
        return b;
    }
}
slib::subyte slib::sbio::sna::comp4(const subyte b) {
    switch (b) {
    case 0:
        return 3;
    case 1:
        return 2;
    case 2:
        return 1;
    case 3:
        return 0;
    default:
        return 4;
    }
}
slib::subyte slib::sbio::sna::comp16(const subyte b) {
    switch (b) {
    case 0:
        return 0;
    case 1:
        return 8;
    case 2:
        return 4;
    case 3:
        return 12;
    case 4:
        return 2;
    case 5:
        return 10;
    case 6:
        return 6;
    case 7:
        return 14;
    case 8:
        return 1;
    case 9:
        return 9;
    case 10:
        return 5;
    case 11:
        return 13;
    case 12:
        return 3;
    case 13:
        return 11;
    case 14:
        return 7;
    default:
        return 15;
    }
};
slib::String slib::sbio::sna::complement(const slib::String& seq) {
    String comp(seq.size(), '\0');
    auto p1 = &seq[0]; auto p2 = &comp[-1];
    sforin(i, 0, seq.size()) { *p2 = sna::compBase(*p1); --p2; ++p1; }
    return comp;
}
void slib::sbio::sna::toComplement(String& seq) {
    auto comp = complement(seq);
    seq.swap(comp);
}
void slib::sbio::sna::toComplement4(ubytearray& seq) {
    if (seq.size() == 1) {
        seq[0] = comp4(seq[0]);
        return;
    }
    auto beg = seq.data(), end = seq.data(-1);
    subyte tmp = 0;
    while (beg < end) {
        tmp = *beg;
        *beg = comp4(*end);
        *end = comp4(tmp);
        ++beg; --end;
    }
}
void slib::sbio::sna::toComplement16(ubytearray& seq) {
    if (seq.size() == 1) {
        seq[0] = comp16(seq[0]);
        return;
    }
    auto beg = seq.data(), end = seq.data(-1);
    subyte tmp = 0;
    while (beg < end) {
        tmp = *beg;
        *beg = comp16(*end);
        *end = comp16(tmp);
        ++beg; --end;
    }
}
void slib::sbio::sna::complement1(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    if (length) {
        ori += pos; seq += length - 1;
        sforin(i, 0, length) { *seq = sna::compBase(*ori); --seq; ++ori; }
    }
}
void slib::sbio::sna::complement4(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    if (length) {
        ori += pos; seq += length - 1;
        sforin(i, 0, length) { *seq = sna::comp4(*ori); --seq; ++ori; }
    }
}
void slib::sbio::sna::complement16(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    if (length) {
        ori += pos; seq += length - 1;
        sforin(i, 0, length) { *seq = sna::comp16(*ori); --seq; ++ori; }
    }
}
void slib::sbio::sdna::dec2(const subyte b, char* s) {
    s[1] = DNA_BASE16[b & 0x0F]; s[0] = DNA_BASE16[(b >> 4) & 0x0F];
}
void slib::sbio::sdna::dec4(const subyte b, char* s) {
    s[3] = DNA_BASE4[b & 0x03]; s[2] = DNA_BASE4[(b >> 2) & 0x03];
    s[1] = DNA_BASE4[(b >> 4) & 0x03]; s[0] = DNA_BASE4[(b >> 6) & 0x03];
}
slib::subyte slib::sbio::sdna::b24(subyte b) {
    if (b == 0 || b == 15) return 4;
    sbyte c = -1; while (0 < b) { b >>= 1; ++c; } return c;
}
slib::subyte slib::sbio::sdna::b42(subyte b) { return b == 4 ? 15 : (1 << b); }
slib::subyte slib::sbio::sdna::enc2(const char* s) {
    return (sna::encBase16(s[0]) << 4) | sna::encBase16(s[1]);
}
slib::subyte slib::sbio::sdna::enc4(const char* s) {
    return ((sna::encBase4(s[0]) & 0x03) << 6) |
        ((sna::encBase4(s[1]) & 0x03) << 4) |
        ((sna::encBase4(s[2]) & 0x03) << 2) |
        (sna::encBase4(s[3]) & 0x03);
}

void slib::sbio::sdna::encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = sna::encBase16((char)(*ori)); ++seq; ++ori; }
}
void slib::sbio::sdna::encode2(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto sz = length / 2;
    ori += pos;
    sforin(i, 0, sz) { *seq = enc2((const char*)ori); ori += 2; ++seq; }
    if (2 * sz < length) *seq = sna::encBase16(*ori) << 4;
}
void slib::sbio::sdna::encode4(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto sz = length / 4, rest = length - 4 * sz;
    ori += pos;
    sforin(i, 0, sz) { *seq = enc4((const char*)ori); ori += 4; ++seq; }
    if (rest) {
        *seq = 0; sforin(r, 0, rest) { *seq |= ((sna::encBase4(*ori) & 0x03) << (6 - 2 * r)); ++ori; }
    }
}
void slib::sbio::srna::encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = sna::encBase4(*ori); ++ori; ++seq; }
}
void slib::sbio::saa::encode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = saa::encAA((char)*ori); ++seq; ++ori; }
}
void slib::sbio::sdna::decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = DNA_BASE16[*ori]; ++seq; ++ori; }
}
void slib::sbio::sdna::decode2(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto count = 0; ori += (pos / 2);
    if (pos % 2 && 0 < length) {
        *seq = sbio::DNA_BASE16[(*ori) & 0x0F]; ++seq; ++count; ++ori;
    }
    while (count + 2 <= length) {
        dec2(*ori, (char*)seq); seq += 2; count += 2; ++ori;
    }
    if (count < length) {
        *seq = DNA_BASE16[((*ori) >> 4) & 0x0F];
    }
}
void slib::sbio::sdna::decode4(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    char tmp[4];
    auto count = 0;
    ori += pos / 4;
    if ((pos % 4) && length) {
        sdna::dec4(*ori, tmp);
        sforin(i, (pos % 4), 4) {
            *seq = (subyte)tmp[i];
            ++seq; ++count;
            if (count == length) break;
        }
        ++ori;
    }
    while (count + 4 <= length) { dec4(*ori, (char*)seq); seq += 4; count += 4; ++ori; }
    if (count < length) {
        dec4(*ori, tmp);
        sforin(i, 0, length - count) {
            *seq = (subyte)tmp[i]; ++seq;
        }
    }
}
void slib::sbio::srna::decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = RNA_BASE[*ori]; ++ori; ++seq; }
}
void slib::sbio::saa::decode(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    ori += pos; sforin(i, 0, length) { *seq = AMINO_ACID[*ori]; ++seq; ++ori; }
}
slib::subyte denc12(const slib::subyte* s) { return (s[0] << 4) | s[1]; }
void slib::sbio::sdna::compress2(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto count = 0;
    const subyte* p = ori + pos;
    while (count < length - 2) { *seq = denc12(p); ++seq; p += 2; count += 2; }
    if (count < length) *seq = (*ori) << 4;
}
slib::subyte denc14(const slib::subyte* s) {
    return ((slib::sbio::sdna::b24(s[0]) & 0x03) << 6) |
        ((slib::sbio::sdna::b24(s[1]) & 0x03) << 4) |
        ((slib::sbio::sdna::b24(s[2]) & 0x03) << 2) |
        (slib::sbio::sdna::b24(s[3]) & 0x03);
}
void slib::sbio::sdna::compress4(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto count = 0;
    const subyte* p = ori + pos;
    while (count < length - 4) { *seq = denc14(p); ++seq; p += 4; count += 4; }
    if (count < length) {
        *seq = 0; sforin(r, 0, length - count) { (*seq) |= ((b24(*p) & 0x03) << (6 - 2 * r)); ++p; }
    }
}
void slib::sbio::sdna::ddec21(const slib::subyte b, slib::subyte* s) { s[1] = b & 0x0F; s[0] = (b >> 4) & 0x0F; }
void slib::sbio::sdna::expand2(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    auto count = 0;
    ori += (pos / 2);
    if (pos % 2 && 0 < length) {
        *seq = (*ori) & 0x0F; ++seq; ++count; ++ori;
    }
    while (count + 2 <= length) {
        ddec21(*ori, seq); seq += 2; count += 2; ++ori;
    }
    if (count < length) { *seq = ((*ori) >> 4) & 0x0F; }
}
void slib::sbio::sdna::ddec41(const slib::subyte b, slib::subyte* s) {
    s[3] = slib::sbio::sdna::b42(b & 0x03); s[2] = slib::sbio::sdna::b42((b >> 2) & 0x03);
    s[1] = slib::sbio::sdna::b42((b >> 4) & 0x03); s[0] = slib::sbio::sdna::b42((b >> 6) & 0x03);
}
void slib::sbio::sdna::expand4(const subyte* ori, const size_t pos, const size_t length, subyte* seq) {
    subyte tmp[4];
    auto count = 0;
    ori += pos / 4;
    if ((pos % 4) && length) {
        ddec41(*ori, tmp);
        sforin(i, (pos % 4), 4) {
            *seq = tmp[i];
            ++seq; ++count;
            if (count == length) break;
        }
        ++ori;
    }
    while (count + 4 <= length) { ddec41(*ori, seq); seq += 4; count += 4; ++ori; }
    if (count < length) {
        ddec41(*ori, tmp);
        sforin(i, 0, length - count) {
            *seq = tmp[i]; ++seq;
        }
    }
}

inline float nnEnthalpy(const slib::subyte* seq, const size_t length) {
    float enthalpy = 0.0f;
    sforin(i, 0, length - 1) {
        if ((*seq) == 0x01) {
            if (*(seq + 1) == 0x01) enthalpy -= 9.1f;
            else if (*(seq + 1) == 0x02) enthalpy -= 6.5f;
            else if (*(seq + 1) == 0x04) enthalpy -= 7.8f;
            else if (*(seq + 1) == 0x08) enthalpy -= 8.6f;
            else return 0.0f;
        }
        else if ((*seq) == 0x02) {
            if (*(seq + 1) == 0x01) enthalpy -= 5.8f;
            else if (*(seq + 1) == 0x02) enthalpy -= 11.0f;
            else if (*(seq + 1) == 0x04) enthalpy -= 11.9f;
            else if (*(seq + 1) == 0x08) enthalpy -= 7.8f;
            else return 0.0f;
        }
        else if ((*seq) == 4) {
            if (*(seq + 1) == 0x01) enthalpy -= 5.6f;
            else if (*(seq + 1) == 0x02) enthalpy -= 11.1f;
            else if (*(seq + 1) == 0x04) enthalpy -= 11.0f;
            else if (*(seq + 1) == 0x08) enthalpy -= 6.5f;
            else return 0.0f;
        }
        else if ((*seq) == 0x08) {
            if (*(seq + 1) == 0x01) enthalpy -= 6.0f;
            else if (*(seq + 1) == 0x02) enthalpy -= 5.6f;
            else if (*(seq + 1) == 0x04) enthalpy -= 5.8f;
            else if (*(seq + 1) == 0x08) enthalpy -= 9.1f;
            else return 0.0f;
        }
        else return 0.0f;
        ++seq;
    }
    return enthalpy;
}
inline float nnEntropy(const slib::subyte* seq, const size_t length) {
    float entropy = 0.0f;
    sforin(i, 0, length - 1) {
        if ((*seq) == 0x01) {
            if (*(seq + 1) == 0x01) entropy -= 24.0f;
            else if (*(seq + 1) == 0x02) entropy -= 17.3f;
            else if (*(seq + 1) == 0x04) entropy -= 20.8f;
            else if (*(seq + 1) == 0x08) entropy -= 23.9f;
            else return 0.0f;
        }
        else if ((*seq) == 0x02) {
            if (*(seq + 1) == 0x01) entropy -= 12.9f;
            else if (*(seq + 1) == 0x02) entropy -= 26.6f;
            else if (*(seq + 1) == 0x04) entropy -= 27.8f;
            else if (*(seq + 1) == 0x08) entropy -= 20.8f;
            else return 0.0f;
        }
        else if ((*seq) == 0x04) {
            if (*(seq + 1) == 0x01) entropy -= 13.5f;
            else if (*(seq + 1) == 0x02) entropy -= 26.7f;
            else if (*(seq + 1) == 0x04) entropy -= 26.6f;
            else if (*(seq + 1) == 0x08) entropy -= 17.3f;
            else return 0.0f;
        }
        else if ((*seq) == 0x08) {
            if (*(seq + 1) == 0x01) entropy -= 16.9f;
            else if (*(seq + 1) == 0x02) entropy -= 13.5f;
            else if (*(seq + 1) == 0x04) entropy -= 12.9f;
            else if (*(seq + 1) == 0x08) entropy -= 24.0f;
            else return 0.0f;
        }
        else return 0.0f;
        ++seq;
    }
    return entropy;
}
slib::Pair<int, slib::svec3d> slib::sbio::sdna::meltTemp(const slib::String& seq) {
    auto count = sna::gcCount(seq);
    ubytearray bytes(seq.size());
    sdna::encode((const subyte *)seq.cstr(), 0, seq.size(), bytes.data());
    slib::svec3d vec;
    vec[0] = 16.6f * log10(0.05) - 273.15 + (1000.0 * nnEnthalpy(bytes.data(), seq.size())) / (-10.8 + nnEntropy(bytes.data(), seq.size()) + 1.987 * (log(0.125) - 6 * log(10.0)));
    vec[1] = 4.0 * count + 2.0 * (seq.size() - count);
    vec[2] = 81.5 + 16.6 * log10(0.05) + 41.0 * count / seq.size() - 500.0 / seq.size();
    return Pair<int, svec3d>(vec[0] < 20.0 ? 1 : (vec[0] < 80.0 ? 0 : 2), vec);
}
slib::Pair<int, slib::svec3d> slib::sbio::sdna::meltTemp(const ubytearray& seq) {
    auto count = sna::gcCount(seq);
    slib::svec3d vec;
    vec[0] = 16.6f * log10(0.05) - 273.15 + (1000.0 * nnEnthalpy(seq.data(), seq.size())) / (-10.8 + nnEntropy(seq.data(), seq.size()) + 1.987 * (log(0.125) - 6 * log(10.0)));
    vec[1] = 4.0 * count + 2.0 * (seq.size() - count);
    vec[2] = 81.5 + 16.6 * log10(0.05) + 41.0 * count / seq.size() - 500.0 / seq.size();
    return Pair<int, svec3d>(vec[0] < 20.0 ? 1 : (vec[0] < 80.0 ? 0 : 2), vec);
}



slib::subyte slib::sbio::saa::encAA(const char c) {
    switch (c) {
    case 'A':
        return 0;
    case 'R':
        return 1;
    case 'N':
        return 2;
    case 'D':
        return 3;
    case 'C':
        return 4;
    case 'Q':
        return 5;
    case 'E':
        return 6;
    case 'G':
        return 7;
    case 'H':
        return 8;
    case 'I':
        return 9;
    case 'L':
        return 10;
    case 'K':
        return 11;
    case 'M':
        return 12;
    case 'F':
        return 13;
    case 'P':
        return 14;
    case 'S':
        return 15;
    case 'T':
        return 16;
    case 'W':
        return 17;
    case 'Y':
        return 18;
    case 'V':
        return 19;
    case 'B':
        return 20;
    case 'J':
        return 21;
    case 'Z':
        return 22;
    case 'X':
        return 23;
    default:
        return 24;
    }
}

slib::String slib::sbio::sdna::transcript(const slib::String& seq) {
    slib::String trs(seq.size(), '\0');
    sfor2(trs, seq) $_1 = RNA_BASE[(int)sna::encBase4($_2)];
    return trs;
}
slib::String slib::sbio::srna::rtranscript(const String& seq) {
    slib::String rtrs(seq.size(), '\0');
    sfor2(rtrs, seq) $_1 = DNA_BASE4[(int)sna::encBase4($_2)];
    return rtrs;
}
slib::String slib::sbio::srna::protein(const String& seq, const CODON_TABLE& codon) {
    slib::String prot((seq.size()-1)/3 + 1, '\0');
    ubytearray tmp(seq.size());
    sfor2(tmp, seq) $_1 = sna::encBase4($_2);
    sseq::translate(tmp.data(), 0, tmp.size(), (subyte*)&prot[0], codon);
    sfor(prot) $_ = AMINO_ACID[(int)$_];
    return prot;
}
/*
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
}

smat<SEQ_CONVERTER> slib::sbio::RNA_CONVERTER =
smat<SEQ_CONVERTER>(2, 2, {
    rawcopy, sseq::rencode,
    sseq::rdecode, rawcopy
});

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
    auto beg = &seq[0], end = &seq[(sint)seq.length()-1];
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
*/

slib::sbio::SeqNote::SeqNote() {
    _region.resize(1);
    dir = false; 
    category = (subyte)ANNOT_CATEGORY::FEATURE; 
    type = 0;
}
slib::sbio::SeqNote::SeqNote(const Range<sint>& rng, bool d, const subyte c, const suint t, const char* s, const char* n) : SeqNote() {
    setRange(rng);
    dir = d; 
    category = c; 
    type = t;
    name = s;
    note = n;
}
slib::sbio::SeqNote::SeqNote(const Region<sint>& reg, bool d, const subyte c, const suint t, const char* s, const char* n) {
    _region = reg; 
    dir = d; 
    category = c; 
    type = t; 
    name = s; 
    note = n;
}
slib::sbio::SeqNote::SeqNote(const slib::sbio::AnnotInfo& info) : SeqNote() {
    setRange(slib::shift(info, -1));
    dir = info.dir;
    type = info.type;
    name = info.name;
}
slib::sbio::SeqNote::SeqNote(const SeqNote& sn) {
    _region = sn._region;
    dir = sn.dir;
    category = sn.category;
    type = sn.type;
    name = sn.name;
    note = sn.note;
}
slib::sbio::SeqNote::~SeqNote() {}
slib::sbio::SeqNote& slib::sbio::SeqNote::operator=(const slib::sbio::SeqNote& sn) {
    _region = sn._region;
    dir = sn.dir;
    category = sn.category;
    type = sn.type;
    name = sn.name;
    note = sn.note;
    return *this;
}
size_t slib::sbio::SeqNote::count() const { return _region.size(); }
const slib::Range<slib::sint>& slib::sbio::SeqNote::range() const { return _region[0]; }
const slib::Region<slib::sint>& slib::sbio::SeqNote::region() const { return _region; }
void slib::sbio::SeqNote::setRange(const srange& range) { _region[0] = range; }
void slib::sbio::SeqNote::addRange(const srange& range) { _region.add(range); }
void slib::sbio::SeqNote::setRegion(const sregion& region) { _region = region; }
void slib::sbio::SeqNote::shift(const int i) { _region.shift(i); }
void slib::sbio::SeqNote::complement(const size_t sz) {
    sfor(_region) { $_ = srange(sz - $_.end - 1, sz - $_.begin - 1); }
    _region.sort(); dir = !dir;
}
bool slib::sbio::SeqNote::include(const int pos) const { return _region.include(pos); }
bool slib::sbio::SeqNote::overlap(const srange& range) const { return _region.overlap(range); }
slib::sbio::SeqNote slib::sbio::shift(const SeqNote& sn, const int i) {
    slib::sbio::SeqNote note(sn);
    note.shift(i);
    return note;
}
