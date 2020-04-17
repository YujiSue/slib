#include "sbioinfo/sbam.h"

using namespace slib;
using namespace slib::sbio;

sbam::voffset::voffset() : file_offset(0), block_offset(0) {}
sbam::voffset::voffset(int64_t fo, sushort bo) : file_offset(fo), block_offset(bo) {}
sbam::voffset::voffset(suinteger offset) : file_offset(offset>>16), block_offset(offset&0xFFFF) {}
sbam::voffset::voffset(const sbam::voffset &v) : file_offset(v.file_offset), block_offset(v.block_offset) {}
sbam::voffset::~voffset() {}

sbam::voffset & sbam::voffset::operator = (const sbam::voffset &v) {
    file_offset = v.file_offset; block_offset = v.block_offset;
    return (*this);
}
suinteger sbam::voffset::intOffset() { return (file_offset<<16)|block_offset; }
bool sbam::voffset::operator < (const sbam::voffset &v) const {
    if (file_offset < v.file_offset) return true;
    else if (file_offset == v.file_offset && block_offset < v.block_offset) return true;
    return false;
}
bool sbam::voffset::operator == (const sbam::voffset &v) const {
    return file_offset == v.file_offset && block_offset == v.block_offset;
}

sbam::header::header() : ref_num(0) {}
sbam::header::~header() {}
void sbam::header::set(int n) {
    ref_num = n;
    ref_length.resize(ref_num);
    ref_name.resize(ref_num);
}
void sbam::header::init() { text.clear(); }

sbam::readinfo::readinfo() {}
sbam::readinfo::readinfo(const sbam::readinfo &ri) {
    offset = ri.offset;
    read_length = ri.read_length;
    seq_length = ri.seq_length;
    pos = ri.pos;
    cigars = ri.cigars;
    bin = ri.bin;
    flag = ri.flag;
    mapq = ri.mapq;
    next_refid = ri.next_refid;
    next_pos = ri.next_pos;
    template_length = ri.template_length;
    sequence = ri.sequence;
    qual = ri.qual;
    name = ri.name;
    auxiliary = ri.auxiliary;
}
sbam::readinfo::~readinfo() {}

bool sbam::readinfo::headclip(int len) {
    return (cigars.first().option == scigar::SCLIP) &&
    len < cigars.first().length;
}
bool sbam::readinfo::tailclip(int len) {
    return (cigars.last().option == scigar::SCLIP) &&
    len < cigars.last().length;
}
void sbam::readinfo::init() {
    read_length = 0;
    seq_length = 0;
    pos.init();
    bin = 0;
    flag = 0;
    mapq = 0;
    next_refid = -1;
    next_pos = -1;
    template_length = 0;
    sequence.clear();
    cigars.clear();
    qual.clear();
    name = "";
    auxiliary  = "";
}
srange sbam::readinfo::readRange() {
    return srange(pos.begin, pos.begin+cigars.countRef()-1);
}

String sbam::readinfo::toString() {
    String str, seq(seq_length, '\0'), qstr(seq_length, '\0');
    sseq::ddecode2(sequence.ptr(), 0, seq_length, (subyte *)&seq[0]);
    sforin(i, 0, qual.size()) qstr[i] = qual[i]+33;
    str<<name<<String::TAB<<(int)flag<<String::TAB<<pos.idx<<String::TAB<<pos.begin+1<<String::TAB<<
    (int)mapq<<String::TAB<<cigars.toString()<<String::TAB<<(int)next_refid<<String::TAB<<(int)next_pos<<String::TAB<<
    seq_length<<String::TAB<<seq<<String::TAB<<qstr;
    return str;
}
bool sbam::readinfo::operator<(const sbam::readinfo &ri) const {
    return pos < ri.pos;
}
bool sbam::readinfo::operator==(const sbam::readinfo &ri) const {
    return pos == ri.pos && sequence == ri.sequence;
}

sbam::bai::bai() : ref_num(0) {}
sbam::bai::bai(const char *path) : sbam::bai() { load(path); }
sbam::bai::~bai() {};

void sbam::bai::setNum(int n) {
    ref_num = n;
    chunks.resize(ref_num);
    loffset.resize(ref_num);
    _bin_map.resize(ref_num);
    sforeach(chunks) E_.resize(MAX_BIN);
}

void sbam::bai::load(const char *path) {
    try {
        sio::SFile file(path, sio::READ);
        file.readBytes(_magic, 4);
        if (memcmp(BAI_MAGIC, _magic, 4) != 0)
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, _magic, "BAI");
        file.readInt(ref_num);
        if(ref_num < 0) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, "ref_num", ">0");
        setNum(ref_num);
        sforin(i, 0, ref_num) {
            auto &map = _bin_map[i];
            file.readInt(_bin_num);
            chunks[i].resize(_bin_num);
            sforin(j, 0, _bin_num) {
                file.readUInt(_bin);
                map.insert(std::pair<suint, suint>(_bin, j));
                file.readInt(_chunk_num);
                chunks[i][j].resize(_chunk_num);
                sforin(k, 0, _chunk_num) {
                    file.readUInteger(_beg);
                    file.readUInteger(_end);
                    chunks[i][map[_bin]][k] = voff_chunk(_beg, _end);
                }
            }
            file.readInt(_lin_num);
            loffset[i].resize(_lin_num);
            sforin(l, 0, _lin_num) {
                file.readUInteger(_beg);
                loffset[i][l] = sbam::voffset(_beg);
            }
        }
    } catch (sio::SIOException ie) {
		ie.print();
    }
}
//void SBamFile_index::save(const char *path){}

void sbam::bai::init() {
    sforin(i, 0, ref_num) {
        sforeach(chunks[i]) E_.clear();
        loffset[i].clear();
    }
}

inline int bgzf_uncompress(void *dest, void *ori, sushort &length) {
    z_stream zstrm;
    zstrm.zalloc = Z_NULL;
    zstrm.zfree = Z_NULL;
    zstrm.opaque = Z_NULL;
    zstrm.next_in = (unsigned char *)ori;
    zstrm.avail_in = length-25;
    zstrm.next_out = (unsigned char *)dest;
    zstrm.avail_out = BGZF_MAX_BLOCK_SIZE;
    inflateInit2(&zstrm, -15);
    int res = inflate(&zstrm, Z_FINISH);
    inflateEnd(&zstrm);
    return res;
}

sbam::bgzf_dat::bgzf_dat() {
    init();
}

sbam::bgzf_dat::~bgzf_dat() {
    free(ori_data);
    free(bam_data);
}

void sbam::bgzf_dat::init() {
    memset(_magic, 0, 16);
    result = 0;
    offset = 0;
    ori_length = 0;
    block_length = 0;
    ori_data = (subyte *)malloc(BGZF_MAX_BLOCK_SIZE);
    bam_data = (subyte *)malloc(BGZF_MAX_BLOCK_SIZE);
    current = bam_data;
}
void sbam::bgzf_dat::load(SBamFile *bam) {
    offset.file_offset = bam->offset();
    offset.block_offset = 0;
    ori_length = 0; block_length = 0; current = bam_data;
    if (bam->eof() || offset.file_offset == bam->size()) return;
    bam->readBytes(_magic, 16);
    if(memcmp(GZ_MAGIC, _magic, 16) != 0)
        throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, _magic, "BGZF");
    bam->readUShort(ori_length);
    bam->readBytes(ori_data, ori_length-21);
    bam->readInt(block_length);
    result = bgzf_uncompress(bam_data, ori_data, ori_length);
}
void sbam::bgzf_dat::setOffset(sushort boff) {
    offset.block_offset = boff;
    current = &bam_data[boff];
}
size_t sbam::bgzf_dat::left() {
    return block_length - offset.block_offset;
}
void sbam::bgzf_dat::read(void *dest, size_t size, size_t off) {
    memcpy((subyte *)dest+off, current, size);
    current += size;
    offset.block_offset += size;
}


SBamFile::SBamFile() : sio::SFile() {
    _data = new sbam::bgzf_dat();
    _buffer = new sbam::bgzf_dat();
    _threads.setSize(1);
}
SBamFile::~SBamFile() {
    if (_data) delete _data;
    if (_buffer) delete _buffer;
}
void SBamFile::_swapdat() {
    auto tmp = _data;
    _data = _buffer;
    _buffer = tmp;
    _threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
}
bool SBamFile::_readData(void *dest, size_t size, size_t off) {
    if (!_data->block_length) return false;
    if (size < _data->left()) _data->read(dest, size, off);
    else {
        size_t left = _data->left();
        _data->read(dest, left, off);
        off += left; size -= left;
        _threads.complete();
        _checkError();
        _swapdat();
        if(size) _readData(dest, size, off);
    }
    return true;
}
void SBamFile::_readHeader() {
    char s[4];
    _readData(s, 4);
    if(memcmp(BAM_MAGIC, s, 4) != 0)
        throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, s, "BAM");
    sint tmp;
    _readData(&tmp, 4);
    info.text.resize(tmp);
    _readData(&info.text[0], tmp);
    _readData(&tmp, 4);
    info.set(tmp);
    sforin(i, 0, info.ref_num) {
        _readData(&tmp, 4);
        info.ref_name[i].resize(tmp, '\0');
        _readData(&info.ref_name[i][0], tmp);
        info.ref_name[i].resize(tmp-1);
        _readData(&(info.ref_length[i]), 4);
    }
}
void SBamFile::_checkError() {
	if (_data->result == Z_STREAM_ERROR || _data->result == Z_DATA_ERROR || _data->result == Z_BUF_ERROR)
            throw SBioInfoException(ERR_INFO, SLIB_EXEC_ERROR, "inflate", std::to_string(_data->result).c_str());
}
void SBamFile::init() {
    if (isOpened()) close();
    index.init();
}
void SBamFile::load(const char *path) {
    sio::SFile::open(path);
    _data->load(this);
    _checkError();
    _threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
    info.init();
    loadIndex(String(path)+".bai");
    _readHeader();
}
void SBamFile::loadIndex(const char *path) {
    index.init();
    if (sio::SFile(path).exist()) index.load(path);
}
bool SBamFile::hasIndex() const { return 0 < index.ref_num; }

sbam::voffset SBamFile::voff() const {
    return _data->offset;
}
void SBamFile::setVOff(const sbam::voffset &off) {
    if(eof()) clear();
    _threads.complete();
    if (off.file_offset != _data->offset.file_offset) {
        if (off.file_offset == _buffer->offset.file_offset) _swapdat();
        else {
            sio::SFile::setOffset(off.file_offset);
            _data->load(this);
            _checkError();
            _threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
        }
    }
    _data->setOffset(off.block_offset);
}

//void SBamFile::sort() {}
//void SBamFile::CREATEIndex() {}
inline void lenCheck(sint &len) {
    if (len < 1) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(len).c_str(), READ_SIZE_ERR_MSG);
}
bool SBamFile::next(sbam::readinfo *ri) {
    if (!ri) ri = &read;
    ri->init();
    ri->offset = _data->offset;
    sint tmp, len;
    if(!_readData(&len, 4)) return false;
    ri->read_length = len;
    if (!ri->read_length) return false;
    _readData(&ri->pos.idx, 4); len-=4; lenCheck(len);
    _readData(&ri->pos.begin, 4); len-=4; lenCheck(len);
    if(ri->pos.idx < -1 || ri->pos.begin < -1)
        throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "position", "Read info");
    _readData(&tmp, 4); len-=4; lenCheck(len);
    ri->name.resize(tmp&0xff);
    ri->mapq = (tmp>>8)&0xff;
    ri->bin = (tmp>>16)&0xffff;
    _readData(&tmp, 4); len-=4; lenCheck(len);
    ri->flag = (tmp>>16)&0xffff;
    if(ri->flag&0x10) ri->pos.dir = true;
    tmp = tmp&0xffff;
    ri->cigars.resize(tmp, false);
    _readData(&ri->seq_length, 4); len-=4; lenCheck(len);
    ri->sequence.resize((ri->seq_length+1)/2);
    ri->qual.resize(ri->seq_length);
    _readData(&ri->next_refid, 4); len-=4; lenCheck(len);
    _readData(&ri->next_pos, 4); len-=4; lenCheck(len);
    _readData(&ri->template_length, 4); len-=4; lenCheck(len);
    _readData(&ri->name[0], ri->name.size());
    len-=ri->name.size(); lenCheck(len);
    sforeach(ri->cigars) {
        _readData(&tmp, 4); len-=4; lenCheck(len);
        E_ = tmp;
    }
    _readData(ri->sequence.ptr(), ri->sequence.size());
    len-=ri->sequence.size(); lenCheck(len);
    _readData(&ri->qual[0], ri->qual.size());
    len-=ri->qual.size(); lenCheck(len);
    ri->auxiliary.resize(len);
    _readData(&ri->auxiliary[0], len);
    return true;
}
void SBamFile::reads(sbam::read_vec &list, int idx, const srange &rng) {
    list.clear();
    if(!hasIndex()) return;
    sizearray bins;
    SBIUtil::getBins(bins, rng);
    if (bins.empty()) return;
    sforeach(bins) {
        auto &chunks = index.chunks[idx][index._bin_map[idx][E_]];
        if (chunks.empty()) continue;
        sforeach_(cit, chunks) {
            setVOff(cit->begin);
            do {
                if(!next()) break;
                if(rng.overlap(read.readRange())) list.add(read);
            }
            while (_data->offset < cit->end);
        }
    }
}
void SBamFile::reads(sbam::read_vec &list, int idx, const sregion &reg) {
    list.clear();
    if(!hasIndex()) return;
    sizearray bins;
    SBIUtil::getBins(bins, reg);
    if (bins.empty()) return;
    sforeach(bins) {
        auto &chunks = index.chunks[idx][index._bin_map[idx][E_]];
        if (chunks.empty()) continue;
        sforeach_(cit, chunks) {
            setVOff(cit->begin);
            do {
                if(!next()) break;
                if(reg.overlap(read.readRange())) list.add(read);
            }
            while (_data->offset < cit->end);
        }
    }
}