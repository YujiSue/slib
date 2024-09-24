#include "sobj/sobjptr.h"
#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
slib::SElement::SElement() { eclass = E_CLASS::OBJECT; element.obj = nullptr; }
slib::SElement::SElement(SObjPtr* o) { eclass = E_CLASS::OBJECT; element.obj = o; }
slib::SElement::SElement(ArrayIterator<SObjPtr> it) { eclass = E_CLASS::OBJECT; element.obj = it.ptr(); }
slib::SElement::SElement(ArrayCIterator<SObjPtr> it) { eclass = E_CLASS::OBJECT; element.obj = const_cast<SObjPtr *>(it.ptr()); }
slib::SElement::SElement(Utf8Iterator it) { eclass = E_CLASS::CHAR; element.ch = *it; }
slib::SElement::SElement(Utf8CIterator it) { eclass = E_CLASS::CHAR; element.ch = *it; }
slib::SElement::SElement(MapIterator<String, SObjPtr> it) { eclass = E_CLASS::PAIR; element.pair = it.ptr(); }
slib::SElement::SElement(MapCIterator<String, SObjPtr> it) { eclass = E_CLASS::PAIR; element.pair = const_cast<MapData<String, SObjPtr>*>(it.ptr()); }
slib::SElement::SElement(const SElement& elem) {
	eclass = elem.eclass;
	switch (eclass) {
	case E_CLASS::OBJECT:
		element.obj = elem.element.obj;
		break;
	case E_CLASS::CHAR:
		element.ch = elem.element.ch;
		break;
	case E_CLASS::PAIR:
		element.pair = elem.element.pair;
		break;
	default:
		break;
	}
}
slib::SElement::~SElement() {}
slib::SElement& slib::SElement::operator=(const slib::SElement& elem) {
	eclass = elem.eclass;
	switch (eclass) {
	case E_CLASS::OBJECT:
	{
		if (elem.element.obj) {
			if (!element.obj) element.obj = elem.element.obj;
			else if (element.obj != elem.element.obj) (*element.obj) = (*elem.element.obj);
		}
		else throw NullException(nullErrorText("assigned element"));
		break;
	}
	case E_CLASS::CHAR:
	{
		element.ch = elem.element.ch;
		break;
	}
	case E_CLASS::PAIR: 
	{
		if (elem.element.pair) {
			if (!element.pair) element.pair = elem.element.pair;
			else if (element.pair != elem.element.pair) (*element.pair) = (*elem.element.pair);
		}
		else throw NullException(nullErrorText("assigned element"));
		break;
	}
	default:
		break;
	}
	return *this;
}
slib::SElement& slib::SElement::operator=(const slib::SObjPtr& o) {
	if (isObj()) (*element.obj) = o;
	else if (isChar()) element.ch = o.toString();
	return *this;
}

slib::SObjPtr slib::SElement::operator+(const char* s) {
	if (isNull()) return SObjPtr(s);
	else if (isObj()) {
		if (element.obj->isNull()) return SObjPtr(s);
		else return object() + s;
	}
	else throw Exception();
}
slib::SObjPtr& slib::SElement::operator+=(const char* s) {
	if (isNull()) throw Exception();
	if (isObj()) {
		if (element.obj->isNull()) (*element.obj) = "";
		object() += s;
		return object();
	}
	else throw Exception();
}


/*
* Getter
*/
size_t slib::SElement::size() const {
	switch (eclass) {
	case E_CLASS::OBJECT:
		return object().size();
	case E_CLASS::CHAR:
		return character().size();
	default:
		return 0;
	}
}
size_t slib::SElement::length() const { return object().length(); }
slib::String& slib::SElement::key() { return pair().key(); }
const slib::String& slib::SElement::key() const { return pair().key(); }
slib::SObjPtr& slib::SElement::value() { return pair().value (); }
const slib::SObjPtr& slib::SElement::value() const { return pair().value(); }

slib::String slib::SElement::toString(const char* format) const {
	if (isObj()) return element.obj ? element.obj->toString(format) : "";
	else if (isChar()) return element.ch.toString();
	return "";
}

/*
* Setter
*/
void slib::SElement::resize(const size_t sz) { return object().resize(sz); }

/*
* Access
*/
slib::SElement slib::SElement::operator[](const int i) { return object()[i]; }
const slib::SElement slib::SElement::operator[](const int i) const { return object()[i]; }
slib::SElement slib::SElement::operator[](const char* s) { 
	if (isObj() && element.obj->isNull()) element.obj->instance = new SDictionary();
	return object()[s]; 
}
const slib::SElement slib::SElement::operator[](const char* s) const { return object()[s]; }
slib::SElement slib::SElement::operator[](const SElement& e) {
	if (e.isNull()) return object()[0];
	else if (e.isObj() && e.isNum()) return object()[e.intValue()];
	else return object()[(const char*)e];
}
const slib::SElement slib::SElement::operator[](const SElement& e) const {
	if (e.isNull()) return object()[0];
	else if (e.isObj() && e.isNum()) return object()[e.intValue()];
	else return object()[(const char*)e];
}
slib::SIterator slib::SElement::begin() { return object().begin(); }
slib::SCIterator slib::SElement::begin() const { return object().begin(); }
slib::SIterator slib::SElement::end() { return object().end(); }
slib::SCIterator slib::SElement::end() const { return object().end(); }

/*
* Editor
*/
slib::SObjPtr& slib::SElement::add(const SObjPtr& obj) { 
	if (isObj() && element.obj->isNull()) element.obj->instance = new SArray();
	return object().add(obj);
}
slib::SObjPtr& slib::SElement::append(const SObjPtr obj) { return object().append(obj); }
slib::SObjPtr& slib::SElement::append(const SArray& array) { return object().append(array); }
/*slib::SObjPtr& slib::SElement::set(const MapData<slib::String, SObjPtr>& md) { return object().set(md); }
*/
slib::SObjPtr& slib::SElement::set(const slib::String& key, const SObjPtr& obj) { 
	return object().set(key, obj);
}
/*
* Checker
*/
bool slib::SElement::empty() const { return object().empty(); }
bool slib::SElement::hasKey(const char* key) const { return object().hasKey(key); }
stringarray slib::SElement::split(const char* s) const { return object().split(s); }
bool slib::SElement::beginWith(const char* que) const { return object().beginWith(que); }
bool slib::SElement::endWith(const char* que) const { return object().endWith(que); }
bool slib::SElement::include(const SElement& elem) const { return object().include(elem); }
bool slib::SElement::include(const SObjPtr& obj) const { return object().include(obj); }
bool slib::SElement::match(const char* s) const { return object().match(s); }
bool slib::SElement::match(const Regex& reg) const { return object().string().match(reg); }
size_t slib::SElement::indexOf(const SObjPtr& obj) const { return object().indexOf(obj); }
size_t slib::SElement::find(const char* s) const { return object().find(s); }

bool slib::SElement::isNull() const { return eclass == E_CLASS::OBJECT && !element.obj; }
bool slib::SElement::isObj() const { return eclass == E_CLASS::OBJECT; }
bool slib::SElement::isChar() const { return eclass == E_CLASS::CHAR; }
bool slib::SElement::isPair() const { return eclass == E_CLASS::PAIR; }

bool slib::SElement::isNum() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isNum(); }
bool slib::SElement::isStr() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isStr(); }
bool slib::SElement::isDate() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isDate(); }
bool slib::SElement::isData() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isData(); }
bool slib::SElement::isArray() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isArray(); }
bool slib::SElement::isDict() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isDict(); }
//bool slib::SElement::isText() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isText(); }
bool slib::SElement::isFile() const { return eclass == E_CLASS::OBJECT && element.obj && element.obj->isFile(); }

/*
* Cast
*/
slib::sbyte slib::SElement::byteValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->byteValue();
	else if (isChar()) return element.ch.toString().byteValue();
	else throw Exception();
}
slib::subyte slib::SElement::ubyteValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->ubyteValue();
	else if (isChar()) return element.ch.toString().ubyteValue();
	else throw Exception();
}
slib::sshort slib::SElement::shortValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->shortValue();
	else if (isChar()) return element.ch.toString().shortValue();
	else throw Exception();
}
slib::sushort slib::SElement::ushortValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->ushortValue();
	else if (isChar()) return element.ch.toString().ushortValue();
	else throw Exception();
}
int slib::SElement::intValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->intValue();
	else if (isChar()) return element.ch.toString().intValue();
	else throw Exception();
}
unsigned slib::SElement::uintValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->uintValue();
	else if (isChar()) return element.ch.toString().uintValue();
	else throw Exception();
}
slib::sinteger slib::SElement::integer() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->integer();
	else if (isChar()) return element.ch.toString().integer();
	else throw Exception();
}
size_t slib::SElement::sizeValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->sizeValue();
	else if (isChar()) return element.ch.toString().sizeValue();
	else throw Exception();
}
float slib::SElement::floatValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->floatValue();
	else if (isChar()) return element.ch.toString().floatValue();
	else throw Exception();
}
double slib::SElement::doubleValue() const {
	if (isNull()) throw NullException("");
	else if (isObj()) return element.obj->doubleValue();
	else if (isChar()) return element.ch.toString().doubleValue();
	else throw Exception();
}

slib::SObjPtr& slib::SElement::object() { if (isObj()) return *element.obj; else throw Exception(); }
const slib::SObjPtr& slib::SElement::object() const { if (isObj()) return *element.obj; else throw Exception(); }
slib::Char& slib::SElement::character() { if (isChar()) return element.ch; else throw Exception(); }
const slib::Char& slib::SElement::character() const { if (isChar()) return element.ch; else throw Exception(); }
slib::MapData<slib::String, slib::SObjPtr>& slib::SElement::pair() { 
	if (isPair()) return *(element.pair); 
	else throw Exception(); 
}
const slib::MapData<slib::String, slib::SObjPtr>& slib::SElement::pair() const { if (isPair()) return *element.pair; else throw Exception(); }

slib::SNumber& slib::SElement::number() {
	if (isObj() && element.obj) return element.obj->number();
	throw CastException("");
}
const slib::SNumber& slib::SElement::number() const {
	if (isObj() && element.obj) return element.obj->number();
	throw CastException("");
}
slib::SString& slib::SElement::string() {
	if (isObj() && element.obj) return element.obj->string();
	throw CastException("");
}
const slib::SString& slib::SElement::string() const {
	if (isObj() && element.obj) return element.obj->string();
	throw CastException("");
}
slib::SDate& slib::SElement::date() {
	if (isObj() && element.obj) return element.obj->date();
	throw CastException("");
}
const slib::SDate& slib::SElement::date() const {
	if (isObj() && element.obj) return element.obj->date();
	throw CastException("");
}
slib::SData& slib::SElement::data() {
	if (isObj() && element.obj) return element.obj->data();
	throw CastException("");
}
const slib::SData& slib::SElement::data() const {
	if (isObj() && element.obj) return element.obj->data();
	throw CastException("");
}
slib::SArray& slib::SElement::array() {
	if (isObj() && element.obj) return element.obj->array();
	throw CastException("");
}
const slib::SArray& slib::SElement::array() const {
	if (isObj() && element.obj) return element.obj->array();
	throw CastException("");
}
slib::SDictionary& slib::SElement::dict() {
	if (isObj() && element.obj) return element.obj->dict();
	throw CastException("");
}
const slib::SDictionary& slib::SElement::dict() const {
	if (isObj() && element.obj) return element.obj->dict();
	throw CastException("");
}

slib::SFigure& slib::SElement::figure() {
	if (isObj() && element.obj) return element.obj->figure();
	throw CastException("");
}
const slib::SFigure& slib::SElement::figure() const {
	if (isObj() && element.obj) return element.obj->figure();
	throw CastException("");
}
slib::SCanvas& slib::SElement::canvas() {
	if (isObj() && element.obj) return element.obj->canvas();
	throw CastException("");
}
const slib::SCanvas& slib::SElement::canvas() const {
	if (isObj() && element.obj) return element.obj->canvas();
	throw CastException("");
}


slib::SElement::operator bool()const { 
	if (isNull()) return false;
	if (isObj()) {
		if (isNull()) return false;
		else return (bool)(*element.obj);
	}
	return false;
}
slib::SElement::operator slib::sbyte() const { return byteValue(); }
slib::SElement::operator slib::subyte() const { return ubyteValue(); }
slib::SElement::operator slib::sshort() const { return shortValue(); }
slib::SElement::operator slib::sushort() const { return ushortValue(); }
slib::SElement::operator int() const { return intValue(); }
slib::SElement::operator unsigned() const { return uintValue(); }
slib::SElement::operator long long() const { return integer(); }
slib::SElement::operator size_t() const { return sizeValue(); }
slib::SElement::operator float() const { return floatValue(); }
slib::SElement::operator double() const { return doubleValue(); }

slib::SElement::operator const char* ()const {
	if (isObj()) {
		if (isNull()) return nullptr;
		else if (isStr()) return element.obj->string().cstr();
		else return element.obj->toString().cstr();
	}
	else if (isChar()) {
		return element.ch.toString();
	}
	return nullptr;
}

slib::SElement::operator slib::SObjPtr& () { return object(); }
slib::SElement::operator const slib::SObjPtr& () const { return object(); }
slib::SElement::operator slib::Char& () { return character(); }
slib::SElement::operator const slib::Char& () const { return character(); }
slib::SElement::operator slib::MapData<slib::String, slib::SObjPtr>& () { return pair(); }
slib::SElement::operator const slib::MapData<slib::String, slib::SObjPtr>& () const { return pair(); }

slib::SElement::operator const slib::SNumber& () const { return number(); }
slib::SElement::operator slib::SNumber& () { return number(); }
slib::SElement::operator const slib::SString& () const { return string(); }
slib::SElement::operator slib::SString& () { return string(); }
slib::SElement::operator const slib::SDate& () const { return date(); }
slib::SElement::operator slib::SDate& () { return date(); }
slib::SElement::operator const slib::SData& () const { return data(); }
slib::SElement::operator slib::SData& () { return data(); }
slib::SElement::operator const slib::SArray& () const { return array(); }
slib::SElement::operator slib::SArray& () { return array(); }
slib::SElement::operator const slib::SDictionary& () const { return dict(); }
slib::SElement::operator slib::SDictionary& () { return dict(); }

/*
* Compare
*/
bool slib::SElement::operator < (const SElement& elem) const {
	if (eclass != elem.eclass) throw Exception();
	switch (eclass) {
	case E_CLASS::OBJECT:
		return object() < elem.object();
	case E_CLASS::CHAR:
		return character() < elem.character();
	default:
		return false;
	}
}
bool slib::SElement::operator < (const SObjPtr& obj) const {
	if (isObj()) return object() < obj;
	else throw Exception();
}
bool slib::SElement::operator <= (const SElement& elem) const { return (*this < elem) || (*this == elem); }
bool slib::SElement::operator <= (const SObjPtr& obj) const { return (*this < obj) || (*this == obj); }
bool slib::SElement::operator > (const SElement& elem) const { return elem  < *this; }
bool slib::SElement::operator > (const SObjPtr& obj) const { return obj  < *this; }
bool slib::SElement::operator >= (const SElement& elem) const { return (elem  < *this) || (*this == elem); }
bool slib::SElement::operator >= (const SObjPtr& obj) const { return (obj < *this) || (*this == obj); }
bool slib::SElement::operator == (const char* s) const {
	return toString() == s;
}
bool slib::SElement::operator == (const int i) const {
	return number() == i;
}
bool slib::SElement::operator == (const SElement& elem) const {
	if (eclass != elem.eclass) throw Exception();
	switch (eclass) {
	case E_CLASS::OBJECT:
		return object() == elem.object();
	case E_CLASS::CHAR:
		return character() == elem.character();
	default:
		return false;
	}
}
bool slib::SElement::operator == (const SObjPtr& obj) const {
	if (isObj()) return object() == obj;
	else throw Exception();
}
bool slib::SElement::operator != (const SElement& elem) const { return !(*this == elem); }
bool slib::SElement::operator != (const SObjPtr& obj) const { return !(*this == obj); }
slib::SObjPtr slib::operator+(const char* s, const slib::SElement& e) {
	sobj str = SString(s);
	str += e.toString();
	return str;
}
slib::String slib::toString(const slib::SElement& elem, const char* format) {
	return elem.toString(format);
}
std::ostream& slib::operator<<(std::ostream& os, const slib::SElement& elem) {
	return os << elem.toString();
}
slib::String::String(const slib::SElement& elem) : String(elem.toString()) {}
slib::String& slib::String::operator=(const slib::SElement& elem) { 
	*this = elem.toString(); return *this;
}
slib::String& slib::String::operator+=(const slib::SElement& elem) { append(elem.toString()); return *this; }
slib::String& slib::String::operator<<(const slib::SElement& elem) {
	append(elem.toString()); return *this;
}
slib::String slib::String::operator+(const slib::SElement& elem) const { slib::String str(*this); str << elem.toString(); return str; }
