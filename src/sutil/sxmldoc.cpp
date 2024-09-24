#include "sio/stream.h"
#include "sio/sfile.h"
#include "sutil/sxml.h"
slib::SXml::SXml() : SXmlNode(sxml::ROOT_NODE) {}
slib::SXml::SXml(SObjPtr obj) : SXml() {
	auto declaration = SXmlNode(sxml::DECLARATION_NODE, "xml", 
		sattribute({
			SS_("version", "1.0"),
			SS_("encoding", "utf-8") 
			}));
	auto doctype = SXmlNode(sxml::DOCTYPE_PUB_NODE, "plist",
		sattribute({
			SS_("public", "-//Apple Computer//DTD PLIST 1.0//EN"),
			SS_("dtd", "http://www.apple.com/DTDs/PropertyList-1.0.dtd")
			}));
	auto entity = SXmlNode(sxml::PAIRED_TAG, "plist",
		sattribute({
			SS_("version", "1.0")
			}));
	entity.addChild(sxml::plistNode(obj));
	addChild(declaration);
	addChild(doctype);
	addChild(entity);
}
slib::SXml::SXml(const SXml& xml) : SXml() {}
slib::SXml::~SXml() {}
/*
void SXml::setEntity(sxnode node) { 
	_root.removeChild(node);
	_entity = node;
	_root.addChild(_entity);
}
void SXml::addToEntity(sxnode node) { _entity->addChild(node); }
*/
/*
void SXml::_read(SFile &file, String &tag, SXmlNode *parent) {
    if(file.eof()) return;
    if(tag[0] == '?' && tag.last() == '?') {
		_type |= xml::XML;
        _definition = sxnode(xml::DEFINITION_NODE, "");
        _definition->parseTag(String::dequot(tag));
    }
    else if(tag[0] == '!') {
        if(tag.beginWith("![CDATA[")) {
            auto node = sxnode(xml::CDATA_NODE, "");
            if (!tag.endWith("]]")) {
                String tmp;
                file.readTo(tmp, "]]>");
				node->content = tag.substring(8) + tmp;
            }
            else node->content = tag.substring(8, tag.length()-10);
            if (parent) parent->addChild(node);
            else _root.addChild(node);
        }
        else if(tag.beginWith("!--")) {
            auto node = sxnode(xml::COMMENT_NODE, "");
            if (!tag.endWith("--")) {
                SString tmp;
                file.readTo(tmp, "-->");
				node->content = tag.substring(3) + tmp;
            }
            else node->content = tag.substring(3, tag.length()-5);
            if (parent) parent->addChild(node);
            else _root.addChild(node);
        }
        else if(tag.beginWith("!DOCTYPE ")) {
            _doctype = sxnode(xml::DOCTYPE_NODE, "");
            auto tags = tag.substring(9).split(" ");
            _doctype->tag = tags[0];
            if(tags[0] == "plist") _type |= xml::PLIST;
            else _type = xml::XML;
            if(tags[1] == "PUBLIC") {
                if (2 < tags.size()) {
                    _doctype->type = xml::DOCTYPE_PUB_NODE;
                    _doctype->attribute["public"] = tags[2];
                    if (3 < tags.size())
                        _doctype->attribute["dtd"] = tags[3];
                }
            }
            else if(tags[1] == "SYSTEM") {
                _doctype->type |= xml::DOCTYPE_SYS_NODE;
                _doctype->attribute["dtd"] = tags[2];
            }
        }
    }
    else if(tag.last() == '/') {
        auto node = sxnode(xml::START_TAG, "");
        tag.resize(tag.length()-1);
		node->parseTag(tag);
        if (!parent) _entity = node;
        else parent->addChild(node);
    }
    else {
        auto node = sxnode(xml::START_TAG, "");
        node->parseTag(tag);
        while (!file.eof()) {
            String content;
            file.readTo(content, "<");
            file.readTo(tag, ">");
            if(tag[0] == '/') {
				if (tag == '/' + node->tag) {
					xml::decodeXML(content);
					node->content = content;
					break;
				}
				else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, tag, "XML");
			}
            else _read(file, tag, node);
        }
        if (!parent) _entity = node;
        else parent->addChild(node);
    }
}
*/
/*
slib::SXmlNode& slib::SXml::operator[] (int i) { return SXmlNode::child(i); }
slib::SXmlNode& slib::SXml::operator[] (const char* tag) {
	sfor(SXmlNode::_children) {
		if ($_.tag == tag) return $_;
	}
	throw NotFoundException(slib::nofoundErrorText(tag, "document"));
}
*/
/*
slib::ArrayIterator<slib::SPointer<slib::SXmlNode>> slib::SXml::begin() { return SXmlNode::begin(); }
slib::ArrayCIterator<slib::SPointer<slib::SXmlNode>> slib::SXml::begin() const { return SXmlNode::begin(); }
slib::ArrayIterator<slib::SPointer<slib::SXmlNode>> slib::SXml::end() { return SXmlNode::end(); }
slib::ArrayCIterator<slib::SPointer<slib::SXmlNode>> slib::SXml::end() const { return SXmlNode::end(); }
*/
void slib::SXml::load(const char *path) {
	slib::SFile f(path);
	String buf;
	f >> buf;
	buf.trim();
	auto it = buf.begin();
	while (it < buf.end()) {
		addChild(SXmlNode());
		it = sxml::readXmlNode(it, buf.end(), child(-1));
	}

}
/*
inline void _write(SFile &file, sxnode node) {
	size_t l = node->layer() - 1;
	if (node->type == xml::DEFINITION_NODE)
		file << String("<?xml") <<
		(node->attribute["version"] ? " version=" + String::dquot(node->attribute["version"]) : "") <<
		(node->attribute["encoding"] ? " encoding=" + String::dquot(node->attribute["encoding"]) : "") << "?>" << NEW_LINE;
	else if (node->type == xml::CDATA_NODE)
		file << TAB * l << "<![CDATA[" << node->content << "]]>" << NEW_LINE;
	else if (node->type == xml::COMMENT_NODE)
		file << TAB * l << "<!--" << node->content << "-->" << NEW_LINE;
	else if (node->type & xml::DOCTYPE_NODE) {
		file << String("<!DOCTYPE ") << node->tag << " ";
		if (node->type == xml::DOCTYPE_PUB_NODE)
			file << "PUBLIC " << String::dquot(node->attribute["public"]) << " " << String::dquot(node->attribute["dtd"]) << ">" << NEW_LINE;
		else if (node->type == xml::DOCTYPE_SYS_NODE)
			file << "SYSTEM " << String::dquot(node->attribute["dtd"]) << ">" << NEW_LINE;
		else {
			file << "[" << NEW_LINE;
			//
			file << "]>" << NEW_LINE;
		}
	}
	else if (node->type != xml::HIDDEN_TAG) {
		file << TAB * l << "<" << node->tag;
		if (!node->attribute.empty()) {
			auto keys = node->attribute.hasKey("_key") ? node->attribute["_key"].split(",") : node->attribute.keyset();
			sforeach(keys) {
				String tmp = node->attribute[E_];
				xml::encodeXML(tmp);
				file << " " << E_ << "=" << String::dquot(tmp);
			}
		}
		if (node->type & xml::SINGLE_TAG) file << "/>" << NEW_LINE;
		else {
			file << ">";
			if (node->childCount()) {
				file << NEW_LINE;
				sforeach(node->children()) _write(file, E_);
				file << TAB * l;
			}
			else {
				String tmp = node->content;
				xml::encodeXML(tmp);
				file << tmp;
			}
			file << "</" << node->tag << ">" << NEW_LINE;
		}
		file.flush();
	}
	file.flush();
}
*/
void slib::SXml::save(const char *path, bool formed) {
	slib::SFile f(path, sio::MAKE);
	slib::IOStream fos(f, sio::OSTREAM);
	int l = 0;
	sfor(SXmlNode::_children) sxml::writeXmlNode(fos, $_, formed, l);
}



	//sforeach(child, cnvs) {
	/*
		if (child.brush().type == slib::sstyle::LINEAR_GRAD || child.brush().type == slib::sstyle::RADIAL_GRAD) {
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
	*/
	//	node->addChild(SXmlNode::svgNode(child));
	//}
