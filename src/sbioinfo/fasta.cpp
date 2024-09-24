#include "sbioinfo/fasta.h"
#include "sbioinfo/seq.h"
#include "sbioinfo/seqio.h"
slib::sbio::Fasta::Fasta() : _type(0), _idx(0) {}
slib::sbio::Fasta::Fasta(const char* path, sushort type) : Fasta() { open(path, type); }
slib::sbio::Fasta::~Fasta() {}
void slib::sbio::Fasta::operator>>(Sequence& seq) {
	if (titles.empty()) makeIndex();
	seq.name = titles[(int)_idx];
	if (_idx < count() - 1) _buffer.resize(offsets[(int)_idx + 1] - offsets[(int)_idx]);
	else _buffer.resize(_file.size() - offsets[(int)_idx]);
	_file.readBytes(&_buffer[0], _buffer.size());
	auto end = _buffer.rfind(">");
	if (end != NOT_FOUND) _buffer.resize(end);
	seq.setSeqAs(_buffer, _type);
	++_idx;
}
void slib::sbio::Fasta::operator>>(SeqList& list) {
	if (titles.empty()) makeIndex();
	setIndex(0);
	list.resize(count());
	list.attribute["_src_"] = _file.path();
	sfor2(list, titles) {
		$_1.name = $_2;
		if (_idx < count() - 1) _buffer.resize(offsets[(int)_idx + 1] - offsets[(int)_idx]);
		else _buffer.resize(_file.size() - offsets[(int)_idx]);
		_file.readBytes(&_buffer[0], _buffer.size());
		_buffer.resize(_buffer.rfind(">"));
		$_1.setSeqAs(_buffer, _type);
		++_idx;
	}
	_file.clear();
	setIndex(0);
}
void slib::sbio::Fasta::open(const char* path, sushort type) { 
	init();
	_file.open(path); 
	if (type == 0xFFFF) {
		while (_file) {
			_file.readLine(_buffer);
			if (_buffer.empty() || _buffer[0] == '>') continue;
			else break;
		}
		if (_buffer.size()) _type = sseq::checkType(_buffer);
		else _type = type;
		_file.clear();
		_file.setOffset(0);
	}
	else _type = type;	
	_idx = 0;
}
void slib::sbio::Fasta::makeIndex() {
	String ln;
	titles.clear();
	offsets.clear();
	while (_file) {
		_file.readTo(">");
		_file.readLine(ln);
		if (ln.empty()) continue;
		auto pos = ln.find(SP);
		if (pos == NOT_FOUND) titles.add(ln);
		else {
			titles.add(ln.substring(0, pos));
			attributes.add(ln.substring(pos + 1));
		}
		offsets.add(_file.offset());
	}
	_file.clear();
	_file.setOffset(0);
	_idx = 0;
}
size_t slib::sbio::Fasta::count() { return titles.size(); }
void slib::sbio::Fasta::setIndex(const size_t idx) { 
	_idx = idx; _file.setOffset(offsets[(int)_idx]);
}
void slib::sbio::Fasta::init() {
	_file.close();
	_type = 0;
	_idx = 0;
	titles.clear();
	offsets.clear();
	_buffer.clear();
}

