#include "sobj/sobject.h"
slib::SObject::SObject() { _scope.exchange(1); }
slib::SObject::~SObject() {}
void slib::SObject::addScope() { ++_scope; }
void slib::SObject::removeScope() { --_scope; }
bool slib::SObject::scoped() const { return 0 < _scope.load(); }
void slib::SObject::swapScope(slib::SObject& obj) {
	auto tmp = _scope.load();
	_scope.exchange(obj._scope.load());
	obj._scope.exchange(tmp);
}
