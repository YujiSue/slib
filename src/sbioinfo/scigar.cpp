#include "sbioinfo/sbalign.h"

using namespace slib;
using namespace slib::sbio;

const char *scigar::CIGAR_STRING = "MIDNSHP=X";
typedef std::pair<char, subyte> cub;
const Map<char, subyte> scigar::CIGAR_INDEX = {
    cub('M', 0), cub('I', 1), cub('D', 2),
    cub('N', 3), cub('S', 4), cub('H', 5),
    cub('P', 6), cub('=', 7), cub('X', 8)
};
scigar::scigar() : option(-1), length(0) {}
scigar::scigar(subyte o, sint l) : option(o), length(l) {}
scigar::scigar(suint i) : option(i&0x0F), length((i>>4)&0x0FFFFFFF) {}
scigar::scigar(const scigar &c) : option(c.option), length(c.length) {}

scigar & scigar::operator = (const scigar &c) { option = c.option; length = c.length; return *this; }
bool scigar::operator < (const scigar &c) const { return option!=c.option?option<c.option:length < c.length; }
bool scigar::operator == (const scigar &c) const { return option == c.option && length == c.length; }
bool scigar::operator != (const scigar &c) const { return !(*this == c); }

SCigarArray::SCigarArray() : cigarray() {}
SCigarArray::SCigarArray(size_t n) : cigarray(n) {}
SCigarArray::SCigarArray(const char *s) : SCigarArray() {
    if(!s || s[0] == '\0') return;
    String lstr;
    auto p = s;
    while (*p != '\0') {
        if (Char::isNumChar(*p)) lstr.add(*p);
        else {
            add(scigar(scigar::CIGAR_INDEX[*p], lstr.intValue()));
            lstr.clear();
        }
        ++p;
    }
}
SCigarArray::SCigarArray(const scigar &c) : SCigarArray() { add(c); }
SCigarArray::SCigarArray(sint n, suint *cigars) : cigarray(n) { set(n, cigars); }
SCigarArray::SCigarArray(std::initializer_list<scigar> li) : cigarray(li) {}
SCigarArray::SCigarArray(const SCigarArray &array) : cigarray(array) {}
SCigarArray::~SCigarArray() {}
SCigarArray &SCigarArray::operator=(const SCigarArray &array) {
    clear();
    if (!array.empty()) {
        resize(array.size());
        auto it_ = array.begin();
        sforeach(*this) { E_ = *it_; ++it_; }
    }
    return *this;
}
void SCigarArray::add(const scigar &c) {
    if (cigarray::size() && last().option == c.option) last().length += c.length;
    else cigarray::add(c);
}
void SCigarArray::append(const SCigarArray &c) {
    if (c.empty()) return;
    if (c.first().option == last().option) {
        last().length += c.first().length;
        if (1 < c.size()) cigarray::append(&c[1], c.size()-1);
    }
    else cigarray::append(c);
}
void SCigarArray::put(const scigar &c) {
    if (size() && first().option == c.option) first().length += c.length;
    else cigarray::put(c);
}
void SCigarArray::pile(const SCigarArray &c) {
    if (c.empty()) return;
    if (size() && first().option == c.last().option) {
        first().length += c.last().length;
        if (1 < c.size()) cigarray::pile(&c[0], c.size()-1);
    }
    else cigarray::pile(c);
}
void SCigarArray::set(int n, suint *c) {
    cigarray::resize(n, false);
    auto p = ptr();
    sforin(i, 0, n) { *p = *c; ++p; ++c; }
}
void SCigarArray::reverse() {
    auto p = &at(0), p_ = &at(-1);
    while (p < p_) {
        auto tmp = *p;
        *p = *p_;
        *p_ = tmp;
		++p; --p_;
    }
}
size_t SCigarArray::countRef(size_t beg , size_t len) const {
    if (len == -1) len = cigarray::size();
    size_t length = 0;
    auto it = begin()+beg, end = it+len;
    while (it < end) {
        if (E_.option == scigar::MATCH ||
			E_.option == scigar::DELETION ||
			E_.option == scigar::PMATCH ||
			E_.option == scigar::MMATCH) length += E_.length;
        NEXT_;
    }
    return length;
}
size_t SCigarArray::countQue(size_t beg, size_t len) const {
    if (len == -1) len = cigarray::size();
    size_t length = 0;
    auto it = begin()+beg, end = it+len;
    while (it < end) {
        if (E_.option == scigar::MATCH ||
			E_.option == scigar::INSERTION ||
			E_.option == scigar::SCLIP ||
			E_.option == scigar::HCLIP ||
			E_.option == scigar::PMATCH ||
			E_.option == scigar::MMATCH) length += E_.length;
		NEXT_;
    }
    return length;
}
size_t SCigarArray::countCigar(subyte op) {
    size_t count = 0;
    sforeach(*this) { if(E_.option == op) count += E_.length; }
    return count;
}
String SCigarArray::toString() const {
	String str;
	sforeach(*this) { str << E_.length << scigar::CIGAR_STRING[E_.option]; }
    return str;
}
bool SCigarArray::operator==(const SCigarArray &array) const {
    if (size() != array.size()) return false;
    auto it = begin(), it_ = array.begin();
    while (it < end()) {
        if (E_ != *it_) return false;
        NEXT_; ++it_;
    }
    return true;
}