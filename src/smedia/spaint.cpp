#include "smedia/spaint.h"

using namespace slib;
using namespace slib::smedia;

sushort slib::smedia::SStroke::toLineStyle(const char *style) {
    String st = style;
    if (st == "solid") return sstyle::SOLID_LINE;
    if (st == "broken") return sstyle::BROKEN_LINE;
    if (st == "double") return sstyle::DOUBLE_LINE;
    return sstyle::STROKE_NONE;
}

SStroke::SStroke() : type(sstyle::SOLID_LINE), width(1.0), color(SColor::BLACK) {}
SStroke::SStroke(sushort t, float w, const SColor &col) : type(t), width(w), color(col) {}
SStroke::SStroke(sobj obj) : SStroke() {
    if (obj.hasKey("type")) type = obj["type"];
    if (obj.hasKey("width")) width = obj["width"];
    if (obj.hasKey("color")) {
        if (obj["color"].isNumeric()) color = SColor((uint32_t)obj["color"]);
        else color = color = SColor((const char *)obj["color"]);
    }
    if (obj.hasKey("inter") && !obj["inter"].empty()) {
        sforeach(obj["inter"]) interval.add(E_);
    }
}
SStroke::SStroke(const SStroke &st) {
    type = st.type;
    width = st.width;
    color = st.color;
    interval = st.interval;
}
SStroke::~SStroke() {}

SStroke &SStroke::operator=(const SStroke &st) {
    type = st.type;
    width = st.width;
    color = st.color;
    interval = st.interval;
    return *this;
}

sushort slib::smedia::SBrush::toBrushStyle(const String &style) {
    if (style == "fill") return sstyle::FILL_UNIFORM;
    if (style == "line_grad") return sstyle::LINEAR_GRAD;
    if (style == "radgrad") return sstyle::RADIAL_GRAD;
    return sstyle::FILL_NONE;
}

SBrush::SBrush() : type(sstyle::FILL_NONE) {}
SBrush::SBrush(sushort t, const SColor &col) : type(t), color(col) {}
SBrush::SBrush(const SColor & col) {
	type = sstyle::FILL_UNIFORM;
	color = col;
}
SBrush::SBrush(const SBrush &brush) {
    type = brush.type;
    color = brush.color;
}
SBrush::SBrush(const sobj& obj) {
	*this = obj.color();
}
SBrush::~SBrush() {}

SBrush &SBrush::operator=(const SBrush &brush) {
    type = brush.type;
    color = brush.color;
    return *this;
}
bool SBrush::isGradient() const { return type&sstyle::FILL_GRADIENT; }
SGradient& SBrush::gradient() { return *dynamic_cast<SGradient*>(color.ptr()); }
const SGradient & SBrush::gradient() const { return *dynamic_cast<const SGradient *>(color.ptr()); }


SFilter::SFilter() {}
SFilter::SFilter(sobj obj) { _filter = obj; }
SFilter::SFilter(const SFilter &filter) { _filter = filter._filter; }
SFilter::~SFilter() {}

SPaint::SPaint() {}
SPaint::SPaint(const SStroke &s) { stroke = s; }
SPaint::SPaint(const SBrush &b) { brush = b; }
SPaint::SPaint(const SDictionary &dic) {
    if (dic["stroke"]) stroke = SStroke(dic["stroke"]);
    if (dic["brush"]) brush = SBrush(dic["brush"]);
    if (dic["filter"]) filter = SFilter(dic["filter"]);
}
SPaint::SPaint(const SPaint &p) {
    stroke = p.stroke;
    brush = p.brush;
    filter = p.filter;
}
SPaint::~SPaint() {}
