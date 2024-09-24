#include "sobj/sobjptr.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sobj/stext.h"
#include "sobj/stable.h"
#include "sio/sfile.h"
#include "smedia/simage.h"
#include "smedia/sfigure.h"
#include "sutil/sjson.h"

slib::String slib::toString(const slib::SObjPtr& obj, const char* format) { return obj.toString(format); }
std::ostream& slib::operator<<(std::ostream& os, const slib::SObjPtr& obj) { return os << obj.toString(); }
slib::String::String(const slib::SObjPtr& obj) : String() { clear(); append(obj.toString()); }
slib::String& slib::String::operator=(const slib::SObjPtr& obj) { clear(); append(obj.toString()); return *this; }
slib::String& slib::String::operator+=(const slib::SObjPtr& obj) { append(obj.toString()); return *this; }
slib::String& slib::String::operator<<(const slib::SObjPtr& obj) { append(obj.toString()); return *this; }
slib::String slib::String::operator+(const slib::SObjPtr& obj) const { String str(*this); str << obj.toString(); return str; }

slib::SObjPtr::SObjPtr() { instance = nullptr; }
slib::SObjPtr::SObjPtr(const bool n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const sbyte n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const subyte n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const sshort n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const sushort n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const int n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(unsigned int n)  : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const size_t n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const long n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const long long n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const float n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const double n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const Fraction<sint>& n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(const Complex<float>& n) : SObjPtr() { instance = new slib::SNumber(n); }

slib::SObjPtr::SObjPtr(const SNumber& n) : SObjPtr() { instance = new slib::SNumber(n); }
slib::SObjPtr::SObjPtr(SNumber* n) : SObjPtr() { instance = n; }


slib::SObjPtr::SObjPtr(const char* s) : SObjPtr() { instance = new slib::SString(s); }
slib::SObjPtr::SObjPtr(const slib::String& s) : SObjPtr() { instance = new slib::SString(s); }
slib::SObjPtr::SObjPtr(const slib::SString& s) : SObjPtr() { instance = new slib::SString(s); }
slib::SObjPtr::SObjPtr(slib::SString* s) : SObjPtr() { instance = s; }

slib::SObjPtr::SObjPtr(const slib::SDate& d) : SObjPtr() { instance = new slib::SDate(d); }


slib::SObjPtr::SObjPtr(std::initializer_list<slib::SObjPtr> li) : SObjPtr() { instance = new slib::SArray(li); }
slib::SObjPtr::SObjPtr(const stringarray &array) : SObjPtr() { 
    auto sarr = new SArray(array.size());
    if (array.size()) {
        sfor2(*sarr, array) $_1 = toObj($_2);
    }
    instance = sarr;
}
slib::SObjPtr::SObjPtr(const slib::SArray& array) : SObjPtr() { instance = new slib::SArray(array); }
slib::SObjPtr::SObjPtr(slib::SArray* array) : SObjPtr() { instance = array; }
slib::SObjPtr::SObjPtr(const sattribute& attr) : SObjPtr() {
    auto sdict = new SDictionary();
    if (attr.size()) {
        sfor(attr) sdict->set($_.key(), toObj($_.value()));
    }
    instance = sdict;
}
slib::SObjPtr::SObjPtr(std::initializer_list<Pair<slib::String, slib::SObjPtr>> li) : SObjPtr() { instance = new slib::SDictionary(li); }
slib::SObjPtr::SObjPtr(const slib::SDictionary& dict) : SObjPtr() { instance = new slib::SDictionary(dict); }
slib::SObjPtr::SObjPtr(slib::SDictionary* dict) : SObjPtr() { instance = dict; }
slib::SObjPtr::SObjPtr(const slib::SData& dat) : SObjPtr() { instance = new slib::SData(dat); }

slib::SObjPtr::SObjPtr(const slib::SFile& f) : SObjPtr() { instance = new slib::SFile(f); }
slib::SObjPtr::SObjPtr(SFile* f) : SObjPtr() { instance = f; }


slib::SObjPtr::SObjPtr(const slib::STable& tbl) { instance = new slib::STable(tbl); }

slib::SObjPtr::SObjPtr(const slib::SFigure& fig) { instance = new slib::SFigure(fig); }
slib::SObjPtr::SObjPtr(const slib::SCanvas& cnvs) { instance = new slib::SCanvas(cnvs); }



slib::SObjPtr::SObjPtr(const SElement& elem) : SObjPtr() {
    instance = const_cast<SObject*>(elem.element.obj->instance);
    share();
}
slib::SObjPtr::SObjPtr(const SObjPtr& obj) : SObjPtr() {
    instance = obj.instance; share();
}
/*
#if defined(WIN64_OS)
slib::SObjPtr::SObjPtr(long i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
slib::SObjPtr::SObjPtr(unsigned long ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
#endif
#if defined(MAC_OS)
slib::SObjPtr::SObjPtr(unsigned long long ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
#endif
#ifdef LINUX_OS
slib::SObjPtr::SObjPtr(sinteger i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
#endif
slib::SObjPtr::SObjPtr(const Time& t) : _type(DICT_OBJ) {
    _ptr = new SDictionary({
        kv("__key__", V({ "year", "month", "day", "hour", "minute", "second" })),
        kv("year", t.year), kv("month", t.month), kv("day", t.day),
        kv("hour", t.hour), kv("minute", t.minute), kv("second", t.second)
        });
}
SObjPtr::SObjPtr(const std::string& str) : _type(STRING_OBJ), _ptr(new SString(str)) {}
SObjPtr::SObjPtr(String&& str) : _type(STRING_OBJ), _ptr(new SString(std::forward<SString>(str))) {}
SObjPtr::SObjPtr(std::initializer_list<SObjPtr> li) : _type(ARRAY_OBJ), _ptr(new SArray(li)) {}
SObjPtr::SObjPtr(std::initializer_list<std::pair<String, SObjPtr>> li)
    : _type(DICT_OBJ), _ptr(new SDictionary(li)) {}
SObjPtr::SObjPtr(std::initializer_list<kvpair<String, SObjPtr>> li)
    : _type(DICT_OBJ), _ptr(new SDictionary(li)) {}
SObjPtr::SObjPtr(const stringarray& array) : _type(ARRAY_OBJ) {
    auto a = new SArray(array.size());
    if (array.size()) { sforeach2(*a, array) E1_ = E2_; }
    _ptr = a;
}
SObjPtr::SObjPtr(const sattribute& attr) : _type(DICT_OBJ) {
    auto d = new SDictionary();
    sforeach(attr) d->insert(kv(E_.key, E_.value));
    _ptr = d;
}
SObjPtr::SObjPtr(const SString& str) : _type(STRING_OBJ), _ptr(new SString(str)) {}
SObjPtr::SObjPtr(const SDate& date) : _type(DATE_OBJ), _ptr(new SDate(date)) {}
SObjPtr::SObjPtr(const SData& dat) : _type(DATA_OBJ), _ptr(new SData(dat)) {}
SObjPtr::SObjPtr(const SText& txt) : _type(TEXT_OBJ), _ptr(new SText(txt)) {}
SObjPtr::SObjPtr(const SColumn& col) : _type(COLUMN_OBJ), _ptr(new SColumn(col)) {}
SObjPtr::SObjPtr(const STable& tbl) : _type(TABLE_OBJ), _ptr(new STable(tbl)) {}
SObjPtr::SObjPtr(const SDataBase& db) : _type(DB_OBJ), _ptr(new SDataBase(db.path())) {}
SObjPtr::SObjPtr(const sio::SFile& file) : _type(FILE_OBJ), _ptr(new SFile(file)) {}
SObjPtr::SObjPtr(const smedia::SColor& col) : _type(COLOR_OBJ), _ptr(new smedia::SColor(col)) {}
SObjPtr::SObjPtr(const smedia::SImage& img) : _type(IMAGE_OBJ), _ptr(new smedia::SImage(img)) {}
SObjPtr::SObjPtr(const smedia::SFigure& fig)
    : _type(FIGURE_OBJ), _ptr(new smedia::SFigure(fig)) {}
SObjPtr::SObjPtr(const smedia::SCanvas& cnvs)
    : _type(CANVAS_OBJ), _ptr(new smedia::SCanvas(cnvs)) {}
//SObjPtr::SObjPtr(const smedia::SAudio &aud) : _type(AUDIO_OBJ), _ptr(new smedia::SAudio(aud)) {}
//SObjPtr::SObjPtr(const smedia::SMovie& mov) : _instance(new smedia::SMovie(mov)) {}
*/
/*
slib::SObjPtr::SObjPtr(SObject* so) {
    _ptr = so;
    if (so) {
        if (instanceOf<SNumber>(so)) _type = NUMBER_OBJ;
        else if (instanceOf<SChar>(so)) _type = CHAR_OBJ;
        else if (instanceOf<SText>(so)) _type = TEXT_OBJ;
        else if (instanceOf<SString>(so)) _type = STRING_OBJ;
        else if (instanceOf<SDate>(so)) _type = DATE_OBJ;

        else if (instanceOf<smedia::SImage>(so)) _type = IMAGE_OBJ;
        else if (instanceOf<SData>(so)) _type = DATA_OBJ;

        else if (instanceOf<SColumn>(so)) _type = COLUMN_OBJ;
        else if (instanceOf<SArray>(so)) _type = ARRAY_OBJ;

        else if (instanceOf<SPair>(so)) _type = PAIR_OBJ;
        else if (instanceOf<SDictionary>(so)) _type = DICT_OBJ;

        else if (instanceOf<STable>(so)) _type = TABLE_OBJ;

        else if (instanceOf<sio::SFile>(so)) _type = FILE_OBJ;
        else if (instanceOf<smedia::SFigure>(so)) _type = FIGURE_OBJ;
        else if (instanceOf<smedia::SCanvas>(so)) _type = CANVAS_OBJ;

        else if (instanceOf<smedia::SMovie>(so)) _type = MOVIE_OBJ;


    }
    else _type = NULL_OBJ;
}
slib::SObjPtr::SObjPtr(SObjPtr&& obj) noexcpet : _instance(obj._ptr) { obj.discard(); }
*/
slib::SObjPtr::~SObjPtr() { release(); }
slib::SObjPtr& slib::SObjPtr::operator = (const slib::SObjPtr& obj) { 
    if (instance == obj.instance) return *this;
    if (!instance) {
        instance = obj.instance; share();
    }
    else if (obj.isNull()) {
        release(); instance = nullptr;
    }
    else {
        /*
        if (isNum() && obj.isNum()) {
            auto& num = number();
            num = obj.number();
        }
        else if (isStr()) {
            auto& str = string();
            str = obj.toString();
        }
        
        if (isStr() && !obj.isStr()) {
            auto& str = string();
            str = obj.toString();
        }
        else {
            release();
            instance = obj.instance;
            share();
        }
        */
        release();
        instance = obj.instance;
        share();
    }
    return *this; 
}

slib::SObjPtr& slib::SObjPtr::operator++() { 
    //if (isNum()) return ++number();

    return *this; 
}
slib::SObjPtr& slib::SObjPtr::operator--() {
    //if (isNum()) return --number();

    return *this;
}
slib::SObjPtr slib::SObjPtr::operator-() const {
    if (isNull()) return snull;
    else if (isNum()) return -number();
    throw CastException(castErrorText(getClass(), "SNumber"));
}
slib::SObjPtr slib::SObjPtr::operator+(const char* s) const {
    if (isNull()) return SString(s);
    else if (isStr()) return SObjPtr(string() + s);
    return SObjPtr(toString() + s);
}
slib::SObjPtr slib::SObjPtr::operator+(const slib::String& s) const {
    if (isNull()) return SString(s);
    else if (isStr()) return SObjPtr(string() + s);
    return SObjPtr(toString() + s);
}
slib::SObjPtr slib::SObjPtr::operator+(const int n) const {
    if (isNull()) return SNumber(n);

    else if (isNum()) return number() + SNumber(n);

    throw CastException(castErrorText(getClass(), "SNumber"));
}
slib::SObjPtr slib::SObjPtr::operator+(const SElement& e) const {
    if (e.isObj()) return (*this) + e.object();
    else if (e.isChar()) return this->string() + e.toString();
    else if (e.isPair()) {
        SDictionary d(dict()); d.set(e.key(), e.value());
        return d;
    }
    return *this;
}
slib::SObjPtr slib::SObjPtr::operator+(const SObjPtr obj) const {

    if (isNum()) {
        if (obj.isNull()) return number();
        if (obj.isNum()) return number() + obj.number();

    }
    else if (isStr()) {
        if (obj.isStr()) return string() + obj.string();
        return string() + obj.toString();
    }

    return snull;
}

slib::SObjPtr slib::SObjPtr::operator-(const int n) const {
    if (isNull()) return SNumber(-n);

    else if (isNum()) return number() - SNumber(n);

    throw CastException(castErrorText(getClass(), "SNumber"));
}
slib::SObjPtr slib::SObjPtr::operator-(const SObjPtr obj) const {
    if (isNum()) {
        if (obj.isNull()) return number();
        if (obj.isNum()) return number() - obj.number();

    }

    return snull;
}

slib::SObjPtr slib::SObjPtr::operator*(const int n) const {
    if (isNull()) return snull;

    else if (isNum()) return number() * SNumber(n);
    else if (isStr()) return string() * n;

    throw CastException(castErrorText(getClass(), "SNumber or SString"));
}
slib::SObjPtr slib::SObjPtr::operator*(const SObjPtr obj) const {
    if (isNum()) {
        if (obj.isNull()) return 0;
        if (obj.isNum()) return number() * obj.number();

    }
    return snull;
}

slib::SObjPtr slib::SObjPtr::operator/(const int n) const {
    if (isNull()) return snull;

    else if (isNum()) return number() / SNumber(n);

    throw CastException(getClass() + " can not cast to SNumber.");
}
slib::SObjPtr slib::SObjPtr::operator/(const SObjPtr obj) const {
    if (isNum()) {
        if (obj.isNull()) return snum::NaN;
        if (obj.isNum()) return number() / obj.number();

    }
    return snull;
}

slib::SObjPtr slib::SObjPtr::operator%(const int n) const {
    if (isNull()) return snull;

    else if (isNum()) return number() % SNumber(n);

    throw CastException(getClass() + " can not cast to SNumber.");
}
slib::SObjPtr slib::SObjPtr::operator%(const SObjPtr obj) const {

    if (isNull()) return snull;

    else if (isNum()) {
        if (obj.isNum()) return number() % obj.number();
    }

    return snull;
}


slib::SObjPtr& slib::SObjPtr::operator+=(const char* s) {
    if (isNull()) instance = new SString(s);
    else if (isStr()) string() += s;
    else throw CastException("");
    return *this;
}
slib::SObjPtr& slib::SObjPtr::operator+=(const slib::String& s) {
    if (isNull()) instance = new SString(s);
    else if (isStr()) string() += s;
    else throw CastException("");
    return *this;
}
slib::SObjPtr& slib::SObjPtr::operator+=(const SObjPtr obj) {
    if (isNull()) *this = obj;
    else if (isNum()) {
        if (obj.isNull()) {}
        else if (obj.isNum()) number() += obj.number();
    }
    else if (isStr()) string() += obj.toString();
    
    else if (isArray()) {
        if (obj.isArray()) array().append(obj.array());
        else array().add(obj);
    }
    else if (isDict()) {
        //if (obj.isDict()) dict()(obj.array());
        //else array().add(obj);
    }
    return *this;
}



slib::SObjPtr slib::SObjPtr::toObj(const char* s) {
    if (!s) return snull;
    auto str = sstr::trim(s);
    if (sstr::isQuoted(str)) return sstr::dequote(str);
    else {
        if (str.equal(REG_(/\\(null\\)/i))) return snull;
        else if (sstr::isNumeric(str)) return SNumber(str);
        return str;
    }
}
bool slib::SObjPtr::isNull() const { return !instance;  }
bool slib::SObjPtr::isNum() const { return instance && instanceOf<SNumber>(instance); }
bool slib::SObjPtr::isStr() const { return instance && instanceOf<SString>(instance); }
bool slib::SObjPtr::isDate() const { return instance && instanceOf<SDate>(instance); }
bool slib::SObjPtr::isData() const { return instance && instanceOf<SData>(instance); }
bool slib::SObjPtr::isArray() const { return instance && instanceOf<SArray>(instance); }
bool slib::SObjPtr::isDict() const { return instance && instanceOf<SDictionary>(instance); }
bool slib::SObjPtr::isText() const { return instance && instanceOf<SText>(instance); }
bool slib::SObjPtr::isFile() const { return instance && instanceOf<SFile>(instance); }
bool slib::SObjPtr::isTable() const { return instance && instanceOf<STable>(instance); }
bool slib::SObjPtr::isImg() const { return instance && instanceOf<SImage>(instance); }
bool slib::SObjPtr::isCnvs() const { return instance && instanceOf<SCanvas>(instance); }
bool slib::SObjPtr::isFig() const { return instance && instanceOf<SFigure>(instance); }
bool slib::SObjPtr::include(const SElement& elem) const {
    if (isStr() && elem.isChar()) return string().match(elem.character().toString());
    if (isArray() && elem.isObj()) return array().include(elem.object());
    return false;
}
bool slib::SObjPtr::include(const SObjPtr& obj) const {
    if (isStr()) return string().match(obj.toString());
    if (isArray()) return array().include(obj);
    return false;
}
bool slib::SObjPtr::match(const slib::Regex& reg) const {
    if (isStr()) return string().match(reg);
    else return toString().match(reg);
}
bool slib::SObjPtr::equal(const slib::Regex& reg) const {
    if (isStr()) return string().equal(reg);
    else return toString().equal(reg);
}
bool slib::SObjPtr::match(const char* s) const {
    if (isStr()) return string().match(s);
    else return toString().match(s);
}
size_t slib::SObjPtr::indexOf(const SObjPtr& obj) const {
    if (isArray()) return array().indexOf(obj);
    else if (isTable()) return table().colIndex(obj);

    return NOT_FOUND;
}
size_t slib::SObjPtr::find(const char* s) const {

    if (isStr()) return string().find(s);

    return NOT_FOUND;
}

void slib::SObjPtr::copy(const slib::SObjPtr& o) {
    release(); 
    instance = o.instance;
    share();
}
void slib::SObjPtr::move(slib::SObjPtr& o) {
    release();
    instance = o.instance;
    o.instance = nullptr;
}
slib::SObjPtr slib::SObjPtr::clone() const {
    if (isNull()) return snull;
    return instance->clone();
}
void slib::SObjPtr::assign(const slib::SObjPtr& obj) {
    release();
    instance = obj.instance;
    share();
}
void slib::SObjPtr::share() {
    if (instance) instance->addScope();
}
void slib::SObjPtr::release() {
    if (instance) {
        //std::cout << instance->getClass() << ":" << instance->_scope << std::endl;
        instance->removeScope();
        if (!(instance->scoped())) delete instance;
        instance = nullptr;
    }
}


size_t slib::SObjPtr::size() const {
//    if (isNum()) return sizeof(SNumber::nu)
    if (isStr()) return string().size();
    else if (isData()) return data().size();
    else if (isArray()) return array().size();
    else if (isDict()) return dict().size();
    else if (isTable()) return table().nrow();
    return 0;
}
void slib::SObjPtr::resize(const size_t sz) {
    if (isStr()) return string().resize(sz);
    else if (isArray()) return array().resize(sz);

}
size_t slib::SObjPtr::length() const {
    if (isStr()) return string().length();
    return 0;
}
bool slib::SObjPtr::empty() const {
    if (isNull()) return true;
    else if (isStr()) return string().empty();
    else if (isData()) return data().empty();
    else if (isArray()) return array().empty();
    else if (isDict()) return dict().empty();

    return false;
}
slib::suint slib::SObjPtr::type() const {
    if (isNull()) return 0;
    else if (isNum()) return number().type();

    
    return 0;
}

bool slib::SObjPtr::hasKey(const char* key) const {
    if (isDict()) return dict().hasKey(key);
    return false;
}
bool slib::SObjPtr::beginWith(const char* que) const {
    if (isStr()) return string().beginWith(que);
    return false;
}
bool slib::SObjPtr::endWith(const char* que) const {
    if (isStr()) return string().endWith(que);
    return false;
}
stringarray slib::SObjPtr::keyset(bool sort) const {
    if (isDict()) return dict().keyset(sort);
    throw CastException(castErrorText(getClass(), "Dict"));
}
stringarray slib::SObjPtr::split(const char* sep, const bool trim, const bool ignore_quot, const bool dequote) const {
    if (isStr()) return string().split(sep, trim, ignore_quot, dequote);
    else return toString().split(sep, trim, ignore_quot, dequote);
}
/*
slib::stringarray SObjPtr::splitline(bool trim) const {
    if (isStr()) return string().splitline(trim);
    else return toString().splitline(trim);
}
*/
sattribute slib::SObjPtr::parse(const char* sep, const char* delim, const bool trim, const bool ignore_quot, bool dequote) const {
    if (isNull()) return sattribute();
    else if (isStr()) return string().parse(sep, delim, trim, ignore_quot, dequote);
    else return toString().parse(sep, delim, trim, ignore_quot, dequote);
}


slib::SElement slib::SObjPtr::operator[](const int i) {
    if (isStr()) return string().u8iterAt(i);
    else if (isArray()) return array().iterAt(i);

    return slib::SElement();
}
const slib::SElement slib::SObjPtr::operator[](const int i) const {
    if (isStr()) return string().u8iterAt(i);
    else if (isArray()) return array().iterAt(i);

    return slib::SElement();
}
slib::SElement slib::SObjPtr::operator[](const char* s) {
    if (isNull()) instance = new SDictionary();
    if (isDict()) return &dict().iterAt(s)->value();

    return slib::SElement();
}
const slib::SElement slib::SObjPtr::operator[](const char* s) const {
    if (isDict()) return const_cast<SObjPtr*>(&(dict().iterAt(s)->value()));

    return slib::SElement();
}
slib::SElement slib::SObjPtr::operator[](const String& s) {
    if (isNull()) instance = new SDictionary();
    if (isDict()) return &(dict().iterAt(s)->value());

    return slib::SElement();
}
const slib::SElement slib::SObjPtr::operator[](const String& s) const {
    if (isDict()) return const_cast<SObjPtr*>(&(dict().iterAt(s)->value()));

    return slib::SElement();
}
//slib::SObjPtr slib::SObjPtr::operator[](const Range<sinteger>& range) {}
slib::SElement slib::SObjPtr::operator[](const SElement& obj) {
    if (obj.isNum()) return (*this)[obj.intValue()];
    else if (obj.isStr()) return (*this)[(const char*)obj];

    return slib::SElement();
}
const slib::SElement slib::SObjPtr::operator[](const SElement& obj) const {
    if (obj.isNum()) return (*this)[obj.intValue()];
    else if (obj.isStr()) return (*this)[(const char*)obj];

    return slib::SElement();
}
slib::SElement slib::SObjPtr::operator[](const SObjPtr obj) {
    if (obj.isNum()) return (*this)[obj.intValue()];
    else if (obj.isStr()) return (*this)[(const char*)obj];

    return slib::SElement();
}
const slib::SElement slib::SObjPtr::operator[](const SObjPtr obj) const {
    if (obj.isNum()) return (*this)[obj.intValue()];
    else if (obj.isStr()) return (*this)[(const char*)obj];

    return slib::SElement();
}
slib::SIterator slib::SObjPtr::begin() { 
    if (isStr()) return string().u8begin();
    else if (isArray()) return array().begin();
    else if (isDict()) return dict().begin();

    throw Exception();
}
slib::SCIterator slib::SObjPtr::begin() const {
    if (isStr()) return string().u8begin();
    else if (isArray()) return array().begin();
    else if (isDict()) return dict().begin();

    throw Exception();
}
slib::SIterator slib::SObjPtr::end() {
    if (isStr()) return string().u8end();
    else if (isArray()) return array().end();
    else if (isDict()) return dict().end();

    throw Exception();
}
slib::SCIterator slib::SObjPtr::end() const {
    if (isStr()) return string().u8end();
    else if (isArray()) return array().end();
    else if (isDict()) return dict().end();

    throw Exception();
}







slib::SObjPtr& slib::SObjPtr::add(const slib::SObjPtr& o) {
    if (isNull()) { instance = new SArray(); }
    if (isArray()) array().add(o);
    else if (isTable()) table().addRow(o);
    return *this;
    /*
    else if (!ptr.isNull()) {
        if (isNum() && ptr.isNum()) number() += ptr.number();
        else if (isStr()) {
            if (ptr.isStr()) string() += ptr.string();
            else string() += ptr.toString();
        }
        else if (isDict() && ptr.isPair()) dict().insert(ptr.pair());
        else if (isTable()) table().addRow(ptr);
        //else if (isCnvs() && ptr.isFig()) canvas().addFigure(ptr);
        //else if (isMov() && ptr.isImg()) movie().addFrame(ptr);
    }
    */
}
slib::SObjPtr& slib::SObjPtr::append(const slib::SObjPtr& obj) {
    if (isArray()) {
        if (obj.isArray()) array().append(obj.array());
        else array().add(obj);
    }
    return *this;
}
slib::SObjPtr& slib::SObjPtr::append(const slib::SArray& arr) {
    if (isArray()) array().append(arr);

    return *this;
}
slib::SObjPtr& slib::SObjPtr::insert(const int idx, const slib::SObjPtr& obj) {
    if (isArray()) array().insert(idx, obj);

    return *this;
}

/*
slib::SObjPtr& slib::SObjPtr::insert(const slib::MapData<slib::String, slib::SObjPtr> &md) {
    if (isNull()) instance = new SDictionary();
    if (isDict()) dict().insert(md);
    return *this;
}*/
slib::SObjPtr& slib::SObjPtr::set(const slib::String& key, const SObjPtr&obj) {
    if (isNull()) instance = new SDictionary();
    if (isDict()) dict()[key] = obj;
    return *this;
}
slib::SObjPtr& slib::SObjPtr::remove(const size_t off, const size_t len) {
    if (isArray()) array().remove(off, len);

    return *this;
}
slib::SObjPtr& slib::SObjPtr::erase(const SObjPtr& obj) {
    if (isArray()) array().erase(obj);
 
    return *this;
}
void slib::SObjPtr::clear() {
    if (isStr()) string().clear();
    else if (isArray()) array().clear();
    else if (isDict()) dict().clear();
}
slib::sbyte slib::SObjPtr::byteValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().byteValue();
    else if (isStr()) return string().byteValue();
    else return 0;
}
slib::subyte slib::SObjPtr::ubyteValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().ubyteValue();
    else if (isStr()) return string().ubyteValue();
    else return 0;
}
slib::sshort slib::SObjPtr::shortValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().shortValue();
    else if (isStr()) return string().shortValue();
    else return 0;
}
slib::sushort slib::SObjPtr::ushortValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().ushortValue();
    else if (isStr()) return string().ushortValue();
    else return 0;
}
int slib::SObjPtr::intValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().intValue();
    else if (isStr()) return string().intValue();
    else return 0;
}
unsigned slib::SObjPtr::uintValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uintValue();
    else if (isStr()) return string().uintValue();
    else return 0;
}
slib::sinteger slib::SObjPtr::integer() const {
    if (isNull()) return 0;
    else if (isNum()) return number().integer();
    else if (isStr()) return string().integer();
    else return 0;
}
size_t slib::SObjPtr::sizeValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uinteger();
    else if (isStr()) return string().uinteger();
    else return 0;
}
float slib::SObjPtr::floatValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().floatValue();
    else if (isStr()) return string().floatValue();
    else return 0;
}
double slib::SObjPtr::doubleValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().doubleValue();
    else if (isStr()) return string().doubleValue();
    else return 0;
}

slib::SVoid& slib::SObjPtr::any() {
    return *(dynamic_cast<SVoid*>(instance));
}
const slib::SVoid& slib::SObjPtr::any() const {
    return *(dynamic_cast<SVoid*>(instance));
}
slib::SNumber& slib::SObjPtr::number() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isNum()) return *(dynamic_cast<SNumber*>(instance));
    throw CastException(castErrorText(getClass(), "SNumber"));
}
const slib::SNumber& slib::SObjPtr::number() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isNum()) return *(dynamic_cast<SNumber*>(instance));
    throw CastException(castErrorText(getClass(), "SNumber"));
}
slib::SString& slib::SObjPtr::string() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isStr()) return *(dynamic_cast<SString*>(instance));
    throw CastException(castErrorText(getClass(), "SString"));
}
const slib::SString& slib::SObjPtr::string() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isStr()) return *(dynamic_cast<SString*>(instance));
    throw CastException(castErrorText(getClass(), "SString"));
}
slib::SDate& slib::SObjPtr::date() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isDate()) return *(dynamic_cast<SDate*>(instance));
    throw CastException(castErrorText(getClass(), "SDate"));
}
const slib::SDate& slib::SObjPtr::date() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isDate()) return *(dynamic_cast<SDate*>(instance));
    throw CastException(castErrorText(getClass(), "SDate"));
}
slib::SData& slib::SObjPtr::data() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isData()) return *(dynamic_cast<SData*>(instance));
    throw CastException(castErrorText(getClass(), "SData"));
}
const slib::SData& slib::SObjPtr::data() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isData()) return *(dynamic_cast<SData*>(instance));
    throw CastException(castErrorText(getClass(), "SData"));
}
slib::SArray& slib::SObjPtr::array() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isArray()) return *(dynamic_cast<SArray *>(instance));
    throw CastException(castErrorText(getClass(), "SArray"));
}
const slib::SArray& slib::SObjPtr::array() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isArray()) return *(dynamic_cast<const SArray*>(instance));
    throw CastException(castErrorText(getClass(), "SArray"));
}
slib::SDictionary& slib::SObjPtr::dict() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isDict()) return *(dynamic_cast<SDictionary*>(instance));
    throw CastException(castErrorText(getClass(), "SDictionary"));
}
const slib::SDictionary& slib::SObjPtr::dict() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isDict()) return *(dynamic_cast<const SDictionary*>(instance));
    throw CastException(castErrorText(getClass(), "SDictionary"));
}
slib::SText& slib::SObjPtr::text() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isText()) return *(dynamic_cast<SText*>(instance));
    throw CastException(castErrorText(getClass(), "SText"));
}
const slib::SText& slib::SObjPtr::text() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isText()) return *(dynamic_cast<const SText*>(instance));
    throw CastException(castErrorText(getClass(), "SText"));
}

slib::SFile& slib::SObjPtr::file() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isFile()) return *(dynamic_cast<SFile*>(instance));
    throw CastException(castErrorText(getClass(), "SFile"));
}
const slib::SFile& slib::SObjPtr::file() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isFile()) return *(dynamic_cast<SFile*>(instance));
    throw CastException(castErrorText(getClass(), "SFile"));
}
slib::STable& slib::SObjPtr::table() { 
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isTable()) return *(dynamic_cast<STable*>(instance));
    throw CastException(castErrorText(getClass(), "STable"));
}
const slib::STable& slib::SObjPtr::table() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isTable()) return *(dynamic_cast<const STable*>(instance));
    throw CastException(castErrorText(getClass(), "STable"));
}




slib::SFigure& slib::SObjPtr::figure() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isFig()) return *(dynamic_cast<SFigure*>(instance));
    throw CastException(castErrorText(getClass(), "Figure"));
}
const slib::SFigure& slib::SObjPtr::figure() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isFig()) return *(dynamic_cast<const SFigure*>(instance));
    throw CastException(castErrorText(getClass(), "Figure"));
}
slib::SCanvas& slib::SObjPtr::canvas() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isCnvs()) return *(dynamic_cast<SCanvas*>(instance));
    throw CastException(castErrorText(getClass(), "Canvas"));
}
const slib::SCanvas& slib::SObjPtr::canvas() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isCnvs()) return *(dynamic_cast<const SCanvas*>(instance));
    throw CastException(castErrorText(getClass(), "Canvas"));
}
slib::SImage& slib::SObjPtr::image() {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isImg()) return *(dynamic_cast<SImage*>(instance));
    throw CastException(castErrorText(getClass(), "Image"));
}
const slib::SImage& slib::SObjPtr::image() const {
    if (isNull()) throw NullException(nullErrorText("Object instance"));
    if (isImg()) return *(dynamic_cast<const SImage*>(instance));
    throw CastException(castErrorText(getClass(), "Image"));
}



slib::String slib::SObjPtr::getClass() const {
    return instance ? instance->getClass() : "(null)";
}
slib::String slib::SObjPtr::toString(const char *format) const {
    if (instance) {
        return instance->toString(format);
    }
    return "(null)";
}

slib::SObjPtr::operator bool() const { 
    if (isNull()) return false;
    else if (isNum()) return number().boolean();
    else if (isFile()) return (bool)file();
    else return true;
}
slib::SObjPtr::operator slib::sbyte() const { return byteValue(); }
slib::SObjPtr::operator slib::subyte() const { return ubyteValue(); }
slib::SObjPtr::operator slib::sshort() const { return shortValue(); }
slib::SObjPtr::operator slib::sushort() const { return ushortValue(); }
slib::SObjPtr::operator int() const { return intValue(); }
slib::SObjPtr::operator unsigned() const { return uintValue(); }
slib::SObjPtr::operator long long() const { return integer(); }
slib::SObjPtr::operator size_t() const { return sizeValue(); }
slib::SObjPtr::operator float() const { return floatValue(); }
slib::SObjPtr::operator double() const { return doubleValue(); }
slib::SObjPtr::operator const char* () const {
    if (isNull()) return nullptr;
    else if (isStr()) return string().cstr();
    else return toString().cstr();
}
slib::SObjPtr::operator slib::SString& () { return string(); }
slib::SObjPtr::operator slib::SArray& () { return array(); }
slib::SObjPtr::operator slib::SDictionary& () { return dict(); }
slib::SObjPtr::operator slib::STable& () { return table(); }
slib::SObjPtr::operator const slib::SString&() const { return string(); }
slib::SObjPtr::operator const slib::SArray& () const { return array(); }
slib::SObjPtr::operator const slib::SDictionary& () const { return dict(); }
slib::SObjPtr::operator const slib::STable& () const  { return table(); }

bool slib::SObjPtr::operator < (const int n) const {
    if (isNull()) return 0 < n;
    else if (isNum()) return number() < n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator < (const size_t n) const {
    if (isNull()) return 0 < n;
    else if (isNum()) return number() < n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator < (const float n) const {
    if (isNull()) return 0 < n;
    else if (isNum()) return number() < n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator < (const double n) const {
    if (isNull()) return 0 < n;
    else if (isNum()) return number() < n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator < (const SElement& elem) const {
    if (isNull()) return !elem.isNull();
    if (isNum() && elem.isNum()) return number() < elem.number();
    else if (isDate() && elem.isDate()) return date() < elem.date();
    else if (isStr() && elem.isStr()) return string() < elem.string();

    return false;
}
bool slib::SObjPtr::operator < (const SObjPtr& obj) const {
    if (isNull()) return true;
    else if (obj.isNull()) return false;
    if (isNum() && obj.isNum()) return number() < obj.number();
    else if (isDate() && obj.isDate()) return date() < obj.date();
    else if (isStr() && obj.isStr()) return string() < obj.string();
    else {
        
    }
    return false;
}
bool slib::SObjPtr::operator > (const int n) const {
    if (isNull()) return 0 > n;
    else if (isNum()) return number() > n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator > (const size_t n) const {
    if (isNull()) return false;
    else if (isNum()) return number() > n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator > (const float n) const {
    if (isNull()) return 0 > n;
    else if (isNum()) return number() > n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator > (const double n) const {
    if (isNull()) return 0 > n;
    else if (isNum()) return number() > n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator > (const SElement& elem) const {
    if (isNull()) return !elem.isNull();
    if (isNum() && elem.isNum()) return number() > elem.number();
    else if (isDate() && elem.isDate()) return elem.date() < date();
    else if (isStr() && elem.isStr()) return elem.string() < string();

    return false;
}
bool slib::SObjPtr::operator > (const slib::SObjPtr& obj) const { return obj < *this; }
bool slib::SObjPtr::operator <= (const slib::SObjPtr& obj) const { return *this < obj || *this == obj; }
bool slib::SObjPtr::operator >= (const slib::SObjPtr& obj) const { return obj < *this || *this == obj; }
bool slib::SObjPtr::operator == (const char* s) const { 
    if (isStr()) return string() == String(s);
    else return toString() == s;
}
bool slib::SObjPtr::operator == (const int n) const {
    if (isNull()) return n == 0;
    else if (isNum()) return number() == n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator == (const size_t n) const {
    if (isNull()) return n == 0;
    else if (isNum()) return number() == n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator == (const float n) const {
    if (isNull()) return n == 0;
    else if (isNum()) return number() == n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator == (const double n) const {
    if (isNull()) return n == 0;
    else if (isNum()) return number() == n;
    throw CastException(getClass() + " can not cast to SNumber.");
}
bool slib::SObjPtr::operator == (const slib::SElement& elem) const {
    if (elem.isObj()) return *this == elem.object();
    else if (isStr() && elem.isChar()) return string() == elem.toString();
    return false;
}
bool slib::SObjPtr::operator == (const slib::SObjPtr& obj) const {
    if (getClass() == obj.getClass()) {
        if (isNum()) return number() == obj.number();
        else if (isStr()) return string() == obj.string();
        else if (isDate()) return date() == obj.date();
        else if (isData()) return data() == obj.data();
        else if (isArray()) {
            auto eq = true;
            auto& array1 = array(), & array2 = obj.array();
            sfor2(array1, array2) {
                if ($_1.instance != $_2.instance) {
                    eq = false; break;
                }
            }
            return eq;
        }
        else if (isDict()) {
            auto& dict1 = dict(), & dict2 = obj.dict();
            auto keys = dict1.keyset();
            if (keys == dict2.keyset()) {
                auto eq = true;
                sfor(keys) {
                    if (dict1[$_].instance != dict2[$_].instance) {
                        eq = false; break;
                    }
                }
                return eq;
            }
        }
    }
    return false;
}
bool slib::SObjPtr::operator != (const SObjPtr& obj) const { return !(*this == obj); }

bool slib::operator < (const int n, const slib::SObjPtr& obj) { return obj > n; }
bool slib::operator < (const float n, const slib::SObjPtr& obj) { return obj > n; }
bool slib::operator < (const double n, const slib::SObjPtr& obj) { return obj > n; }
bool slib::operator > (const int n, const slib::SObjPtr& obj) { return obj < n; }
bool slib::operator > (const float n, const slib::SObjPtr& obj) { return obj < n; }
bool slib::operator > (const double n, const slib::SObjPtr& obj) { return obj < n; }
bool slib::operator == (const int n, const slib::SObjPtr& obj) { return obj == n; }
bool slib::operator == (const float n, const slib::SObjPtr& obj) { return obj == n; }
bool slib::operator == (const double n, const slib::SObjPtr& obj) { return obj == n; }

bool slib::String::operator==(const SObjPtr& o) const { auto s = o.toString(); return !strcmp(cstr(), s.cstr()); }

slib::SObjPtr slib::smath::abs(const slib::SObjPtr& obj) {
    if (obj.isNull()) return snull;
    else if (obj.isNum()) return obj.number().absolute();
    throw UndefinedException("'smath::abs' function to obtain mathematical absolute value is not defined for '" + obj.getClass() + "' class.");
}

/*
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sobj/stable.h"
#include "sobj/sfunc.h"
#include "sio/sfile.h"
#include "sutil/sdb.h"

#include "smedia/simage.h"
#include "smedia/sfigure.h"
#include "smedia/smovie.h"
*/
/*

SObjPtr &SObjPtr::operator = (SObjPtr &&obj) {
	if (_type == NULL_OBJ) {
		if (!(obj.isNull())) obj.moveTo(*this);
	}
	else if (_type == CHAR_OBJ) {
		if (obj._type == CHAR_OBJ) character() = obj.character();
		else character() = (const char*)obj;
	}
	else { release(); obj.moveTo(*this); }
    return *this;
}

SObjPtr& SObjPtr::operator += (const Time& t) {
	if (isDate()) date() += t;
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
	return *this;
}
SObjPtr &SObjPtr::operator += (const char *s) {
    if (isStr()) string()+=s;
    return *this;
}
SObjPtr &SObjPtr::operator += (const std::string &s) {
    if (isStr()) string()+=s;
    return *this;
}
SObjPtr &SObjPtr::operator += (const String &s) {
    if (isStr()) string()+=s;
    return *this;
}
SObjPtr &SObjPtr::operator += (const SString &s) {
    if (isStr()) string()+=s;
    return *this;
}
SObjPtr &SObjPtr::operator += (const SObjPtr &obj) {
    if (isArray()) array().add(obj);
    if (!obj.isNull()) {
        if (isStr()) {
            if (obj.isStr()) string() += obj.string();
            else string() += obj.toString();
        }
        else if (isNum() && obj.isNum()) number()+=obj.number();
    }
    return *this;
}
SObjPtr& SObjPtr::operator -= (const Time& t) {
	if (isDate()) date() -= t;
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
	return *this;
}
SObjPtr &SObjPtr::operator -= (const SObjPtr &obj) {
	if (isNum() && obj.isNum()) {
		if (isNull()) {
			if (obj.isNull()) number() = 0;
			else number() = -obj.number();
		}
		else if (obj.isNull()) return *this;
		else number() -= obj.number();
	}
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
    return *this;
}
SObjPtr &SObjPtr::operator *= (int i) {
    if (isNum()) number()*=i;
    else if (isStr()) string()*=i;
    return *this;
}
SObjPtr &SObjPtr::operator *= (size_t s) {
    if (isNum()) number()*=s;
    else if (isStr()) string()*=s;
    return *this;
}
SObjPtr &SObjPtr::operator *= (const SObjPtr &obj) {
    if (obj.isNull()) return (*this) *= 0;
    if (isStr()) {
        if (obj.isNum() && obj.number().type() == SNumber::INTEGER) string()*=obj.intValue();
    }
    else if (isNum() && obj.isNum()) number()*=obj.number();
    return *this;
}
SObjPtr &SObjPtr::operator /= (const SObjPtr &obj) {
    if (isNum() && obj.isNum() && !obj.isNull()) number()/=obj.number();
    return *this;
}
SObjPtr &SObjPtr::operator %= (const SObjPtr &obj) {
    if (isNum() && obj.isNum() && !obj.isNull()) number()%=obj.number();
    return *this;
}
SObjPtr &SObjPtr::operator++() {
    if (isNum()) ++number();
    return *this;
}
SObjPtr &SObjPtr::operator--() {
    if (isNum()) --number();
    return *this;
}
SObjPtr SObjPtr::operator-() const {
    if (isNum()) return -number();
    return snull;
}
SObjPtr SObjPtr::operator+(const Time& t) {
	if (isDate()) return date() + t;
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr SObjPtr::operator+(const std::string &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString() + s);
}
SObjPtr SObjPtr::operator+(const String &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString() + s);
}
SObjPtr SObjPtr::operator+(const SString &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString() + s);
}
SObjPtr SObjPtr::operator+(const SObjPtr &obj) const {
    if (_ptr) {
        if (obj._ptr) {
            if (isNum() && obj.isNum()) return number()+obj.number();
            else if (isStr()) {
                if (obj.isStr()) return string()+obj.string();
                else return string()+obj.toString();
            }
            else throw SException(ERR_INFO, SLIB_CAST_ERROR);
        }
        else return *this;
    }
    else {
        if (obj._ptr) return obj;
        else return snull;
    }
}
SObjPtr SObjPtr::operator-(const Time& t) {
	if (isDate()) return date() - t;
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr SObjPtr::operator-(const SObjPtr &obj) const {
	if (isNum() && obj.isNum()) {
		if (isNull()) {
			if (obj.isNull()) return 0;
			else return -obj.number();
		}
		else if (obj.isNull()) return *this;
		else return number() - obj.number();
	}
	else if (isDate() && !isNull() && obj.isDate() && !obj.isNull()) {
		return date() - obj.date();
	}
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr SObjPtr::operator*(int i) const {
    if (isNum()) return SObjPtr(number()*i);
    else if (isStr()) return SObjPtr(string()*i);
    return snull;
}
SObjPtr SObjPtr::operator*(size_t s) const{
    if (isNum()) return SObjPtr(number()*s);
    else if (isStr()) return SObjPtr(string()*s);
    return snull;
}
SObjPtr SObjPtr::operator*(const SObjPtr &obj) const{
    if (obj.isNum()) {
        if (isNum()) return number()*obj.number();
        else if (isStr() && obj.number().type() == SNumber::INTEGER) return string()*obj.intValue();
    }
    return snull;
}
SObjPtr SObjPtr::operator/(const SObjPtr &obj) const {
    if (isNum() && obj.isNum()) return number()/obj.number();
    return snull;
}
SObjPtr SObjPtr::operator%(const SObjPtr &obj) const {
    if (isNum() && obj.isNum()) return number()%obj.number();
    return snull;
}

SObject *SObjPtr::ptr() { return _ptr; }
const SObject *SObjPtr::ptr() const { return _ptr; }
SObject *SObjPtr::operator->() const { return _ptr; }
SObject &SObjPtr::operator*() {
    if (!_ptr) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    return *_ptr;
}
const SObject &SObjPtr::operator*() const {
    if (!_ptr) throw SException(ERR_INFO, SLIB_NULL_ERROR, "_ptr");
    return *_ptr;
}

void SObjPtr::copyTo(SObjPtr &ptr) const {
    ptr._type = _type; ptr._ptr = _ptr; ptr.share();
}
void SObjPtr::moveTo(SObjPtr &ptr) {
    ptr._type = _type; ptr._ptr = _ptr; discard();
}
void SObjPtr::clone(const SObjPtr &obj) {
    if (obj.isHollow()) {
        release(); _type = NULL_OBJ;
    }
    else if (_type == obj._type) {
        switch (_type) {
            case NUMBER_OBJ:
                number() = obj.number();
                break;
            case STRING_OBJ:
                string() = obj.string();
                break;
            case DATE_OBJ:
                date() = obj.date();
                break;
            case DATA_OBJ:
                data() = obj.data();
                break;
            case ARRAY_OBJ:
                array() = obj.array();
                break;
            case DICT_OBJ:
                dict() = obj.dict();
                break;
            default:
                break;
        }
    }
    else {
        if (isChar() && obj.isStr()) character() = obj.string();
        else throw SException(ERR_INFO, SLIB_CAST_ERROR);
    }
}
void SObjPtr::discard() { _ptr = nullptr; _type = NULL_OBJ; }
void SObjPtr::swap(SObjPtr &ptr) {
    auto t = _type; _type = ptr._type; ptr._type = t;
    auto p = _ptr; _ptr = ptr._ptr; ptr._ptr = p;
}
SObjPtr& SObjPtr::operator[](int idx) { return at((sinteger)idx); }
const SObjPtr& SObjPtr::operator[](int idx) const { return at((sinteger)idx); }
SObjPtr& SObjPtr::operator[](size_t idx) { return at((sinteger)idx); }
const SObjPtr& SObjPtr::operator[](size_t idx) const { return at((sinteger)idx); }
SObjPtr& SObjPtr::operator[](sinteger idx) { return at(idx); }
const SObjPtr &SObjPtr::operator[](sinteger idx) const { return at(idx); }
SObjPtr &SObjPtr::operator[](const char *key) { return at(key); }
const SObjPtr &SObjPtr::operator[](const char *key) const { return at(key); }
SObjPtr &SObjPtr::operator[](const std::string &key) { return at(key); }
const SObjPtr &SObjPtr::operator[](const std::string &key) const { return at(key); } 
SObjPtr &SObjPtr::operator[](const String &key) { return at(key); } 
const SObjPtr &SObjPtr::operator[](const String &key) const { return at(key); }
SObjPtr &SObjPtr::operator[](const SString &key) { return at(key); }
const SObjPtr &SObjPtr::operator[](const SString &key) const { return at(key); }
SObjPtr &SObjPtr::operator[](const SObjPtr &obj) { return at(obj); }
const SObjPtr &SObjPtr::operator[](const SObjPtr &obj) const { return at(obj); }
SObjPtr& SObjPtr::at(sinteger idx) {
	if (isNull()) { _type = ARRAY_OBJ; _ptr = new SArray(); }
	if (isStr()) { return string().charAt(idx); }
	if (isArray()) return array()[idx];
	if (isColumn()) return column()[idx];
	//if (isCnvs()) return canvas()[idx];
	//if (isMov()) return movie()[idx];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(sinteger idx) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isStr()) { return string().charAt(idx); }
	if (isArray()) return array()[idx];
	if (isColumn()) return column()[idx];
	//if (isCnvs()) return canvas()[idx];
	//if (isMov()) return movie()[idx];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr& SObjPtr::at(const char* key) {
	if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(const char* key) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr& SObjPtr::at(const ::std::string& key) {
	if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(const ::std::string& key) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr& SObjPtr::at(const String& key) {
	if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(const String& key) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr& SObjPtr::at(const SString& key) {
	if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(const SString& key) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isDict()) return dict()[key];
	//if (isCnvs()) return canvas()[key];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr& SObjPtr::at(const SObjPtr& obj) {
	if (isArray() && obj.isNum()) return array()[obj.intValue()];
	if (isDict() && obj.isStr()) return dict()[obj.str()];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr& SObjPtr::at(const SObjPtr& obj) const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isArray() && obj.isNum()) return array()[obj.intValue()];
	if (isDict() && obj.isStr()) return dict()[obj.str()];
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}

SIterator SObjPtr::begin() {
    if (isStr()) return SIterator(string().ubegin());
    else if (isArray()) return SIterator(array().begin());
    else if (isDict()) return SIterator(dict().begin());
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SCIterator SObjPtr::begin() const {
    if (isStr()) return SCIterator(string().ubegin());
    else if (isArray()) return SCIterator(array().begin());
    else if (isDict()) return SCIterator(dict().begin());
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SIterator SObjPtr::end() {
    if (isStr()) return SIterator(string().uend());
    else if (isArray()) return SIterator(array().end());
    else if (isDict()) return SIterator(dict().end());
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SCIterator SObjPtr::end() const {
    if (isStr()) return SCIterator(string().uend());
    else if (isArray()) return SCIterator(array().end());
    else if (isDict()) return SCIterator(dict().end());
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr SObjPtr::toSObj(const String &s) {
	if (s == "null") return snull;
	else if (s.isQuoted()) return String::dequot(s);
	else if (s.isNumeric()) return SNumber::toNumber(s);
	else return s;
}
sobj SObjPtr::import(sobj info) {
	sobj obj;
	obj.load(info);
	return obj;
}
void SObjPtr::load(sobj info) {
	if (isHollow()) {}
	else if (isStr()) {
		if (isNull()) _ptr = new SString();
		string().load(info["path"]);
	}
	else if (isArray()) {
		if (isNull()) _ptr = new SArray();
		array().load(info["path"]);
	}
	else if (isDict()) {
		if (isNull()) _ptr = new SDictionary();
		dict().load(info["path"]);
	}
}
void SObjPtr::save(sobj info) {
	if (isHollow()) {}
	else if (isStr()) {
		if (isNull()) _ptr = new SString();
		string().save(info["path"].file().path());
	}
	else if (isArray()) {
		if (isNull()) _ptr = new SArray();
		array().save(info["path"].file().path());
	}
	else if (isDict()) {
		if (isNull()) _ptr = new SDictionary();
		dict().save(info["path"].file().path());
	}
}
suint SObjPtr::type() const {
    if (_ptr) {
        if (isNum()) return number().type();
        else if (isColumn()) return column().type();
        else if (isImg()) return image().type();
        else if (isFig()) return figure().type();
    }
    return 0;
}
size_t SObjPtr::size() const {
    if (isNull()) return 0;
    else if (isStr()) return string().size();
    else if (isArray()) return array().size();
    else if (isDict()) return dict().size();
    else if (isColumn()) return column().size();
    //
    return 0;
}
void SObjPtr::resize(size_t s, sobj obj) {
	if (isNull()) return;
	else if (isStr()) return string().resize(s);
	else if (isArray()) return array().resize(s, obj);
	
}
size_t SObjPtr::length() const {
    if (isNull()) return 0;
    else if (isChar()) return character().length();
    else if (isStr()) return string().charCount();
    return 0;
}
bool SObjPtr::empty() const {
    if (isNull()) return true;
    else if (isStr()) return string().empty();
    else if (isArray()) return array().empty();
    else if (isDict()) return dict().empty();
    else if (isColumn()) return column().empty();
    //
    return true;
}
String SObjPtr::name() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isColumn()) return column().name();
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
String SObjPtr::key() const {
    if (isPair()) return pair().key;
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::value() const {
    if (isPair()) return pair().value;
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
bool SObjPtr::beginWith(const char* que) const {
	if (isStr()) return string().beginWith(que);
	else return toString().beginWith(que);
}
bool SObjPtr::endWith(const char* que) const {
	if (isStr()) return string().endWith(que);
	else return toString().endWith(que);
}
String SObjPtr::substring(size_t offset, size_t len) const {
    if (isStr()) return string().substring(offset, len);
    else return toString().substring(offset, len);
}
String SObjPtr::substring(srange range) const {
    if (isStr()) return string().substring(range);
    else return toString().substring(range);
}
SObjPtr SObjPtr::subset(size_t offset, size_t len) const {
    if (isNull()) return snull;
    if (isStr()) return string().substring(offset, len);
    else if (isArray()) return array().subarray(offset, len);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr SObjPtr::subset(srange range) const{
    if (isNull()) return snull;
    if (isStr()) return string().substring(range);
    else if (isArray()) return array().subarray(range);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
String SObjPtr::fill(size_t size, char fill, bool head) {
	if (isStr()) return string().filled(size, fill, head);
	else return toString().filled(size, fill, head);
}
SObjPtr& SObjPtr::sort(std::function<bool(const SObjPtr & o1, const SObjPtr & o2)> Comparer) {
	if (isArray()) array().sort(Comparer);
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
	return *this;
}
SObjPtr& SObjPtr::sortBy(const char* key, slib::ORDER order) {
	if (isArray()) array().sortBy(key, order);
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
	return *this;
}
void SObjPtr::convert(int t) {
    if (isNum()) number().setType(t);
    if (isColumn()) column().convert(t);
    //if (isImg()) image().convert(t);
}
void SObjPtr::transform(int t) {
    if (isStr()) string().transform(t);
    //if (isFig()) figure().transform();
}
void SObjPtr::add(SObjPtr &&ptr) {
    if (isNull()) { _type = ARRAY_OBJ; _ptr = new SArray(); }
    if (isArray()) array().add(std::forward<SObjPtr>(ptr));
    else if (isNum() && ptr.isNum()) number()+=ptr.number();
    else if (isStr()) {
        if (ptr.isStr()) string() += ptr.string();
        else string() += ptr.toString();
    }
    else if (isDict() && ptr.isPair()) dict().insert(ptr.pair());
    else if (isTable()) table().addRow(ptr);
    //else if (isCnvs() && ptr.isFig()) canvas().addFigure(ptr);
    //else if (isMov() && ptr.isImg()) movie().addFrame(ptr);
}

void SObjPtr::insert(int idx, const SObjPtr &ptr) {
    if (isArray()) array().insert(idx, ptr);
    else if (!ptr.isNull()) {
        if (isStr() && ptr.isStr()) string().insert(idx, ptr.str());
        else if (isTable()) table().insertRow(idx, ptr);
        
    }
}
void SObjPtr::insert(const SObjPtr& ptr) {
	if (isArray()) array().add(ptr);
	else if (isDict() && ptr.isPair()) dict().insert(ptr.pair());
	else if (isStr() && ptr.isStr()) string().append(ptr.str());
	else if (isTable()) {
		if (ptr.isColumn()) table().addColumn(ptr.column());

	}
}
void SObjPtr::insert(const kvpair<String, SObjPtr> &pair) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) dict().insert(pair);
}
void SObjPtr::set(int idx, const SObjPtr &ptr) {
    if (isArray()) array().set(idx, ptr);
    else if (!ptr.isNull()) {
        if (isStr() && ptr.isStr()) string().replace(idx, Char::u8size(string().ptr(idx)), ptr);
        else if (isTable()) table().setRow(idx, ptr);
    }
}
void SObjPtr::set(const char *key, const SObjPtr &ptr) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) dict().set(key, ptr);
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::set(const sobj &obj, const SObjPtr& ptr) {
	if (obj.isNum()) set(obj.intValue(), ptr);
	else set(obj.toString().cstr(), ptr);
}
void SObjPtr::removeAt(int idx) {
    if (isStr()) string().removeAt(idx);
    else if (isArray()) array().removeAt(idx);
    else if (isTable()) table().removeRow(idx);
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::remove(const char *key) {
    if (isStr()) string().replace(key, "");
    else if (isDict()) dict().remove(key);
    else if (isTable()) table().removeColumn(table().columnIndex(key));
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::remove(const size_t &off, const size_t &len) {
    if (isStr()) string().remove(off, len);
    else if (isArray()) string().remove(off, len);
    else if (isTable()) table().removeRows(off, len);
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::clear() {
	if (isNull()) return;
    else if (isStr()) string().clear();
    else if (isArray()) array().clear();
    else if (isDict()) dict().clear();
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
bool SObjPtr::contain(const char *que) const {
    if (_ptr) {
        if (isStr()) return string().contain(que);
        else if (isArray()) return array().search(que)!=NOT_FOUND;
    }
    return false;
}
bool SObjPtr::contain(const String &que) const { return contain(que.cstr()); }
bool SObjPtr::contain(const SObjPtr &ptr) const {
    if (isStr()) return string().contain((const char *)ptr);
    else if (isArray()) return array().contain(ptr);
    return false;
}
bool SObjPtr::match(const Regex &rgx, size_t offset) const {
    if (isStr() && !isNull()) return string().match(rgx, offset);
    return false;
}
bool SObjPtr::equal(const Regex &rgx) const {
    if (isStr() && !isNull()) return string().equal(rgx);
    return false;
}
size_t SObjPtr::find(const char *que) const {
    if (isStr()) return string().find(que);
    return NOT_FOUND;
}
size_t SObjPtr::find(const SObjPtr &ptr) const {
    if (isStr()) return string().find(ptr);
    else if (isArray()) return array().find(ptr);
    return NOT_FOUND;
}
size_t SObjPtr::rfind(const char *que) const {
    if (isStr()) return string().rfind(que);
    return NOT_FOUND;
}
size_t SObjPtr::rfind(const SObjPtr &ptr) const{
    if (isStr()) return string().find(ptr);
    else if (isArray()) return array().find(ptr);
    return NOT_FOUND;
}
sizearray SObjPtr::search(const Regex &rgx, size_t offset) const{
    if (isStr()) return string().search(rgx, offset);
    return sizearray();
}
String SObjPtr::toString(const sobj& format) const {
	if (isNull()) return "(null)";
	else if (isStr()) return string();
	else if (isDate()) return format.isNull() ? date().toString() : date().toString(format);
	else if (isDat()) return format.isNull() ? data().toString() : data().toString(format);
    return _ptr->toString();
}
String SObjPtr::getClass() const {
	if (isNull()) return "null";
	return _ptr->getClass();
}
bool SObjPtr::isFunc() const { return _type == FUNC_OBJ; }
bool SObjPtr::isRow() const { return _type == ROW_OBJ; }
bool SObjPtr::isColumn() const { return _type == COLUMN_OBJ; }
bool SObjPtr::isTable() const { return _type == TABLE_OBJ; }
bool SObjPtr::isRecord() const { return _type == RECORD_OBJ; }
bool SObjPtr::isDB() const { return _type == DB_OBJ; }
bool SObjPtr::isNode() const { return _type == NODE_OBJ; }
bool SObjPtr::isColor() const { return _type == COLOR_OBJ; }
bool SObjPtr::isImg() const { return _type == IMAGE_OBJ; }
bool SObjPtr::isFig() const { return _type == FIGURE_OBJ; }
bool SObjPtr::isCnvs() const { return _type == CANVAS_OBJ; }
//bool SObjPtr::isSound() const { return _type == SOUND_OBJ; }
bool SObjPtr::isMov() const { return _type == MOVIE_OBJ; }

//Convert
bool SObjPtr::boolean() const {
    if (isNull()) return false;
    else if (isNum()) return number().boolean();
    else if (isStr()) {
        if (empty()) return false;
        else return string().boolean();
    }
    return true;
}
sbyte SObjPtr::byteValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().byteValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().intValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
subyte SObjPtr::ubyteValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().ubyteValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sshort SObjPtr::shortValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().shortValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().intValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sushort SObjPtr::ushortValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().ushortValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
int SObjPtr::intValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().intValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().intValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
unsigned int SObjPtr::uintValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uintValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
size_t SObjPtr::sizeValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().sizeValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
long SObjPtr::longValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uintValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
unsigned long SObjPtr::ulongValue() const {
    if (isNull()) return 0;
    else if (isNum()) return (unsigned long)number().sizeValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return (unsigned long)string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
long long SObjPtr::llongValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uintValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
unsigned long long SObjPtr::ullongValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().sizeValue();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().sizeValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
float SObjPtr::floatValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().floatValue();
    else if (isStr()) {
        if (empty()) return 0.0f;
        else if (isNumeric()) return string().floatValue();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
double SObjPtr::doubleValue() const {
    if (isNull()) return 0;
    else if (isNum()) return number().doubleValue();
    else if (isStr()) {
        if (empty()) return 0.0;
        else if (isNumeric()) return string().real();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sinteger SObjPtr::integer() const {
    if (isNull()) return 0;
    else if (isNum()) return number().integer();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().integer();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
suinteger SObjPtr::uinteger() const {
    if (isNull()) return 0;
    else if (isNum()) return number().uinteger();
    else if (isStr()) {
        if (empty()) return 0;
        else if (isNumeric()) return string().uinteger();
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sreal SObjPtr::real() const {
    if (isNull()) return 0;
    else if (isNum()) return number().real();
    else if (isStr()) {
        if (empty()) return 0.0;
        else if (isNumeric()) return string().real();
    }
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}

String SObjPtr::str() const {
    if (isNull()) return "";
    else if (isStr()) return string();
    else return toString();
}

SNumber &SObjPtr::number() {
    if (isNull()) *this = SNumber();
    if (isNum()) return *dynamic_cast<SNumber *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SNumber &SObjPtr::number() const {
    if (isNum()) {
        if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
        return *dynamic_cast<SNumber *>(_ptr);
    }
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SChar &SObjPtr::character() {
    if (isNull()) *this = SChar();
    if (isChar()) return *dynamic_cast<SChar *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SChar &SObjPtr::character() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isChar()) return *dynamic_cast<SChar *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SString &SObjPtr::string() {
	if (isNull()) *this = SString();
    if (isStr()) return *dynamic_cast<SString *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SString &SObjPtr::string() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isStr()) return *dynamic_cast<SString *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SDate &SObjPtr::date() {
    if (isNull()) *this = SDate();
    if (isDate()) return *dynamic_cast<SDate *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SDate &SObjPtr::date() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDate()) return *dynamic_cast<SDate *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SData &SObjPtr::data() {
    if (isNull()) *this = SData();
    if (isDat()) return *dynamic_cast<SData *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SData &SObjPtr::data() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDat()) return *dynamic_cast<SData *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SArray &SObjPtr::array() {
    if (isNull()) *this = SArray();
    if (isArray()) return *dynamic_cast<SArray *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SArray &SObjPtr::array() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isArray()) return *dynamic_cast<SArray *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SPair &SObjPtr::pair() {
    if (isNull()) *this = SPair();
    if (isPair()) return *dynamic_cast<SPair *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SPair &SObjPtr::pair() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isPair()) return *dynamic_cast<SPair *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SDictionary &SObjPtr::dict() {
    if (isNull()) *this = SDictionary();
    if (isDict()) return *dynamic_cast<SDictionary *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SDictionary &SObjPtr::dict() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDict()) return *dynamic_cast<SDictionary *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SText& SObjPtr::text() {
	if (isNull()) *this = SText();
	if (isText()) return *dynamic_cast<SText*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SText& SObjPtr::text() const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isText()) return *dynamic_cast<SText*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sio::SFile& SObjPtr::file() {
	if (isNull()) *this = sio::SFile::home();
	if (isFile()) return *dynamic_cast<sio::SFile*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const sio::SFile& SObjPtr::file() const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isFile()) return *dynamic_cast<sio::SFile*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SColumn &SObjPtr::column() {
    if (isNull()) *this = SColumn();
    if (isColumn()) return *dynamic_cast<SColumn *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SColumn &SObjPtr::column() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isColumn()) return *dynamic_cast<SColumn *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
STable &SObjPtr::table() {
    if (isNull()) *this = STable();
    if (isTable()) return *dynamic_cast<STable *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const STable &SObjPtr::table() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isTable()) return *dynamic_cast<STable *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SDataBase& SObjPtr::db() {
	if (isNull()) *this = SDataBase();
	if (isDB()) return *dynamic_cast<SDataBase*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SDataBase& SObjPtr::db() const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isDB()) return *dynamic_cast<SDataBase*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
smedia::SColor& SObjPtr::color() {
	if (isNull()) *this = smedia::SColor();
	if (isColor()) return *dynamic_cast<smedia::SColor*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const smedia::SColor& SObjPtr::color() const {
	if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isColor()) return *dynamic_cast<smedia::SColor*>(_ptr);
	throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
smedia::SImage &SObjPtr::image() {
    if (isNull()) *this = smedia::SImage();
    if (isImg()) return *dynamic_cast<smedia::SImage *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const smedia::SImage &SObjPtr::image() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isImg()) return *dynamic_cast<smedia::SImage *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
smedia::SFigure &SObjPtr::figure() {
    if (isNull()) *this = smedia::SFigure();
    if (isFig()) return *dynamic_cast<smedia::SFigure *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const smedia::SFigure &SObjPtr::figure() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isFig()) return *dynamic_cast<smedia::SFigure *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
smedia::SCanvas &SObjPtr::canvas() {
    if (isNull()) *this = smedia::SCanvas();
    if (isCnvs()) return *dynamic_cast<smedia::SCanvas *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const smedia::SCanvas &SObjPtr::canvas() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isCnvs()) return *dynamic_cast<smedia::SCanvas *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
//smedia::SAudio &audio();
//const smedia::SAudio &audio() const;
smedia::SMovie &SObjPtr::movie() {
    if (isNull()) *this = smedia::SMovie();
    if (isMov()) return *dynamic_cast<smedia::SMovie *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const smedia::SMovie &SObjPtr::movie() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isMov()) return *dynamic_cast<smedia::SMovie *>(_ptr);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
sobj SObjPtr::clone() const {
	if (isNull()) return snull;
	else if (isHollow()) return sobj(_type);
	return sobj(_type, _ptr->clone());
}
SObjPtr::operator SObject *() const { return _ptr; }

SObjPtr::operator sbyte() const { return byteValue(); }
SObjPtr::operator subyte() const { return ubyteValue(); }
SObjPtr::operator sshort() const { return shortValue(); }
SObjPtr::operator sushort() const { return ushortValue(); }
SObjPtr::operator int() const { return intValue(); }
SObjPtr::operator unsigned int() const { return uintValue(); }
SObjPtr::operator size_t() const { return sizeValue(); }
#ifdef WIN64_OS
SObjPtr::operator long() const { return longValue(); }
#ifndef MAC_OS
SObjPtr::operator unsigned long() const { return ulongValue(); }
#endif
#endif
SObjPtr::operator long long() const { return llongValue(); }
#ifdef MAC_OS
SObjPtr::operator unsigned long long() const { return ullongValue(); }
#endif
#ifdef LINUX_OS
SObjPtr::operator sinteger() const { return integer(); }
#endif
SObjPtr::operator float() const { return floatValue(); }
SObjPtr::operator double() const { return doubleValue(); }

SObjPtr::operator const char *() const {
    if (isNull()) return nullptr;
    if (isStr()) return string().cstr();
    else return toString().cstr();
}

bool SObjPtr::operator < (const SObjPtr &obj) const {
    if (isNum() && obj.isNum()) return number() < obj.number();
    if (isStr() && obj.isStr()) return string() < obj.string();
    return _ptr < obj._ptr;
}
bool SObjPtr::operator == (const SNumber &n) const {
    if (isNull()) return n == 0;
    else if (isNum()) return number() == n;
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
bool SObjPtr::operator == (const SObjPtr &obj) const {
    if (_ptr == obj._ptr) return true;
    else {
        if (!_ptr || !obj._ptr) return false;
        if (isNum() && obj.isNum()) return number() == obj.number();
        if (isStr() && obj.isStr()) return string() == obj.string();
    }
    return false;
}
bool SObjPtr::operator == (const char *s) const {
    if (isNull()) return s == nullptr;
    else if (isStr()) return string() == s;
    else return toString() == s;
}
bool SObjPtr::operator != (const SObjPtr &obj) const {
    return !(*this == obj);
}
bool SObjPtr::operator != (const char *s) const {
    return !((*this)==s);
}
String slib::operator+(const char* s, const SObjPtr& obj) {
	return String(s) + obj.toString();
}
String slib::operator+(const ::std::string& s, const SObjPtr& obj) {
	return String(s) + obj.toString();
}
bool slib::operator<(const int& i, const SObjPtr& obj) {
	if (obj.isNum()) return SNumber(i) < obj.number();
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}

slib::SObjPtr slib::smath::absolute(const slib::SObjPtr& obj) {
	if (obj.isNum()) return obj.number().absolute();
	else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
*/
slib::SObjPtr slib::operator+(const char* s, const slib::SObjPtr& obj) {
    return new SString(S(s) + obj.toString());
}