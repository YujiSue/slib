#ifndef SLIB_FIXARRAY_H
#define SLIB_FIXARRAY_H
#include "sbasic/memory.h"
#include "sbasic/range.h"
#include "sbasic/arrayiter.h"
#include "sbasic/exception.h"
namespace slib {
    /**
    * @class FArray
    * \~english @brief Fixed size array
    * \~japanese @brief 固定サイズ配列クラス
    */
    template <typename T, size_t S>
    class FArray {
	public:
		typedef T value_type;
    protected:
        T* _ptr;
    public:
        FArray() { 
            _ptr = Memory<T>::alloc(S);
            if (!_ptr) throw MemAllocException();
            auto p = _ptr;
            sforin(i, 0, S) {
                Memory<T>::init(p); ++p;
            }
        }
        FArray(const T& val) { 
            _ptr = Memory<T>::alloc(S);
            if (!_ptr) throw MemAllocException();
            auto p = _ptr;
            sforin(i, 0, S) {
                Memory<T>::init(p, val); ++p;
            }
        }
        FArray(std::initializer_list<T> li) : FArray<T, S>() {
            sfor2(*this, li) Memory<T>::assign(&$_1, $_2);
        }
        FArray(FArray<T, S>&& array) noexcept {
            Memory<T*>::swap(&_ptr, &array._ptr);
        }
        FArray(const FArray<T, S>& array) : FArray<T, S>() {
            sfor2(*this, array)  Memory<T>::assign(&$_1, $_2);
        }
        ~FArray() { if (_ptr) Memory<T>::dealloc(_ptr); }
        FArray<T, S>& operator=(FArray<T, S>&& array) noexcept {
            Memory<T*>::swap(&_ptr, &array._ptr); return *this;
        }
        FArray<T, S>&operator=(const FArray<T, S>& array) {
            sfor2(*this, array)  Memory<T>::assign(&$_1, $_2);
            return *this;
        }
        size_t size() const { return S; }
        void reset(T&& val) noexcept { sfor(*this) $_ = std::forward<T&&>(val); }
        void reset(const T& val) { sfor(*this) $_ = val; }
        ArrayIterator<T> iterAt(const int i) {
            auto p = _ptr + (i < 0 ? (int)S : 0) + i;
            if (p < _ptr || _ptr + S <= p)
                throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, S - 1));
            return ArrayIterator<T>(p);
        }
        ArrayCIterator<T> iterAt(const int i) const {
            auto p = _ptr + (i < 0 ? (int)S : 0) + i;
            if (p < _ptr || _ptr + S <= p)
                throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, S - 1));
            return ArrayCIterator<T>(p);
        }
        T* data(const int i = 0) { return iterAt(i).ptr(); }
        const T* data(const int i = 0) const { return iterAt(i).ptr(); }
        T& at(const int i) { return *iterAt(i); }
        const T& at(const int i) const { return *iterAt(i); }
        T& operator[](const int i) { return at(i); }
        const T& operator[](const int i) const { return at(i); }
        T get(const int i) const { return T(at(i)); }
        ArrayIterator<T> begin() { return ArrayIterator<T>(_ptr); }
        ArrayCIterator<T> begin() const { return ArrayCIterator<T>(_ptr); }
        ArrayIterator<T> end() { return ArrayIterator<T>(_ptr + S); }
        ArrayCIterator<T> end() const { return ArrayCIterator<T>(_ptr + S); }
        void swap(FArray<T, S>& array) { Memory<T*>::swap(&_ptr, &array._ptr); }
        bool include(const T& val) const { return indexOf(val) != NOT_FOUND; }
        size_t indexOf(const T& val, const size_t off = 0) const {
            auto it = begin() + off;
            while (it < end()) {
                if ($_ == val) break;
                $NEXT;
            }
            return it == end() ? NOT_FOUND : $INDEX(*this);
        }
        bool operator<(const FArray<T, S> &array) const {
            sfor2(*this, array) { if ($_1 != $_2) return $_1 < $_2; }
            return false;
        }
        bool operator==(const FArray<T, S> &array) const {
            sfor2(*this, array) { if ($_1 != $_2) return false; }
            return true;
        }
    };
}
#endif