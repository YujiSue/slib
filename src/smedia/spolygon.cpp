#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

SPolygon::SPolygon() : SFigure(sshape::POLYGON) {}
SPolygon::SPolygon(size_t n) : SFigure(sshape::POLYGON) {
    _vertex.resize(n);
}
SPolygon::SPolygon(const v2fvec &v, const SPaint &paint) : SPolygon() {
    _vertex = v;
    _paint = paint;
}
SPolygon::SPolygon(const v2fvec &v, const SDictionary &dic) : SFigure(sshape::POLYGON, dic) {
    _vertex = v;
}
SPolygon::SPolygon(const SPolygon &poly) : SFigure(poly) {}
SPolygon::~SPolygon() {}

v2f SPolygon::center() const {
    v2f c; sforeach(_vertex) c += E_;
    c /= vnum(); return c;
}
double SPolygon::length() const {
    double len = 0.0;
    auto cyc = SCycleIterator<v2f>(_vertex.ptr(), 0, vnum());
    do { len += sgeom::length(*(cyc+1)-(*cyc)); ++cyc; } while (cyc.index());
    return len;
}
double SPolygon::area() const {
    double area = 0.0;
    auto cyc = SCycleIterator<v2f>(_vertex.ptr(), 0, vnum());
    do { area += (cyc->x-(cyc+1)->x)*(cyc->y+(cyc+1)->y); ++cyc;
    } while (cyc.index());
    return area*0.5;
}
String SPolygon::getClass() const { return "polygon"; }
String SPolygon::toString() const {
    String str = getClass()+":{";
    if (vnum()) {
        sforeach(_vertex) str<<"("<<E_.x<<","<<E_.y<<"),";
        str.last() = '}';
    }
    else str<<"}";
    return str;
}
SObject *SPolygon::clone() const { return new SPolygon(*this); }


SPicture::SPicture() : SRectangle() { _type = sshape::PICTURE; }
SPicture::SPicture(float w, float h, const char *s) : SRectangle(0, 0, w, h) {
    _type = sshape::PICTURE; _image.link(s);
}
SPicture::SPicture(const SPicture &pict) : SRectangle(pict) {
    _image = pict._image;
}
SPicture::~SPicture() {}

String SPicture::getClass() const { return "pict"; }
String SPicture::toString() const {
    String str = getClass()+":{";
    return str<<"link:"<<_image.file().path()<<"}";
}
SObject *SPicture::clone() const { return new SPicture(*this); }


SCalligraphy::SCalligraphy() : SFigure(sshape::TEXT) {}
SCalligraphy::SCalligraphy(float x, float y, const char *s) : SCalligraphy() {
    _text = s;
    auto w = _attribute.size*_text.length();
    auto h = _attribute.size;
    addVertex(v2f(x, y));
    addVertex(v2f(x, y+h));
    addVertex(v2f(x+w, y+h));
    addVertex(v2f(x+w, y));
}

SCalligraphy::SCalligraphy(const SCalligraphy &c) : SFigure(c) {
    _text = c._text;
    _attribute = c._attribute;
}
SCalligraphy::~SCalligraphy() {}

const char *SCalligraphy::text() const { return _text.cstr(); }
const STextStyle &SCalligraphy::style() const { return _attribute; }
void SCalligraphy::setText(const char *s) {
    _text = s;
    v2f init = _vertex[0];
    _vertex.clear();
    auto w = _attribute.size*_text.length();
    auto h = _attribute.size;
    addVertex(init);
    addVertex(v2f(init.x, init.y+h));
    addVertex(v2f(init.x+w, init.y+h));
    addVertex(v2f(init.x+w, init.y));
}
void SCalligraphy::setFont(const char *font, float size) {
    _attribute = STextStyle(sstyle::PLAIN, font, size, SColor::BLACK, SColor::CLEAR);
    v2f init = _vertex[0];
    _vertex.clear();
    auto w = _attribute.size*_text.length();
    auto h = _attribute.size;
    addVertex(init);
    addVertex(v2f(init.x, init.y+h));
    addVertex(v2f(init.x+w, init.y+h));
    addVertex(v2f(init.x+w, init.y));
}
void SCalligraphy::setStyle(uint16_t type, const char *font, float size, SColor col, SColor bg) {
    _attribute = STextStyle(type, font, size, col, bg);
    v2f init = _vertex[0];
    _vertex.clear();
    auto w = _attribute.size*_text.length();
    auto h = _attribute.size;
    addVertex(init);
    addVertex(v2f(init.x, init.y+h));
    addVertex(v2f(init.x+w, init.y+h));
    addVertex(v2f(init.x+w, init.y));
}
String SCalligraphy::getClass() const {
    return "calligraphy";
}
String SCalligraphy::toString() const {
    String str = getClass()+":";
    str<<String::DQUOT<<_text<<String::DQUOT;
    return str;
}
SObject *SCalligraphy::clone() const { return new SCalligraphy(*this); }