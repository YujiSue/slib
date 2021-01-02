#include "sobj/sdata.h"
#include "sutil/scode.h"
#include "sio/sfile.h"

using namespace slib;
using namespace slib::sio;

SData::SData(): ubytearray(), SObject() {}
SData::SData(size_t size, void *dat) : ubytearray(size), SObject() {
    if (dat) ubytearray::copy((subyte *)dat, size);
}
SData::SData(const char* s) : SData(s?(strlen(s)):0, s?(void *)s:nullptr) {}
SData::SData(const slib::String& str) : SData(str.length(), (void*)str.cstr()) {}
SData::SData(const slib::SString& str) : SData(str.length(), (void*)str.cstr()) {}
SData::SData(const sobj &obj) : SData() {
	if (obj.isDat()) *this = obj.data();
    else throw SException(ERR_INFO, SLIB_CAST_ERROR, "obj", CAST_TEXT(obj->getClass(), "SData"));
}
SData::SData(SData&& data) noexcept : ubytearray(std::forward<ubytearray&&>(data)) {}
SData::SData(const SData &data) : SData(data.size(), const_cast<subyte *>(data.ptr())) {}
SData::~SData() {}
SData SData::decode(const char* b64) {
	SData dat;
	/**/
	return dat;
}
SData &SData::operator = (const SData &dat) {
    clear(); if (!dat.empty()) ubytearray::copy(dat.ptr(), dat.size()); return *this;
}
SData &SData::operator = (const sobj &obj) { *this = obj.data(); return *this; }

void SData::load(const char *path) {
    sio::SFile file(path, sio::READ);
    resize(file.size());
    if (!empty()) file.readBytes(ptr(), size());
}
void SData::save(const char *path) {
    sio::SFile file(path, sio::CREATE);
    if (!empty()) file.writeBytes(ptr(), size());
}
void SData::compress() { SCode::compress(*this); }
void SData::expand() { SCode::expand(*this); }
void SData::asString(String& str) {
	if (_capacity) {
		str.reserve(_capacity);
		str.interpret(ptr(), size());
		discard();
	}
	else str.clear();
}
String SData::getClass() const { return "data"; }
String SData::toString() const { return toString("base64"); }
String SData::toString(const char* f) const {
	if (empty()) return "";
	String format = String::lower(f), str;
	if (format == "base64") SCode::encodeBASE64(*this, str);
	else if (format == "binary") {
		str.reserve(size() * 8 + 1);
		sforeach(*this) str << SNumber::toBinary(E_);
	}
	else if (format == "oct") {
		str.reserve(size() * 3 + 1);
		sforeach(*this) str << "\\" << SNumber::toOct(E_);

	}
	else if (format == "hex") {
		str.reserve(size() * 2 + 1);
		sforeach(*this) str << SNumber::toHex(E_);
	}
	return str;
}
SObject * SData::clone() const { return new SData(*this); }

bool SData::operator < (const sobj &obj) const {
    if(obj.isDat()) return (*this) < obj.data();
    return false;
}
bool SData::operator < (const SData &data) const {
    if(ubytearray::size() != data.size()) return ubytearray::size() < data.size();
    return memcmp(ubytearray::ptr(), data.ptr(), ubytearray::size()) < 0;
}
bool SData::operator == (const sobj &obj) const {
    if(obj.isDat()) return (*this) == obj.data();
    return false;
}
bool SData::operator == (const SData &data) const {
    if(ubytearray::size() != data.size()) return false;
    return memcmp(ubytearray::ptr(), data.ptr(), ubytearray::size()) == 0;
}

