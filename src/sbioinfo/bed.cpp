#include "sbioinfo/bed.h"

slib::sbio::BEDData::BEDData() : slib::sbio::RefPos(), score(0) {}
slib::sbio::BEDData::BEDData(const BEDData& bd) : slib::sbio::RefPos(bd) {
    name = bd.name; score = bd.score; attribute = bd.attribute;
}
slib::sbio::BEDData::~BEDData() {}
slib::sbio::BEDData& slib::sbio::BEDData::operator=(const BEDData& bd) {
    idx = bd.idx; begin = bd.begin; end = bd.end; dir = bd.dir;
    name = bd.name; score = bd.score; attribute = bd.attribute; return *this;
}
void slib::sbio::BEDData::clear() {
    slib::sbio::RefPos::init();
    name.clear(); score = 0.f; attribute.clear();
}

slib::sbio::BEDFile::BEDFile() : _ref(nullptr) {}
slib::sbio::BEDFile::BEDFile(const char* path) : BEDFile() { open(path); }
slib::sbio::BEDFile::BEDFile(const char* path, SeqList& ref) { load(path, ref); }
slib::sbio::BEDFile::~BEDFile() {}
void slib::sbio::BEDFile::operator>>(slib::Array<slib::Region<slib::sint>>& reg) {
    if (!_ref) throw NullException(nullErrorText("Reference data"));
    reg.resize(_ref->size());
    while (_file && next()) {
        reg[_buffer.idx].add(_buffer);
    }
}
void slib::sbio::BEDFile::setRef(SeqList& ref) { _ref = &ref; }
void slib::sbio::BEDFile::open(const char* path) { _file.open(path); }
void slib::sbio::BEDFile::load(const char* path, SeqList& ref) {
    open(path); 
    setRef(ref);
    while (_file) { next(true); }
}
const slib::String slib::sbio::BEDFile::path() const { return _file.path(); }
slib::sbio::BEDData* slib::sbio::BEDFile::next(bool stack) {
    if (!_ref) throw NullException(nullErrorText("Reference data"));
    if (!_file) return nullptr;
    _file.readLine(_ln);
    if (_ln.empty() || _ln.beginWith("#")) return next(stack);
    auto values = _ln.split(slib::TAB);
    if (values.size() < 3) throw FormatException(formatErrorText("BED file data", _ln, "| Ref ID | Start | End | ... (>= 3 columns)"));
    _buffer.idx = (int)_ref->index(values[0]);
    _buffer.begin = values[1].intValue();
    _buffer.end = values[2].intValue();
    if (3 < values.size()) _buffer.name = values[3];
    if (4 < values.size()) _buffer.score = values[4].floatValue();
    if (5 < values.size()) _buffer.dir = values[5] == "-";
    /*
    if (6 < values.size()) _buffer.attribute["start2"] = values[6].intValue();
    if (7 < values.size()) _buffer.attribute["end2"] = values[7].intValue();
    if (8 < values.size()) _buffer.attribute["color"] = values[8];
    */
    if (stack) add(_buffer);
    return &_buffer;
}
slib::sbio::BEDFile::operator bool() const { return _file.isOpened() && (!_file.eof()); }
