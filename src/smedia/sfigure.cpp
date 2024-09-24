#include "smedia/sfigure.h"
template<typename T>
inline void _updateBnd(slib::Area<float> &boundary, slib::smath::Vector<slib::smath::VectorND<T, 2>> &vertex) {
	boundary.ori_x = vertex[0][0];
	boundary.ori_y = vertex[0][1];
	boundary.width = 0;
	boundary.height = 0;
	sforeach(pt, vertex) {
		if (pt[0] < boundary.ori_x) {
			boundary.width += (boundary.ori_x - pt[0]);
			boundary.ori_x = pt[0];
		}
		else if (boundary.ori_x + boundary.width < pt[0]) {
			boundary.width = pt[0] - boundary.ori_x;
		}
		if (pt[1] < boundary.ori_y) {
			boundary.height += (boundary.ori_y - pt[1]);
			boundary.ori_y = pt[1];
		}
		else if (boundary.ori_y + boundary.height < pt[1]) {
			boundary.height = pt[1] - boundary.ori_y;
		}
	}
}

slib::FigureElement::FigureElement() {}
slib::FigureElement::~FigureElement() {}

size_t slib::FigureElement::num() const { return 0; }
void slib::FigureElement::setVertex(int idx, const smath::Vector2D<float>& pt) {}
void slib::FigureElement::expand(const smath::Vector2D<float>& vec) {}
void slib::FigureElement::translate(const smath::Vector2D<float>& vec) {}
void slib::FigureElement::rotate(const float rot) {}
void slib::FigureElement::shear(const float shear, smath::AXIS ax) {}
void slib::FigureElement::reflect(smath::AXIS ax) {}
void slib::FigureElement::updateBoundary() {}

slib::String slib::FigureElement::getClass() const { return "figure"; }

slib::SPoint::SPoint() : Point2D<float>(), FigureElement() {}
slib::SPoint::SPoint(const SPoint& pt) : Point2D<float>(pt), FigureElement(pt) {}
slib::SPoint::~SPoint() {}

size_t slib::SPoint::num() const { return vertex.size(); }
void slib::SPoint::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SPoint::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SPoint::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SPoint::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SPoint::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SPoint::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SPoint::getClass() const { return "point"; }

slib::SLine::SLine() : Line2D<float>(), FigureElement() {}
slib::SLine::SLine(const float x1, const float y1, const float x2, const float y2, const Stroke& strk)
	: Line2D<float>(svec2f(x1, y1), svec2f(x2, y2)), FigureElement() { stroke = strk; updateBoundary(); }
slib::SLine::SLine(const svec2f& p1, const svec2f& p2) : Line2D<float>(p1, p2), FigureElement() { updateBoundary(); }
slib::SLine::SLine(const svec2f& p1, const svec2f& p2, const Stroke& stk) : Line2D<float>(p1, p2), FigureElement() { stroke = stk; }
slib::SLine::SLine(const SLine& line) : Line2D<float>(line), FigureElement(line) {}
slib::SLine::~SLine() {}

size_t slib::SLine::num() const { return vertex.size(); }
void slib::SLine::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SLine::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SLine::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SLine::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SLine::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SLine::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SLine::getClass() const { return "line"; }

slib::SCurve::SCurve() : Curve2D<float>(), FigureElement() {}
slib::SCurve::SCurve(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const Stroke& strk)
	: Curve2D<float>(svec2f(x1, y1), svec2f(x2, y2), svec2f(x3, y3), svec2f(x4, y4)), FigureElement() { stroke = strk; updateBoundary(); }
slib::SCurve::SCurve(const smath::Vector2D<float>& p1, const smath::Vector2D<float>& p2, const smath::Vector2D<float>& p3, const smath::Vector2D<float>& p4, const Stroke& strk)
	: Curve2D<float>(p1, p2, p3, p4), FigureElement() { stroke = strk; updateBoundary(); }
slib::SCurve::SCurve(const SCurve& curve) : Curve2D<float>(curve), FigureElement(curve) {}
slib::SCurve::~SCurve() {}

size_t slib::SCurve::num() const { return 4; }

void slib::SCurve::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SCurve::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SCurve::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SCurve::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SCurve::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SCurve::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SCurve::getClass() const { return "curve"; }

slib::SPath::SPath() : Path2D<float>(), FigureElement() {}
slib::SPath::SPath(const sgeom::Path<float, 2>& path) : Path2D<float>(path), FigureElement() { updateBoundary(); }
slib::SPath::SPath(const SPath& path) : Path2D<float>(path), FigureElement(path) { updateBoundary(); }
slib::SPath::~SPath() {}

size_t slib::SPath::num() const { return vertex.size(); }
void slib::SPath::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SPath::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SPath::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SPath::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SPath::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SPath::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SPath::getClass() const { return "path"; }


slib::SRectangle::SRectangle() : sgeom::Rectangle<float>(), FigureElement() {}
slib::SRectangle::SRectangle(const float p1, const float p2, const float p3, const float p4) : sgeom::Rectangle<float>(p1, p2, p3, p4), FigureElement() { updateBoundary(); }
//slib::SRectangle::SRectangle(const Area<float>& area) : sgeom::Rectangle<float>(area), FigureElement() {}
slib::SRectangle::SRectangle(const SRectangle& rect) : sgeom::Rectangle<float>(rect), FigureElement(rect) {}
slib::SRectangle::~SRectangle() {}

size_t slib::SRectangle::num() const { return vertex.size(); }
void slib::SRectangle::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SRectangle::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SRectangle::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SRectangle::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SRectangle::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SRectangle::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SRectangle::getClass() const { return "rect"; }

slib::SEllipse::SEllipse() : sgeom::Ellipse<float>(), FigureElement() {}
slib::SEllipse::SEllipse(const float p1, const float p2, const float p3, const float p4) : sgeom::Ellipse<float>(p1, p2, p3, p4), FigureElement() { updateBoundary(); }
slib::SEllipse::SEllipse(const Area<float>& area) : sgeom::Ellipse<float>(area), FigureElement() { updateBoundary(); }
slib::SEllipse::SEllipse(const SEllipse& elps) : sgeom::Ellipse<float>(elps), FigureElement(elps) {}
slib::SEllipse::~SEllipse() {}


size_t slib::SEllipse::num() const { return vertex.size(); }
void slib::SEllipse::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SEllipse::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SEllipse::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SEllipse::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SEllipse::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SEllipse::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SEllipse::getClass() const { return "ellipse"; }

slib::SArc::SArc() : sgeom::Arc<float>(), FigureElement() {}
slib::SArc::SArc(const SArc& arc) : sgeom::Arc<float>(arc), FigureElement(arc) {}
slib::SArc::~SArc() {}

size_t slib::SArc::num() const { return vertex.size(); }
void slib::SArc::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SArc::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SArc::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SArc::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SArc::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SArc::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SArc::getClass() const { return "arc"; }

slib::SPolygon::SPolygon() : sgeom::Polygon<float>(), FigureElement() {}
slib::SPolygon::SPolygon(const size_t sz) : sgeom::Polygon<float>(sz), FigureElement() {}
slib::SPolygon::SPolygon(std::initializer_list<smath::Vector2D<float>> li) : sgeom::Polygon<float>(li), FigureElement() { updateBoundary(); }
slib::SPolygon::SPolygon(const SPolygon& poly) : sgeom::Polygon<float>(poly), FigureElement(poly) {}
slib::SPolygon::~SPolygon() {}

void slib::SPolygon::setVertex(int idx, const smath::Vector2D<float>& pt) {
	vertex[idx] = pt;
	updateBoundary();
}

size_t slib::SPolygon::num() const { return vertex.size(); }
void slib::SPolygon::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SPolygon::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SPolygon::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SPolygon::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SPolygon::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SPolygon::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SPolygon::getClass() const { return "polygon"; }

slib::SPicture::SPicture() {}
/*
slib::SPicture::SPicture(const char* s, bool load) {}
slib::SPicture::SPicture(const SPicture& pict) {}
*/
slib::SPicture::~SPicture() {}

bool slib::SPicture::loaded() const { return _image.loaded(); }
slib::String slib::SPicture::url() const { return _image.url(); }
slib::SImage& slib::SPicture::image() { return _image; }
const slib::SImage& slib::SPicture::image() const { return _image; }

size_t slib::SPicture::num() const { return vertex.size(); }
void slib::SPicture::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SPicture::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SPicture::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SPicture::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SPicture::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SPicture::updateBoundary() { _updateBnd(boundary, vertex); }
slib::String slib::SPicture::getClass() const { return "pict"; }

slib::SCaption::SCaption() : sgeom::Rectangle<float>(), FigureElement() {}
slib::SCaption::SCaption(const float x, const float y, const char* s, const TextAttribute& attr) : SCaption() {
	vertex[0][0] = x;
	vertex[0][1] = y;
	setText(s);
	_attribute = attr;
}
slib::SCaption::SCaption(const SCaption& c) : sgeom::Rectangle<float>(c), FigureElement(c) {
	_text = c._text;
	_attribute = c._attribute;
}
slib::SCaption::~SCaption() {}

const char* slib::SCaption::text() const { return _text.cstr(); }
const slib::String& slib::SCaption::string() const { return _text; }
const slib::TextAttribute& slib::SCaption::attribute() const { return _attribute; }

void slib::SCaption::setText(const char* s) {
	_text = s;
	vertex[1][0] = vertex[0][0];
	vertex[1][1] = vertex[0][1] - _attribute.size;
	vertex[2][0] = vertex[0][0] + _attribute.size * 0.5f * (2 * _text.size()  - _text.length());
	vertex[2][1] = vertex[0][1];
	vertex[3][0] = vertex[2][0];
	vertex[3][1] = vertex[1][1];
	updateBoundary();
}
void slib::SCaption::setFont(const char* f, float sz) { _attribute.font = f; _attribute.size = sz; }
void slib::SCaption::setStyle(suint type) { _attribute.style = type; }
void slib::SCaption::setColor(const Color& col) { _attribute.color = col; }
slib::smath::Vector2D<float> slib::SCaption::position() const { return Shape<float, 2>::vertex[0]; }

size_t slib::SCaption::num() const { return 1; }
void slib::SCaption::expand(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::expand(vec, transform.origin, vertex); }
void slib::SCaption::translate(const smath::Vector2D<float>& vec) { smath::AffineTransform2D<float>::translate(vec, vertex); }
void slib::SCaption::rotate(const float rot) { smath::AffineTransform2D<float>::rotate(rot, transform.origin, vertex); }
void slib::SCaption::shear(const float shear, smath::AXIS ax) { smath::AffineTransform2D<float>::shear(shear, ax, transform.origin, vertex); }
void slib::SCaption::reflect(smath::AXIS ax) { smath::AffineTransform2D<float>::reflect(ax, transform.origin, vertex); }
void slib::SCaption::updateBoundary() { _updateBnd(boundary, vertex); }

slib::String slib::SCaption::getClass() const { return "caption"; }

slib::SFigure::SFigure() : SNode<SFigure>(), SObject() { _element = new FigureElement(); }
slib::SFigure::SFigure(const SPoint& pt) : SNode<SFigure>(), SObject() { _element = new SPoint(pt); }
slib::SFigure::SFigure(const SLine& ln) : SNode<SFigure>(), SObject() { _element = new SLine(ln); }
slib::SFigure::SFigure(const SCurve& crv) : SNode<SFigure>(), SObject() { _element = new SCurve(crv); }
slib::SFigure::SFigure(const SPath& path) : SNode<SFigure>(), SObject() { _element = new SPath(path); }
slib::SFigure::SFigure(const SRectangle& rect) : SNode<SFigure>(), SObject() { _element = new SRectangle(rect); }
slib::SFigure::SFigure(const SEllipse& el) : SNode<SFigure>(), SObject() { _element = new SEllipse(el); }
slib::SFigure::SFigure(const SArc& arc) : SNode<SFigure>(), SObject() { _element = new SArc(arc); }
slib::SFigure::SFigure(const SPolygon& poly) : SNode<SFigure>(), SObject() { _element = new SPolygon(poly); }
slib::SFigure::SFigure(const SPicture& pict) : SNode<SFigure>(), SObject() { _element = new SPicture(pict); }
slib::SFigure::SFigure(const SCaption& cap) : SNode<SFigure>(), SObject() { _element = new SCaption(cap); }
slib::SFigure::SFigure(FigureElement* felem) : SNode<SFigure>(), SObject() { _element = felem; }
slib::SFigure::SFigure(const slib::SFigure& fig) : SNode<SFigure>(fig), SObject() {
	_element = fig._element;
}
slib::SFigure::~SFigure() {}

const slib::String& slib::SFigure::figId() const { return _element->figid; }
const slib::Area<float>& slib::SFigure::boundary() const { return _element->boundary; }
const slib::Transform2D& slib::SFigure::transform() const { return _element->transform; }
const slib::Stroke& slib::SFigure::stroke() const { return _element->stroke; }
const slib::SColor& slib::SFigure::color() const { return _element->brush; }
const slib::SDictionary& slib::SFigure::attribute() const { return _element->attribute; }

void slib::SFigure::setId(const char* s) { _element->figid = s; }

void slib::SFigure::setStroke(const Stroke& strk) { _element->stroke = strk; }
void slib::SFigure::setStrokeStyle(const sint style) { _element->stroke.type = style; }
void slib::SFigure::setStrokeWidth(const float width) { _element->stroke.width = width; }
void slib::SFigure::setStrokeColor(const SColor& col) { _element->stroke.color = col; }
void slib::SFigure::setFillColor(const SColor& col) { _element->brush = col; }
void slib::SFigure::setAttribute(const char* s, const SObjPtr& obj) { _element->attribute[s] = obj; }
void slib::SFigure::setVertex(int idx, const slib::smath::Vector2D<float>& pt) { _element->setVertex(idx, pt); }
void slib::SFigure::expand(const smath::Vector2D<float>& vec) {
	_element->expand(vec);
	sfor(*this) $_.expand(vec);
	_update();
}
void slib::SFigure::translate(const smath::Vector2D<float>& vec) {
	_element->translate(vec);
	sfor(*this) $_.translate(vec);
	_update();
}
void slib::SFigure::rotate(const float rot) {
	_element->rotate(rot);
	sfor(*this) $_.rotate(rot);
	_update();
}
void slib::SFigure::shear(const float shear, smath::AXIS ax) {
	_element->shear(shear, ax);
	sfor(*this) $_.shear(shear, ax);
	_update();
}
void slib::SFigure::reflect(smath::AXIS ax) {
	_element->reflect(ax);
	sfor(*this) $_.reflect(ax);
	_update();
}


/*
slib::SFigure::SFigure(const SPoint& pt) : Node<SFigure>(), SObject() { _fig = new SPoint(pt); }
slib::SFigure::SFigure(const SLine& ln) : Node<SFigure>(), SObject() { _fig = new SLine(ln); }
slib::SFigure::SFigure(const SCurve& cv) : Node<SFigure>(), SObject() { _fig = new SCurve(cv); }
slib::SFigure::SFigure(const SPath& path) : Node<SFigure>(), SObject() { _fig = new SPath(path); }
slib::SFigure::SFigure(const SRectangle& rect) : Node<SFigure>(), SObject() { _fig = new SRectangle(rect); }
slib::SFigure::SFigure(const SEllipse& elps) : Node<SFigure>(), SObject() { _fig = new SEllipse(elps); }
slib::SFigure::SFigure(const SArc& arc) : Node<SFigure>(), SObject() { _fig = new SArc(arc); }
slib::SFigure::SFigure(const SPolygon& poly) : Node<SFigure>(), SObject() { _fig = new SPolygon(poly); }
slib::SFigure::SFigure(const SPicture& pict) : Node<SFigure>(), SObject() { _fig = new SPicture(pict); }
slib::SFigure::SFigure(const SCalligraphy& calli) : Node<SFigure>(), SObject() { _fig = new SCalligraphy(calli); }
*/

/*
const slib::smath::Vector<slib::smath::Vector2D<float>>& slib::SFigure::vertex() const { 
	if (_element->shape) return _element->shape->vertex();
	else throw NullException(nullErrorText("Shape"));
}
*/
bool slib::SFigure::isAbstract() const { return _element->num() == 0; }
bool slib::SFigure::isGroup() const { return isAbstract() && _children.size(); }
bool slib::SFigure::isPoint() const { return !isAbstract() && slib::instanceOf<SPoint>(_element.ptr()); }
bool slib::SFigure::isLine() const { return !isAbstract() && slib::instanceOf<SLine>(_element.ptr()); }
bool slib::SFigure::isCurve() const { return !isAbstract() && slib::instanceOf<SCurve>(_element.ptr()); }
bool slib::SFigure::isPath() const { return !isAbstract() && slib::instanceOf<SPath>(_element.ptr()); }
bool slib::SFigure::isRect() const { return !isAbstract() && slib::instanceOf<SRectangle>(_element.ptr()); }
bool slib::SFigure::isEllipse() const { return !isAbstract() && slib::instanceOf<SEllipse>(_element.ptr()); }
bool slib::SFigure::isArc() const { return !isAbstract() && slib::instanceOf<SArc>(_element.ptr()); }
bool slib::SFigure::isPolygon() const { return !isAbstract() && slib::instanceOf<SPolygon>(_element.ptr()); }
bool slib::SFigure::isPict() const { return !isAbstract() && slib::instanceOf<SPicture>(_element.ptr()); }
bool slib::SFigure::isCaption() const { return !isAbstract() && slib::instanceOf<SCaption>(_element.ptr()); }

slib::SPoint& slib::SFigure::point() {
	if (isPoint()) return *dynamic_cast<SPoint*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "point"));
}
const slib::SPoint& slib::SFigure::point() const {
	if (isPoint()) return *dynamic_cast<const SPoint*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "point"));
}
slib::SLine& slib::SFigure::line() {
	if (isLine()) return *dynamic_cast<SLine*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "line"));
}
const slib::SLine& slib::SFigure::line() const {
	if (isLine()) return *dynamic_cast<SLine*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "line"));
}
slib::SCurve& slib::SFigure::curve() {
	if (isCurve()) return *dynamic_cast<SCurve*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "curve"));
}
const slib::SCurve& slib::SFigure::curve() const {
	if (isCurve()) return *dynamic_cast<const SCurve*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "curve"));
}
slib::SPath& slib::SFigure::path() {
	if (isPath()) return *dynamic_cast<SPath*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "SPath"));
}
const slib::SPath& slib::SFigure::path() const {
	if (isPath()) return *dynamic_cast<const SPath*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "SPath"));
}
slib::SRectangle& slib::SFigure::rectangle() {
	if (isRect()) return *dynamic_cast<SRectangle*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "rectangle"));
}
const slib::SRectangle& slib::SFigure::rectangle() const {
	if (isRect()) return *dynamic_cast<const SRectangle*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "rectangle"));
}
slib::SEllipse& slib::SFigure::ellipse() {
	if (isEllipse()) return *dynamic_cast<SEllipse*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "ellipse"));
}
const slib::SEllipse& slib::SFigure::ellipse() const {
	if (isEllipse()) return *dynamic_cast<const SEllipse*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "ellipse"));
}
slib::SArc& slib::SFigure::arc() {
	if (isArc()) return *dynamic_cast<SArc*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "arc"));
}
const slib::SArc& slib::SFigure::arc() const {
	if (isArc()) return *dynamic_cast<const SArc*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "arc"));
}
slib::SPolygon& slib::SFigure::polygon() {
	if (isPolygon()) return *dynamic_cast<SPolygon*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "polygon"));
}
const slib::SPolygon& slib::SFigure::polygon() const {
	if (isPolygon()) return *dynamic_cast<const SPolygon*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "polygon"));
}
slib::SPicture& slib::SFigure::pict() {
	if (isPict()) return *dynamic_cast<SPicture*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "pict"));
}
const slib::SPicture& slib::SFigure::pict() const {
	if (isPict()) return *dynamic_cast<const SPicture*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "pict"));
}
slib::SCaption& slib::SFigure::caption() {
	if (isCaption()) return *dynamic_cast<SCaption*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "cation"));
}
const slib::SCaption& slib::SFigure::caption() const {
	if (isCaption()) return *dynamic_cast<const SCaption*>(_element.ptr());
	else throw CastException(castErrorText(getClass(), "cation"));
}

void slib::SFigure::_update() {
	if (_element->num()) _element->updateBoundary();
	if (count()) _element->boundary = _children[0].boundary();
	sforeach(child, _children) {
		_element->boundary.merge(child._element->boundary);
	}
	auto fig = this;
	do {
		if (fig->isRoot()) break;
		auto &par = fig->parent();
		par._update();
		fig = &par;
	} while (true);
}
bool slib::SFigure::overlap(const slib::SFigure& fig) const {
	return boundary().overlap(fig.boundary());
}
bool slib::SFigure::include(const slib::SFigure& fig) const {
	return boundary().include(fig.boundary());
}
slib::SFigure& slib::SFigure::draw(const SFigure& fig) {
	addChild(fig);
	_update();
	return *this;
}
slib::SFigure::operator slib::Point2D<float>& () {
	if (isPoint()) return point();
	else throw CastException(castErrorText(getClass(), "Point"));
}
slib::SFigure::operator const slib::Point2D<float>& () const {
	if (isPoint()) return point();
	else throw CastException(castErrorText(getClass(), "Point"));
}
slib::SFigure::operator slib::SPoint& () {
	if (isPoint()) return point();
	else throw CastException(castErrorText(getClass(), "Point"));
}
slib::SFigure::operator const slib::SPoint& () const {
	if (isPoint()) return point();
	else throw CastException(castErrorText(getClass(), "Point"));
}

slib::String slib::SFigure::getClass() const {
	if (count()) return "group";
	else return _element->getClass();
}
slib::String slib::SFigure::toString(const char *format) const {
	
	return "";
}
slib::SObject* slib::SFigure::clone() const { return new SFigure(*this); }

/*
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

void SFigure::_makeBoundary(v2f pt) {
	if (_vertex.empty()) {
		_boundary.ori_x = pt[0]; _boundary.ori_y = pt[1];
	}
	else {
		if (_boundary.ori_x + _boundary.width < pt[0]) _boundary.width = pt[0] - _boundary.ori_x;
		if (_boundary.ori_y + _boundary.height < pt[1]) _boundary.height = pt[1] - _boundary.ori_y;
		if (pt[0] < _boundary.ori_x) {
			_boundary.width += _boundary.ori_x - pt[0];  
			_boundary.ori_x = pt[0];
		}
		if (pt[1] < _boundary.ori_y) {
			_boundary.height += _boundary.ori_y - pt[1];
			_boundary.ori_y = pt[1];
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
		_boundary = sareaf(_vertex[0][0], _vertex[0][1], 0.0f, 0.0f);
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
    if (_vertex.empty()) { _boundary.ori_x = v[0]; _boundary.ori_y = v[1]; }
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
float SFigure::length() const {
    return 2.0*(_boundary.width+_boundary.height);
}
float SFigure::area() const { return _boundary.width*_boundary.height; }
bool SFigure::include(v2f v) const { return _boundary.include(v[0], v[1]); }
bool SFigure::cross(sfig fig) const { return _boundary.overlap(fig->_boundary); }

*/


