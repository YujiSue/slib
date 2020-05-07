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
    _frame.width = w; _frame.height = h;
}
SCanvas::SCanvas(const SCanvas &canvas) : SDocument<SFigure>(canvas) {
    _background = canvas._background;
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

size_t SCanvas::width() const { return _frame.width; }
size_t SCanvas::height() const { return _frame.height; }
sarea SCanvas::frame() const { return _frame; }
const SColor &SCanvas::background() const { return _background; }
void SCanvas::resize(size_t w, size_t h) { _frame.width = w; _frame.height = h; }
void SCanvas::setBackGround(const SColor &col) { _background = col; }
void SCanvas::setFrame(sarea area) { _frame = area; }

void SCanvas::addPoint(spt pt) { _root.addFigure(pt); }
void SCanvas::addLine(sline ln) { _root.addFigure(ln); }
void SCanvas::addPath(spath path) { _root.addFigure(path); }
void SCanvas::addRect(srect rect) { _root.addFigure(rect); }
void SCanvas::addPolygon(spolygon poly) { _root.addFigure(poly); }
void SCanvas::addEllipse(sellipse elps) { _root.addFigure(elps); }
void SCanvas::addArc(sarc arc) { _root.addFigure(arc); }
void SCanvas::addPict(spict pic) { _root.addFigure(pic); }
void SCanvas::addText(scalligraphy txt) { _root.addFigure(txt); }

String SCanvas::getClass() const { return "canvas"; }
String SCanvas::toString() const {
    auto str = getClass()<<"("<<width()<<","<<height()<<","<<") {";
    if (_root.childCount()) {
        sforeach(_root.children()) {
			 str<< E_->name()<<"("<< E_->boundary().ori_x<<" "<< E_->boundary().ori_y<<" "<<
				 E_->boundary().width<<" "<< E_->boundary().height<<")"<<NEW_LINE;
        }
    }
    str<<"}";
    return str;
}
SObject *SCanvas::clone() const { return new SCanvas(*this); }