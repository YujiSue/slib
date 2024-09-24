#include "sbioinfo/align.h"
inline slib::subyte slib::sbio::scigar::index(const char c) {
    switch (c) {
    case 'M':
        return 0;
    case 'I':
        return 1;
    case 'D':
        return 2;
    case 'N':
        return 3;
    case 'S':
        return 4;
    case 'H':
        return 5;
    case 'P':
        return 6;
    case '=':
        return 7;
    case 'X':
        return 8;
    default:
        return 0;
    }
}

slib::sbio::Cigar::Cigar() : option(-1), length(0) {}
slib::sbio::Cigar::Cigar(subyte o, sint l) : option(o), length(l) {}
slib::sbio::Cigar::Cigar(suint i) : option(i&0x0F), length((i>>4)&0x0FFFFFFF) {}
slib::sbio::Cigar::Cigar(const Cigar &c) : option(c.option), length(c.length) {}
slib::sbio::Cigar & slib::sbio::Cigar::operator = (const Cigar &c) { option = c.option; length = c.length; return *this; }
bool slib::sbio::Cigar::ref() const {
    return option == scigar::MATCH ||
        option == scigar::DELETION ||
        option == scigar::SKIP ||
        option == scigar::PMATCH ||
        option == scigar::MMATCH;
}
bool slib::sbio::Cigar::que() const {
    return option == scigar::MATCH ||
        option == scigar::INSERTION ||
        option == scigar::PADDING ||
        option == scigar::SCLIP ||
        option == scigar::HCLIP ||
        option == scigar::PMATCH ||
        option == scigar::MMATCH;
}
bool slib::sbio::Cigar::gap() const {
    return option == scigar::DELETION ||
        option == scigar::SKIP ||
        option == scigar::INSERTION ||
        option == scigar::PADDING;
}
bool slib::sbio::Cigar::clip() const {
    return option == scigar::SCLIP ||
        option == scigar::HCLIP;
}

bool slib::sbio::Cigar::operator < (const Cigar &c) const { return option!=c.option?option<c.option:length < c.length; }
bool slib::sbio::Cigar::operator == (const Cigar &c) const { return option == c.option && length == c.length; }
bool slib::sbio::Cigar::operator != (const Cigar &c) const { return !(*this == c); }

slib::sbio::CigarArray::CigarArray() : _ptr(nullptr), _offset(nullptr), _end(nullptr), _capacity(0) {}
slib::sbio::CigarArray::CigarArray(const size_t n) : CigarArray() { resize(n); }
slib::sbio::CigarArray::CigarArray(const char *s) : CigarArray() {
    if(!s || s[0] == '\0') return;
    String lstr;
    auto p = s;
    while (*p != '\0') {
        if (sutf8::isNumChar(*p)) lstr << String(*p);
        else {
            add(Cigar(sbio::scigar::index(*p), lstr.intValue()));
            lstr.clear();
        }
        ++p;
    }
}
slib::sbio::CigarArray::CigarArray(const Cigar &c) : CigarArray() { add(c); }
slib::sbio::CigarArray::CigarArray(const int n, const suint *cigars) : CigarArray() { set(n, cigars); }
slib::sbio::CigarArray::CigarArray(CigarArray&& array) noexcept { swap(array); }
slib::sbio::CigarArray::CigarArray(const CigarArray &array) : CigarArray() {
    reserve(array.size() + 2, 1);
    resize(array.size());
    sfor2(*this, array) $_1 = $_2;
}
slib::sbio::CigarArray::~CigarArray() {}
slib::sbio::CigarArray& slib::sbio::CigarArray::operator=(slib::sbio::CigarArray&& array) noexcept {
    swap(array); return *this;
}
slib::sbio::CigarArray & slib::sbio::CigarArray::operator=(const slib::sbio::CigarArray &array) {
    clear();
    reserve(array.size() + 2, 1);
    resize(array.size());
    sfor2(*this, array) $_1 = $_2;
    return *this;
}
bool slib::sbio::CigarArray::empty() const { return _offset == _end; }
size_t slib::sbio::CigarArray::size() const { return _end - _offset; }
size_t slib::sbio::CigarArray::capacity() const { return _capacity; }
size_t slib::sbio::CigarArray::offset() const { return _offset - _ptr; }
void slib::sbio::CigarArray::setOffset(const size_t off) {
    if (_capacity) {
        if (_capacity <= off) throw OverFlowException(overflowErrorText("offset",_capacity - 1));
        auto current = offset(), sz = size();
        if (off < current) {
            if (sz) Memory<Cigar>::shift(_offset, _ptr + off, sz);
            _offset = _ptr + off; _end = _offset + sz;
        }
        else if (current < off) {
            if (current + sz <= off) clear();
            else memset(_offset, 0, sizeof(Cigar) * (off - current));
            _offset = _ptr + off;
            if (_end < _offset)  _end = _offset;
        }
    }
    else reserve(off < 4 ? 8 : 2 * off, off < 4 ? 4 : off);
}
void slib::sbio::CigarArray::resize(const size_t sz) {
    auto s = size();
    if (!_capacity) reserve(sz < 6 ? 8 : (sz + 2), 1);
    if (sz < s) _end -= (s - sz);
    else if (s < sz) {
        auto off = offset();
        if (_capacity <= off + sz) {
            if (sz + 1 < _capacity) {
                if (s) Memory<Cigar>::shift(_offset, _ptr + (_capacity - sz) / 2, s);
                _offset = _ptr + (_capacity - sz) / 2;
                _end = _offset + s;
            }
            else {
                auto c = _capacity;
                while (c <= sz) { c <<= 1; }
                reserve(c, (c - sz) / 2);
            }
        }
        sforin(i, s, sz) { Memory<Cigar>::init(_end); ++_end; }
    }
}
void slib::sbio::CigarArray::reserve(const size_t sz, const size_t off) {
    auto off_ = off == -1 ? (sz - size()) / 2 : off;
    if (_capacity) {
        if (sz <= _capacity && off_ + size() < _capacity) {
            auto s = size(), o = offset();
            if (o < off_) Memory<Cigar>::shift(_offset, _ptr + off_, size());
        }
        else {
            auto s = size(), c = _capacity;
            while (c < sz || c <= (off_ + s)) c <<= 1;
            auto tmp = Memory<Cigar>::alloc(c);
            Memory<Cigar>::copy(tmp + off_, _offset, s);
            _offset = tmp + off_;
            _end = _offset + s;
            _capacity = c;
            Memory<Cigar*>::swap(&tmp, &_ptr);
            Memory<Cigar>::dealloc(tmp);
        }
    }
    else {
        _ptr = Memory<Cigar>::alloc(sz);
        if (sz <= off_) throw OverFlowException(overflowErrorText("Offset", sz - 1));
        _offset = _ptr + off_;
        _end = _offset;
        _capacity = sz;
    }
}
bool slib::sbio::CigarArray::clipped(const size_t sz) const {
    if (empty()) return false;
    return ((at(0).option == scigar::SCLIP || at(0).option == scigar::HCLIP) && sz < at(0).length) ||
        ((at(-1).option == scigar::SCLIP || at(-1).option == scigar::HCLIP) && sz < at(-1).length);
}
slib::ArrayIterator<slib::sbio::Cigar> slib::sbio::CigarArray::iterAt(const int i) {
    auto it = (i < 0 ? _end : _offset) + i;
    if (_offset <= $ && $ < _end) return slib::ArrayIterator<slib::sbio::Cigar>($);
    throw RangeException(outRangeErrorText("index", (int)($ - _offset), 0, (int)(_end - _offset)));
}
slib::ArrayCIterator<slib::sbio::Cigar> slib::sbio::CigarArray::iterAt(const int i) const {
    auto it = (i < 0 ? _end : _offset) + i;
    if (_offset <= $ && $ < _end) return slib::ArrayCIterator<slib::sbio::Cigar>($);
    throw RangeException(outRangeErrorText("index", (int)($ - _offset), 0, (int)(_end - _offset)));
}
slib::sbio::Cigar& slib::sbio::CigarArray::at(const int i) { return *iterAt(i); }
const slib::sbio::Cigar& slib::sbio::CigarArray::at(const int i) const { return *iterAt(i); }
slib::sbio::Cigar& slib::sbio::CigarArray::operator[](const int i) { return at(i); }
const slib::sbio::Cigar& slib::sbio::CigarArray::operator[](const int i) const { return at(i); }
slib::ArrayIterator<slib::sbio::Cigar> slib::sbio::CigarArray::begin() { return slib::ArrayIterator<slib::sbio::Cigar>(_offset); }
slib::ArrayCIterator<slib::sbio::Cigar> slib::sbio::CigarArray::begin() const { return slib::ArrayCIterator<slib::sbio::Cigar>(_offset); }
slib::ArrayIterator<slib::sbio::Cigar> slib::sbio::CigarArray::end() { return slib::ArrayIterator<slib::sbio::Cigar>(_end); }
slib::ArrayCIterator<slib::sbio::Cigar> slib::sbio::CigarArray::end() const { return slib::ArrayCIterator<slib::sbio::Cigar>(_end); }

void slib::sbio::CigarArray::add(const Cigar &c, DIRECTION dir) {
    if (!_capacity) reserve(8, 4);
    if (dir == DIRECTION::HEAD) {
        if (size() && at(0).option == c.option) at(0).length += c.length;
        else {
            if (_offset == _ptr) {
                if (_end < _ptr + _capacity - 1) {
                    Memory<Cigar>::shift(_offset, _offset + 1, size());
                    ++_offset; ++_end;
                }
                else reserve(2 * _capacity, (2 * _capacity - size()) / 2);
            }
            Memory<Cigar>::init(_offset - 1, c);
            --_offset;
        }
    }
    else {
        if (size() && at(-1).option == c.option) at(-1).length += c.length;
        else {
            if (_end == _ptr + _capacity - 1) {
                if (_ptr < _offset) {
                    Memory<Cigar>::shift(_offset, _offset - 1, size());
                    --_offset; --_end;
                }
                else reserve(2 * _capacity, (2 * _capacity - size()) / 2);
            }
            Memory<Cigar>::init(_end, c);
            ++_end;
        }
    }
}
void slib::sbio::CigarArray::append(const CigarArray &array, DIRECTION dir) {
    if (array.empty()) return;
    if (dir == DIRECTION::HEAD) { srfor(array) add($_, dir); }
    else { sfor(array) add($_, dir); }
}
void slib::sbio::CigarArray::set(const int n, const suint* c) {
    resize(n);
    sfor(*this) { $_ = *c; ++c; }
}
void slib::sbio::CigarArray::remove(const size_t off, const size_t len) {
    if (empty()) return;
    if (off == 0) {
        if (len == -1 || size() <= len) clear();
        else _offset += len;
    }
    else if (len == -1 || size() <= off + len) resize(off);
    else {
        Memory<Cigar>::shift(_offset + off + len, _offset + off, len);
        resize(size() - len);
    }
}
void slib::sbio::CigarArray::reverse() {
    auto beg = _offset, end = _end - 1;
    while (beg < end) {
        Memory<Cigar>::swap(beg, end);
        ++beg; --end;
    }
}
void slib::sbio::CigarArray::clear() { 
    sforin(it, _offset, _end) { Memory<Cigar>::destroy($); $NEXT; }
    _end = _offset;
}
void slib::sbio::CigarArray::swap(slib::sbio::CigarArray& array) {
    Memory<Cigar*>::swap(&_ptr, &array._ptr);
    Memory<Cigar*>::swap(&_offset, &array._offset);
    Memory<Cigar*>::swap(&_end, &array._end);
    Memory<size_t>::swap(&_capacity, &array._capacity);
}
size_t slib::sbio::CigarArray::refSize(size_t beg , size_t len) const {
    auto length = 0;
    auto b = begin() + beg, e = len == -1 ? end() : b + len;
    sforin(it, b, e) { if ($_.ref()) length += $_.length; }
    return length;
}
size_t slib::sbio::CigarArray::queSize(size_t beg, size_t len) const {
    auto length = 0;
    auto b = begin() + beg, e = len == -1 ? end() : b + len;
    sforin(it, b, e) { if ($_.que()) length += $_.length; }
    return length;
}
size_t slib::sbio::CigarArray::countCigar(const subyte op) {
    size_t count = 0;
    sfor(*this) { if ($_.option == op) ++count; }
    return count;
}
size_t slib::sbio::CigarArray::sizeofOP(const subyte op) {
    size_t length = 0;
    sfor(*this) { if ($_.option == op) length += $_.length; }
    return length;
}
slib::String slib::sbio::CigarArray::toString() const {
	String str;
	sfor(*this) { str << $_.length << scigar::LABEL[$_.option]; }
    return str;
}
bool slib::sbio::CigarArray::operator==(const slib::sbio::CigarArray &array) const {
    if (size() != array.size()) return false;
    sfor2(*this, array) { if ($_1 != $_2) return false; }
    return true;
}