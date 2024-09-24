#ifndef SLIB_CYCITER_H
#define SLIB_CYCITER_H
#include "sbasic/container.h"
namespace slib {
	template<typename T>
	inline void recurring(T** p, T* b, const size_t s) {
		if ((*p) < b) {
			auto diff = (b - (*p)) % s;
			*p = b + s - diff;
		}
		else if (b + s <= (*p)) {
			auto diff = ((*p) - b - s) % s;
			*p = b + diff;
		}
	}
	template<typename T>
	class CycleIterator {
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	private:
		pointer _ptr, _init;
		size_t _size;
	public:
		CycleIterator() : _ptr(nullptr), _init(nullptr), _size(0) {}
		CycleIterator(T* p, ArrayIterator<T> b, size_t s) : _ptr(p), _init(b.ptr()), _size(s) {}
		CycleIterator(const CycleIterator& cit) : _ptr(cit._ptr), _init(cit._init), _size(cit._size) {}
		~CycleIterator() {}
		CycleIterator& operator=(const CycleIterator& cit) {
			_ptr = cit._ptr; _init = cit._init; _size = cit._size; return *this;
		}
		reference operator *() { return *_ptr; }
		pointer operator ->() { return _ptr; }
		reference operator[](std::ptrdiff_t diff) { auto p = _ptr + diff; recurring(&p, _init, _size); return *p; }
		CycleIterator& operator ++() { ++_ptr; recurring(&_ptr, _init, _size); return *this; }
		CycleIterator operator ++(int) { CycleIterator it(*this); return ++it; }
		CycleIterator& operator --() { --_ptr; recurring(&_ptr, _init, _size); return *this; }
		CycleIterator operator --(int) { CycleIterator it(*this); return --it; }
		CycleIterator& operator +=(std::ptrdiff_t diff) { _ptr += diff; recurring(&_ptr, _init, _size); return *this; }
		CycleIterator& operator -=(std::ptrdiff_t diff) { _ptr -= diff; recurring(&_ptr, _init, _size); return *this; }
		CycleIterator operator +(std::ptrdiff_t diff) { CycleIterator it(*this); return it += diff; }
		CycleIterator operator -(std::ptrdiff_t diff) { CycleIterator it(*this); return it -= diff; }
		std::ptrdiff_t operator -(CycleIterator cit) { return (_ptr < cit._ptr ? (_ptr + _size) : _ptr) - cit._ptr; }
		size_t index() const { return _ptr - _init; }
		pointer ptr() { return _ptr; }
		void swap(CycleIterator cit1, CycleIterator cit2) {
			Memory<T>::swap(cit1._ptr, cit2._ptr);
			Memory<T>::swap(cit1._init, cit2._init);
			Memory<size_t>::swap(&cit1._size, &cit2._size);
		}
		bool operator ==(const CycleIterator& cit) const { return _ptr == cit._ptr; }
		bool operator !=(const CycleIterator& cit) const { return _ptr != cit._ptr; }
		bool operator <(const CycleIterator& cit) const { return _ptr < cit._ptr; }
		bool operator <=(const CycleIterator& cit) const { return _ptr <= cit._ptr; }
		bool operator >(const CycleIterator& cit) const { return cit._ptr < _ptr; }
		bool operator >=(const CycleIterator& cit) const { return cit._ptr <= _ptr; }
	};
	template<typename T>
	class CycleCIterator {
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef const T value_type;
		typedef std::ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	private:
		pointer _ptr, _init;
		size_t _size;
	public:
		CycleCIterator() : _ptr(nullptr), _init(nullptr), _size(0) {}
		CycleCIterator(T* p, ArrayCIterator<T> b, size_t s) : _ptr(p), _init(b.ptr()), _size(s) {}
		CycleCIterator(const CycleCIterator& cit) : _ptr(cit._ptr), _init(cit._init), _size(cit._size) {}
		~CycleCIterator() {}
		CycleCIterator& operator=(const CycleCIterator& cit) {
			_ptr = cit._ptr; _init = cit._init; _size = cit._size; return *this;
		}
		reference operator *() { return *_ptr; }
		pointer operator ->() { return _ptr; }
		reference operator[](std::ptrdiff_t diff) { auto p = _ptr + diff; recurring(&p, _init, _size); return *p; }
		CycleCIterator& operator ++() { ++_ptr; recurring(&_ptr, _init, _size); return *this; }
		CycleCIterator operator ++(int) { CycleCIterator it(*this); return ++it; }
		CycleCIterator& operator --() { --_ptr; recurring(&_ptr, _init, _size); return *this; }
		CycleCIterator operator --(int) { CycleCIterator it(*this); return --it; }
		CycleCIterator& operator +=(std::ptrdiff_t diff) { _ptr += diff; recurring(&_ptr, _init, _size); return *this; }
		CycleCIterator& operator -=(std::ptrdiff_t diff) { _ptr -= diff; recurring(&_ptr, _init, _size); return *this; }
		CycleCIterator operator +(std::ptrdiff_t diff) { CycleCIterator it(*this); return it += diff; }
		CycleCIterator operator -(std::ptrdiff_t diff) { CycleCIterator it(*this); return it -= diff; }
		std::ptrdiff_t operator -(CycleCIterator cit) { return (_ptr < cit._ptr ? (_ptr + _size) : _ptr) - cit._ptr; }
		size_t index() const { return _ptr - _init; }
		pointer ptr() { return _ptr; }
		void swap(CycleCIterator cit1, CycleCIterator cit2) {
			Memory<T>::swap(cit1._ptr, cit2._ptr);
			Memory<T>::swap(cit1._init, cit2._init);
			Memory<size_t>::swap(&cit1._size, &cit2._size);
		}
		bool operator ==(const CycleCIterator& cit) const { return _ptr == cit._ptr; }
		bool operator !=(const CycleCIterator& cit) const { return _ptr != cit._ptr; }
		bool operator <(const CycleCIterator& cit) const { return _ptr < cit._ptr; }
		bool operator <=(const CycleCIterator& cit) const { return _ptr <= cit._ptr; }
		bool operator >(const CycleCIterator& cit) const { return cit._ptr < _ptr; }
		bool operator >=(const CycleCIterator& cit) const { return cit._ptr <= _ptr; }
	};
}
#endif