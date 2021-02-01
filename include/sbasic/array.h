#ifndef SLIB_ARRAY_H
#define SLIB_ARRAY_H

#include "sbasic/memory.h"
#include "sbasic/range.h"
#include "sbasic/arrayiter.h"
#include "sbasic/exception.h"

#define boolarray slib::CArray<bool>
#define bytearray slib::CArray<sbyte>
#define ubytearray slib::CArray<subyte>
#define shortarray slib::CArray<sshort>
#define ushortarray slib::CArray<sushort>
#define intarray slib::CArray<sint>
#define uintarray slib::CArray<suint>
#define sizearray slib::CArray<size_t>
#define floatarray slib::CArray<float>
#define doublearray slib::CArray<double>
#define integerarray slib::CArray<sinteger>
#define uintegerarray slib::CArray<suinteger>
#define voidarray slib::CArray<void *>
#define stringarray slib::Array<slib::String>

#define boolarray2d slib::Array<boolarray>
#define bytearray2d slib::Array<bytearray>
#define ubytearray2d slib::Array<ubytearray>
#define shortarray2d slib::Array<shortarray>
#define ushortarray2d slib::Array<ushortarray>
#define intarray2d slib::Array<intarray>
#define sizearray2d slib::Array<sizearray>
#define floatarray2d slib::Array<floatarray>
#define doublearray2d slib::Array<doublearray>
#define integerarray2d slib::Array<integerarray>
#define uintegerarray2d slib::Array<uintegerarray>
#define voidarray2d slib::Array<voidarray>

#define boolarray3d slib::Array<boolarray2d>
#define bytearray3d slib::Array<bytearray2d>
#define ubytearray3d slib::Array<ubytearray2d>
#define shortarray3d slib::Array<shortarray2d>
#define ushortarray3d slib::Array<ushortarray2d>
#define intarray3d slib::Array<intarray2d>
#define sizearray3d slib::Array<sizearray2d>
#define floatarray3d slib::Array<floatarray2d>
#define doublearray3d slib::Array<doublearray2d>
#define integerarray3d slib::Array<integerarray2d>
#define uintegerarray3d slib::Array<uintegerarray2d>
#define voidarray3d slib::Array<voidarray2d>

#define MAX_ARRAY_SIZE 0x7FFFFFFFFFFFFFFF

namespace slib {
	class SLIB_DLL String;
	class SLIB_DLL SData;
    template <typename T, class M=SMemory<T>>
    class Array {
		friend SData;
	public:
		typedef T value_type;
        typedef std::function<bool(const T &t1, const T &t2)> Comparer;
    private:
        void _expand(size_t s);
		void _set(T* p, const T& v);
		sarr_iter<T> _insert(T* p, const T* v, size_t s);
		sarr_iter<T> _remove(T* b, T* e);
		Array<T, M> _subarray(const T* b, const T* e) const;
    protected:
        size_t _capacity;
        T *_begin, *_end;
    public:
        Array();
        Array(size_t s);
        Array(size_t s, const T &val);
        Array(std::initializer_list<T> li);
        Array(Array &&array) noexcept;
        Array(const Array &array);
        virtual ~Array();
        Array &operator = (const Array &array);
        Array &operator = (Array &&array);
        T &operator[] (sinteger idx);
        const T &operator[] (sinteger idx) const;
        T &at(sinteger idx);
        const T &at(sinteger idx) const;
        T &first();
        const T &first() const;
        T &last();
        const T &last() const;
        T *ptr() const;
        T *ptr(size_t idx) const;
        sarr_iter<T> begin();
        sarr_citer<T> begin() const;
        sarr_iter<T> end();
        sarr_citer<T> end() const;
        Array subarray(size_t off, size_t len = -1) const;
        Array subarray(sarr_citer<T> beg, sarr_citer<T> end) const;
        Array subarray(srange range) const;
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        void add(T &&val);
        void add(const T &val);
        template<class... Args>
        void add(Args... args) { add(T(args...)); }
        void append(const T *val, size_t s);
        void append(const Array &array);
        void set(sinteger idx, const T &val);
        void set(sarr_iter<T> iter, const T &val);
        void exchange(sarr_iter<T> iter1, sarr_iter<T> iter2);
        void exchange(size_t idx1, size_t idx2);
		sarr_iter<T> insert(sinteger idx, const T &val);
		sarr_iter<T> insert(sarr_iter<T> iter, const T& val);
		sarr_iter<T> insert(sinteger idx, T *val, size_t s);
		sarr_iter<T> insert(sinteger idx, const Array &array);
		sarr_iter<T> remove(const T &val);
        sarr_iter<T> remove(sarr_iter<T> beg, sarr_iter<T> end);
        sarr_iter<T> remove(size_t off, size_t len);
        sarr_iter<T> removeAt(sinteger idx);
		T pop();
		T get(sinteger idx) const;
        void clear();
        void shrink();
        void reset(const T &val);
        void sort(Comparer comp=sortAsc<T>);
        void copy(const T *ptr, size_t s);
        void copyTo(Array &array) const;
        void moveTo(Array &array);
        void swap(Array &array);
        bool contain(const T &val) const;
		sarr_iter<T> locate(const T& val, size_t off = 0);
		sarr_citer<T> locate(const T& val, size_t off = 0) const;
		size_t find(const T &val, size_t off = 0) const;
        size_t rfind(const T &val, size_t off = 0) const;
        void resize(size_t s);
        void resize(size_t s, const T &val);
        void reserve(size_t s);
        void release();
        void discard();
        bool operator<(const Array &array) const;
        bool operator==(const Array &array) const;
    };
    template<typename T>
	class CArray : public Array<T, CMemory<T>> {
	public:
		CArray();
		CArray(size_t s);
		CArray(size_t s, const T& val);
		CArray(std::initializer_list<T> li);
		CArray(CArray&& array) noexcept;
		CArray(const CArray& array);
		~CArray();
		CArray& operator = (const CArray& array);
		CArray& operator = (CArray&& array);
	};
	template<typename T>
	class RArray : public Array<T, RMemory<T>> {
	public:
		RArray();
		RArray(size_t s);
		RArray(size_t s, const T& val);
		RArray(std::initializer_list<T> li);
		RArray(RArray&& array);
		RArray(const RArray& array);
		~RArray();
		RArray& operator = (const RArray& array);
		RArray& operator = (RArray&& array);
	};
	template<typename T>
	class UArray : public Array<T, UMemory<T>> {
	public:
		UArray();
		UArray(size_t s);
		~UArray();
	};
	extern inline intarray iarray(int num, bool zero = false) {
		intarray iarray(num);
		sforin(i, 0, num) iarray[i] = (zero ? i : i + 1);
		return iarray;
	}
	extern inline intarray iarray(const String& str, const char* sep) {
		intarray iarray;
		auto arr = str.split(sep);
		sforeach(arr) iarray.add(it->integer());
		return iarray;
	}
	template<typename T, class M>
	extern String toString(const Array<T, M>& array, const char* sep = ",") {
		String str;
		if (!array.empty()) sforeach(array) str << E_ << sep;
		if (!str.empty()) str.resize(str.length() - strlen(sep));
		return str;
	}
	template<typename T, size_t S, class M>
	extern String toString(const FixedArray<T, S, M>& array, const char* sep = ",") {
		String str;
		if (!array.empty()) sforeach(array) str << E_ << sep;
		if (!str.empty()) str.resize(str.length() - strlen(sep));
		return str;
	}
	template<typename T, class M>
	extern String toString(const BiArray<T, M>& array, const char* sep = ",") {
		String str;
		if (!array.empty()) sforeach(array) str << E_ << sep;
		if (!str.empty()) str.resize(str.length() - strlen(sep));
		return str;
	}
	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const Array<T, M>& array) { return os << toString(array); }
	template<typename T, size_t S, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const FixedArray<T, S, M>& array) { return os << toString(array); }
	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const BiArray<T, M>& array) { return os << toString(array); }

    /*============================================================*/
    template <typename T, class M>
	void Array<T, M>::_expand(size_t s) {
        auto cap = _capacity;
		if (!cap) reserve(s + 1);
        else {
			while (cap <= s) cap <<= 1;
            reserve(cap);
        }
    }
	template <typename T, class M>
	void Array<T, M>::_set(T *p, const T &v) {
		if (_begin <= p && p < _end) {
			M::release(p, 1); M::assign(p, v);
		}
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
	template <typename T, class M>
	sarr_iter<T> Array<T, M>::_insert(T* p, const T* v, size_t s) {
		if (_begin <= p && p < _end) {
			auto s_ = _end - _begin + s;
			if (_capacity <= s_) { auto i = p - _begin; _expand(s); p = _begin + i; }
			M::shift(p + s, p, _end - p);
			M::copy(p, v, s); _end += s;
			return sarr_iter<T>(p);
		}
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
	template <typename T, class M>
	sarr_iter<T> Array<T, M>::_remove(T* b, T* e) {
		if (_begin <= b && e < _end) {
			auto len = e - b;
			M::release(b, len); M::shift(b, e, _end - e); _end -= len;
			return sarr_iter<T>(b);
		}
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
	template <typename T, class M>
	Array<T, M> Array<T, M>::_subarray(const T* beg, const T* end) const {
		if (end < beg || beg < _begin || end < _begin || _end < beg || _end < end) throw SException(ERR_INFO, SLIB_RANGE_ERROR);
		Array<T, M> array(end - beg);
		auto ptr = array.ptr();
		while (beg < end) { M::assign(ptr, *beg); ++beg; ++ptr; }
		return array;
	}
    template <typename T, class M>
    Array<T, M>::Array() : _capacity(0), _begin(nullptr), _end(nullptr) {}
    template <typename T, class M>
    Array<T, M>::Array(size_t s) : Array() { if (s) resize(s); }
    template <typename T, class M>
    Array<T, M>::Array(size_t s, const T &val) : Array(s) {
        if (s) { 
			auto p = _begin;
			while (p < _end) { M::assign(p, val); ++p; }
		}
    }
    template <typename T, class M>
    Array<T, M>::Array(std::initializer_list<T> li) : Array(li.size()) {
        if (li.size()) {
			auto p = _begin;
            auto it = li.begin();
			while (p < _end) { M::assign(p, E_); ++p; NEXT_; }
        }
    }
    template <typename T, class M>
    Array<T, M>::Array(Array<T, M> &&array) noexcept : Array() {
        _capacity = array._capacity;
        _begin = array._begin;
        _end = array._end;
        array.discard();
    }
    template <typename T, class M>
    Array<T, M>::Array(const Array<T, M> &array) : Array(array.size()) {
		if (!array.empty()) {
			auto p = _begin, p_ = array._begin;
			while (p < _end) { M::assign(p, *p_); ++p; ++p_; }
        }
    }
    template <typename T, class M>
    Array<T, M>::~Array() { release(); }
    template <typename T, class M>
    Array<T, M> &Array<T, M>::operator=(const Array<T, M> &array) {
		if (array.empty()) clear();
		else {
            resize(array.size());
			auto p = _begin, p_ = array._begin;
			while (p < _end) { M::assign(p, *p_); ++p; ++p_; }
        } 
        return (*this);
    }
    template <typename T, class M>
    Array<T, M> &Array<T, M>::operator = (Array<T, M> &&array) {
        release();
        _capacity = array._capacity;
        _begin = array._begin;
        _end = array._end;
        array.discard();
        return (*this);
    }
    template <typename T, class M>
	T &Array<T, M>::operator[] (sinteger idx) { return at(idx); }
    template <typename T, class M>
	const T &Array<T, M>::operator[] (sinteger idx) const { return at(idx); }
    template <typename T, class M>
    T &Array<T, M>::at(sinteger idx) {
		auto p = idx < 0 ? _end + idx : _begin + idx;
        if (p < _begin || _end <= p)
            throw SException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(idx).c_str(), std::string(RANGE_TEXT(0, size())).c_str());
        return *p;
    }
    template <typename T, class M>
	const T &Array<T, M>::at(sinteger idx) const {
		auto p = idx < 0 ? _end + idx : _begin + idx;
        if (p < _begin || _end <= p)
            throw SException(ERR_INFO, SLIB_RANGE_ERROR);
        return *p;
    }
    template <typename T, class M>
	T &Array<T, M>::first() { return at(0); }
    template <typename T, class M>
	const T &Array<T, M>::first() const { return at(0); }
    template <typename T, class M>
	T &Array<T, M>::last() { return at(-1); }
    template <typename T, class M>
	const T &Array<T, M>::last() const { return at(-1); }
    template <typename T, class M>
	T *Array<T, M>::ptr() const { return _begin; }
    template <typename T, class M>
	T *Array<T, M>::ptr(size_t idx) const { return _begin+idx; }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::begin() { return SArrayIterator<T>(_begin); }
    template <typename T, class M>
	sarr_citer<T> Array<T, M>::begin() const { return SArrayCIterator<T>(_begin); }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::end() { return SArrayIterator<T>(_end); }
    template <typename T, class M>
	sarr_citer<T> Array<T, M>::end() const { return SArrayCIterator<T>(_end); }
	template <typename T, class M>
	Array<T, M> Array<T, M>::subarray(size_t off, size_t len) const {
		return _subarray(_begin + off, len == -1 ? _end : _begin + off + len);
    }
    template <typename T, class M>
	Array<T, M> Array<T, M>::subarray(sarr_citer<T> beg, sarr_citer<T> end) const {
        return _subarray(beg.ptr(), end.ptr());
    }
    template <typename T, class M>
	Array<T, M> Array<T, M>::subarray(srange range) const {
		return _subarray(_begin + range.begin, _begin + range.end);
    }
    template <typename T, class M>
	bool Array<T, M>::empty() const { return _begin == _end; }
    template <typename T, class M>
	size_t Array<T, M>::size() const { return _capacity?(_end - _begin):0; }
    template <typename T, class M>
	size_t Array<T, M>::capacity() const { return _capacity; }
    template <typename T, class M>
	void Array<T, M>::add(T &&val) {
        if (!_capacity) reserve(16);
		size_t s = _end - _begin + 1;
        if (_capacity == s) _expand(s);
		new(_end) T(std::forward<T &&>(val));
		++_end;
    }
    template <typename T, class M>
	void Array<T, M>::add(const T &val) {
        if (!_capacity) reserve(16);
		size_t s = _end - _begin + 1;
        if (_capacity == s) _expand(s);
        M::init(_end,1); 
		M::assign(_end, val);
		++_end;
    }
    template <typename T, class M>
	void Array<T, M>::append(const T *val, size_t s) {
		auto s_ = _end - _begin + s;
		if (!_capacity) reserve(s + 1);
        if (_capacity <= s_) _expand(s_);
        M::copy(_end, val, s); _end += s;
    }
    template <typename T, class M>
	void Array<T, M>::append(const Array<T, M> &array) {
		if (!array.empty()) append(array._begin, array.size());
    }
    template <typename T, class M>
	void Array<T, M>::set(sinteger idx, const T& val) { _set((idx < 0 ? _end : _begin) + idx, val); }
    template <typename T, class M>
	void Array<T, M>::set(sarr_iter<T> iter, const T &val) { _set(iter.ptr(), val); }
    template <typename T, class M>
	void Array<T, M>::exchange(sarr_iter<T> iter1, sarr_iter<T> iter2) { iter1.swap(iter1, iter2); }
    template <typename T, class M>
	void Array<T, M>::exchange(size_t idx1, size_t idx2) { exchange(begin() + idx1, begin() + idx2); }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::insert(sinteger idx, const T& val) { return _insert((idx < 0 ? _end : _begin) + idx, &val, 1); }
	template <typename T, class M>
	sarr_iter<T> Array<T, M>::insert(sarr_iter<T> iter, const T& val) { return _insert(iter.ptr(), &val, 1); }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::insert(sinteger idx, T* val, size_t s) { return s ? _insert((idx < 0 ? _end : _begin) + idx, val, s) : begin() + idx; }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::insert(sinteger idx, const Array<T, M> &array) {
		if (array.empty()) return begin() + idx;
		else return _insert((idx < 0 ? _end : _begin) + idx, array.ptr(), array.size());
    }
	template <typename T, class M>
	sarr_iter<T> Array<T, M>::remove(const T& val) {
		auto it = locate(val);
		return _remove(it.ptr(), it.ptr() + 1);
	}
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::remove(sarr_iter<T> beg, sarr_iter<T> end) { return _remove(beg.ptr(), end.ptr()); }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::remove(size_t off, size_t len) {
		return _remove(_begin + off, len == -1 ? _end : _begin + off + len);
    }
    template <typename T, class M>
	sarr_iter<T> Array<T, M>::removeAt(sinteger idx) { 
		return _remove((idx < 0 ? _end : _begin) + idx, (idx < 0 ? _end : _begin) + idx + 1); 
	}
	template <typename T, class M>
	T Array<T, M>::pop() {
		if (!empty()) {
			T tmp = last();
			resize(size() - 1);
			return tmp;
		}
		throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
	template <typename T, class M>
	T Array<T, M>::get(sinteger idx) const { return at(idx); }
    template <typename T, class M>
	void Array<T, M>::clear() { resize(0); }
    template <typename T, class M>
	void Array<T, M>::shrink() {
        Array<T, M> array(*this);
        swap(array);
    }
    template <typename T, class M>
	void Array<T, M>::reset(const T &val) {
        if (!empty()) {
            M::release(_begin, size());
			auto p = _begin;
			while (p < _end) { M::assign(p, val); ++p; }
        }
    }
    template <typename T, class M>
	void Array<T, M>::sort(Comparer comp) {
        std::sort(begin(), end(), comp);
    }
    template <typename T, class M>
	void Array<T, M>::copy(const T *ptr, size_t s) {
        clear(); resize(s);
		if (!ptr || !s) return;
		auto p = _begin;
		while (p < _end) { M::assign(p, *ptr); ++p; ++ptr; }
    }
    template <typename T, class M>
	void Array<T, M>::copyTo(Array<T, M> &array) const {
        array.copy(_begin, size());
    }
    template <typename T, class M>
	void Array<T, M>::moveTo(Array<T, M> &array) {
		array.release();
        array._capacity = _capacity;
        array._begin = _begin;
        array._end = _end;
        discard();
    }
    template <typename T, class M>
	void Array<T, M>::swap(Array<T, M> &array) {
        auto tmpc = _capacity; _capacity = array._capacity; array._capacity = tmpc;
        auto tmpb = _begin; _begin = array._begin; array._begin = tmpb;
        auto tmpe = _end; _end = array._end; array._end = tmpe;
    }
    template <typename T, class M>
	bool Array<T, M>::contain(const T &val) const { return find(val) != NOT_FOUND; }
	template <typename T, class M>
	sarr_iter<T> Array<T, M>::locate(const T& val, size_t off) {
		auto p = _begin + off;
		while (p < _end) {
			if (*p == val) return sarr_iter<T>(p);
			++p;
		}
		return end();
	}
	template <typename T, class M>
	sarr_citer<T> Array<T, M>::locate(const T& val, size_t off) const {
		auto p = _begin + off;
		while (p < _end) {
			if (*p == val) return sarr_citer<T>(p);
			++p;
		}
		return end();
	}
    template <typename T, class M>
	size_t Array<T, M>::find(const T &val, size_t off) const {
		auto it = _begin + off;
		while (it < _end) { if (E_ == val) return it - _begin; else NEXT_; }
        return NOT_FOUND;
    }
    template <typename T, class M>
	size_t Array<T, M>::rfind(const T &val, size_t off) const {
		auto it = _end - off - 1;
		while (_begin <= it) { if (E_ == val) return it - _begin; else PREV_; }
        return NOT_FOUND;
    }
    template <typename T, class M>
	void Array<T, M>::resize(size_t s) {
        if (s) {
			if (!_capacity) reserve(s + 1);
            else if (_capacity <= s) _expand(s);
        }
        auto s_ = size();
        if (s < s_) M::release(_begin+s, s_-s);
        else if (s_ < s) M::init(_begin+s_, s-s_);
        _end = _begin + s;
    }
    template <typename T, class M>
	void Array<T, M>::resize(size_t s, const T &val) {
        auto s_ = size();
        resize(s);
        if (s_ < s) {
			auto p = _begin + s_;
			while (p < _end) { M::assign(p, val); ++p; }
        }
    }
    template <typename T, class M>
	void Array<T, M>::reserve(size_t s) {
		if (MAX_ARRAY_SIZE < s) throw SException(ERR_INFO, SLIB_RANGE_ERROR);
        if (_capacity < s) {
            if (_begin) {
                auto s_ = size();
                auto tmp = M::alloc(s);
                if (s_) {
                    M::init(tmp, s_);
                    M::copy(tmp, _begin, s_);
                    M::release(_begin, s_);
                }
                M::dealloc(_begin);
                _begin = tmp; _end = _begin+s_;
            }
            else { _begin = M::alloc(s); _end = _begin; }
            _capacity = s;
        }
    }
    template <typename T, class M>
	void Array<T, M>::release() {
        if (_begin) {
            if (_begin < _end) M::release(_begin, _end-_begin);
            M::dealloc(_begin);
        }
        discard();
    }
    template <typename T, class M>
	void Array<T, M>::discard() {
        _capacity = 0; _begin = nullptr; _end = nullptr;
    }
	template<typename T, class M>
	bool Array<T, M>::operator<(const Array& array) const {
		if (size() != array.size()) return size() < array.size();
		sforeach2(*this, array) {
			if (!(E1_ == E2_)) return E1_ < E2_;
		}
		return false;
	}
    template <typename T, class M>
	bool Array<T, M>::operator==(const Array<T, M> &array) const {
        if (size() != array.size()) return false;
		sforeach2(*this, array) {
			if (!(E1_ == E2_)) return false;
		}
        return true;
    }
	template <typename T>
	CArray<T>::CArray() : Array<T, CMemory<T>>() {}
	template <typename T>
	CArray<T>::CArray(size_t s) : Array<T, CMemory<T>>(s) {}
	template <typename T>
	CArray<T>::CArray(size_t s, const T& val) : Array<T, CMemory<T>>(s, val) {}
	template <typename T>
	CArray<T>::CArray(std::initializer_list<T> li) : Array<T, CMemory<T>>(li) {}
	template <typename T>
	CArray<T>::CArray(CArray&& array) noexcept : Array<T, CMemory<T>>(std::forward<CArray&&>(array)) {}
	template <typename T>
	CArray<T>::CArray(const CArray& array) : Array<T, CMemory<T>>(array) {}
	template <typename T>
	CArray<T>::~CArray() {}
	template <typename T>
	CArray<T>& CArray<T>::operator = (const CArray<T>& array) {
		if (!array.empty()) {
			Array<T, CMemory<T>>::resize(array.size());
			auto p = Array<T, CMemory<T>>::_begin, p_ = array._begin;
			while (p < Array<T, CMemory<T>>::_end) { CMemory<T>::assign(p, *p_); ++p; ++p_; }
		}
		else Array<T, CMemory<T>>::clear();
		return (*this);
	}
	template <typename T>
	CArray<T>& CArray<T>::operator = (CArray<T>&& array) {
		Array<T, CMemory<T>>::release();
		Array<T, CMemory<T>>::_capacity = array._capacity;
		Array<T, CMemory<T>>::_begin = array._begin;
		Array<T, CMemory<T>>::_end = array._end;
		array.discard();
		return (*this);
	}
	template <typename T>
	RArray<T>::RArray() : Array<T, RMemory<T>>() {}
	template <typename T>
	RArray<T>::RArray(size_t s) : Array<T, RMemory<T>>(s) {}
	template <typename T>
	RArray<T>::RArray(size_t s, const T& val) : Array<T, RMemory<T>>(s, val) {}
	template <typename T>
	RArray<T>::RArray(std::initializer_list<T> li) : Array<T, RMemory<T>>(li) {}
	template <typename T>
	RArray<T>::RArray(RArray&& array) : Array<T, RMemory<T>>(array) {}
	template <typename T>
	RArray<T>::RArray(const RArray& array) : Array<T, RMemory<T>>(array) {}
	template <typename T>
	RArray<T>::~RArray() {}
	template <typename T>
	RArray<T>& RArray<T>::operator = (const RArray<T>& array) {
		if (!array.empty()) {
			Array<T, RMemory<T>>::resize(array.size());
			auto p = Array<T, RMemory<T>>::_begin, p_ = array._begin;
			while (p < Array<T, RMemory<T>>::_end) { RMemory<T>::assign(p, *p_); ++p; ++p_; }
		}
		else Array<T, RMemory<T>>::clear();
		return (*this);
	}
	template <typename T>
	RArray<T>& RArray<T>::operator = (RArray<T>&& array) {
		Array<T, RMemory<T>>::release();
		Array<T, RMemory<T>>::_capacity = array._capacity;
		Array<T, RMemory<T>>::_begin = array._begin;
		Array<T, RMemory<T>>::_end = array._end;
		array.discard();
		return (*this);
	}
	template<typename T>
	UArray<T>::UArray() : Array<T, UMemory<T>>() {}
	template<typename T>
	UArray<T>::UArray(size_t s) : Array<T, UMemory<T>>(s) {}
	template<typename T>
	UArray<T>::~UArray() {}
}

#endif
