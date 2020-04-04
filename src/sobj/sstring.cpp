#include "sobj/snumber.h"
#include "sobj/snode.h"
#include "sobj/sstring.h"
#include "sio/sfile.h"

using namespace slib;
using namespace slib::sio;

SChar::SChar() : Char() {}
SChar::SChar(String *s, const char *c) : Char(s, c) {}
SChar::SChar(const Char &c) : Char(c) {}
SChar::SChar(const SChar &c) : Char() {
    _str = c._str; _ptr = c._ptr;
}
SChar::~SChar() {}

SChar& SChar::operator=(const char* s) {
	if (_str) {
		if (s) _str->replace(index(), u8size(_ptr), s);
		else _str->remove(index(), u8size(_ptr));
	}
	return *this;
}
SChar &SChar::operator=(const String &s) {
    if (_str) {
        if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
SChar &SChar::operator=(const SString &s) {
    if (_str) {
        if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
SChar &SChar::operator=(const Char &c) {
    *this = SChar(c);
    return *this;
}
SChar& SChar::operator=(const SChar& c) {
    _str = c._str; _ptr = c._ptr;
    return *this;
}

String SChar::getClass() const { return "char"; }
String SChar::toString() const { return Char::toString(); }
SObject *SChar::clone() const { return new SChar(*this); }

SString::SString() : SObject(), String(), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(bool b) : SObject(), String(b?"true":"false"), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(int i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(unsigned int ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(size_t ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
#if defined(WIN64_OS)
SString::SString(long i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(unsigned long ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
#endif
SString::SString(long long i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
#if defined(MAC_OS)
SString::SString(unsigned long long ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
#endif
SString::SString(float f) : SObject(), String(std::to_string(f)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(double d) : SObject(), String(std::to_string(d)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(sbyte i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(subyte ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(sshort i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(sushort ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const size_t &s, char c) : SObject(), String(s, c), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(char c) : SObject(), String(c), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const SChar &c) : SObject(), String(c.toStr()), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const char *s) : SObject(), String(s), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const String &s) : SObject(), String(s), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const std::string &s) : SObject(), String(s), _char(SChar(this, cstr()))/*, _offset(nullptr)*/ {}
SString::SString(const sobj &obj) : SString() {
    if (obj.isStr()) *this = obj.string();
    else *this = obj.toString();
}
SString::SString(const SNumber &sn) : SString(sn.toString()) {}
SString::SString(SString &&s) : SString() { s.swap(*this); }
SString::SString(const SString &s) : SString() {
    auto len = s.length();
    if (len < SHORT_STRING_CAPACITY-1) {
        if (len) CMemory<char>::copy(_str._ss.str, &s[0], len);
        _str._ss.size = len << 1;
    }
    else {
        _str._ls.capacity = (((len>>4)+1)<<4)|0x01;
        _str._ls.str = CMemory<char>::alloc(_str._ls.capacity);
        CMemory<char>::copy(_str._ls.str, &s[0], len);
        _str._ls.size = len; _str._ls.str[len] = '\0';
    }
}
SString::~SString() {}

SString &SString::operator=(bool b) { *this = (b?"true":"false"); return *this; }
SString &SString::operator=(int i) { *this = std::to_string(i); return *this; }
SString &SString::operator=(unsigned int ui) { *this = std::to_string(ui); return *this; }
SString &SString::operator=(size_t ui) { *this = std::to_string(ui); return *this; }
#if defined(WIN64_OS)
SString &SString::operator=(long i) { *this = std::to_string(i); return *this; }
SString &SString::operator=(unsigned long ui) { *this = std::to_string(ui); return *this; }
#endif
SString &SString::operator=(long long i) { *this = std::to_string(i); return *this; }
#if defined(MAC_OS)
SString &SString::operator=(unsigned long long ui) { *this = std::to_string(ui); return *this; }
#endif
SString &SString::operator=(float f) { *this = std::to_string(f); return *this; }
SString &SString::operator=(double d) { *this = std::to_string(d); return *this; }
SString &SString::operator=(sbyte i) { *this = std::to_string(i); return *this; }
SString &SString::operator=(subyte ui) { *this = std::to_string(ui); return *this; }
SString &SString::operator=(sshort i) { *this = std::to_string(i); return *this; }
SString &SString::operator=(sushort ui) { *this = std::to_string(ui); return *this; }
SString &SString::operator=(char c) { String::resize(1, c); return *this; }
SString &SString::operator=(const char *s) { String::copy(s); return *this; }
//SString &SString::operator=(const SChar &c);
SString &SString::operator=(const std::string &s) {
	String::copy(s.c_str(), s.length()); return *this;
}
SString &SString::operator=(const String &s) {
    String::copy(s.cstr(), s.length()); return *this;
}
SString &SString::operator=(const SString &s) {
    auto ins = s._cinstance();
	String::copy(ins.first, ins.second);
    return *this;
}
SString &SString::operator=(const SNumber &n) { *this = n.toString(); return *this; }
SString &SString::operator=(const sobj &obj) {
    if (obj.isStr()) *this = obj.string(); else *this = obj->toString(); return *this;
}

SString &SString::operator+=(bool b) { *this += (b?"true":"false"); return *this; }
SString &SString::operator+=(int i) { *this += std::to_string(i); return *this; }
SString &SString::operator+=(unsigned int ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator+=(size_t ui) { *this += std::to_string(ui); return *this; }
#if defined(WIN64_OS)
SString &SString::operator+=(long i) { *this += std::to_string(i); return *this; }
SString &SString::operator+=(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
SString &SString::operator+=(long long i) { *this += std::to_string(i); return *this; }
#if defined(MAC_OS)
SString &SString::operator+=(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
SString &SString::operator+=(float f) { *this += std::to_string(f); return *this; }
SString &SString::operator+=(double d) { *this += std::to_string(d); return *this; }
SString &SString::operator+=(sbyte i) { *this += std::to_string(i); return *this; }
SString &SString::operator+=(subyte ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator+=(sshort i) { *this += std::to_string(i); return *this; }
SString &SString::operator+=(sushort ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator+=(char c) { (*this) += std::string(1, c); return (*this); }
SString &SString::operator+=(const Char &c) { (*this) += c.toStr(); return (*this); }
SString &SString::operator+=(const SChar &c) { (*this) += c.toString(); return (*this); }
SString &SString::operator+=(const char *s) { this->append(s); return (*this); }
SString &SString::operator+=(const std::string &s) { this->append(s); return (*this); }
SString &SString::operator+=(const String &s) { this->append(s); return (*this); }
SString &SString::operator+=(const SString &s) { this->append(s); return (*this); }
SString &SString::operator+=(const SNumber &n) { (*this) += n.toString(); return *this; }
SString &SString::operator+=(const sobj &obj) {
    if (obj.isStr()) return *this += obj.string();
    else return  *this += obj->toString();
}

SString SString::operator+(bool b) const { return SString(*this)+=(b?"true":"false"); }
SString SString::operator+(int i) const { return SString(*this)+=std::to_string(i); }
SString SString::operator+(unsigned int ui) const { return SString(*this)+=std::to_string(ui); }
SString SString::operator+(size_t ui) const { return SString(*this)+=std::to_string(ui); }
#if defined(WIN64_OS)
SString SString::operator+(long i) const { return SString(*this)+=std::to_string(i); }
SString SString::operator+(unsigned long ui) const { return SString(*this)+=std::to_string(ui); }
#endif
SString SString::operator+(long long i) const { return SString(*this)+=std::to_string(i); }
#if defined(MAC_OS)
SString SString::operator+(unsigned long long ui) const { return SString(*this)+=std::to_string(ui); }
#endif
SString SString::operator+(float f) const { return SString(*this)+=std::to_string(f); }
SString SString::operator+(double d) const { return SString(*this)+=std::to_string(d); }
SString SString::operator+(sbyte i) const { return SString(*this)+=std::to_string(i); }
SString SString::operator+(subyte ui) const { return SString(*this)+=std::to_string(ui); }
SString SString::operator+(sshort i) const { return SString(*this)+=std::to_string(i); }
SString SString::operator+(sushort ui) const { return SString(*this)+=std::to_string(ui); }
SString SString::operator+(char c) const { return SString(*this) += std::string(1, c); }
SString SString::operator+(const Char &c) const { return SString(*this) += c.toStr(); }
SString SString::operator+(const SChar &c) const { return SString(*this) += c.toString(); }
SString SString::operator+(const char *s) const { return SString(*this) += std::string(s); }
SString SString::operator+(const std::string &s) const { return SString(*this) += s; }
SString SString::operator+(const String &s) const { return SString(*this) += s; }
SString SString::operator+(const SString &s) const { return SString(*this) += s; }
SString SString::operator+(const SNumber &n) const { return SString(*this) += n.toString(); }
SString SString::operator+(const sobj &obj) const {
    if (obj.isStr()) return SString(*this) += obj.string();
    else return SString(*this) += obj.toString();
}

SString &SString::operator<<(bool b) { *this += (b?"true":"false"); return *this; }
SString &SString::operator<<(int i) { *this += std::to_string(i); return *this; }
SString &SString::operator<<(unsigned int ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator<<(size_t ui) { *this += std::to_string(ui); return *this; }
#if defined(WIN64_OS)
SString &SString::operator<<(long i) { *this += std::to_string(i); return *this; }
SString &SString::operator<<(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
SString &SString::operator<<(long long i) { *this += std::to_string(i); return *this; }
#if defined(MAC_OS)
SString &SString::operator<<(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
SString &SString::operator<<(float f) { *this += std::to_string(f); return *this; }
SString &SString::operator<<(double d) { *this += std::to_string(d); return *this; }
SString &SString::operator<<(sbyte i) { *this += std::to_string(i); return *this; }
SString &SString::operator<<(subyte ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator<<(sshort i) { *this += std::to_string(i); return *this; }
SString &SString::operator<<(sushort ui) { *this += std::to_string(ui); return *this; }
SString &SString::operator<<(char c) { return (*this)+=std::string(1, c); }
SString &SString::operator<<(const Char &c) { return (*this) += c.toStr(); }
SString &SString::operator<<(const SChar &c) { return (*this) += c.toString(); }
SString &SString::operator<<(const char *s) { return (*this)+=s;; }
SString &SString::operator<<(const std::string &s) { return (*this)+=s; }
SString &SString::operator<<(const String &s) { return (*this)+=s; }
SString &SString::operator<<(const SString &s) { return (*this)+=s; }
SString &SString::operator<<(const SNumber &n) { return (*this)+=n; }
SString &SString::operator<<(const sobj &obj) { return (*this)+=obj; }
SString &SString::operator*=(const int &i) {
    if (!i) clear();
    else if (1 < i) {
        resize(size()*i);
        auto ins = _instance();
        auto p = &ins.first[ins.second];
        sforin(i, 1, i) { CMemory<char>::copy(p, ins.first, ins.second); p+=ins.second; }
    }
    return *this;
}
SString &SString::operator*=(const size_t & num) {
    if (!num) clear();
    else if (1 < num) {
        resize(size()* num);
        auto ins = _instance();
        auto p = &ins.first[ins.second];
		sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p+=ins.second; }
    }
    return *this;
}
SString SString::operator*(const int & num) const {
    SString str;
    if (num) {
        auto ins = _cinstance();
        str.resize(ins.second* num);
        auto p = str.ptr();
        sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p+=ins.second; }
    }
    return str;
}
SString SString::operator*(const size_t & num) const {
    SString str;
    if (num) {
        auto ins = _cinstance();
        str.resize(ins.second* num);
        auto p = str.ptr();
        sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p+=ins.second; }
    }
    return str;
}
//void SString::setOffset(const char *p) { _offset = p; }
void SString::load(const char *path) {
    clear();
    sio::SFile file(path, sio::READ);
    file>>(*this);
}
void SString::save(const char *path) {
    sio::SFile file(path, sio::CREATE);
    file<<(*this);
}

sobj& SString::u8char(int idx) { 
	_char.character() = charAt(idx);
	return _char;
}
const sobj& SString::u8char(int idx) const {
	const_cast<SChar &>(_char.character()) = charAt(idx);
	return _char;
}

String SString::getClass() const { return "string"; }
String SString::toString() const { return *this; }
SObject *SString::clone() const { return new SString(*this); }

bool SString::operator<(const char *s) const { return strcmp(cstr(), s) < 0; }
bool SString::operator<(const std::string &s) const { return *this < s.c_str(); }
bool SString::operator<(const sobj &obj) const {
    if (obj.isStr()) return *this < obj.string();
    return *this < obj.toString();
}
bool SString::operator<(const String &s) const { return *static_cast<const String *>(this) < s; }
bool SString::operator<(const SString &s) const {
    return *static_cast<const String *>(this) < *static_cast<const String *>(&s);
}
bool SString::operator==(const char *s) const { return !strcmp(cstr(), s); }
bool SString::operator==(const std::string &s) const { return *this == s.c_str(); }
bool SString::operator==(const sobj &obj) const {
    if (obj.isNull()) return false;
    else if (obj.isStr()) return (*this) == obj.string();
    return (*this) == obj.toString();
}
bool SString::operator==(const String &s) const { return *static_cast<const String *>(this) == s; }
bool SString::operator==(const SString &s) const {
    return *static_cast<const String *>(this) == *static_cast<const String *>(&s);
}
bool SString::operator!=(const char *s) const { return strcmp(cstr(), s); }
bool SString::operator!=(const std::string &s) const { return *this != s.c_str(); }
bool SString::operator!=(const sobj &obj) const { return !((*this) == obj); }
bool SString::operator!=(const String &s) const { return !(*this == s); }
bool SString::operator!=(const SString &s) const { return !(*this == s); }
SString slib::operator+(const char &c, const SString &s) { return SString(c)+=s; }
SString slib::operator+(const char *s1, const SString &s2) { return SString(s1)+=s2; }
SString slib::operator+(const int &i, const SString &s) { return SString(i)+=s; }
SString slib::operator+(const sinteger &i, const SString &s) { return SString(i)+=s; }
SString slib::operator+(const size_t &u, const SString &s) { return SString(u)+=s; }
SString slib::operator+(const float &f, const SString &s) { return SString(f)+=s; }
SString slib::operator+(const sreal &r, const SString &s) { return SString(r)+=s; }
SString slib::operator+(const bool &b, const SString &s) { return SString(b)+=s; }
//SString slib::operator+(const char *s, const SNumber &num) { return SString(s) += num.toString(); }
String slib::operator+(const char *s, const sobj &obj) {
    if (obj.isStr()) return String(s) += obj.string();
    else return String(s) += obj->toString();
}
String slib::operator+(const std::string &s, const sobj &obj) {
    if (obj.isStr()) return String(s) += obj.string();
    else return String(s) += obj->toString();
}
