#include "sbioinfo/gff.h"
slib::sbio::GffData::GffData() : slib::sbio::RefPos() { init(); }
slib::sbio::GffData::GffData(const slib::sbio::GffData &g) : sbpos(g) {
    seqid = g.seqid;
    source = g.source;
    type = g.type;
    phase = g.phase;
    score = g.score;
    attribute = g.attribute;
}
slib::sbio::GffData::~GffData() {}
slib::sbio::GffData & slib::sbio::GffData::operator=(const slib::sbio::GffData &g) {
    idx = g.idx;
    begin = g.begin;
    end = g.end;
    dir = g.dir;
    seqid = g.seqid;
    source = g.source;
    type = g.type;
    phase = g.phase;
    score = g.score;
    attribute = g.attribute;
    return *this;
}
void slib::sbio::GffData::init() {
    RefPos::init();
    seqid = ""; source = ""; type = ""; phase = 0;
    score = 0.0; attribute.clear();
}
inline void set(const stringarray& row, slib::sbio::GffData &data) {
    data.init();
    if (row.size() != 9) throw FormatException(slib::formatErrorText("GFF data", toString(row, slib::TAB), "9 columns"));
    data.seqid = row[0];
    data.source = row[1];
    data.type = row[2];
    data.begin = row[3] == "." ? -1 : row[3].intValue();
    data.end = row[4] == "." ? -1 : row[4].intValue();
    data.score = row[5] == "." ? -1.f : row[5].floatValue();
    data.strand = row[6];
    data.dir = data.strand == "-";
    data.phase = row[7] == "." ? -1 : row[7].intValue();
    data.attribute = row[8] == "." ? sattribute() : row[8].parse(";", "=");
}
slib::sbio::GffFile::GffFile() {}
slib::sbio::GffFile::GffFile(const char *path) : GffFile() { open(path); }
slib::sbio::GffFile::~GffFile() {}
void slib::sbio::GffFile::readHeader() {
    auto cur = SFile::offset();
    while (true) {
        cur = SFile::offset();
        SFile::readLine(_ln, true);
        if (_ln.beginWith("##")) {
            auto p = _ln.find(" ");
            if (p != NOT_FOUND) attribute[_ln.substring(2, p - 2)] = _ln.substring(p + 1);
        }
        else {
            SFile::setOffset(cur);
            break;
        }
    }
}
void slib::sbio::GffFile::open(const char *path, SeqList* reference) {
    SFile::open(path);
    readHeader();
}
slib::sbio::GffData* slib::sbio::GffFile::next() {
    if (!SFile::isOpened() || SFile::eof()) return nullptr;
    else {
        SFile::readLine(_ln, true);
        if (_ln.empty() || _ln[0] == '#') return next();
        _buffer = _ln.split(TAB, false, true);
        if (_buffer.size() != 9) return next();
        set(_buffer, _data);
        return &_data;
    }
}