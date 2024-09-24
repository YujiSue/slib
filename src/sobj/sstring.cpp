#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sobjptr.h"

slib::SString::SString() : SObject(), slib::String() {}
slib::SString::SString(size_t sz, const char c) : slib::String() { resize(sz, c); }
slib::SString::SString(std::initializer_list<char> li) : slib::String() {
	resize(li.size()); sfor2(*this, li) { $_1 = $_2; }
}
slib::SString::SString(const char* s) : SObject(), String(s) {}
slib::SString::SString(String&& s) noexcept : SObject(), String(std::forward<String &&>(s)) {}
slib::SString::SString(const String& s) : SObject(), String(s) {}
slib::SString::SString(SString&& s) noexcept : SObject(), String(std::forward<String&&>(s)) {}
slib::SString::SString(const SString& s) : SObject(), String(s) {}
slib::SString::~SString() {}
//slib::SString& slib::SString::operator=(const String& s) {}
slib::SString& slib::SString::operator=(SString&& s) noexcept {
	String::swap(s); return *this;
}
slib::SString& slib::SString::operator=(const SString& s) {
	String::clear();
	String::append(s);
	return *this;
}

slib::SString slib::SString::operator+(const char* s) const {
	slib::SString str(*this);
	return str += s;
}
slib::SString slib::SString::operator+(const String& s) const {
	slib::SString str(*this);
	return str += s;
}
slib::SString slib::SString::operator+(const SString& s) const {
	slib::SString str(*this);
	return str += s;
}

slib::SString& slib::SString::operator+=(const char * s) {
	String::append(s);
	return *this;
}
slib::SString& slib::SString::operator+=(const String& s) {
	String::append(s);
	return *this;
}
slib::SString& slib::SString::operator+=(const SString& s) {
	String::append(s);
	return *this;
}
slib::SString& slib::SString::operator<<(const char* s) {
	String::append(s);
	return *this;
}
slib::SString& slib::SString::operator<<(const String& s) {
	String::append(s);
	return *this;
}
slib::SString& slib::SString::operator<<(const SString& s) {
	String::append(s);
	return *this;
}

slib::SString& slib::SString::trim() {
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
slib::SString& slib::SString::clip(const size_t off, const size_t len) {
	size_t sz = (len == (size_t)-1) ? size() - off : len;
	if (off) {
		if (size() <= off) throw RangeException("");
		remove(0, off);
	}
	resize(sz);
	return *this;
}
slib::SString& slib::SString::clip(const srange& range) {
	return clip(iterAt(range.begin) - begin(), iterAt(range.end) - iterAt(range.begin));
}
slib::SString& slib::SString::replace(const size_t off, const size_t len, const char* wrd) {
	if (len == (size_t)-1) {
		resize(off); append(wrd); return *this;
	}
	auto sz = wrd ? strlen(wrd) : 0;
	slib::SString str(size() - len + sz, '\0');
	Memory<char>::copy(&str[0], _begin(), off);
	if (wrd) Memory<char>::copy(&str[0] + off, wrd, sz);
	Memory<char>::copy(&str[0] + off + sz, _begin() + off + len, size() - off - len);
	swap(str);
	return *this;
}
slib::SString& slib::SString::replace(const char* ori, const char* alt) {
	if (empty()) return *this;
	if (ori) {
		slib::SString str;
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
slib::SString& slib::SString::replace(const Regex& reg, const char* alt) {
	if (empty()) return *this;
	slib::SString str;
	auto set = reg.search(cstr());
	size_t current = 0;
	if (set.begin == set.end) return *this;
	else {
		auto it = set.begin;
		while (it != set.end) {
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


void slib::SString::swap(slib::SString& str) {
	String::swap(str);
	SObject::swapScope(str);
}

slib::SString::operator const char* () const { return String::cstr(); }
//slib::SString::operator String& () { return *this; }
//slib::SString::operator const String& () const { return *this; }

/*
SChar::SChar() : Char(), SObject() {}
SChar::SChar(String* s, const char* c) : Char(s, c), SObject() {}
SChar::SChar(const Char& c) : Char(c), SObject() {}
SChar::SChar(const SChar& c) : Char(c._str, c._ptr), SObject() {}
SChar::~SChar() {}
SChar& SChar::operator=(const char* s) {
	if (_str) {
		if (s) _str->replace(index(), u8size(_ptr), s);
		else _str->remove(index(), u8size(_ptr));
	}
	return *this;
}
SChar& SChar::operator=(const String& s) {
	if (_str) {
		if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
		else _str->remove(index(), u8size(_ptr));
	}
	return *this;
}
SChar& SChar::operator=(const SString& s) {
	if (_str) {
		if (s.length()) _str->replace(index(), u8size(_ptr), s.cstr());
		else _str->remove(index(), u8size(_ptr));
	}
	return *this;
}
SChar& SChar::operator=(const Char& c) {
	*this = SChar(c);
	return *this;
}
SChar& SChar::operator=(const SChar& c) {
	_str = c._str; _ptr = c._ptr;
	return *this;
}
String SChar::getClass() const { return "char"; }
String SChar::toString() const { return Char::toString(); }
SObject* SChar::clone() const { return new SChar(*this); }

SString::SString(bool b) : SObject(), String(b ? "true" : "false"), _char(SChar(this, cstr())) {}
SString::SString(int i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
SString::SString(unsigned int ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
SString::SString(size_t ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
#ifdef WIN64_OS
SString::SString(long i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
#ifndef MAC_OS
SString::SString(unsigned long ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
#endif
#endif
SString::SString(long long i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
#ifdef MAC_OS
SString::SString(unsigned long long ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
#endif
#ifdef LINUX_OS
SString::SString(sinteger i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
#endif
SString::SString(float f) : SObject(), String(std::to_string(f)), _char(SChar(this, cstr())) {}
SString::SString(double d) : SObject(), String(std::to_string(d)), _char(SChar(this, cstr())) {}
SString::SString(sbyte i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
SString::SString(subyte ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
SString::SString(sshort i) : SObject(), String(std::to_string(i)), _char(SChar(this, cstr())) {}
SString::SString(sushort ui) : SObject(), String(std::to_string(ui)), _char(SChar(this, cstr())) {}
SString::SString(const size_t& s, char c) : SObject(), String(s, c), _char(SChar(this, cstr())) {}
SString::SString(char c) : SObject(), String(c), _char(SChar(this, cstr())) {}
SString::SString(const SChar& c) : SObject(), String(c.toStr()), _char(SChar(this, cstr())) {}
SString::SString(const std::string& s) : SObject(), String(s), _char(SChar(this, cstr())) {}
SString::SString(std::initializer_list<char> li) : SObject(), String(li.size(), 0x00) { sforeach2(*this, li) E1_ = E2_; }
SString::SString(const sobj& obj) : SString() {
	if (obj.isStr()) *this = obj.string();
	else *this = obj.toString();
}
SString::SString(const SNumber& sn) : SString(sn.toString()) {}
SString::SString(SString&& s) : SString() { s.swap(*this); }

SString& SString::operator=(bool b) { *this = (b ? "true" : "false"); return *this; }
SString& SString::operator=(int i) { *this = std::to_string(i); return *this; }
SString& SString::operator=(unsigned int ui) { *this = std::to_string(ui); return *this; }
SString& SString::operator=(size_t ui) { *this = std::to_string(ui); return *this; }
#ifdef WIN64_OS
SString& SString::operator=(long i) { *this = std::to_string(i); return *this; }
#ifndef MAC_OS
SString& SString::operator=(unsigned long ui) { *this = std::to_string(ui); return *this; }
#endif
#endif
SString& SString::operator=(long long i) { *this = std::to_string(i); return *this; }
#ifdef MAC_OS
SString& SString::operator=(unsigned long long ui) { *this = std::to_string(ui); return *this; }
#endif
#ifdef LINUX_OS
SString& SString::operator=(sinteger i) { *this = std::to_string(i); return *this; }
#endif
SString& SString::operator=(float f) { *this = std::to_string(f); return *this; }
SString& SString::operator=(double d) { *this = std::to_string(d); return *this; }
SString& SString::operator=(sbyte i) { *this = std::to_string(i); return *this; }
SString& SString::operator=(subyte ui) { *this = std::to_string(ui); return *this; }
SString& SString::operator=(sshort i) { *this = std::to_string(i); return *this; }
SString& SString::operator=(sushort ui) { *this = std::to_string(ui); return *this; }
SString& SString::operator=(char c) { String::resize(1, c); return *this; }
SString& SString::operator=(const char* s) { String::copy(s); return *this; }
SString& SString::operator=(const std::string& s) {
	String::copy(s.c_str(), s.length()); return *this;
}
SString& SString::operator=(const SNumber& n) { *this = n.toString(); return *this; }
SString& SString::operator=(const sobj& obj) {
	if (obj.isStr()) *this = obj.string(); else *this = obj->toString(); return *this;
}
SString& SString::operator+=(bool b) { *this += (b ? "true" : "false"); return *this; }
SString& SString::operator+=(int i) { *this += std::to_string(i); return *this; }
SString& SString::operator+=(unsigned int ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator+=(size_t ui) { *this += std::to_string(ui); return *this; }
#ifdef WIN64_OS
SString& SString::operator+=(long i) { *this += std::to_string(i); return *this; }
#ifndef MAC_OS
SString& SString::operator+=(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
#endif
SString& SString::operator+=(long long i) { *this += std::to_string(i); return *this; }
#ifdef MAC_OS
SString& SString::operator+=(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
#ifdef LINUX_OS
SString& SString::operator+=(sinteger i) { *this += std::to_string(i); return *this; }
#endif
SString& SString::operator+=(float f) { *this += std::to_string(f); return *this; }
SString& SString::operator+=(double d) { *this += std::to_string(d); return *this; }
SString& SString::operator+=(sbyte i) { *this += std::to_string(i); return *this; }
SString& SString::operator+=(subyte ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator+=(sshort i) { *this += std::to_string(i); return *this; }
SString& SString::operator+=(sushort ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator+=(char c) { (*this) += std::string(1, c); return (*this); }
SString& SString::operator+=(const Char& c) { (*this) += c.toStr(); return (*this); }
SString& SString::operator+=(const SChar& c) { (*this) += c.toString(); return (*this); }
SString& SString::operator+=(const char* s) { this->append(s); return (*this); }
SString& SString::operator+=(const std::string& s) { this->append(s); return (*this); }
SString& SString::operator+=(const String& s) { this->append(s); return (*this); }
SString& SString::operator+=(const SString& s) { this->append(s); return (*this); }
SString& SString::operator+=(const SNumber& n) { (*this) += n.toString(); return *this; }
SString& SString::operator+=(const sobj& obj) {
	if (obj.isStr()) return *this += obj.string();
	else return  *this += obj->toString();
}
SString SString::operator+(bool b) const { return SString(*this) += (b ? "true" : "false"); }
SString SString::operator+(int i) const { return SString(*this) += std::to_string(i); }
SString SString::operator+(unsigned int ui) const { return SString(*this) += std::to_string(ui); }
SString SString::operator+(size_t ui) const { return SString(*this) += std::to_string(ui); }
#ifdef WIN64_OS
SString SString::operator+(long i) const { return SString(*this) += std::to_string(i); }
#ifndef MAC_OS
SString SString::operator+(unsigned long ui) const { return SString(*this) += std::to_string(ui); }
#endif
#endif
SString SString::operator+(long long i) const { return SString(*this) += std::to_string(i); }
#ifdef MAC_OS
SString SString::operator+(unsigned long long ui) const { return SString(*this) += std::to_string(ui); }
#endif
#ifdef LINUX_OS
SString SString::operator+(sinteger i) const { return SString(*this) += std::to_string(i); }
#endif
SString SString::operator+(float f) const { return SString(*this) += std::to_string(f); }
SString SString::operator+(double d) const { return SString(*this) += std::to_string(d); }
SString SString::operator+(sbyte i) const { return SString(*this) += std::to_string(i); }
SString SString::operator+(subyte ui) const { return SString(*this) += std::to_string(ui); }
SString SString::operator+(sshort i) const { return SString(*this) += std::to_string(i); }
SString SString::operator+(sushort ui) const { return SString(*this) += std::to_string(ui); }
SString SString::operator+(char c) const { return SString(*this) += std::string(1, c); }
SString SString::operator+(const Char& c) const { return SString(*this) += c.toStr(); }
SString SString::operator+(const SChar& c) const { return SString(*this) += c.toString(); }
SString SString::operator+(const char* s) const { return SString(*this) += std::string(s); }
SString SString::operator+(const std::string& s) const { return SString(*this) += s; }
SString SString::operator+(const String& s) const { return SString(*this) += s; }
SString SString::operator+(const SString& s) const { return SString(*this) += s; }
SString SString::operator+(const SNumber& n) const { return SString(*this) += n.toString(); }
SString SString::operator+(const sobj& obj) const {
	if (obj.isStr()) return SString(*this) += obj.string();
	else return SString(*this) += obj.toString();
}
SString& SString::operator<<(bool b) { *this += (b ? "true" : "false"); return *this; }
SString& SString::operator<<(int i) { *this += std::to_string(i); return *this; }
SString& SString::operator<<(unsigned int ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator<<(size_t ui) { *this += std::to_string(ui); return *this; }
#ifdef WIN64_OS
SString& SString::operator<<(long i) { *this += std::to_string(i); return *this; }
#ifndef MAC_OS
SString& SString::operator<<(unsigned long ui) { *this += std::to_string(ui); return *this; }
#endif
#endif
SString& SString::operator<<(long long i) { *this += std::to_string(i); return *this; }
#ifdef MAC_OS
SString& SString::operator<<(unsigned long long ui) { *this += std::to_string(ui); return *this; }
#endif
#ifdef LINUX_OS
SString& SString::operator<<(sinteger i) { *this += std::to_string(i); return *this; }
#endif
SString& SString::operator<<(float f) { *this += std::to_string(f); return *this; }
SString& SString::operator<<(double d) { *this += std::to_string(d); return *this; }
SString& SString::operator<<(sbyte i) { *this += std::to_string(i); return *this; }
SString& SString::operator<<(subyte ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator<<(sshort i) { *this += std::to_string(i); return *this; }
SString& SString::operator<<(sushort ui) { *this += std::to_string(ui); return *this; }
SString& SString::operator<<(char c) { return (*this) += std::string(1, c); }
SString& SString::operator<<(const Char& c) { return (*this) += c.toStr(); }
SString& SString::operator<<(const SChar& c) { return (*this) += c.toString(); }
SString& SString::operator<<(const char* s) { return (*this) += s;; }
SString& SString::operator<<(const std::string& s) { return (*this) += s; }
SString& SString::operator<<(const String& s) { return (*this) += s; }
SString& SString::operator<<(const SString& s) { return (*this) += s; }
SString& SString::operator<<(const SNumber& n) { return (*this) += n; }
SString& SString::operator<<(const sobj& obj) { return (*this) += obj; }
SString& SString::operator*=(const int& num) {
	if (!num) clear();
	else if (1 < num) {
		resize(size() * num);
		auto ins = _instance();
		auto p = &ins.first[ins.second];
		sforin(i, 1, num) { CMemory<char>::copy(p, ins.first, ins.second); p += ins.second; }
	}
	return *this;
}
SString& SString::operator*=(const size_t& num) {
	if (!num) clear();
	else if (1 < num) {
		resize(size() * num);
		auto ins = _instance();
		auto p = &ins.first[ins.second];
		sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p += ins.second; }
	}
	return *this;
}
SString SString::operator*(const int& num) const {
	SString str;
	if (num) {
		auto ins = _cinstance();
		str.resize(ins.second * num);
		auto p = str.ptr();
		sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p += ins.second; }
	}
	return str;
}
SString SString::operator*(const size_t& num) const {
	SString str;
	if (num) {
		auto ins = _cinstance();
		str.resize(ins.second * num);
		auto p = str.ptr();
		sforin(i, 0, num) { CMemory<char>::copy(p, ins.first, ins.second); p += ins.second; }
	}
	return str;
}
void SString::load(const char* path) {
	clear();
	sio::SFile file(path, sio::READ);
	file >> (*this);
}
void SString::save(const char* path) {
	sio::SFile file(path, sio::CREATE);
	file << (*this);
}
sobj& SString::charAt(int idx) {
	_char.character() = u8charAt(idx);
	return _char;
}
const sobj& SString::charAt(int idx) const {
	const_cast<SChar&>(_char.character()) = u8charAt(idx);
	return _char;
}
bool SString::operator<(const char* s) const { return strcmp(cstr(), s) < 0; }
bool SString::operator<(const std::string& s) const { return *this < s.c_str(); }
bool SString::operator<(const sobj& obj) const {
	if (obj.isStr()) return *this < obj.string();
	return *this < obj.toString();
}
bool SString::operator<(const String& s) const { return *static_cast<const String*>(this) < s; }
bool SString::operator<(const SString& s) const {
	return *static_cast<const String*>(this) < *static_cast<const String*>(&s);
}
bool SString::operator!=(const char* s) const { return strcmp(cstr(), s); }
bool SString::operator!=(const std::string& s) const { return *this != s.c_str(); }
bool SString::operator!=(const sobj& obj) const { return !((*this) == obj); }
bool SString::operator!=(const String& s) const { return !(*this == s); }
bool SString::operator!=(const SString& s) const { return !(*this == s); }
SString slib::operator+(const char& c, const SString& s) { return SString(c) += s; }
SString slib::operator+(const SChar& c, const SString& s) { return c.toString() += s; }
SString slib::operator+(const char* s1, const SString& s2) { return SString(s1) += s2; }
SString slib::operator+(const int& i, const SString& s) { return SString(i) += s; }
SString slib::operator+(const sinteger& i, const SString& s) { return SString(i) += s; }
SString slib::operator+(const size_t& u, const SString& s) { return SString(u) += s; }
SString slib::operator+(const float& f, const SString& s) { return SString(f) += s; }
SString slib::operator+(const double& r, const SString& s) { return SString(r) += s; }
SString slib::operator+(const bool& b, const SString& s) { return SString(b) += s; }
*/
bool slib::SString::operator==(const char* s) const { return !strcmp(cstr(), s); }
bool slib::SString::operator==(const std::string& s) const { return *this == s.c_str(); }
bool slib::SString::operator==(const slib::SObjPtr& obj) const {
	if (obj.isNull()) return false;
	else if (obj.isStr()) return (*this) == obj.string();
	return (*this) == obj.toString();
}
bool slib::SString::operator==(const slib::String& s) const { return *static_cast<const String*>(this) == s; }
bool slib::SString::operator==(const slib::SString& s) const {
	return *static_cast<const String*>(this) == *static_cast<const String*>(&s);
}

slib::String slib::SString::getClass() const { return "string"; }
slib::String slib::SString::toString(const char *format) const { 
	if (!format) return *this;
	else throw FormatException(formatErrorText("string code", format, ""));
}
slib::SObject* slib::SString::clone() const { return new SString(*this); }

slib::String slib::toString(const slib::SString& str, const char* format) { return str.toString(format); }
std::ostream& operator<<(std::ostream& os, const slib::SString& str) { return os << str.toString(); }