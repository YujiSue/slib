#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sobj/stable.h"
#include "sutil/sjson.h"
#include "sio/stream.h"
//
inline slib::String jsArrayString(const slib::SArray& array, bool form, int layer) {
    slib::String str = (form ? slib::SP * layer : slib::String()) << "[";
    if (array.empty()) str << "]";
    else {
        str << (form ? "" : slib::NL);
        sfor(array) {
            if ($_.isArray()) {
                str << (form ? slib::SP * layer : slib::String()) << "[" << (form && $_.size() ? slib::NL : "");
                if ($_.empty()) str << "]," << (form ? "" : slib::NL);
                else {
                    sforeach(elem, $_.array()) {
                        str << slib::sjson::jsString(elem, form, layer + 2) << "," << (form ? slib::NL : "");
                    }
                    str.removeAt((int)str.rfind(","));
                    str << (form ? slib::SP * layer : slib::String()) << "]," << slib::NL;
                }
            }
            else if ($_.isDict()) {
                str << (form ? slib::SP * layer : slib::String()) << "{" << (form && $_.size() ? slib::NL : "");
                if ($_.empty()) str << "}," << (form ? "" : slib::NL);
                else {
                    sforeach(pair, $_.dict()) {
                        str << slib::sjson::jsString(pair.key(), form, layer + 2) << ":" << "," << (form ? slib::NL : "");
                    }
                    str.removeAt((int)str.rfind(","));
                    str << (form ? slib::SP * layer : slib::String()) << "]," << slib::NL;
                }
            }

            str << slib::sjson::jsString($_, form, layer + 1) << "," << (form ? slib::NL : "");
        }
        str.removeAt((int)str.rfind(","));
        str << (form ? slib::SP * layer : slib::String()) << "]";
    }
    return str;
}
inline slib::String jsDictString(const slib::SDictionary& dict, bool form, int layer) {
    slib::String str = (form ? slib::SP * layer : slib::String()) << "{"; 
    if (dict.empty()) str << "}";
    else {
        str << (form ? "" : slib::NL);
        sfor(dict) {
            str << (form ? slib::SP * (layer + 1) : slib::String()) << slib::sstr::dquote($_.key()) << ":";
            auto& val = $_.value();
            if (val.isArray()) {
                str << (form ? slib::SP * layer : slib::String()) << "[" << (form && val.size() ? slib::NL : "");
                if (val.empty()) str << "]," << (form ? "" : slib::NL);
                else {
                    sforeach(elem, val.array()) {
                        str << slib::sjson::jsString(elem, form, layer + 2) << "," << (form && val.size() ? slib::NL : "");
                    }
                    str.removeAt((int)str.rfind(","));
                    str << (form ? slib::SP * layer : slib::String()) << "]," << slib::NL;
                }
            }
            else if (val.isDict()) {
                str << (form ? slib::SP * layer : slib::String()) << "{" << (form && val.size() ? slib::NL : "");
                if (val.empty()) str << "}," << (form ? "" : slib::NL);
                else {
                    sforeach(pair, val.dict()) {
                        str << slib::sjson::jsString(pair.key(), form, layer + 2) << ":" << "," << (form && val.size() ? slib::NL : "");
                    }
                    str.removeAt((int)str.rfind(","));
                    str << (form ? slib::SP * layer : slib::String()) << "]," << slib::NL;
                }
            }
            else str << slib::sjson::jsString(val, form, 0) << "," << (form ? slib::NL : "");
        }
        str.removeAt((int)str.rfind(","));
        str << (form ? slib::SP * layer : slib::String()) << "}";
    }
    return str;
}
inline sobj toArray(const char** cur);
inline sobj toDict(const char** cur);
inline sobj toArray(const char** cur) {
    sobj array;
    bool sq = false, dq = false;
    size_t len = 0;
    while (**cur != '\0') {
        if (!sq && !dq) {
            switch (**cur) {
            case ',':
            {
                if (len) {
                    slib::String tmp(len, '\0');
                    memcpy(&tmp[0], *cur - len, len);
                    tmp.trim();
                    if (tmp.length()) array.add(slib::sjson::jsObj(tmp));
                    len = 0;
                }
                break;
            }
            case ']':
            {
                if (len) {
                    slib::String tmp(len, '\0');
                    memcpy(&tmp[0], *cur - len, len);
                    tmp.trim();
                    if (tmp.length()) array.add(slib::sjson::jsObj(tmp));
                }
                return array;
            }
            case '[':
                ++(*cur);
                array.add(toArray(cur));
                len = 0;
                break;
            case '{':
                ++(*cur);
                array.add(toDict(cur));
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
                ++(*cur);
                len += 2;
                break;
            default:
                ++len;
                break;
            }
        }
        else {
            switch (**cur) {
            case '\'':
                if (!dq) sq = !sq;
                break;
            case '\"':
                if (!sq) dq = !dq;
                break;
            case '\\':
                ++cur;
                ++len;
                break;
            default:
                break;
            }
            ++len;
        }
        ++(*cur);
    }
    return array;
}
// 
inline sobj toDict(const char** cur) {
    slib::SDictionary dict;
    slib::String key;
    bool sq = false, dq = false, k = true;
    size_t len = 0;
    while (**cur != '\0') {
        if (!sq && !dq) {
            switch (**cur) {
            case ':':
            {
                if (k) {
                    if (!len) key = "";
                    else {
                        key.resize(len);
                        memcpy(&key[0], *cur - len, len);
                        key.trim();
                        if (slib::sstr::isQuoted(key)) key = slib::sstr::dequote(key);
                        len = 0;
                    }
                    k = false;
                }
                break;
            }
            case ',':
            {
                if (len) {
                    slib::String tmp(len, '\0');
                    memcpy(&tmp[0], *cur - len, len);
                    tmp.trim();
                    if (tmp.length()) dict[key] = slib::sjson::jsObj(tmp);
                    len = 0;
                }
                k = true;
                break;
            }
            case '}':
            {
                if (len) {
                    slib::String tmp(len, '\0');
                    memcpy(&tmp[0], *cur - len, len);
                    tmp.trim();
                    if (tmp.length()) dict[key] = slib::sjson::jsObj(tmp);
                }
                return dict;
            }
            case '[':
                ++(*cur);
                dict[key] = toArray(cur);
                len = 0;
                break;
            case '{':
                ++(*cur);
                dict[key] = toDict(cur);
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
                ++(*cur);
                len += 2;
                break;
            default:
                ++len;
                break;
            }
        }
        else {
            switch (**cur) {
            case '\'':
                if (!dq) sq = !sq;
                break;
            case '\"':
                if (!sq) dq = !dq;
                break;
            case '\\':
                ++(*cur);
                ++len;
                break;
            default:
                break;
            }
            ++len;
        }
        ++(*cur);
    }
    return dict;
}
inline slib::SObjPtr jsArray(slib::String& str) {
    auto ptr = str.cstr();
    if (1 < str.size() && str[0] == '[' && str[-1] == ']') {
        ++ptr;
        return toArray(&ptr);
    }
    return snull;
}
inline slib::SObjPtr jsDict(slib::String& str) {
    auto ptr = str.cstr();
    if (1 < str.size() && str[0] == '{' && str[-1] == '}') {
        ++ptr;
        return toDict(&ptr);
    }
    return snull;
}
// 
inline void readObj(slib::IOStream& strm, slib::SObjPtr& obj) {


}
// 
inline void writeObj(slib::IOStream& strm, const slib::SObjPtr& obj, bool form, int layer) {
    if (obj.isArray()) {
        strm << "[";
        if (obj.empty()) strm << "]";
        else {
            strm << slib::NL;
            sfor(obj) {
                writeObj(strm, $_, form, layer + 1);
                if ($ < obj.end() - 1) strm << ",";
                strm << slib::NL;
            }
            strm << slib::TAB * layer << "]";
        }
        strm.flush();
    }
    else if (obj.isDict()) {
        strm << "{";
        if (obj.empty()) strm << "}";
        else {
            strm << slib::NL;
            auto keys = obj.keyset();
            sfor(keys) {
                strm << slib::TAB * (layer + 1) << slib::sstr::dquote($_) << ":";
                if (obj[$_].isArray()) writeObj(strm, obj[$_], form, layer + 1);
                else if (obj[$_].isDict()) writeObj(strm, obj[$_], form, layer + 1);
                else writeObj(strm, obj[$_], form, 0);
                if ($ < keys.end() - 1) strm << ",";
                strm << slib::NL;
            }
            strm << slib::TAB * layer << "}";
        }
        strm.flush();
    }
    else strm << slib::TAB * layer << slib::sjson::jsString(obj);
}
// Return 
slib::String slib::sjson::jsString(const SObjPtr& obj, bool form, int layer) {
    // Null object
    if (obj.isNull()) return "null";
    // Numeric object
    else if (obj.isNum()) {
        // Determine numeric type
        auto ntype = obj.type();
        if (ntype == stype::INTEGER ||
            ntype == stype::UINTEGER ||
            ntype == stype::BOOL) {
            return obj.toString();
        }
        else if (ntype == stype::REAL) {
            double temp = obj;
            if (std::isnan(temp)) return "NaN";
            else if (std::isinf(temp)) return (temp < 0 ? "-Inf" : "Inf");
            else return obj.toString();
        }
    }
    else if (obj.isDate()) return slib::sstr::dquote(obj.toString(slib::sstyle::ISO8601));
    return slib::sstr::dquote(obj.toString().replace("\"", "\\\""));
    /*
    
    else if (obj.isStr()) return (form ? slib::SP * layer : String()) << slib::sstr::dquote(obj);
    else if (obj.isDate()) return (form ? slib::SP * layer : String()) << slib::sstr::dquote(obj.toString(slib::sstyle::ISO8601));
    else if (obj.isArray()) return jsArrayString(obj, form, layer);
    else if (obj.isDict()) return jsDictString(obj, form, layer);
    else if (obj.isTable()) return jsTableString(obj, form, layer);
    return (form ? slib::SP * layer : String()) << slib::sstr::dquote(obj);
    */
}
// 
sobj slib::sjson::jsObj(const char* s) {
    auto str = sstr::trim(s);
    if (str.empty()) return snull;
    else if (str[0] == '[') return jsArray(str);
    else if (str[0] == '{') return jsDict(str);
    else if (sstr::isQuoted(str)) return sstr::dequote(str);
    else if (str == "null") return snull;
    return SNumber(str.cstr());
}
// Load
sobj slib::sjson::load(const char* path) {
    // Result
    slib::String str;
    // File obj
    SFile file(path);
    // Write out to the file
    file >> str;
    // Return object
    return slib::sjson::parse(str);
}
// Save
void slib::sjson::save(const sobj& obj, const char* path, bool form) {
    // File obj
    slib::SFile file(path, sio::MAKE);
    // Set stream
    auto strm = IOStream(file, sio::FILEIO | sio::OSTREAM);;
    // Write out to the stream
    writeObj(strm, obj, form, 0);
}
// String => JSON object
slib::SObjPtr slib::sjson::parse(const char* s) { return jsObj(s); }
// Stringify
slib::String slib::sjson::toString(const sobj& obj, bool form) { 
    // Result
    slib::String str;
    // Set stream
    auto strm = IOStream(str, sio::STRIO | sio::OSTREAM);
    // Write out to the stream
    writeObj(strm, obj, form, 0);
    // Return
    return str;
}
