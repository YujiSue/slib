#include "sbasic/string.h"
#include "sbasic/array.h"
#include "sbasic/list.h"
#include "sbasic/map.h"
#include "sbasic/exception.h"

bool slib::sstr::isQuoted(const char* s) {
    if (s) {
        auto len = strlen(s);
        if (len < 2) return false;
        return (s[0] == s[len - 1]) && (s[0] == '"' || s[0] == '\'');
    }
    return false;
}
bool slib::sstr::isEnclosed(const char* s, const char* bracket) {
    if (s && bracket) {
        auto len = strlen(s), blen = strlen(bracket);
        if (len < blen) return false;
        blen /= 2;
        return !memcmp(s, bracket, blen) && !memcmp(&s[len - blen], &bracket[blen], blen);
    }
    return false;
}
bool slib::sstr::isNumeric(const char* s) {
    String str(sstr::toLower(sstr::trim(s)));
#if defined(__GNUC__) && (__GNUC__ < 5)
    if (str == "nan" || str == "inf" || str == "-inf" || 
        str == "null" || str == "true" || str == "false") return true;
    if (str[0] == '+' || str[0] == '-') str.clip(1);
    bool h = false;
    if (str[-1] == 'i') {
        str.resize(str.size() - 1);
        if (str.match("+")) {
            if (1 < str.count("+")) return false;
            auto vals = str.split("+");
            str = vals[0] + vals[1];
        }
        else if (str.match("-")) {
            if (1 < str.count("-")) return false;
            auto vals = str.split("-");
            str = vals[0] + vals[1];
        }
    }
    if (str.beginWith("0x")) { h = true;  str.clip(2); }
    if (str.beginWith("0b") || str.beginWith("0o")) str.clip(2);
    if (str.match("/")) {
        if (1 < str.count("/")) return false;
        auto vals = str.split("/");
        str = vals[0] + vals[1];
    } 
    if (str.match(".")) {
        if (1 < str.count(".")) return false;
        auto vals = str.split(".");
        str = vals[0] + vals[1];
    }
    sfor(str) {
        auto c = (int)$_;
        if (0x2f < c && c < 0x3a) continue;
        if (h && ((0x40 < c && c < 0x47) || (0x60 < c && c < 0x67))) continue;
        else return false;
    }
    return true;

#else
    return 
        str.equal(REG("/^[+-]*\\d+/")) || 
        str.equal(REG("/^0x[0-9a-fA-F]+/")) || 
        str.equal(REG("/^0o[0-7]+/")) || 
        str.equal(REG("/^0b[01]+/")) ||
        str.equal(REG("/^nan/i")) ||
        str.equal(REG("/^[+-]*inf/i")) ||
        str.equal(REG("/^true/i")) ||
        str.equal(REG("/^false/i")) ||
        str.equal(REG("/^[+-]*\\d+\\.\\d+/i")) ||
        str.equal(REG("/^[+-]*\\d+\\.*\\d*[eE][+-]*\\d+/"));
#endif
}
slib::String slib::sstr::trim(const char *s) {
    String str;
    if (s && s[0] != '\0') {
        auto init = s, end = s + strlen(s);
        while (init < end && sutf8::isWS(*init)) ++init;
        while (init < end && sutf8::isWS(*(end - 1))) --end;
        if (init < end) {
            str.resize(end - init);
            Memory<char>::copy(&str[0], init, str.size());
        }
    }
    return str;
}
slib::String slib::sstr::squote(const char *s) {
    String str;
    str<<'\''<<(s?s:"")<<'\'';
    return str;
}
slib::String slib::sstr::dquote(const char* s) {
    String str;
    str << '\"' << s << '\"';
    return str;
}
slib::String slib::sstr::dequote(const char* s, bool check) {
    String str(s);
    if (str.size() < 2) return str;
    bool apply = false;
    if (check) {
        auto& b = str[0], & e = str[-1];
        if ((b == '\'' && e == '\'') ||
            (b == '\"' && e == '\"') ||
            (b == '(' && e == ')') ||
            (b == '{' && e == '}') ||
            (b == '<' && e == '>') ||
            (b == '[' && e == ']')) apply = true;
    }
    else apply = true;
    if (!apply) return s;
    else return str.substring(srange(1, -1));
}
slib::String slib::sstr::toUpper(const char *s) {
    slib::String str(s);
    sfor(str) $_ = sutf8::toUpper($_);
    return str;
}
slib::String slib::sstr::toLower(const char *s) {
    slib::String str(s);
    sfor(str) $_ = sutf8::toLower($_);
    return str;
}
slib::String slib::sstr::enclose(const char *s, const char* bracket) {
    String str;
    if (!bracket) throw NullException(nullErrorText("Bracket characters"));
    int blen = (int)strlen(bracket);
    sforin(i, 0, blen / 2) str << bracket[i];
    str << (s ? s : "");
    sforin(i, blen / 2, blen) str << bracket[i];
    return str;
}
slib::String slib::sstr::toWide(const char *s) {
    String str;
    if (s) {
        int len = (int)strlen(s);
        sforin(i, 0, len) str << sutf8::toWide(s[i]);
    }
    return str;
}
slib::String slib::sstr::toNarrow(const char *s) {
    String str;
    if (s) {
        while (s[0] != '\0') {
            str << sutf8::toNarrow(s);
            s += sutf8::size(s);
        }
    }
    return str;
}
slib::String slib::sstr::fill(const char* s, const char c, const size_t sz, slib::DIRECTION dir) {
    auto len = s ? strlen(s) : 0;
    if (sz < len) return s;
    slib::String str;
    switch (dir) {
    case DIRECTION::HEAD:
        str << slib::String(sz - len, c) << s;
        break;
    case DIRECTION::TAIL:
        str << s << slib::String(sz - len, c);
        break;
    case DIRECTION::BI:
    {
        auto sz_ = (sz - len) / 2;
        str << slib::String(sz_, c) << s << slib::String(sz - len - sz_, c);
        break;
    }
    default:
        break;
    }
    return str;
}
slib::String slib::sstr::lfill(const char* s, const char c, const size_t sz) {
    slib::String str(sz, c);
    auto len = s ? strlen(s) : 0;
    Memory<char>::copy(&str[(int)(len < sz ? sz - len : 0)], s, len);
    return str;
}
slib::String slib::sstr::rfill(const char* s, const char c, const size_t sz) {
    slib::String str(sz, c);
    auto len = s ? strlen(s) : 0;
    Memory<char>::copy(&str[0], s, (len < sz ? len : sz));
    return str;
}
slib::String slib::sstr::bfill(const char* s, const char c, const size_t sz) {
    slib::String str(sz, c);
    auto len = s ? strlen(s) : 0;
    auto off = (len < sz ? sz - len : 0) / 2;
    Memory<char>::copy(&str[(int)off], s, (len < sz ? len : sz));
    return str;
}
/*
slib::String slib::SP = { ' ' };
slib::String slib::CR = { '\r' };
slib::String slib::LF = { '\n' };
slib::String slib::CRLF = { '\r', '\n' };
slib::String slib::TAB = { '\t' };
slib::String slib::DEL = { '\b' };
#ifdef WIN_OS
slib::String slib::NL =  { '\r', '\n' };
#else
slib::String slib::NL = { '\n' };
#endif
*/
#if defined(WIN_OS)
slib::String slib::String::toUTF8(const wchar_t* ws) {
	auto wsize = ::WideCharToMultiByte(CP_UTF8, 0U, ws, -1, nullptr, 0, nullptr, nullptr);
	String str(wsize, '\0');
	auto res = ::WideCharToMultiByte(CP_UTF8, 0U, ws, -1, &str[0], wsize, nullptr, nullptr);
	if (str[-1] == '\0') str.resize(str.size() - 1);
	return str;
}
slib::String slib::String::toUTF8(const char* s) {
	auto wsize = ::MultiByteToWideChar(CP_ACP, 0U, s, -1, nullptr, 0U);
	Array<wchar_t> u16s(wsize);
	auto res = ::MultiByteToWideChar(CP_ACP, 0U, s, -1, u16s.data(), wsize);
	auto u8size = ::WideCharToMultiByte(CP_UTF8, 0U, u16s.data(), -1, nullptr, 0, nullptr, nullptr);
	String str(u8size, '\0');
	res = ::WideCharToMultiByte(CP_UTF8, 0U, u16s.data(), -1, &str[0], u8size, nullptr, nullptr);
	if (str[-1] == '\0') str.resize(str.size() - 1);
	return str;
}
#endif

slib::String::short_string::short_string() { size = 0; memset(ptr, 0, SHORT_STRING_CAPACITY); }
slib::String::short_string::~short_string() { size = 0; memset(ptr, 0, SHORT_STRING_CAPACITY); }
void slib::String::short_string::copyTo(short_string& dest) {
    dest.size = size; 
    Memory<char>::copy(dest.ptr, ptr, SHORT_STRING_CAPACITY);
}
slib::String::long_string::long_string() { capacity = 1; size = 0; ptr = nullptr; }
slib::String::long_string::~long_string() {
    if (ptr) {
        Memory<char>::dealloc(ptr);
        capacity = 0; size = 0; ptr = nullptr;
    }
}

bool slib::String::_isLong() const { return (reinterpret_cast<const subyte *>(&_str))[0] & 0x01; }
char* slib::String::_begin() const { return const_cast<char *>(_isLong() ? _str.ls.ptr : &_str.ss.ptr[0]); }
char* slib::String::_end() const { return const_cast<char*>(_isLong() ? (_str.ls.ptr + _str.ls.size) : &_str.ss.ptr[_str.ss.size>>1]); }

slib::String::String() {}
slib::String::String(const bool n) : String() { append(n ? "true" : "false"); }
slib::String::String(const int n) : String() { append(std::to_string(n)); }
slib::String::String(const unsigned int n) : String() { append(std::to_string(n)); }
slib::String::String(const size_t n) : String() { append(std::to_string(n)); }
//slib::String::String(const long long n) : String() { append(std::to_string(n)); }
slib::String::String(const int64_t n) : String() { append(std::to_string(n)); }
#ifdef MAC_OS
//slib::String::String(const sinteger n) : String() { append(std::to_string(n)); }
slib::String::String(const suinteger n) : String() { append(std::to_string(n)); }
#endif
slib::String::String(const float n) : String() { append(std::to_string(n)); }
slib::String::String(const double n) : String() { append(std::to_string(n)); }
slib::String::String(const Char& c) : String() { append(toString(c)); }
slib::String::String(const char* s) : String() { append(s); }
slib::String::String(const std::string& s) : String() { append(s); }
#if defined(WIN32_OS) || defined(WIN64_OS)
//slib::String::String(const wchar_t* ws);
//slib::String::String(const std::wstring &ws);
#endif
slib::String::String(size_t sz, const char c) : String() { resize(sz, c); }
slib::String::String(std::initializer_list<char> li) : String() { 
    resize(li.size()); sfor2(*this, li) { $_1 = $_2; }
}
slib::String::String(String&& s) noexcept : String() { swap(s); }
slib::String::String(const String& s) : String() { append(s); }
slib::String::~String() {}
slib::String& slib::String::operator=(String&& s) noexcept { clear(); swap(s); return *this; }
slib::String& slib::String::operator=(const String& s) { clear(); append(s); return *this; }
slib::String& slib::String::operator=(const char* s) { clear(); append(s); return *this; }
slib::String& slib::String::operator+=(const String& s) { append(s); return *this; }
slib::String& slib::String::operator+=(const char* s) { append(s); return *this; }
slib::String& slib::String::operator<<(const char c) { add(c); return *this; }
slib::String& slib::String::operator<<(const int n) { append(String(n)); return *this; }
slib::String& slib::String::operator<<(const int64_t n) { append(String(n)); return *this; }
slib::String& slib::String::operator<<(const size_t n) { append(String(n)); return *this; }
slib::String& slib::String::operator<<(const float n) { append(String(n)); return *this; }
slib::String& slib::String::operator<<(const double n) { append(String(n)); return *this; }
slib::String& slib::String::operator<<(const Char& c) { append(c.toString()); return *this; }
slib::String& slib::String::operator<<(const String& s) { append(s); return *this; }
slib::String& slib::String::operator<<(const char* s) { append(s); return *this; }
slib::String& slib::String::operator*=(const int i) {
    if (i < 0) throw FormatException(String("Not negative number should be passed to multiply operator of String class. But '") << i << "' was received.");
    else if (!i) clear();
    else {
        slib::String tmp(*this);
        sforin(k, 0, i - 1) { append(tmp); }
    }
    return *this;
}
slib::String slib::String::operator+(const char c) const { String str(*this); str.add(c); return str; }
slib::String slib::String::operator+(const char* s) const { String str(*this); str.append(s); return str; }
slib::String slib::String::operator+(const String& s) const { String str(*this); str.append(s); return str; }
slib::String slib::String::operator*(const int i) const { slib::String str(*this); str *= i; return str; }
size_t slib::String::capacity() const { 
    return _isLong() ? _str.ls.capacity : SHORT_STRING_CAPACITY;
}
bool slib::String::empty() const { return size() == 0; }
size_t slib::String::size() const { return _isLong() ? _str.ls.size : (_str.ss.size >> 1); }
size_t slib::String::length() const {
    size_t len = 0; 
    sforc(*this) ++len;
    return len;
}
void slib::String::reserve(const size_t s) { 
    if (s < capacity()) return;
    if (MAX_ARRAY_CAPACITY < s) throw OverFlowException(String("Maximum String class capacity should be less than '") << MAX_ARRAY_CAPACITY << "'. But '" << s << "' was received.");
    if (_isLong()) {
        _str.ls.capacity = (s % 2 ? s : s + 1);
        auto tmp = Memory<char>::alloc(_str.ls.capacity);
        Memory<char>::copy(tmp, _str.ls.ptr, _str.ls.size);
        Memory<char*>::swap(&tmp, &_str.ls.ptr);
        Memory<char>::dealloc(tmp);
    }
    else {
        auto sz = size(), cap = (s % 2 ? s : s + 1);
        auto tmp = Memory<char>::alloc(cap);
        Memory<char>::copy(tmp, _str.ss.ptr, sz);
        _str.ls.capacity = cap;
        _str.ls.size = sz;
        _str.ls.ptr = tmp;
    }
}
void slib::String::resize(const size_t s) {
    auto cap = capacity();
    if (!cap) reserve(s + 1);
    else if (cap <= s + 1) {
        while (cap <= s) { cap <<= 1; }
        reserve(cap);
    }
    if (_isLong()) _str.ls.size = s;
    else _str.ss.size = (slib::sbyte)(s << 1);
    *_end() = '\0';
}
void slib::String::resize(const size_t s, const char c) {
    auto sz = size();
    if (s == sz) return;
    resize(s);
    memset(_begin() + sz, (int)c, s - sz);
}
void slib::String::swap(String& str) {
    if (_isLong() && str._isLong()) {
        Memory<size_t>::swap(&_str.ls.capacity, &str._str.ls.capacity);
        Memory<size_t>::swap(&_str.ls.size, &str._str.ls.size);
        Memory<char*>::swap(&_str.ls.ptr, &str._str.ls.ptr);
    }
    else if (_isLong()) {
        short_string ss;
        str._str.ss.copyTo(ss);
        str._str.ls.size = _str.ls.size;
        str._str.ls.capacity = _str.ls.capacity;
        str._str.ls.ptr = _str.ls.ptr;
        ss.copyTo(_str.ss);
    }
    else if (str._isLong()) {
        short_string ss;
        _str.ss.copyTo(ss);
        _str.ls.size = str._str.ls.size;
        _str.ls.capacity = str._str.ls.capacity;
        _str.ls.ptr = str._str.ls.ptr;
        ss.copyTo(str._str.ss);
    }
    else {
        short_string ss;
        _str.ss.copyTo(ss);
        str._str.ss.copyTo(_str.ss);
        ss.copyTo(str._str.ss);
    }
}
slib::ArrayIterator<char> slib::String::iterAt(const int i) { return (i < 0 ? _end() : _begin()) + i; }
slib::ArrayCIterator<char> slib::String::iterAt(const int i) const { return (i < 0 ? _end() : _begin()) + i; }
char& slib::String::at(const int i) { auto ptr = (i < 0 ? _end() : _begin()) + i; return *ptr; }
const char& slib::String::at(const int i) const { auto ptr = (i < 0 ? _end() : _begin()) + i; return *ptr; }
char& slib::String::operator[](const int i) { return at(i); }
const char& slib::String::operator[](const int i) const { return at(i); }
slib::Utf8Iterator slib::String::u8iterAt(const int i) { return (i < 0 ? u8end() : u8begin()) + i; }
slib::Utf8CIterator slib::String::u8iterAt(const int i) const { return (i < 0 ? u8end() : u8begin()) + i; }
slib::Char slib::String::charAt(const int i) const { auto it = (i < 0 ? u8end() : u8begin()) + i; return $_; }
slib::ArrayIterator<char> slib::String::begin() { return ArrayIterator<char>(_begin()); }
slib::ArrayCIterator<char> slib::String::begin() const { return ArrayCIterator<char>(const_cast<const char*>(_begin())); }
slib::ArrayIterator<char> slib::String::end() { return ArrayIterator<char>(_end()); }
slib::ArrayCIterator<char> slib::String::end() const { return ArrayCIterator<char>(const_cast<const char*>(_end())); }
slib::Utf8Iterator slib::String::u8begin() { return Utf8Iterator(_begin(), *this); }
slib::Utf8CIterator slib::String::u8begin() const { return Utf8CIterator(_begin(), *this); }
slib::Utf8Iterator slib::String::u8end() { return Utf8Iterator(_end(), *this); }
slib::Utf8CIterator slib::String::u8end() const { return Utf8CIterator(_end(), *this); }
void slib::String::add(const char c) { resize(size() + 1); at(-1) = c; }
void slib::String::append(const char* s) { 
    if (s) {
        auto sz = size(), len = strlen(s);
        resize(sz + len);
        if (len) Memory<char>::copy(_begin() + sz, s, len);
    }
}
void slib::String::append(const std::string& s) {
    auto sz = size(), sz_ = s.size();
    resize(sz + sz_);
    if (sz_) Memory<char>::copy(_begin() + sz, s.c_str(), sz_);
}
void slib::String::append(const String& s) { 
    auto sz = size(), sz_ = s.size();
    resize(sz + sz_);
    if (sz_) Memory<char>::copy(_begin() + sz, s.cstr(), sz_);
}
void slib::String::insert(const int i, const char* s) {
    if (!s) return;
    auto len = strlen(s);
    resize(size() + len);
    auto p = &at(i);
    Memory<char>::shift(p, p + len, _end() - p);
    Memory<char>::copy(p, s, len);
}
void slib::String::insert(const int i, const String& s) {
    auto sz = s.size();
    resize(size() + sz);
    auto p = &at(i);
    Memory<char>::shift(p, p + sz, _end() - p);
    Memory<char>::copy(p, s.cstr(), sz);
}
void slib::String::removeAt(const int i) {
    auto p = &at(i);
    Memory<char>::shift(p + 1, p, _end() - p - 1);
    resize(size() - 1);
}
void slib::String::remove(const size_t off, const size_t len) {
    auto sz = size();
    if (len == (size_t)-1 || sz < off + len) resize(off);
    else {
        Memory<char>::shift(_begin() + off + len, _begin() + off, sz - off - len);
        resize(sz - len);
    }
}
void slib::String::remove(const Range<sint>& range) {
    auto beg = &at(range.begin), end = &at(range.end);
    if (beg < end) {
        auto len = end - beg;
        Memory<char>::shift(end, beg, _end() - beg - len);
        resize(size() - len);
    }
    //else throw 
}
void slib::String::clear() { resize(0); }
slib::String& slib::String::trim() {
    if (empty()) return *this;
    srange range(0, (int)size());
    srfor(*this) {
        if (sutf8::isWS($_)) --range.end;
        else break;
    }
    sfor(*this) {
        if (sutf8::isWS($_)) ++range.begin;
        else break;
    }
    if (range.begin < range.end) return clip(range.begin, range.length());
    else {
        clear(); return *this;
    }
}
slib::String& slib::String::clip(const size_t off, const size_t len) {
    auto sz = len == (size_t)-1 ? size() - off : len;
    if (off) {
        if (size() <= off) throw RangeException(outRangeErrorText("clip offset", off, 0, size()));
        remove(0, off);
    }
    resize(sz);
    return *this;
}
slib::String& slib::String::clip(const srange& range) {
    return clip(iterAt(range.begin) - begin(), iterAt(range.end) - iterAt(range.begin));
}
bool slib::String::beginWith(const char* que) const {
    if (que) {
        auto len = strlen(que);
        return !memcmp(_begin(), que, len);
    }
    return false;
}
bool slib::String::endWith(const char* que) const {
    if (que) {
        auto len = strlen(que);
        return !memcmp(_end() - len, que, len);
    }
    return false;
}
slib::String slib::String::substring(const size_t off, size_t length) const {
    auto sz = size();
    if (sz < off) throw RangeException(outRangeErrorText("substring offset", off, 0, sz));
    if (length == (size_t)-1 || sz < off + length) length = sz - off;
    slib::String str(length, '\0');
    if (length) Memory<char>::copy(&str[0], &at((int)off), length);
    return str;
}
slib::String slib::String::substring(const srange range) const {
    size_t off = range.begin < 0 ? size() - (size_t)std::abs(range.begin) : (size_t)range.begin,
        length = (range.end < 0 ? size() - (size_t)std::abs(range.end) : (size_t)range.end ) - off;
    return substring(off, length);
}

slib::String &slib::String::replace(const size_t off, const size_t len, const char* wrd) {
    if (len == (size_t)-1) {
        resize(off); append(wrd); return *this;
    }
    auto sz = wrd ? strlen(wrd) : 0;
    slib::String str(size() - len + sz, '\0');
    Memory<char>::copy(&str[0], _begin(), off);
    if(wrd) Memory<char>::copy(&str[0] + off, wrd, sz);
    Memory<char>::copy(&str[0] + off + sz, _begin() + off + len, size() - off - len);
    swap(str);
    return *this;
}
slib::String &slib::String::replace(const char* ori, const char* alt) {
    if (empty()) return *this;
    if (ori) {
        slib::String str;
        auto len = strlen(ori);
        auto off = (size_t)0, pos = find(ori, off);
        while (pos != NOT_FOUND) {
            str << substring(off, pos - off) << (alt ? alt : "");
            off = pos + len, pos = find(ori, off);
        }
        str << substring(off, pos);
        swap(str);
    }
    else *this = alt;
    return *this;
}
slib::String &slib::String::replace(const Regex& reg, const char* alt) {
    if (empty()) return *this;
    slib::String str;
    auto set = reg.search(cstr());
    size_t current = 0;
    if (set.begin == set.end) return *this;
    else {
        auto it = set.begin;
        while(it != set.end) {
            if (current == (size_t)$->position()) str << alt;
            else str << substring(current, (size_t)$->position() - current) << alt;
            current = (size_t)($->position() + $->length());
            $NEXT;
        }
    }
    if (current != size()) str << substring(current);
    swap(str);
    return *this;
}
slib::Array<slib::String> slib::String::split(const char* sep, const bool trimming, const bool ignore_quot, const bool dequote) const {
    stringarray array;
    if (empty()) return array;
    if (sep) {
        auto len = strlen(sep);
        if (len) {
            bool dq = false;
            size_t off = 0;
            auto it = begin();
            while(it <= end() - len) {
                if ($_ == sep[0] && !memcmp($.ptr(), sep, len)) {
                    if (ignore_quot || (!ignore_quot && !dq)) {
                        auto pos = $INDEX(*this);
                        auto str = substring(off, pos - off);
                        if (trimming) str.trim();
                        if (dequote && sstr::isQuoted(str)) str.clip(1, str.size() - 2);
                        array.add(str);
                        off = pos + len;
                    }
                    it += len;
                }
                else {
                    if ($_ == '\"') dq = !dq;
                    $NEXT;
                }
            }
            if (off == size()) array.add("");
            else {
                auto str = substring(off, -1);
                if (trimming) str.trim();
                if (dequote && sstr::isQuoted(str)) str.clip(1, str.size() - 2);
                array.add(str);
            }
        }
    }
    else array.add(*this);
    return array;
}
slib::Array<slib::String> slib::String::split(const Regex& reg) const {
    slib::Array<slib::String> array;
    if (empty()) return array;
    auto set = reg.search(cstr());
    size_t current = 0;
    if (set.begin == set.end) array.add(*this);
    else {
        auto it = set.begin;
        while (it != set.end) {
            if (current == (size_t)$->position()) array.add("");
            else array.add(substring(current, (size_t)$->position() - current));
            current = (size_t)($->position() + $->length());
            $NEXT;
        }
    }
    if (current == size()) array.add("");
    else array.add(substring(current));
    return array;
}
slib::Array<slib::String> slib::String::splitLine(const bool trimming, const bool ignore_quot) const {
    stringarray array;
    if (empty()) return array;
    bool dq = false;
    size_t off = 0;
    auto it = begin();
    while (it < end()) {
        if ($_ == '\n' || $_ == '\r') {
            if (ignore_quot || (!ignore_quot && !dq)) {
                auto pos = $INDEX(*this);
                auto str = substring(off, pos - off);
                if (trimming) str.trim();
                array.add(str);
                off = pos;
                if (it < end() - 1 && $_ == '\r' && $_NEXT == '\n') {
                    off += 2; $NEXT;
                }
                else off += 1;
            }
            $NEXT;
        }
        else {
            if ($_ == '\"') dq = !dq;
            $NEXT;
        }
    }
    if (off < size()) {
        auto str = substring(off, -1);
        if (trimming) str.trim();
        array.add(str);
    }
    return array;
}
slib::Map<slib::String, slib::String> slib::String::parse(const char* sep, const char* delim, const bool trimming, const bool ignore_quot, bool dequote) const {
    Map<String, String> attr;
    String key;
    stringarray array = split(sep, trimming, ignore_quot);
    sfor(array) {
        if ($_.empty()) continue;
        auto strs = $_.split(delim, trimming, ignore_quot, dequote);
        if (strs.size() < 2) throw FormatException(String("To parse string as map, each element shoud be <KEY>") << delim << "<VALUE>" << sep << "... format. But '" << $_ << "' was received.");
        attr[strs[0]] = strs[1];
    }
    return attr;
}

size_t slib::String::count(const char* que) const {
    size_t count = 0, off = 0;
    while (true) {
        off = find(que, off);
        if (off != NOT_FOUND) { ++count; ++off; }
        else break;
    }
    return count;
}
inline const char* _find(const char *ref, const char* end, const char* que) {
    if (!que || que[0] == '\0') return nullptr;
    size_t shift;
    auto current = ref;
    while (current < end) {
        shift = 1;
        if (*current == *que) {
            auto r = current + 1, q = que + 1;
            while (r < end && *q != '\0' && *r == *q) { ++shift; ++r; ++q; }
            if (*q == '\0') return current;
        }
        current += shift;
    }
    return nullptr;
}
inline const char* _rfind(const char* ref, const char* beg, const char* que) {
    if (!que || que[0] == '\0') return nullptr;
    size_t shift, len = strlen(que);
    auto current = ref, que_ = que + len - 1;
    while (current >= beg) {
        shift = 1;
        if (*current == *que_) {
            auto r = current - 1, q = que_ - 1;
            while (r >= beg && q >= que && *r == *q) { ++shift; --r; --q; }
            if (q == que - 1) return current - len + 1;
        }
        current -= shift;
    }
    return nullptr;
}
size_t slib::String::find(const char* que, const size_t offset) const { 
    auto ptr = _find(_begin() + offset, _end(), que);
    if (ptr) return ptr - _begin();
    else return NOT_FOUND;
}
size_t slib::String::find(const Regex& reg, const size_t offset) const {
    return reg.find(cstr(), offset);
}
size_t slib::String::rfind(const char* que, const size_t offset) const {
    auto ptr = _rfind(_end() - offset - 1, _begin(), que);
    if (ptr) return ptr - _begin();
    else return NOT_FOUND;
}
size_t slib::String::rfind(const Regex& reg, const size_t offset) const {
    return reg.find(cstr(), offset, true);
}
sizearray slib::String::findAll(const char* que) const {
    size_t off = 0;
    sizearray array;
    while (true) {
        off = find(que, off);
        if (off != NOT_FOUND) { array.add(off); ++off; }
        else break;
    }
    return array;
}
sizearray slib::String::findAll(const Regex& reg) const {
    return reg.findAll(cstr());
}
bool slib::String::match(const char* que, const size_t offset) const { return find(que, offset) != NOT_FOUND; }
bool slib::String::match(const Regex& reg) const {
    return reg.match(cstr());
}
bool slib::String::equal(const Regex& reg) const {
    return reg.equal(cstr());
}
slib::Pair<size_t, slib::String> slib::String::search(const Regex& reg) const {
    auto set = reg.search(cstr());
    if (set.begin == set.end) return Pair<size_t, slib::String>(NOT_FOUND, "");
    else return Pair<size_t, slib::String>(set.begin->position(), set.begin->str());
}
slib::Array<slib::Pair<size_t, slib::String>> slib::String::searchAll(const Regex& reg) const {
    auto set = reg.search(cstr());
    slib::Array<slib::Pair<size_t, slib::String>> res;
    if (set.begin == set.end) return res;
    else {
        auto it = set.begin;
        while (it != set.end) {
            res.add(Pair<size_t, slib::String>($->position(), $->str()));
            $NEXT;
        }
    }
    return res;
}
bool slib::String::boolean() const { auto s = sstr::toLower(cstr()); return s == "true" || s == "yes" || s == "ok"; }
slib::sbyte slib::String::byteValue() const { return (sbyte)strtol(cstr(), NULL, 0); }
slib::subyte slib::String::ubyteValue() const { return (subyte)strtoul(cstr(), NULL, 0); }
slib::sshort slib::String::shortValue() const { return (sshort)strtol(cstr(), NULL, 0); }
slib::sushort slib::String::ushortValue() const { return (sushort)strtoul(cstr(), NULL, 0); }
int slib::String::intValue() const { return (int)strtol(cstr(), NULL, 0); }
unsigned int slib::String::uintValue() const { return (unsigned)strtoul(cstr(), NULL, 0); }
size_t slib::String::sizeValue() const { return strtoull(cstr(), NULL, 0); }
long slib::String::longValue() const { return strtol(cstr(), NULL, 0); }
unsigned long slib::String::ulongValue() const { return strtoul(cstr(), NULL, 0); }
long long slib::String::llongValue() const { return strtoll(cstr(), NULL, 0); }
unsigned long long slib::String::ullongValue() const { return strtoull(cstr(), NULL, 0); }
float slib::String::floatValue() const { return strtof(cstr(), NULL); }
double slib::String::doubleValue() const { return strtod(cstr(), NULL); }
slib::sinteger slib::String::integer() const { return strtoll(cstr(), NULL, 0); }
slib::suinteger slib::String::uinteger() const { return strtoull(cstr(), NULL, 0); }
slib::sreal slib::String::real() const { return strtold(cstr(), NULL); }
bool slib::String::isUtf8() const {
    sforin(it, begin(), end()) { 
        if (slib::sutf8::check(it.ptr())) it += slib::sutf8::size(it.ptr());
	else return false;
    }
    return true;
}
#if defined(WIN_OS)
std::wstring slib::String::unicode() const {
    auto wsize = ::MultiByteToWideChar(CP_UTF8, 0U, _begin(), -1, nullptr, 0U);
    std::wstring ws(wsize, '\0');
    auto res = ::MultiByteToWideChar(CP_UTF8, 0U, _begin(), -1, &ws[0], wsize);
    return ws;
}
slib::String slib::String::localize() const {
    auto wsize = ::MultiByteToWideChar(CP_UTF8, 0U, _begin(), -1, nullptr, 0U);
    slib::Array<wchar_t> ws(wsize);
    auto res = ::MultiByteToWideChar(CP_UTF8, 0U, _begin(), -1, ws.data(), wsize);
    auto sjissize = ::WideCharToMultiByte(CP_ACP, 0U, ws.data(), -1, nullptr, 0, nullptr, nullptr);
    slib::String str(sjissize, '\0');
    res = ::WideCharToMultiByte(CP_ACP, 0U, ws.data(), -1, str._begin(), sjissize, nullptr, nullptr);
    return str;
}
#endif
const char* slib::String::cstr() const { return _begin(); }
const std::string slib::String::toStr() const { return std::string(cstr()); }
slib::String::operator const char* () const { return cstr(); }

bool slib::String::operator<(const char* s) const { return strcmp(cstr(), s) < 0; }
bool slib::String::operator<(const String& s) const { return strcmp(cstr(), s.cstr()) < 0; }
bool slib::String::operator==(const char* s) const { return !strcmp(cstr(), s); }
bool slib::String::operator==(const String& s) const { return !strcmp(cstr(), s.cstr()); }
bool slib::String::operator!=(const char* s) const { return strcmp(cstr(), s); }
bool slib::String::operator!=(const String& s) const { return strcmp(cstr(), s.cstr()); }
slib::String slib::toString(const char c, const char* format) { 
    if (!format) return String(1, c);
    throw FormatException(formatErrorText("string code", format, ""));
}
slib::String slib::toString(const char* s, const char* format) { 
    if (!format) return String(s);
    throw FormatException(formatErrorText("string code", format, ""));
}
slib::String slib::toString(const slib::String& str, const char* format) { 
    if (!format) return str;
    throw FormatException(formatErrorText("string code", format, ""));
}
slib::String slib::operator+(const char* s, const slib::String& str) {
    slib::String str_(s); str_ << str; return str_;
}
std::ostream& slib::operator<<(std::ostream& os, const slib::Char& c) {
    return os << toString(c);
}
std::ostream& slib::operator<<(std::ostream& os, const slib::String& str) {
#ifdef WIN_OS
    os << str.localize().cstr(); 
#else
    os << str.cstr();
#endif
    return os;
}

std::istream& slib::operator>>(std::istream& is, slib::String& str) {
    std::string s;
    is >> s;
    if (s.size()) {
#if defined(WIN_OS)
        str = slib::String::toUTF8(s.c_str());
#else
        str = s;
#endif
    }
    return is;
}
