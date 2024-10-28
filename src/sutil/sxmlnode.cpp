#include "smath/numeric.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sio/stream.h"
#include "sio/sfile.h"
#include "sutil/scode.h"
#include "sutil/sxml.h"

slib::SPointer<slib::SXmlNode> slib::sxml::declaration(const char* s, const Map<String, String>& attr) {
    return new SXmlNode(slib::sxml::DECLARATION_NODE, s, attr);
}
slib::SPointer<slib::SXmlNode> slib::sxml::doctype() {
    return new SXmlNode(slib::sxml::DOCTYPE_NODE);
}
slib::SPointer<slib::SXmlNode> slib::sxml::leaf(const char* s, const Map<String, String>& attr, const char* c) {
    return new SXmlNode((c ? slib::sxml::SINGLE_TAG : slib::sxml::PAIRED_TAG), s, attr, c);
}
slib::SPointer<slib::SXmlNode> slib::sxml::node(const sushort t, const char* s, const Map<String, String>& attr, const char* c) {
    return new SXmlNode(t, s, attr, c);
}
slib::SPointer<slib::SXmlNode> slib::sxml::comment(const char* s) {
    return new SXmlNode(slib::sxml::CDATA_NODE, "", {}, s);
}

slib::String slib::sxml::encode(const char *s) {
    slib::String enc(s);
    enc.replace("&", "&amp;");
    enc.replace("\"", "&quot;");
    enc.replace("\'", "&apos;");
    enc.replace("<", "&lt;");
    enc.replace(">", "&gt;");
    return enc;
}
slib::String slib::sxml::decode(const char* s) {
    slib::String dec(s);
    dec.replace("&quot;", "\"");
    dec.replace("&apos;", "\'");
    dec.replace("&lt;", "<");
    dec.replace("&gt;", ">");
    dec.replace("&amp;", "&");
    return dec;
}
slib::SPointer<slib::SXmlNode> slib::sxml::plistNode(slib::SObjPtr obj) {
    if (obj.isNum()) {
        auto type = obj.type();
        switch (type) {
        case stype::INTEGER:
            return new SXmlNode("integer", {}, obj.toString());
        case stype::UINTEGER:
            return new SXmlNode("integer", {}, obj.toString());
        case stype::REAL:
            return new SXmlNode("real", {}, obj.toString());
        case stype::BOOL:
            return new SXmlNode(obj ? "true" : "false");
        default:
            return new SXmlNode("string", {}, obj.string());
        }
    }
    else if (obj.isDate()) return new SXmlNode("date", {}, obj.toString("YYYY/MM/DD HH:mm:ss"));
    else if (obj.isData()) return new SXmlNode("data", {}, obj.toString("base64"));
    else if (obj.isArray()) {
        auto node = new SXmlNode("array");
        sfor(obj) node->addChild(plistNode($_));
        return node;
    }
    else if (obj.isDict()) {
        auto node = new SXmlNode("dict");
        sfor(obj) {
            node->addChild(SXmlNode("key", {}, $_.key()));
            node->addChild(plistNode($_.value()));
        }
        return node;
    }
    return new SXmlNode("string", {}, obj.string());
}
slib::SObjPtr slib::sxml::toPlistObj(const slib::SXmlNode& node) {
    if (node.tag == "integer") return node.content[0].integer();
    else if (node.tag == "real") return node.content[0].doubleValue();
    else if (node.tag == "true") return true;
    else if (node.tag == "false") return false;
    else if (node.tag == "string") return node.content[0];
    else if (node.tag == "date") return SDate(node.content[0], "auto");
    else if (node.tag == "data") {
        sobj data = SData();
        sutil::decodeBase64(node.content[0], data);
        return data;
    }
    else if (node.tag == "array") {
        sobj array;
        sfor(node.children()) array.add(slib::sxml::toPlistObj($_));
        return array;
    }
    else if (node.tag == "dict") {
        sobj dict;
        sfor(node.children()) {
            String key = $_.content[0]; $NEXT;
            dict.set(key, slib::sxml::toPlistObj($_));
        }
        return dict;
    }
    return snull;
}

void slib::sxml::setSvgTextStyle(sattribute& attribute, const TextAttribute& attr) {
    String style;
    style << (attr.style & slib::sstyle::BOLD ? "font-weight: bold; ": "") <<
        (attr.style & slib::sstyle::ITALIC ? "font-style: italic; " : "") <<
        (attr.style & slib::sstyle::UNDERLINE ? "text-decoration: underline; ": "") << 
        (attr.style & slib::sstyle::DEPRECATE ? "text-decoration: line-through; ": "") <<
        (attr.style & slib::sstyle::OVERLINE ? "text-decoration: overline; " : "") << 
        (attr.font.size() ? "font-family: " + attr.font + ";" : "") <<
        "font-size: " << attr.size << "px; " << "stroke: " << "none; " <<
        "fill: " << attr.color.toString("html") << "; ";
    if (style.size()) attribute["style"] = style;
}

void slib::sxml::setSvgStroke(sattribute& attribute, const slib::Stroke& stroke) {
    if (stroke.type == sstyle::NO_STROKE) attribute["stroke-width"] = "0";
    else attribute["stroke-width"] = S(stroke.width);
    attribute["stroke"] = stroke.color.toString("html");
    //if (stroke.color.hasAlpha())
    //    attribute["stroke-opacity"] = stroke.color->alphaf();
    auto edge = stroke.type & 0x0F00;
    switch (edge) {
    case slib::sstyle::BUTT_CAP:
        attribute["stroke-linecap"] = "butt";
        break;
    case slib::sstyle::ROUND_CAP:
        attribute["stroke-linecap"] = "round";
        break;
    case slib::sstyle::SQUARE_CAP:
        attribute["stroke-linecap"] = "square";
        break;
    default:
        break;
    }
    auto joint = stroke.type & 0xF000;
    switch (joint) {
    case slib::sstyle::MITER_JOIN:
        attribute["stroke-linejoin"] = "miter";
        break;
    case slib::sstyle::ROUND_JOIN:
        attribute["stroke-linejoin"] = "round";
        break;
    case slib::sstyle::BEVEL_JOIN:
        attribute["stroke-linejoin"] = "bevel";
        break;
    default:
        break;
    }
    //if (stroke.type & slib::sstyle::BROKEN_LINE && !stroke.interval.empty())
    //    attribute["stroke-dasharray"] = slib::toString(stroke.interval, ",");
}

void slib::sxml::setSvgTransform(sattribute& attribute, const slib::Transform2D& transform) {
    slib::String trans = "";
    if (transform.rotation != 0.0f)
        trans << "rotate(" << slib::smath::rad2deg(transform.rotation) << "," << transform.origin[0] << "," << transform.origin[1] << ") ";
    if (transform.shear[0] != 0.0f)
        trans << "skewX(" << transform.shear[0] << ") ";
    if (transform.shear[1] != 0.0f)
        trans << "skewY(" << transform.shear[1] << ") ";
    if (transform.scale[0] != 1.0f || transform.scale[1] != 1.0f)
        trans << "scale(" << transform.scale[0] << "," << transform.scale[1] << ") ";
    if (transform.shift[0] != 0.f || transform.shift[1] != 0.f)
        trans << "translate(" << transform.shift[0] << "," << transform.shift[1] << ") ";
    if (trans.size()) {
        trans.resize(trans.size() - 1);
        attribute["transform"] = trans;
    }
}
void slib::sxml::setSvgBrush(sattribute& attribute, const slib::SColor& brush, const slib::String& fid) {
    if (brush.isGradient()) {
        if (brush.type() & scolor::LINEAR_GRAD) attribute["fill"] = String("url(#lgrad-") + fid + ")";
        else attribute["fill"] = String("url(#rgrad-") + fid + ")";
    }
    else {
        auto& col = brush.color();
        if (col.isClear()) attribute["fill"] = "none";
        else {
            attribute["fill"] = col.toString("html");
            if (col.hasAlpha()) attribute["fill-opacity"] = S((float)col[3] / 255.f);
        }
    }
}
slib::SPointer<slib::SXmlNode> slib::sxml::svgDeclaration(const slib::SCanvas& cnvs) {
    auto area = cnvs.frame();
    return slib::sxml::node(slib::sxml::PAIRED_TAG, "svg", sattribute({
        SS_("version", "1.1"),
        SS_("id", "svg"),
        SS_("xmlns", "http://www.w3.org/2000/svg"),
        SS_("xmlns:xlink", "http://www.w3.org/1999/xlink"),
        SS_("x", S(area.ori_x) + "px"),
        SS_("y", S(area.ori_y) + "px"),
        SS_("width", S(area.width) + "px"),
        SS_("height", S(area.height) + "px"),
        SS_("viewBox", "0 0 " + S(area.width) + slib::SP + S(area.height)),
        SS_("xml:space", "preserve")
        }));
}

slib::SPointer<slib::SXmlNode> slib::sxml::svgNode(const SCanvas& cnvs) {
    auto root = svgDeclaration(cnvs);
    sfor(cnvs) {
        root->addChild(svgNode($_));
    }
    return root;
}
slib::SPointer<slib::SXmlNode> slib::sxml::svgNode(const slib::SFigure& fig) {
    SXmlNode node;
    if (fig.isGroup()) {
        node = SXmlNode(slib::sxml::PAIRED_TAG, "g");
        if (fig.figId().size()) node.attribute["id"] = fig.figId();
        sforeach(child, fig) {
            if (child.color().isGradient()) {
                auto defs = slib::SXmlNode(sxml::PAIRED_TAG, "defs");
                auto& grad = child.color().grad();
                if (grad.type() & scolor::LINEAR_GRAD) {
                    /*
                    auto lgrad = slib::SXmlNode(sxml::PAIRED_TAG, "linearGradient", {
                        SS_("id", "lgrad-" + child.figId())
                        });
                        */
                    /*
                    sfor2(grad.colors()) {
                        auto cnode = SXmlNode(sxml::SINGLE_TAG, "stop", {
                            SS_("offset", ),
                            SS_("stop-color", )
                            });
                        lgrad.addChild(cnode);
                    }
                    */
                    /*
                    for (int f = 0; f < gcolor.count(); ++f) {
                        auto stop = sxnode(xml::SINGLE_TAG, "stop", nullptr);
                        stop->attribute =
                        {
                            kv("offset", gcolor.points()[f]),
                            kv("stop-color", gcolor[f].toString(SColor::HTML_HEX))
                        };
                        lgrad->addChild(stop);
                    }
                    */
                    //defs.addChild(lgrad);
                }
                else {
                    auto rgrad = slib::SXmlNode(sxml::PAIRED_TAG, "radialGradient", {
                        SS_("id", "rgrad-" + child.figId())
                        });
                    /*
                    */


                    defs.addChild(rgrad);
                }
                node.addChild(defs);
            }
            node.addChild(svgNode(child));
        }
    }
    else {
        if (fig.isAbstract()) {
            /*
            auto& area = fig.boundary();
            if (fig.attribute()["type"] == "html") {
                node = SXmlNode(sxml::PAIRED_TAG, "foreignObject", {
                    SS_("x", S(area.ori_x)),
                    SS_("y", S(area.ori_y)),
                    SS_("width", S(area.width)),
                    SS_("height", S(area.height))
                    });
                node.content = { fig.attribute()["content"] };
            }
            */
        }
        else {
            if (fig.isLine()) {
                auto& line = fig.line();
                node = SXmlNode(sxml::SINGLE_TAG, "line", {
                    SS_("x1", S(line.vertex[0][0])),
                    SS_("y1", S(line.vertex[0][1])),
                    SS_("x2", S(line.vertex[1][0])),
                    SS_("y2", S(line.vertex[1][1]))
                    });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isCurve()) {
                auto& curve = fig.curve();
                String str;
                str << "M" << curve.vertex[0][0] << "," << curve.vertex[0][1] <<
                    "C" << curve.vertex[1][0] << "," << curve.vertex[1][1] <<
                    SP << curve.vertex[2][0] << "," << curve.vertex[2][1] <<
                    SP << curve.vertex[3][0] << "," << curve.vertex[3][1];
                node = SXmlNode(sxml::SINGLE_TAG, "path", { SS_("d", str) });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isPath()) {
                auto& path = fig.path();
                String str;
                auto pt = path.vertex.begin(), last = pt;
                str << "M" << (*pt)[0] << "," << (*pt)[1];
                sforeach(p, path.pathtype()) {
                    if ((*pt) != (*last)) str << "M" << (*pt)[0] << "," << (*pt)[1];
                    if (p == (subyte)sgeom::PATH_TYPE::LINE) {
                        ++pt; str << " L" << (*pt)[0] << "," << (*pt)[1] << " ";
                    }
                    else if (p == (subyte)sgeom::PATH_TYPE::BEZIER3) {
                        ++pt; str << "C";
                        sforin(i, 0, 3) { str << (*pt)[0] << "," << (*pt)[1] << " "; ++pt; }
                    }
                    last = pt; ++pt;
                }
                if (path.isClosed()) str << "z";
                node = SXmlNode(sxml::SINGLE_TAG, "path", { SS_("d", str) });
                setSvgStroke(node.attribute, fig.stroke());
                if (path.isClosed()) setSvgBrush(node.attribute, fig.color(), fig.figId());
                setSvgTransform(node.attribute, fig.transform());

            }
            else if (fig.isRect()) {
                auto& rect = fig.rectangle();
                node = SXmlNode(sxml::SINGLE_TAG, "rect", {
                    SS_("x", S(rect.vertex[0][0])),
                    SS_("y", S(rect.vertex[0][1])),
                    SS_("width", S(rect.width())),
                    SS_("height", S(rect.height()))
                    });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgBrush(node.attribute, fig.color(), fig.figId());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isEllipse()) {
                auto& ellipse = fig.ellipse();
                auto center = ellipse.center();
                node = SXmlNode(sxml::SINGLE_TAG, "ellipse", {
                    SS_("cx", S(center[0])),
                    SS_("cy", S(center[1])),
                    SS_("rx", S(ellipse.width())),
                    SS_("ry", S(ellipse.height()))
                    });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgBrush(node.attribute, fig.color(), fig.figId());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isArc()) {
                auto& arc = fig.arc();
                auto center = arc.center();
                String str;
                auto rad = svec2f(arc.width() / 2.f, arc.height() / 2.f);
                str << "M" << center[0] + rad[0] * cos(2.0 * snum::PI * arc.phase[0]) << "," <<
                    center[1] - rad[1] * sin(2.0 * snum::PI * arc.phase[0]) <<
                    "A" << rad[0] << "," << rad[1] << ",0,0,0," <<
                    center[0] + rad[0] * cos(2.0 * snum::PI * arc.phase[1]) << "," <<
                    center[1] - rad[1] * sin(2.0 * snum::PI * arc.phase[1]);
                node = SXmlNode(sxml::SINGLE_TAG, "path", { SS_("d", str) });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgBrush(node.attribute, fig.color(), fig.figId());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isPolygon()) {
                auto& polygon = fig.polygon();
                String pts;
                sforeach(pt, polygon.vertex) pts << pt[0] << "," << pt[1] << SP;
                if (pts.size()) pts.resize(pts.size() - 1);
                node = SXmlNode(sxml::SINGLE_TAG, "polygon", { SS_("points", pts) });
                setSvgStroke(node.attribute, fig.stroke());
                setSvgBrush(node.attribute, fig.color(), fig.figId());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isPict()) {
                auto& pict = fig.pict();
                if (pict.loaded()) {
                    node = SXmlNode(sxml::SINGLE_TAG, "image", {
                        SS_("xlink:href", pict.url()),
                        SS_("width", S(pict.boundary.width)),
                        SS_("height", S(pict.boundary.height))
                        });
                }
                if (pict.figid.size()) node.attribute["id"] = pict.figid;
                setSvgStroke(node.attribute, fig.stroke());
                setSvgTransform(node.attribute, fig.transform());
            }
            else if (fig.isCaption()) {
                auto& caption = fig.caption();
                auto pos = caption.position();
                node = SXmlNode(sxml::PAIRED_TAG, "text", {
                    SS_("x", pos[0]),
                    SS_("y", pos[1])
                    });
                node.content = { caption.text() };
                setSvgTextStyle(node.attribute, caption.attribute());
                setSvgTransform(node.attribute, fig.transform());
            }
            else throw Exception();
        }
    }
    return node;
}
slib::SFigure slib::sxml::toSVGObj(const slib::SXmlNode& node) {
    



    return SFigure();
}



slib::SXmlNode::SXmlNode() : slib::SNode<slib::SXmlNode>() { type = slib::sxml::PAIRED_TAG; }
slib::SXmlNode::SXmlNode(const char* s, const sattribute& attr, const char* c) : slib::SNode<slib::SXmlNode>() {
    type = slib::sxml::PAIRED_TAG; tag = s; attribute = attr; content = { sxml::encode(c) }; 
}
slib::SXmlNode::SXmlNode(sushort t, const char* s, const sattribute& attr, const char* c) : slib::SNode<slib::SXmlNode>() {
    type = t; tag = s; attribute = attr; content = { sxml::encode(c) };
}
slib::SXmlNode::SXmlNode(const slib::SXmlNode &node) : slib::SNode<slib::SXmlNode>(node) {
    type = node.type;
    tag = node.tag;
    content = node.content;
    attribute = node.attribute;
}
slib::SXmlNode::~SXmlNode() {}
slib::SXmlNode& slib::SXmlNode::operator=(const slib::SXmlNode& node) {
    _parent = node._parent;
    _children = node._children;
    type = node.type;
    tag = node.tag;
    content = node.content;
    attribute = node.attribute;
    return *this;
}


    /*
    sforeach(cnvs->root().children()) {
        if (E_->brush().type == slib::sstyle::LINEAR_GRAD || E_->brush().type == slib::sstyle::RADIAL_GRAD) {
            auto brush = E_->brush();
            auto& gcolor = brush.gradient();

            auto defs = sxnode(xml::START_TAG, "defs", nullptr);
            if (brush.type == slib::sstyle::LINEAR_GRAD) {
                auto lgrad = sxnode(xml::START_TAG, "linearGradient", nullptr);
                lgrad->attribute = { kv("id", "lgrad-" + slib::toString(E_->address(), "-")) };
                for (int f = 0; f < gcolor.count(); ++f) {
                    auto stop = sxnode(xml::SINGLE_TAG, "stop", nullptr);
                    stop->attribute =
                    {
                        kv("offset", gcolor.points()[f]),
                        kv("stop-color", gcolor[f].toString(SColor::HTML_HEX))
                    };
                    lgrad->addChild(stop);
                }
                defs->addChild(lgrad);
            }
            else {
                auto rgrad = sxnode(xml::START_TAG, "radialGradient", nullptr);
                rgrad->attribute = { kv("id", "rgrad-" + slib::toString(E_->address(), "-")) };
                for (int f = 0; f < gcolor.count(); ++f) {
                    auto stop = sxnode(xml::SINGLE_TAG, "stop", nullptr);
                    stop->attribute =
                    {
                        kv("offset", gcolor.points()[f]),
                        kv("stop-color", gcolor[f].toString(SColor::HTML_HEX))
                    };
                    rgrad->addChild(stop);
                }
                defs->addChild(rgrad);
            }
            node->addChild(defs);
        }
        node->addChild(SXmlNode::svgNode(E_));
    }
    return node;
    */

inline void _interpretXmlAttribute(slib::SXmlNode& node) {
    if (node.tag.match(" ")) {
        auto pos = node.tag.find(" ");
        auto attrs = node.tag.substring(pos).trim().split(" ");
        node.tag.resize(pos);
        sforeach(attr, attrs) {
            if (attr.empty()) continue;
            auto kv = attr.split("=");
            if (kv.size() != 2) throw FormatException(slib::formatErrorText("XML attribute", attr, "KEY=\"VALUE\""));
            node.attribute[kv[0]] = slib::sstr::dequote(kv[1]);
        }
    }
}
inline void _interpretXmlTag(slib::SXmlNode& node) {
    if (node.tag.beginWith("<?") && node.tag.endWith("?")) {
        node.tag.clip(2, node.tag.size() - 3).trim();
        if (node.tag.beginWith("xml")) {
            node.type = slib::sxml::DECLARATION_NODE;
            _interpretXmlAttribute(node);
        } 
        else node.type = slib::sxml::INFORMATION_NODE;
    }
    else if (node.tag.beginWith("<!")) {
        if (node.tag.beginWith("<![CDATA[")) node.type = slib::sxml::CDATA_NODE; 
        else if (node.tag.beginWith("<!--")) node.type = slib::sxml::COMMENT_NODE;
        else if (node.tag.beginWith("<!DOCTYPE")) {
            node.type = slib::sxml::DOCTYPE_NODE;
            node.tag.clip(9, node.tag.size() - 10).trim();

            /*
            * Parse doctype
            */
            
        }
        else throw FormatException(slib::formatErrorText("XML node with exclamation", node.tag, "<!DOCTYPE | <!-- | <![CDATA["));
    }
    else {
        if (node.tag.endWith("/")) {
            node.type = slib::sxml::SINGLE_TAG;
            node.tag.clip(1, node.tag.size() - 2).trim();
        }
        else {
            node.type = slib::sxml::PAIRED_TAG;
            node.tag.clip(1, node.tag.size() - 1).trim();
        }
        _interpretXmlAttribute(node);
    }
}
slib::ArrayIterator<char> slib::sxml::readXmlNode(slib::ArrayIterator<char> current, const slib::ArrayIterator<char>& end, slib::SXmlNode& node) {
    // Read tag
    if (end <= current) return end;
    while (current < end && (*current) != '>') {
        node.tag << (*current);
        ++current;
    }
    node.tag.trim();
    _interpretXmlTag(node);
    if (node.type == DECLARATION_NODE || node.type == INFORMATION_NODE || node.type == DOCTYPE_NODE) return current + 1;
    // Skip psuede node (CDATA / Comment)
    if (node.type == slib::sxml::CDATA_NODE) {
        while (current < end && !node.tag.endWith("]]>")) {
            node.tag << (*current);
            ++current;
        }
        node.tag.clip(9, node.tag.size() - 12).trim();
        node.content.add(node.tag);
        node.tag.clear();
    }
    if (node.type == slib::sxml::COMMENT_NODE) {
        while (current < end && !node.tag.endWith("-->")) {
            node.tag << (*current);
            ++current;
        }
        node.tag.clip(4, node.tag.size() - 7).trim();
        node.content.add(node.tag);
        node.tag.clear();
    }
    // Content
    if (node.type == slib::sxml::PAIRED_TAG) {
        ++current;
        String tmp;
        while (current < end && (*current) != '<') {
            tmp << (*current);
            ++current;
        }
        tmp.trim();
        node.content.add(tmp);
        //
        if (*(current + 1) == '/') {
            auto nxt = current + 2;
            slib::String tag;
            while (nxt < end && (*nxt) != '>') {
                tag << (*nxt);
                ++nxt;
            }
            if (tag != node.tag) throw FormatException(slib::formatErrorText("XML node", "<" + node.tag + "..>..</" + tag + ">", "<tag .*>.*</tag>"));
            return nxt + 1;
        }
        else {
            while (current < end) {
                node.add(SXmlNode());
                current = sxml::readXmlNode(current, end, node.child(-1));
                tmp.clear();
                while (current < end && (*current) != '<') {
                    tmp << (*current);
                    ++current;
                }
                tmp.trim();
                node.content.add(tmp);
                if (*(current + 1) == '/') {
                    auto nxt = current + 2;
                    slib::String tag;
                    while (nxt < end && (*nxt) != '>') {
                        tag << (*nxt);
                        ++nxt;
                    }
                    if (tag == node.tag) { current = nxt + 1; break; }
                }
            }
            return current;
        }
    }
    else if (node.type == slib::sxml::SINGLE_TAG) return current + 1;
    return current;
}


void slib::sxml::writeXmlNode(slib::IOStream& ostream, const slib::SXmlNode& node, bool formed, int l) {
    if (node.type == slib::sxml::ROOT_NODE) {
        sfor(node.children()) writeXmlNode(ostream, $_, formed, l);
    }
    else if (node.type == slib::sxml::DECLARATION_NODE) {
        ostream << "<" << (node.tag == "xml" ? "?" : "") << node.tag <<
            (node.attribute.hasKey("version") ? " version=" + slib::sstr::dquote(node.attribute["version"]) : "");
        sforeach(attr, node.attribute) {
            if (attr.key() == "version") continue;
            ostream << SP << attr.key() << "=" << slib::sstr::dquote(attr.value());
        }
        ostream << (node.tag == "xml" ? "?" : "") << ">" << slib::NL;
    }
    else if (node.type == slib::sxml::INFORMATION_NODE) {
        ostream << "<?" << node.tag << "?>" << slib::NL;
    }
    else if (node.type & slib::sxml::DOCTYPE_NODE) {
        ostream << "<!DOCTYPE ";
        if (node.type == slib::sxml::DOCTYPE_PUB_NODE)
            ostream << "PUBLIC " << slib::sstr::dquote(node.attribute["public"]) << " " << slib::sstr::dquote(node.attribute["dtd"]) << ">" << slib::NL;
        else if (node.type == slib::sxml::DOCTYPE_SYS_NODE)
            ostream << "SYSTEM " << slib::sstr::dquote(node.attribute["dtd"]) << ">" << slib::NL;
        else {
            ostream << "[" + slib::NL;
            /*
            *
            */
            ostream << "]>" << slib::NL;
        }
    }
    else if (node.type == slib::sxml::CDATA_NODE) {
        ostream << (formed ? slib::TAB * l : "") << "<![CDATA[" << node.content << "]]>" << slib::NL;
    }
    else if (node.type == slib::sxml::COMMENT_NODE) {
        ostream << (formed ? slib::TAB * l : "") << "<!--" << node.content << "-->" << slib::NL;
    }
    else {
        ostream << (formed ? slib::TAB * l : "") << "<" << node.tag;
        sfor(node.attribute) {
            ostream << slib::SP << $_.key() << "=" << slib::sstr::dquote($_.value());
        }
        if (node.type == sxml::SINGLE_TAG || (node.type == 0 && node.content.empty() && node.count() == 0)) ostream << " />" << slib::NL;
        else {
            ostream << ">";
            if (node.count()) {
                ostream << slib::NL;
                if (node.content[0].size()) ostream << (formed ? slib::TAB * (l + 1) : "") << node.content[0] << slib::NL;
                if (node.content.size() == 1) {
                    sforeach(child, node.children()) { slib::sxml::writeXmlNode(ostream, child, formed, l + 1); }
                }
                else {
                    auto count = node.count();
                    sforin(n, 0_u, count) {
                        slib::sxml::writeXmlNode(ostream, node.child(n), formed, l + 1);
                        ostream << (formed ? slib::TAB * (l + 1) : "") << node.content[n + 1] << slib::NL;
                    }
                }
                ostream << (formed ? slib::TAB * l : "") << "</" << node.tag << ">" << slib::NL;
            }
            else ostream << node.content[0] << "</" << node.tag << ">" << slib::NL;
        }
    }
}


void slib::SXmlNode::add(const SXmlNode& node) {
    SNode<SXmlNode>::addChild(SXmlNodeElement(node));
}
void slib::SXmlNode::insert(const int idx, const SXmlNode& node) {
    SNode<SXmlNode>::insertChild(idx, SXmlNodeElement(node));
}
void slib::SXmlNode::insertBefore(const SXmlNode& node, SXmlNode& dest) {
    if (dest.isRoot()) throw Exception();
    dest.parent().insert(dest.index(), node);
}
void slib::SXmlNode::insertAfter(const SXmlNode& node, SXmlNode& dest) {
    if (dest.isRoot()) throw Exception();
    dest.parent().insert(dest.index() + 1, node);
}
bool slib::SXmlNode::match(const char* s, const sattribute& attr) const {
    if (s) {
        bool b = (tag == s);
        if (attr.size() && b) {
            sfor(attr) {
                if (attribute.hasKey($_.key()) && attribute[$_.key()].match(Regex($_.value()))) continue;
                else return false;
            }
        }
        return b;
    }
    else {
        if (attr.size()) {
            sfor(attr) {
                if (attribute.hasKey($_.key()) && attribute[$_.key()].match(Regex($_.value()))) continue;
                else return false;
            }
            return true;
        }
        return false;
    }
}

inline slib::SPointer<slib::SXmlNode> _find(slib::SXmlNode *parent, const char* s, const sattribute& attr) {
    sfor(*parent) {
        if ($_.match(s, attr)) return $.sptr();
        else if ($_.count()) {
            auto node =_find(&$_, s, attr);
            if (node) return node;
        }
    }
    return nullptr;
}
inline const slib::SPointer<slib::SXmlNode> _find(const slib::SXmlNode *parent, const char* s, const sattribute& attr) {
    sfor(*parent) {
        if ($_.match(s, attr)) return $.sptr();
        else if ($_.count()) {
            auto node =_find(&$_, s, attr);
            if (node) return node;
        }
    }
    return nullptr;
}

inline void _findall(slib::PArray<slib::SXmlNode> array, const slib::SXmlNode *parent, const char* s, const sattribute& attr) {
    sfor(*parent) {
        if ($_.match(s, attr)) array.add($.sptr());
        else if ($_.count()) _findall(array, &$_, s, attr);
    }
}

slib::PArrayIterator<slib::SXmlNode> slib::SXmlNode::find(const char* s, const sattribute& attr) {
    
    sfor(*this) {
        if ($_.match(s, attr)) return it;
        else if ($_.count()) {
            auto res = $_.find(s, attr);
            if (res != $_.end()) return res;
        }
    }
    return end();
}
slib::PArrayCIterator<slib::SXmlNode> slib::SXmlNode::find(const char* s, const sattribute& attr) const {
    sfor(*this) {
        if ($_.match(s, attr)) return it;
    }
    return end();
}
slib::Array<slib::PArrayIterator<slib::SXmlNode>> slib::SXmlNode::findAll(const char* s, const sattribute& attr) {
    slib::Array<slib::PArrayIterator<slib::SXmlNode>> array;
    sfor(*this) {
        if ($_.match(s, attr)) array.add($);
    }
    return array;
}
slib::Array<slib::PArrayCIterator<slib::SXmlNode>> slib::SXmlNode::findAll(const char* s, const sattribute& attr) const {
    slib::Array<slib::PArrayCIterator<slib::SXmlNode>> array;
    sfor(*this) {
        if ($_.match(s, attr)) array.add($);
    }
    return array;
}
slib::SXmlNode &slib::SXmlNode::search(const char *s, const sattribute& attr) {
    auto it = _find(this, s, attr);
    if (it) return $_;
    else throw NotFoundException(nofoundErrorText(s, tag));
}
const slib::SXmlNode &slib::SXmlNode::search(const char *s, const sattribute& attr) const {
    auto it = _find(this, s, attr);
    if (it) return $_;
    else throw NotFoundException(nofoundErrorText(s, tag));
}
slib::PArray<slib::SXmlNode> slib::SXmlNode::searchAll(const char *tag, const sattribute& attr) const {
    slib::PArray<slib::SXmlNode> res;
    
    return res;
}
		
slib::SXmlNode &slib::SXmlNode::operator[](const char *s) {
    sfor(*this) {
        if ($_.match(s)) return $_;
    }
    throw NotFoundException(nofoundErrorText(s, tag));
}
const slib::SXmlNode &slib::SXmlNode::operator[](const char *s) const {
    sfor(*this) {
        if ($_.match(s)) return $_;
    }
    throw NotFoundException(nofoundErrorText(s, tag));
}



void _toTxt(slib::String &str, const slib::SXmlNode& node, bool tag = false) {
    if (tag) str << "<" << node.tag << ">";
    if (node.count()) {
        str << node.content[0];
        if (node.content.size() == 1) {
            sforeach(child, node) _toTxt(str, child, true);
        }
        else {
            auto count = node.count();
            sforin(n, 0_u, count) {
                _toTxt(str, node.child(n), true);
                str << node.content[n + 1];
            }
        }
    }
    else str << node.content[0];
    if (tag) str << "</" << node.tag << ">";
}
slib::String slib::SXmlNode::text() const {
    slib::String str;
    _toTxt(str, *this);
    return str;
}

void slib::SXmlNode::parse(const char *s) {
    String str(s), tmp;
    str.trim();
    if (str[0] != '<' || str[-1] != '>') throw FormatException(formatErrorText("xml node", str, "<.*>.*</.*>"));
    sxml::readXmlNode(str.begin(), str.end(), *this);
}
slib::String slib::SXmlNode::toString(const char *format) const {
    String f(format), str;
    int layer = 0;
    IOStream ostream;
    ostream.setStrOStream(str);
    sxml::writeXmlNode(ostream, *this, f == "formed", layer);
	return str;
}

std::ostream& slib::operator<<(std::ostream& os, const slib::SXmlNode& node) { return os << node.toString(); }