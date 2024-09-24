#include "sbasic/u8iter.h"
#include "sbasic/string.h"
slib::Utf8Iterator::Utf8Iterator() {}
slib::Utf8Iterator::Utf8Iterator(char* s, String& b) : _char(s, &b) {}
slib::Utf8Iterator::Utf8Iterator(const Utf8Iterator &uit) : _char(uit._char) {}
slib::Utf8Iterator::~Utf8Iterator() {}
slib::Utf8Iterator& slib::Utf8Iterator::operator=(const Utf8Iterator& uit) { _char = uit._char; return *this; }
slib::Char slib::Utf8Iterator::operator *() { return _char; }
char * slib::Utf8Iterator::operator ->() { return _char._ptr; }
slib::Char slib::Utf8Iterator::operator[](std::ptrdiff_t diff) {
    auto ptr = _char._ptr;
    sforin(i, 0, diff) ptr += sutf8::size(ptr);
    return Char(ptr, _char._base);
}
slib::Utf8Iterator & slib::Utf8Iterator::operator++() { _char._ptr += sutf8::size(_char._ptr); return *this; }
slib::Utf8Iterator slib::Utf8Iterator::operator++(int) const { return ++Utf8Iterator(*this); }
slib::Utf8Iterator & slib::Utf8Iterator::operator --() { do { --_char._ptr; } while (sutf8::size(_char._ptr) < -1); return *this; }
slib::Utf8Iterator slib::Utf8Iterator::operator --(int) const { return --Utf8Iterator(*this); }
slib::Utf8Iterator & slib::Utf8Iterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
slib::Utf8Iterator & slib::Utf8Iterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --(*this); return *this; }
slib::Utf8Iterator slib::Utf8Iterator::operator +(std::ptrdiff_t diff) const { auto it_ = *this; sforin(i, 0, diff) ++it_; return it_; }
slib::Utf8Iterator slib::Utf8Iterator::operator -(std::ptrdiff_t diff) const { auto it_ = *this; sforin(i, 0, diff) --it_; return it_; }
std::ptrdiff_t slib::Utf8Iterator::operator-(Utf8Iterator it) const { return _char._ptr - it._char._ptr; }
char* slib::Utf8Iterator::ptr() { return _char._ptr; }
slib::String& slib::Utf8Iterator::str() { return *_char._base; }
size_t slib::Utf8Iterator::index() const { return _char._ptr - _char._base->cstr(); }
size_t slib::Utf8Iterator::length() const { return sutf8::size(_char._ptr); }
void slib::Utf8Iterator::swap(Utf8Iterator uit1, Utf8Iterator uit2) {
    auto tmp = uit1._char; uit1._char = uit2._char; uit2._char = tmp;
}
bool slib::Utf8Iterator::operator ==(const Utf8Iterator &uit) const { return _char == uit._char; }
bool slib::Utf8Iterator::operator !=(const Utf8Iterator &uit) const { return !(_char == uit._char); }
bool slib::Utf8Iterator::operator <(const Utf8Iterator &uit) const { return _char < uit._char; }
bool slib::Utf8Iterator::operator <=(const Utf8Iterator &uit) const { return _char < uit._char || _char == uit._char; }
bool slib::Utf8Iterator::operator >(const Utf8Iterator &uit) const { return uit._char < _char; }
bool slib::Utf8Iterator::operator >=(const Utf8Iterator &uit) const { return uit <= *this; }

slib::Utf8CIterator::Utf8CIterator() {}
slib::Utf8CIterator::Utf8CIterator(const char* s, const String& b) : _char(s, &b) {}
slib::Utf8CIterator::Utf8CIterator(const Utf8CIterator &uit) : _char(uit._char) {}
slib::Utf8CIterator::~Utf8CIterator() {}

slib::Utf8CIterator & slib::Utf8CIterator::operator=(const Utf8CIterator &uit) { _char = uit._char; return *this; }
const slib::Char slib::Utf8CIterator::operator *() const { return _char; }
const char * slib::Utf8CIterator::operator ->() const { return _char._ptr; }
const slib::Char slib::Utf8CIterator::operator[](std::ptrdiff_t diff) { 
    auto ptr = _char._ptr;
    sforin(i, 0, diff) ptr += sutf8::size(ptr);
    return Char(ptr, _char._base);
}
slib::Utf8CIterator & slib::Utf8CIterator::operator ++() { _char._ptr += sutf8::size(_char._ptr); return *this; }
slib::Utf8CIterator slib::Utf8CIterator::operator ++(int) const { return ++Utf8CIterator(*this); }
slib::Utf8CIterator & slib::Utf8CIterator::operator --() { do { --_char._ptr; } while (sutf8::size(_char._ptr) < -1); return *this; }
slib::Utf8CIterator slib::Utf8CIterator::operator --(int) const { return --Utf8CIterator(*this); }
slib::Utf8CIterator & slib::Utf8CIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
slib::Utf8CIterator & slib::Utf8CIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --(*this); return *this; }
slib::Utf8CIterator slib::Utf8CIterator::operator +(std::ptrdiff_t diff) const {
    auto it_ = *this; sforin(i, 0, diff) ++it_; return it_;
}
slib::Utf8CIterator slib::Utf8CIterator::operator -(std::ptrdiff_t diff) const {
    auto it_ = *this; sforin(i, 0, diff) --it_; return it_;
}
std::ptrdiff_t slib::Utf8CIterator::operator-(slib::Utf8CIterator it) const { return _char._ptr - it._char._ptr; }
const char* slib::Utf8CIterator::ptr() { return _char._ptr; }
const slib::String& slib::Utf8CIterator::str() { return *_char._base; }
size_t slib::Utf8CIterator::index() const { return _char._ptr - _char._base->cstr(); }
size_t slib::Utf8CIterator::length() const { return sutf8::size(_char._ptr); }
void slib::Utf8CIterator::swap(slib::Utf8CIterator uit1, slib::Utf8CIterator uit2) {
    auto tmp = uit1._char; uit1._char = uit2._char; uit2._char = tmp;
}
bool slib::Utf8CIterator::operator ==(const slib::Utf8CIterator &uit) const { return _char == uit._char; }
bool slib::Utf8CIterator::operator !=(const slib::Utf8CIterator &uit) const { return !(_char == uit._char); }
bool slib::Utf8CIterator::operator <(const slib::Utf8CIterator &uit) const { return _char < uit._char; }
bool slib::Utf8CIterator::operator <=(const slib::Utf8CIterator &uit) const {
    return _char < uit._char || _char == uit._char;
}
bool slib::Utf8CIterator::operator >(const Utf8CIterator &uit) const { return uit._char < _char; }
bool slib::Utf8CIterator::operator >=(const slib::Utf8CIterator &uit) const { return uit <= *this; }