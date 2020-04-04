#include "sobj.h"

using namespace slib;

Regex::Regex() : _global(false) {}
Regex::Regex(const char *s) {
    String s_(s);
    size_t len = s_.length(), end = s_.rfind("/");
    if (len < 3 || s_.first() != '/' || end == NOT_FOUND)
        throw SException(ERR_INFO, SLIB_FORMAT_ERROR, s, "Numeric text");
    auto constant = std::regex::ECMAScript;
    if (end < len-1) {
		auto p = s_.ptr(end + 1);
        sforin(i, end+1, len) {
            if (*p == 'i') constant |= std::regex::icase;
            else if (*p == 'g') _global = true;
			++p;
        }
    }
    _rgx = std::regex(s_.substring(1, end-1), constant);
}
Regex::~Regex() {}

bool Regex::match(const char *s) const { return std::regex_search(s, _rgx); }
bool Regex::equal(const char *s) const { return std::regex_match(s, _rgx); }
void Regex::search(CArray<size_t> &array, const char *s, const char *e) const {
    if (_global) {
        std::cregex_iterator beg(s, e, _rgx), end;
        while(beg != end) { array.add(beg->position()); ++beg; }
    }
    else {
        std::cmatch m;
        if(std::regex_search(s, m, _rgx)) array.add(m.position());
    }
}
void Regex::search(Array<String, SMemory<String>> &array, const char *s, const char *e) const {
    if (_global) {
        std::cregex_iterator beg(s, e, _rgx), end;
        while(beg != end) { array.add(beg->str()); ++beg; }
    }
    else {
        std::cmatch m;
        if(std::regex_search(s, m, _rgx)) array.add(m.str());
    }
}

void Regex::split(Array<String, SMemory<String>> &array, const String *str) const {
    size_t off = 0;
    auto p = str->ptr();
    std::cregex_iterator end;
    sfortill(it, std::cregex_iterator(p, p+str->size(), _rgx), end) {
        array.add(str->substring(srange(off, E_.position())));
        off = E_.position()+E_.size();
    }
}
String Regex::replace(const char *s, const char *alt) const {
    if (_global) return String(std::regex_replace(s, _rgx, alt));
    else return String(std::regex_replace(s, _rgx, alt, std::regex_constants::format_first_only));
}
inline String regexOrder(const CArray<sint>&order) {
    String str;
    if (order.size()) { sforeach(order) str<<"$"<<E_; }
    return str;
}
String Regex::rearrange(const char *s, const CArray<sint> &order) const {
    if (_global) return String(std::regex_replace(s, _rgx, regexOrder(order).cstr()));
    else  return String(std::regex_replace(s, _rgx, regexOrder(order).cstr(), std::regex_constants::format_first_only));
}

const String String::SPACE = " ";
const String String::TAB = "\t";
const String String::LF = "\n";
const String String::CR = "\r";
const String String::CRLF = "\r\n";
const String String::BS = "\\";
const String String::SQUOT = "\'";
const String String::DQUOT = "\"";
const String String::ESC = String(1, (char)27);

String String::trim(const char *s) {
    String str;
    if (s) {
        size_t init = 0, end = strlen(s);
        while (init < end && Char::isWSChar(s[init])) ++init;
        while (init < end && Char::isWSChar(s[end-1])) --end;
        size_t len = end-init;
        if (len) {
            str.resize(len);
            memcpy(&str[0], &s[init], len);
        }
    }
    return str;
}
String String::squot(const char *s) {
    String str;
    str<<'\''<<(s?s:"")<<'\'';
    return str;
}
String String::dquot(const char *s) {
    String str;
    str<<'\"'<<(s?s:"")<<'\"';
    return str;
}
String String::dequot(const char *s) {
    if (!s || strlen(s) < 3) return "";
    size_t len = strlen(s)-2;
    String str(len, '0');
    memcpy(&str[0], &s[1], len);
    return str;
}
String String::upper(const char *s) {
    if (!s) return "";
    String str(s);
    str.transform(String::TO_UPPER);
    return str;
}
String String::lower(const char *s) {
    if (!s) return "";
    String str(s);
    str.transform(String::TO_LOWER);
    return str;
}
String String::enclose(const char *s, const char *c1, const char *c2) {
    String str;
    str<<(c1?c1:"")<<(s?s:"")<<(c2?c2:"");
    return str;
}
String String::wide(const char *s) {
    String str;
    if (s) {
        size_t len = strlen(s);
        sforin(i, 0, len) str += Char::wideChar(s[i]);
    }
    return str;
}
String String::narrow(const char *s) {
    String str;
    if (s) {
        size_t i = 0, len = strlen(s);
        while (i < len) {
            str += Char::narrowChar(s);
            i += Char::u8size(s);
        }
    }
    return str;
}
#if defined(WIN32_OS) || defined(WIN64_OS)
String String::toUTF8(const wchar_t* ws) {
	auto wsize = ::WideCharToMultiByte(CP_UTF8, 0U, ws, -1, nullptr, 0, nullptr, nullptr);
	String str(wsize, '\0');
	auto res = ::WideCharToMultiByte(CP_UTF8, 0U, ws, -1, str.ptr(), wsize, nullptr, nullptr);
	if (!res) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "ws");
	if (str[-1] == '\0') str.resize(str.length() - 1);
	return str;
}
String String::toUTF8(const char* s) {
	auto wsize = ::MultiByteToWideChar(CP_ACP, 0U, s, -1, nullptr, 0U);
	Array<wchar_t> u16s(wsize);
	auto res = ::MultiByteToWideChar(CP_ACP, 0U, s, -1, u16s.ptr(), wsize);
	if (!res) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "s");
	auto u8size = ::WideCharToMultiByte(CP_UTF8, 0U, u16s.ptr(), -1, nullptr, 0, nullptr, nullptr);
	String str(u8size, '\0');
	res = ::WideCharToMultiByte(CP_UTF8, 0U, u16s.ptr(), -1, str.ptr(), u8size, nullptr, nullptr);
	if (!res) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "s");
	if (str[-1] == '\0') str.resize(str.length() - 1);
	return str;
}
#endif
const char *String::HEX_STR = "0123456789ABCDEF";
bool String::_isLong() const { return (*((uint8_t *)&_str))&0x01; }
std::pair<char *, size_t> String::_instance() {
    if (_isLong()) return std::pair<char *, size_t>(_str._ls.str, _str._ls.size);
    return std::pair<char *, size_t>(_str._ss.str, _str._ss.size>>1);
}
std::pair<const char *, size_t> String::_cinstance() const {
    if (_isLong()) return std::pair<const char *, size_t>(_str._ls.str, _str._ls.size);
    return std::pair<const char *, size_t>(_str._ss.str, _str._ss.size>>1);
}
void String::_append(const char *s, size_t l) {
    if (l) {
        auto tmp = size();
        resize(tmp+l);
        memcpy(ptr(tmp), s, l);
    }
}
void String::_insert(const size_t idx, const char *s, size_t l) {
    if (l) {
        auto tmp = size();
        if (idx < tmp) {
            resize(tmp+l);
            auto p = ptr();
            CMemory<char>::shift(&p[idx+l], &p[idx], tmp-idx);
            CMemory<char>::copy(&p[idx], s, l);
        }
        else _append(s, std::forward<size_t>(l));
    }
}

const char *String::_find(const char *que, size_t s, const char *current, const char *end) const {
    const char *p_, *q_;
    size_t shift;
    bool match;
    while (current < end) {
        if (*current == *que) {
            match = true;
            shift = 0;
            p_ = current+1; q_ = que+1;
            sforin(i, 1, s) {
                if (*p_ == *que) shift = i;
                if (*p_ != *q_) {
                    if (!shift) shift = i+1;
                    match = false; break;
                }
                ++p_; ++q_;
            }
            if (match) return current;
            else current += shift;
        }
        else ++current;
    }
    return nullptr;
}
const char *String::_rfind(const char *que, size_t s, const char *begin, const char *current) const {
    const char *p_, *q_;
    size_t shift;
    bool match;
    while (begin <= current) {
        if (*current == *que) {
            match = true;
            shift = 0;
            p_ = current+1; q_ = que+1;
            sforin(i, 1, s) {
                if (*p_ == *que) shift = i;
                if (*p_ != *q_) {
                    if (!shift) shift = i+1;
                    match = false; break;
                }
                ++p_; ++q_;
            }
            if (match) return current;
            else current -= shift;
        }
        else --current;
    }
    return nullptr;
}

String::String() { memset(&_str, 0, sizeof(_str)); }
String::String(bool b) : String(b?"true":"false") {}
String::String(int i) : String(std::to_string(i)) {}
String::String(unsigned int ui) : String(std::to_string(ui)) {}
String::String(size_t ui) : String(std::to_string(ui)) {}
#if defined(WIN64_OS)
String::String(long i) : String(std::to_string(i)) {}
String::String(unsigned long ui) : String(std::to_string(ui)) {}
#endif
String::String(long long i) : String(std::to_string(i)) {}
#if defined(MAC_OS)
String::String(unsigned long long ui) : String(std::to_string(ui)) {}
#endif
String::String(float f) : String(std::to_string(f)) {}
String::String(double d) : String(std::to_string(d)) {}
String::String(sbyte i) : String(std::to_string(i)) {}
String::String(subyte ui) : String(std::to_string(ui)) {}
String::String(sshort i) : String(std::to_string(i)) {}
String::String(sushort ui) : String(std::to_string(ui)) {}

String::String(size_t s, const char &c) : String() {
    if (s < SHORT_STRING_CAPACITY-1) {
        if (s) {
            memset(_str._ss.str, c, s);
            _str._ss.str[s] = '\0';
            _str._ss.size = s<<1;
        }
    }
    else {
        _str._ls.capacity = (((s>>1)+1)<<1)|0x01;
        _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
        _str._ls.size = s;
        memset(_str._ls.str, c, s);
        _str._ls.str[s] = '\0';
    }
}
String::String(char c) : String(1, c) {}
String::String(const Char &c) : String(c.toStr()) {}
String::String(const char *s) : String() {
    if (s) {
        auto len = strlen(s);
        if (len < SHORT_STRING_CAPACITY-1) {
            if (len) strcpy(_str._ss.str, s);
            _str._ss.size = len << 1;
        }
        else {
            _str._ls.capacity = (((len>>4)+1)<<4)|0x01;
            _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
            strcpy(_str._ls.str, s);
            _str._ls.size = len;
        }
    }
}
String::String(const std::string &s) : String() {
    auto len = s.length();
    if (len < SHORT_STRING_CAPACITY-1) {
        if (len) {
            CMemory<char>::copy(_str._ss.str, &s[0], len);
            _str._ss.size = len<<1;
            _str._ss.str[len] = '\0';
        }
    }
    else {
        _str._ls.capacity = (((len>>4)+1)<<4)|0x01;
        _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
        CMemory<char>::copy(_str._ls.str, &s[0], len);
        _str._ls.size = len;
        _str._ls.str[len] = '\0';
    }
}
String::String(String &&s) : String() {
    memcpy(&_str, &s._str, sizeof(string));
    memset(&s._str, 0, sizeof(string));
}
String::String(const String &s) : String() {
    auto len = s.length();
    if (len < SHORT_STRING_CAPACITY-1) {
        if (len)  {
            CMemory<char>::copy(_str._ss.str, &s[0], len);
            _str._ss.size = len << 1;
            _str._ss.str[len] = '\0';
        }
    }
    else {
        _str._ls.capacity = (((len>>4)+1)<<4)|0x01;
        _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
        CMemory<char>::copy(_str._ls.str, &s[0], len);
        _str._ls.size = len;
        _str._ls.str[len] = '\0';
    }
}
String::String(const SString &s) : String() {
    auto len = s.length();
    if (len < SHORT_STRING_CAPACITY-1) {
        if (len) {
            CMemory<char>::copy(_str._ss.str, &s[0], len);
            _str._ss.size = len << 1;
            _str._ss.str[len] = '\0';
        }
    }
    else {
        _str._ls.capacity = (((len>>4)+1)<<4)|0x01;
        _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
        CMemory<char>::copy(_str._ls.str, &s[0], len);
        _str._ls.size = len;
        _str._ls.str[len] = '\0';
    }
}
String::String(const SObjPtr &obj) : String() {
    if (obj.isNull()) *this = "";
    else if (obj.isStr()) *this = obj.string();
    else *this = obj.toString();
}
String::~String() {
    if (_isLong()) CMemory<char>::dealloc(_str._ls.str);
    memset(&_str, 0, sizeof(_str));
}

String &String::operator=(bool b) { *this = b?"true":"false"; return *this; }
String &String::operator=(int i) { *this = std::to_string(i); return *this; }
String &String::operator=(unsigned int ui) { *this = std::to_string(ui); return *this; }
String &String::operator=(size_t ui) { *this = std::to_string(ui); return *this; }
#if defined(WIN64_OS)
String &String::operator=(long i) { *this = std::to_string(i); return *this; }
String &String::operator=(unsigned long ui) { *this = std::to_string(ui); return *this; }
#endif
String &String::operator=(long long i) { *this = std::to_string(i); return *this; }
#if defined(MAC_OS)
String &String::operator=(unsigned long long ui) { *this = std::to_string(ui); return *this; }
#endif
String &String::operator=(float f) { *this = std::to_string(f); return *this; }
String &String::operator=(double d) { *this = std::to_string(d); return *this; }

String &String::operator=(sbyte i) { *this = std::to_string(i); return *this; }
String &String::operator=(subyte ui) { *this = std::to_string(ui); return *this; }
String &String::operator=(sshort i) { *this = std::to_string(i); return *this; }
String &String::operator=(sushort ui) { *this = std::to_string(ui); return *this; }
String &String::operator=(char c) { clear(); resize(1, c); return *this; }
String &String::operator=(const char *s) {
    clear(); if (s && s[0] != '\0') copy(s, strlen(s)); return *this;
}
String &String::operator=(const std::string &s) {
    clear(); if (s.size()) copy(&s[0], s.length()); return *this;
}
String &String::operator=(String &&s) {
    clear(); if (s.size()) memcpy(&_str, &s._str, sizeof(string));
    memset(&s._str, 0, sizeof(string)); return *this;
}
String &String::operator=(const String &s) {
    clear(); if (s.size()) copy(&s[0], s.length()); return *this;
}
String &String::operator=(const SString &s) {
    clear(); if (s.size()) copy(&s[0], s.length()); return *this;
}
String &String::operator=(SObjPtr obj) {
    if (!obj.isNull()) {
        if (obj.isStr()) *this = obj.string();
        else *this = obj.toString();
    }
    return *this;
}

String &String::operator+=(bool b) { *this += b?"true":"false"; return *this; }
String &String::operator+=(int i) { *this += std::to_string(i); return *this; }
String &String::operator+=(unsigned int ui) { *this += std::to_string(ui); return *this; }
String &String::operator+=(size_t ui) { *this += std::to_string(ui); return *this; }
#if defined(WIN64_OS)
String &String::operator+=(long i) { *this += std::to_string(i); return *this; }
String &String::operator+=(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
String &String::operator+=(long long i) { *this += std::to_string(i); return *this; }
#if defined(MAC_OS)
String &String::operator+=(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
String &String::operator+=(float f) { *this += std::to_string(f); return *this; }
String &String::operator+=(double d) { *this += std::to_string(d); return *this; }
String &String::operator+=(sbyte i) { *this += std::to_string(i); return *this; }
String &String::operator+=(subyte ui) { *this += std::to_string(ui); return *this; }
String &String::operator+=(sshort i) { *this += std::to_string(i); return *this; }
String &String::operator+=(sushort ui) { *this += std::to_string(ui); return *this; }
String &String::operator+=(char c) { resize(size()+1, c); return (*this); }
String &String::operator+=(const char *s) { append(s); return (*this); }
String &String::operator+=(const std::string &s) { append(s); return (*this); }
String &String::operator+=(const String &s) { append(s); return (*this); }
String &String::operator+=(const SString &s) { append(s); return (*this); }
String &String::operator+=(SObjPtr obj) {
    if (obj.isStr()) append(obj.string());
    else append(obj.toString());
    return *this;
}

String String::operator+(bool b) const { return String(*this)+=(b?"true":"false"); }
String String::operator+(int i) const { return String(*this)+=i; }
String String::operator+(unsigned int ui) const { return String(*this)+=ui; }
String String::operator+(size_t ui) const { return String(*this)+=ui; }
#if defined(WIN64_OS)
String String::operator+(long i) const { return String(*this)+=i; }
String String::operator+(unsigned long ui) const { return String(*this)+=ui; }
#endif
String String::operator+(long long i) const { return String(*this)+=i; }
#if defined(MAC_OS)
String String::operator+(unsigned long long ui) const { return String(*this)+=ui; }
#endif
String String::operator+(float f) const { return String(*this)+=f; }
String String::operator+(double d) const { return String(*this)+=d; }

String String::operator+(sbyte i) const { return String(*this)+=i; }
String String::operator+(subyte ui) const { return String(*this)+=ui; }
String String::operator+(sshort i) const { return String(*this)+=i; }
String String::operator+(sushort ui) const { return String(*this)+=ui; }
String String::operator+(char c) const { return String(*this) += c; }
String String::operator+(const char *s) const { return String(*this) += s; }
String String::operator+(const std::string &s) const { return String(*this) += s; }
String String::operator+(const String &s) const { return String(*this) += s; }
String String::operator+(const SString &s) const { return String(*this) += s; }
String String::operator+(SObjPtr obj) const { return String(*this) += obj; }

String &String::operator<<(bool b) { *this += b?"true":"false"; return *this; }
String &String::operator<<(int i) { *this += std::to_string(i); return *this; }
String &String::operator<<(unsigned int ui) { *this += std::to_string(ui); return *this; }
String &String::operator<<(size_t ui) { *this += std::to_string(ui); return *this; }
#if defined(WIN64_OS)
String &String::operator<<(long i) { *this += std::to_string(i); return *this; }
String &String::operator<<(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
String &String::operator<<(long long i) { *this += std::to_string(i); return *this; }
#if defined(MAC_OS)
String &String::operator<<(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
String &String::operator<<(float f) { *this += std::to_string(f); return *this; }
String &String::operator<<(double d) { *this += std::to_string(d); return *this; }
String &String::operator<<(sbyte i) { *this += std::to_string(i); return *this; }
String &String::operator<<(subyte ui) { *this += std::to_string(ui); return *this; }
String &String::operator<<(sshort i) { *this += std::to_string(i); return *this; }
String &String::operator<<(sushort ui) { *this += std::to_string(ui); return *this; }
String &String::operator<<(char c) { return (*this) += c; }
String &String::operator<<(const char *s) { return (*this) += s; }
String &String::operator<<(const std::string &s) { return (*this) += s; }
String &String::operator<<(const String &s) { return (*this) += s; }
String &String::operator<<(const SString &s) { return (*this) += s; }
String& String::operator<<(const SText& t) { return (*this) += t.string(); }
String& String::operator<<(const sio::SFile& f) { return (*this) += f.path(); }
String &String::operator<<(SObjPtr obj) { return (*this) += obj; }

String &String::operator*=(int num) {
    if (!empty()) {
        if (!num) clear();
        else if (1 < num) {
            auto s = size();
            resize(s* num); auto ori = ptr(), p = ori+s;
            sforin(i, 1, num) { CMemory<char>::copy(p, ori, s); p += s; }
        }
    }
    return *this;
}
String &String::operator*=(size_t num) { return (*this) *= (int)num; }
String String::operator*(int num) const { return String(*this) *= num; }
String String::operator*(size_t num) const { return String(*this) *= (int)num; }

bool String::isNumeric() const {
    return equal(R(/[+-]*\\d+/)) || equal(R(/0x[0-9a-fA-F]+/)) ||
    equal(R(/nan/i)) || equal(R(/[+-]*inf/i)) || equal(R(/[+-]*infinity/i)) ||
    equal(R(/[+-]*\\d+\\.\\d+/i)) || equal(R(/[+-]*\\d+[eE][+-]*\\d+/)) ||
    equal(R(/[+-]*\\d+\\/\\d+/)) || equal(R(/[+-]*[0-9.]*[+-]*[0-9.]+i/));
}
bool String::isQuoted() const {
    auto ins = _cinstance();
    if (ins.second < 2) return false;
    return (ins.first[0] == '\'' || ins.first[0] == '\"') && ins.first[0] == ins.first[ins.second-1] && ins.first[ins.second-2] != '\\';
}

bool String::empty() const { return !size(); }
size_t String::size() const { return _isLong()?_str._ls.size:(_str._ss.size>>1); }
size_t String::length() const { return size(); }
size_t String::capacity() const { return _isLong()?_str._ls.capacity:22; }
char *String::ptr(size_t idx) { return _isLong()?&_str._ls.str[idx]:_str._ss.str+idx; }
const char *String::ptr(size_t idx) const { return _isLong()?&_str._ls.str[idx]:_str._ss.str+idx; }
const char *String::cstr() const { return _isLong()?_str._ls.str:_str._ss.str; }
std::string String::toStr() const { return std::string(cstr()); }

char &String::operator[] (int idx) { return at(idx); }
const char &String::operator[] (int idx) const { return at(idx); }
char &String::at(int idx) {
    if (_isLong()) {
		int p = idx < 0 ? _str._ls.size + idx : idx;
        if (-1 < p && p < _str._ls.size) return _str._ls.str[p];
    }
    else {
        int p = idx<0?(_str._ss.size>>1)+idx:idx;
        if (-1 < p && p < _str._ss.size) return _str._ss.str[p];
    }
	throw SException(ERR_INFO, SLIB_RANGE_ERROR);
}
const char &String::at(int idx) const {
    if (_isLong()) {
        int p = idx<0?_str._ls.size+idx:idx;
        if (-1 < p && p < _str._ls.size) return _str._ls.str[p];
    }
    else {
        int p = idx<0?(_str._ss.size>>1)+idx:idx;
        if (-1 < p && p < _str._ss.size) return _str._ss.str[p];
    }
    throw SException(ERR_INFO, SLIB_RANGE_ERROR);
}
char &String::first(){ return at(0); }
const char &String::first() const{ return at(0); }
char &String::last() { return at(-1); }
const char &String::last() const { return at(-1); }

void String::interpret(subyte* bytes, size_t size) {
	if (size < SHORT_STRING_CAPACITY - 1) {
		if (size) Memory<char>::copy(_str._ss.str, (char*)bytes, size);
		_str._ss.str[size] = '\0';
		_str._ss.size = size << 1;
	}
	else {
		_str._ls.str = (char *)bytes;
		_str._ls.str[size] = '\0';
		_str._ls.size = size;
	}
}
void String::copy(const char *dat, size_t size) {
    if (!dat || dat[0] == '\0' || size == 0) clear();
    else {
        auto len = strlen(dat);
        if (size == -1 || len < size) size = len;
        if (!_isLong() && size < SHORT_STRING_CAPACITY-1) {
			Memory<char>::copy(_str._ss.str, dat, size);
            _str._ss.str[size] = '\0';
            _str._ss.size = size<<1;
        }
        else {
            reserve((((len>>4)+1)<<4)|0x01);
            CMemory<char>::copy(ptr(), dat, size);
            _str._ls.str[size] = '\0';
            _str._ls.size = size;
        }
    }
}
void String::clear() { resize(0); }
void String::resize(size_t s) {
    if (!_isLong() && s < SHORT_STRING_CAPACITY-1) {
        _str._ss.str[s] = '\0';
        _str._ss.size = s<<1;
        return;
    }
    else {
        if (!_isLong()) reserve((((s>>4)+1)<<4)|0x01);
        else if (_str._ls.capacity-1 <= s) {
            auto cap = _str._ls.capacity-1;
            while (cap <= s) cap<<=1;
            reserve(cap|0x01);
        }
    }
    _str._ls.str[s] = '\0';
    _str._ls.size = s;
}
void String::resize(size_t s, const char &c) {
    auto tmp = size();
    resize(s);
    if (tmp < s) {
        auto p = ptr(tmp);
        sforin(i, tmp, s) { *p = c; ++p; }
    }
}
void String::reserve(size_t s) {
    if (_isLong()) {
        if (_str._ls.capacity-1 < s) {
            _str._ls.capacity = s|0x01;
            auto tmp = CMemory<char>::alloc(_str._ls.capacity);
            CMemory<char>::copy(tmp, _str._ls.str, _str._ls.size+1);
            CMemory<char>::dealloc(_str._ls.str);
            _str._ls.str = tmp;
        }
    }
    else if (SHORT_STRING_CAPACITY-1 < s) {
        size_t cap = s|0x01, len = _str._ss.size>>1;
        auto tmp = CMemory<char>::alloc(cap);
        CMemory<char>::copy(tmp, _str._ss.str, len+1);
        _str._ls.capacity = cap;
        _str._ls.size = len;
        _str._ls.str = tmp;
    }
}
void String::swap(String &str) {
    if (_isLong()) {
        auto tmpc = _str._ls.capacity;
        auto tmps = _str._ls.size;
        auto tmpp = _str._ls.str;
        if (str._isLong()) {
            _str._ls.capacity = str._str._ls.capacity;
            _str._ls.size = str._str._ls.size;
            _str._ls.str = str._str._ls.str;
        }
        else {
            _str._ss.size = str._str._ss.size;
            CMemory<char>::copy(_str._ss.str, str._str._ss.str, SHORT_STRING_CAPACITY);
        }
        str._str._ls.capacity = tmpc;
        str._str._ls.size = tmps;
        str._str._ls.str = tmpp;
    }
    else {
        auto tmps = _str._ss.size;
        char tmpc[SHORT_STRING_CAPACITY];
        CMemory<char>::copy(tmpc, _str._ss.str, SHORT_STRING_CAPACITY);
        if (str._isLong()) {
            _str._ls.capacity = str._str._ls.capacity;
            _str._ls.size = str._str._ls.size;
            _str._ls.str = str._str._ls.str;
        }
        else {
            _str._ss.size = str._str._ss.size;
            CMemory<char>::copy(_str._ss.str, str._str._ss.str, SHORT_STRING_CAPACITY);
        }
        str._str._ss.size = tmps;
        CMemory<char>::copy(str._str._ss.str, tmpc, SHORT_STRING_CAPACITY);
    }
}

SArrayIterator<char> String::begin() { return SArrayIterator<char>(ptr()); }
SArrayCIterator<char> String::begin() const { return SArrayCIterator<char>(cstr()); }
SArrayIterator<char> String::end() { auto ins = _instance(); return SArrayIterator<char>(&ins.first[ins.second]); }
SArrayCIterator<char> String::end() const { auto ins = _cinstance(); return SArrayCIterator<char>(&ins.first[ins.second]); }

void String::add(const char &c) { _append(&c, 1); }
void String::append(const char *s) { if (s) _append(s, strlen(s)); }
void String::append(const std::string &s) { _append(s.c_str(), s.length()); }
void String::append(const String &s) { auto ins = s._cinstance(); _append(ins.first, ins.second); }
void String::append(const SString &s) { auto ins = s._cinstance(); _append(ins.first, ins.second); }

void String::insert(size_t idx, const char *s) { if (s) _insert(idx, s, strlen(s)); }
void String::insert(size_t idx, const std::string &s) { _insert(idx, s.c_str(), s.length()); }
void String::insert(size_t idx, const String &s) { auto ins = s._cinstance(); _insert(idx, ins.first, ins.second); }

void String::removeAt(size_t idx) { remove(idx, 1); }
void String::remove(size_t off, size_t len) {
    auto ins = _instance();
    if (off < ins.second) {
        if (len == -1 || ins.second < off+len) len = ins.second-off;
        if (ins.second-off-len) CMemory<char>::shift(&ins.first[off], &ins.first[off+len], ins.second-off-len);
        resize(ins.second-len);
    }
}
void String::remove(const srange &rng) { remove(rng.begin, rng.end-rng.begin); }
void String::replace(const srange &rng, const char *alt) { replace(rng.begin, rng.end-rng.begin, alt); }
void String::replace(size_t off, size_t len, const char *alt) {
    if (alt) {
        auto ins = _instance();
        if (off < ins.second) {
            if (len == -1 || ins.second < off+len) len = ins.second-off;
            auto alen = strlen(alt);
            if (len < alen) {
                CMemory<char>::copy(&ins.first[off], alt, len);
                insert(off+len, &alt[len]);
            }
            else if (alen < len) {
                CMemory<char>::copy(&ins.first[off], alt, alen);
                remove(off+alen, len-alen);
            }
            else CMemory<char>::copy(&ins.first[off], alt, len);
        }
    }
}
void String::replace(const char *ori, const char *alt) {
    if (ori && alt && ori[0] != '\0') {
        auto len = strlen(ori), alen = strlen(alt);
        auto pos = search(ori);//findAll(ori);
        if (pos.empty()) return;
        else {
            String tmp;
            tmp.reserve(capacity());
            size_t off = 0;
            sforeach(pos) {
                if (E_ < off) continue;
                tmp += substring(off, E_-off);
                tmp += alt;
                off = E_+len;
            }
            tmp += substring(off, size()-off);
            swap(tmp);
        }
    }
}
void String::replace(const Regex &rgx, const char *alt) {
    *this = rgx.replace(cstr(), alt);
}
void String::rearrange(const Regex &rgx, const CArray<sint> &order) {
    *this = rgx.rearrange(cstr(), order);
}
void String::clip(size_t off, size_t len) {
    auto ins = _instance();
    if (off < ins.second) {
        if (off) remove(0, off);
        if (len == -1 || ins.second < off+len) len = ins.second-off;
        resize(len);
    }
}
void String::clip(const srange &rng) { clip(rng.begin, rng.end-rng.begin); }
void String::fill(size_t s, char fill, bool head) {
    auto tmp = size();
    if (tmp < s) {
        resize(s, fill);
        if (head) {
            auto p = ptr();
            CMemory<char>::shift(&p[s-tmp], p, tmp);
            sforin(i, tmp, s) { *p = fill; ++p; }
        }
    }
}
void String::trimming() {
    auto ins = _instance();
    auto beg = ins.first, end = &ins.first[ins.second];
    while (beg < end && Char::isWSChar(*beg)) ++beg;
    while (beg < end && Char::isWSChar(*(end-1))) --end;
    if (ins.first < beg) remove(0, beg-ins.first);
    resize(end-beg);
}
void String::transform(subyte trans) {
    auto ins = _instance();
    auto beg = ins.first, end = &ins.first[ins.second];
    if (trans&SString::DELETE_QUOTE) { ++beg; --end; }
    if (ins.first < beg) remove(0, beg-ins.first);
    resize(end-beg);
    if (trans&SString::SINGLE_QUOTE) { insert(0, "\'"); append("\'"); }
    if (trans&SString::DOUBLE_QUOTE) { insert(0, "\""); append("\""); }
    if (trans&SString::TO_UPPER)
        std::transform(this->begin(), this->end(), begin(), toupper);
    else if (trans&SString::TO_LOWER)
        std::transform(this->begin(), this->end(), begin(), tolower);
    if (trans&SString::TO_WIDE) {
        /*
         *
         */
    }
    else if (trans&SString::TO_NARROW) {
        /*
         *
         */
    }
}
String String::substring(size_t off, size_t len) const {
    auto ins = _cinstance();
    if (off < ins.second) {
        if (len == -1 || ins.second < off+len) len = ins.second-off;
        if (len) {
            String sub(len, 0);
            CMemory<char>::copy(sub.ptr(), &ins.first[off], len);
            return sub;
        }
    }
    return String();
}
String String::substring(const srange &range) const { return substring(range.begin, range.length()); }
String String::replaced(const char *ori, const char *alt) const {
    if (!ori || !alt || ori[0] == '\0') return *this;
    auto ins = _cinstance();
    if (!ins.second) return "";
    SString str;
    str.reserve(capacity());
    size_t len = strlen(ori), alen = strlen(alt);
    const char *init = ins.first, *end = ins.first+ins.second, *pos = _find(ori, len, init, end);
    while (pos) {
        str._append(init, pos-init);
        str._append(alt, alen);
        init = pos+len;
        pos = _find(ori, len, init, end);
    }
    return str;
}
String String::replaced(const Regex &rgx, const char *alt) const {
    return rgx.replace(cstr(), alt);
}

String String::rearranged(const Regex &rgx, const intarray &order) const {
    return rgx.rearrange(cstr(), order);
}
String String::filled(size_t size, char fill, bool head) const {
    String str;
    auto ins = _cinstance();
    if (ins.second < size) {
        str.resize(size, fill);
        if (head) CMemory<char>::copy(str.ptr(size-ins.second), ins.first, ins.second);
        else CMemory<char>::copy(str.ptr(), ins.first, ins.second);
    }
    else str = *this;
    return str;
}
String String::transformed(uint8_t trans) const {
    String str = *this;
    str.transform(trans);
    return str;
}

size_t String::charCount() const { size_t count = 0; if (!empty()) { sforeachc(*this) ++count; } return count; }
size_t String::charIndex(size_t idx) const { return (ubegin()+idx)->index(); }
Char String::charAt(size_t idx) const { return *(ubegin()+idx); }
String String::strAt(size_t idx) const { return (ubegin()+idx)->toStr(); }
SUtf8Iterator String::ubegin() { return SUtf8Iterator(this, ptr()); }
SUtf8CIterator String::ubegin() const { return SUtf8CIterator(this, ptr()); }
SUtf8Iterator String::uend() { return SUtf8Iterator(this, ptr(size())); }
SUtf8CIterator String::uend() const { return SUtf8CIterator(this, ptr(size())); }

size_t String::count(const char *s, size_t offset) const {
    size_t c = 0;
    if (s) {
        auto len = strlen(s);
        while (offset != NOT_FOUND) {
            offset = find(s, offset);
            if (offset == NOT_FOUND) break;
            else { ++c; ++offset; }
        }
    }
    return c;
}
bool String::contain(const char *que, size_t offset) const {
    return find(que, offset) != NOT_FOUND;
}
bool String::match(const Regex &rgx, size_t offset) const {
    return rgx.match(&at(offset));
}
bool String::equal(const Regex &rgx) const {
    return rgx.equal(cstr());
}
size_t String::find(const char *que, size_t offset) const {
    if (que && que[0] != '\0') {
        auto ins = _cinstance();
        auto pos = _find(que, strlen(que), ins.first+offset, ins.first+ins.second);
        if (pos) return pos-ins.first;
    }
    return NOT_FOUND;
}
size_t String::rfind(const char *que, size_t offset) const {
    if (que && que[0] != '\0') {
        auto ins = _cinstance();
        auto pos = _rfind(que, strlen(que), ins.first, ins.first+ins.second-offset-1);
        if (pos) return pos-ins.first;
    }
    return NOT_FOUND;
}
sizearray String::search(const char *que, size_t offset) const {
    sizearray array;
    if (!que) return array;
    auto len = strlen(que), pos = offset;
    while (offset+len <= length()) {
        if ((pos = find(que, offset))!= NOT_FOUND) {
            array.add(pos); offset = pos+1;
        }
        else break;
    }
    return array;
}
sizearray String::search(const Regex &rgx, size_t offset) const {
    sizearray array;
    rgx.search(array, &at(offset), &last());
    return array;
}
stringarray String::matched(const Regex &rgx, size_t offset) const {
    stringarray array;
    rgx.search(array, &at(offset), &last());
    return array;
}
inline void addsubstr(const String &str, stringarray &array, size_t init, size_t current, bool trim) {
    auto l = current-init;
    if (trim) {
        while (init < current && Char::isWSChar(str[init])) ++init;
        l = current-init;
        while (0 < l && Char::isWSChar(str[init+l-1])) --l;
    }
    if (l) array.add(str.substring(init, l));
    else array.add("");
}
stringarray String::split(const char *sep, bool trim) const {
    stringarray array;
    if (sep) {
        auto len = strlen(sep);
        if (len) {
            bool sq = false, dq = false;
            auto ins = _cinstance();
            auto off = 0, pos = 0;
            while (off < ins.second) {
                if (*ins.first == '\'' && !dq) sq = !sq;
                else if (*ins.first == '\"' && !sq) dq = !dq;
                else if (*ins.first == sep[0] && !sq && !dq && off+len <= ins.second) {
                    if (!memcmp(ins.first, sep, len)) {
                        addsubstr(*this, array, pos, off, trim);
                        pos = off+len; off += len-1; ins.first += len-1;
                    }
                }
                ++off; ++ins.first;
            }
            addsubstr(*this, array, pos, off, trim);
        }
    }
    return array;
}

stringarray String::splitline(bool trim) const {
    stringarray array;
    if(empty()) return array;
    bool sq = false, dq = false;
    auto ins = _cinstance();
    auto off = 0, pos = 0;
    while (off < ins.second) {
        if (*ins.first == '\'' && !dq) sq = !sq;
        else if (*ins.first == '\"' && !sq) dq = !dq;
        else if ((*ins.first == '\n' || *ins.first == '\r') && !sq && !dq) {
            addsubstr(*this, array, pos, off, trim);
            if (off < ins.second-1 && *ins.first == '\r' && *(ins.first+1) == '\n') {
                pos = off+2; ++off; ++ins.first;
            }
            else pos = off+1;
        }
        ++off; ++ins.first;
    }
    addsubstr(*this, array, pos, off, trim);
    return array;
}
stringarray String::split(const Regex &rgx) const {
    stringarray array;
    rgx.split(array, this);
    return array;
}
Map<String, String> String::parse(const char *sep, const char *part, bool trim) const {
    Map<String, String> attr;
    String key;
    size_t pos, len;
    stringarray array = split(sep, trim);
    sforeach(array) {
        len = it->size();
        if (!len) continue;
        pos = it->find(part);
        if (!pos || pos == NOT_FOUND) key = "key";
        else key = E_.substring(0, pos);
        pos+=strlen(part);
        if (trim) {
            key.trimming();
            while (pos < len && Char::isWSChar(E_[pos])) ++pos;
            while (pos < len && Char::isWSChar(E_[len-1])) --len;
        }
        if (pos == len) attr[key] = "";
        else attr[key] = it->substring(pos, len-pos);
    }
    return attr;
}
bool String::beginWith(const char *que) const {
    if (que) {
        auto len = strlen(que);
        if (len <= size()) return !(memcmp(cstr(), que, len));
    }
    return false;
}
bool String::endWith(const char *que) const {
    if (que) {
        auto len = strlen(que);
        if (len <= size()) return !(memcmp(ptr(size()-len), que, len));
    }
    return false;
}

bool String::boolean() const {
    auto ins = _cinstance();
    return strcmp(ins.first, "false") && strcmp(ins.first, "FALSE") && strcmp(ins.first, "NO") && strcmp(ins.first, "no");
}
sbyte String::byteValue() const {
    try { return atoi(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "byte"));
    }
}
subyte String::ubyteValue() const {
    try { return atoi(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "ubyte"));
    }
}
sshort String::shortValue() const {
    try { return atoi(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "short"));
    }
}
sushort String::ushortValue() const {
    try { return atoi(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "ushort"));
    }
}
int String::intValue() const {
    try { return atoi(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "int"));
    }
}
unsigned int String::uintValue() const {
    try { return atol(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "uint"));
    }
}
size_t String::sizeValue() const {
    try { return atol(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "size"));
    }
}
long String::longValue() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "long"));
    }
}
unsigned long String::ulongValue() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "ulong"));
    }
}
long long String::llongValue() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "llong"));
    }
}
unsigned long long String::ullongValue() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "ullong"));
    }
}
float String::floatValue() const {
    try { return atof(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "float"));
    }
}
double String::doubleValue() const {
    try { return atof(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "double"));
    }
}
sinteger String::integer() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "integer"));
    }
}
suinteger String::uinteger() const {
    try { return atoll(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "uinteger"));
    }
}
sreal String::real() const {
    try { return atof(cstr()); }
    catch (std::invalid_argument e) {
        throw SException(ERR_INFO, SLIB_CAST_ERROR, cstr(), CAST_TEXT("string", "real"));
    }
}
SNumber String::number() const {
    //Integer
    if (equal(R(/[+-]*\\d+/)) || equal(R(/0x[0-9a-fA-F]+/))) return SNumber(atoll(cstr()));
    else if (equal(R(/null/i))) return SNumber(0);
    //Real number
    else if (equal(R(/nan/i))) return SNumber(NAN);
    else if (equal(R(/[+-]*inf/i)) || equal(R(/[+-]*infinity/i))) {
        if (match("-")) return SNumber(-INFINITY);
        else return SNumber(INFINITY);
    }
    else if (equal(R(/[+-]*\\d+\\.\\d+/i)) ||
             equal(R(/[+-]*\\d+[eE][+-]*\\d+/))) {
        return SNumber(atof(cstr()));
    }
    //Boolean
    else if (equal(R(/true|yes/i))) return SNumber(true);
    else if (equal(R(/false|no/i)))  return SNumber(false);
    //Fraction
    else if (equal(R(/[+-]*\\d+\\/\\d+/))) return SNumber(sfrac(cstr()));
    //Complex
    else if (equal(R(/[+-]*[0-9.]*[+-]*[0-9.]+i/))) return SNumber(scomp(cstr()));
    throw SException(ERR_INFO);
}
#if defined(WIN32_OS) || defined(WIN64_OS)
std::wstring String::unicode() const {
	auto wsize = ::MultiByteToWideChar(CP_UTF8, 0U, ptr(), -1, nullptr, 0U);
	std::wstring ws(wsize, '\0');
	auto res = ::MultiByteToWideChar(CP_UTF8, 0U, ptr(), -1, &ws[0], wsize);
	if (res == 0) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "this");
	return ws;
}
String String::localize() const {
auto wsize = ::MultiByteToWideChar(CP_UTF8, 0U, ptr(), -1, nullptr, 0U);
	Array<wchar_t> ws(wsize);
	auto res = ::MultiByteToWideChar(CP_UTF8, 0U, ptr(), -1, ws.ptr(), wsize);
	if (res == 0) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "this");
	auto sjissize = ::WideCharToMultiByte(CP_ACP, 0U, ws.ptr(), -1, nullptr, 0, nullptr, nullptr);
	String str(sjissize, '\0');
	res = ::WideCharToMultiByte(CP_ACP, 0U, ws.ptr(), -1, str.ptr(), sjissize, nullptr, nullptr);
	if (res == 0) throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "this");
	return str;
}
#endif
String::operator bool() const { return boolean(); }
String::operator sbyte() const { return byteValue(); }
String::operator subyte() const { return ubyteValue(); }
String::operator sshort() const { return shortValue(); }
String::operator sushort() const { return ushortValue(); }
String::operator int() const { return intValue(); }
String::operator unsigned int() const { return uintValue(); }
String::operator size_t() const { return sizeValue(); }
#if defined(WIN64_OS)
String::operator long() const { return longValue(); }
String::operator unsigned long() const { return ulongValue(); }
#endif
String::operator long long() const { return llongValue(); }
#if defined(MAC_OS)
String::operator unsigned long long() const { return ullongValue(); }
#endif
String::operator float() const { return floatValue(); }
String::operator double() const { return doubleValue(); }
String::operator const char *() const { return cstr(); }

bool String::operator < (const char *s) const { return strcmp(cstr(), s) < 0; }
bool String::operator < (const std::string &s) const { return strcmp(cstr(), s.c_str()) < 0; }
bool String::operator < (const String &s) const { return strcmp(cstr(), s.cstr()) < 0; }
bool String::operator < (const SString &s) const { return strcmp(cstr(), s.cstr()) < 0; }
bool String::operator == (const char *s) const { return !strcmp(cstr(), s); }
bool String::operator == (const std::string &s) const { return !strcmp(cstr(), s.c_str()); }
bool String::operator == (const String &s) const { return !strcmp(cstr(), s.cstr()); }
bool String::operator == (const SString &s) const { return !strcmp(cstr(), s.cstr()); }
bool String::operator != (const char *s) const { return strcmp(cstr(), s); }
bool String::operator != (const std::string &s) const { return strcmp(cstr(), s.c_str()); }
bool String::operator != (const String &s) const { return strcmp(cstr(), s.cstr()); }
bool String::operator != (const SString &s) const { return strcmp(cstr(), s.cstr()); }

String slib::operator+(const char &c, const String &s) { return String(c)+s; }
String slib::operator+(const char *s1, const String &s2) { return String(s1)+s2; }
String slib::operator+(const std::string &s1, const String &s2) { return String(s1)+s2; }
