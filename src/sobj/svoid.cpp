#include "smath/numeric.h"
#include "sobj/svoid.h"

slib::SVoid::SVoid() : SObject() { _ptr = nullptr; }
slib::SVoid::SVoid(void *p) : SObject() { _ptr = p; }
slib::SVoid::SVoid(const SVoid& ptr) : SObject() { _ptr = ptr._ptr; }
slib::SVoid::~SVoid() { if (_ptr) free(_ptr); }
slib::SVoid& slib::SVoid::operator = (const SVoid& ptr) { _ptr = ptr._ptr; return *this; }
slib::String slib::SVoid::getClass() const { return "void"; }
slib::String slib::SVoid::toString(const char *format) const { 
	return slib::numToString((suinteger)_ptr, format);
}
slib::SObject* slib::SVoid::clone() const { return new SVoid(*this); }
bool slib::SVoid::operator < (const SVoid& ptr) const { return _ptr < ptr._ptr; }
bool slib::SVoid::operator == (const SVoid& ptr) const { return _ptr == ptr._ptr; }