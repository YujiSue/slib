#include "smedia/sfigure.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::smedia;

SCanvas::SCanvas() : SFigure() {
    _type = sshape::GROUP;
}
SCanvas::SCanvas(size_t w, size_t h, const char *name, SColor col) : SFigure() {
    _type = sshape::GROUP;
    _id = name;
    _paint = SPaint({ kv("brush", V({ kv("type", sstyle::FILL_UNIFORM), kv("color", col.intColor()) }))});
    _boundary.width = w; _boundary.height = h;
}
SCanvas::SCanvas(const SCanvas &canvas) {
    _type = canvas._type;
    _id = canvas._id;
    _parent = canvas._parent;
    _children = canvas._children;
    _boundary.ori_x = canvas._boundary.ori_x; _boundary.ori_y = canvas._boundary.ori_y;
    _boundary.width = canvas._boundary.width; _boundary.height = canvas._boundary.height;
    _paint = canvas._paint;
}
SCanvas::~SCanvas() {}

void SCanvas::loadSVG(const char *path) {
    SXmlDoc doc;
    doc.load(path);
    auto node = doc.entity();
	if (node->tag != "svg") throw SMediaException(ERR_INFO, SMEDIA_ERROR | SLIB_FORMAT_ERROR, node->tag, "SVG");
}

void SCanvas::saveSVG(const char *path) {
    SXmlDoc doc(sio::SVG_FILE);
    doc.root().add(SXmlNode::svgNode(this));
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

size_t SCanvas::width() const { return _boundary.width; }
size_t SCanvas::height() const { return _boundary.height; }
sarea SCanvas::frame() const { return _frame; }
const SColor &SCanvas::background() const { return _background; }
void SCanvas::resize(size_t w, size_t h) { _boundary.width = w; _boundary.height = h; }
void SCanvas::setBackGround(const SColor &col) { _background = col; }
void SCanvas::setFrame(sarea area) { _frame = area; }

void SCanvas::addPoint(spt pt) { addFigure(pt); }
void SCanvas::addLine(sline ln) { addFigure(ln); }
void SCanvas::addPath(spath path) { addFigure(path); }
void SCanvas::addRect(srect rect) { addFigure(rect); }
void SCanvas::addPolygon(spolygon poly) { addFigure(poly); }
void SCanvas::addEllipse(sellipse elps) { addFigure(elps); }
void SCanvas::addArc(sarc arc) { addFigure(arc); }
void SCanvas::addPict(spict pic) { addFigure(pic); }
void SCanvas::addText(scalligraphy txt) { addFigure(txt); }

String SCanvas::getClass() const { return "canvas"; }
String SCanvas::toString() const {
    auto str = getClass()<<"("<<width()<<","<<height()<<","<<") {";
    if (count()) {
        sforeach(_children) {
            str<<E_->name()<<"("<<E_->boundary().ori_x<<" "<<E_->boundary().ori_y<<" "<<
            E_->boundary().width<<" "<<E_->boundary().height<<")"<<NEW_LINE;
        }
    }
    str<<"}";
    return str;
}
SObject *SCanvas::clone() const { return new SCanvas(*this); }