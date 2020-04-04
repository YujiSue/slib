#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

SXmlDoc::SXmlDoc() : SDocument<sxnode>() {}
SXmlDoc::SXmlDoc(suint t) : SDocument<sxnode>(t) { _init(); }
SXmlDoc::SXmlDoc(const SXmlDoc &doc) : SDocument<sxnode>(doc) {
    _definition = doc._definition;
    _doctype = doc._doctype;
    _entity = doc._entity;
}
SXmlDoc::~SXmlDoc() {}

void SXmlDoc::_init() {
	_definition = sxnode(xml::DEFINITION_NODE, "xml");
	_definition->attribute = { ks("version", "1.0"), ks("encoding", "utf-8") };
	switch (_type) {
	case sio::PLIST_FILE:
	{
		_doctype = sxnode(xml::DOCTYPE_PUB_NODE, "plist");
		_doctype->attribute =
		{
			ks("public", "-//Apple Computer//DTD PLIST 1.0//EN"),
			ks("dtd", "http://www.apple.com/DTDs/PropertyList-1.0.dtd")
		};
		_entity = sxnode(xml::START_TAG, "plist");
		_entity->attribute = { ks("version", "1.0") };
		break;
	}
	case sio::SVG_FILE:
	{
		_doctype = sxnode(xml::DOCTYPE_PUB_NODE, "svg", "");
		_doctype->attribute =
		{
			ks("public", "-//W3C//DTD SVG 1.1//EN"),
			ks("dtd", "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd")
		};
		_entity = sxnode(xml::START_TAG);
		break;
	}
	default:
		_doctype = sxnode(xml::DOCTYPE_SYS_NODE, "unkown");
		_entity = sxnode(xml::START_TAG, "unkown");
		break;
	}
}

sxnode SXmlDoc::definition() const { return _definition; }
sxnode SXmlDoc::doctype() const { return _doctype; }
sxnode SXmlDoc::entity() const { return _entity; }

void SXmlDoc::addToEntity(sxnode node) { _entity->add(node); }
//void SXmlDoc::addComment(const char *s) { _root.add(xml::commentNode(s)); }

void SXmlDoc::_read(SFile &file, String &tag, SXmlNode *parent) {
    if(file.eof()) return;
    if(tag[0] == '?' && tag.last() == '?') {
        _type |= sio::XML_FILE;
        _definition = sxnode(xml::DEFINITION_NODE);
        _definition->_parseTag(String::dequot(tag));
    }
    else if(tag[0] == '!') {
        if(tag.beginWith("![CDATA[")) {
            auto node = sxnode(xml::CDATA_NODE);
            if (!tag.endWith("]]")) {
                String tmp;
                file.readTo(tmp, "]]>");
				node->content = tag.substring(8) + tmp;
            }
            else node->content = tag.substring(8, tag.length()-10);
            if (parent) parent->add(node);
            else _root.add(node);
        }
        else if(tag.beginWith("!--")) {
            auto node = sxnode(xml::COMMENT_NODE);
            if (!tag.endWith("--")) {
                SString tmp;
                file.readTo(tmp, "-->");
				node->content = tag.substring(3) + tmp;
            }
            else node->content = tag.substring(3, tag.length()-5);
            if (parent) parent->add(node);
            else _root.add(node);
        }
        else if(tag.beginWith("!DOCTYPE ")) {
            _doctype = sxnode(xml::DOCTYPE_NODE);
            auto tags = tag.substring(9).split(" ");
            _doctype->tag = tags[0];
            if(tags[0] == "plist") _type |= sio::PLIST_FILE;
            else _type = sio::XML_FILE;
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
        auto node = sxnode(xml::START_TAG);
        tag.resize(tag.length()-1);
		node->_parseTag(tag);
        if (!parent) _entity = node;
        else parent->add(node);
    }
    else {
        auto node = sxnode(xml::START_TAG);
        node->_parseTag(tag);
        while (!file.eof()) {
            String content;
            file.readTo(content, "<");
            file.readTo(tag, ">");
            if(tag[0] == '/') {
				if (tag == '/' + node->tag) {
					SXmlNode::decodeXML(content);
					node->content = content;
					break;
				}
				else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, tag, "XML");
			}
            else _read(file, tag, node);
        }
        if (!parent) _entity = node;
        else parent->add(node);
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

inline void _write(SFile &file, SXmlNode *node) {
    size_t layer = node->layer();
    file<<String::TAB*layer<<"<";
    if (node->type == xml::DEFINITION_NODE) {
        file<<"?xml"<<" version="<<String::dquot(node->attribute["version"])<<
        " encoding="<<String::dquot(node->attribute["encoding"])<<"?>"<<NEW_LINE;
    }
    else if (node->type == xml::CDATA_NODE)
        file<<"![CDATA["<<node->content<<"]]>"<<NEW_LINE;
    else if (node->type == xml::COMMENT_NODE)
        file<<"!--"<<node->content<<"-->"<<NEW_LINE;
    else if (node->type&xml::DOCTYPE_NODE) {
        auto doc = "!DOCTYPE "+node->tag+" ";
        if (node->type == xml::DOCTYPE_PUB_NODE)
            file<<doc<<"PUBLIC "<<String::dquot(node->attribute["public"])<<" "<<String::dquot(node->attribute["dtd"])<<">"<<NEW_LINE;
        else if (node->type == xml::DOCTYPE_NODE)
            file<<doc<<"SYSTEM "<<String::dquot(node->attribute["dtd"])<<">"<<NEW_LINE;
        else {
            file<<"["<<NEW_LINE;
            sforeach(node->children()) {
                /*
                 */
            }
            file<<"]>"<<NEW_LINE;
        }
    }
    else {
        file<<node->tag;
        if (!node->attribute.empty()) {
            auto keys = node->attribute.hasKey("_key")?node->attribute["_key"].split(","):node->attribute.keyset();
			sforeach(keys) {
				SXmlNode::encodeXML(node->attribute[E_]);
				file << " " << E_ << "=" << String::dquot(node->attribute[E_]);
			}
        }
        if(node->type&xml::EMPTY_TAG) file<<"/>"<<NEW_LINE;
        else {
            file<<">";
            if(node->hasChild()) {
                file<<NEW_LINE;
                sforeach(node->children()) _write(file, E_.toClassPtr<SXmlNode>());
                file<<String::TAB*layer;
            }
			else {
				SXmlNode::encodeXML(node->content);
				file << node->content;
			}
            file<<"</"<<node->tag<<">"<<NEW_LINE;
            file.flush();
        }
    }
}

void SXmlDoc::save(const char *path) {
    SFile file(path, sio::CREATE);
    _write(file, _definition);
    _write(file, _doctype);
    sforeach(_root) _write(file, E_.toClassPtr<SXmlNode>());
    _write(file, _entity);
}
void SXmlDoc::clear() {
    _definition.release(); _definition.discard();
    _doctype.release(); _doctype.discard();
    _entity.release(); _entity.discard();
	_root.clear();
}
