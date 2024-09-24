#include "sbasic/search.h"
#include "sbasic/array.h"

slib::Regex slib::makeRegexQuery(const char* que, MATCH m, bool ignore) {
    String reg("/^");
    switch (m) {
    case MATCH::EXACT:
        reg << que << "$/" << (ignore ? "i" : "");
        break;
    case MATCH::START:
        reg << que << ".*$/" << (ignore ? "i" : "");
        break;
    case MATCH::END:
        reg << ".*" << que << "$/" << (ignore ? "i" : "");
        break;
    case MATCH::PARTIAL:
        reg << ".*" << que << ".*$/" << (ignore ? "i" : "");
        break;
    default:
        break;
    }
    return Regex(reg);
}

slib::Regex::Regex() : global(false) {}
slib::Regex::Regex(const char *s) {
    if (!s) throw NullException(nullErrorText("Regex pattern"));
    auto len = strlen(s);
    auto end = &s[len - 1];
    auto constant = std::regex::ECMAScript;
    while (s < end) {
        if (*end == '/') break;
        else if (*end == 'i') constant |= std::regex::icase;
        else if (*end == 'g') global = true;
        --end;
    }
    if (len < 3 || s[0] != '/' || end == s)
        throw FormatException(formatErrorText("slib::Regex", s, "ECMA script regex pattern (i.e. /.*/)"));
    _rgx = std::regex(&s[1], end, constant);
}
slib::Regex::~Regex() {}
slib::Regex& slib::Regex::operator=(const char* s) {
    if (!s) throw NullException(nullErrorText("Regex pattern"));
    auto len = strlen(s);
    auto end = &s[len - 1];
    auto constant = std::regex::ECMAScript;
    while (s < end) {
        if (*end == '/') break;
        else if (*end == 'i') constant |= std::regex::icase;
        else if (*end == 'g') global = true;
        --end;
    }
    if (len < 3 || s[0] != '/' || end == s)
        throw FormatException(formatErrorText("slib::Regex", s, "ECMA script regex pattern (i.e. /.*/)"));
    _rgx = std::regex(&s[1], end, constant);
    return *this;
}
slib::Regex& slib::Regex::operator=(const slib::Regex& reg) {
    _rgx = reg._rgx; global = reg.global; return *this;
}
const std::regex &slib::Regex::context() const { return _rgx; }
bool slib::Regex::match(const char *s) const {
    std::cmatch m;
    std::regex_search(s, m, _rgx);
    return m.size();
}
bool slib::Regex::equal(const char* s) const {
    return std::regex_match(s, _rgx);
}
size_t slib::Regex::find(const char* s, size_t offset, bool rev) const {
    if (!s || s[0] == '\0') return NOT_FOUND;
    auto len = strlen(s);
    if (rev) {
        size_t p;
        std::cregex_iterator beg(s, s + len - offset, _rgx), end;
        if (beg == end) return NOT_FOUND;
        while (beg != end) { p = beg->position(); ++beg; }
        return p;
    }
    else {
        std::cmatch m;
        std::regex_search(s + offset, m, _rgx);
        if (m.empty()) return NOT_FOUND;
        else return m.position();
    }
}
slib::Array<size_t> slib::Regex::findAll(const char* s) const {
    if (!s || s[0] == '\0') return { NOT_FOUND };
    auto len = strlen(s);
    slib::Array<size_t> pos;
    std::cregex_iterator beg(s, s + len, _rgx), end;
    if (beg == end) return pos;
    while (beg != end) { pos.add(beg->position()); ++beg; }
    return pos;
}
slib::Range<std::cregex_iterator> slib::Regex::search(const char* s, size_t offset) const {
    if (!s || s[0] == '\0') return slib::Range<std::cregex_iterator>(std::cregex_iterator(), std::cregex_iterator());
    auto len = strlen(s);
    return slib::Range<std::cregex_iterator>(
        std::cregex_iterator(s + offset, s + len, _rgx),
        std::cregex_iterator()
    );
}