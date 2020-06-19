#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

SFigure::SFigure() : SNode<SFigure, FIGURE_OBJ>() {}
SFigure::SFigure(int t) : SFigure() { _type = t; }
SFigure::SFigure(int t, const SDictionary &dic) : SFigure() {
    _type = t;
    if (dic["stroke"]) _paint.stroke = dic["stroke"];
    if (dic["brush"]) _paint.brush = dic["brush"];
    if (dic["filter"]) _paint.filter = dic["filter"];
}
SFigure::SFigure(const SFigure &fig) : SNode<SFigure, FIGURE_OBJ>(fig) {}
SFigure::~SFigure() {}

void SFigure::_makeBoundary(v2f &point) {
    if (point.x < _boundary.ori_x) _boundary.ori_x = point.x;
    if (point.y < _boundary.ori_y) _boundary.ori_y = point.y;
    if (_boundary.ori_x+_boundary.width < point.x) _boundary.width = point.x-_boundary.ori_x;
    if (_boundary.ori_y+_boundary.height < point.y) _boundary.height = point.y-_boundary.ori_y;
}
void SFigure::_resetBoundary() {
    _boundary = sareaf();
    if (_vertex.size()) {
        auto beg = _vertex.begin(), end = _vertex.end();
        _boundary = sareaf(beg->x, beg->y, 0.0f, 0.0f); ++beg;
        while (beg < end) {
            _makeBoundary(*beg); ++beg;
        }
    }
}
void SFigure::_updateBoundary() {
    if (SNode<SFigure, FIGURE_OBJ>::isRoot()) return;
    auto parent = SNode<SFigure, FIGURE_OBJ>::_parent;
    do {
        parent->_boundary = _boundary;
        sforeach(parent->children()) parent->_boundary.merge(E_->_boundary);
        parent = parent->parent();
    } while (parent);
}


uint16_t SFigure::type() const { return _type; }
const String &SFigure::name() const { return _attribute["name"]; }
strans2d SFigure::transformer() const { return _trans; }
SPaint &SFigure::painter() { return _paint; }
const SPaint &SFigure::painter() const { return _paint; }
SStroke& SFigure::stroke() { return _paint.stroke; }
const SStroke& SFigure::stroke() const { return _paint.stroke; }
SBrush& SFigure::brush() { return _paint.brush; }
const SBrush &SFigure::brush() const { return _paint.brush; }
sareaf SFigure::boundary() const { return _boundary; }
size_t SFigure::vnum() const { return _vertex.size(); }
v2fvec &SFigure::vertex() { return _vertex; }
const v2fvec &SFigure::vertex() const { return _vertex; }

void SFigure::setName(const char *s) { _attribute["name"] = s; }
void SFigure::setOrigin(smath::sgeom::ORIGIN ori) {
    if (_trans.isNull()) _trans = STransform2D();
    _origin = oriPos(_boundary, ori);
}
void SFigure::setOrigin(v2f ori) {
    if (_trans.isNull()) _trans = STransform2D();
    _origin = ori;
}
void SFigure::setScale(v2f v) {
    if (_trans.isNull()) _trans = STransform2D();
    _trans->scale = v;
}
void SFigure::setTranslate(v2f v) {
    if (_trans.isNull()) _trans = STransform2D();
    _trans->translate = v;
}
void SFigure::setSkew(v2f v) {
    if (_trans.isNull()) _trans = STransform2D();
    _trans->skew = v;
}
void SFigure::setRot(float f) {
    if (_trans.isNull()) _trans = STransform2D();
    _trans->rotation = f;
}
void SFigure::setReflect(subyte i) {
    if (_trans.isNull()) _trans = STransform2D();
    _trans->reflection = i;
}
void SFigure::setTransform(const STransform2D &trans) {
    if (_trans.isNull()) _trans = STransform2D(trans);
    else *_trans = trans;
}
void SFigure::setStrokeType(sushort t) { _paint.stroke.type = t; }
void SFigure::setStrokeWidth(float w) { _paint.stroke.width = w; }
void SFigure::setStrokeColor(const SColor &c) { _paint.stroke.color = c; }
void SFigure::setStrokeInterval(const intarray &a) { _paint.stroke.interval = a; }
void SFigure::setStroke(const SStroke &s) { _paint.stroke = s; }
void SFigure::setFillColor(const SColor &c) {
    if (c.isClear()) _paint.brush.type = sstyle::FILL_NONE;
    else _paint.brush.type = sstyle::FILL_UNIFORM;
    _paint.brush.color = c;
}
void SFigure::setFillColor(const SGradient &g) {
    if (g.coordinate()&sgeom::XYZ) _paint.brush.type = sstyle::LINEAR_GRAD;
    else if (g.coordinate()&sgeom::POLAR) _paint.brush.type = sstyle::RADIAL_GRAD;
    _paint.brush.color = g;
}
void SFigure::setBrush(const SBrush &b) { _paint.brush = b; }
void SFigure::setPaint(const SPaint &p) { _paint = p; }
void SFigure::setAttribute(const SDictionary &dic) {
    if (dic["id"]) _attribute["name"] = dic["id"];
    if (dic["stroke"]) _paint.stroke = dic["stroke"];
    if (dic["brush"]) _paint.brush = dic["brush"];
    if (dic["filter"]) _paint.filter = dic["filter"];
}
void SFigure::expand(v2f v) {
    STransform2D::scaling(v, _origin, _vertex, _boundary);
    sforeach(_children) { E_->expand(v); }
    _updateBoundary();
}
void SFigure::shift(v2f v) {
    STransform2D::shift(v, _vertex, _boundary);
    sforeach(_children) { E_->shift(v); }
    _updateBoundary();
}
void SFigure::shear(v2f v) {
    STransform2D::shear(v, _origin, _vertex, _boundary);
    sforeach(_children) { E_->shear(v); }
    _updateBoundary();
}
void SFigure::rotate(float f) {
    STransform2D::rotate(f, _origin, _vertex, _boundary);
    sforeach(_children) { E_->rotate(f); }
    _updateBoundary();
}
void SFigure::reflect(subyte i) {
    STransform2D::reflect(i, _origin, _vertex, _boundary);
    sforeach(_children) { E_->reflect(i); }
    _updateBoundary();
}
void SFigure::transform() {
    if (_trans.isNull()) return;
    sforeach(_children) { E_->setTransform(*_trans); E_->transform(); }
    STransform2D::trans(_trans->transMatrix(), _origin, _vertex, _boundary);
    _trans->clear();
    _updateBoundary();
}

void SFigure::addVertex(v2f v) {
    if (_vertex.empty()) { _boundary.ori_x = v.x; _boundary.ori_y = v.y; }
    _vertex.add(v); _makeBoundary(v); _updateBoundary();
}
void SFigure::setVertex(size_t idx, v2f v) { _vertex.set(idx, v); _makeBoundary(v); _updateBoundary(); }
void SFigure::insertVertex(size_t idx, v2f v) { _vertex.insert(idx, v); _makeBoundary(v); _updateBoundary(); }
void SFigure::removeVertex(size_t idx) { _vertex.removeAt(idx); _resetBoundary(); _updateBoundary(); }
void SFigure::clearVertex() { _vertex.clear(); _boundary = sareaf(); _updateBoundary(); }

void SFigure::addFigure(SFigure *fig) {
    if (_children.empty()) _boundary = fig->boundary();
    else _boundary.merge(fig->boundary());
    addChild(fig);
    _updateBoundary();
}
void SFigure::addFigure(sfig &&fig) {
    if (_children.empty()) _boundary = fig->boundary();
    else _boundary.merge(fig->boundary());
    SNode<SFigure, FIGURE_OBJ>::addChild(fig);
    fig.discard(); _updateBoundary();
}
void SFigure::addFigure(sfig &fig) {
    if (_children.empty()) _boundary = fig->boundary();
    else _boundary.merge(fig->boundary());
    SNode<SFigure, FIGURE_OBJ>::addChild(fig);
    fig.share(); _updateBoundary();
}

v2f SFigure::center() const {
    return v2f(_boundary.ori_x+_boundary.width/2.0f, _boundary.ori_y+_boundary.height/2.0f);
}
double SFigure::length() const {
    return 2.0*(_boundary.width+_boundary.height);
}
double SFigure::area() const { return _boundary.width*_boundary.height; }

bool SFigure::include(v2f v) const { return _boundary.include(v.x, v.y); }
bool SFigure::cross(sfig fig) const { return _boundary.overlap(fig->_boundary); }

String SFigure::getClass() const { return "figure"; }
String SFigure::toString() const {
    String str = getClass()+":{";
    if (childCount()) {
        sforeach(_children) str<<E_.toString()<<NEW_LINE;
    }
    return str<<"}";
}
SObject *SFigure::clone() const { return new SFigure(*this); }