#include "sobj/sobjptr.h"
#include "sobj/stext.h"

slib::String slib::stxt::wrap(const slib::String& str, const size_t row, const size_t offset, const char* breaker, bool forced) {
    String wrapped, tmp, buf;
    size_t blen = breaker ? strlen(breaker) : 0;
    if (row <= offset) throw RangeException(outRangeErrorText("Offset", offset, 0, row));
    size_t count = 0, num = row - offset - (forced ? blen : 0);
    sforc(str) {
        if (forced) {
            if (count == num) {
                wrapped << (SP * offset) << buf << breaker << NL;
                count = 0; continue;
            }
            else {
                buf << $_; ++count;
            }
        }
        else {
            if (sutf8::isWS($_.cstr()[0])) {
                if (num < count) {
                    count -= tmp.length();
                    if (sutf8::isWS(tmp[0])) tmp.removeAt(0);
                    if (tmp.size()) wrapped << (SP * offset) << tmp << NL; 
                    tmp.clear();
                }
                tmp << buf;
                buf.clear();
            }
            buf << $_; ++count;
        }
    }
    if (num < count) {
        if (sutf8::isWS(tmp[0])) tmp.removeAt(0);
        if (tmp.size())  wrapped << (SP * offset) << tmp << NL;
        tmp.clear();
    }
    if (buf.size()) tmp << buf;
    if (tmp.size()) {
        if (sutf8::isWS(tmp[0])) tmp.removeAt(0);
        if (tmp.size()) wrapped << (SP * offset) << tmp;
    }
    return wrapped;
}

slib::TextAttribute::TextAttribute() { size = 12.f; style = 0; color = scolor::BLACK; background = scolor::CLEAR; }
slib::TextAttribute::TextAttribute(const SDictionary& dict) : TextAttribute() {
    if (dict.hasKey("size")) size = dict["size"];
    if (dict.hasKey("font")) font = dict["font"];
    if (dict.hasKey("style")) style = dict["style"];
    if (dict.hasKey("color")) color = Color(dict["color"].intValue());
    if (dict.hasKey("bg")) background = Color(dict["bg"].intValue());
}
slib::TextAttribute::TextAttribute(const TextAttribute& attr) : TextAttribute() {
    font = attr.font;
    size = attr.size;
    style = attr.style;
    color = attr.color;
    background = attr.background;
}
slib::TextAttribute::~TextAttribute() {}
slib::TextAttribute& slib::TextAttribute::operator=(const TextAttribute& attr) {
    font = attr.font;
    size = attr.size;
    style = attr.style;
    color = attr.color;
    background = attr.background;
    return *this;
}

slib::TextAnnotation::TextAnnotation() {}
slib::TextAnnotation::~TextAnnotation() {}
size_t slib::TextAnnotation::count() const { return _attributes.size(); }
inline void _searchAnnot(slib::Range<slib::PArrayCIterator<slib::TextAttribute>>& range, int p) {
    if (range.begin == range.end) return;
    auto len = range.end - range.begin;
    if (len < 16) {
        sforin(it, range.begin, range.end) {
            if (p < $_.end) {
                range.end = range.begin;
                return;
            }
        }
        range.begin = range.end;
        return;
    }
    else {
        auto med = range.begin + len / 2;
        if (med->include(p)) {

        }
        else {
            

        }
    }
}
inline void _searchAnnot(slib::Range<slib::PArrayCIterator<slib::TextAttribute>>& range, slib::Range<int> rng) {
}

bool slib::TextAnnotation::annotated(const int pos) const {
    slib::Range<slib::PArrayCIterator<slib::TextAttribute>> range(begin(), end());
    _searchAnnot(range, pos);
    return (range.begin)->include(pos);
}
bool slib::TextAnnotation::annotated(const srange& rng) const {
    slib::Range<slib::PArrayCIterator<slib::TextAttribute>> range(begin(), end());
    _searchAnnot(range, rng);
    sforin(it, range.begin, range.end) {
        if ($_.overlap(rng)) return true;
    }
    return false;
}
slib::PArray<slib::TextAttribute> slib::TextAnnotation::attributes(const srange& rng) const {
    slib::Range<slib::PArrayCIterator<slib::TextAttribute>> range(begin(), end());
    _searchAnnot(range, rng);
    slib::PArray<slib::TextAttribute> array;
    sforin(it, range.begin, range.end) {
        if ($_.overlap(rng)) array.add($.sptr());
    }
    return array;
}
slib::TextAttribute& slib::TextAnnotation::operator[](const int i) { return _attributes[i]; }
const slib::TextAttribute& slib::TextAnnotation::operator[](const int i) const { return _attributes[i]; }
slib::PArrayIterator<slib::TextAttribute> slib::TextAnnotation::begin() { return _attributes.begin(); }
slib::PArrayCIterator<slib::TextAttribute> slib::TextAnnotation::begin() const { return _attributes.begin(); }
slib::PArrayIterator<slib::TextAttribute> slib::TextAnnotation::end() { return _attributes.end(); }
slib::PArrayCIterator<slib::TextAttribute> slib::TextAnnotation::end() const { return _attributes.end(); }

void slib::TextAnnotation::merge(const srange& rng, const TextAttribute& c) {}
void slib::TextAnnotation::erase(const srange& rng) {}

void slib::TextAnnotation::insert(const int pos, const size_t sz) {}
void slib::TextAnnotation::remove(const int pos, const size_t sz) {}
void slib::TextAnnotation::shift(const int sz) {}
void slib::TextAnnotation::clear() { _attributes.clear(); }


slib::SText::SText() : SObject() {}
slib::SText::SText(const char* str, const SDictionary& a) : SText() {
    _string = str;
    if (a.size()) setAttribute(srange(0, _string.size()), a);
}
slib::SText::SText(const String& str, const SDictionary& a) : SText() {
    _string = str;
    if (a.size()) setAttribute(srange(0, _string.size()), a);
}
slib::SText::SText(const SString& str, const SDictionary& a) : SText() {
    _string = str;
    if (a.size()) setAttribute(srange(0, _string.size()), a);
}
slib::SText::SText(const SText& text) : SText() {
    _string = text._string;
    _attributes = text._attributes;
}
slib::SText::~SText() {}


bool slib::SText::annotated(const int pos) const {
    sfor(_attributes) {
        if ($_.first.include(pos)) return true;
    }
    return false;
}
bool slib::SText::annotated(const srange& rng) const {
    sfor(_attributes) {
        if ($_.first.overlap(rng)) return true;
    }
    return false;
}


void slib::SText::setStyle(const srange& rng, const suint st) {
    TextAttribute ta;
    ta.style = st;
    _attributes.add(Pair<srange, TextAttribute>(rng, ta));
}
void slib::SText::setFont(const srange& rng, const char* f) {
    TextAttribute ta;
    ta.font = f;
    _attributes.add(Pair<srange, TextAttribute>(rng, ta));
}
void slib::SText::setSize(const srange& rng, const size_t sz) {
    TextAttribute ta;
    ta.size = sz;
    _attributes.add(Pair<srange, TextAttribute>(rng, ta));
}
void slib::SText::setColor(const srange& rng, const Color& col) {
    TextAttribute ta;
    ta.color = col;
    _attributes.add(Pair<srange, TextAttribute>(rng, ta));
}
void slib::SText::setBackground(const srange& rng, const Color& col) {
    TextAttribute ta;
    ta.background = col;
    _attributes.add(Pair<srange, TextAttribute>(rng, ta));
}
void slib::SText::setAttribute(const srange& rng, const TextAttribute& attr) {
    _attributes.add(Pair<srange, TextAttribute>(rng, attr));
}
void slib::SText::removeAttribute(const srange& rng) {}
void slib::SText::clearAttribute() { _attributes.clear(); }

const char* slib::SText::cstr() const { return _string.cstr(); }
slib::String& slib::SText::string() { return _string; }
const slib::String& slib::SText::string() const { return _string; }
slib::SText::operator const char*() const { return _string.cstr(); }

slib::String slib::SText::getClass() const { return "text"; }
slib::String slib::SText::toString(const char* format) const {
    String f(format);


    return "";
}
slib::SObject* slib::SText::clone() const { return new SText(*this); }

/*
text_style::text_style(suint t, const char *f, float s, smedia::SColor c, smedia::SColor b) :
                                        type(t), font(f), weight(1.0), size(s), color(c), background(b) {}
text_style::text_style(SDictionary& dict) : text_style() {
	if (dict["type"]) type = dict["type"];
	if (dict["font"]) font = dict["font"];
	if (dict["size"]) size = dict["size"];
	if (dict["weight"]) size = dict["weight"];
	if (dict["color"]) color = smedia::SColor((const char*)dict["color"]);
	if (dict["bg"]) background = smedia::SColor((const char*)dict["bg"]);
}
text_style::text_style(const text_style &style) {
    type = style.type;
    font = style.font;
    size = style.size;
	weight = style.weight;
    color = style.color;
    background = style.background;
}
text_style::~text_style() {}
text_style& text_style::operator=(const text_style& style) {
	type = style.type;
	font = style.font;
	size = style.size;
	weight = style.weight;
	color = style.color;
	background = style.background;
	return *this;
}
sobj text_style::toObj() const {
	return { kv("type", type), kv("font", font), kv("size", size), kv("weight", weight), kv("color", color), kv("bg", background) };
}
//SText::SText() : SObject() {}
//SText::SText(const char* s) : _string(s), SObject() {}
//SText::SText(const char* s, SDictionary&& a) : SText(s) {
    auto attr = text_style(a);
	auto range = srange(0, _string.length());
	auto atp = std::make_pair(range, attr);
	_attribute.add(atp);
}
SText::SText(sobj obj) : SText() {
	if (obj.isText()) *this = obj.text();
	else if (obj.isDict()) {
		_string = obj["text"];


	}
	else _string = obj.toString();
}
SText::SText(const SText& text) : SText() {}// { _string = text._string; _attribute = text._attribute; }
SText::~SText() {}

SText &SText::operator = (const char *s) { _string = s; return *this; };
SText &SText::operator = (String &&s) { _string = s; return *this; };
SText &SText::operator = (const String &s) { _string = s; return *this; };


SText &SText::operator = (const SString &s) {
    //_string = s; _attribute.clear(); 
    return *this;
}
SText &SText::operator = (const SText &txt) {
    //_string = txt._string; _attribute = txt._attribute; 
    return *this;
}

SText &SText::operator += (const SText &txt) {
    append(txt);
    return *this;
}
SText SText::operator + (const SText &txt) {
    SText t = *this;
    return t += txt;
}
*/
/*
srange SText::aindexAt(size_t idx) const {
    if (length() < idx) throw slib_exception(SLIB_RANGE_ERROR, "Out of text range.");
    if (!text_attributes.empty() && idx <= text_attributes.last().range.end) {
        auto it = text_attributes.begin();
        if (idx < it->range.begin) return srange(-1, 0);
        else if(it->range.include(idx)) return srange(0, 0);
        ++it;
        while (it < text_attributes.end()) {
            if (it->range.include(idx))
                return srange(it-text_attributes.begin(), it-text_attributes.begin());
            else if((it-1)->range.end < idx && idx < it->range.begin)
                return srange(it-text_attributes.begin()-1, it-text_attributes.begin());
            ++it;
        }
    }
    return srange(text_attributes.size(), text_attributes.size());
}
SText &SText::append(const SText &txt) {
    int len = _string.length();
    _string.append(txt.string());
    //sforeach(txt._attribute) _attribute.add(std::make_pair((E_.first >> len), E_.second));
    return *this;
}

void SText::insert(size_t idx, const char *s) {
    if (s) {
        size_t len = strlen(s);
        _string.insert(idx, s);
        if (!_attribute.empty()) {
            
            sforeach(_attribute) {
                if (idx <= E_.first.begin) E_.first.shift(len);
                if (E_.first.include(idx)) E_.first.expand(len);
            
        }
        }
    }
}
void SText::insert(size_t idx, SText &txt) {
    
    
    
    size_t len = txt.length();
    srange range(idx, idx+1);
    if (!text_attributes.empty()) {
        sforeach(text_attributes) {
            if (idx <= E_.first.begin) E_.first.shift(idx);
            if (E_.first.include(idx)) E_.first.expand(len);
        }
    }
    if (!txt.text_attributes.empty()) {
        sforeach(txt.text_attributes) {
            text_attributes.add(E_); text_attributes.last().first.shift(idx);
        }
    }
    std::sort(text_attributes.begin(), text_attributes.end(),
              [](const text_attribute &a1, const text_attribute &a2) { return a1.first < a2.first; } );
    std::string::insert(idx, txt.cstr());
     
}

void SText::removeAt(size_t idx) { remove(srange(idx, idx+1)); }
void SText::remove(size_t off, size_t len){ remove(srange(off, off+len)); }
void SText::remove(srange range) {
    auto len = range.length();
    //
    
}
SText SText::subtext(srange range) { return subtext(range.begin, range.length()); }

SText SText::subtext(size_t off, size_t len) {
    SText txt;
    //
    return txt;
}

void SText::clip(size_t off, size_t len) {
    
}
void SText::clip(srange range) { clip(range.begin, range.length()); }

void SText::resize(size_t size) {
    if (size < _string.size()) {
        _attribute.sort([](const text_attribute &t1, const text_attribute &t2) {
            return t1.first < t2.first;
        });
        sforeach(_attribute) {
            if (E_.first.include(size)) E_.first.end = size;
            else if (size < E_.first.begin) { _attribute.remove(it-_attribute.begin(), -1); break; }
        }
    }
    _string.resize(size);
}

void SText::load(const char *path) {//}
void SText::save(const char *path) {//}

const Array<text_attribute>& SText::attributes() const { return _attribute; }
//Array<text_attribute>& SText::attributes() { return _attribute; }
slib::Array<text_attribute> SText::attribute(srange range) const {
    Array<text_attribute> list;
    if (_attribute.size()) {
        sforeach(_attribute) { if (E_.first.overlap(range)) list.add(E_); }
    }
    return list;
}



void SText::setStyle(srange range, const text_style &style) {
    _attribute.add(std::make_pair(range, style));
}
void SText::setType(srange range, uint16_t type) {
    text_attribute ta;
    ta.first = range;
    ta.second.type = type;
    _attribute.add(ta);
}
void SText::setFont(srange range, const char *font) {
    text_attribute ta;
    ta.first = range;
    ta.second.font = font;
    _attribute.add(ta);
}
void SText::setSize(srange range, const float &size) {
    text_attribute ta;
    ta.first = range;
    ta.second.size = size;
    _attribute.add(ta);
}
void SText::setColor(srange range, const smedia::SColor &col) {
    text_attribute ta;
    ta.first = range;
    ta.second.color = col;
    _attribute.add(ta);
}
void SText::setBGColor(srange range, const smedia::SColor &col) {
    text_attribute ta;
    ta.first = range;
    ta.second.background = col;
    _attribute.add(ta);
}
void SText::setAttribute(srange range, sobj attr) {
    text_attribute ta;
    ta.first = range;
    ta.second = attr.dict();
    _attribute.add(ta);
}

const char* SText::cstr() const { return _string.cstr(); }
String& SText::string() { return _string; }
const String& SText::string() const { return _string; }

String SText::getClass() const { return "text"; }
String SText::toString() const {
    //
    return _string;
}
SObject *SText::clone() const { return new SText(*this); }
*/