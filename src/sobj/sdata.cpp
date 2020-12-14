#include "sobj/sdata.h"
#include "sutil/scode.h"
#include "sio/sfile.h"

using namespace slib;
using namespace slib::sio;

SData::SData(): ubytearray(), SObject() {}
SData::SData(size_t size, void *dat) : ubytearray(size), SObject() {
    if (dat) ubytearray::copy((subyte *)dat, size);
}
/*
SData::SData(const char *s) : SData(SCode::decodeCharCount(s)) {
    SCode::decodeBASE64(s, (void *)ubytearray::ptr(), ubytearray::size());
}
*/
SData::SData(const char* s) : SData(strlen(s), (void *)s) {}
SData::SData(const slib::String& str) : SData(str.length(), (void*)str.cstr()) {}
SData::SData(const slib::SString& str) : SData(str.length(), (void*)str.cstr()) {}
SData::SData(const sobj &obj) : SData() {
    if (obj.isDat()) *this = obj.data();
    else throw SException(ERR_INFO, SLIB_CAST_ERROR, "obj", CAST_TEXT(obj->getClass(), "SData"));
}
SData::SData(const SData &data) : SData(data.size(), const_cast<subyte *>(data.ptr())) {}
SData::~SData() {}
SData& SData::decode(const char* b64) {
	/**/
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
String SData::toString() const {
	if (ubytearray::size()) {
		String str(SCode::base64CharCount(ubytearray::size()), '=');
		SCode::encodeBASE64(ubytearray::ptr(), ubytearray::size(), &str[0]);
		return str;
	}
	else return "";
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

