#include "sobj/sarray.h"
#include "sobj/sobjptr.h"
#include "sutil/sjson.h"
slib::SArray::SArray() : Array<slib::SObjPtr>() {}
slib::SArray::SArray(const size_t size) : Array<slib::SObjPtr>(size) {}
slib::SArray::SArray(const size_t size, const SObjPtr &o) : Array<slib::SObjPtr>(size, o) {}
slib::SArray::SArray(std::initializer_list<slib::SObjPtr> li) : Array<slib::SObjPtr>(li) {}
slib::SArray::SArray(const stringarray& strarray) : SArray() {
    if (strarray.empty()) return;
    Array<slib::SObjPtr>::resize(strarray.size());
    sfor2(*this, strarray) $_1 = slib::SObjPtr::toObj($_2);
}
slib::SArray::SArray(const intarray& iarray) : SArray() {
    if (iarray.empty()) return;
    Array<slib::SObjPtr>::resize(iarray.size());
    sfor2(*this, iarray) $_1 = $_2;
}
slib::SArray::SArray(SArray&& array) noexcept : slib::Array<slib::SObjPtr>(std::forward<slib::Array<slib::SObjPtr>&&>(array)) {}
slib::SArray::SArray(const SArray &array) : slib::Array<slib::SObjPtr>(array) {}
slib::SArray::~SArray() {}
slib::SArray & slib::SArray::operator = (const SArray &array) {
    resize(array.size());
	sfor2(*this, array) $_1 = $_2;
    return *this;
}
size_t slib::SArray::search(const slib::SObjPtr& que, size_t offset) const {
    sforin(it, begin() + offset, end()) {
        if ($_ == que) return $INDEX(*this);
    }
    return NOT_FOUND;
}
size_t slib::SArray::findWithKey(const slib::SObjPtr& que, const char* key) const {
    sfor(*this) {
        if ($_[key] == que) return $INDEX(*this);
    }
    return NOT_FOUND;
}
slib::SArray slib::SArray::arrayOfKey(const char* key) const {
    SArray tmp;
    sfor(*this) { if ($_.hasKey(key)) tmp.add($_[key]); }
    return tmp;
}
void slib::SArray::sortByKey(const char* key, slib::ORDER order) {
    if (order == ORDER::ASC)
        std::sort(Array<slib::SObjPtr>::begin(), Array<slib::SObjPtr>::end(),
            [key](const slib::SObjPtr& s1, const slib::SObjPtr& s2) {
                return s1[key] < s2[key];
            });
    else 
        std::sort(Array<slib::SObjPtr>::begin(), Array<slib::SObjPtr>::end(),
            [key](const slib::SObjPtr& s1, const slib::SObjPtr& s2) {
                return s2[key] < s1[key];
            });
}
void slib::SArray::sortBy(const char *key, ORDER order) {
    if(order == ORDER::ASC)
        std::sort(Array<slib::SObjPtr>::begin(), Array<slib::SObjPtr>::end(),
                  [key](const slib::SObjPtr &s1, const slib::SObjPtr &s2) {
                      if(!s1.isDict()) return false;
                      else if(!s2.isDict()) return true;
                      return s1[key] < s2[key];
                  });
    else std::sort(Array<slib::SObjPtr>::begin(), Array<slib::SObjPtr>::end(),
                   [key](const slib::SObjPtr &s1, const slib::SObjPtr &s2) {
                       if(!s1.isDict()) return false;
                       else if(!s2.isDict()) return true;
                       return s2[key] < s1[key];
                   });
}
bool slib::SArray::operator < (const SArray &array) const {
    if (size() != array.size()) return size() < array.size();
	sfor2(*this, array) {
		if ($_1 != $_2) return $_1 < $_2;
	}
    return false;
}
bool slib::SArray::operator == (const SArray &array) const {
    if (size() != array.size()) return false;
    sfor2(*this, array) {
        if ($_1 != $_2) return false;
	}
    return true;
}
slib::String slib::SArray::getClass() const { return "array"; }
slib::String slib::SArray::toString(const char* format) const {
    String f = format ? sstr::toLower(format) : "";
    if (f.empty()) {
        if (empty()) return "[]";
        String str = "[", dat;
        sfor(*this) {
            if ($_.isNull()) str += "null,";
            else if ($_.isNum()) str += $_ + ",";
            else if ($_.isArray()) {
                if (&$_.array() == this) str += "(self),";
                else str += $_.toString() + ",";
            }
            else if ($_.isDict()) str += $_.toString() + ",";
            else {
                dat = $_;
                dat.replace("\"", "\\\"");
                str += sstr::dquote(dat) + ",";
            }
        }
        str[-1] = ']';
        return str;
    }
    else if (f == "json") return slib::sjson::toString(*this);
    else if (f == "csv") return slib::toString((const Array<slib::SObjPtr> &)(*this), ",");
    else if (f == "tsv") return slib::toString((const Array<slib::SObjPtr> &)(*this), "\t");
    else return slib::toString((const Array<slib::SObjPtr> &)(*this), f);
}
slib::SObject* slib::SArray::clone() const {
    if (empty()) return new SArray();
    auto array = new SArray(size());
    sfor2(*array, *this) $_1 = $_2.clone();
    return array;
}

slib::String slib::toString(const slib::SArray& array, const char* format) { return array.toString(format); }
std::ostream& operator<<(std::ostream& os, const slib::SArray& array) { return os << array.toString(); }