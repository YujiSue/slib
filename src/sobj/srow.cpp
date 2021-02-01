#include "sobj/stable.h"

using namespace slib;
using namespace slib::sio;

void SRow::setTable(STable* tbl) { _table = tbl; }
SRow::SRow() : _table(nullptr), SObject(), Array<SObjPtr>() {}
SRow::SRow(size_t size) : _table(nullptr), SObject(), Array<SObjPtr>(size) {}
SRow::SRow(std::initializer_list<SObjPtr> li) : _table(nullptr), SObject(), Array<SObjPtr>(li.size()) {
	sforeach2(*this, li) E1_ = E2_; 
}
SRow::SRow(const stringarray& strarray) : _table(nullptr), SObject(), Array<SObjPtr>(strarray.size()) {
	sforeach2(*this, strarray) {
		


		
	}
}
SRow::SRow(const sobj& obj) : _table(nullptr), SObject(), Array<SObjPtr>() {
	if (obj.isArray()) {}
	//else if (obj.isRow()) {}
}
SRow::SRow(SRow&& row) noexcept : _table(nullptr), SObject(), Array<SObjPtr>(std::forward<Array<SObjPtr>&&>(row)) {}
SRow::SRow(const SRow& row) : _table(nullptr), SObject(), Array<SObjPtr>(row) {}
SRow::~SRow() {}

SObjPtr& SRow::operator[](const char* s) {
	if (!_table) throw SException(ERR_INFO, SLIB_NULL_ERROR, "table");
	return Array<SObjPtr>::at(_table->columnIndex(s));
}
const SObjPtr& SRow::operator[](const char* s) const {
	if (!_table) throw SException(ERR_INFO, SLIB_NULL_ERROR, "table");
	return Array<SObjPtr>::at(_table->columnIndex(s));
}
sobj SRow::get(sinteger i) const { return Array<SObjPtr>::at(i); }
SArray SRow::get(sinteger c, sinteger w) const {
	SArray values;
	auto p = Array<SObjPtr>::ptr(c);
	sforin(i, 0, w) { values.add(*p); ++p; }
	return values;
}
void SRow::set(sinteger i, sobj v) { Array<SObjPtr>::at(i) = v; }
void SRow::set(sinteger c, sinteger w, sobj v) {
	auto p1 = Array<SObjPtr>::begin() + c;
	if (v.isArray() || v.isRow()) {
		auto p2 = v.begin();
		sforin(i, 0, w) { *p1 = *p2; ++p1; ++p2; }
	}
}
void SRow::clear(sinteger i) { Array<SObjPtr>::at(i) = snull; }
void SRow::clear(sinteger c, sinteger w) {
	auto p = Array<SObjPtr>::ptr(c);
	sforin(i, 0, w) { *p = snull; ++p; }
}
String SRow::getClass() const { return "row"; }
String SRow::toString() const { return slib::toString(*(static_cast<const Array<SObjPtr>*>(this))); }
SObject* SRow::clone() const {
	auto row = new SRow(Array<SObjPtr>::size());
	sforeach2(*row, *this) E1_ = E2_->clone();
	return row;
}