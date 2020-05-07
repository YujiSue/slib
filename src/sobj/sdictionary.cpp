#include "sobj/sstring.h"
#include "sobj/sdict.h"
#include "sutil/sjson.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

SPair::SPair() : kvpair<String, sobj>(), SObject() {}
SPair::SPair(const kvpair<String, sobj> &pair) : kvpair<String, sobj>(pair), SObject() {}
SPair::SPair(const SPair &pair) : kvpair<String, sobj>(pair.key, pair.value), SObject() {}
SPair::~SPair() {}
SPair &SPair::operator=(const SPair &pair) {
    key = pair.key;
    value = pair.value;
    return *this;
}
String SPair::getClass() const { return "pair"; }
String SPair::toString() const { return String("(")<<key<<","<<value<<")"; }
SObject *SPair::clone() const { return new SPair(*this); }

SDictionary::SDictionary() : SObject(), Map<String, sobj>(){}
SDictionary::SDictionary(std::initializer_list<std::pair<String, sobj>> li) : SObject(), Map<String, sobj>(li) {}
SDictionary::SDictionary(std::initializer_list<kvpair<String, sobj>> li) : SObject(), Map<String, sobj>(li) {}
SDictionary::SDictionary(const char *s, const char *sep, const char *part) : SDictionary(SString(s).parse(sep, part)) {}
SDictionary::SDictionary(const sindex &idx) : SDictionary() { sforeach(idx) set(it->key, it->value); }
SDictionary::SDictionary(const sattribute &attr) : SDictionary() {
    if(attr.empty()) return;
    sforeach(attr) {
        if(it->value == "null") set(it->key, snull);
        else if(it->value.isQuoted()) set(it->key, SString::dequot(it->value));
        else if(it->value.isNumeric()) set(it->key, it->value.number());
        else set(it->key, it->value);
    }
}
SDictionary::SDictionary(const sobj &obj) : SDictionary(obj.dict()) {}
SDictionary::SDictionary(const SDictionary &dict) : SObject(), Map<String, sobj>(dict) {}
SDictionary::SDictionary(SDictionary&& dict) : SObject(), Map<String, sobj>(std::forward<Map<String, sobj> &&>(dict)) {}
SDictionary::~SDictionary() {}
SDictionary &SDictionary::operator=(const SDictionary &dic) {
    clear();
	reserve(dic.capacity());
	if (dic.size()) {
		sforeach(dic) set(E_.key, E_.value);
	}
	return *this;
}
SDictionary& SDictionary::operator=(SDictionary&& dic) { swap(dic); return *this; }
void SDictionary::load(const char *path) {
    if(!empty()) clear();
    auto ext = SFile(path).extension();
    if (ext == "plist") {
        SXmlDoc doc;
        doc.load(path);
        auto node = doc.entity()->children().first();
        if (!(doc.type()& xml::PLIST))
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, path);
        *this = SXmlNode::toPlistObj(node);
    }
    else if(ext == "json") {
        SJson js;
        js.load(path);
        *this = js.dict();
    }
}
void SDictionary::save(const char *path) {
    auto ext = SFile(path).extension();
    if (ext == "plist") {
        SXmlDoc doc(xml::PLIST);
        doc.addToEntity(SXmlNode::plistNode(*this));
        doc.save(path);
    }
    else if(ext == "json") {
        SJson js(*this);
        js.save(path);
    }
}
SMapIterator<String, sobj> SDictionary::search(const char *que) {
    auto query = String::lower(que);
    sforeach(*this) {
        if (String::lower(E_.value->toString()).contain(query)) return it;
    }
    return end();
}
SArray SDictionary::lump(const stringarray &keys) {
    SArray array;
    sforeach(keys) array.add(at(E_));
    return array;
}
String SDictionary::getClass() const { return "dict"; }
String SDictionary::toString() const {
    if(empty()) return "";
    String str, dat;
    sforeach(*this) {
        str += it->key+":";
        if(it->value.isNull()) str += "null,";
        else if(it->value.isNum()) str += it->value.toString()+",";
        else if(it->value.isArray()) str += "{"+it->value.toString()+"},";
        else if(it->value.isDict()) str += "{"+it->value.toString()+"},";
        else {
            dat = it->value.toString();
            if(dat.contain("\"")) dat.replace("\"", "\\\"");
            str += String::dquot(dat)+",";
        }
    }
    str.resize(str.length()-1);
    return str;
}
SObject * SDictionary::clone() const {
    return new SDictionary(*this);
}
bool SDictionary::operator < (const sobj &obj) const {
    if (obj.isNull()) return false;
    if (obj.isDict()) (*this) < obj.dict();
    return getClass() < obj->getClass();
}
bool SDictionary::operator < (const SDictionary &dic) const {
    if (size() != dic.size()) return size() < dic.size();
    sforeach(*this) {
        if (dic.hasKey(E_.key) && E_.value != dic[E_.key])
            return E_.value < dic[E_.key];
    }
    return false;
}
bool SDictionary::operator == (const sobj &obj) const {
    if (obj.isDict()) (*this) < obj.dict();
    return false;
}
bool SDictionary::operator == (const SDictionary &dic) const {
    if (size() != dic.size()) return false;
    sforeach(*this) {
        if (!dic.hasKey(E_.key) || E_.value != dic[E_.key]) return false;
    }
    return true;
}
