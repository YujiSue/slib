#include "sutil/sjson.h"

using namespace slib;
using namespace slib::sio;

SJson::SJson() : sobj() {}
SJson::SJson(sio::SFile& file) : sobj() { load(file); }
SJson::SJson(const sobj &obj) : sobj(obj) {}
SJson::SJson(const SJson &js) : sobj() { js.copyTo(*this); }
SJson::~SJson() {}

SJson &SJson::operator=(const sobj &obj) {
    *static_cast<sobj *>(this) = obj;
    return *this;
}
SJson &SJson::operator=(const SJson &js) {
    js.copyTo(*this); return *this;
}

inline String jsArrayString(const SArray &array) {
    String str = "[";
    if (array.empty()) str+="]";
    else {
        sforeach(array) str += SJson::jsString(E_)+",";
        str.last() = ']';
    }
    return str;
}
inline String jsDictString(const SDictionary &dict) {
    String str = "{";
    if (dict.empty()) str+="}";
    else {
        auto keys = dict.hasKey("_key")?dict["_key"].split(","):dict.keyset();
        sforeach(keys) str += String::dquot(E_)+":"+SJson::jsString(dict[E_])+",";
        str.last() = '}';
    }
    return str;
}
String SJson::jsString(const sobj &obj) {
    if (obj.isNull()) return "null";
    else if (obj.isNum()) {
        auto ntype = obj.type();
        if (ntype == SNumber::INTEGER ||
            ntype == SNumber::UINTEGER ||
            ntype == SNumber::BOOL) return obj.toString();
        else if (ntype == SNumber::REAL) {
            sreal temp = obj;
            if (std::isnan(temp)) return "NaN";
            else if (std::isinf(temp)) return temp<0? "-Infinity":"Infinity";
            else return obj.toString();
        }
    }
    else if (obj.isStr()) return SString::dquot(obj);
    else if (obj.isArray()) return jsArrayString(obj);
    else if (obj.isDict()) return jsDictString(obj);
    return String::dquot(obj);
}

inline sobj toArray(int &offset, const char *s);
inline sobj toDict(int &offset, const char *s);

inline sobj toArray(int &offset, const char *s) {
    sarray array;
    bool sq = false, dq = false;
    size_t len = 0;
    while (s[offset] != '\0') {
        if (!sq && !dq) {
            switch (s[offset]) {
                case ',':
                {
                    if (len) {
                        String tmp(len, '\0');
                        memcpy(&tmp[0], &s[offset-len], len);
                        tmp.trimming();
                        if (tmp.length()) array.add(SJson::jsObj(tmp));
                        len = 0;
                    }
                    break;
                }
                case ']':
                {
                    if (len) {
                        SString tmp(len, '\0');
                        memcpy(&tmp[0], &s[offset-len], len);
                        tmp.trimming();
                        if (tmp.length()) array.add(SJson::jsObj(tmp));
                    }
                    return array;
                }
                case '[':
                    array.add(toArray(++offset, s));
                    len = 0;
                    break;
                case '{':
                    array.add(toDict(++offset, s));
                    len = 0;
                    break;
                case '\'':
                    sq = !sq;
                    ++len;
                    break;
                case '\"':
                    dq = !dq;
                    ++len;
                    break;
                case '\\':
                    ++offset;
                    len+=2;
                    break;
                default:
                    ++len;
                    break;
            }
        }
        else {
            switch (s[offset]) {
                case '\'':
                    if (!dq) sq = !sq;
                    break;
                case '\"':
                    if (!sq) dq = !dq;
                    break;
                case '\\':
                    ++offset;
                    ++len;
                    break;
                default:
                    break;
            }
            ++len;
        }
        ++offset;
    }
    return array;
}
inline sobj toDict(int &offset, const char *s) {
    sdict dict;
    String key;
    bool sq = false, dq = false, k = true;
    size_t len = 0;
    while (s[offset] != '\0') {
        if (!sq && !dq) {
            switch (s[offset]) {
                case ':':
                {
                    if (k) {
                        if (!len) key = "";
                        else {
                            key.resize(len);
                            memcpy(&key[0], &s[offset-len], len);
                            key.trimming();
                            if (key.isQuoted()) key.transform(SString::DELETE_QUOTE);
                            len = 0;
                        }
                        k = false;
                    }
                    break;
                }
                case ',':
                {
                    if (len) {
                        String tmp(len, '\0');
                        memcpy(&tmp[0], &s[offset-len], len);
                        tmp.trimming();
                        if(tmp.length()) dict[key] = SJson::jsObj(tmp);
                        len = 0;
                    }
                    k = true;
                    break;
                }
                case '}':
                {
                    if (len) {
                        SString tmp(len, '\0');
                        memcpy(&tmp[0], &s[offset-len], len);
                        tmp.trimming();
                        if(tmp.length()) dict[key] = SJson::jsObj(tmp);
                    }
                    return dict;
                }
                case '[':
                    dict[key] = toArray(++offset, s);
                    len = 0;
                    break;
                case '{':
                    dict[key] = toDict(++offset, s);
                    len = 0;
                    break;
                case '\'':
                    sq = !sq;
                    ++len;
                    break;
                case '\"':
                    dq = !dq;
                    ++len;
                    break;
                case '\\':
                    ++offset;
                    len+=2;
                    break;
                default:
                    ++len;
                    break;
            }
        }
        else {
            switch (s[offset]) {
                case '\'':
                    if (!dq) sq = !sq;
                    break;
                case '\"':
                    if (!sq) dq = !dq;
                    break;
                case '\\':
                    ++offset;
                    ++len;
                    break;
                default:
                    break;
            }
            ++len;
        }
        ++offset;
    }
    return dict;
}

inline sobj jsArray(String &str) {
    int offset = 1;
    str.trimming();
    if(!str.empty() && str[0] == '[') return toArray(offset, &str[0]);
    return snull;
}
inline sobj jsDict(String &str) {
    int offset = 1;
    str.trimming();
    if(!str.empty() && str[0] == '{') return toDict(offset, &str[0]);
    return sobj();
}

sobj SJson::jsObj(const char *s) {
    auto str = String::trim(s);
    if (str.empty()) return snull;
    else if (str[0] == '[') return jsArray(str);
    else if (str[0] == '{') return jsDict(str);
    else if (str.isQuoted()) return str.transformed(String::DELETE_QUOTE);
    else if (str.equal(R(/null/))) return snull;
    return str.number();
}

void SJson::load(const char *path) {
    SString json;
    json.load(path);
    parse(json);
}

inline void writeObj(sio::SFile &file, const sobj &obj, size_t &indent) {
    if (obj.isArray()) {
        file<<String::TAB*indent<<"[";
        if (obj.empty()) file<<"]"<<NEW_LINE;
        else {
            file<<NEW_LINE; ++indent;
            sforeachi(obj) {
                writeObj(file, obj[i], indent);
                if (i < obj.size()-1) file<<","<<NEW_LINE;
                else file<<NEW_LINE;
            }
            --indent;
            file<<String::TAB*indent<<"]"<<NEW_LINE;
        }
        file.flush();
    }
    else if (obj.isDict()) {
        file<<String::TAB*indent<<"{";
        if (obj.empty()) file<<"}"<<NEW_LINE;
        else {
            file<<NEW_LINE; ++indent;
            auto keys = obj.keyset();
            sforeachi(keys) {
                file<<String::TAB<<String::dquot(keys[i])<<":";
                writeObj(file, obj[keys[i]], indent);
                if (i < keys.size()-1) file<<","<<NEW_LINE;
                else file<<NEW_LINE;
            }
            --indent;
            file<<String::TAB*indent<<"}"<<NEW_LINE;
        }
        file.flush();
    }
    else file<<String::TAB*indent<<SJson::jsString(obj);
}

void SJson::save(const char *path) {
    sio::SFile file(path, sio::CREATE);
    size_t indent = 0; 
	writeObj(file, *this, indent);
}

void SJson::parse(const char *s) { *this = SJson::jsObj(s); }
String SJson::toString() { return SJson::jsString(*this); }
