#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

SBioSeq::SBioSeq() : _type(0), _length(0), _dec(nullptr), _enc(nullptr), ubytearray() {}
SBioSeq::SBioSeq(sushort t, const char *name, const char *seq) : SBioSeq() {
    _type = t;
    _init();
    if (name) _name = name;
    if (seq) setSeq(seq);
}
SBioSeq::SBioSeq(const SBioSeq &seq) {
    _type = seq._type;
    _name = seq._name;
    _mask = seq._mask;
    _dec = seq._dec;
    _enc = seq._enc;
    setLength(seq.length());
    ubytearray::copy(seq.ptr(), seq.size());
}
SBioSeq::~SBioSeq() {}

SBioSeq &SBioSeq::operator=(const SBioSeq &seq) {
    _type = seq._type;
    _length = seq._length;
    _name = seq._name;
    _mask = seq._mask;
    _annotation = seq._annotation;
    _dec = seq._dec;
    _enc = seq._enc;
	ubytearray::copy(seq.ptr(), seq.size());
    return *this;
}
void SBioSeq::load(sushort t, const char *path) {
    _type = t;
    if (_type) _init();
    SFile file(path, sio::READ);
	auto ftype = SBSeqIO::fileType(file);
	switch (ftype) {
	case sbio::FASTA:
		SBSeqIO::loadFASTA(t, file, this);
		break;
	case sbio::GENBANK:
		SBSeqIO::loadGBK(file, this);
		break;
	case sbio::ABI:
		SBSeqIO::loadABI(file, this);
		break;
	default:
		SBSeqIO::loadTXT(file, this);
		break;
	}
}
void SBioSeq::save(const char *path) {
    SFile file(path, sio::WRITE);
    auto ext = file.extension();
    if (ext == "fa" || ext == "fasta") SBSeqIO::saveFASTA(file, this);
    else if (ext == "gb" || ext == "gbk") SBSeqIO::saveGBK(file, this);
    else SBSeqIO::saveTXT(file, this);
}

void SBioSeq::_init() {
    int t = seqtype(), c = compress();
    switch (t) {
        case DNA_SEQ:
            _dec = DNA_CONVERTER[c][0];
            _enc = DNA_CONVERTER[0][c];
            break;
        case RNA_SEQ:
            _dec = RNA_CONVERTER[c][0];
            _enc = RNA_CONVERTER[0][c];
            break;
        case AA_SEQ:
            _dec = AA_CONVERTER[c][0];
            _enc = AA_CONVERTER[0][c];
            break;
        default:
            _dec = DNA_CONVERTER[0][0];
            _enc = DNA_CONVERTER[0][0];
            break;
    }
}

bool SBioSeq::isCircular() const { return _type & CIRCULAR; }
bool SBioSeq::isMasked() const { return _type & MASKED; }
bool SBioSeq::isScored() const { return _type & SCORED; }
bool SBioSeq::isAnnotated() const { return _type & ANNOTATED; }
bool SBioSeq::isAttributed() const { return _type & ATTRIBUTED; }
sushort SBioSeq::type() const { return _type; }
subyte SBioSeq::seqtype() const { return _type&0x0F; }
subyte SBioSeq::compress() const { return (_type>>4)&0x0F; }

const sint &SBioSeq::length() const { return _length; }
const String &SBioSeq::name() const { return _name; }
const sregion &SBioSeq::mask() const { return _mask; }

const sdict& SBioSeq::attribute() const { return _attribute; }

void SBioSeq::setSeq(const char *seq) {
    setLength(strlen(seq));
    encode(seq);
}
void SBioSeq::setName(const char *name) { _name = name; }
void SBioSeq::setLength(const size_t &l, bool alias) {
    _length = (int)l;
    if (!alias) {
        auto c = compress();
        if (l && 1 < c) ubytearray::resize((l-1)/c+1);
        else ubytearray::resize(l);
    }
}
void SBioSeq::addMask(const srange &range) { _mask.add(range); }

void SBioSeq::addAttribute(const char* key, sobj val) { 
	_attribute.set(key, val);
	if (!isAttributed()) _type |= ATTRIBUTED;
}
void SBioSeq::removeMask(const srange &range) { _mask.exclude(range); }

void SBioSeq::removeAttribute(const char* key) { 
	_attribute.remove(key); 
	if (_attribute.empty()) _type -= ATTRIBUTED;
}

void SBioSeq::clearAll() {
    ubytearray::clear();
    _type = 0;
    _length = 0;
    _name.clear();
    _mask.clear();
    _annotation.clear();
	_dec = DNA_CONVERTER[0][0];
	_enc = DNA_CONVERTER[0][0];
}

void SBioSeq::encode(const char *seq, size_t off, size_t len, bool dir) {
    if (len == -1 || _length < off+len) len = _length-off;
    _enc((const subyte *)seq, off, len==-1?(_length-off):len, ubytearray::ptr());
    if (dir) complement();
}
void SBioSeq::decode(char *seq, size_t off, size_t len, bool dir) {
    _enc(ubytearray::ptr(), off, len==-1?(_length-off):len, (subyte *)seq);
    if (dir) {
        switch (seqtype()) {
            case DNA_SEQ:
                sseq::dcomp(seq);
                break;
            case RNA_SEQ:
                sseq::rcomp(seq);
                break;
            default:
                break;
        }
    }
    if (isMasked() && compress() == 4) {
        auto mask = maskChar(_type);
        sforeach(_mask) {
            sforin(i, E_.begin, E_.end+1) seq[i] = mask;
        }
    }
}
void SBioSeq::recode(subyte c, ubytearray &seq, size_t off, size_t len, bool dir) {
    if (off < _length) {
        if (len == -1 || _length < off+len) len = _length-off;
        c>>=4; seq.resize((len-1)/(c?c:1)+1);
        switch (seqtype()) {
            case DNA_SEQ:
                DNA_CONVERTER[compress()][c](ptr(), off, len, seq.ptr());
                break;
            case RNA_SEQ:
                RNA_CONVERTER[compress()][c](ptr(), off, len, seq.ptr());
                break;
            case AA_SEQ:
                AA_CONVERTER[compress()][c](ptr(), off, len, seq.ptr());
                break;
            default:
                break;
        }
    }
}
void SBioSeq::convert(sushort t) {
    if ((_type&0x00FF) != (t&0x00FF)) {
        if ((_type&0x000F) == (t&0x000F)) {
            auto c1 = (_type>>4)&0x000F, c2 = (t>>4)&0x000F;
            switch (seqtype()) {
                case DNA_SEQ:
                    if (c1 < c2 && 1 < c2) {
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                        resize((_length-1)/c2+1);
                    }
                    else if (c2 < c1 && 1 < c1) {
                        resize((_length-1)/c2+1);
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
            auto t1 = _type&0x000F, t2 = t&0x000F;
            auto c1 = (_type>>4)&0x000F, c2 = (t>>4)&0x000F;
            
            switch (t1) {
                case DNA_SEQ:
                    if (c1 < c2 && 1 < c2) {
                        DNA_CONVERTER[c1][c2](ptr(), 0, _length, ptr());
                        resize((_length-1)/c2+1);
                    }
                    else if (c2 < c1 && 1 < c1) {
                        resize((_length-1)/c2+1);
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
SBioSeq SBioSeq::subseq(const sbpos &pos) {
    if (compress()) return SBioSeq(COMPRESS1|seqtype(), _name+"_sub", raw(pos));
    else return SBioSeq(seqtype(), _name+"_sub", raw(pos));
}
SBioSeq SBioSeq::subseq(size_t off, size_t len, bool dir) {
    if (compress()) return SBioSeq(COMPRESS1|seqtype(), _name+"_sub", raw(off, len, dir));
    else return SBioSeq(seqtype(), _name+"_sub", raw(off, len, dir));
}
String SBioSeq::raw(const sbpos &pos) const {
    return raw(pos.begin, pos.length()+1, pos.dir);
}
String SBioSeq::raw(size_t off, size_t len, bool dir) const {
    if (len == -1 || _length < off+len) len = _length-off;
    String seq(len+1, '\0'); seq.resize(len);
    _dec(ubytearray::ptr(), off, len, (subyte *)seq.ptr());
    if (isMasked()) {
        auto mask = maskChar(_type);
        srange range((int)off, (int)(off+len-1));
        auto region = _mask.subregion(range);
        if (!region.empty()) {
            sforeach(region) {
                sforin(i, E_.begin-off, E_.end-off+1) seq[i] = mask;
            }
        }
    }
    return seq;
}
void SBioSeq::circulate() { _type |= CIRCULAR; }
void SBioSeq::complement() {
    switch (seqtype()) {
        case DNA_SEQ:
            if (1 < compress()) {
                auto t = _type;
                _type = COMPRESS1|(_type&0xFF0F);
                convert(_type);
                sseq::dcompi(*this);
                convert(t);
            }
            else sseq::dcompi(*this);
            break;
        case RNA_SEQ:
            sseq::rcompi(*this);
            break;
        default:
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "DNA or RNA");
            break;
    }
}
void SBioSeq::splice(const sregion &region) {
    auto t = _type;
    switch (type()) {
        case DNA_SEQ:
            if (1 < compress()) {
                auto t = _type;
                _type = COMPRESS1|seqtype();
                convert(_type);
            }
            else {
                /**/
            }
            break;
        case RNA_SEQ:
            
            break;
        default:
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "DNA or RNA");
            break;
    }
}
void SBioSeq::transcribe() {
    if (_type & DNA_SEQ) {
        convert((_type&0xFF0F)|COMPRESS1);
        sseq::dtransi(ubytearray::ptr(), 0, _length, ubytearray::ptr());
        _type = RNA_SEQ1;
        _init();
    }
    else throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "DNA");
}
void SBioSeq::rtranscribe() {
    if (_type & RNA_SEQ) {
        convert((_type&0xFF0F)|COMPRESS1);
        sseq::rtransi(ubytearray::ptr(), 0, _length, ubytearray::ptr());
        _type = DNA_SEQ1;
        _init();
    }
    else throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "RNA");
    
}
void SBioSeq::translate(const CODON_TABLE &code) {
    if (_type & RNA_SEQ) {
        convert((_type&0xFF0F)|COMPRESS1);
        sseq::atrans(ubytearray::ptr(), 0, _length, ubytearray::ptr(), code);
        setLength(_length/3);
        _type = AA_SEQ1;
        _init();
    }
    else throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "RNA");
}
SBioSeq SBioSeq::transcript() {
    if (_type & DNA_SEQ) {
        SBioSeq seq = *this;
        seq.transcribe();
        return seq;
    }
    throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "DNA");
}
SBioSeq SBioSeq::rtranscript() {
    if (_type & RNA_SEQ) {
        SBioSeq seq = *this;
        seq.rtranscribe();
        return seq;
    }
    throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "RNA");
}
SBioSeq SBioSeq::translated(const CODON_TABLE &code) {
    if (_type & RNA_SEQ) {
        SBioSeq seq = *this;
        seq.translate(code);
        return seq;
    }
    throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "sequence", "RNA");
}