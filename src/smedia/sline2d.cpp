#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

SPoint2D::SPoint2D() : SFigure(sshape::POINT) { addVertex(v2f()); }
SPoint2D::SPoint2D(v2f v, sushort s, float f, const SPaint &paint) : SPoint2D() {
    addVertex(v); _shape = s; _size = f; setPaint(paint);
}
SPoint2D::SPoint2D(v2f v, const SDictionary &dic) : SFigure(sshape::POINT, dic) {
    addVertex(v);
    _shape = dic["shape"];
    _size = dic["size"];
}
SPoint2D::SPoint2D(const SPoint2D &p) : SFigure(p) {
    _shape = p._shape; _size = p._size;
}
SPoint2D::~SPoint2D() {}

sushort SPoint2D::shape() const { return _shape; }
float SPoint2D::size() const { return _size; }

void SPoint2D::setShape(sushort s) { _shape = s; }
void SPoint2D::setSize(float f) { _size = f; }

slib::smath::v2f SPoint2D::center() const { return _vertex[0]; }
double SPoint2D::length() const { return 0.0f; }
double SPoint2D::area() const { return 0.0f; }

bool SPoint2D::include(v2f v) const { return _boundary.ori_x == v.x && _boundary.ori_y == v.y; }
bool SPoint2D::cross(sfig fig) const {
    return fig->include(_vertex[0]);
}
String SPoint2D::getClass() const {
    return "point";
}
String SPoint2D::toString() const {
    String str = getClass()+":";
    return str<<"("<<_vertex[0].x<<","<<_vertex[0].y<<")";
}
SObject *SPoint2D::clone() const { return new SPoint2D(*this); }


SLine2D::SLine2D() : SFigure(sshape::LINE) { addVertex(v2f(0, 0)); addVertex(v2f(1.0, 0.0)); }
SLine2D::SLine2D(v2f p1, v2f p2, const SStroke &s) : SFigure(sshape::LINE) {
    addVertex(p1); addVertex(p2);
    _paint.stroke = s;
}
SLine2D::SLine2D(v2f p1, v2f p2, const SDictionary &dic) : SFigure(sshape::LINE, dic) {
    addVertex(p1); addVertex(p2);
}
SLine2D::SLine2D(const SLine2D &l) : SFigure(l) {}
SLine2D::~SLine2D() {}

void SLine2D::setStraightLine() {
    _paint.stroke.type = sstyle::SOLID_LINE;
    _paint.stroke.interval.clear();
}
void SLine2D::setDotLine(intarray &i) {
    _paint.stroke.type = sstyle::DOT_LINE;
    _paint.stroke.interval = i;
}
void SLine2D::setBrokenLine(intarray &i) {
    _paint.stroke.type = sstyle::DASHED_LINE;
    _paint.stroke.interval = i;
}

v2f SLine2D::center() const {
    return (_vertex[0]+_vertex[1])/2.0f;
}
double SLine2D::length() const { return sgeom::distance(_vertex[0], _vertex[1]); }
double SLine2D::area() const { return 0.0; }
bool SLine2D::include(v2f v) const {
    return (v.x-_vertex[0].x)*(_vertex[1].y-_vertex[0].y)==(v.y-_vertex[0].y)*(_vertex[1].x-_vertex[0].x);
}

String SLine2D::getClass() const {
    return "line";
}
String SLine2D::toString() const {
    String str = getClass();+":{";
    return str<<"("<<_vertex[0].x<<","<<_vertex[0].y<<"),("<<_vertex[1].x<<","<<_vertex[1].y<<")}";
}
SObject *SLine2D::clone() const { return new SLine2D(*this); }


SCurve2D::SCurve2D() : SFigure() { _type = sshape::CURVE; }
SCurve2D::SCurve2D(const v2fvec &v, const SPaint &p) : SCurve2D() {
    _vertex = v;
    _paint = p;
}
SCurve2D::SCurve2D(const v2fvec &v, const SDictionary &dic) : SFigure(sshape::CURVE, dic) {
    _vertex = v;
}
SCurve2D::SCurve2D(const SCurve2D &c) : SFigure(c) {}
SCurve2D::~SCurve2D() {}

//slib::smath::v2f center() const {}
//double length() const;
//double area() const;

//bool include(float x, float y) const;
//bool cross(sfig fig) const;

String SCurve2D::getClass() const { return "curve"; }
String SCurve2D::toString() const {
    String str = getClass()+":";
    
    return str;
}
SObject *SCurve2D::clone() const { return new SCurve2D(*this); }

SPath2D::SPath2D() : SFigure(sshape::PATH), _closed(false) {}
SPath2D::SPath2D(const Array<sfig> &array, const SStroke &s) : SPath2D() {
    sforeach(array) addFigure(E_);
    _paint.stroke = s;
}
SPath2D::SPath2D(const Array<sfig> &array, const SDictionary &dic) : SFigure(sshape::PATH, dic) {
    sforeach(array) addFigure(E_);
}
SPath2D::SPath2D(const SPath2D &p) : SFigure(p) {
    _closed = p._closed;
}
SPath2D::~SPath2D() {}

bool SPath2D::isClosed() const { return _closed; }
void SPath2D::setClosed(bool b) { _closed = b; }

//slib::smath::v2f center() const;
//double length() const;
//double area() const;

//bool include(float x, float y) const;
//bool cross(sfig fig) const;

String SPath2D::getClass() const { return "path"; }
String SPath2D::toString() const {
    String str = getClass()+":{";
    if (vnum()) {
        sforeach(_children) {
            str<<E_->toString()<<",";
        }
        str.last()='}';
    }
    else str<<"}";
    return str;
}
SObject *SPath2D::clone() const { return new SPath2D(*this); }