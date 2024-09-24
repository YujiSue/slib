#ifndef SLIB_ARRAYITER_H
#define SLIB_ARRAYITER_H
#include "sbasic/container.h"
#include "sbasic/ptr.h"
namespace slib {
    /**
    * @class ArrayIterator
    * \~english @brief Array's iterator class
    * \~japanese @brief 配列イテレータクラス
    */
    template<typename T>
    class ArrayIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
    private:
        pointer _ptr;
    public:
        ArrayIterator() : _ptr(nullptr) {}
        ArrayIterator(pointer p) : _ptr(p) {}
        ArrayIterator(const ArrayIterator<T>& it) : _ptr(it._ptr) {}
        ~ArrayIterator() {}
        ArrayIterator<T>& operator=(const ArrayIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator*() const { return *_ptr; }
        pointer operator->() { return _ptr; }
        pointer ptr() { return _ptr; }
        reference operator[](std::ptrdiff_t diff) { return *_ptr[diff]; }
        ArrayIterator<T>& operator++() { 
            if (_ptr) ++_ptr;
            return *this; 
        }
        ArrayIterator<T> operator++(int) { ArrayIterator<T> it(*this); ++it; return it; }
        ArrayIterator<T>& operator--() { 
            if (_ptr) --_ptr; 
            return *this; 
        }
        ArrayIterator<T> operator--(int) { ArrayIterator<T> it(*this); --it; return it; }
        ArrayIterator<T>& operator+=(std::ptrdiff_t diff) { 
            if (_ptr) _ptr += diff; 
            return *this; 
        }
        ArrayIterator<T>& operator-=(std::ptrdiff_t diff) { 
            if (_ptr)_ptr -= diff; 
            return *this; 
        }
        ArrayIterator<T> operator+(std::ptrdiff_t diff) { ArrayIterator<T> it(*this); it += diff; return it; }
        ArrayIterator<T> operator-(std::ptrdiff_t diff) { ArrayIterator<T> it(*this); it -= diff; return it; }
        difference_type operator-(const ArrayIterator<T> it) const { return _ptr - it._ptr; }
        void swap(ArrayIterator<T> it1, ArrayIterator<T> it2) { auto tmp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = tmp; }
        bool operator<(const ArrayIterator<T>& it) const { return _ptr < it._ptr; }
        bool operator<=(const ArrayIterator<T>& it) const { return _ptr <= it._ptr; }
        bool operator>(const ArrayIterator<T>& it) const { return _ptr > it._ptr; }
        bool operator>=(const ArrayIterator<T>& it) const { return _ptr >= it._ptr; }
        bool operator==(const ArrayIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const ArrayIterator<T>& it) const { return _ptr != it._ptr; }
    };
    /**
    * @class ArrayCIterator
    * \~english @brief Arrray's iterator class (const)
    * \~japanese @brief 配列イテレータクラス (const)
    */
    template<typename T>
    class ArrayCIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
    private:
        pointer _ptr;
    public:
        ArrayCIterator() : _ptr(nullptr) {}
        ArrayCIterator(const T* p) : _ptr(p) {}
        ArrayCIterator(const ArrayCIterator<T>& it) : _ptr(it._ptr) {}
        ~ArrayCIterator() {}
        ArrayCIterator& operator=(const ArrayCIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator *() { return *_ptr; }
        pointer operator ->() { return _ptr; }
        pointer ptr() { return _ptr; }
        reference operator [](std::ptrdiff_t diff) { return *_ptr[diff]; }
        ArrayCIterator<T>& operator++() {
            if (_ptr) ++_ptr;
            return *this;
        }
        ArrayCIterator<T> operator++(int) { ArrayCIterator<T> it(*this); ++it; return it; }
        ArrayCIterator<T>& operator--() {
            if (_ptr) --_ptr;
            return *this;
        }
        ArrayCIterator<T> operator--(int) { ArrayCIterator<T> it(*this); --it; return it; }
        ArrayCIterator<T>& operator+=(std::ptrdiff_t diff) {
            if (_ptr) _ptr += diff;
            return *this;
        }
        ArrayCIterator<T>& operator-=(std::ptrdiff_t diff) {
            if (_ptr)_ptr -= diff;
            return *this;
        }
        ArrayCIterator<T> operator+(std::ptrdiff_t diff) { ArrayCIterator<T> it(*this); it += diff; return it; }
        ArrayCIterator<T> operator-(std::ptrdiff_t diff) { ArrayCIterator<T> it(*this); it -= diff; return it; }
        difference_type operator-(const ArrayCIterator<T> it) const { return _ptr - it._ptr; }
        void swap(ArrayCIterator<T> it1, ArrayCIterator<T> it2) { auto tmp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = tmp; }
        bool operator<(const ArrayCIterator<T>& it) const { return _ptr < it._ptr; }
        bool operator<=(const ArrayCIterator<T>& it) const { return _ptr <= it._ptr; }
        bool operator>(const ArrayCIterator<T>& it) const { return _ptr > it._ptr; }
        bool operator>=(const ArrayCIterator<T>& it) const { return _ptr >= it._ptr; }
        bool operator==(const ArrayCIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const ArrayCIterator<T>& it) const { return _ptr != it._ptr; }
    };
    template<typename T>
    class PArrayIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef SPointer<T> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef SPointer<T>* pointer;
        typedef T& reference;
    private:
        pointer _ptr;
    public:
        PArrayIterator() : _ptr(nullptr) {}
        PArrayIterator(pointer p) : _ptr(p) {}
        PArrayIterator(const PArrayIterator<T>& it) : _ptr(it._ptr) {}
        ~PArrayIterator() {}
        PArrayIterator<T>& operator=(const PArrayIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator*() const { return **_ptr; }
        T* operator->() { return (T*)*_ptr; }
        pointer ptr() { return _ptr; }
        value_type sptr() { return *_ptr; }
        reference operator[](std::ptrdiff_t diff) { return **_ptr[diff]; }
        PArrayIterator<T>& operator++() {
            if (_ptr) ++_ptr;
            return *this;
        }
        PArrayIterator<T> operator++(int) { PArrayIterator<T> it(*this); ++it; return it; }
        PArrayIterator<T>& operator--() {
            if (_ptr) --_ptr;
            return *this;
        }
        PArrayIterator<T> operator--(int) { PArrayIterator<T> it(*this); --it; return it; }
        PArrayIterator<T>& operator+=(std::ptrdiff_t diff) {
            if (_ptr) _ptr += diff;
            return *this;
        }
        PArrayIterator<T>& operator-=(std::ptrdiff_t diff) {
            if (_ptr)_ptr -= diff;
            return *this;
        }
        PArrayIterator<T> operator+(std::ptrdiff_t diff) { PArrayIterator<T> it(*this); it += diff; return it; }
        PArrayIterator<T> operator-(std::ptrdiff_t diff) { PArrayIterator<T> it(*this); it -= diff; return it; }
        difference_type operator-(const PArrayIterator<T>& it) const { return _ptr - it._ptr; }
        void swap(PArrayIterator<T> it1, PArrayIterator<T> it2) { auto tmp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = tmp; }
        bool operator<(const PArrayIterator<T>& it) const { return _ptr < it._ptr; }
        bool operator<=(const PArrayIterator<T>& it) const { return _ptr <= it._ptr; }
        bool operator>(const PArrayIterator<T>& it) const { return _ptr > it._ptr; }
        bool operator>=(const PArrayIterator<T>& it) const { return _ptr >= it._ptr; }
        bool operator==(const PArrayIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const PArrayIterator<T>& it) const { return _ptr != it._ptr; }
    };
    template<typename T>
    class PArrayCIterator {
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const SPointer<T> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const SPointer<T>* pointer;
        typedef const T& reference;
    private:
        pointer _ptr;
    public:
        PArrayCIterator() : _ptr(nullptr) {}
        PArrayCIterator(pointer p) : _ptr(p) {}
        PArrayCIterator(const PArrayCIterator<T>& it) : _ptr(it._ptr) {}
        ~PArrayCIterator() {}
        PArrayCIterator& operator=(const PArrayCIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator *() { return **_ptr; }
        const T* operator ->() { return (const T*)*_ptr; }
        pointer ptr() { return _ptr; }
        value_type sptr() { return *_ptr; }
        reference operator [](std::ptrdiff_t diff) { return **_ptr[diff]; }
        PArrayCIterator<T>& operator++() {
            if (_ptr) ++_ptr;
            return *this;
        }
        PArrayCIterator<T> operator++(int) { PArrayCIterator<T> it(*this); ++it; return it; }
        PArrayCIterator<T>& operator--() {
            if (_ptr) --_ptr;
            return *this;
        }
        PArrayCIterator<T> operator--(int) { PArrayCIterator<T> it(*this); --it; return it; }
        PArrayCIterator<T>& operator+=(std::ptrdiff_t diff) {
            if (_ptr) _ptr += diff;
            return *this;
        }
        PArrayCIterator<T>& operator-=(std::ptrdiff_t diff) {
            if (_ptr)_ptr -= diff;
            return *this;
        }
        PArrayCIterator<T> operator+(std::ptrdiff_t diff) { PArrayCIterator<T> it(*this); it += diff; return it; }
        PArrayCIterator<T> operator-(std::ptrdiff_t diff) { PArrayCIterator<T> it(*this); it -= diff; return it; }
        difference_type operator-(const PArrayCIterator<T>& it) const { return _ptr - it._ptr; }
        void swap(PArrayCIterator<T> it1, PArrayCIterator<T> it2) { auto tmp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = tmp; }
        bool operator<(const PArrayCIterator<T>& it) const { return _ptr < it._ptr; }
        bool operator<=(const PArrayCIterator<T>& it) const { return _ptr <= it._ptr; }
        bool operator>(const PArrayCIterator<T>& it) const { return _ptr > it._ptr; }
        bool operator>=(const PArrayCIterator<T>& it) const { return _ptr >= it._ptr; }
        bool operator==(const PArrayCIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const PArrayCIterator<T>& it) const { return _ptr != it._ptr; }
    };
}
#endif
