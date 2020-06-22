#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

SXmlDoc::SXmlDoc() : SDocument<SXmlNode>() {}
SXmlDoc::SXmlDoc(suint t) : SDocument<SXmlNode>(t) { _init(); }
SXmlDoc::SXmlDoc(const SXmlDoc &doc) : SDocument<SXmlNode>(doc) {
    _definition = doc._definition;
    _doctype = doc._doctype;
    _entity = doc._entity;
}
SXmlDoc::~SXmlDoc() {}
void SXmlDoc::_init() {
	_definition = sxnode(xml::DEFINITION_NODE, "xml");
	_definition->attribute = { ks("version", "1.0"), ks("encoding", "utf-8") };
	_root.addChild(_definition);
	switch (_type) {
	case xml::PLIST:
	{
		_doctype = sxnode(xml::DOCTYPE_PUB_NODE, "plist");
		_doctype->attribute =
		{
			ks("public", "-//Apple Computer//DTD PLIST 1.0//EN"),
			ks("dtd", "http://www.apple.com/DTDs/PropertyList-1.0.dtd")
		};
		_root.addChild(_doctype);
		_entity = sxnode(xml::START_TAG, "plist");
		_entity->attribute = { ks("version", "1.0") };
		_root.addChild(_entity);
		break;
	}
	case xml::SVG:
	{
		_doctype = sxnode(xml::DOCTYPE_PUB_NODE, "svg", "");
		_doctype->attribute =
		{
			ks("public", "-//W3C//DTD SVG 1.1//EN"),
			ks("dtd", "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd")
		};
		_root.addChild(_doctype);
		break;
	}
	default:
		break;
	}
}
sxnode SXmlDoc::definition() const { return _definition; }
sxnode SXmlDoc::doctype() const { return _doctype; }
sxnode SXmlDoc::entity() const { return _entity; }
void SXmlDoc::addToEntity(sxnode node) { _entity->addChild(node); }
void SXmlDoc::_read(SFile &file, String &tag, SXmlNode *parent) {
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
void SXmlDoc::load(const char *path) {
    SFile file(path, sio::READ);
    String buf, tag;
    while (!(file.eof())) {
        file.readTo(buf, "<");
        file.readTo(tag, ">");
        _read(file, tag, nullptr);
    }
}
inline void _write(SFile &file, sxnode node) {
	size_t l = node->layer() - 1;
	if (node->type == xml::DEFINITION_NODE)
		file << String("<?xml") <<
		(node->attribute["version"] ? " version=" + String::dquot(node->attribute["version"]) : "") <<
		(node->attribute["encoding"] ? " encoding=" + String::dquot(node->attribute["encoding"]) : "") << "?>" << NEW_LINE;
	else if (node->type == xml::CDATA_NODE)
		file << String::TAB * l << "<![CDATA[" << node->content << "]]>" << NEW_LINE;
	else if (node->type == xml::COMMENT_NODE)
		file << String::TAB * l << "<!--" << node->content << "-->" << NEW_LINE;
	else if (node->type & xml::DOCTYPE_NODE) {
		file << String("<!DOCTYPE ") << node->tag << " ";
		if (node->type == xml::DOCTYPE_PUB_NODE)
			file << "PUBLIC " << String::dquot(node->attribute["public"]) << " " << String::dquot(node->attribute["dtd"]) << ">" << NEW_LINE;
		else if (node->type == xml::DOCTYPE_SYS_NODE)
			file << "SYSTEM " << String::dquot(node->attribute["dtd"]) << ">" << NEW_LINE;
		else {
			file << "[" << NEW_LINE;
			/*
			 */
			file << "]>" << NEW_LINE;
		}
	}
	else if (node->type != xml::HIDDEN_TAG) {
		file << String::TAB * l << "<" << node->tag;
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
				file << String::TAB * l;
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
void SXmlDoc::save(const char *path) {
    SFile file(path, sio::CREATE);
	sforeach(_root) _write(file, E_);
}
void SXmlDoc::clear() {
    _definition.release(); _definition.discard();
    _doctype.release(); _doctype.discard();
    _entity.release(); _entity.discard();
	_root.clearChildren();
}
