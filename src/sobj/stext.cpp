#include "sobj/stext.h"
#include "sio/sfile.h"
#include "sutil/sxml.h"

using namespace slib;
using namespace slib::sio;

STextStyle::STextStyle(int t, const char *f, float s, smedia::SColor c, smedia::SColor b) :
                                        type(t), font(f), size(s), color(c), background(b) {}
STextStyle::STextStyle(const sobj &obj) : STextStyle() {
	if(obj["type"]) type = obj["type"];
	if(obj["font"]) font = obj["font"];
	if(obj["size"]) size = obj["size"];
	if(obj["color"]) color = smedia::SColor((const char *)obj["color"]);
	if(obj["bg"]) background = smedia::SColor((const char*)obj["color"]);
}
STextStyle::STextStyle(const STextStyle &style) {
    type = style.type;
    font = style.font;
    size = style.size;
    color = style.color;
    background = style.background;
}
STextStyle::~STextStyle() {}

STextStyle &STextStyle::operator=(const STextStyle &style) {
    type = style.type;
    font = style.font;
    size = style.size;
    color = style.color;
    background = style.background;
    return *this;
}
STextStyle &STextStyle::operator=(const SDictionary &dict) {
    type = dict["type"];
    font = dict["font"];
    size = dict["size"];
    color = dict["color"];
    background = dict["background"];
    return *this;
}

SText::SText() {}
SText::SText(const String &s) : _string(s) {}
SText::SText(const String &s, const SDictionary &a) : _string(s) { 
	auto attr = STextStyle(a);
	auto range = srange(0, s.length());
	auto atp = std::make_pair(range, attr);
	_attribute.add(atp);
}
SText::SText(const SText &text) : SText(text._string) { _attribute = text._attribute; }
SText::~SText() {}

SText &SText::operator = (const char *s) { _string = s; return *this; };
SText &SText::operator = (String &&s) { _string = s; return *this; };
SText &SText::operator = (const String &s) { _string = s; return *this; };


SText &SText::operator = (const SString &s) {
    _string = s; _attribute.clear(); return *this;
}
SText &SText::operator = (const SText &txt) {
    _string = txt._string; _attribute = txt._attribute; return *this;
}

SText &SText::operator += (const SText &txt) {
    append(txt);
    return *this;
}
SText SText::operator + (const SText &txt) {
    SText t = *this;
    return t += txt;
}
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
*/
SText &SText::append(const SText &txt) {
    int len = _string.length();
    _string.append(txt.string());
    sforeach(txt._attribute) _attribute.add(std::make_pair(E_.first.shifted(len), E_.second));
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
    
    
    
    /*
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
     */
}

void SText::removeAt(size_t idx) { remove(srange(idx, idx+1)); }
void SText::remove(size_t off, size_t len){ remove(srange(off, off+len)); }
void SText::remove(srange range) {
    auto len = range.length();
    /*
     */
    
}

SText SText::subtext(srange range) { return subtext(range.begin, range.length()); }

SText SText::subtext(size_t off, size_t len) {
    SText txt;
    /*
     */
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

void SText::load(const char *path) {/**/}
void SText::save(const char *path) {/**/}

const slib::Array<SText::text_attribute> &SText::attributes() const { return _attribute; }

slib::Array<SText::text_attribute> SText::attribute(srange range) const {
    Array<SText::text_attribute> list;
    if (_attribute.size()) {
        sforeach(_attribute) { if (E_.first.overlap(range)) list.add(E_); }
    }
    return list;
}

const char *SText::cstr() const { return _string.cstr(); }
const String &SText::string() const { return _string; }


void SText::setStyle(srange range, const STextStyle &style) {
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
void SText::setAttribute(srange range, const SDictionary &dic) {
    text_attribute ta;
    ta.first = range;
    ta.second = dic;
    _attribute.add(ta);
}

String SText::getClass() const { return "text"; }
String SText::toString() const {
    /*
     */
    return _string;
}
SObject *SText::clone() const { return new SText(*this); }



