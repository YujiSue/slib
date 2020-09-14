#include "sbasic/sutf8.h"
#include "sbasic/string.h"

using namespace slib;

SUtf8Iterator::SUtf8Iterator() {}
SUtf8Iterator::SUtf8Iterator(String *s, const char *c) : _char(s, c) {}
SUtf8Iterator::SUtf8Iterator(const SUtf8Iterator &uit) : _char(uit._char) {}
SUtf8Iterator::~SUtf8Iterator() {}

SUtf8Iterator &SUtf8Iterator::operator=(const SUtf8Iterator &uit) { _char = uit._char; return *this; }
Char &SUtf8Iterator::operator *() { return _char; }
Char *SUtf8Iterator::operator ->() { return &_char; }
Char &SUtf8Iterator::operator[](std::ptrdiff_t diff) {
    sforin(i, 0, diff) ++_char; return _char;
}
SUtf8Iterator &SUtf8Iterator::operator++() { ++_char; return *this; }
SUtf8Iterator SUtf8Iterator::operator++(int) const { return ++SUtf8Iterator(*this); }
SUtf8Iterator &SUtf8Iterator::operator --() { --_char; return *this; }
SUtf8Iterator SUtf8Iterator::operator --(int) const { return --SUtf8Iterator(*this); }
SUtf8Iterator &SUtf8Iterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
SUtf8Iterator &SUtf8Iterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --(*this); return *this; }
SUtf8Iterator SUtf8Iterator::operator +(std::ptrdiff_t diff) const { auto it_ = *this; sforin(i, 0, diff) ++it_; return it_; }
SUtf8Iterator SUtf8Iterator::operator -(std::ptrdiff_t diff) const { auto it_ = *this; sforin(i, 0, diff) --it_; return it_; }
sinteger SUtf8Iterator::operator-(SUtf8Iterator it) const { return (int)(_char.cstr()-it._char.cstr()); }
void SUtf8Iterator::swap(SUtf8Iterator uit1, SUtf8Iterator uit2) {
    auto tmp = uit1._char; uit1._char = uit2._char; uit2._char = tmp;
}
bool SUtf8Iterator::operator ==(const SUtf8Iterator &uit) const { return _char == uit._char; }
bool SUtf8Iterator::operator !=(const SUtf8Iterator &uit) const { return !(_char == uit._char); }
bool SUtf8Iterator::operator <(const SUtf8Iterator &uit) const { return _char < uit._char; }
bool SUtf8Iterator::operator <=(const SUtf8Iterator &uit) const { return _char < uit._char || _char == uit._char; }
bool SUtf8Iterator::operator >(const SUtf8Iterator &uit) const { return uit._char < _char; }
bool SUtf8Iterator::operator >=(const SUtf8Iterator &uit) const { return uit <= *this; }

SUtf8CIterator::SUtf8CIterator() {}
SUtf8CIterator::SUtf8CIterator(const String *s, const char *c) : _char(const_cast<String *>(s), c) {}
SUtf8CIterator::SUtf8CIterator(const SUtf8CIterator &uit) : _char(uit._char) {}
SUtf8CIterator::~SUtf8CIterator() {}

SUtf8CIterator &SUtf8CIterator::operator=(const SUtf8CIterator &uit) { _char = uit._char; return *this; }
const Char &SUtf8CIterator::operator *() const { return _char; }
const Char *SUtf8CIterator::operator ->() const { return &_char; }
const Char &SUtf8CIterator::operator[](std::ptrdiff_t diff) { sforin(i, 0, diff) ++_char; return _char; }
SUtf8CIterator &SUtf8CIterator::operator ++() { ++_char; return *this; }
SUtf8CIterator SUtf8CIterator::operator ++(int) const { return ++SUtf8CIterator(*this); }
SUtf8CIterator &SUtf8CIterator::operator --() { --_char; return *this; }
SUtf8CIterator SUtf8CIterator::operator --(int) const { return --SUtf8CIterator(*this); }
SUtf8CIterator &SUtf8CIterator::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
SUtf8CIterator &SUtf8CIterator::operator -=(std::ptrdiff_t diff) { sforin(i, 0, diff) --(*this); return *this; }
SUtf8CIterator SUtf8CIterator::operator +(std::ptrdiff_t diff) const {
    auto it_ = *this; sforin(i, 0, diff) ++it_; return it_;
}
SUtf8CIterator SUtf8CIterator::operator -(std::ptrdiff_t diff) const {
    auto it_ = *this; sforin(i, 0, diff) --it_; return it_;
}
sinteger SUtf8CIterator::operator-(SUtf8CIterator it) const { return _char.cstr()-it._char.cstr(); }
void SUtf8CIterator::swap(SUtf8CIterator uit1, SUtf8CIterator uit2) {
    auto tmp = uit1._char; uit1._char = uit2._char; uit2._char = tmp;
}
bool SUtf8CIterator::operator ==(const SUtf8CIterator &uit) const { return _char == uit._char; }
bool SUtf8CIterator::operator !=(const SUtf8CIterator &uit) const { return !(_char == uit._char); }
bool SUtf8CIterator::operator <(const SUtf8CIterator &uit) const { return _char < uit._char; }
bool SUtf8CIterator::operator <=(const SUtf8CIterator &uit) const {
    return _char < uit._char || _char == uit._char;
}
bool SUtf8CIterator::operator >(const SUtf8CIterator &uit) const { return uit._char < _char; }
bool SUtf8CIterator::operator >=(const SUtf8CIterator &uit) const { return uit <= *this; }