#include "sobj/stable.h"
void slib::SColumn::setTable(STable* tbl) { _table = tbl; }
slib::SColumn::SColumn(const char* s, const SDictionary& attr) {
	name = s; attribute = attr;
}
slib::SColumn::SColumn(const SColumn& column) {
	name = column.name; attribute = column.attribute; _table = column._table; 
}
slib::SColumn::~SColumn() {}
slib::SColumn& slib::SColumn::operator=(const SDictionary& dict) {
	sfor(dict) {
		if ($_.key() == "name") name = $_.value();
		else attribute.insert($_);
	}
	return *this;
}
slib::SColumn& slib::SColumn::operator=(const SColumn& col) {
	name = col.name; attribute = col.attribute; _table = col._table; 
	return *this;
}
size_t slib::SColumn::size() const {
	if (!_table) return 0;
	return _table->nrow();
}
bool slib::SColumn::empty() const { return size() == (size_t)0; }
slib::SObjPtr& slib::SColumn::at(const int i) {
	if (!_table) throw NullException("Table is null.");
	auto cidx = (int)_table->colIndex(name);
	return _table->_rows[cidx].array()[i];
}
const slib::SObjPtr& slib::SColumn::at(const int i) const {
	if (!_table) throw NullException("Table is null.");
	auto cidx = (int)_table->colIndex(name);
	return _table->_rows[cidx].array()[i];
}
slib::SObjPtr& slib::SColumn::operator[](const int i) { return at(i); }
const slib::SObjPtr& slib::SColumn::operator[](const int i) const { return at(i); }
void slib::SColumn::clear(const size_t off, const size_t sz) {
	if (!_table) throw NullException("Table is null.");
	auto beg = _table->begin() + off, end = sz == (size_t)-1 ? _table->end() : _table->begin() + off + sz;
	auto cidx = (int)_table->colIndex(name);
	sforin(it, beg, end) { $_[cidx] = snull; }
}
slib::SObjPtr slib::SColumn::toObj() const {
	sobj obj(attribute);
	obj["name"] = name;
	return obj;
}
