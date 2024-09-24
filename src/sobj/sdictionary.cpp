#include "sobj/sobjptr.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sutil/sjson.h"
slib::SDictionary::SDictionary() : SObject(), Map<slib::String, slib::SObjPtr>(){}
slib::SDictionary::SDictionary(std::initializer_list<Pair<slib::String, slib::SObjPtr>> li) : SObject(), Map<slib::String, slib::SObjPtr>(li) {}
slib::SDictionary::SDictionary(const char *s, const char *sep, const char *delim) : SDictionary(String(s).parse(sep, delim)) {}
slib::SDictionary::SDictionary(const sindex &idx) : SDictionary() { sfor(idx) set($_.key(), $_.value()); }
slib::SDictionary::SDictionary(const sattribute &attr) : SDictionary() { sfor(attr) set($_.key(), SObjPtr::toObj($_.value())); }
slib::SDictionary::SDictionary(const SDictionary &dict) : SObject(), Map<String, slib::SObjPtr>(dict) {}
slib::SDictionary::SDictionary(SDictionary&& dict) noexcept : SObject(), Map<String, slib::SObjPtr>(std::forward<Map<String, slib::SObjPtr> &&>(dict)) {}
slib::SDictionary::~SDictionary() {}
slib::SDictionary & slib::SDictionary::operator=(const slib::SDictionary &dic) {
    clear(); reserve(dic.capacity());
	sfor(dic) { set($_.key(), $_.value()); }
	return *this;
}
slib::SDictionary& slib::SDictionary::operator=(slib::SDictionary&& dic) noexcept { swap(dic); return *this; }
void slib::SDictionary::combine(const SDictionary& dic) {
	sfor(dic) {
		if (hasKey($_.key())) {
			SArray arr({ at($_.key()), $_.value() });
		}
		else insert($_);
	}
}

/*
slib::MapIterator<slib::String, sobj> SDictionary::search(const char *que) {
    auto query = String::lower(que);
    sforeach(*this) {
        if (String::lower(E_.value->toString()).contain(query)) return it;
    }
    return end();
}
void SDictionary::combine(const SDictionary& dic) {
	sforeach(dic) set(E_.key, E_.value);
}
inline void _merge(SDictionary &d1, const SDictionary &d2) {
	sforeach(d2) {
		if (d1[E_.key]) {
			auto& v1 = d1[E_.key];
			auto& v2 = E_.value;
			if (v1.isArray()) {
				if (v2.isArray()) {
					sforeach_(vit, v2) {
						if (v1.find(*vit) == NOT_FOUND) v1.add(*vit);
					}
				}
				else v1.add(v2);
			}
			else if (v1.isDict() && v2.isDict()) _merge(v1.dict(), v2.dict());
			else v1 = v2;
		}
		else d1[E_.key] = E_.value;
	}
}
void SDictionary::merge(const SDictionary& dic) { _merge(*this, dic); }
SArray SDictionary::search(const stringarray &keys) {
    SArray array;
    sforeach(keys) array.add(at(E_));
    return array;
}
*/
bool slib::SDictionary::operator < (const SDictionary& dic) const {
	if (size() != dic.size()) return size() < dic.size();
	sfor(*this) {
		if (dic.hasKey($_.key()) && $_.value() != dic[$_.key()]) return $_.value() < dic[$_.key()];
	}
	return false;
}
bool slib::SDictionary::operator == (const SDictionary& dic) const {
	if (size() != dic.size()) return false;
	sfor(*this) {
		if (!dic.hasKey($_.key()) || $_.value() != dic[$_.key()]) return false;
	}
	return true;
}

slib::String slib::SDictionary::getClass() const { return "dict"; }
slib::String slib::SDictionary::toString(const char * format) const {
	String f(format);
	if (f == "json") return sjson::toString(*this);
	else if (f == "url") {
		String str;
		if (empty()) return "";
		sfor(*this) {
			str << $_.key() << "=";
			if ($_.value().isArray()) str << $_.value().toString(nullptr);
			else if ($_.value().isDict()) str << $_.value().toString("url");
			else str << $_.value().toString();
			str << "&";
		}
		str.resize(str.size() - 1);
		return str;
	}
	else {
		if (empty()) return "";
		String str("{"), dat;
		if (hasKey("_key_")) {
			auto keys = at("_key_");
			sfor(keys) {
				str << $_ << ":";
				if (at($_).isNull()) str << "(null),";
				else if (at($_).isNum()) str << at($_) << ",";
				else if (at($_).isArray()) str << at($_) << ",";
				else if (at($_).isDict()) {
					if (&at($_).dict() == this) str += "(self),";
					else str += at($_).toString() + ",";
				}
				else {
					dat = at($_);
					if (dat.match("\"")) dat.replace("\"", "\\\"");
					str << sstr::dquote(dat) << ",";
				}
			}
		}
		else {
			sfor(*this) {
				str += $_.key() + ":";
				if ($_.value().isNull()) str += "(null),";
				else if ($_.value().isNum()) str += $_.value().toString() + ",";
				else if ($_.value().isArray()) str += $_.value().toString() + ",";
				else if ($_.value().isDict()) {
					if (&$_.value().dict() == this) str += "(self),";
					else str += $_.value().toString() + ",";
				}
				else {
					dat = $_.value().toString();
					if (dat.match("\"")) dat.replace("\"", "\\\"");
					str += sstr::dquote(dat) + ",";
				}
			}
		}
		if (str.size()) str[-1] = '}';
		return str;
	}
}
slib::SObject * slib::SDictionary::clone() const { return new SDictionary(*this); }
