#include "sbasic/char.h"
#include "sbasic/string.h"
#include "sobj/sstring.h"

using namespace slib;

sbyte Char::u8size(const char *c) {
    const subyte i = *reinterpret_cast<const subyte *>(c);
    if (i == 0) return 0;
    else if (i < 0x80) return 1;
    else if (i < 0xC0) return -1;
    else if (i < 0xE0) return 2;
    else if (i < 0xF0) return 3;
    return -1;
}
bool Char::isNarrow(const char* s) { return u8size(s) == 1; }
bool Char::isWide(const char* s) { return 1 < u8size(s); }
bool Char::isNumeric(const char* s) { return u8size(s) == 1 ? isNumChar(s[0]) : isWideNum(s); }
bool Char::isNumChar(char c) { return 0x2F < c && c < 0x3A; }
bool Char::isWideNum(const char *s) {
    return u8size(s) == 3 && s[0] == (char)0xEF &&
    ((s[1] == (char)0xBC &&
      (((char)0x8F < s[2] && s[2] < (char)0x9A) ||
       ((char)0xA0 < s[2] && s[2] < (char)0xA7))) ||
     (s[1] == (char)0xBD && ((char)0x80 < s[2] && s[2] < (char)0x87)));
}
bool Char::isWSChar(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
sbyte Char::toNum(char c) {
	if (0x2F < c && c < 0x3A) return (subyte)c - 0x30;
	else if (0x40 < c && c < 0x47) return (subyte)c - 0x37;
	else if (0x60 < c && c < 0x67) return (subyte)c - 0x57;
    return 0;
}
sbyte Char::toNum(const char *s) {
    if (s[1] == (char)0xBC) {
        if ((char)0x8F < s[2] && s[2] < (char)0x9A) return s[2]-(char)0x90;
        else if ((char)0xA0 < s[2] && s[2] < (char)0xA7) return s[2]-(char)0x97;
    }
    else if (s[1] == (char)0xBD) {
        if((char)0x80 < s[2] && s[2] < (char)0x87) return s[2]-(char)0x77;
    }
    return s[0];
}
String Char::wideChar(char c) {
    if (0x20 < (int)c && (int)c < 0x7F) {
        String str(3, '\0');
		str[0] = 0xEF; str[1] = 0xBC; str[2] = c + 0x60;
        return str;
    }
    else return String(1, c);
}
char Char::narrowChar(const char *s) {
    size_t len = Char::u8size(s);
    if (len == 1) return s[0];
    const subyte *p = reinterpret_cast<const subyte *>(s);
	if (len == 3 && 0x80 < p[2] && p[2] < 0x9F)
		return (char)(0x20 + (p[2] - 0x80));
    else throw SException(ERR_INFO, SLIB_FORMAT_ERROR, "s");
}

Char::Char() : _ptr(nullptr), _str(nullptr) {}
Char::Char(String *s, const char *c) : _ptr(c), _str(s) {}
Char::Char(const Char &c) : _ptr(c._ptr), _str(c._str) {}
Char::~Char() {}
Char &Char::operator=(const char *s) {
    if (_str) {
        if (s && s[0] != '\0') _str->replace(index(), u8size(_ptr), s);
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
Char &Char::operator=(const std::string &s) {
    if (_str) {
        if (s.length()) _str->replace(index(), u8size(_ptr), s.c_str());
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
Char &Char::operator=(const String &s) {
    if (_str) {
        if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
Char &Char::operator=(const SString &s) {
    if (_str) {
        if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
        else _str->remove(index(), u8size(_ptr));
    }
    return *this;
}
Char &Char::operator=(const Char &c) {
    _ptr = c._ptr; _str = c._str; return *this;
}
Char &Char::operator=(const SChar &c) {
    _ptr = c._ptr; _str = c._str; return *this;
}

Char &Char::operator=(const SObjPtr &obj) {
    if (_str) {
        if (obj.isNull())  _str->remove(index(), u8size(_ptr));
        else if (obj.isChar()) *this = obj.character();
        else if (obj.isStr()) {
            if (obj.length()) _str->replace(index(), u8size(_ptr), obj.string().cstr());
            else _str->remove(index(), u8size(_ptr));
        }
        else {
            auto s = obj.toString();
            if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
            else _str->remove(index(), u8size(_ptr));
        }
    }
    return *this;
}
Char& Char::operator+=(int p) { sforin(i, 0, p) ++(*this); return *this; }
Char& Char::operator-=(int p) { sforin(i, 0, p) --(*this); return *this; }
Char& Char::operator ++() { if (_str) { _ptr += u8size(_ptr); } return *this; }
Char &Char::operator --() {
	if (_str) { do { --_ptr; } while (u8size(_ptr) < 0); }
	return *this;
}
size_t Char::index() const { return _str ? _ptr - _str->cstr() : 0; }
size_t Char::length() const { return _str ? u8size(_ptr) : 0; }
void Char::setIndex(size_t s) { _ptr = _str->cstr() + s; }
void Char::setOffset(size_t s) { _ptr = _str->cstr(); sforin(i, 0, s) _ptr += u8size(_ptr); }
const char *Char::cstr() const { return _str?_ptr:nullptr; }
std::string Char::toStr() const {
    std::string str(length(), 0);
    if (_str) CMemory<char>::copy(&str[0], _ptr, length());
    return str;
}
String Char::toString() const {
    String str(length(), 0);
    if (_str) CMemory<char>::copy(str.ptr(), _ptr, length());
    return str;
}
bool Char::operator<(const Char &c) const {
    if (!_str) return false;
    if (!c._str) return false;
    if (_str == c._str) return _ptr < c._ptr;
    return _str < c._str;
}
bool Char::operator==(const char &c) const {
    return Char::isNarrow(_ptr) && _ptr[0] == c;
}
bool Char::operator==(const char *s) const {
    return Char::u8size(_ptr) == u8size(s) && !(memcmp(_ptr, s, u8size(s)));
}
bool Char::operator==(const Char &c) const {
    return _str == c._str && _ptr == c._ptr;
}
