#include "smedia/sfigure.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::smedia;

SCanvas::SCanvas() : SDocument<SFigure>() {
    _type = sshape::GROUP;
}
SCanvas::SCanvas(size_t w, size_t h, const char *name, const SColor &col) : SDocument<SFigure>() {
    _type = sshape::GROUP;
    _title = name;
    _background = col;
	_size = v2f((float)w, (float)h);
    _frame = sareaf(0, 0, (float)w, (float)h);
}
SCanvas::SCanvas(const SCanvas &canvas) : SDocument<SFigure>(canvas) {
    _background = canvas._background;
	_size = canvas._size;
	_frame = canvas._frame;
}
SCanvas::~SCanvas() {}

void SCanvas::loadSVG(const char *path) {
    SXmlDoc doc;
    doc.load(path);
    auto node = doc.entity();
	if (node->tag != "svg") throw SMediaException(ERR_INFO, SMEDIA_ERROR | SLIB_FORMAT_ERROR, node->tag, "SVG");
}
void SCanvas::saveSVG(const char *path) {
    SXmlDoc doc(xml::SVG);
    doc.root().addChild(SXmlNode::svgNode(this));
    doc.save(path);
}

void SCanvas::load(const char *path) {
    String ext = String::lower(SFile(path).extension());
    if(ext == "svg") loadSVG(path);
    /*
     */
}
void SCanvas::save(const char *path) {
    String ext = SFile(path).extension();
    if(ext == "svg") saveSVG(path);
    /*
     */
}

size_t SCanvas::width() const { return (size_t)_size.x; }
size_t SCanvas::height() const { return (size_t)_size.y; }
v2i SCanvas::size() const { return _size; }
sareaf SCanvas::frame() const { return _frame; }
const SColor &SCanvas::background() const { return _background; }
void SCanvas::resize(size_t w, size_t h) { _size = v2i(w, h); }
void SCanvas::setBackGround(const SColor &col) { _background = col; }
void SCanvas::setFrame(sareaf area) { _frame = area; }

void SCanvas::setPaint(const SPaint& paint) { _paint = paint; }
/*
void drawPoint(v2f pos) {}
void drawLine(v2f init, v2f end);
void drawPath();
void drawRect(float x, float y, float w, float h);
void drawRect(sareaf area);
void drawPolygon(const v2fvec& v);
void drawEllipse(float x, float y, float r);
void drawEllipse(float x, float y, float w, float h);
void drawEllipse(sareaf area);
void drawArc();
void drawPict(float x, float y, const char* s);
void drawText(float x, float y, const char* s);
*/
void SCanvas::drawFigure(sfig fig) { _root.addFigure(fig); }
String SCanvas::getClass() const { return "canvas"; }
String SCanvas::toString() const {
    auto str = getClass()<<"("<<width()<<","<<height()<<","<<") {";
    if (_root.childCount()) {
        sforeach(_root.children()) {
			 str<< E_->getClass()<<"("<< E_->boundary().ori_x<<" "<< E_->boundary().ori_y<<" "<<
				 E_->boundary().width<<" "<< E_->boundary().height<<")"<<NEW_LINE;
        }
    }
    str<<"}";
    return str;
}
SObject *SCanvas::clone() const { return new SCanvas(*this); }