#include "sbioinfo/sbam.h"

using namespace slib;
using namespace slib::sbio;

inline void lenCheck(sint& len) {
	if (len < 1) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(len), READ_SIZE_ERR_MSG);
}
sbam::voffset::voffset() : file_offset(0), block_offset(0) {}
sbam::voffset::voffset(sinteger fo, sushort bo) : file_offset(fo), block_offset(bo) {}
sbam::voffset::voffset(suinteger offset) : file_offset(offset>>16), block_offset(offset&0xFFFF) {}
sbam::voffset::voffset(const sbam::voffset &v) : file_offset(v.file_offset), block_offset(v.block_offset) {}
sbam::voffset::~voffset() {}
sbam::voffset & sbam::voffset::operator = (const sbam::voffset &v) {
    file_offset = v.file_offset; block_offset = v.block_offset; return (*this);
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
String sbam::header::toString() const{
	String str;
	str << String("=") * 60 << NEW_LINE;
	str << SPACE * 5 << "Index" << SPACE * 5 << "| Name" << SPACE * 16 << "| Length (bp)" << SPACE * 6 << NEW_LINE;
	sforin(r, 0, ref_num) {
		str << SPACE * 5 << String(r + 1).filled(10, ' ') << "| " << ref_name[r].filled(20, ' ') << "| " << ref_length[r] << NEW_LINE;
	}
	str << String("-") * 60 << NEW_LINE;
	str << text << NEW_LINE;
	str << String("=") * 60 << NEW_LINE;
	return str;
}
void sbam::header::init() { 
	ref_num = 0;
	ref_length.clear();
	ref_name.clear();
	text.clear(); 
}
sbam::readinfo::readinfo() { init(); }
sbam::readinfo::readinfo(ubytearray& dat) { interpret(dat); }
sbam::readinfo::readinfo(const sbam::readinfo& ri) {
	pos = ri.pos;
	cigars = ri.cigars;
	bin = ri.bin;
	flag = ri.flag;
	mapq = ri.mapq;
	next = ri.next;
	tmplen = ri.tmplen;
	seqlen = ri.seqlen;
	sequence = ri.sequence;
	qual = ri.qual;
	name = ri.name;
	auxiliary = ri.auxiliary;
}
sbam::readinfo::~readinfo() {}
sint sbam::readinfo::getRef(ubytearray& data) {
	if(data.size() < 4) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	return *reinterpret_cast<sint*>(data.ptr());
}
sint sbam::readinfo::getPos(ubytearray& data) {
	if (data.size() < 8) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	return *reinterpret_cast<sint*>(data.ptr(4));
}
sint sbam::readinfo::getLen(ubytearray& data) {
	if (data.size() < 20) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	return *reinterpret_cast<sint*>(data.ptr(16));
}
sushort sbam::readinfo::getBin(ubytearray& data) {
	if (data.size() < 12) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	sint tmp =  *reinterpret_cast<sint*>(data.ptr(8));
	return (tmp >> 16) & 0xFFFF;
}
sushort sbam::readinfo::getFlag(ubytearray& data) {
	if (data.size() < 16) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	sint tmp = *reinterpret_cast<sint*>(data.ptr(12));
	return (tmp >> 16) & 0xFFFF;
}
subyte sbam::readinfo::getMapQ(ubytearray& data) {
	if (data.size() < 12) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, String(4), READ_SIZE_ERR_MSG);
	sint tmp = *reinterpret_cast<sint*>(data.ptr(8));
	return (tmp >> 8) & 0xFF;
}
void sbam::readinfo::init() {
	pos.init();
	cigars.clear();
	bin = 0;
	flag = 0;
	mapq = 0;
	next.init();
	tmplen = 0;
	seqlen = 0;
    sequence.clear();
    qual.clear();
    name.clear();
    auxiliary.clear();
}
void sbam::readinfo::interpret(ubytearray& data, bool aux) {
	init();
	sint length = (sint)data.size(), tmp = 0;
	auto p = data.ptr();
	pos.idx = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	pos.begin = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	if (pos.idx < -1 || pos.begin < -1)
		throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, "position", "Read info");
	tmp = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	name.resize(tmp & 0xFF);
	mapq = (tmp >> 8) & 0xFF;
	bin = (tmp >> 16) & 0xFFFF;
	tmp = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	cigars.resize(tmp & 0xFFFF, false);
	flag = (tmp >> 16) & 0xFFFF;
	if (flag & sbam::COMPLEMET_READ) pos.dir = true;
	seqlen = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	sequence.resize((seqlen + 1) / 2);
	qual.resize(seqlen);
	next.idx = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	next.begin = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	tmplen = *reinterpret_cast<sint*>(p);
	p += 4; length -= 4; lenCheck(length);
	memcpy(&name[0], p, name.size());
	p += name.size(); length -= (sint)name.size(); lenCheck(length);
	if (name.last() == '\0') name.resize(name.size() - 1);
	sforeach(cigars) {
		E_ = *reinterpret_cast<sint*>(p);
		p += 4; length -= 4; lenCheck(length);
	}
	pos.end = pos.begin + (sint)cigars.countRef() - 1;
	memcpy(sequence.ptr(), p, sequence.size());
	p += sequence.size(); length -= (sint)sequence.size(); lenCheck(length);
	memcpy(qual.ptr(), p, qual.size());
	p += qual.size(); length -= (sint)qual.size(); lenCheck(length);
	if (aux) {
		auxiliary.resize(length);
		memcpy(auxiliary.ptr(), p, auxiliary.size());
	}
}
srange sbam::readinfo::range() {
	return srange(pos.begin, pos.begin + (sint)cigars.countRef() - 1);
}
String sbam::readinfo::decode() {
	String seq(seqlen, '\0');
	sseq::ddecode2(sequence.ptr(), 0, seqlen, (subyte*)&seq[0]);
	return seq;
}
String sbam::readinfo::toString() {
	String str, seq(seqlen, '\0'), qstr(seqlen, '\0');
	sseq::ddecode2(sequence.ptr(), 0, seqlen, (subyte*)&seq[0]);
	sforin(i, 0, qual.size()) qstr[i] = qual[i] + 33;
	str << name << TAB << String(flag) << TAB << pos.idx << TAB << pos.begin + 1 << TAB <<
		(int)mapq << TAB << cigars.toString() << TAB << next.idx << TAB << next.begin << TAB <<
		seqlen << TAB << seq << TAB << qstr;
	return str;
}
bool sbam::readinfo::operator<(const sbam::readinfo &ri) const { return pos < ri.pos; }
bool sbam::readinfo::operator==(const sbam::readinfo &ri) const { return pos == ri.pos && sequence == ri.sequence; }
sbam::bai::bai() : ref_num(0) {}
sbam::bai::bai(const char *path) : sbam::bai() { load(path); }
sbam::bai::~bai() {};
void sbam::bai::setNum(int n) {
    ref_num = n;
    chunks.resize(ref_num);
    loffset.resize(ref_num);
    bin_map.resize(ref_num);
}
void sbam::bai::load(const char *path) {
    try {
		char magic[4];
		sint bnum, cnum, lnum;
		suint bin;
		suinteger off;
        sio::SFile file(path, sio::READ);
        file.readBytes(magic, 4);
        if (memcmp(BAI_MAGIC, magic, 4) != 0)
            throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, magic, "BAI");
        file.readInt(ref_num);
        if(ref_num < 1) throw SBioInfoException(ERR_INFO, SLIB_RANGE_ERROR, "ref_num", "> 0");
        setNum(ref_num);
        sforin(i, 0, ref_num) {
            auto &map = bin_map[i];
            file.readInt(bnum);
            chunks[i].resize(bnum);
            sforin(j, 0, bnum) {
                file.readUInt(bin);
                map.insert(std::pair<suint, suint>(bin, j));
                file.readInt(cnum);
                chunks[i][j].resize(cnum);
                sforin(k, 0, cnum) {
					file.readUInteger(off); chunks[i][map[bin]][k].begin = off;
                    file.readUInteger(off); chunks[i][map[bin]][k].end = off;
                }
            }
            file.readInt(lnum);
            loffset[i].resize(lnum);
            sforin(l, 0, lnum) {
                file.readUInteger(off);
                loffset[i][l] = off;
            }
        }
    } catch (sio::SIOException ie) {
		ie.print();
    }
}
//void SBam_index::save(const char *path){}
void sbam::bai::init() {
    sforin(i, 0, ref_num) {
        sforeach(chunks[i]) E_.clear();
        loffset[i].clear();
    }
}
sbam::bgzf_dat::bgzf_dat() { init(); }
sbam::bgzf_dat::~bgzf_dat() {}
void sbam::bgzf_dat::init() {
    memset(_magic, 0, 16);
    result = 0;
    offset = 0;
    ori_length = 0;
    block_length = 0;
	ori_data.reserve(BGZF_MAX_BLOCK_SIZE + 1);
	bam_data.reserve(BGZF_MAX_BLOCK_SIZE + 1);
    current = bam_data.ptr();
}
void sbam::bgzf_dat::load(SBam* bam) {
    offset.file_offset = bam->_file.offset();
    offset.block_offset = 0;
    ori_length = 0; block_length = 0; current = bam_data.ptr();
    if (bam->_file.eof() || offset.file_offset == bam->_file.size()) return;
    bam->_file.readBytes(_magic, 16);
    if(memcmp(GZ_MAGIC, _magic, 16) != 0)
        throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, _magic, "BGZF");
    bam->_file.readUShort(ori_length);
	ori_data.resize(ori_length - 25);
	bam->_file.readBytes(ori_data.ptr(), ori_length - 25);
	bam->_file.readInt(block_length);
    bam->_file.readInt(block_length);
	SCode::expandTo(ori_data, bam_data, BGZF_MAX_BLOCK_SIZE, -15, Z_FINISH);
}
void sbam::bgzf_dat::setOffset(sushort boff) {
    offset.block_offset = boff;
    current = &bam_data[boff];
}
size_t sbam::bgzf_dat::left() {
    return (size_t)block_length - offset.block_offset;
}
void sbam::bgzf_dat::read(void *dest, size_t size, size_t off) {
    memcpy((subyte *)dest+off, current, size);
    current += size;
    offset.block_offset += (sushort)size;
}

SBam::SBam() {
    _data = new sbam::bgzf_dat();
    _buffer = new sbam::bgzf_dat();
    _threads.setSize(1);
}
SBam::~SBam() {
	close();
    if (_data) delete _data;
    if (_buffer) delete _buffer;
}
void SBam::_swapdat() {
    auto tmp = _data;
    _data = _buffer;
    _buffer = tmp;
    _threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
}
bool SBam::_readData(void *dest, size_t size, size_t off) {
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
void SBam::_readHeader() {
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
		info.ref_name[i].resize((size_t)tmp, '\0');
		_readData(&info.ref_name[i][0], tmp);
		info.ref_name[i].resize((size_t)tmp - 1);
		_readData(&(info.ref_length[i]), 4);
	}
}
void SBam::_checkError() {
	if (_data->result == Z_STREAM_ERROR || _data->result == Z_DATA_ERROR || _data->result == Z_BUF_ERROR)
            throw SBioInfoException(ERR_INFO, SLIB_EXEC_ERROR, "inflate", std::to_string(_data->result).c_str());
}
void SBam::init() {
    if (_file.isOpened()) _file.close();
    index.init();
}
void SBam::open(const char* path) {
	if (_file.isOpened()) _file.close();
	_file.open(path);
	_data->load(this);
	_checkError();
	_threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
}
void SBam::load(const char *path) {
	open(path);
    info.init();
    loadIndex(String(path)+".bai");
    _readHeader();
}
void SBam::close() {
	if (_file.isOpened()) _file.close();
}
suinteger SBam::size() const { return _file.size(); }
suinteger SBam::offset() { return _file.offset(); }
String SBam::path() const { return _file.path(); }
void SBam::loadIndex(const char *path) {
    index.init();
    if (sio::SFile(path).exist()) index.load(path);
}
bool SBam::hasIndex() const { return 0 < index.ref_num; }
sbam::voffset SBam::voff() const { return _data->offset; }
void SBam::setVOff(const sbam::voffset &off) {
    if(_file.eof()) _file.clear();
    _threads.complete();
    if (off.file_offset != _data->offset.file_offset) {
        if (off.file_offset == _buffer->offset.file_offset) _swapdat();
        else {
			_file.setOffset(off.file_offset);
            _data->load(this);
            _checkError();
            _threads.addTask(&sbam::bgzf_dat::load, _buffer, this);
        }
    }
    _data->setOffset(off.block_offset);
}
//void SBam::sort() {}
//void SBam::createIndex() {}
ubytearray* SBam::next(ubytearray* dat) {
	if (!dat) dat = &read; dat->clear();
	sint len;
	if (!_readData(&len, 4)) return nullptr;
	dat->resize(len);
	if (!_readData(dat->ptr(), len)) return nullptr;
	return dat;
}
void SBam::getReads(sbam::read_array& array, const sbpos& pos) {
	array.clear();
	if (!hasIndex()) return;
	ushortarray bins;
	sbiutil::getBins(bins, pos);
	if (bins.empty()) return;
	sforeach(bins) {
		auto& chunks = index.chunks[pos.idx][index.bin_map[pos.idx][(suint)E_]];
		if (chunks.empty()) continue;
		sforeach_(cit, chunks) {
			setVOff(cit->begin);
			do {
				if (!next()) break;
				sbam::readinfo ri(read);
				if(E_ == ri.bin && pos.overlap(ri.range())) array.add(ri);
			} while (_data->offset < cit->end);
		}
	}
}
void SBam::getReads(sbam::read_array& array, sint idx, const sregion& region) {
	array.clear();
	if (!hasIndex()) return;
	ushortarray bins;
	sbiutil::getBins(bins, region);
	if (bins.empty()) return;
	sforeach(bins) {
		auto& chunks = index.chunks[idx][index.bin_map[idx][(suint)E_]];
		if (chunks.empty()) continue;
		sforeach_(cit, chunks) {
			setVOff(cit->begin);
			do {
				if (!next()) break;
				sbam::readinfo ri(read);
				if (E_ == ri.bin && region.overlap(ri.range())) array.add(ri);
			} while (_data->offset < cit->end);
		}
	}
}

/*
SBamData::SBamData() { init(); }
SBamData::SBamData(const SBamData& dat) : SBamData() {
	_bam = dat._bam;
	idx = dat.idx;
	count = dat.count;
	offset = dat.offset;
	last = dat.last;
	length = dat.length;
	raw = dat.raw;
	buffer[0] = dat.buffer[0];
	buffer[1] = dat.buffer[1];
	readinfo = dat.readinfo;
	current = dat.current;
	rawptr = dat.rawptr;
	curptr = dat.curptr;
}
SBamData::~SBamData() {

}
void SBamData::init() {
	memset(_magic, 0, 16);
	_bam = nullptr;
	_thread.setSize(1);
	idx = 0;
	result = 0;
	buffer[0].reserve(BGZF_MAX_BLOCK_SIZE);
	buffer[1].reserve(BGZF_MAX_BLOCK_SIZE);
	current = buffer;
	curptr = current->ptr();
	resize(2);
}
void SBamData::resize(sint n) {
	count = n;
	raw.reserve((size_t)BGZF_MAX_BLOCK_SIZE * n);
	rawptr = raw.ptr();
	length.resize(n);
}
void SBamData::setBam(SBamFile* f) { _bam = f; }

void SBamData::load() { 
	if (!_bam) throw SBioInfoException(ERR_INFO, SLIB_NULL_ERROR, "_bam");
	_bam->lock();
	raw.clear();
	rawptr = raw.ptr();
	length.reset(0);
	offset = sbam::voffset(_bam->offset(), 0);
	sforin(i, 0, count) {
		result = _bam->readBytes(&length[i], 4);
		if (result) throw SBioInfoException(ERR_INFO, SLIB_READ_ERROR);
		raw.resize(raw.size() + length[i]);
		result = _bam->readBytes(rawptr, length[i]);
		rawptr += length[i];
	}
	last = sbam::voffset(_bam->offset(), 0);
	_bam->unlock();
}
void SBamData::copy(void* p, size_t s) {
	if (offset.block_offset + s < current->size()) {
		memcpy(p, (const void *)curptr, s);
		curptr += s; offset.block_offset += s;
	}
	else {
		auto left = current->size() - offset.block_offset;
		memcpy(p, (const void*)curptr, left);
		p = (void*)((unsigned char*)p + left);
		swap();
		memcpy(p, (const void*)curptr, s - left);
	}
}
ubytearray* SBamData::next() {
	sint len = 0;
	copy(&len, 4);
	if (!len) return nullptr;
	readinfo.resize(len);
	copy(readinfo.ptr(), len);
	return &readinfo;
}
void SBamData::swap() {


	if (idx == count - 1) update();
	else {
		offset.file_offset += length[idx];
		offset.block_offset = 0;
		++idx;
		_thread.complete();
		if (current == &buffer[1]) {
			current = &buffer[0];
			if (idx < count - 1) _thread.addTask()
		}
		else {
			current = &buffer[1];

		}
		curptr = 
	}
}
SBamFile::SBamFile() {
	_single = true;
	_store.resize(1);
	_data->setBam(this);
	_data = &_store[0];
}
SBamFile::SBamFile(sngs_param* par) { setParam(par); }
SBamFile::~SBamFile() {
	close();
}
void SBamFile::_readHeader() {
	char s[4];
	_data->copy(s, 4);
	if (memcmp(BAM_MAGIC, s, 4) != 0)
		throw SBioInfoException(ERR_INFO, SLIB_FORMAT_ERROR, s, "BAM");
	sint tmp;
	_data->copy(&tmp, 4);
	info.text.resize(tmp);
	_data->copy(&info.text[0], tmp);
	_data->copy(&tmp, 4);
	info.set(tmp);
	sforin(i, 0, info.ref_num) {
		_data->copy(&tmp, 4);
		info.ref_name[i].resize((size_t)tmp, '\0');
		_data->copy(&info.ref_name[i][0], tmp);
		info.ref_name[i].resize((size_t)tmp - 1);
		_data->copy(&(info.ref_length[i]), 4);
	}
}
void SBamFile::_checkError() {
	if (_data->result == Z_STREAM_ERROR || _data->result == Z_DATA_ERROR || _data->result == Z_BUF_ERROR)
		throw SBioInfoException(ERR_INFO, SLIB_EXEC_ERROR, "inflate", std::to_string(_data->result).c_str());
}
void SBamFile::init() {
	if (_file.isOpened()) _file.close();
	index.init();
}
void SBamFile::setParam(sngs_param* p) {
	_single = !p->parallele;
	if (_single) {
		_store.resize(1);
		_data->setBam(this);
		_data->resize(p->block);
	}
	else {
		_store.resize(p->count);
		sforeach(_store) { E_.setBam(this); E_.resize(p->block); }
	}
	_data = &_store[0];
}
void SBamFile::open(const char* path) {
	if (_file.isOpened()) _file.close();
	_file.open(path);
	_data->load();
	_checkError();
	info.init();
	loadIndex(String(path) + ".bai");
	_readHeader();
}
void SBamFile::close() {
	if (_file.isOpened()) _file.close();
}
void SBamFile::loadIndex(const char* path) {
	index.init();
	if (sio::SFile(path).exist()) index.load(path);
}
bool SBamFile::hasIndex() const { return 0 < index.ref_num; }
void SBamFile::lock() { _lock.lock(); }
void SBamFile::unlock() { _lock.unlock(); }
suinteger SBamFile::filesize() const { return _file.size(); }
suinteger SBamFile::offset() { return _file.offset(); }
void SBamFile::setOffset(suinteger off) { return _file.setOffset(off); }
String SBamFile::path() const { return _file.path(); }


sbam::voffset SBamFile::voff() const { return _data->offset; }


void SBamFile::setVOff(const sbam::voffset& off) {
	if (_file.eof()) _file.clear();
	lock();
	_file.setOffset(off.file_offset);
	_data->load(); 
	_data->setOffset(off.block_offset);
	unlock();
}
void SBamFile::setVOff(sint idx, const sbam::voffset& off) {
	lock();
	if (_file.eof()) _file.clear();
	_file.setOffset(off.file_offset);
	_store[idx].load(); 
	_store[idx].setOffset(off.block_offset);
	unlock();
}
//void SBamFile::sort() {}
//void SBamFile::createIndex() {}
ubytearray* SBamFile::next(ubytearray* dat) { return _data->next(); }
SBamData* SBamFile::getData(sint idx) { return &_store[idx]; }
void SBamFile::setData(sint idx) {
	lock();
	_data = &_store[idx];
	unlock();
}
void SBamFile::getReads(sbam::read_array& array, const sbpos& pos) {
	array.clear();
	if (!hasIndex()) return;
	ushortarray bins;
	sbiutil::getBins(bins, pos);
	if (bins.empty()) return;
	sforeach(bins) {
		auto& chunks = index.chunks[pos.idx][index.bin_map[pos.idx][(suint)E_]];
		if (chunks.empty()) continue;
		sforeach_(cit, chunks) {
			setVOff(cit->begin);
			do {
				if (!next()) break;
				sbam::readinfo ri(read);
				if (E_ == ri.bin && pos.overlap(ri.range())) array.add(ri);
			} while (_data->offset < cit->end);
		}
	}
}
void SBamFile::getReads(sbam::read_array& array, sint idx, const sregion& region) {
	array.clear();
	if (!hasIndex()) return;
	ushortarray bins;
	sbiutil::getBins(bins, region);
	if (bins.empty()) return;
	sforeach(bins) {
		auto& chunks = index.chunks[idx][index.bin_map[idx][(suint)E_]];
		if (chunks.empty()) continue;
		sforeach_(cit, chunks) {
			setVOff(cit->begin);
			do {
				if (!next()) break;
				sbam::readinfo ri(read);
				if (E_ == ri.bin && region.overlap(ri.range())) array.add(ri);
			} while (_data->offset < cit->end);
		}
	}
}
*/