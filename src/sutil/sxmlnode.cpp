#include "sutil/sxml.h"
#include "smedia/sfigure.h"

using namespace slib;
using namespace slib::sio;
using namespace slib::smedia;

inline void xml::encodeXML(String &str) {
    str.replace("&", "&amp;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&apos;");
	str.replace("<", "&lt;");
	str.replace(">", "&gt;");
}
inline void xml::decodeXML(String& str) {
	str.replace("&quot;", "\"");
	str.replace("&apos;", "\'");
	str.replace("&lt;", "<");
	str.replace("&gt;", ">");
	str.replace("&amp;", "&");
}

SXmlNode::SXmlNode() : SNode<SXmlNode>() {}
SXmlNode::SXmlNode(suint t, const char* s, const char* c) : SNode<SXmlNode>() {
    type = t;
    if (s) tag = s;
	if (c) content = c;
}
SXmlNode::~SXmlNode() {}

inline String _plistTag(const sobj &obj) {
    if (obj.isNum()) {
        auto type = obj.type();
        switch (type) {
            case SNumber::INTEGER:
                return "integer";
            case SNumber::UINTEGER:
                return "integer";
            case SNumber::REAL:
                return "real";
            case SNumber::BOOL:
                return obj?"true":"false";
            default:
                return "string";
        }
    }
    else if (obj.isDate()) return "date";
    else if (obj.isDat()) return "data";
    else if (obj.isArray()) return "array";
    else if (obj.isDict()) return "dict";
    return "string";
}
sxnode SXmlNode::plistNode(const sobj &obj) {
    sxnode node(xml::PAIRED_TAG, _plistTag(obj));
    if(obj.isStr() || obj.isDat()) node->content = obj;
    else if(obj.isNum()) {
        node->content = obj;
        if(obj.type() == SNumber::BOOL) node->type = xml::SINGLE_TAG;
    }
    else if(obj.isDate()) {
        node->content = obj.date().toString();
    }
    else if(obj.isArray()) {
        if(obj.empty()) node->type = xml::SINGLE_TAG;
        else sforeach(obj) node->addChild(SXmlNode::plistNode(E_));
    }
    else if(obj.isDict()) {
        if(obj.empty()) node->type = xml::SINGLE_TAG;
        auto keys = obj.hasKey("_key")?obj["_key"].split(","):obj.keyset();
        sforeach(keys) {
            node->addChild(sxnode(xml::PAIRED_TAG, "key", E_));
            node->addChild(SXmlNode::plistNode(obj[E_]));
        }
    }
    return node;
}
sobj SXmlNode::toPlistObj(const sxnode& node) {
	if (node->tag == "integer") return node->content.integer();
	else if (node->tag == "real") return node->content.real();
	else if (node->tag == "true") return true;
	else if (node->tag == "false") return false;
	else if (node->tag == "string") return node->content;
	else if (node->tag == "date") return SDate(node->content, "auto");
	else if (node->tag == "data") return SData(node->content);
	else if (node->tag == "array") {
		sarray array;
		sforeach(node->children()) array.add(SXmlNode::toPlistObj(E_));
		return array;
	}
	else if (node->tag == "dict") {
		sdict dict;
		sforeach(node->children()) {
			auto key = E_->content; NEXT_;
			dict[key] = SXmlNode::toPlistObj(E_);
		}
		return dict;
	}
	return snull;
}

void SXmlNode::fillSVG(SDictionary&attribute, const smedia::SBrush &brush, const char* fid) {
    switch (brush.type) {
        case sstyle::FILL_NONE:
            attribute["fill"] = "none";
            break;
        case sstyle::FILL_UNIFORM:
	        attribute["fill"] = brush.color->toString(SColor::HTML_HEX);
            if (brush.color->hasAlpha())
                attribute["fill-opacity"] = brush.color->alphaf();
            break;
        case sstyle::LINEAR_GRAD:
			attribute["fill"] = String("url(#lgrad-") << fid << ")";
            break;
        case sstyle::RADIAL_GRAD:
            attribute["fill"] = String("url(#rgrad-") << fid << ")";
            break;
        default:
            break;
    }
}
void SXmlNode::strokeSVG(SDictionary &attribute, const smedia::SStroke &stroke) {
	if (stroke.type == sstyle::STROKE_NONE) {
		attribute["stroke-width"] = 0; return;
	}
	attribute["stroke"] = stroke.color->toString(SColor::HTML_HEX);
	attribute["stroke-width"] = stroke.width;
	if (stroke.color->hasAlpha())
		attribute["stroke-opacity"] = stroke.color->alphaf();
	auto edge = stroke.type & 0x0F00;
	switch (edge) {
	case sstyle::BUTT_CAP:
		attribute["stroke-linecap"] = "butt";
		break;
	case sstyle::ROUND_CAP:
		attribute["stroke-linecap"] = "round";
		break;
	case sstyle::SQUARE_CAP:
		attribute["stroke-linecap"] = "square";
		break;
	default:
		break;
	}
	auto joint = stroke.type & 0xF000;
	switch (joint) {
	case sstyle::MITER_JOIN:
		attribute["stroke-linejoin"] = "miter";
		break;
	case sstyle::ROUND_JOIN:
		attribute["stroke-linejoin"] = "round";
		break;
	case sstyle::BEVEL_JOIN:
		attribute["stroke-linejoin"] = "bevel";
		break;
	default:
		break;
	}
	if (stroke.type & sstyle::BROKEN_LINE && !stroke.interval.empty())
		attribute["stroke-dasharray"] = slib::toString(stroke.interval, ",");
}
void SXmlNode::txtstyleSVG(SDictionary&attribute, text_style &tattr) {
	String style = "";
	if (tattr.type & sstyle::BOLD) style << "font-weight: bold; ";
	if (tattr.type & sstyle::ITALIC) style << "font-style: italic; ";
	if (tattr.type & sstyle::UNDERLINE) style << "text-decoration: underline; ";
	if (tattr.type & sstyle::DEPRECATE) style << "text-decoration: line-through; ";
	if (tattr.type & sstyle::OVERLINE) style << "text-decoration: overline; ";
	style << "font-family: " << tattr.font << "; ";
	style << "font-size: " << tattr.size << "px; ";
	style << "stroke: " << "none; ";
	style << "fill: " << tattr.color.toString(SColor::HTML_HEX) << "; ";
	attribute["style"] = style;
}

sxnode SXmlNode::svgNode(SCanvas *cnvs) {
	sxnode node(xml::START_TAG, "svg");
    node->attribute["version"] = "1.1";
    node->attribute["id"] = "svg";
    node->attribute["xmlns"] = "http://www.w3.org/2000/svg";
    node->attribute["xmlns:xlink"] = "http://www.w3.org/1999/xlink";
    node->attribute["x"] = "0px";
    node->attribute["y"] = "0px";
    node->attribute["width"] = String(cnvs->width())+"px";
    node->attribute["height"] = String(cnvs->height())+"px";
	node->attribute["viewBox"] = String("0 0 ") + String(cnvs->width()) + " " + String(cnvs->height());
    node->attribute["xml:space"] = "preserve";
    sforeach(cnvs->root().children()) {
        if (E_->brush().type == sstyle::LINEAR_GRAD || E_->brush().type == sstyle::RADIAL_GRAD) {
            auto brush = E_->brush();
            auto &gcolor = brush.gradient();
			
            auto defs = sxnode(xml::START_TAG, "defs", nullptr);
            if (brush.type == sstyle::LINEAR_GRAD) {
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
}

sxnode SXmlNode::svgNode(SFigure *fig) {
	sxnode node(xml::SINGLE_TAG, "");
	node->attribute = fig->attribute();
    String transform = "";
	auto& trans = fig->transformer();
	if (trans.rotation != 0.0f) {
		transform << "rotate(" << smath::rad2deg(trans.rotation) << "," << fig->center().x << "," << fig->center().y << ") ";
	}
	if (trans.skew.x != 0.0f)
		transform << "skewX(" << trans.skew.x << ") ";
	if (trans.skew.y != 0.0f)
		transform << "skewY(" << trans.skew.y << ") ";
	if (!(trans.translate == v2f(0.0f, 0.0f)))
		transform << "translate(" << trans.translate.x << "," << trans.translate.y << ") ";
	if (!(trans.scale == v2f(1.0f, 1.0f)))
		transform << "scale(" << trans.scale.x << "," << trans.scale.y << ") ";
	if (transform.length()) transform.resize(transform.size() - 1);

    switch (fig->type()) {
        case sshape::POINT:
            break;
        //case sshape::MULTI_POINT:
        //    break;
        case sshape::LINE:
        {
            node->tag = "line";
            node->attribute["x1"] = String(fig->vertex()[0].x);
            node->attribute["y1"] = String(fig->vertex()[0].y);
            node->attribute["x2"] = String(fig->vertex()[1].x);
            node->attribute["y2"] = String(fig->vertex()[1].y);
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::CURVE:
        {
            node->tag = "path";
            String path;
            if (fig->vcount()) {
                sforeach(fig->vertex()) {
                    path<<"M"<<E_.x<<","<<E_.y; ++it;
                    path<<"C"<<E_.x<<","<<E_.y<<" "; ++it;
                    path<<E_.x<<","<<E_.y<<" "; ++it;
                    path<<E_.x<<","<<E_.y;
                }
            }
            node->attribute["d"] = path;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::PATH:
        {
            node->tag = "path";
            String path;
            if (fig->vcount()) {
                sforeach(fig->vertex()) {
                    path<<"M"<<E_.x<<","<<E_.y; ++it;
                    path<<"L"<<E_.x<<","<<E_.y;
                }
            }
            node->attribute["d"] = path;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::POLYGON:
        {
            node->tag = "polygon";
            String pts;
            if (fig->vcount()) {
                sforeach(fig->vertex()) pts<<E_.x<<" "<<E_.y<<" ";
            }
            if (pts.size()) pts.resize(pts.size()-1);
            node->attribute["points"] = pts;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD || fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::RECTANGLE:
        {
            node->tag = "rect";
            node->attribute["x"] = fig->vertex()[0].x;
            node->attribute["y"] = fig->vertex()[0].y;
            node->attribute["width"] = fig->vertex()[2].x-fig->vertex()[0].x;
            node->attribute["height"] = fig->vertex()[2].y-fig->vertex()[0].y;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::CIRCLE:
        {
            node->tag = "circle";
            auto circ = dynamic_cast<const SEllipse *>(fig);
            node->attribute["cx"] = circ->center().x;
            node->attribute["cy"] = circ->center().y;
            node->attribute["r"] = circ->width()/2.0f;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::ELLIPSE:
        {
            node->tag = "ellipse";
            auto elps = static_cast<const SEllipse *>(fig);
            auto cent = elps->center();
            auto w = ss(elps->width()), h = ss(elps->height());
            node->attribute["cx"] = elps->center().x;
            node->attribute["cy"] = elps->center().y;
            node->attribute["rx"] = elps->width();
            node->attribute["ry"] = elps->height();
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::ARC:
        {
            node->tag = "path";
            auto arc = static_cast<const SArc *>(fig);
            auto cent = arc->center();
            auto rw = arc->width()/2.0f, rh = arc->height()/2.0f;
            auto angle = arc->phase();
            String path;
            path<<"M"<<cent.x+rw*cos(smath::deg2rad(angle[0]))<<","<<
            cent.y+(arc->direction()==CCW?-1:1)*rh*sin(smath::deg2rad(angle[0]));
            path<<"A"<<rw<<","<<rh<<",0,0,0,";
            path<<cent.x+rw*cos(smath::deg2rad(angle[1]))<<","<<
            cent.y+(arc->direction()==CCW?-1:1)*rh*sin(smath::deg2rad(angle[1]));
            node->attribute["d"] = path;
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        case sshape::TEXT:
        {
            auto txt = static_cast<SCalligraphy *>(fig);
            node->type = xml::START_TAG;
            node->tag = "text";
            node->attribute["x"] = fig->vertex()[0].x;
            node->attribute["y"] = fig->vertex()[0].y;
            SXmlNode::txtstyleSVG(node->attribute, txt->style());
            node->content = txt->text();
            break;
        }
        case sshape::GROUP:
        {
            node->type = xml::START_TAG;
            node->tag = "g";
            sforeach(fig->children()) {
                if (E_->brush().type == sstyle::LINEAR_GRAD ||
					E_->brush().type == sstyle::RADIAL_GRAD) {
                    auto brush = E_->brush();
                    auto &gcolor = brush.gradient();
                    auto defs = sxnode(xml::START_TAG, "defs", nullptr);
                    if (brush.type == sstyle::LINEAR_GRAD) {
                        auto lgrad = sxnode(xml::START_TAG, "linearGradient", nullptr);
                        lgrad->attribute = { kv("id", "lgrad-"+ slib::toString(E_->address(), "-")) };
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
                        rgrad->attribute = { kv("id", "rgrad-"+ slib::toString(E_->address(), "-")) };
                        for (int f = 0; f < gcolor.count(); ++f) {
                            auto stop = sxnode(xml::SINGLE_TAG, "stop", nullptr);
                            stop->attribute = {
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
            SXmlNode::strokeSVG(node->attribute, fig->stroke());
            if (fig->brush().type == sstyle::LINEAR_GRAD ||
                fig->brush().type == sstyle::RADIAL_GRAD) {
                SXmlNode::fillSVG(node->attribute, fig->brush(), slib::toString(fig->address(), "-"));
            }
            else SXmlNode::fillSVG(node->attribute, fig->brush(), nullptr);
            if (transform.length())
                node->attribute["transform"] = transform;
            break;
        }
        default:
            throw SMediaException(ERR_INFO, SLIB_FORMAT_ERROR, std::to_string(fig->type()).c_str(), "SLIB SHAPE");
            break;
    }
    return node;
}

void slib::SXmlNode::parseTag(const char* s) {
	String s_ = String::trim(s);
	auto list = s_.split(" ");
	tag = list[0];
	if (1 < list.size()) {
		sforin(i, 1, list.size()) {
			auto dat = list[i].split("=");
			xml::decodeXML(dat[1]);
			attribute[dat[0]] = String::dequot(dat[1]);
		}
	}
}
void SXmlNode::parse(const char *s) {
    String str = String::dequot(s);
    if (str[0] == '?' && str.last() == '?') {
        parseTag(str.substring(1, str.length()-2));
        if (tag == "xml") type = xml::DEFINITION_NODE;
    }
    else if (str[0] == '!') {
        if(str.beginWith("![CDATA[") && str.endWith("]]")) {
			type = xml::CDATA_NODE;
			content = str.substring(8, str.length()-10);
        }
        else if(str.beginWith("!--") && str.endWith("--")) {
			type = xml::COMMENT_NODE;
			content = str.substring(3, str.length()-5);
        }
        else if(str.beginWith("!DOCTYPE")) {
			type = xml::DOCTYPE_NODE;
            auto array = str.split(" ");
			tag = array[1];
			if (array[2] == "PUBLIC") {
				type = xml::DOCTYPE_PUB_NODE;
				attribute["public"] = String::dequot(array[3]);
				attribute["dtd"] = String::dequot(array[4]);
			}
			else if (array[2] == "SYSTEM") {
				type = xml::DOCTYPE_SYS_NODE;
				attribute["dtd"] = String::dequot(array[3]);
			}
			else if (array[2] == "[") {
				/*
				 */
			}
			else throw SIOException(ERR_INFO, SLIB_FORMAT_ERROR, str, "XML");
        }
        else throw SIOException(ERR_INFO, SLIB_FORMAT_ERROR, str, "XML");
    }
    else {
        if (str[0] == '/') type = xml::CLOSE_TAG;
        else if (str.last() == '/') {
			type = xml::SINGLE_TAG;
            parseTag(str.substring(0, str.length()-1));
        }
        else {
			type = xml::START_TAG;
            parseTag(str);
        }
    }
}
String SXmlNode::toString() const {
	size_t l = layer();
	if (type == xml::DEFINITION_NODE)
		return String("<?xml") <<
		(attribute["version"] ? " version=" + String::dquot(attribute["version"]) : "") <<
		(attribute["encoding"] ? " encoding=" + String::dquot(attribute["encoding"]) : "") << "?>" << NEW_LINE;
	else if (type == xml::CDATA_NODE)
		return String::TAB * l << "<![CDATA[" << content << "]]>" << NEW_LINE;
	else if (type == xml::COMMENT_NODE)
		return String::TAB * l << "<!--" << content << "-->" << NEW_LINE;
	else if (type & xml::DOCTYPE_NODE) {
		auto doc = String("<!DOCTYPE ") << tag << " ";
		if (type == xml::DOCTYPE_PUB_NODE)
			return doc << "PUBLIC " << String::dquot(attribute["public"]) << " " << String::dquot(attribute["dtd"]) << ">" << NEW_LINE;
		else if (type == xml::DOCTYPE_SYS_NODE)
			return doc << "SYSTEM " << String::dquot(attribute["dtd"]) << ">" << NEW_LINE;
		else {
			doc += "[" + NEW_LINE;
			/*
			 */
			return doc << "]>" << NEW_LINE;
		}
	}
	else if (type != xml::HIDDEN_TAG) {
		String xstr = String::TAB * l << "<" << tag;
		if (!attribute.empty()) {
			auto keys = attribute.hasKey("_key") ? attribute["_key"].split(",") : attribute.keyset();
			sforeach(keys) {
				String tmp = attribute[E_];
				xml::encodeXML(tmp);
				xstr << " " << E_ << "=" << String::dquot(tmp);
			}
		}
		if (type & xml::SINGLE_TAG) xstr << "/>" << NEW_LINE;
		else {
			xstr << ">";
			if (childCount()) {
				xstr << NEW_LINE;
				sforeach(children()) xstr << E_->toString();
				xstr << String::TAB * l;
			}
			else {
				String tmp = content;
				xml::encodeXML(tmp);
				xstr << tmp;
			}
			xstr << "</" << tag << ">" << NEW_LINE;
		}
		return xstr;
	}
	return "";
}