#include "sutil/ssearch.h"

using namespace slib;
sindex slib::SEARCH_OPERATOR = {
	ki("<", slib::LT), ki(">", slib::GT), ki("<=", slib::ELT), ki(">=", slib::EGT),
	ki("==", slib::EQ), ki("not", slib::NOT), ki("!", slib::NOT), ki("!=", slib::NEQ), ki("<>", slib::NEQ),
	ki("in", slib::CONTAIN), ki("glob", slib::MATCH), ki("like", slib::LIKE)
};

SearchQuery::SearchQuery() : operation(0) {}
SearchQuery::SearchQuery(const char* k, const char* op, const sobj& v) : key(k), operation((suint)slib::SEARCH_OPERATOR[op]), value(v) {}
SearchQuery::SearchQuery(const char* k, suint op, const sobj& v) : key(k), operation(op), value(v) {}
SearchQuery::SearchQuery(const SearchQuery& que) : key(que.key), operation(que.operation), value(que.value) {}
SearchQuery::~SearchQuery() {}
SearchQuery& SearchQuery::operator=(const SearchQuery& que) {
	key = que.key; operation = que.operation; value = que.value; return *this;
}
SearchSorter::SearchSorter() : order(DESC) {}
SearchSorter::SearchSorter(const char* k, slib::ORDER o) : key(k), order(o) {}
SearchSorter::SearchSorter(const SearchSorter& sorter) : key(sorter.key), order(sorter.order) {}
SearchSorter::~SearchSorter() {}
SearchSorter& SearchSorter::operator=(const SearchSorter& sorter) {
	key = sorter.key; order = sorter.order; return *this;
}
SSearchCondition::SSearchCondition() { queries.resize(1); query = queries.ptr(); range = Range<suinteger>(0, -1); }
SSearchCondition::~SSearchCondition() {}
void SSearchCondition::addQuery(const SearchQuery& que) { query->add(que); }
void SSearchCondition::orQuery() {
	if (!query->empty()) { queries.resize(queries.size() + 1); query = &queries[-1]; }
}
void SSearchCondition::addSorter(const SearchSorter& sorter) { sorters.add(sorter); }
void SSearchCondition::setOffset(size_t o) { range.begin = o; }
void SSearchCondition::setLimit(size_t l) { range.end = range.begin + l; }
void SSearchCondition::setRange(sranges r) { range = r; }
void SSearchCondition::reset() {
	queries.resize(1);
	query = queries.ptr();
	query->clear();
	sorters.clear();
	range = Range<suinteger>(0, -1);
}