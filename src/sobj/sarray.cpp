#include "sobj/sarray.h"
#include "sutil/sjson.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

intarray slib::iarray(const String &str, const char *sep) {
    intarray iarray;
    auto arr = str.split(sep);
    sforeach(arr) iarray.add(it->integer());
    return iarray;
}
intarray slib::iarray(int num, bool zero) {
    intarray iarray(num);
    sforin(i, 0, num) iarray[i] = (zero?i:i+1);
    return iarray;
}
SArray::SArray() : Array<sobj>() {}
SArray::SArray(int size) : Array<sobj>(size) {}
SArray::SArray(size_t size) : Array<sobj>(size) {}
SArray::SArray(std::initializer_list<sobj> li) : Array<sobj>(li) {}
SArray::SArray(const char *s, const char *sep) : SArray(SString(s).split(sep)) {}
SArray::SArray(const intarray &iarray) : SArray() {
    if (iarray.empty()) return;
    Array<sobj>::resize(iarray.size());
    auto p = ptr();
    sforeach(iarray) { *p = E_; ++p; }
}
SArray::SArray(const stringarray &strarray) : SArray() {
    if (strarray.empty()) return;
    Array<sobj>::resize(strarray.size());
    auto p = ptr();
    sforeach(strarray) {
        if(E_ == "null") *p = snull;
        else if(E_.isQuoted()) *p = String::dequot(E_);
        else if(E_.isNumeric()) *p = SNumber::toNumber(E_);
        else *p = E_;
        ++p;
    }
}
SArray::SArray(const sobj &obj) : SArray() {
    if (obj.isNull()) return;
    if (obj.isArray()) *this = obj.array();
    else add(obj);
}
SArray::SArray(const SArray &array) : SArray(array.size()) {
    auto it = begin(), e = end();
    auto p = array.ptr();
    while(it < e) { E_ = *p; NEXT_; ++p; }
}
SArray::~SArray() {}

SArray &SArray::operator = (const SArray &array) {
    clear(); resize(array.size());
    auto it = begin(), e = end();
    auto p = array.ptr();
    while(it < e) { E_ = *p; NEXT_; ++p; }
    return *this;
}

void SArray::load(const char *path) {
    if(!empty()) clear();
    auto ext = SFile(path).extension();
    if (ext == "plist") {
        SXmlDoc doc;
        doc.load(path);
        auto node = doc.entity()->children().first();
        if(doc.type() != sio::PLIST_FILE || node->tag != "array")
            throw SException(ERR_INFO, SLIB_FORMAT_ERROR, node->tag, "PLIST array");
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
    if (ext == "plist") {
        SXmlDoc doc(sio::PLIST_FILE);
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
SObject *SArray::clone() const { return new SArray(*this);
}
bool SArray::operator < (const sobj &obj) const {
    if (obj.isNull()) return false;
    if (obj.isArray()) return (*this) < obj.array();
    return getClass() < obj->getClass();
}
bool SArray::operator < (const SArray &array) const {
    if (size() != array.size()) return size() < array.size();
    auto ait = array.begin();
    sforeach(*this) { if (E_ != *ait) return E_ < *ait; else ++ait; }
    return false;
}
bool SArray::operator == (const sobj &obj) const {
    if (obj.isArray()) return (*this) == obj.array();
    return false;
}
bool SArray::operator == (const SArray &array) const {
    if (size() != array.size()) return false;
    auto ait = array.begin();
    sforeach(*this) { if (E_ != *ait) return false; else ++ait; }
    return true;
}
