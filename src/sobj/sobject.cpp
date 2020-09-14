#include "sobj/sobject.h"
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

using namespace slib;

#if defined(WIN32_OS) || defined(WIN64_OS)
#ifdef _MANAGED
using namespace System::Threading;
#endif
#endif

SObject::SObject() {
#ifndef _MANAGED
    _scope.exchange(1);
#else
    Interlocked::Exchange(_scope, 1);
#endif
    
}
SObject::~SObject() {}

void SObject::addScope() { 
#ifndef _MANAGED
	++_scope;
#else
	Interlocked::Increment(_scope);
#endif
}
void SObject::removeScope() { 
#ifndef _MANAGED
	--_scope;
#else
	Interlocked::Decrement(_scope);
#endif
}
bool SObject::scoped() const  {
#ifndef _MANAGED
	return 0 < _scope.load();
#else
	return 0 < _scope;
#endif
}

SObjPtr::SObjPtr() : _type(NULL_OBJ), _ptr(nullptr) {}
SObjPtr::SObjPtr(OBJ_TYPE ot) : _type(ot), _ptr(nullptr) {}
SObjPtr::SObjPtr(bool b) : _type(NUMBER_OBJ), _ptr(new SNumber(b)) {}
SObjPtr::SObjPtr(sbyte i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
SObjPtr::SObjPtr(subyte ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
SObjPtr::SObjPtr(short i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
SObjPtr::SObjPtr(unsigned short ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
SObjPtr::SObjPtr(int i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
SObjPtr::SObjPtr(unsigned int ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
SObjPtr::SObjPtr(size_t ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
#if defined(WIN64_OS)
SObjPtr::SObjPtr(long i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
SObjPtr::SObjPtr(unsigned long ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
#endif
SObjPtr::SObjPtr(long long i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
#if defined(MAC_OS)
SObjPtr::SObjPtr(unsigned long long ui) : _type(NUMBER_OBJ), _ptr(new SNumber(ui)) {}
#endif
#ifdef LINUX_OS
SObjPtr::SObjPtr(sinteger i) : _type(NUMBER_OBJ), _ptr(new SNumber(i)) {}
#endif
SObjPtr::SObjPtr(float f) : _type(NUMBER_OBJ), _ptr(new SNumber(f)) {}
SObjPtr::SObjPtr(double d) :_type(NUMBER_OBJ), _ptr(new SNumber(d)) {}
SObjPtr::SObjPtr(const smath::Fraction<sint> &frac) : _type(NUMBER_OBJ), _ptr(new SNumber(frac)) {}
SObjPtr::SObjPtr(const smath::Complex<float> &comp) :_type(NUMBER_OBJ), _ptr(new SNumber(comp)) {}
SObjPtr::SObjPtr(const char *s) : _type(STRING_OBJ), _ptr(new SString(s)) {}
SObjPtr::SObjPtr(const std::string &str) : _type(STRING_OBJ), _ptr(new SString(str)) {}
SObjPtr::SObjPtr(String &&str) : _type(STRING_OBJ), _ptr(new SString(std::forward<SString>(str))) {}
SObjPtr::SObjPtr(const String &str) : _type(STRING_OBJ), _ptr(new SString(str)) {}
SObjPtr::SObjPtr(std::initializer_list<SObjPtr> li) : _type(ARRAY_OBJ), _ptr(new SArray(li)) {}
SObjPtr::SObjPtr(std::initializer_list<std::pair<String, SObjPtr>> li)
    : _type(DICT_OBJ), _ptr(new SDictionary(li)) {}
SObjPtr::SObjPtr(std::initializer_list<kvpair<String, SObjPtr>> li)
    : _type(DICT_OBJ), _ptr(new SDictionary(li)) {}
SObjPtr::SObjPtr(const SNumber &num) : _type(NUMBER_OBJ), _ptr(new SNumber(num)) {}
SObjPtr::SObjPtr(const SChar &ch) : _type(CHAR_OBJ), _ptr(new SChar(ch)) {}
SObjPtr::SObjPtr(const SString &str) : _type(STRING_OBJ), _ptr(new SString(str)) {}
SObjPtr::SObjPtr(const SDate &date) : _type(DATE_OBJ), _ptr(new SDate(date)) {}
SObjPtr::SObjPtr(const SData &dat) : _type(DATA_OBJ), _ptr(new SData(dat)) {}
SObjPtr::SObjPtr(const SArray &array) : _type(ARRAY_OBJ), _ptr(new SArray(array)) {}
SObjPtr::SObjPtr(const SPair &pair) : _type(PAIR_OBJ), _ptr(new SPair(pair)) {}
SObjPtr::SObjPtr(const SDictionary &dict) : _type(DICT_OBJ), _ptr(new SDictionary(dict)) {}
SObjPtr::SObjPtr(const SText& txt) : _type(TEXT_OBJ), _ptr(new SText(txt)) {}
SObjPtr::SObjPtr(const SColumn& col) : _type(COLUMN_OBJ), _ptr(new SColumn(col)) {}
SObjPtr::SObjPtr(const STable &tbl) : _type(TABLE_OBJ), _ptr(new STable(tbl)) {}
SObjPtr::SObjPtr(const SDataBase& db) : _type(DB_OBJ), _ptr(new SDataBase(db.path())) {}
SObjPtr::SObjPtr(const sio::SFile &file) : _type(FILE_OBJ), _ptr(new SFile(file)) {}
SObjPtr::SObjPtr(const smedia::SColor& col) : _type(COLOR_OBJ), _ptr(new smedia::SColor(col)) {}
SObjPtr::SObjPtr(const smedia::SImage &img) : _type(IMAGE_OBJ), _ptr(new smedia::SImage(img)) {}
SObjPtr::SObjPtr(const smedia::SFigure &fig)
    : _type(FIGURE_OBJ), _ptr(new smedia::SFigure(fig)) {}
SObjPtr::SObjPtr(const smedia::SCanvas &cnvs)
    : _type(CANVAS_OBJ), _ptr(new smedia::SCanvas(cnvs)) {}
//SObjPtr::SObjPtr(const smedia::SAudio &aud) : _type(AUDIO_OBJ), _ptr(new smedia::SAudio(aud)) {}
SObjPtr::SObjPtr(const smedia::SMovie &mov)
    : _type(TABLE_OBJ), _ptr(new smedia::SMovie(mov)) {}
SObjPtr::SObjPtr(SObject *so) {
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
		else if (instanceOf<smedia::SFigure> (so)) _type = FIGURE_OBJ;
		else if (instanceOf<smedia::SCanvas>(so)) _type = CANVAS_OBJ;

		else if (instanceOf<smedia::SMovie>(so)) _type = MOVIE_OBJ;


	}
	else _type = NULL_OBJ;
}
SObjPtr::SObjPtr(SObjPtr &&obj) : _type(obj._type), _ptr(obj._ptr) { obj.discard(); }
SObjPtr::SObjPtr(const SObjPtr &obj) : _type(obj._type), _ptr(obj._ptr) { share(); }
SObjPtr::~SObjPtr() { release(); }

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
SObjPtr &SObjPtr::operator = (const SObjPtr &obj) {
	if (_type == NULL_OBJ) {
		if (!(obj.isNull())) obj.copyTo(*this);
	}
	else if (_type == CHAR_OBJ) {
		if (obj._type == CHAR_OBJ) character() = obj.character();
		else character() = (const char*)obj;
	}
    else { release(); obj.copyTo(*this); }
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
SObjPtr &SObjPtr::operator -= (const SObjPtr &obj) {
    if (isNum() && obj.isNum() & !obj.isNull()) number()-=obj.number();
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

SObjPtr SObjPtr::operator+(const char *s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString())+s;
}
SObjPtr SObjPtr::operator+(const std::string &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString())+s;
}
SObjPtr SObjPtr::operator+(const String &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString())+s;
}
SObjPtr SObjPtr::operator+(const SString &s) const {
    if (isNull()) return SObjPtr("")+=s;
    if (isStr()) return SObjPtr(string())+=s;
    else return SObjPtr(toString())+s;
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
SObjPtr SObjPtr::operator-(const SObjPtr &obj) const {
    if (isNum() && obj.isNum() && !obj.isNull()) return number()-obj.number();
    return snull;
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
    if (!_ptr) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    return *_ptr;
}
void SObjPtr::share() { if (_ptr) _ptr->addScope(); }
void SObjPtr::release() {
    if (_ptr) {
        _ptr->removeScope();
        if (!(_ptr->scoped())) {
            delete _ptr; discard();
        }
    }
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
SObjPtr &SObjPtr::operator[](int idx) {
    if (isNull()) { _type = ARRAY_OBJ; _ptr = new SArray(); }
	if (isStr()) { return string().charAt(idx); }
    if (isArray()) return array()[idx];
    if (isTable()) return table()[idx];
    //if (isCnvs()) return canvas()[idx];
    //if (isMov()) return movie()[idx];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](int idx) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
	if (isStr()) { return string().charAt(idx); }

	
	if (isArray()) return array()[idx];
    if (isTable()) return table()[idx];
    //if (isCnvs()) return canvas()[idx];
    //if (isMov()) return movie()[idx];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr &SObjPtr::operator[](const char *key) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](const char *key) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr &SObjPtr::operator[](const std::string &key) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](const std::string &key) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr &SObjPtr::operator[](const String &key) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](const String &key) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr &SObjPtr::operator[](const SString &key) {
    if (isNull()) { _type = DICT_OBJ; _ptr = new SDictionary(); }
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](const SString &key) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isDict()) return dict()[key];
    //if (isCnvs()) return canvas()[key];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
SObjPtr &SObjPtr::operator[](const SObjPtr &obj) {
    
    
    if (isArray() && obj.isNum()) return array()[obj.intValue()];
    if (isDict() && obj.isStr()) return dict()[obj.str()];
    /*
    if (isTable()) {
        if (obj.isNum()) return table()[obj.intValue()];
        if (obj.isStr()) return table()[obj.str()];
    }
     */
    /*
    if (isCnvs()) {
        if (obj.isNum()) return canvas()[obj.intValue()];
        if (obj.isStr()) return canvas()[obj.str()];
    }
     */
    //if (isMov()) return movie()[obj.intValue()];
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::operator[](const SObjPtr &obj) const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isArray() && obj.isNum()) return array()[obj.intValue()];
    if (isDict() && obj.isStr()) return dict()[obj.str()];
    /*
     if (isTable()) {
     if (obj.isNum()) return table()[obj.intValue()];
     if (obj.isStr()) return table()[obj.str()];
     }
     */
    /*
     if (isCnvs()) {
     if (obj.isNum()) return canvas()[obj.intValue()];
     if (obj.isStr()) return canvas()[obj.str()];
     }
     */
    //if (isMov()) return movie()[obj.intValue()];
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
const String &SObjPtr::name() const {
    if (isNull()) throw SException(ERR_INFO, SLIB_NULL_ERROR);
    if (isColumn()) return column().name();
    if (isTable()) return table().name();
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const String &SObjPtr::key() const {
    if (isPair()) return pair().key;
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
const SObjPtr &SObjPtr::value() const {
    if (isPair()) return pair().value;
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
bool SObjPtr::hasKey(const char *key) const {
    if (isDict()) return dict().hasKey(key);
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
stringarray SObjPtr::keyset() const {
    if (isDict()) return dict().keyset();
    throw SException(ERR_INFO, SLIB_CAST_ERROR);
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
stringarray SObjPtr::split(const char *sep, bool trim) const {
    if (isStr()) return string().split(sep, trim);
    else return toString().split(sep, trim);
}
stringarray SObjPtr::splitline(bool trim) const {
    if (isStr()) return string().splitline(trim);
    else return toString().splitline(trim);
}
sattribute SObjPtr::parse(const char *sep, const char *part, bool trim) const {
    if (isStr()) return string().parse(sep, part, trim);
    else return toString().parse(sep, part, trim);
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
    else if (isColumn()) column().add(std::forward<SObjPtr>(ptr));
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
void SObjPtr::add(const SObjPtr &ptr) {
    if (isNull()) { _type = ARRAY_OBJ; _ptr = new SArray(); }
    if (isArray()) array().add(ptr);
    else if (isColumn()) column().add(ptr);
    else if (!ptr.isNull()) {
        if (isNum() && ptr.isNum()) number()+=ptr.number();
        else if (isStr()) {
            if (ptr.isStr()) string() += ptr.string();
            else string() += ptr.toString();
        }
        else if (isDict() && ptr.isPair()) dict().insert(ptr.pair());
        else if (isTable()) table().addRow(ptr);
        //else if (isCnvs() && ptr.isFig()) canvas().addFigure(ptr);
        //else if (isMov() && ptr.isImg()) movie().addFrame(ptr);
    }
}
void SObjPtr::insert(int idx, const SObjPtr &ptr) {
    if (isArray()) array().insert(idx, ptr);
    else if (isColumn()) column().insert(idx, ptr);
    else if (!ptr.isNull()) {
        if (isStr() && ptr.isStr()) string().insert(idx, ptr.str());
        else if (isTable()) table().insertRow(idx, ptr);
        
    }
}
void SObjPtr::insert(const SObjPtr& ptr) {
	if (isArray()) array().add(ptr);
	else if (isDict() && ptr.isPair()) dict().insert(ptr.pair());
	else if (isStr() && ptr.isStr()) string().append(ptr.str());
	else if (isColumn()) column().add(ptr);
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
    else if (isColumn()) column().set(idx, ptr);
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
    else if (isColumn()) column().removeAt(idx);
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
    else if (isColumn()) column().remove(off, len);
    else if (isTable()) table().removeRows(off, len);
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::remove(const srange &range) {
    if (isStr()) string().remove(range);
    else if (isArray()) string().remove(range);
    else if (isColumn()) column().remove(range);
    else if (isTable()) table().removeRows(range);
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
void SObjPtr::clear() {
	if (isNull()) return;
    else if (isStr()) string().clear();
    else if (isArray()) array().clear();
    else if (isDict()) dict().clear();
    else if (isColumn()) column().clear();
    else throw SException(ERR_INFO, SLIB_CAST_ERROR);
}
bool SObjPtr::contain(const char *que) const {
    if (_ptr) {
        if (isStr()) return string().contain(que);
        else if (isArray()) return array().search(que)!=NOT_FOUND;
        else if (isColumn()) return column().search(que)!=NOT_FOUND;
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
String SObjPtr::toString() const {
    if (isNull()) return "(null)";
    else if (isStr()) return string();
    return _ptr->toString();
}

bool SObjPtr::isNull() const { return !_ptr; }
bool SObjPtr::isNumeric() const {
    if (isNull()) return false;
    if (isNum()) return true;
    if (isStr()) return string().isNumeric();
    else return toString().isNumeric();
}
bool SObjPtr::isHollow() const { return _type == NULL_OBJ; }
bool SObjPtr::isNum() const { return _type == NUMBER_OBJ; }
bool SObjPtr::isChar() const { return _type == CHAR_OBJ; }
bool SObjPtr::isStr() const { return _type == STRING_OBJ; }
bool SObjPtr::isDate() const { return _type == DATE_OBJ; }
bool SObjPtr::isDat() const { return _type == DATA_OBJ; }
bool SObjPtr::isArray() const { return _type == ARRAY_OBJ; }
bool SObjPtr::isPair() const { return _type == PAIR_OBJ; }
bool SObjPtr::isDict() const { return _type == DICT_OBJ; }
bool SObjPtr::isText() const { return _type == TEXT_OBJ; }
bool SObjPtr::isFile() const { return _type == FILE_OBJ; }
bool SObjPtr::isFunc() const { return _type == FUNC_OBJ; }
bool SObjPtr::isColumn() const { return _type == COLUMN_OBJ; }
bool SObjPtr::isTable() const { return _type == TABLE_OBJ; }
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

SObjPtr::operator bool() const { return boolean(); }
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

