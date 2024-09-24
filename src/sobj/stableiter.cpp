#include "sobj/stable.h"
slib::RowIterator::RowIterator() : _row() {}
slib::RowIterator::RowIterator(ArrayIterator<SObjPtr> it, STable* tbl) { _row = SRow(it.ptr(), tbl); }
slib::RowIterator::RowIterator(const RowIterator& it) { _row = it._row; }
slib::RowIterator::~RowIterator() {}
slib::RowIterator& slib::RowIterator::operator=(const RowIterator& it) { _row = it._row; return *this; }
slib::SRow& slib::RowIterator::operator*() { return _row; }
slib::SRow* slib::RowIterator::operator->() { return &_row; }
slib::SRow& slib::RowIterator::operator[](std::ptrdiff_t diff) { _row._values += diff; return _row; }
slib::RowIterator& slib::RowIterator::operator++() { ++_row._values; return *this; }
slib::RowIterator slib::RowIterator::operator++(int) { slib::RowIterator it(*this); ++it; return it; }
slib::RowIterator& slib::RowIterator::operator--() { --_row._values; return *this; }
slib::RowIterator slib::RowIterator::operator--(int) { slib::RowIterator it(*this); --it; return it; }
slib::RowIterator& slib::RowIterator::operator+=(std::ptrdiff_t diff) { 
	sforin(i, 0, diff) { ++(*this); } return *this; 
}
slib::RowIterator& slib::RowIterator::operator-=(std::ptrdiff_t diff) {
	sforin(i, 0, diff) { --(*this); } return *this;
}
slib::RowIterator slib::RowIterator::operator+(std::ptrdiff_t diff) { 
	slib::RowIterator it(*this); it += diff; return it; 
}
slib::RowIterator slib::RowIterator::operator-(std::ptrdiff_t diff) {
	slib::RowIterator it(*this); it -= diff; return it;
}
std::ptrdiff_t slib::RowIterator::operator-(const RowIterator it) const { return _row._values - it._row._values; }
void slib::RowIterator::swap(RowIterator it1, RowIterator it2) {
	Memory<SObjPtr *>::swap(&it1._row._values, &it2._row._values);
	Memory<STable *>::swap(&it1._row._table, &it2._row._table);
}
bool slib::RowIterator::operator<(const RowIterator& it) const { 
	return _row._table == it._row._table ? _row._values < it._row._values : _row._table < it._row._table;
}
bool slib::RowIterator::operator<=(const RowIterator& it) const { return *this < it || *this == it; }
bool slib::RowIterator::operator>(const RowIterator& it) const { return it < *this; }
bool slib::RowIterator::operator>=(const RowIterator& it) const { return it < *this || *this == it; }
bool slib::RowIterator::operator==(const RowIterator& it) const { return _row._values == it._row._values && _row._table == it._row._table; }
bool slib::RowIterator::operator!=(const RowIterator& it) const { return !(*this == it); }