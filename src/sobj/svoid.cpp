#include "sobj/svoid.h"
#include "sobj/snumber.h"

using namespace slib;

SVoid::SVoid() : _ptr(nullptr), SObject() {}
SVoid::SVoid(void *p) { _ptr = p; }
SVoid::SVoid(const SVoid& ptr) : _ptr(ptr._ptr), SObject() {}
SVoid::~SVoid() { if (_ptr) delete _ptr; }
SVoid& SVoid::operator = (const SVoid& ptr) {
	_ptr = ptr._ptr; return *this;
}
String SVoid::getClass() const { return "void"; }
String SVoid::toString() const { return String("void(") << (suinteger)_ptr << ")"; }
SObject* SVoid::clone() const { return new SVoid(*this); }

bool SVoid::operator < (const SVoid& ptr) const { return _ptr < ptr._ptr; }
bool SVoid::operator == (const SVoid& ptr) const { return _ptr == ptr._ptr; }