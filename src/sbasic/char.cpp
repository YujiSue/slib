#include "sbasic/char.h"
#include "sbasic/memory.h"
#include "sbasic/exception.h"
bool slib::sutf8::check(const char* c) { 
    auto c1 = *reinterpret_cast<const subyte*>(c);
    // 1st byte check
    if (c1 < 0x80) return true;
    else if ((0x7F < c1 && c1 < 0xC2) || 0xF4 < c1) return false;
    // 2 bytes char check
    auto c2 = *reinterpret_cast<const subyte*>(++c);
    if (0xC1 < c1 && c1 < 0xE0) return 0x7F < c2 && c2 < 0xC0;
    // 3 bytes char check
    auto c3 = *reinterpret_cast<const subyte*>(++c);
    if (0xDF < c1 && c1 < 0xF0) {
        if (c1 == 0xE0) {
            if (c2 < 0xA0 || 0xBF < c2) return false;
        }
        else if (c1 == 0xED) {
            if (c2 < 0x80 || 0x9F < c2) return false;
        }
        else if (c2 < 0x80 || 0xBF < c2) return false;
        return (0x7F < c3 && c3 < 0xC0);
    }
    // 4 bytes char check
    auto c4 = *reinterpret_cast<const subyte*>(++c);
    if (c1 == 0xF0) {
        if (c2 < 0x90 || 0xBF < c2) return false;
    }
    else if (c1 == 0xF4) {
        if (c2 < 0x80 || 0x8F < c2) return false;
    }
    else if (c2 < 0x80 || 0xBF < c2) return false;
    return (0x7F < c3 && c3 < 0xC0) && (0x7F < c4 && c4 < 0xC0);
}
slib::sbyte slib::sutf8::size(const char* c) {
    auto c1 = *reinterpret_cast<const subyte*>(c);
    if (c1 == 0x00) return 0;
    else if (c1 < 0x80) return 1;
    else if ((0x7F < c1 && c1 < 0xC2) || 0xF4 < c1) return -1;
    auto c2 = *reinterpret_cast<const subyte*>(++c);
    if (0xC1 < c1 && c1 < 0xE0) return (0x7F < c2 && c2 < 0xC0) ? 2 : -1;
    auto c3 = *reinterpret_cast<const subyte*>(++c);
    if (0xDF < c1 && c1 < 0xF0) {
        if (c1 == 0xE0) {
            if (c2 < 0xA0 || 0xBF < c2) return -1;
        }
        else if (c1 == 0xED) {
            if (c2 < 0x80 || 0x9F < c2) return -1;
        }
        else if (c2 < 0x80 || 0xBF < c2) return -1;
        return (0x7F < c3 && c3 < 0xC0) ? 3 : -1;
    }
    auto c4 = *reinterpret_cast<const subyte*>(++c);
    if (c1 == 0xF0) {
        if (c2 < 0x90 || 0xBF < c2) return -1;
    }
    else if (c1 == 0xF4) {
        if (c2 < 0x80 || 0x8F < c2) return -1;
    }
    else if (c2 < 0x80 || 0xBF < c2) return -1;
    return (0x7F < c3 && c3 < 0xC0) && (0x7F < c4 && c4 < 0xC0) ? 4 : -1;
}
bool slib::sutf8::isNarrow(const char* s) { return size(s) == 1; }
bool slib::sutf8::isWide(const char* s) { return 1 < size(s); }
bool slib::sutf8::isWS(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
bool slib::sutf8::isNumChar(const char c) { return 0x2F < (subyte)c && (subyte)c < 0x3A; }
bool slib::sutf8::isWideNum(const char* s) { 
    return size(s) == 3 && s[0] == (char)0xEF &&
        ((s[1] == (char)0xBC &&
            ((0x8F < (subyte)s[2] && (subyte)s[2] < 0x9A) ||
                (0xA0 < (subyte)s[2] && (subyte)s[2] < 0xA7))) ||
            (s[1] == (char)0xBD && (0x80 < (subyte)s[2] && (subyte)s[2] < 0x87)));
}
bool slib::sutf8::isNumeric(const char* s) { return sutf8::size(s) == 1 ? isNumChar(s[0]) : isWideNum(s); }
char slib::sutf8::toChar(const slib::sbyte n) { return (char)(0x30 + n); }
slib::sbyte slib::sutf8::toNum(const char c) {
    if (0x2F < (subyte)c && (subyte)c < 0x3A) return (subyte)c - 0x30;
    else if (0x40 < (subyte)c && (subyte)c < 0x47) return (subyte)c - 0x37;
    else if (0x60 < (subyte)c && (subyte)c < 0x67) return (subyte)c - 0x57;
    return 0;
}
slib::sbyte slib::sutf8::toNum(const char* s) {
    if (!s) return 0;
    if (slib::sutf8::isWideNum(s)) {
        if (s[1] == (char)0xBC) {
            if (0x8F < (subyte)s[2] && (subyte)s[2] < 0x9A) return (subyte)s[2] - 0x90;
            else if (0xA0 < (subyte)s[2] && (subyte)s[2] < 0xA7) return (subyte)s[2] - 0x97;
            else throw RangeException(String("2nd and 3rd bytes of UTF-8 wide number character should be 0xBD81-0xBD86 or 0xBC90-0xBC99 or 0xBCA1-0xBCA6. But ") << (((int)s[1] << 8) | s[2]) << " was found.");
        }
        else if (s[1] == (char)0xBD) {
            if (0x80 < (subyte)s[2] && (subyte)s[2] < 0x87) return (subyte)s[2] - 0x77;
            else throw RangeException(String("2nd and 3rd bytes of UTF-8 wide number character should be 0xBD81-0xBD86 or 0xBC90-0xBC99 or 0xBCA1-0xBCA6. But ") << (((int)s[1] << 8) | s[2]) << " was found.");
        }
    }
    return slib::sutf8::toNum(s[0]);
}
char slib::sutf8::toUpper(const char c) {
    if (0x60 < (subyte)c && (subyte)c < 0x7B) return (char)((subyte)c - 0x20);
    return c;
}
char slib::sutf8::toLower(const char c) {
    if (0x40 < (subyte)c && (subyte)c < 0x5B) return (char)((subyte)c + 0x20);
    return c;
}
slib::String slib::sutf8::toWide(char c) {
    slib::String ws;
    if (0x20 < (int)c && (int)c < 0x60) {
        ws.resize(3);
        ws[0] = (char)0xEF; ws[1] = (char)0xBC; ws[2] = (char)((int)c + 0x60);
    }
    else if (0x5F < (int)c && (int)c < 0x7F) {
        ws.resize(3);
        ws[0] = (char)0xEF; ws[1] = (char)0xBD; ws[2] = (char)((int)c + 0x20);
    }
    else {
        ws.resize(1); ws[0] = c;
    }
    return ws;
}
char slib::sutf8::toNarrow(const char* s) {
    auto len = sutf8::size(s);
    if (len == 1) return s[0];
    else if (len == 3 && s[0] == (char)0xEF) {
        if (s[1] == (char)0xBC && 0x80 < (subyte)s[2] && (subyte)s[2] < 0xC0) return (char)((subyte)s[2] - 0x60);
        if (s[1] == (char)0xBD && 0x7F < (subyte)s[2] && (subyte)s[2] < 0x9F) return (char)((subyte)s[2] - 0x20);
        else throw RangeException(String("2nd and 3rd bytes of UTF-8 wide number character should be 0xBC81-0xBC8F or 0xBD80-0xBD9E. But ") << (((int)s[1] << 8) | s[2]) << " was found.");
    }
    else throw RangeException(String("1st byte of UTF-8 wide character should be 0xEF. But ") << (int)s[0] << " was found.");
}

inline size_t charIndex(char* c, slib::String* s) { return const_cast<const char*>(c) - s->cstr(); }
slib::Char::Char() : _ptr(nullptr), _base(nullptr) {}
slib::Char::Char(const char* c, const String* s) : _ptr(const_cast<char *>(c)), _base(const_cast<slib::String *>(s)) {}
slib::Char::Char(const Char& c) { _ptr = c._ptr; _base = c._base; }
slib::Char::~Char() {}
slib::Char& slib::Char::operator=(const char* s) {
    if (_ptr && _base) _base->replace(charIndex(_ptr, _base), size(), s);
    else throw NullException(slib::nullErrorText("char pointer"));
    return *this;
}
slib::Char& slib::Char::operator=(const Char& c) {
    if (_ptr && _base) _base->replace(charIndex(_ptr, _base), size(), c.toString());
    else { _ptr = c._ptr; _base = c._base; }
    return *this;
}
slib::Char& slib::Char::operator++() {
    if (!_ptr) throw NullException(nullErrorText("char pointer"));
    else if (!_base) throw NullException(nullErrorText("string object"));
    else { 
        _ptr += sutf8::size(_ptr); 
    }
    return *this;
}
slib::Char& slib::Char::operator--() {
    if (!_ptr) throw NullException(nullErrorText("char pointer"));
    else if (!_base) throw NullException(nullErrorText("string object"));
    --_ptr;
    while (_base->cstr() <= _ptr && !sutf8::check(_ptr)) { --_ptr; }
    return *this;
}
size_t slib::Char::size() const { return _ptr ? sutf8::size(_ptr) : 0; }
size_t slib::Char::index() const {
    if (!_ptr) throw NullException(nullErrorText("char pointer"));
    else if (!_base) throw NullException(nullErrorText("string object"));
    if (_ptr < _base->cstr() || (_base->cstr() + _base->size()) <= _ptr)
        throw RangeException(outRangeErrorText("char index", _ptr - _base->cstr(), 0, _base->size() - 1));
    size_t count = 0, len = _base->length();
    const char* p = _base->cstr();
    while (count < len && p != _ptr) {
        p += sutf8::size(p); ++count;
    }
    return count;
}
bool slib::Char::isUTF8() const { return _ptr ? sutf8::check(_ptr) : false; }
const char* slib::Char::cstr() const { return _ptr; }
slib::String slib::Char::toString() const { 
    String str(size(), '\0');
    Memory<char>::copy(&str[0], cstr(), size());
    return str;
}
bool slib::Char::operator<(const char* s) const {
    if (_ptr) {
        if (s) return !memcmp(cstr(), s, (size() < strlen(s) ? size() : strlen(s)));
        else return false;
    }
    else return true;
}
bool slib::Char::operator<(const slib::Char& c) const {
    if (_base != c._base) return _base < c._base;
    return _ptr < c._ptr;
}
bool slib::Char::operator==(const char* s) const {
    if (_ptr && s && size() == strlen(s)) return  !memcmp(cstr(), s, size());
    else return false;
}
bool slib::Char::operator==(const slib::Char& c) const { return _ptr == c._ptr && _base == c._base; }
slib::String slib::toString(const Char& ch, const char* format) {
    if (!format) {
        slib::String str(ch.size(), '\0');
        Memory<char>::copy(&str[0], ch.cstr(), ch.size());
        return str;
    }
    else {
        // Transformation of char code (not available now)
        throw FormatException(formatErrorText("char code", format, "utf-8"));
    }
}
