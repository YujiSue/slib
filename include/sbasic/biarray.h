#ifndef __SLIB_BIARRAY_H__
#define __SLIB_BIARRAY_H__

#include "sbasic/array.h"

namespace slib {
    
    template <typename T, class M=SMemory<T>>
    class BiArray {
	public:
		typedef T value_type;
		typedef std::function<bool(const T & t1, const T & t2)> Comparer;
        
    protected:
        size_t _capacity;
        T *_begin, *_off, *_end;
        
    private:
		size_t _remain() const;
        void _expandHead(size_t s);
        void _expandTail(size_t s);
        
    public:
        BiArray();
        BiArray(size_t s);
        BiArray(size_t s, const T &val);
        BiArray(std::initializer_list<T> li);
        BiArray(BiArray<T> &&barray);
        BiArray(const BiArray<T> &barray);
        ~BiArray();
        
        BiArray &operator = (BiArray &&barray);
        BiArray &operator = (const BiArray &barray);
        
        T &operator[] (int idx);
        const T &operator[] (int idx) const;
        const T &at(int idx) const;
        T &at(int idx);
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
        
        BiArray subarray(size_t off, size_t len = -1) const;
        BiArray subarray(sarr_citer<T> beg, sarr_citer<T> end) const;
        BiArray subarray(srange range) const;
        
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        size_t offset() const;
        void setOffset(size_t off);
        
        void add(T &&val);
        void add(const T &val);
        template<class... Args>
        void add(Args... args) {
            if (!_capacity) reserve(3, 1);
            auto s = _end-_begin+1;
            if (_capacity <= s) _expandTail(s);
            *_end = T(args...); ++_end;
        }
        void put(T &&val);
        void put(const T &val);
        template<class... Args>
        void put(Args... args) {
            if (!_capacity) reserve(3, 1);
            int s = offset()-1;
            if (s < 0) _expandHead(_capacity-s);
            --_off; *_off = T(args...);
        }
        void append(const T *val, size_t s);
        void pile(const T *val, size_t s);
        void append(const BiArray &array);
        void pile(const BiArray &array);
        void set(size_t idx, const T &val);
        void set(sarr_iter<T> iter, const T &val);
        void exchange(sarr_iter<T> iter1, sarr_iter<T> iter2);
        void exchange(size_t idx1, size_t idx2);
		sarr_iter<T> insert(size_t idx, const T &val);
		sarr_iter<T> insert(sarr_iter<T> iter, const T& val);
		sarr_iter<T> insert(size_t idx, T *val, size_t s);
		sarr_iter<T> insert(size_t idx, const BiArray &array);
        sarr_iter<T> remove(sarr_iter<T> beg, sarr_iter<T> end);
        sarr_iter<T> remove(srange range);
        sarr_iter<T> remove(size_t off, size_t len);
        sarr_iter<T> removeAt(size_t idx);
        
        void trim(size_t s);
        void cut(size_t s);
        T get();
        T pick();
        void clear();
        void reset(const T &val);
        
        void sort(Comparer comp=sortAsc<T>);
        void copy(const T *ptr, size_t s);
        void copyTo(BiArray &array);
        void moveTo(BiArray &array);
        void swap(BiArray &array);
        
        bool contain(const T &val) const;
        size_t find(const T &val, size_t off = 0) const;
        size_t rfind(const T &val, size_t off = 0) const;
        
        void resize(size_t s, bool head = false);
        void resize(size_t s, const T &val, bool head = false);
        void reserve(size_t s, size_t off = -1);
        void release();
        void discard();
        bool operator < (const BiArray &array) const;
        bool operator == (const BiArray &array) const;
    };
    /*============================================================*/
	template <typename T, class M>
	size_t BiArray<T, M>::_remain() const {
		return _begin + _capacity - _end;
	}
    template <typename T, class M>
    void BiArray<T, M>::_expandHead(size_t s) {
		auto cap = _capacity;
		auto remain = _remain();
		while (cap <= s) cap <<= 1;
		reserve(cap, cap - remain - size());
    }
    template <typename T, class M>
    void BiArray<T, M>::_expandTail(size_t s) {
        auto cap = _capacity;
		while (cap <= s) cap <<= 1;
        reserve(cap, offset());
    }
    template <typename T, class M>
    BiArray<T, M>::BiArray() : _capacity(0), _begin(nullptr), _off(nullptr), _end(nullptr) {}
    template <typename T, class M>
    BiArray<T, M>::BiArray(size_t s) : BiArray() { if (s) resize(s); }
    template <typename T, class M>
    BiArray<T, M>::BiArray(size_t s, const T &val) : BiArray(s) {
		if (s) {
			auto p = _off;
			while (p < _end) { M::assign(p, val); ++p; }
		}
    }
    template <typename T, class M>
    BiArray<T, M>::BiArray(std::initializer_list<T> li) : BiArray(li.size()) {
        auto p = _off;
        auto it = li.begin();
		while (p < _end) { M::assign(p, E_); ++p; NEXT_; }
    }
    template <typename T, class M>
    BiArray<T, M>::BiArray(BiArray<T> &&barray) : BiArray() {
        _capacity = barray._capacity;
        _begin = barray._begin;
        _end = barray._end;
        _off = barray._off;
        barray.discard();
    }
    template <typename T, class M>
    BiArray<T, M>::BiArray(const BiArray<T> &barray) : BiArray() {
        if (!barray.empty()) {
            resize(barray.size());
			auto p = _off, p_ = barray._off;
			while (p < _end) { M::assign(p, *p_); ++p; ++p_; }
        }
    }
    template <typename T, class M>
    BiArray<T, M>::~BiArray() { release(); }
    template <typename T, class M>
    BiArray<T, M> &BiArray<T, M>::operator = (BiArray &&barray) {
        release();
        _capacity = barray._capacity;
        _begin = barray._begin;
        _end = barray._end;
        _off = barray._off;
        barray.discard();
        return (*this);
    }
    template <typename T, class M>
    BiArray<T, M> &BiArray<T, M>::operator = (const BiArray<T, M> &barray) {
        clear();
        auto s = barray.size();
        if(s) {
            resize(s);
			auto p = _off, p_ = barray._off;
			while (p < _end) { M::assign(p, *p_); ++p; ++p_; }\
        }
        return (*this);
    }
    template <typename T, class M>
    T &BiArray<T, M>::operator[] (int idx) { return at(idx); }
    template <typename T, class M>
    const T &BiArray<T, M>::operator[] (int idx) const { return at(idx); }
    template <typename T, class M>
    const T &BiArray<T, M>::at(int idx) const {
		auto p = idx < 0 ? _end + idx : _off + idx;
		if (p < _off || _end <= p)
			throw SException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(p - _begin).c_str(), RANGE_TEXT(offset(), size()));
        return *p;
    }
    template <typename T, class M>
    T &BiArray<T, M>::at(int idx) {
		auto p = idx < 0 ? _end + idx : _off + idx;
		if (p < _off || _end <= p)
			throw SException(ERR_INFO, SLIB_RANGE_ERROR, std::to_string(p - _begin).c_str(), RANGE_TEXT(offset(), size()));
        return *p;
    }
    template <typename T, class M>
    T &BiArray<T, M>::first() { return at(0); }
    template <typename T, class M>
    const T &BiArray<T, M>::first() const { return at(0); }
    template <typename T, class M>
    T &BiArray<T, M>::last() { return at(-1); }
    template <typename T, class M>
    const T &BiArray<T, M>::last() const { return at(-1); }
    template <typename T, class M>
    T *BiArray<T, M>::ptr() const { return _off; }
    template <typename T, class M>
    T *BiArray<T, M>::ptr(size_t idx) const { return _off+idx; }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::begin() { return sarr_iter<T>(_off); }
    template <typename T, class M>
    sarr_citer<T> BiArray<T, M>::begin() const { return sarr_citer<T>(_off); }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::end() { return sarr_iter<T>(_end); }
    template <typename T, class M>
    sarr_citer<T> BiArray<T, M>::end() const { return sarr_citer<T>(_end); }
    template <typename T, class M>
    BiArray<T, M> BiArray<T, M>::subarray(size_t off, size_t len) const {
		return subarray(begin() + off, begin() + off + len);
    }
    template <typename T, class M>
    BiArray<T, M> BiArray<T, M>::subarray(sarr_citer<T> beg, sarr_citer<T> end) const {
        auto b = beg._ptr, e = end._ptr;
        if (_end <= b) return BiArray();
        if (_end <= e) e = _end;
		BiArray array(e - b);
        auto p = array._off;
        while(b < e) { M::assign(p, *b); ++p; ++b; }
        return array;
    }
    template <typename T, class M>
    BiArray<T, M> BiArray<T, M>::subarray(srange range) const {
		return subarray(begin() + range.begin, begin() + range.end);
    }
    template <typename T, class M>
    bool BiArray<T, M>::empty() const { return _off == _end; }
    template <typename T, class M>
    size_t BiArray<T, M>::size() const { return _end - _off; }
    template <typename T, class M>
    size_t BiArray<T, M>::capacity() const { return _capacity; }
    template <typename T, class M>
    size_t BiArray<T, M>::offset() const { return _off-_begin; }
    template <typename T, class M>
    void BiArray<T, M>::setOffset(size_t off) {
		auto p = _begin + off;
		if (_end <= p) throw SException(ERR_INFO, SLIB_RANGE_ERROR);
		if (p < _off) { M::init(p, _off - p); _off = p; }
		else if (_off < p) { M::release(_off, p - _off); _off = p; }
	}
    template <typename T, class M>
    void BiArray<T, M>::add(T &&val) {
        if (!_capacity) reserve(16);
		auto s = _end - _begin + 1;
		if (_capacity == s) _expandTail(s);
		M::init(_end, 1);
		new(_end) T(std::forward<T>(val));
		++_end;
    }
    template <typename T, class M>
    void BiArray<T, M>::add(const T &val) {
        if (!_capacity) reserve(16);
		auto s = _end - _begin + 1;
		if (_capacity == s) _expandTail(s);
		M::init(_end, 1);
		M::assign(_end, val);
		++_end;
    }
    template <typename T, class M>
    void BiArray<T, M>::put(T &&val) {
        if (!_capacity) reserve(16);
		if (_off - 1 == _begin) _expandHead(_capacity);
        --_off; 
		M::init(_off, 1);
		new(_off) T(std::forward<T>(val));
    }
    template <typename T, class M>
    void BiArray<T, M>::put(const T &val) {
		if (!_capacity) reserve(16);
		if (_off - 1 == _begin) _expandHead(_capacity);
		--_off;
		M::init(_off, 1);
		M::assign(_off, val);
    }
    template <typename T, class M>
    void BiArray<T, M>::append(const T *val, size_t s) {
		if (!_capacity) reserve(s + 2, 1);
		if (_capacity <= s + offset() + size()) _expandTail(s + offset() + size());
        M::copy(_end, val, s); 
		_end += s;
    }
    template <typename T, class M>
    void BiArray<T, M>::pile(const T *val, size_t s) {
		if (!_capacity) reserve(s + 2, 1);
		if (_capacity <= s + size() + _remain()) _expandHead(s + size() + _remain());
        _off -= s; 
		M::copy(_off, val, s);
    }
    template <typename T, class M>
    void BiArray<T, M>::append(const BiArray &array) { if (!array.empty()) append(array.ptr(), array.size()); }
    template <typename T, class M>
    void BiArray<T, M>::pile(const BiArray &array) { if (!array.empty()) pile(array.ptr(), array.size()); }
    template <typename T, class M>
    void BiArray<T, M>::set(size_t idx, const T &val) { set(begin()+idx, val); }
    template <typename T, class M>
    void BiArray<T, M>::set(sarr_iter<T> iter, const T &val) {
        if (iter._ptr < _end) {
			if (_off <= iter._ptr) {
				M::release(iter._ptr, 1);
				M::assign(iter._ptr, val);
			}
			else put(val);
        }
        else add(val);
    }
    template <typename T, class M>
    void BiArray<T, M>::exchange(sarr_iter<T> iter1, sarr_iter<T> iter2) { iter1.swap(iter1, iter2); }
    template <typename T, class M>
    void BiArray<T, M>::exchange(size_t idx1, size_t idx2) { exchange(begin()+idx1, begin()+idx2); }
    template <typename T, class M>
	sarr_iter<T> BiArray<T, M>::insert(size_t idx, const T &val) {
		auto p = _off + idx;
        if (p < _end) {
			if (_capacity <= size() + 1) { _expandTail(size() + 1); p = _off + idx; }
			M::shift(p + 1, p, _end - p);
			M::assign(p, val); 
			++_end;
			return sarr_iter<T>(p);
        }
		else { add(val); return end() - 1; }
    }
	template <typename T, class M>
	sarr_iter<T> BiArray<T, M>::insert(sarr_iter<T> iter, const T& val) {
		if (iter < end()) {
			auto p = iter._ptr;
			auto idx = p - _begin;
			if (_capacity <= size() + 1) { _expandTail(size() + 1); p = _off + idx; }
			M::shift(p + 1, p, _end - p);
			M::assign(p, val); 
			++_end;
			return sarr_iter<T>(p);
		}
		else { add(val); return end() - 1; }
	}
    template <typename T, class M>
	sarr_iter<T> BiArray<T, M>::insert(size_t idx, T *val, size_t s) {
		auto p = _off + idx;
        if (p < _end) {
			auto s_ = _end - _begin + s;
			if (_capacity <= s_) { _expandTail(s); p = _off + idx; }
			M::shift(p + s, p, _end - p);
            M::copy(p, val, s); _end += s;
			return sarr_iter<T>(p);
        }
		else { append(val, s); return end() - s; }
    }
    template <typename T, class M>
	sarr_iter<T> BiArray<T, M>::insert(size_t idx, const BiArray &array) {
		return insert(idx, array._off, array.size());
    }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::remove(sarr_iter<T> beg, sarr_iter<T> end) {
        auto b = beg._ptr, e = end._ptr;
		if (_end <= b) return sarr_iter<T>(_end);
        if (_end <= e) e = _end;
        auto len = e - b; 
		M::release(b, len);
        if (e < _end) M::shift(b, e, _end-e); 
		_end -= len;
        return sarr_iter<T>(b);
    }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::remove(srange range) {
		return remove(begin() + range.begin, begin() + range.end);
    }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::remove(size_t off, size_t len) {
		return remove(begin() + off, begin() + off + len);
    }
    template <typename T, class M>
    sarr_iter<T> BiArray<T, M>::removeAt(size_t idx) { return remove(idx, 1); }
    template <typename T, class M>
    void BiArray<T, M>::trim(size_t s) {
		if (_end <= _off + s) s = _end - _off;
        M::release(_end-s, s); _end -= s;
    }
    template <typename T, class M>
    void BiArray<T, M>::cut(size_t s) {
		if (_end <= _off + s) s = _end - _off;
        M::release(_off, s); _off += s;
    }
    template <typename T, class M>
    T BiArray<T, M>::get() {
        if (!empty()) {
			T tmp;
			M::assign(&tmp, last());
			resize(size() - 1);
            return tmp;
        }
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T, class M>
    T BiArray<T, M>::pick() {
        if (!empty()) {
            T tmp;
			M::assign(&tmp, first());
			setOffset(offset() + 1);
            return tmp;
        }
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T, class M>
    void BiArray<T, M>::clear() { resize(0); }
    template <typename T, class M>
    void BiArray<T, M>::reset(const T &val) {
        if (!empty()) {
            M::release(_off, size());
			auto p = _off;
			while (p < _end) { M::assign(p, val); ++p; }
        }
    }
    template <typename T, class M>
    void BiArray<T, M>::sort(Comparer comp) { std::sort(begin(), end(), comp); }
    template <typename T, class M>
    void BiArray<T, M>::copy(const T *ptr, size_t s) {
        clear(); resize(s);
		auto p = _off;
		while (p < _end) { M::assign(p, *ptr); ++p; ++ptr; }
    }
    template <typename T, class M>
    void BiArray<T, M>::copyTo(BiArray<T, M> &array) { array.copy(_off, size()); }
    template <typename T, class M>
    void BiArray<T, M>::moveTo(BiArray<T, M> &array) {
        array._capacity = _capacity;
        array._begin = _begin;
        array._end = _end;
        array._off = _off;
        discard();
    }
    template <typename T, class M>
    void BiArray<T, M>::swap(BiArray<T, M> &array) {
        auto tmpc = _capacity; _capacity = array._capacity; array._capacity = tmpc;
        auto tmpb = _begin; _begin = array._begin; array._begin = tmpb ;
        auto tmpe = _end; _end = array._end; array._end = tmpe;
        auto tmpo = _off; _off = array._off; array._off = tmpo;
    }
    template <typename T, class M>
    bool BiArray<T, M>::contain(const T &val) const { return find(val) != NOT_FOUND; }
    template <typename T, class M>
    size_t BiArray<T, M>::find(const T &val, size_t off) const {
		auto it = _off + off;
        while(it < _end) { if (E_ == val) return it-_off; else NEXT_; }
        return NOT_FOUND;
    }
    template <typename T, class M>
    size_t BiArray<T, M>::rfind(const T &val, size_t off) const {
		auto it = _end - off - 1;
        while (_off <= it) { if (E_ == val) return it-_off; else PREV_; }
        return NOT_FOUND;
    }
    template <typename T, class M>
    void BiArray<T, M>::resize(size_t s, bool head) {
        if (s) {
			if (!_capacity) reserve(s + 2, 1);
			else {
				if (head && _capacity <= s + _remain()) _expandHead(s + _remain());
				else if (!head && _capacity <= s + offset()) _expandTail(s + offset());
			}
        }
        auto s_ = size();
		if (head) {
			if (s < s_) M::release(_off, s_ - s);
			else if (s_ < s) M::init(_end - s, s - s_);
			_off = _end - s;
		}
		else {
			if (s < s_) M::release(_off + s, s_ - s);
			else if (s_ < s) M::init(_off + s_, s - s_);
			_end = _off + s;
		}
    }
    template <typename T, class M>
    void BiArray<T, M>::resize(size_t s, const T &val, bool head) {
        auto s_ = size();
        resize(s, head);
        if (s_ < s) {
			if (head) {
				auto p = _off, p_ = _end - s_;
				while (p < p_) { M::assign(p, val); ++p; }
			}
			else {
				auto p = _off + s_;
				while (p < _end) { M::assign(p, val); ++p; }
			}
        }
    }
    template <typename T, class M>
    void BiArray<T, M>::reserve(size_t s, size_t off) {
        auto s_ = size();
		if (off == -1) off = (s - s_) / 2;
        if (_capacity < s) {
            if (_begin) {
                auto tmp = M::alloc(s);
                if (s_) {
                    M::init(tmp+off, s_);
                    M::copy(tmp+off, _off, s_);
                    M::release(_off, s_);
                }
                M::dealloc(_begin);
                _begin = tmp;
            }
            else _begin = M::alloc(s); 
			_capacity = s; _off = _begin + off; _end = _off + s_;
        }
    }
    template <typename T, class M>
    void BiArray<T, M>::release() {
        if (_begin) {
            if (_off < _end) M::release(_off, _end-_off);
            M::dealloc(_begin);
        }
        discard();
    }
    template <typename T, class M>
    void BiArray<T, M>::discard() {
        _capacity = 0; _begin = nullptr; _off = nullptr; _end = nullptr;
    }
    template <typename T, class M>
    bool BiArray<T, M>::operator < (const BiArray &array) const {
        if (size() != array.size()) return size() < array.size();
        if (!empty()) {
            auto it_ = array.begin();
            sforeach(*this) {
                if (!(E_ == (*it_))) return E_ < (*it_); else ++it_;
            }
        }
        return false;
    }
    template <typename T, class M>
    bool BiArray<T, M>::operator == (const BiArray &array) const {
        if (size() != array.size()) return false;
        if (!empty()) {
            auto it_ = array.begin();
            sforeach(*this) {
                if (!(E_ == (*it_))) return false; else ++it_;
            }
        }
        return true;
    }
}

#endif