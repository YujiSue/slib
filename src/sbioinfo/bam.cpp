#include "sutil/scode.h"
#include "sbioinfo/bam.h"

slib::String slib::sbio::bamFormatErrorText(char* s) {
	String txt("BAM file magic code should be '"), hex("0x");
	sforin(i, 0, 4) hex << slib::toHex(BAM_MAGIC[i]);
	txt << hex << "', but detected magic was '";
	hex = "0x";
	sforin(i, 0, 4) { hex << slib::toHex(*s); ++s; }
	txt << "'.";
	txt << hex << "'.";
	return txt;
}
slib::String slib::sbio::baiFormatErrorText(char* s) {
	String txt("BAI file magic code should be '"), hex("0x");
	sforin(i, 0, 4) hex << slib::toHex(BAI_MAGIC[i]);
	txt << hex << "', but detected magic was '";
	hex = "0x";
	sforin(i, 0, 4) { hex << slib::toHex(*s); ++s; }
	txt << hex << "'.";
	return txt;
}
slib::String slib::sbio::bgzfFormatErrorText(char* s) {
	String txt("BGZF block magic code should be '"), hex("0x");
	sforin(i, 0, 16) hex << slib::toHex(BGZF_MAGIC[i]);
	txt << hex << "', but detected magic was '";
	hex = "0x";
	sforin(i, 0, 16) { hex << slib::toHex(*s); ++s; }
	txt << hex << "'.";
	return txt;
}
slib::String slib::sbio::bsmFormatErrorText(char* s) {
	String txt("BSM magic code should be '"), hex("0x");
	sforin(i, 0, 3) hex << slib::toHex(BSM_MAGIC[i]);
	txt << hex << "', but detected magic was '";
	hex = "0x";
	sforin(i, 0, 3) { hex << slib::toHex(*s); ++s; }
	txt << hex << "'.";
	return txt;
}

inline size_t slib::sbio::sbam::countBin(const srange& range) {
	size_t count = 1;
	slib::Map<int, size_t> order;
	order[0] = 0;
	auto beg = range.begin >> 26, end = (range.end >> 26) + 1;
	sforin(i, beg, end) { order[1 + i] = count; ++count; }
	beg = range.begin >> 23, end = (range.end >> 23) + 1;
	sforin(i, beg, end) { order[9 + i] = count; ++count; }
	beg = range.begin >> 20, end = (range.end >> 20) + 1;
	sforin(i, beg, end) { order[73 + i] = count; ++count; }
	beg = range.begin >> 17, end = (range.end >> 17) + 1;
	sforin(i, beg, end) { order[585 + i] = count; ++count; }
	beg = range.begin >> 14, end = (range.end >> 14) + 1;
	sforin(i, beg, end) { order[4681 + i] = count; ++count; }
	return count;
}
slib::sushort slib::sbio::sbam::getBin(srange range) {
	--range.end;
	if (range.begin >> 14 == range.end >> 14) return ((1 << 15) - 1) / 7 + (range.begin >> 14);
	if (range.begin >> 17 == range.end >> 17) return ((1 << 12) - 1) / 7 + (range.begin >> 17);
	if (range.begin >> 20 == range.end >> 20) return ((1 << 9) - 1) / 7 + (range.begin >> 20);
	if (range.begin >> 23 == range.end >> 23) return ((1 << 6) - 1) / 7 + (range.begin >> 23);
	if (range.begin >> 26 == range.end >> 26) return ((1 << 3) - 1) / 7 + (range.begin >> 26);
	return 0;
}
slib::ushortarray slib::sbio::sbam::getBins(srange range) {
	ushortarray bins;
	--range.end;
	bins.add(0);
	sforin(k, 1 + (range.begin >> 26), 2 + (range.end >> 26)) bins.add(k);
	sforin(k, 9 + (range.begin >> 23), 10 + (range.end >> 23)) bins.add(k);
	sforin(k, 73 + (range.begin >> 20), 74 + (range.end >> 20)) bins.add(k);
	sforin(k, 585 + (range.begin >> 17), 586 + (range.end >> 17)) bins.add(k);
	sforin(k, 4681 + (range.begin >> 14), 4682 + (range.end >> 14)) bins.add(k);
	return bins;
}
slib::ushortarray slib::sbio::sbam::getBins(const sregion& region) {
	ushortarray bins;
	sfor(region) bins.append(getBins($_));
	bins.unique();
	return bins;
}
slib::sbio::sbam::VOffset::VOffset() : file_offset(0), block_offset(0) {}
slib::sbio::sbam::VOffset::VOffset(sinteger fo, sushort bo) : file_offset(fo), block_offset(bo) {}
slib::sbio::sbam::VOffset::VOffset(suinteger offset) : file_offset(offset>>16), block_offset(offset&0xFFFF) {}
slib::sbio::sbam::VOffset::VOffset(const sbam::VOffset &v) : file_offset(v.file_offset), block_offset(v.block_offset) {}
slib::sbio::sbam::VOffset::~VOffset() {}
slib::sbio::sbam::VOffset & slib::sbio::sbam::VOffset::operator = (const slib::sbio::sbam::VOffset &v) {
    file_offset = v.file_offset; block_offset = v.block_offset; return (*this);
}
slib::sbio::sbam::VOffset::operator slib::suinteger() const { return ((suinteger)file_offset << 16) | (suinteger)block_offset; }
bool slib::sbio::sbam::VOffset::operator < (const sbam::VOffset &v) const {
    if (file_offset < v.file_offset) return true;
    else if (file_offset == v.file_offset && block_offset < v.block_offset) return true;
    return false;
}
bool slib::sbio::sbam::VOffset::operator == (const sbam::VOffset &v) const {
    return file_offset == v.file_offset && block_offset == v.block_offset;
}
slib::sbio::sbam::Header::Header() : ref_num(0) {}
slib::sbio::sbam::Header::~Header() {}
void slib::sbio::sbam::Header::set(int n) {
    ref_num = n;
    ref_length.resize(ref_num);
    ref_name.resize(ref_num);
}
slib::String slib::sbio::sbam::Header::toString() const{
	String str;
	str << String("=") * 60 << NL;
	str << SP * 5 << "Index" << SP * 5 << "| Name" << SP * 16 << "| Length (bp)" << SP * 6 << NL;
	sforin(r, 0, ref_num) {
		str << SP * 5 <<  
			sstr::fill(String(r + 1), ' ', 10, DIRECTION::BI) << "| " << 
			sstr::fill(ref_name[r], ' ', 20, DIRECTION::BI) << "| " << 
			ref_length[r] << NL;
	}
	str << String("-") * 60 << NL;
	if (text.size() <= 60) str << text << NL;
	else str << text.substring(0, 57) << "..." << NL;
	str << String("=") * 60 << NL;
	return str;
}
void slib::sbio::sbam::Header::init() {
	ref_num = 0;
	ref_length.clear();
	ref_name.clear();
	text.clear(); 
}
slib::sbio::sbam::Index::Index() : ref_num(0) {}
slib::sbio::sbam::Index::Index(const char* path) : slib::sbio::sbam::Index() { load(path); }
slib::sbio::sbam::Index::~Index() {};
void slib::sbio::sbam::Index::setNum(int n) {
	ref_num = n;
	chunks.resize(ref_num);
	loffsets.resize(ref_num);
	bin_map.resize(ref_num);
}
void slib::sbio::sbam::Index::load(const char* path) {
	try {
		if (!sfs::exist(path)) throw FileNotFoundException(slib::sio::nofileErrorText(path));
		char magic[4];
		sint bnum, cnum, lnum;
		suint bin;
		suinteger off;
		SFile file(path, sio::READ);
		file.readBytes(magic, 4);
		if (memcmp(BAI_MAGIC, magic, 4) != 0) throw BioFileFormatException(BAI_FORM_ERR, baiFormatErrorText(magic));// (ERR_INFO, SLIB_FORMAT_ERROR, magic, "BAI");
		file.readInt(ref_num);
		if (ref_num < 1) throw Exception();// (ERR_INFO, SLIB_RANGE_ERROR, "ref_num", "> 0");
		setNum(ref_num);
		sforin(i, 0, ref_num) {
			auto& map = bin_map[i];
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
			loffsets[i].resize(lnum);
			sforin(l, 0, lnum) {
				file.readUInteger(off);
				loffsets[i][l] = off;
			}
		}
	}
	catch (SIOException ie) {
		ie.print();
	}
}
//void slib::sbio::sbam::Index::save(const char *path){}
void slib::sbio::sbam::Index::init() {
	sforin(i, 0, ref_num) {
		sfor(chunks[i]) $_.clear();
		loffsets[i].clear();
	}
}
slib::sbio::sbam::Block::Block() { init(); }
slib::sbio::sbam::Block::~Block() {}
void slib::sbio::sbam::Block::init() {
	offset = 0;
	ori_length = 0;
	block_length = 0;
	ori_data.reserve(BGZF_MAX_BLOCK_SIZE + 1);
	memset(ori_data.data(), 0, BGZF_MAX_BLOCK_SIZE);
	bam_data.reserve(BGZF_MAX_BLOCK_SIZE + 1);
	memset(bam_data.data(), 0, BGZF_MAX_BLOCK_SIZE);
	current = bam_data.data();
}
size_t slib::sbio::sbam::Block::read(void* dest, const size_t sz) {
	if (block_length <= (size_t)(current - bam_data.data()) + sz) {
		auto remain = block_length - (current - bam_data.data());
		if (0 < remain) memcpy(dest, current, remain);
		current = bam_data.data() + block_length;
		offset.block_offset = block_length;
		return remain;
	}
	else {
		memcpy(dest, current, sz);
		current += sz;
		offset.block_offset += (sushort)sz;
		return sz;
	}
}
void slib::sbio::sbam::Block::setOffset(sushort boff) {
	offset.block_offset = boff;
	current = bam_data.data(boff);
}
/*
	size : 4
	---------
	pos.idx : 0-4
	pos.begin : 4-8
	name.len : 8-9
	mapq : 9-10
	bin : 10-12
	ciglen : 12-14
	flag : 14-16
		if (flag & sbam::COMPLEMENT_READ) pos.dir = true;
	seqlen : 16-20
	next.idx : 20-24
	next.begin : 24-28
		if (flag & sbam::$NEXT;REVERSE_READ) next.dir = true;
	tmplen : 28-32
	name : 32+[0-name.len]
	cigars : 32+name.len+[0-cigsize*4]
			pos.end = pos.begin + (sint)cigars.refSize() - 1;
	seq : 32+name.len+cigsize*4+[0-((seqlen-1)/2)+1]
	qual : 32+name.len+cigsize*4+((seqlen-1)/2)+1+[0-((seqlen-1)/2)+1]
	aux : 32+name.len+cigsize*4+(((seqlen-1)/2)+1)*2
	*/
slib::sbio::sbam::ReadBytes::ReadBytes() : ubytearray() { _namelen = 0; _cigsize = 0; _seqsize = 0; }
slib::sbio::sbam::ReadBytes::ReadBytes(const ReadBytes& ri) : ubytearray(ri) { 
	_namelen = ri._namelen; _cigsize = ri._cigsize; _seqsize = ri._seqsize;
}
slib::sbio::sbam::ReadBytes::~ReadBytes() {}
slib::sbio::sbam::ReadBytes& slib::sbio::sbam::ReadBytes::operator=(const ReadBytes& ri) {
	ubytearray::resize(ri.size()); memcpy(ubytearray::data(), ri.data(), ri.size());
	_namelen = ri._namelen; _cigsize = ri._cigsize; _seqsize = ri._seqsize;
	return *this;
}
void slib::sbio::sbam::ReadBytes::interpret() {
	_namelen = (int)ubytearray::at(8);
	_cigsize = (int)*reinterpret_cast<const sushort*>(ubytearray::data(12));
	_seqsize = length();
	_seqsize = ((_seqsize - 1) / 2) + 1;
}
slib::sbio::RefPos slib::sbio::sbam::ReadBytes::pos() const {
	sbpos p;
	p.idx = *reinterpret_cast<const sint*>(ubytearray::data(0));
	p.begin = *reinterpret_cast<const sint*>(ubytearray::data(4));
	p.end = p.begin - 1;
	Cigar cig;
	auto cp = cigars();
	sforin(c, 0, _cigsize) {
		cig = *cp;
		if (cig.ref()) p.end += cig.length;
		++cp;
	}
	auto f = flag();
	p.dir = f & sbam::COMPLEMENT_READ;
	return p;
}
slib::sbio::RefPos slib::sbio::sbam::ReadBytes::next() const {
	sbpos p;
	p.idx = *reinterpret_cast<const sint*>(ubytearray::data(20));
	p.begin = *reinterpret_cast<const sint*>(ubytearray::data(24));
	p.end = p.begin;
	auto f = flag();
	p.dir = f & sbam::NEXT_REVERSE_READ;
	return p;
}
slib::String slib::sbio::sbam::ReadBytes::name() const {
	String s(_namelen, '\0');
	memcpy(&s[0], data(32), _namelen);
	if (s[-1] == '\0') s.resize(s.size() - 1);
	return s;
}
const slib::subyte* slib::sbio::sbam::ReadBytes::seq() const { 
	return ubytearray::data(32 + _namelen + _cigsize * 4); 
}
void slib::sbio::sbam::ReadBytes::expand(ubytearray& s) const {
	s.resize(length());
	sbio::sdna::expand2(seq(), 0, s.size(), s.data());
}
void slib::sbio::sbam::ReadBytes::decode(String& s) const {
	s.resize(length());
	sbio::sdna::decode2(seq(), 0, s.size(), (subyte*)&s[0]);
}
slib::sint slib::sbio::sbam::ReadBytes::length() const { 
	return *reinterpret_cast<const sint*>(ubytearray::data(16)); 
}
const slib::subyte* slib::sbio::sbam::ReadBytes::qual() const { 
	return ubytearray::data(32 + _namelen + _cigsize * 4 + _seqsize); 
}
void slib::sbio::sbam::ReadBytes::toQuality(intarray& seq) const {
	auto p = qual();
	seq.resize(length());
	sfor(seq) $_ = (int)(*p);
}
int slib::sbio::sbam::ReadBytes::cigsize() const { return _cigsize; }
const slib::suint* slib::sbio::sbam::ReadBytes::cigars() const { 
	return reinterpret_cast<const suint *>(ubytearray::data(32 + _namelen)); 
}
void slib::sbio::sbam::ReadBytes::toCigarArray(CigarArray& array) const {
	array.resize(_cigsize);
	auto cig = cigars();
	sfor(array) { $_ = *cig; ++cig; }
}
slib::sint slib::sbio::sbam::ReadBytes::tmplen() const { 
	return *reinterpret_cast<const sint*>(ubytearray::data(28));
}
slib::subyte slib::sbio::sbam::ReadBytes::mapq() const { return ubytearray::at(9); }
slib::sushort slib::sbio::sbam::ReadBytes::bin() const { 
	return *reinterpret_cast<const sushort *>(ubytearray::data(10)); 
}
slib::sushort slib::sbio::sbam::ReadBytes::flag() const { 
	return *reinterpret_cast<const sushort*>(ubytearray::data(14)); 
}
int slib::sbio::sbam::ReadBytes::auxsize() const { 
	return length() - 32 - _namelen - _cigsize * 4 - _seqsize * 2; 
}
const slib::subyte* slib::sbio::sbam::ReadBytes::auxiliary() const { 
	return ubytearray::data(32 + _namelen + _cigsize * 4 + _seqsize * 2); 
}
void slib::sbio::sbam::ReadBytes::clear() {
	ubytearray::clear();
	_namelen = 0;
	_cigsize = 0;
	_seqsize = 0;
}
slib::String slib::sbio::sbam::ReadBytes::toString(int inflate) {
	String str, tmp;
	auto p1 = pos(), p2 = next();
	str << name() << TAB << (int)flag() << TAB <<
		p1.idx << TAB << p1.begin + 1 << TAB << (int)mapq() << TAB;
	auto cp = cigars();
	Cigar cig;
	sforin(i, 0, _cigsize) {
		cig = *cp;
		str << cig.length << scigar::LABEL[cig.option]; ++cp;
	}
	str << TAB << p2.idx << TAB << p2.begin << TAB << length() << TAB;
	decode(tmp);
	str << tmp << TAB;
	auto q = qual();
	sfor(tmp) { $_ = (char)(inflate + (int)(*q)); ++q; }
	str << tmp;
	return str;
}

slib::sbio::sbam::ReadInfo::ReadInfo() { clear(); }
slib::sbio::sbam::ReadInfo::ReadInfo(const slib::sbio::sbam::ReadInfo& ri) : AlignPair(ri) {
	next = ri.next;
	mapq = ri.mapq;
	bin = ri.bin;
	flag = ri.flag;
	tmplen = ri.tmplen;
	name = ri.name;
	qual = ri.qual;
	seq = ri.seq;
	auxiliary = ri.auxiliary;
}
slib::sbio::sbam::ReadInfo::~ReadInfo() {}
void slib::sbio::sbam::ReadInfo::clear() {
	ref.init();
	query = srange(0, -1);
	cigars.clear();
	score = 0.f;
	next.init();
	mapq = 0;
	bin = 0;
	flag = 0;
	tmplen = 0;
	name.clear();
	qual.clear();
	seq.clear();
	auxiliary.clear();
}
inline slib::String qualString(const slib::String& q, int qi = 33) {
	slib::String qual(q.size(), '\0');
	sfor2(q, qual) { $_2 = (char)((int)$_1 + qi); }
	return qual;
}
slib::String slib::sbio::sbam::ReadInfo::raw() const {
	slib::String str(seq.size(), '\0');
	sdna::decode(seq.data(), 0, seq.size(), (subyte*)&str[0]);
	return str;
}
slib::String slib::sbio::sbam::ReadInfo::toString() const {
	String str;
	str << name << TAB << String(flag) << TAB << ref.idx << TAB << ref.begin + 1 << TAB <<
		(int)mapq << TAB << cigars.toString() << TAB << 
		next.idx << TAB << next.begin << TAB << seq.size() << TAB << 
		raw() << TAB << qualString(qual);
	return str;
}
bool slib::sbio::sbam::ReadInfo::operator<(const slib::sbio::sbam::ReadInfo& ri) const { return ref < ri.ref; }
bool slib::sbio::sbam::ReadInfo::operator==(const slib::sbio::sbam::ReadInfo& ri) const { return ref == ri.ref && seq == ri.seq; }

slib::sbio::BamFile::BamFile(const size_t buffer_size) {
	_worker.setSize(2);
	_blocks.resize(buffer_size);
	_current = CycleIterator<sbam::Block>(_blocks.data(), _blocks.begin(), buffer_size);
}
slib::sbio::BamFile::BamFile(const BamFile& bam) {}
slib::sbio::BamFile::~BamFile() { close(); }
void slib::sbio::BamFile::readHeader() {
	char s[4];
	readBytes(s, 4);
	if (memcmp(BAM_MAGIC, s, 4) != 0) throw BioFileFormatException(BAM_FORM_ERR, bamFormatErrorText(s));
	sint tmp;
	readBytes(&tmp, 4);
	info.text.resize(tmp);
	readBytes(&info.text[0], tmp);
	readBytes(&tmp, 4);
	info.set(tmp);
	sforin(i, 0, info.ref_num) {
		readBytes(&tmp, 4);
		info.ref_name[i].resize((size_t)tmp, '\0');
		readBytes(&info.ref_name[i][0], tmp);
		info.ref_name[i].resize((size_t)tmp - 1);
		readBytes(&(info.ref_length[i]), 4);
	}
}
void slib::sbio::BamFile::init() {
	_file.close();
	index.init();
	info.init();
}
void slib::sbio::BamFile::open(const char* path) {
	init(); 
	_file.open(path);
	readBlocks();
	readHeader();
	loadIndex(String(path) + ".bai");	
}
void slib::sbio::BamFile::close() { _file.close(); }
//void BamFile::sort() {}
//void BamFile::makeIndex() {}
void slib::sbio::BamFile::loadIndex(const char* path) { index.load(path); }
bool slib::sbio::BamFile::hasIndex() const { return index.ref_num; }
slib::suinteger slib::sbio::BamFile::filesize() const { return _file.size(); }
slib::String slib::sbio::BamFile::path() const { return _file.path(); }
bool slib::sbio::BamFile::eof() { return _file.eof(); }
slib::sbio::sbam::VOffset slib::sbio::BamFile::voffset() { return _current->offset; }
void slib::sbio::BamFile::setVOffset(const slib::sbio::sbam::VOffset& off) {
	if (_worker.isWorking()) _worker.complete();
	if (_file.eof()) _file.clear();
	sfor(_blocks) $_.init();
	_file.setOffset(off.file_offset);
	readBlocks(); 
	_current->setOffset(off.block_offset);
}
void slib::sbio::BamFile::readBlock(sbam::Block* block) {
	char magic[16];
	block->init();
	block->offset = sbam::VOffset(_file.offset(), 0);
	if (_file.eof() || (size_t)_file.offset() == _file.size()) return;
	// Magic check
	_file.readBytes(magic, 16);
	if (memcmp(BGZF_MAGIC, magic, 16) != 0) throw BioFileFormatException(BGZF_FORM_ERR, bgzfFormatErrorText(magic));
	// Length check
	_file.readUShort(block->ori_length);
	if (block->ori_length < 25) return;
	//
	block->ori_data.resize(block->ori_length - 25);
	_file.readBytes(block->ori_data.data(), block->ori_data.size());
	_file.readInt(block->block_length);
	_file.readInt(block->block_length);
	// 
	if (BGZF_MAX_BLOCK_SIZE < block->block_length) throw BioFileFormatException(BGZF_BLOCKSIZE_ERR, outRangeErrorText("BGZF block size", block->block_length, 0, BGZF_MAX_BLOCK_SIZE));
	// Inflate data
	slib::sutil::inflate(block->bam_data, block->ori_data, BGZF_MAX_BLOCK_SIZE, -15);
}
void slib::sbio::BamFile::readBlocks() {
	sfor(_blocks) readBlock($.ptr());
	_current = CycleIterator<sbam::Block>(_blocks.data(), _blocks.begin(), _blocks.size());
}
inline void readThread(slib::sbio::BamFile* file, slib::sbio::sbam::Block* block) { 
	file->readBlock(block); 
}
void slib::sbio::BamFile::readBytes(void* dest, const size_t sz) {
	auto s = _current->read(dest, sz);
	if (s == sz) return;
	if (_worker.isWorking()) _worker.complete();
	auto buf = _current.ptr();
	_worker.addTask(&readThread, this, buf);
	++_current;
	if (_current->ori_length == 0) return;
	readBytes(((subyte*)dest) + s, sz - s);
}
slib::sbio::sbam::ReadBytes* slib::sbio::BamFile::read() {
	int tmp;
	_data.clear();
	readBytes(&tmp, 4);
	_data.resize(tmp);
	readBytes(_data.data(), tmp);
	_data.interpret();
	return nullptr;
}
inline void _checkSize(int& total) {
	if (total < 0) throw BioFileFormatException("Data size error", "Read data size is too short.");
}
slib::sbio::sbam::ReadInfo* slib::sbio::BamFile::next() {
	if (_current->ori_length == 0) return nullptr;
	_read.clear();
	sint total, tmp;
	suint cig;
	//
	readBytes(&total, 4); _checkSize(total);
	if (!total || _current->offset.file_offset == _file.size()) return nullptr;
	//
	readBytes(&_read.ref.idx, 4); total -= 4; _checkSize(total);
	readBytes(&_read.ref.begin, 4); total -= 4; _checkSize(total);
	if (_read.ref.idx < -1 || _read.ref.begin < -1)	return nullptr;
	//
	readBytes(&tmp, 4); total -= 4; _checkSize(total);
	_read.name.resize(tmp & 0xFF);
	_read.mapq = (tmp >> 8) & 0xFF;
	_read.bin = (tmp >> 16) & 0xFFFF;
	//
	readBytes(&tmp, 4); total -= 4; _checkSize(total);
	_read.cigars.resize(tmp & 0xFFFF);
	_read.flag = (tmp >> 16) & 0xFFFF;
	if (_read.flag & sbam::COMPLEMENT_READ) _read.ref.dir = true;
	//
	readBytes(&tmp, 4); total -= 4; _checkSize(total);
	_read.seq.resize(tmp);
	_read.qual.resize(tmp);
	_read.query = srange(0, tmp - 1);
	//
	readBytes(&_read.next.idx, 4); total -= 4; _checkSize(total);
	readBytes(&_read.next.begin, 4); total -= 4; _checkSize(total);
	_read.next.end = _read.next.begin;
	if (_read.flag & sbam::NEXT_REVERSE_READ) _read.next.dir = true;
	readBytes(&_read.tmplen, 4); total -= 4; _checkSize(total);
	//
	readBytes(&_read.name[0], _read.name.size()); 
	total -= (int)_read.name.size(); _checkSize(total);
	if (_read.name[-1] == '\0') _read.name.resize(_read.name.size() - 1);
	//
	sfor(_read.cigars) {
		readBytes(&cig, 4); $_ = cig;
		total -= 4; _checkSize(total);
	}
	_read.ref.end = _read.ref.begin + (sint)_read.cigars.refSize() - 1;
	//
	_read.auxiliary.resize((_read.seq.size() - 1) / 2 + 1);
	readBytes(_read.auxiliary.data(), _read.auxiliary.size()); 
	total -= (int)_read.auxiliary.size(); _checkSize(total);
	sdna::expand2(_read.auxiliary.data(), 0, _read.seq.size(), _read.seq.data());
	//
	readBytes(&_read.qual[0], _read.qual.size());
	total -= (int)_read.qual.size(); _checkSize(total);
	//
	_read.auxiliary.resize(total);
	readBytes(&_read.auxiliary[0], total);
	return &_read;
}