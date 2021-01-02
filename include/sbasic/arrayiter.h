#ifndef SLIB_ARRAYITER_H
#define SLIB_ARRAYITER_H

#include "sconfig.h"

#define sarr_iter slib::SArrayIterator
#define sarr_citer slib::SArrayCIterator

namespace slib {
    template<typename T>
    class SArrayIterator {
		template <typename T_, class M>
		friend class Array;
		template <typename T_>
		friend class CArray;
		template <typename T_>
		friend class RArray;
		template <typename T_>
		friend class UArray;
		template <typename T_, class M>
		friend class BiArray;
		template<class T_, typename Child>
		friend class Node;

		friend class SLIB_DLL SArray;
		friend class SLIB_DLL SData;

	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;

	private:
		pointer _ptr;

	public:
		SArrayIterator();
        SArrayIterator(pointer p);
        SArrayIterator(const SArrayIterator &it);
        ~SArrayIterator();
        SArrayIterator<T> &operator=(const SArrayIterator &it);
        reference operator*();
        pointer operator->();
		pointer ptr();
        reference operator[](std::ptrdiff_t diff);
        SArrayIterator<T> &operator++();
        SArrayIterator<T> operator++(int);
        SArrayIterator<T> &operator--();
        SArrayIterator<T> operator--(int);
        SArrayIterator<T> &operator+=(std::ptrdiff_t diff);
        SArrayIterator<T> &operator-=(std::ptrdiff_t diff);
        SArrayIterator<T> operator+(std::ptrdiff_t diff);
        SArrayIterator<T> operator-(std::ptrdiff_t diff);
        sinteger operator-(const SArrayIterator<T> it) const;
        void swap(SArrayIterator<T> it1, SArrayIterator<T> it2);
        bool operator<(const SArrayIterator<T> &it) const;
        bool operator<=(const SArrayIterator<T> &it) const;
        bool operator>(const SArrayIterator<T> &it) const;
		bool operator>=(const SArrayIterator<T>& it) const;
		bool operator==(const SArrayIterator<T>& it) const;
        bool operator!=(const SArrayIterator<T> &it) const;
    };
    
    template<typename T>
    class SArrayCIterator {
		template <typename T_, class M>
		friend class Array;
		template <typename T_>
		friend class CArray;
		template <typename T_>
		friend class RArray;
		template <typename T_>
		friend class UArray;
		template <typename T_, class M>
		friend class BiArray;
		template<class T_, typename Child>
		friend class Node;
		friend class SLIB_DLL SArray;
		friend class SLIB_DLL SData;

    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;

	private:
		pointer _ptr;
        
    public:
		SArrayCIterator();
        SArrayCIterator(pointer p);
        SArrayCIterator(const SArrayCIterator &it);
        ~SArrayCIterator();
        SArrayCIterator& operator=(const SArrayCIterator &it);
        reference operator *();
        pointer operator ->();
		pointer ptr();
        reference operator [](std::ptrdiff_t diff);
        SArrayCIterator<T> &operator++();
        SArrayCIterator<T> operator++(int);
        SArrayCIterator<T> &operator--();
        SArrayCIterator<T> operator--(int);
        SArrayCIterator<T> &operator+=(std::ptrdiff_t diff);
        SArrayCIterator<T> &operator-=(std::ptrdiff_t diff);
        SArrayCIterator<T> operator+(std::ptrdiff_t diff);
        SArrayCIterator<T> operator-(std::ptrdiff_t diff);
		sinteger operator-(const SArrayCIterator<T> it) const;
        void swap(SArrayCIterator<T> it1, SArrayCIterator<T> it2);
        bool operator<(const SArrayCIterator<T> &it) const;
        bool operator<=(const SArrayCIterator<T> &it) const;
        bool operator>(const SArrayCIterator<T> &it) const;
        bool operator>=(const SArrayCIterator<T> &it) const;
        bool operator==(const SArrayCIterator<T> &it) const;
        bool operator!=(const SArrayCIterator<T> &it) const;
    };
    
    /*============================================================*/
	template<typename T>
	SArrayIterator<T>::SArrayIterator() : _ptr(nullptr) {}
	template<typename T>
    SArrayIterator<T>::SArrayIterator(pointer p) : _ptr(p) {}
    template<typename T>
    SArrayIterator<T>::SArrayIterator(const SArrayIterator<T> &it) : _ptr(it._ptr) {}
    template<typename T>
    SArrayIterator<T>::~SArrayIterator() {}
    template<typename T>
    SArrayIterator<T> &SArrayIterator<T>::operator=(const SArrayIterator<T> &it) { _ptr = it._ptr; return *this; }
    template<typename T>
	T &SArrayIterator<T>::operator*() { return *_ptr; }
    template<typename T>
	T *SArrayIterator<T>::operator->() { return _ptr; }
	template<typename T>
	T* SArrayIterator<T>::ptr() { return _ptr; }
    template<typename T>
	T &SArrayIterator<T>::operator[](std::ptrdiff_t diff) { return *(_ptr+diff); }
    template<typename T>
	SArrayIterator<T> &SArrayIterator<T>::operator++() { ++_ptr; return *this; }
    template<typename T>
	SArrayIterator<T> SArrayIterator<T>::operator++(int) { return SArrayIterator<T>(_ptr+1); }
    template<typename T>
	SArrayIterator<T> &SArrayIterator<T>::operator--() { --_ptr; return *this; }
    template<typename T>
	SArrayIterator<T> SArrayIterator<T>::operator--(int) { return iterator(_ptr-1); }
    template<typename T>
	SArrayIterator<T> &SArrayIterator<T>::operator+=(std::ptrdiff_t diff) { _ptr += diff; return *this; }
    template<typename T>
	SArrayIterator<T> &SArrayIterator<T>::operator-=(std::ptrdiff_t diff) { _ptr -= diff; return *this; }
    template<typename T>
	SArrayIterator<T> SArrayIterator<T>::operator+(std::ptrdiff_t diff) { return SArrayIterator<T>(_ptr+diff); }
    template<typename T>
	SArrayIterator<T> SArrayIterator<T>::operator-(std::ptrdiff_t diff) { return SArrayIterator<T>(_ptr-diff); }
    template<typename T>
	sinteger SArrayIterator<T>::operator-(const SArrayIterator<T> it) const { return _ptr-it._ptr; }
    template<typename T>
	void SArrayIterator<T>::swap(SArrayIterator<T> it1, SArrayIterator<T> it2) {
		auto temp = *it1._ptr; *it1._ptr = *it2._ptr; *it2._ptr = temp;
    }
    template<typename T>
	bool SArrayIterator<T>::operator<(const SArrayIterator<T> &it) const { return _ptr < it._ptr; }
    template<typename T>
	bool SArrayIterator<T>::operator<=(const SArrayIterator<T> &it) const { return _ptr <= it._ptr; }
    template<typename T>
	bool SArrayIterator<T>::operator>(const SArrayIterator<T> &it) const { return  it < *this; }
    template<typename T>
	bool SArrayIterator<T>::operator>=(const SArrayIterator<T> &it) const { return it <= *this; }
    template<typename T>
	bool SArrayIterator<T>::operator==(const SArrayIterator<T> &it) const { return _ptr == it._ptr; }
    template<typename T>
	bool SArrayIterator<T>::operator!=(const SArrayIterator<T> &it) const { return _ptr != it._ptr; }
    
    /*============================================================*/
	template<typename T>
	SArrayCIterator<T>::SArrayCIterator() : _ptr(nullptr) {}
	template<typename T>
    SArrayCIterator<T>::SArrayCIterator(pointer p) : _ptr(p) {}
    template<typename T>
    SArrayCIterator<T>::SArrayCIterator(const SArrayCIterator<T> &it) : _ptr(it._ptr) {}
    template<typename T>
    SArrayCIterator<T>::~SArrayCIterator() {}
    template<typename T>
    SArrayCIterator<T> &SArrayCIterator<T>::operator=(const SArrayCIterator<T> &it) { _ptr = it._ptr; return *this; }
    template<typename T>
	const T &SArrayCIterator<T>::operator*() { return *_ptr; }
    template<typename T>
	const T *SArrayCIterator<T>::operator->() { return _ptr; }
	template<typename T>
	const T* SArrayCIterator<T>::ptr() { return _ptr; }
	template<typename T>
	const T &SArrayCIterator<T>::operator[](std::ptrdiff_t diff) { return *(_ptr+diff); }
    template<typename T>
	SArrayCIterator<T> &SArrayCIterator<T>::operator++() { ++_ptr; return *this; }
    template<typename T>
	SArrayCIterator<T> SArrayCIterator<T>::operator++(int) { return SArrayCIterator(_ptr+1); }
    template<typename T>
	SArrayCIterator<T> &SArrayCIterator<T>::operator--() { --_ptr; return *this; }
    template<typename T>
	SArrayCIterator<T>SArrayCIterator<T>::operator--(int) { return SArrayCIterator(_ptr-1); }
    template<typename T>
	SArrayCIterator<T> &SArrayCIterator<T>::operator+=(std::ptrdiff_t diff) { _ptr += diff; return *this; }
    template<typename T>
	SArrayCIterator<T> &SArrayCIterator<T>::operator-=(std::ptrdiff_t diff) { _ptr -= diff; return *this; }
    template<typename T>
	SArrayCIterator<T> SArrayCIterator<T>::operator+(std::ptrdiff_t diff) { return SArrayCIterator(_ptr+diff); }
    template<typename T>
	SArrayCIterator<T> SArrayCIterator<T>::operator-(std::ptrdiff_t diff) { return SArrayCIterator(_ptr-diff); }
    template<typename T>
	sinteger SArrayCIterator<T>::operator-(const SArrayCIterator<T> it) const { return _ptr-it._ptr; }
    template<typename T>
	void SArrayCIterator<T>::swap(SArrayCIterator<T> it1, SArrayCIterator<T> it2) {
		auto temp = *it1._ptr; *it1._ptr = *it2._ptr; *it2._ptr = temp;
    }
    template<typename T>
	bool SArrayCIterator<T>::operator<(const SArrayCIterator<T> &it) const { return _ptr < it._ptr; }
    template<typename T>
	bool SArrayCIterator<T>::operator<=(const SArrayCIterator<T> &it) const { return _ptr <= it._ptr; }
    template<typename T>
	bool SArrayCIterator<T>::operator>(const SArrayCIterator<T> &it) const { return  it < *this; }
    template<typename T>
	bool SArrayCIterator<T>::operator>=(const SArrayCIterator<T> &it) const { return it <= *this; }
    template<typename T>
	bool SArrayCIterator<T>::operator==(const SArrayCIterator<T> &it) const { return _ptr == it._ptr; }
    template<typename T>
	bool SArrayCIterator<T>::operator!=(const SArrayCIterator<T> &it) const { return _ptr != it._ptr; }
}
#endif
