#include "sio/stream.h"
#include "sbioinfo/seq.h"
#include "sbioinfo/seqio.h"
#include "sbioinfo/fasta.h"

slib::sbio::SeqAnnotation::SeqAnnotation() {}
slib::sbio::SeqAnnotation::~SeqAnnotation() {}
size_t slib::sbio::SeqAnnotation::count() const { return _annot.size(); }
bool slib::sbio::SeqAnnotation::annotated(const int pos) const {
    sfor(_annot) {
        if ($_.include(pos)) return true;
    }
    return false;
}
bool slib::sbio::SeqAnnotation::annotated(const srange& rng) const {
    sfor(_annot) {
        if ($_.overlap(rng)) return true;
    }
    return false;
}
slib::ArrayIterator<slib::sbio::SeqNote> slib::sbio::SeqAnnotation::begin() { return _annot.begin(); }
slib::ArrayCIterator<slib::sbio::SeqNote> slib::sbio::SeqAnnotation::begin() const { return _annot.begin(); }
slib::ArrayIterator<slib::sbio::SeqNote> slib::sbio::SeqAnnotation::end() { return _annot.end(); }
slib::ArrayCIterator<slib::sbio::SeqNote> slib::sbio::SeqAnnotation::end() const { return _annot.end(); }

void slib::sbio::SeqAnnotation::addNote(const SeqNote& note) { _annot.add(note); }
void slib::sbio::SeqAnnotation::clear() { _annot.clear(); }
void slib::sbio::SeqAnnotation::shift(const int i) { sfor(_annot) $_.shift(i); }

slib::sbio::Sequence::Sequence() : _type(0xFFFF), _length(0), ubytearray() {}
slib::sbio::Sequence::Sequence(const sushort t) : _type(t), _length(0), ubytearray() {}
slib::sbio::Sequence::Sequence(const char* seq, const sushort t) : Sequence() { setSeqAs(seq, t); }
slib::sbio::Sequence::Sequence(slib::sbio::Sequence&& seq) noexcept : ubytearray(std::forward<ubytearray&&>(seq)) {
    _type = seq._type; _length = seq._length; name = seq.name; mask = seq.mask; 
    annotation = std::forward < slib::sbio::SeqAnnotation&&> (seq.annotation);
    attribute = std::forward<slib::SDictionary&&>(seq.attribute);
}
slib::sbio::Sequence::Sequence(const slib::sbio::Sequence &seq) : ubytearray(seq) {
    _type = seq._type; _length = seq._length; name = seq.name; mask = seq.mask; 
    annotation = seq.annotation; attribute = seq.attribute;
}
slib::sbio::Sequence::~Sequence() {}
slib::sbio::Sequence & slib::sbio::Sequence::operator=(const Sequence &seq) {
    _type = seq._type; _length = seq._length; name = seq.name; mask = seq.mask; 
    annotation = seq.annotation; attribute = seq.attribute;
    ubytearray::operator=(seq);
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::operator<<(const char* seq) {
    String str(seq);
    sseq::format(str);
    auto l = _length;
    setLength(_length + str.size());
    auto enc = encoder();
    enc((subyte*)&str[0], 0, str.size(), ubytearray::data((int)l));
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::operator<<(const slib::sbio::Sequence& seq) {
    if (isCircular()) throw SBioInfoException();
    if (this->_type != seq._type) throw SBioInfoException();
    ubytearray::append(seq);
    sfor(seq.mask) mask.add(slib::shift($_,(sint)length()));
    sfor(seq.annotation) annotation.addNote(slib::sbio::shift($_, (sint)length()));
    return *this;
}
bool slib::sbio::Sequence::isDNA() const { return _type & sseq::DNA; }
bool slib::sbio::Sequence::isRNA() const { return _type & sseq::RNA; }
bool slib::sbio::Sequence::isAA() const { return _type & sseq::AA; }
bool slib::sbio::Sequence::isMisc() const { return _type & sseq::MISC; }
bool slib::sbio::Sequence::isCircular() const { return _type & sseq::CIRCULAR; }
bool slib::sbio::Sequence::isMasked() const { return _type & sseq::MASKED; }
bool slib::sbio::Sequence::isAnnotated() const { return attribute.size(); }

slib::sbio::SEQ_CONVERTER slib::sbio::Sequence::decoder() const {
    switch (_type & 0x0F) {
    case sseq::DNA:
    {
        switch (compression()) {
        case 1:
            return &sbio::sdna::decode;
        case 2:
            return &sbio::sdna::decode2;
        case 4:
            return &sbio::sdna::decode4;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    case sseq::RNA:
    {
        switch (compression()) {
        case 1:
            return &sbio::srna::decode;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    case sseq::AA:
    {
        switch (compression()) {
        case 1:
            return &sbio::saa::decode;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    default:
        return &sbio::sseq::rawcopy;
    }
}
slib::sbio::SEQ_CONVERTER slib::sbio::Sequence::encoder() const {
    switch (_type & 0x0F) {
    case sseq::DNA:
    {
        switch (compression()) {
        case 1:
            return &sbio::sdna::encode;
        case 2:
            return &sbio::sdna::encode2;
        case 4:
            return &sbio::sdna::encode4;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    case sseq::RNA:
    {
        switch (compression()) {
        case 1:
            return &sbio::srna::encode;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    case sseq::AA:
    {
        switch (compression()) {
        case 1:
            return &sbio::saa::encode;
        default:
            return &sbio::sseq::rawcopy;
        }
    }
    default:
        return &sbio::sseq::rawcopy;
    }
}
slib::subyte slib::sbio::Sequence::type() const { return _type & 0xFF; }
slib::subyte slib::sbio::Sequence::compression() const { return (_type >> 8) & 0x07; }
size_t slib::sbio::Sequence::length() const { return _length; }
inline void findMaskedRegion(slib::sbio::Sequence& seq, slib::String& str) {
    auto mch = slib::sbio::sseq::maskChar((slib::subyte)seq.type());
    sfor(str) {
        if ($_ == mch) {
            auto idx = (int)$INDEX(str);
            srange rng(idx, idx - 1);
            while ($ < str.end() && $_ == mch) { ++rng.end; $NEXT;($); }
            seq.mask.add(rng);
        }
    }
}
void slib::sbio::Sequence::setSeq(const char* seq) {
    slib::String str(seq);
    sseq::format(str);
    _type = 0x0100 | sseq::checkType(str);
    setLength(str.size());
    if (_type & sbio::sseq::MASKED) findMaskedRegion(*this, str);
    encoder()((const subyte*)str.cstr(), 0, _length, data());
}
void slib::sbio::Sequence::setSeqAs(const char* seq, const sushort t) {
    _type = t;
    if (!seq || seq[0] == '\0') return;
    slib::String str(seq);
    sseq::format(str);
    if (_type == 0xFFFF) _type = (sseq::checkType(str) | 0x0100);
    setLength(str.size());
    if (_type & sbio::sseq::MASKED) findMaskedRegion(*this, str);
    encoder()((const subyte*)str.cstr(), 0, _length, data());
}
void slib::sbio::Sequence::setLength(const size_t len, bool resize) { 
    _length = len; 
    if (resize) ubytearray::resize((_length - 1) / (compression() ? compression() : 1) + 1);
}
//void slib::sbio::Sequence::convert(const sushort t) {}

slib::sbio::Sequence& slib::sbio::Sequence::linearize() { 
    if (_type & sseq::CIRCULAR) _type -= sseq::CIRCULAR; 
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::circulate() {
    _type |= slib::sbio::sseq::CIRCULAR; 
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::complement() {
    if (isDNA() || isRNA()) {
        if (isDNA()) {
            auto c = compression();
            if (c != 1) {
                ubytearray tmp(length());
                if (c == 2) sdna::expand2(ubytearray::data(), 0, length(), &tmp[0]);
                else if (c == 4) sdna::expand4(ubytearray::data(), 0, length(), &tmp[0]);
                ubytearray::swap(tmp);
            }
            sna::toComplement16(*this);
        }
        else if (isRNA()) sna::toComplement4(*this);
        sfor(mask) $_ = srange(length() - $_.end - 1, length() - $_.begin - 1);
        sfor(annotation) $_.complement(length());
    }
    else throw FormatException(formatErrorText("Sequence type", sbio::sutil::seqTypeName(type()), "DNA or RNA"));
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::transcribe() {
    if (isDNA()) {
        auto c = compression();
        if (1 < c) {
            ubytearray tmp(length());
            if (c == 2) sdna::expand2(ubytearray::data(), 0, length(), &tmp[0]);
            else if (c == 4) sdna::expand4(ubytearray::data(), 0, length(), &tmp[0]);
            ubytearray::swap(tmp);
        }
        sseq::transcribe(ubytearray::data(), 0, _length, ubytearray::data());
        _type = RNA_SEQ;
    }
    else throw FormatException(formatErrorText("Sequence type", sbio::sutil::seqTypeName(type()), "DNA"));
    return *this;
}
slib::sbio::Sequence& slib::sbio::Sequence::translate(const CODON_TABLE& coden) {
    if (isRNA()) {
        sseq::translate(ubytearray::data(), 0, _length, ubytearray::data());
        _length /= 3;
        ubytearray::resize(_length);
        _type = AA_SEQ;
    }
    else throw FormatException(formatErrorText("Sequence type", sbio::sutil::seqTypeName(type()), "RNA"));
    return *this;
}
void slib::sbio::Sequence::clearAll() {
    ubytearray::clear();
    _type = 0;
    _length = 0;
    mask.clear();
    name.clear();
    annotation.clear();
    attribute.clear();
}
void slib::sbio::Sequence::load(const char* path) {
    SFile f(path);
    auto ext = sfs::extension(path);
    if (ext.match(REG("/f[na]*a.*/"))) sbio::sio::readFa(f, *this);
    else if (ext.beginWith("gb")) sbio::sio::readGbk(f, *this);
    else if (ext == "ab1") sbio::sio::readAbi(f, *this);
    else sbio::sio::readTxt(f, *this);
}
void slib::sbio::Sequence::save(const char* path) {
    SFile f(path, slib::sio::MAKE);
    IOStream ostream;
    ostream.setFileOStream(f);
    auto ext = slib::sfs::extension(path);
    if (ext.match(REG("/f[na]*a.*/"))) sbio::sio::writeFa(ostream, *this);
    else if (ext.beginWith("gb")) sbio::sio::writeGbk(ostream, *this);
    else sbio::sio::writeTxt(ostream, *this);
}
slib::sbio::Sequence slib::sbio::Sequence::subsequence(const size_t off, const size_t len) const {
    Range<sint> range(off, off + (len == -1 ? length() - off : len) - 1);
    return subsequence(range);
}
slib::sbio::Sequence slib::sbio::Sequence::subsequence(const Range<sint>& range) const {
    slib::sbio::Sequence seq;
    seq.setSeqAs(raw(range), (0x01 << 8) | type());
    seq.name << name << "-subseq(" << range.begin + 1 << "," << range.end + 1 << ")";
    sfor(mask) {
        if ($_.overlap(range)) seq.mask.add($_ & range);
    }
    seq.mask.shift(-range.begin);
    sfor(annotation) {
        if ($_.overlap(range)) seq.annotation.addNote($_);
    }
    seq.annotation.shift(-range.begin);
    seq.attribute = attribute;
    return seq;
}
slib::sbio::Sequence slib::sbio::Sequence::subsequence(const slib::Region<sint>& region) const {
    slib::sbio::Sequence seq;
    sfor(region) seq << subsequence($_);
    return seq;
}

slib::String slib::sbio::Sequence::raw(const size_t off, size_t len, bool dir) const {
    if (_length == off) return "";
    else if (_length < off) throw RangeException(outRangeErrorText("offset", off, 0, _length));
    if (len == -1 || (_length < (off + len))) len = _length - off;
    slib::String rawseq(len, '\0');
    decoder()(data(), off, len, (subyte*)&rawseq[0]);
    if (isMasked()) {
        auto mch = sseq::maskChar(_type & 0x0F);
        srange range((sint)off, (sint)(off + len - 1));
        auto region = mask.subregion(range);
        region.shift(-range.begin);
        if (!region.empty()) {
            sfor(region) {
                sforin(i, $_.begin, $_.end + 1) rawseq[i] = mch;
            }
        }
    }
    if (dir && (isDNA() || isRNA())) sna::toComplement(rawseq);
    return rawseq;
}
slib::String slib::sbio::Sequence::raw(const DirRange& range) const {
    size_t off = range.begin < 0 ? 0 : range.begin,
        len = range.end == -1 ? (_length - off) : range.end - off + 1;
    return raw(off, len, range.dir);
}
slib::String slib::sbio::Sequence::raw(const srange& range, bool dir) const {
    return raw(range.begin, range.length(true), dir);
}
slib::String slib::sbio::Sequence::raw(const sregion& region, bool dir) const {
    slib::String rawseq;
    sfor(region) rawseq << raw($_);
    if (dir) slib::sbio::sna::toComplement(rawseq);
    return rawseq;
}

slib::sbio::Sequence slib::sbio::splice(const Sequence& seq, const sregion& region, bool rev, bool zero) {
    if (seq.isAA()) throw FormatException(formatErrorText("Original sequence", "amino acid sequence", "DNA or RNA"));
    slib::sbio::Sequence spliced("", sbio::DNA_SEQ);
    spliced.setLength(region.length());
    auto ptr = spliced.data();
    sfor(region) {
        switch (seq.compression()) {
        case 1:
            sseq::rawcopy(seq.data(), $_.begin - (zero ? 0 : 1), $_.length(true), ptr);
            break;
        case 2:
            sdna::expand2(seq.data(), $_.begin - (zero ? 0 : 1), $_.length(true), ptr);
            break;
        case 4:
            sdna::expand4(seq.data(), $_.begin - (zero ? 0 : 1), $_.length(true), ptr);
            break;
        default:
            sdna::encode(seq.data(), $_.begin - (zero ? 0 : 1), $_.length(true), ptr);
            break;
        }
        ptr += $_.length(true);
    }
    if (rev) spliced.complement();
    return spliced;
}
slib::sbio::Sequence slib::sbio::transcribe(const Sequence& seq) {
    if (!seq.isDNA()) throw FormatException(formatErrorText("Original sequence", sbio::sutil::seqTypeName(seq.type() & 0x07), "DNA"));
    slib::sbio::Sequence transcript = seq;
    transcript.transcribe();
    return transcript;
}
slib::sbio::Sequence slib::sbio::translate(const Sequence& seq, const CODON_TABLE& code) {
    if (!seq.isRNA()) throw FormatException(formatErrorText("Original sequence", sbio::sutil::seqTypeName(seq.type() & 0x07), "RNA"));
    slib::sbio::Sequence protein = seq;
    protein.translate(code);
    return protein;
}


/*

void Sequence::convert(sushort t) {
    if ((_type&0x00FF) != (t&0x00FF)) {
        if ((_type&0x000F) == (t&0x000F)) {
            auto c1 = (_type>>4)&0x000F, c2 = (t>>4)&0x000F;
            switch (seqtype()) {
                case DNA_SEQ:
                    if (c1 < c2 && 1 < c2) {
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                        resize(((size_t)_length-1)/c2+1);
                    }
                    else if (c2 < c1 && 1 < c1) {
                        resize(((size_t)_length-1)/c2+1);
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    }
                    else DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                case RNA_SEQ:
                    RNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                case AA_SEQ:
                    AA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                default:
                    break;
            }
        }
        else {
			auto t1 = _type & 0x000F, t2 = t & 0x000F;
			auto c1 = (_type >> 4) & 0x000F, c2 = (t >> 4) & 0x000F;
            switch (t1) {
                case DNA_SEQ:
                    if (c1 < c2 && 1 < c2) {
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                        resize(((size_t)_length-1)/c2+1);
                    }
                    else if (c2 < c1 && 1 < c1) {
                        resize(((size_t)_length-1)/c2+1);
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    }
                    else DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                case RNA_SEQ:
                    RNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                case AA_SEQ:
                    AA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                    break;
                default:
                    break;
            }
            
        }
    }
    _type = t;
}
*/

void slib::sbio::Sequence::summary() const {
    SPrint(String("*") * 65);
    SPrint(sstr::fill("Name:", ' ', 10, DIRECTION::TAIL), name);
    SPrint(sstr::fill("Type:", ' ', 10, DIRECTION::TAIL), sbio::sutil::seqTypeName(_type & 0xFF));
    SPrint(sstr::fill("Shape:", ' ', 10, DIRECTION::TAIL), (_type & sseq::CIRCULAR ? "Circular" : "Linear"));
    SPrint(sstr::fill("Length:", ' ', 10, DIRECTION::TAIL), _length, ((isDNA() || isRNA()) ? " bases" : (isAA() ? " residues" : " characters")));
    SPrint(sstr::fill("Size:", ' ', 10, DIRECTION::TAIL), size(), " bytes");
    if (mask.size()) SPrint(sstr::fill("Mask:", ' ', 10, DIRECTION::TAIL), mask);
    SPrint("Sequence:");
    if (_length <= 60) SPrint(SP * 2, raw());
    else SPrint(SP * 2, raw(0, 60), "...");
    SPrint(String("*") * 65);
}
slib::SObjPtr slib::sbio::Sequence::toObj() const {
    slib::SObjPtr obj = {
        D_("seq", raw()),
        D_("type", _type),
        D_("name", name),
        D_("mask", SArray()),
        D_("annotation", SArray()),
        D_("attribute", attribute)
    };
    SArray& masks = obj["mask"];
    sforeach(range, mask) {
        masks.add((((suinteger)range.begin) << 32) | (suinteger)range.end);
    }
    SArray& annots = obj["annotation"];
    sforeach(note, annotation) {
        SDictionary info = {
            D_("category", note.category),
            D_("type", note.type),
            D_("name", note.name),
            D_("pos", SArray()),
            D_("dir", note.dir),
            D_("note", note.note)
        };
        SArray& pos = info["pos"];
        if (note.count() == 1) {
            pos.add((((suinteger)note.range().begin) << 32) | (suinteger)note.range().end);
        }
        else {
            sfor(note.region()) {
                pos.add((((suinteger)$_.begin) << 32) | (suinteger)$_.end);
            }
        }
    }
    return obj;
}
slib::String slib::sbio::Sequence::toString(const char *format) const {
    slib::String str;
    IOStream strm;
    strm.setStrOStream(str);
    sbio::sio::writeSeq(strm, *this, format);
    return str;
}
inline std::ostream& slib::operator<<(std::ostream& os, const sbio::Sequence& seq) {
    return os << seq.raw();
}