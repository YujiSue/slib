#include "sio/sfile.h"
#include "smedia/sfigure.h"
#include "sutil/sxml.h"
inline slib::SPointer<slib::SXmlNode> svgDeclaration(const slib::SCanvas& cnvs) {
    auto area = cnvs.frame();
    return slib::sxml::node(slib::sxml::PAIRED_TAG, "svg", sattribute({
        SS_("version", "1.1"),
        SS_("id", "svg"),
        SS_("xmlns", "http://www.w3.org/2000/svg"),
        SS_("xmlns:xlink", "http://www.w3.org/1999/xlink"),
        SS_("x", "0px"),
        SS_("y", "0px"),
        SS_("width", S(area.width) + "px"),
        SS_("height", S(area.height) + "px"),
        SS_("viewBox", "0 0 " + S(area.width) + slib::SP + S(area.height)),
        SS_("xml:space", "preserve")
        }));
}

slib::SCanvas::SCanvas() : SFigure() { _element->figid = "canvas"; }
slib::SCanvas::SCanvas(const size_t w, const size_t h) : SFigure() { _area = Area<size_t>(0, 0, w, h); }
slib::SCanvas::SCanvas(const SCanvas& cnvs) : SFigure(cnvs) { _area = cnvs._area; }
slib::SCanvas::~SCanvas() {}
const slib::Area<size_t>& slib::SCanvas::frame() const { return _area; }
void slib::SCanvas::resize(const size_t width, const size_t height) {
    _area.width = width; _area.height = height;
}
void slib::SCanvas::load(const char* path) {
    auto ext = slib::sfs::extension(path);
    if (ext == "svg") {
        SXml svg;
        svg.load(path);
        /*
        * 
        */
    }
    //else if (ext == "js") {}
    //
}
void slib::SCanvas::save(const char* path) {
    auto ext = slib::sfs::extension(path);
    if (ext == "svg") {
        SXml svg;
        svg.addChild(sxml::svgNode(*this));
        svg.save(path);
    }
    //else if (ext == "js") {}
    //
}
slib::String slib::SCanvas::getClass() const { return "canvas"; }
slib::String slib::SCanvas::toString(const char* format) const {
    auto f = S(format ? format : "svg");
    if (f == "svg") {
        SXml svg;
        auto root = svgDeclaration(*this);
        root->addChild(sxml::svgNode(*this));
        svg.addChild(root);
        return svg.toString();
    }
    //else if (f == "js") {}
    else throw FormatException(formatErrorText("Canvas format", f, "svg"));
}
slib::SObject* slib::SCanvas::clone() const { return new SCanvas(*this); }