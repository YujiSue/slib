#ifndef SLIB_STABLE_H
#define SLIB_STABLE_H

#include "sbasic/array.h"
#include "smath/geom.h"

namespace slib {
	template<typename T, class ROW, class TABLE>
	class SLIB_DLL TableIterator {
	public:
		typedef ::std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef ::std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	private:
		ArrayIterator<T> _it;
		ROW* _row;
		TABLE* _table;
		smath::DIRECTION _dir;
	public:
		TableIterator();
		TableIterator(ArrayIterator<T> &val, ROW *r, TABLE *t, smath::DIRECTION d);
		TableIterator(const TableIterator<T, ROW, TABLE>& it);
		~TableIterator();
		TableIterator& operator=(const TableIterator<T, ROW, TABLE>& it);
		reference operator*();
		pointer operator->();
		pointer ptr();
		reference operator[](::std::ptrdiff_t diff);
		TableIterator& operator++();
		TableIterator operator++(int);
		TableIterator& operator--();
		TableIterator operator--(int);
		TableIterator& operator+=(::std::ptrdiff_t diff);
		TableIterator& operator-=(::std::ptrdiff_t diff);
		TableIterator operator+(::std::ptrdiff_t diff);
		TableIterator operator-(::std::ptrdiff_t diff);
		sinteger operator-(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator<(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator<=(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator>(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator>=(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator==(const TableIterator<T, ROW, TABLE>& it) const;
		bool operator!=(const TableIterator<T, ROW, TABLE>& it) const;
	};
	/*
	template<typename T>
	class ArrayCIterator {
	public:
		typedef ::std::random_access_iterator_tag iterator_category;
		typedef const T value_type;
		typedef ::std::ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	private:
		pointer _ptr;
	public:
		ArrayCIterator();
		ArrayCIterator(pointer p);
		ArrayCIterator(const ArrayCIterator& it);
		~ArrayCIterator();
		ArrayCIterator& operator=(const ArrayCIterator& it);
		reference operator *();
		pointer operator ->();
		pointer ptr();
		reference operator [](::std::ptrdiff_t diff);
		ArrayCIterator<T>& operator++();
		ArrayCIterator<T> operator++(int);
		ArrayCIterator<T>& operator--();
		ArrayCIterator<T> operator--(int);
		ArrayCIterator<T>& operator+=(::std::ptrdiff_t diff);
		ArrayCIterator<T>& operator-=(::std::ptrdiff_t diff);
		ArrayCIterator<T> operator+(::std::ptrdiff_t diff);
		ArrayCIterator<T> operator-(::std::ptrdiff_t diff);
		sinteger operator-(const ArrayCIterator<T>& it) const;
		bool operator<(const ArrayCIterator<T>& it) const;
		bool operator<=(const ArrayCIterator<T>& it) const;
		bool operator>(const ArrayCIterator<T>& it) const;
		bool operator>=(const ArrayCIterator<T>& it) const;
		bool operator==(const ArrayCIterator<T>& it) const;
		bool operator!=(const ArrayCIterator<T>& it) const;
	};
	*/
	class SLIB_DLL SObjPtr;
	class SLIB_DLL SArray;
	class SLIB_DLL STable;
	using stbl_iter = TableIterator<SObjPtr, SArray, STable>;
	//using stbl_citer = STableCIterator;
}
///@cond
namespace std {
	template<typename T, class ROW, class TABLE>
	void iter_swap(slib::TableIterator<T, ROW, TABLE> it1, slib::TableIterator<T, ROW, TABLE> it2) { auto tmp = *it1; *it1 = *it2; *it2 = tmp; }
}

#endif