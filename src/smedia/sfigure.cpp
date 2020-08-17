#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::smath;
using namespace slib::smedia;

SFigure::SFigure() : SNode<SFigure, FIGURE_OBJ>() {}
SFigure::SFigure(sushort t) : SFigure() { _type = t; }
SFigure::SFigure(sushort t, const SDictionary &dic) : SFigure(t) {
    if (dic["stroke"]) _paint.stroke = dic["stroke"];
    if (dic["brush"]) _paint.brush = dic["brush"];
    if (dic["filter"]) _paint.filter = dic["filter"];
}
SFigure::SFigure(const SFigure &fig) : SNode<SFigure, FIGURE_OBJ>(fig) {
	_type = fig._type;
	_trans = fig._trans;
	_paint = fig._paint;
	_boundary = fig._boundary;
	_origin = fig._origin;
	_vertex = fig._vertex;
	_attribute = fig._attribute;
}
SFigure::~SFigure() {}

void SFigure::_makeBoundary(v2f pt) {
	if (_vertex.empty()) {
		_boundary.ori_x = pt.x; _boundary.ori_y = pt.y;
	}
	else {
		if (_boundary.ori_x + _boundary.width < pt.x) _boundary.width = pt.x - _boundary.ori_x;
		if (_boundary.ori_y + _boundary.height < pt.y) _boundary.height = pt.y - _boundary.ori_y;
		if (pt.x < _boundary.ori_x) {
			_boundary.width += _boundary.ori_x - pt.x;  
			_boundary.ori_x = pt.x;
		}
		if (pt.y < _boundary.ori_y) {
			_boundary.height += _boundary.ori_y - pt.y;
			_boundary.ori_y = pt.y;
		}
	}
}
void SFigure::_resetBoundary() {
	_boundary = sareaf();
	if (childCount()) {
		_boundary = _children[0]->_boundary;
		sforeach(_children) {
			_boundary.merge(E_->boundary());
		}
	}
	if (_vertex.size()) {
		_boundary = sareaf(_vertex[0].x, _vertex[0].y, 0.0f, 0.0f);
		sforeach(_vertex) _makeBoundary(E_);
	}
}
void SFigure::_updateBoundary() {
	auto par = this;
	while (par->_parent != nullptr) {
		par = par->_parent;
		par->_resetBoundary();
	}
}
sushort SFigure::type() const { return _type; }
sareaf SFigure::boundary() const { return _boundary; }
sgeom::ORIGIN SFigure::origin() const { return _origin; }
size_t SFigure::vcount() const { return _vertex.size(); }
v2fvec& SFigure::vertex() { return _vertex; }
const v2fvec& SFigure::vertex() const { return _vertex; }
SDictionary& SFigure::attribute() { return _attribute; }
const SDictionary& SFigure::attribute() const { return _attribute; }
const STransform2D &SFigure::transformer() const { return _trans; }
const SPaint& SFigure::painter() const { return _paint; }
const SBrush& SFigure::brush() const { return _paint.brush; }
const SStroke& SFigure::stroke() const { return _paint.stroke; }

void SFigure::setOrigin(sgeom::ORIGIN ori) { _origin = ori; }
void SFigure::setScale(v2f v) { _trans.scale = v; }
void SFigure::setTranslate(v2f v) { _trans.translate = v; }
void SFigure::setSkew(v2f v) { _trans.skew = v; }
void SFigure::setRotation(float f) { _trans.rotation = f; }
void SFigure::setReflect(subyte i) { _trans.reflection = i; }
void SFigure::setStrokeType(sushort t) { _paint.stroke.type = t; }
void SFigure::setStrokeWidth(float w) { _paint.stroke.width = w; }
void SFigure::setStrokeColor(const SColor &c) { _paint.stroke.color = c; }
void SFigure::setStrokeInterval(const intarray &a) { _paint.stroke.interval = a; }
void SFigure::setStroke(const SStroke &s) { _paint.stroke = s; }
void SFigure::setFillColor(const SColor &c) {
    if (c.isClear()) _paint.brush.type = slib::sstyle::FILL_NONE;
    else _paint.brush.type = slib::sstyle::FILL_UNIFORM;
    _paint.brush.color = c;
}
void SFigure::setFillColor(const SGradient &g) {
    if (g.coordinate()&sgeom::XYZ) _paint.brush.type = slib::sstyle::LINEAR_GRAD;
    else if (g.coordinate()&sgeom::POLAR) _paint.brush.type = slib::sstyle::RADIAL_GRAD;
    _paint.brush.color = g;
}
void SFigure::setBrush(const SBrush &b) { _paint.brush = b; }
void SFigure::setPaint(const SPaint &p) { _paint = p; }
void SFigure::setAttribute(const SDictionary &dic) { _attribute = dic; }
void SFigure::expand(v2f s) {
    STransform2D::expand(s, _vertex, oriPos(_boundary, _origin));
    sforeach(_children) E_->expand(s);
	_resetBoundary(); _updateBoundary();
}
void SFigure::shift(v2f t) {
    STransform2D::shift(t, _vertex);
    sforeach(_children) E_->shift(t);
	_resetBoundary(); _updateBoundary();
}
void SFigure::shear(v2f s) {
    STransform2D::shear(s, _vertex, oriPos(_boundary, _origin));
    sforeach(_children) { 
		STransform2D::shear(s, E_->_vertex, oriPos(E_->_boundary, E_->_origin));
		E_->_resetBoundary();
	}
	_resetBoundary(); _updateBoundary();
}
void SFigure::rotate(float rot) {
    STransform2D::rotate(rot, _vertex, oriPos(_boundary, _origin));
    sforeach(_children) { 
		STransform2D::rotate(rot, E_->_vertex, oriPos(E_->_boundary, E_->_origin));
		E_->_resetBoundary();
	}
	_resetBoundary(); _updateBoundary();
}
void SFigure::reflect(subyte ref) {
    STransform2D::reflect(ref, _vertex, oriPos(_boundary, _origin));
    sforeach(_children) { 
		STransform2D::reflect(ref, E_->_vertex, oriPos(E_->_boundary, E_->_origin));
		E_->_resetBoundary();
	}
	_resetBoundary(); _updateBoundary();
}
void SFigure::addVertex(v2f v) {
    if (_vertex.empty()) { _boundary.ori_x = v.x; _boundary.ori_y = v.y; }
	_makeBoundary(v); _updateBoundary(); _vertex.add(v);
}
void SFigure::setVertex(size_t idx, v2f v) { _makeBoundary(v); _updateBoundary(); _vertex.set(idx, v); }
void SFigure::insertVertex(size_t idx, v2f v) { _makeBoundary(v); _updateBoundary(); _vertex.insert(idx, v); }
void SFigure::removeVertex(size_t idx) { _vertex.removeAt(idx); _resetBoundary(); _updateBoundary(); }
void SFigure::clearVertex() { _vertex.clear(); _resetBoundary(); _updateBoundary(); }
void SFigure::addFigure(sfig fig) {
	SNode<SFigure, FIGURE_OBJ>::addChild(fig);
	_resetBoundary(); _updateBoundary();
}
void SFigure::addFigure(SFigure&&fig) {
	SNode<SFigure, FIGURE_OBJ>::addChild(sfig(fig));
	_resetBoundary(); _updateBoundary();
}
void SFigure::addFigure(const SFigure&fig) {
	SNode<SFigure, FIGURE_OBJ>::addChild(sfig(fig));
	_resetBoundary(); _updateBoundary();
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