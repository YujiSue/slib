#include "sbioinfo/sgf.h"

using namespace slib;
using namespace slib::sbio;

sgff::sgff() : sbpos() { init(); }
sgff::sgff(const String &row) : sbpos() { set(row); }
sgff::sgff(const sgff &g) : sbpos(g) {
    seqid = g.seqid;
    source = g.source;
    type = g.type;
    phase = g.phase;
    score = g.score;
    attribute = g.attribute;
}
sgff::~sgff() {}

void sgff::init() {
    sbpos::init();
    seqid = ""; source = ""; type = ""; phase = 0;
    score = 0.0; attribute.clear();
}
void sgff::set(const String &row) {
    init();
    auto str = row.split("\t");
    seqid = str[0];
    source = str[1];
    type = str[2];
    begin = str[3]=="."?-1:str[3].intValue();
    end = str[4]=="."?-1:str[4].intValue();
    score = str[5]=="."?-1.0:str[5].real();
    strand = str[6];
    phase = str[7]=="."?-1:SNumber::toNumber(str[7]).intValue();
    attribute = str[8]=="."?sattribute():str[8].parse(";", "=");
}

SGFFFile::SGFFFile() {}
SGFFFile::SGFFFile(const char *path, bool l) : SGFFFile() {
    _alias = !l;
    if (_alias) open(path);
    else load(path);
}
SGFFFile::~SGFFFile() {}

void SGFFFile::open(const char *path) {
    _file.open(path);
}
void SGFFFile::load(const char *path) {
    open(path);
    while (next()) { gff_datas.add(gff_data); }
}

const sgff &SGFFFile::data() const { return gff_data; }
const Array<sgff> &SGFFFile::list() const { return gff_datas; }

bool SGFFFile::next() {
    if(!_file.isOpened() || _file.eof()) return false;
    else {
        _file.readLine(_row);
        if(_row.empty()) return false;
        if(_row[0]=='#') next();
        else gff_data.set(_row);
        return true;
    }
}
bool SGFFFile::loaded() { return !_alias; }
