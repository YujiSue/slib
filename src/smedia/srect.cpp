#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

SRectangle::SRectangle() : SFigure(sshape::RECTANGLE) {}
SRectangle::SRectangle(float x, float y, float w, float h, const SPaint &paint) : SRectangle() {
    addVertex(v2f(x, y));
    addVertex(v2f(x, y+h));
    addVertex(v2f(x+w, y+h));
    addVertex(v2f(x+w, y));
    _paint = paint;
}
SRectangle::SRectangle(float x, float y, float w, float h, const SDictionary &dic) : SFigure(sshape::RECTANGLE, dic) {
    addVertex(v2f(x, y));
    addVertex(v2f(x, y+h));
    addVertex(v2f(x+w, y+h));
    addVertex(v2f(x+w, y));
}
SRectangle::SRectangle(const SRectangle &s) : SFigure(s) {}
SRectangle::~SRectangle() {}

float SRectangle::width() const { return _boundary.width; }
float SRectangle::height() const { return _boundary.height; }

String SRectangle::getClass() const {
    //if (_paint.filter) return "";
    return "rect";
}
String SRectangle::toString() const {
    String str = getClass()+":{";
    if (vnum()) {
        sforeach(_vertex) str<<"("<<E_.x<<","<<E_.y<<"),";
        str.last() = '}';
    }
    str+="}";
    return str;
    
}
SObject *SRectangle::clone() const { return new SRectangle(*this); }

SEllipse::SEllipse() : SRectangle() { _type = sshape::ELLIPSE; }
SEllipse::SEllipse(float x, float y, float rad, const SPaint &paint) : SRectangle(x, y, rad, rad, paint) { _type = sshape::CIRCLE; }
SEllipse::SEllipse(float x, float y, float rad, const SDictionary &dic) : SRectangle(x, y, rad, rad, dic) { _type = sshape::CIRCLE; }
SEllipse::SEllipse(float x, float y, float hrad, float vrad, const SPaint &paint) : SRectangle(x, y, hrad, vrad, paint) { _type = hrad==vrad?sshape::CIRCLE:sshape::ELLIPSE; }
SEllipse::SEllipse(float x, float y, float hrad, float vrad, const SDictionary &dic) : SRectangle(x, y, hrad, vrad, dic) { _type = hrad==vrad?sshape::CIRCLE:sshape::ELLIPSE; }
SEllipse::SEllipse(const SRectangle &rect) : SRectangle(rect) {
    _type = width()==height()?sshape::CIRCLE:sshape::ELLIPSE;
}
SEllipse::SEllipse(const SEllipse &elps) : SRectangle(elps) {
    _type = width()==height()?sshape::CIRCLE:sshape::ELLIPSE;
}
SEllipse::~SEllipse() {}

svec2d<v2f> SEllipse::focus() const {
    v2f f1 = center(), f2 = center();
    if (width() < height()) {
        float dis = sqrt(_boundary.height*_boundary.height-_boundary.width*_boundary.width);
        f1.y += dis; f2.y -= dis;
    }
    else {
        float dis = sqrt(_boundary.width*_boundary.width-_boundary.height*_boundary.height);
        f1.x += dis; f2.x -= dis;
    }
    return svec2d<v2f>(f1, f2);
}
double SEllipse::length() const {
    float ratio = width()<height()?width()/height():height()/width();
    float e = 1-ratio*ratio;
    return 2*smath::PI*(1-e/4.0f);
}
double SEllipse::area() const { return smath::PI*width()*height(); }
bool SEllipse::include(v2f v) const {
    if (_type==sshape::CIRCLE) return sgeom::distance(v, center()) < width();
    else {
        auto f = focus(); auto l = width()<height()?height():width();
        return sgeom::distance(v, f.x)+sgeom::distance(v, f.y) < 2*l;
    }
}
String SEllipse::getClass() const {
    return _type==sshape::CIRCLE?"circle":"ellipse";
}
String SEllipse::toString() const {
    String str = getClass()+":";
    if (_type==sshape::CIRCLE)
        str<<"("<<center().x<<","<<center().y<<");"<<width()<<")";
    else {
        auto f = focus();
        str<<"("<<f.x.x<<","<<f.x.y<<"),("<<f.y.x<<","<<f.y.y<<");"<<width()<<","<<height()<<")";
    }
    return str;
}
SObject *SEllipse::clone() const { return new SEllipse(*this); }

SArc::SArc() : SEllipse() {
    _type = sshape::ARC; _phase.x = 0.0f; _phase.y = 360.0f;
}
SArc::SArc(float x, float y, float rad, float init, float end, smath::ROTATION r, const SPaint &paint)
: SEllipse(x, y, rad, paint) {
    _type = sshape::ARC; _phase.x = init; _phase.y = end; _dir = r;
}
SArc::SArc(float x, float y, float rad, float init, float end, smath::ROTATION r, const SDictionary &dic)
: SEllipse(x, y, rad, dic) {
    _type = sshape::ARC; _phase.x = init; _phase.y = end; _dir = r;
}
SArc::SArc(float x, float y, float hrad, float vrad,
           float init, float end, smath::ROTATION r, const SPaint &paint)
: SEllipse(x, y, hrad, vrad, paint) {
    _type = sshape::ARC; _phase.x = init; _phase.y = end; _dir = r;
}
SArc::SArc(float x, float y, float hrad, float vrad,
           float init, float end, smath::ROTATION r, const SDictionary &dic)
: SEllipse(x, y, hrad, vrad, dic) {
    _type = sshape::ARC; _phase.x = init; _phase.y = end; _dir = r;
}
SArc::SArc(const SArc &arc) : SEllipse(arc) {
    _type = sshape::ARC; _phase = arc._phase; _dir = arc._dir;
}
SArc::~SArc() {}

v2f SArc::phase() const { return _phase; }
smath::ROTATION SArc::direction() const { return _dir; }
void SArc::setPhase(v2f ph, smath::ROTATION r) { _phase = ph; _dir = r; }
//double length() const;
//double area() const;
//bool include(v2f v) const;
String SArc::getClass() const {
    return "arc";
}
String SArc::toString() const {
    String str = getClass()+":";
    auto f = focus();
    str<<"("<<f.x.x<<","<<f.x.y<<"),("<<f.y.x<<","<<f.y.y<<");"<<width()<<","<<height()<<";"<<_phase.x<<","<<_phase.y<<":"<<(_dir==CW?"CW":"CCW")<<")";
    return str;
}
SObject *SArc::clone() const { return new SArc(*this); }
