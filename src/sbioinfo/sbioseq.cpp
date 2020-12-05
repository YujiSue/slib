#include "sbioinfo/sbioseq.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::sbio;

SBioSeq::SBioSeq() : _type(0), _length(0), _dec(nullptr), _enc(nullptr), ubytearray() {}
SBioSeq::SBioSeq(const char* seq) : SBioSeq() { setSeq(seq); }
SBioSeq::SBioSeq(sushort t, const char *n, const char *seq) : SBioSeq() {
    _type = t;
    _init();
    if (n) name = n;
    if (seq) setSeq(seq);
}
SBioSeq::SBioSeq(const SBioSeq &seq) {
    _type = seq._type;
	_length = seq._length;
    _mask = seq._mask;
    _dec = seq._dec;
    _enc = seq._enc;
	name = seq.name;
	annotation = seq.annotation;
	attribute = seq.attribute;
	ubytearray::copy(seq.ptr(), seq.size());
}
SBioSeq::~SBioSeq() {}

SBioSeq &SBioSeq::operator=(const SBioSeq &seq) {
	_type = seq._type;
	_length = seq._length;
	_mask = seq._mask;
	_dec = seq._dec;
	_enc = seq._enc;
	name = seq.name;
	annotation = seq.annotation;
	attribute = seq.attribute;
	ubytearray::copy(seq.ptr(), seq.size());
    return *this;
}
void SBioSeq::load(sushort t, const char *path) {
    _type = t;
    if (_type) _init();
    SFile file(path, sio::READ);
	auto ftype = SBSeqIO::fileType(file);
	file.setOffset(0);
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

sint SBioSeq::length() const { return _length; }
const sregion &SBioSeq::mask() const { return _mask; }
void SBioSeq::setSeq(const char *seq) {
    setLength(strlen(seq));
    encode(seq);
}
void SBioSeq::setLength(const size_t &l, bool alias) {
    _length = (int)l;
    if (!alias) {
        auto c = compress();
        if (l && 1 < c) ubytearray::resize((l-1)/c+1);
        else ubytearray::resize(l);
    }
}
void SBioSeq::addMask(const srange &range) { _mask.add(range); }
void SBioSeq::removeMask(const srange &range) { _mask.exclude(range); }

void SBioSeq::clearAll() {
    ubytearray::clear();
    _type = 0;
    _length = 0;
    _mask.clear();
	_dec = DNA_CONVERTER[0][0];
	_enc = DNA_CONVERTER[0][0];
	name.clear();
	annotation.clear();
	attribute.clear();
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
        auto mask = sseq::maskChar(_type);
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
SBioSeq SBioSeq::subseq(const sbpos &pos) {
    if (compress()) return SBioSeq(COMPRESS1|seqtype(), name+"_sub", raw(pos));
    else return SBioSeq(seqtype(), name+"_sub", raw(pos));
}
SBioSeq SBioSeq::subseq(size_t off, size_t len, bool dir) {
    if (compress()) return SBioSeq(COMPRESS1|seqtype(), name+"_sub", raw(off, len, dir));
    else return SBioSeq(seqtype(), name+"_sub", raw(off, len, dir));
}
String SBioSeq::raw(const sbpos &pos) const {
	return raw((size_t)pos.begin, (size_t)pos.length(true), pos.dir);
}
String SBioSeq::raw(size_t off, size_t len, bool dir) const {
    if (len == -1 || _length < off+len) len = _length-off;
    String seq(len+1, '\0'); seq.resize(len);
    _dec(ubytearray::ptr(), off, len, (subyte *)seq.ptr());
    if (isMasked()) {
        auto mask = sseq::maskChar(_type);
        srange range((int)off, (int)(off+len-1));
        auto region = _mask.subregion(range);
        if (!region.empty()) {
            sforeach(region) {
                sforin(i, E_.begin-off, E_.end-off+1) seq[i] = mask;
            }
        }
    }
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