#include "sobj/sarray.h"
#include "sutil/sjson.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

SArray::SArray() : Array<sobj>() {}
SArray::SArray(int size) : Array<sobj>(size) {}
SArray::SArray(size_t size) : Array<sobj>(size) {}
SArray::SArray(std::initializer_list<sobj> li) : Array<sobj>(li) {}
SArray::SArray(const char *s, const char *sep) : SArray(SString(s).split(sep)) {}
SArray::SArray(const intarray &iarray) : SArray() {
    if (iarray.empty()) return;
	Array<sobj>::resize(iarray.size());
	sforeach2(*this, iarray) E1_ = E2_;
}
SArray::SArray(const stringarray &strarray) : SArray() {
    if (strarray.empty()) return;
    Array<sobj>::resize(strarray.size());
	sforeach2(*this, strarray) E1_ = sobj::toSObj(E2_);
}
SArray::SArray(const sobj &obj) : SArray() {
    if (obj.isNull()) return;
    if (obj.isArray()) *this = obj.array();
    else add(obj);
}
SArray::SArray(SArray&& array) : Array(std::forward<Array &&>(array)) {}
SArray::SArray(const SArray &array) : Array(array) {}
SArray::~SArray() {}
SArray &SArray::operator = (const SArray &array) {
    clear(); resize(array.size());
	sforeach2(*this, array) E1_ = E2_;
    return *this;
}
void SArray::load(const char *path) {
    if(!empty()) clear();
    auto ext = SFile(path).extension();
	if (ext == "sobj") {
		sio::SFile file(path, sio::READ);
		sarray array;
		file.readSObject(array);
		*this = array;
	}
	else if (ext == "plist") {
        SXmlDoc doc;
        doc.load(path);
        auto node = doc.entity()->children().first();
        if(doc.type() != xml::PLIST)
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR);
        *this = SXmlNode::toPlistObj(node);
    }
    else if(ext == "json") {
        SJson js;
        js.load(path);
        *this = js.array();
    }
}
void SArray::save(const char *path) {
    auto ext = SFile(path).extension();
	if (ext == "sobj") {
		sio::SFile file(path, sio::CREATE);
		file.writeSObject(*this);
	}
    else if (ext == "plist") {
        SXmlDoc doc(xml::PLIST);
        doc.addToEntity(SXmlNode::plistNode(*this));
        doc.save(path);
    }
    else if(ext == "json") {
        SJson js(*this);
        js.save(path);
    }
}
SArray SArray::subarray(size_t off, size_t len) const {
    if (size() < off+len) len = size()-off;
    return subarray(begin()+off, begin()+off+len);
}
SArray SArray::subarray(SArrayCIterator<SObjPtr> beg, SArrayCIterator<SObjPtr> end) const {
    SArray array; array.append(beg._ptr, end-beg); return array;
}
SArray SArray::subarray(srange range) const {
    return subarray(range.begin, range.length());
}
size_t SArray::search(const char *que, size_t offset) const {
    auto query = SString::lower(que);
    for(auto it = begin()+offset; it < end(); ++it) {
        if(it->isNull()) continue;
        else if(String::lower(it->toString()).contain(query)) return it-begin();
    }
    return NOT_FOUND;
}
void SArray::sortby(const char *key, ORDER order) {
    if(order == ASC)
        std::sort(Array<sobj>::begin(), Array<sobj>::end(),
                  [key](const sobj &s1, const sobj &s2) {
                      if(!s1.isDict()) return false;
                      else if(!s2.isDict()) return true;
                      return s1[key] < s2[key];
                  });
    else std::sort(Array<sobj>::begin(), Array<sobj>::end(),
                   [key](const sobj &s1, const sobj &s2) {
                       if(!s1.isDict()) return false;
                       else if(!s2.isDict()) return true;
                       return s2[key] < s1[key];
                   });
}

String SArray::getClass() const { return "array"; }
String SArray::toString() const {
    if(empty()) return "[]";
    String str = "[", dat;
    sforeach(*this) {
        if(E_.isNull()) str += "null,";
        else if(E_.isNum()) str += E_+",";
        else if(E_.isArray()) str += "{"+E_+"},";
        else if(E_.isDict()) str += "{"+E_+"},";
        else {
            dat = E_;
            if(dat.contain("\"")) dat.replace("\"", "\\\"");
            str += String::dquot(dat)+",";
        }
    }
    str.last() = ']';
    return str;
}
SObject *SArray::clone() const { 
	if (empty()) return new SArray();
	auto array = new SArray(size());
	sforeach2(*array, *this) E1_ = E2_.clone();
	return array;
}
bool SArray::operator < (const sobj &obj) const {
    if (obj.isNull()) return false;
    if (obj.isArray()) return (*this) < obj.array();
    return getClass() < obj->getClass();
}
bool SArray::operator < (const SArray &array) const {
    if (size() != array.size()) return size() < array.size();
	sforeach2(*this, array) {
		if (E1_ != E2_) return E1_ < E2_;
	}
    return false;
}
bool SArray::operator == (const sobj &obj) const {
    if (obj.isArray()) return (*this) == obj.array();
    return false;
}
bool SArray::operator == (const SArray &array) const {
    if (size() != array.size()) return false;
	sforeach2(*this, array) {
		if (E1_ != E2_) return false;
	}
    return true;
}

