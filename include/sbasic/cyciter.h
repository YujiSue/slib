#pragma once

#include "sconfig.h"

#define scyc_iter SCycleIterator
#define scyc_citer SCycleCIterator

template<typename T>
class SCycleIterator {
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef std::ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;

private:
	pointer _ptr;
	sint _current;
	size_t _size;

	void _adjust(int i);

public:
	SCycleIterator(pointer p, int c, size_t s);
	SCycleIterator(const SCycleIterator& cit);
	SCycleIterator& operator=(const SCycleIterator& cit);
	~SCycleIterator();
	reference operator *();
	pointer operator ->();
	reference operator[](std::ptrdiff_t diff);
	SCycleIterator& operator ++();
	SCycleIterator operator ++(int i);
	SCycleIterator& operator --();
	SCycleIterator operator --(int i);
	SCycleIterator& operator +=(std::ptrdiff_t diff);
	SCycleIterator& operator -=(std::ptrdiff_t diff);
	SCycleIterator operator +(std::ptrdiff_t diff);
	SCycleIterator operator -(std::ptrdiff_t diff);
	int operator -(SCycleIterator cit);
	int index() const;
	void swap(SCycleIterator cit1, SCycleIterator cit2);
	bool operator ==(const SCycleIterator& cit) const;
	bool operator !=(const SCycleIterator& cit) const;
	bool operator <(const SCycleIterator& cit) const;
	bool operator <=(const SCycleIterator& cit) const;
	bool operator >(const SCycleIterator& cit) const;
	bool operator >=(const SCycleIterator& cit) const;
};

template<typename T>
class SCycleCIterator {
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef const T value_type;
	typedef std::ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;

private:
	pointer _ptr;
	sint _current;
	size_t _size;

	void _adjust(int i);

public:
	SCycleCIterator(pointer p, int c, size_t s);
	SCycleCIterator(const SCycleCIterator& cit);
	SCycleCIterator& operator=(const SCycleCIterator& cit);
	~SCycleCIterator();
	reference operator *() const;
	pointer operator ->() const;
	reference operator[](std::ptrdiff_t diff) const;
	SCycleCIterator& operator ++();
	SCycleCIterator operator ++(int i);
	SCycleCIterator& operator --();
	SCycleCIterator operator --(int i);
	SCycleCIterator& operator +=(std::ptrdiff_t diff);
	SCycleCIterator& operator -=(std::ptrdiff_t diff);
	SCycleCIterator operator +(std::ptrdiff_t diff);
	SCycleCIterator operator -(std::ptrdiff_t diff);
	int operator -(SCycleCIterator cit);
	int index() const;
	void swap(SCycleCIterator cit1, SCycleCIterator cit2);
	bool operator ==(const SCycleCIterator& cit) const;
	bool operator !=(const SCycleCIterator& cit) const;
	bool operator <(const SCycleCIterator& cit) const;
	bool operator <=(const SCycleCIterator& cit) const;
	bool operator >(const SCycleCIterator& cit) const;
	bool operator >=(const SCycleCIterator& cit) const;
};

/*============================================================*/
template<typename T>
void SCycleIterator<T>::_adjust(int i) {
	if (i < 0) do { i += _size; } while (i < 0);
	else i = i % _size;
	_ptr += i - _current;
	_current = i;
}
template<typename T>
SCycleIterator<T>::SCycleIterator(pointer p, int c, size_t s) : _ptr(p), _current(c), _size(s) {}
template<typename T>
SCycleIterator<T>::SCycleIterator(const SCycleIterator& cit) : SCycleIterator<T>(cit._ptr, cit._current, cit._size) {}
template<typename T>
SCycleIterator<T>& SCycleIterator<T>::operator=(const SCycleIterator<T>& cit) {
	_ptr = cit.ptr;
	_current = cit._current;
	_size = cit._size;
	return *this;
}
template<typename T>
SCycleIterator<T>::~SCycleIterator() {}
template<typename T>
T& SCycleIterator<T>::operator *() { return *_ptr; }
template<typename T>
T* SCycleIterator<T>::operator ->() { return _ptr; }
template<typename T>
T& SCycleIterator<T>::operator[](std::ptrdiff_t diff) { _adjust(_current + diff); return *_ptr; }
template<typename T>
SCycleIterator<T>& SCycleIterator<T>::operator ++() { _adjust(_current + 1); return *this; }
template<typename T>
SCycleIterator<T> SCycleIterator<T>::operator ++(int i) { _adjust(_current + i); return *this; }
template<typename T>
SCycleIterator<T>& SCycleIterator<T>::operator --() { _adjust(_current - 1); return *this; }
template<typename T>
SCycleIterator<T> SCycleIterator<T>::operator --(int i) { _adjust(_current - i); return *this; }
template<typename T>
SCycleIterator<T>& SCycleIterator<T>::operator +=(std::ptrdiff_t diff) {
	if (_size == diff) return *this;
	else _adjust(_current + diff);
	return *this;
}
template<typename T>
SCycleIterator<T>& SCycleIterator<T>::operator -=(std::ptrdiff_t diff) {
	if (_size == diff) return *this;
	else _adjust(_current - diff);
	return *this;
}
template<typename T>
SCycleIterator<T> SCycleIterator<T>::operator +(std::ptrdiff_t diff) { SCycleIterator<T> cit = *this; return cit += diff; }
template<typename T>
SCycleIterator<T> SCycleIterator<T>::operator -(std::ptrdiff_t diff) { SCycleIterator<T> cit = *this; return cit -= diff; }
template<typename T>
int SCycleIterator<T>::operator -(SCycleIterator<T> cit) {
	if (_current < cit._current)
		return _current + _size - cit._current;
	else return _current - cit._current;
}
template<typename T>
int SCycleIterator<T>::index() const { return _current; }
template<typename T>
void SCycleIterator<T>::swap(SCycleIterator<T> cit1, SCycleIterator<T> cit2) { T temp = *cit1; *cit1 = *cit2; *cit2 = temp; }
template<typename T>
bool SCycleIterator<T>::operator ==(const SCycleIterator<T>& cit) const { return _ptr == cit._ptr; }
template<typename T>
bool SCycleIterator<T>::operator !=(const SCycleIterator<T>& cit) const { return !(*this == cit); }
template<typename T>
bool SCycleIterator<T>::operator <(const SCycleIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleIterator<T>::operator <=(const SCycleIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleIterator<T>::operator >(const SCycleIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleIterator<T>::operator >=(const SCycleIterator<T>& cit) const { return true; }

template<typename T>
void SCycleCIterator<T>::_adjust(int i) {
	if (i < 0) do { i += _size; } while (i < 0);
	else i = i % _size;
	_ptr += i - _current;
	_current = i;
}
template<typename T>
SCycleCIterator<T>::SCycleCIterator(pointer p, int c, size_t s) : _ptr(p), _current(c), _size(s) {}
template<typename T>
SCycleCIterator<T>::SCycleCIterator(const SCycleCIterator<T>& cit) : SCycleCIterator<T>(cit._ptr, cit._current, cit._size) {}
template<typename T>
SCycleCIterator<T>& SCycleCIterator<T>::operator=(const SCycleCIterator<T>& cit) {
	_ptr = cit._ptr;
	_current = cit._current;
	_size = cit._size;
	return *this;
}
template<typename T>
SCycleCIterator<T>::~SCycleCIterator() {}
template<typename T>
const T& SCycleCIterator<T>::operator *() const { return *_ptr; }
template<typename T>
const T* SCycleCIterator<T>::operator ->() const { return _ptr; }
template<typename T>
const T& SCycleCIterator<T>::operator[](std::ptrdiff_t diff) const { _adjust(_current + diff); return *_ptr; }
template<typename T>
SCycleCIterator<T>& SCycleCIterator<T>::operator ++() { _adjust(_current + 1); return *this; }
template<typename T>
SCycleCIterator<T> SCycleCIterator<T>::operator ++(int i) { _adjust(_current + i); return *this; }
template<typename T>
SCycleCIterator<T>& SCycleCIterator<T>::operator --() { _adjust(_current - 1); return *this; }
template<typename T>
SCycleCIterator<T> SCycleCIterator<T>::operator --(int i) { _adjust(_current - i); return *this; }
template<typename T>
SCycleCIterator<T>& SCycleCIterator<T>::operator +=(std::ptrdiff_t diff) {
	if (_size == diff) return *this;
	else _adjust(_current + diff);
	return *this;
}
template<typename T>
SCycleCIterator<T>& SCycleCIterator<T>::operator -=(std::ptrdiff_t diff) {
	if (_size == diff) return *this;
	else _adjust(_current - diff);
	return *this;
}
template<typename T>
SCycleCIterator<T> SCycleCIterator<T>::operator +(std::ptrdiff_t diff) { SCycleCIterator<T> cit = *this; return cit += diff; }
template<typename T>
SCycleCIterator<T> SCycleCIterator<T>::operator -(std::ptrdiff_t diff) { SCycleCIterator<T> cit = *this; return cit -= diff; }
template<typename T>
int SCycleCIterator<T>::operator -(SCycleCIterator<T> cit) {
	if (_current < cit._current)
		return _current + _size - cit._current;
	else return _current - cit._current;
}
template<typename T>
int SCycleCIterator<T>::index() const { return _current; }
template<typename T>
void SCycleCIterator<T>::swap(SCycleCIterator<T> cit1, SCycleCIterator<T> cit2) {
	T temp = *cit1; *cit1 = *cit2; *cit2 = temp;
}
template<typename T>
bool SCycleCIterator<T>::operator ==(const SCycleCIterator<T>& cit) const { return _ptr == cit._ptr; }
template<typename T>
bool SCycleCIterator<T>::operator !=(const SCycleCIterator<T>& cit) const { return !(*this == cit); }
template<typename T>
bool SCycleCIterator<T>::operator <(const SCycleCIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleCIterator<T>::operator <=(const SCycleCIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleCIterator<T>::operator >(const SCycleCIterator<T>& cit) const { return true; }
template<typename T>
bool SCycleCIterator<T>::operator >=(const SCycleCIterator<T>& cit) const { return true; }