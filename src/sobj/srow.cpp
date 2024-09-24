#include "sobj/stable.h"

void slib::SRow::setTable(STable* tbl) { _table = tbl; }
slib::SRow::SRow() : _table(nullptr), _values(nullptr) {}
slib::SRow::SRow(slib::SObjPtr *r, slib::STable *t) : _table(t), _values(r) {}
/*
slib::SRow::SRow(size_t size) : _table(nullptr), SObject(), Array<SObjPtr>(size) {}
slib::SRow::SRow(std::initializer_list<SObjPtr> li) : _table(nullptr), SObject(), Array<SObjPtr>(li.size()) {
	sforeach2(*this, li) E1_ = E2_; 
}
slib::SRow::SRow(const stringarray& strarray) : _table(nullptr), SObject(), Array<SObjPtr>(strarray.size()) {
	sforeach2(*this, strarray) {
		


		
	}
}
*/
/*
slib::SRow::SRow(const sobj& obj) : _table(nullptr), SObject(), Array<SObjPtr>() {
	if (obj.isArray()) {}
	//else if (obj.isRow()) {}
}
*/
//slib::SRow::SRow(SRow&& row) noexcept : _table(nullptr), SObject(), Array<SObjPtr>(std::forward<Array<SObjPtr>&&>(row)) {}
slib::SRow::SRow(const SRow& row) : _table(row._table), _values(row._values) {}
slib::SRow::~SRow() {}
slib::SRow& slib::SRow::operator=(const SRow& col) { _table = col._table; _values = col._values; return *this; }
const slib::SArray& slib::SRow::values() const { return _values->array(); }
slib::SObjPtr& slib::SRow::operator[](const int i) { return _values->array()[i]; }
const slib::SObjPtr& slib::SRow::operator[](const int i) const { return _values->array()[i]; }
slib::SObjPtr& slib::SRow::operator[](const String& s) { return _values->array()[(int)_table->colIndex(s)]; }
const slib::SObjPtr& slib::SRow::operator[](const String& s) const { return _values->array()[(int)_table->colIndex(s)]; }
bool slib::SRow::hasColumn(const slib::String& s) const { return _table->colIndex(s) != NOT_FOUND; }
slib::SIterator slib::SRow::begin() { if (_values) return _values->begin(); else throw NullException("Row values"); }
slib::SCIterator slib::SRow::begin() const { if (_values) return const_cast<const SObjPtr *>(_values)->begin(); else throw NullException("Row values"); }
slib::SIterator slib::SRow::end() { if (_values) return _values->end(); else throw NullException("Row values"); }
slib::SCIterator slib::SRow::end() const { if (_values) return const_cast<const SObjPtr*>(_values)->end(); else throw NullException("Row values"); }
slib::SObjPtr slib::SRow::toObj() const {
	slib::SObjPtr obj;
	sfor2(_table->columns(), _values->array()) obj.set($_1.name, $_2);
	return obj;
}
/*
slib::SObjPtr& slib::SRow::operator[](const char* s) {
	if (!_table) throw SException(ERR_INFO, SLIB_NULL_ERROR, "table");
	return Array<SObjPtr>::at(_table->columnIndex(s));
}
const slib::SObjPtr& slib::SRow::operator[](const char* s) const {
	if (!_table) throw SException(ERR_INFO, SLIB_NULL_ERROR, "table");
	return Array<SObjPtr>::at(_table->columnIndex(s));
}

sobj slib::SRow::get(sinteger i) const { return Array<SObjPtr>::at(i); }
slib::SArray slib::SRow::get(sinteger c, sinteger w) const {
	SArray values;
	auto p = Array<SObjPtr>::ptr(c);
	sforin(i, 0, w) { values.add(*p); ++p; }
	return values;
}
void slib::SRow::set(sinteger i, sobj v) { Array<SObjPtr>::at(i) = v; }
void slib::SRow::set(sinteger c, sinteger w, sobj v) {
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
slib::String slib::SRow::getClass() const { return "row"; }
slib::String slib::SRow::toString() const { return slib::toString(*(static_cast<const Array<SObjPtr>*>(this))); }
slib::SObject* slib::SRow::clone() const {
	auto row = new SRow(Array<SObjPtr>::size());
	sforeach2(*row, *this) E1_ = E2_->clone();
	return row;
}
*/
slib::String slib::toString(const slib::SRow& row, const char* format) {
	if (!format) return slib::toString(row.values());
	else {
		String f(format);
		if (f == "tsv") return slib::toString(row.values(), TAB);
		else if (f == "list") return slib::toString(row.values(), NL);
		else if (f == "html") {
			String str("<tr>");
			sfor(row.values()) {
				str << "<td>" << $_ << "</td>";
			}
			str << "</tr>";
			return str;
		}
		else return slib::toString(row.values());
	}
}
std::ostream& slib::operator<<(std::ostream& os, const SRow& tbl) {
	return os << tbl.values().toString("csv");
}