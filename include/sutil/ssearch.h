#ifndef SLIB_SSEARCH_H
#define SLIB_SSEARCH_H

#include "sbasic.h"
#include "sobj/sdict.h"

namespace slib {
	constexpr suinteger LT = 0x0001;
	constexpr suinteger GT = 0x0002;
	constexpr suinteger ELT = 0x0011;
	constexpr suinteger EGT = 0x0012;
	constexpr suinteger EQ = 0x0010;
	constexpr suinteger NOT = 0x0020;
	constexpr suinteger NEQ = 0x0030;
	constexpr suinteger BETWEEN = 0x0003;
	constexpr suinteger CONTAIN = 0x0080;

	constexpr suinteger EXACT = 0x0100;
	constexpr suinteger PREFIX = 0x0200;
	constexpr suinteger POSTFIX = 0x0400;
	constexpr suinteger PARTIAL = 0x0800;
	constexpr suinteger LIKE = 0x1000;
	constexpr suinteger MATCH = 0x2000;

	extern sindex SEARCH_OPERATOR;

	class SLIB_DLL SearchQuery {
	public:
		String key;
		suint operation;
		sobj value;

		SearchQuery();
		SearchQuery(const char* k, const char* op, const sobj& v);
		SearchQuery(const char* k, suint op, const sobj& v);
		SearchQuery(const SearchQuery& que);
		~SearchQuery();
		SearchQuery& operator=(const SearchQuery& que);
	};
	class SLIB_DLL SearchSorter {
	public:
		String key;
		slib::ORDER order;

		SearchSorter();
		SearchSorter(const char* k, slib::ORDER o = ASC);
		SearchSorter(const SearchSorter& sorter);
		~SearchSorter();
		SearchSorter& operator=(const SearchSorter& sorter);
	};

	inline void _addQuery(Array<Array<SearchQuery>> &_queries) {}
	template<class First, class Second, class Third>
	inline void _addQuery(Array<Array<SearchQuery>> &_queries, First &first, Second &second, Third &third) {
		_queries[-1].add(first, second, third);
	}
	template<class First, class Second, class Third, class Fourth, class... Args>
	inline void _addQuery(Array<Array<SearchQuery>>& _queries, First& first, Second& second, Third& third, Fourth &fourth, Args... args) {
		_queries[-1].add(first, second, third);
		if (String(fourth) == "or" || String(fourth) == "OR") _queries.resize(_queries.size() + 1);
		_addQuery(_queries, args...);
	}
	class SLIB_DLL SSearchCondition {
	public:
		Array<Array<SearchQuery>> queries;
		Array<SearchQuery>* query;
		Array<SearchSorter> sorters;
		sranges range;

	public:
		SSearchCondition();
		~SSearchCondition();
		
		void addQuery(const SearchQuery& que);
		template<class... Args>
		void addQuery(Args... args) { addQuery(SearchQuery(args...)); }
		void orQuery();
		template<class... Args>
		void addQueries(Args... args) { _addQuery(queries, args...); }
		void addSorter(const SearchSorter&sorter);
		template<class... Args>
		void addSorter(Args... args) { addSorter(SearchSorter(args...)); }
		void setOffset(size_t o);
		void setLimit(size_t l);
		void setRange(sranges r);
		void reset();
	};

	class SLIB_DLL SCaseCondition {
	public:
		String key, as;
		Array<SearchQuery> queries;
		SArray values;
		sobj exception;

	public:
		SCaseCondition();
		~SCaseCondition();
	};

}

#endif