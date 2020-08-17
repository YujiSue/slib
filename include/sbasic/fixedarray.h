#ifndef SLIB_FIXARRAY_H
#define SLIB_FIXARRAY_H

namespace slib {
    
    template <typename T, size_t S, class M=SMemory<T>>
    class FixedArray {
	public:
		typedef T value_type;
		typedef std::function<bool(const T & t1, const T & t2)> Comparer;

    protected:
        T *_begin, _end;
        
    public:
        FixedArray();
        FixedArray(const T &val);
        FixedArray(std::initializer_list<T> li);
        FixedArray(const FixedArray &s);
        ~FixedArray();
        
        FixedArray &operator=(const FixedArray &array);
        
        T &operator[] (int idx);
        const T &operator[] (int idx) const;
        T &at(int idx);
        const T &at(int idx) const;
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
        
        size_t size() const;
        size_t capacity() const;
        
        void reset(const T &val);
        void copy(const T *ptr);
        void copyTo(FixedArray &array) const;
        void moveTo(FixedArray &array);
        void swap(FixedArray &array);
        
        bool contain(const T &val) const;
        size_t find(const T &val, size_t off = 0) const;
        size_t rfind(const T &val, size_t off = 0) const;
        
        bool operator<(const FixedArray<T, S, M> &array) const;
        bool operator==(const FixedArray<T, S, M> &array) const;
    };
    
    /*============================================================*/
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray() { 
		_begin = M::alloc(S + 1); _end = _begin + S;
	}
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(const T &val) : FixedArray<T, S, M>() {
		sforeach(*this) E_ = val;
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(std::initializer_list<T> li) : FixedArray<T, S, M>() {
		sforeach2(*this, li) E1_ = E2_;
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(const FixedArray<T, S, M> &array) : FixedArray<T, S, M>() {
		sforeach2(*this, array) E1_ = E2_;
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::~FixedArray() { if (_begin) M::dealloc(_begin); }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M> &FixedArray<T, S, M>::operator=(const FixedArray<T, S, M> &array) {
		sforeach2(*this, array) E1_ = E2_; return *this;
    }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::operator[] (int idx) { return at(idx); }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::operator[] (int idx) const { return at(idx); }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::at(int idx) {
		auto p = idx < 0 ? _end + idx : _begin + idx;
        if (p < _begin || _end <= p)
            throw SException(ERR_INFO, SLIB_RANGE_ERROR);
        return *p;
    }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::at(int idx) const {
		auto p = idx < 0 ? _end + idx : _begin + idx;
		if (p < _begin || _end <= p)
            throw SException(ERR_INFO, SLIB_RANGE_ERROR);
        return *p;
    }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::first() { return at(0); }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::first() const { return at(0); }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::last() { return at(-1); }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::last() const { return at(-1); }    
    template <typename T, size_t S, class M>
    T *FixedArray<T, S, M>::ptr() const{ return _begin; }
    template <typename T, size_t S, class M>
    T *FixedArray<T, S, M>::ptr(size_t idx) const{ return _begin + idx; }
    template <typename T, size_t S, class M>
    sarr_iter<T> FixedArray<T, S, M>::begin() { return SArrayIterator<T>(_begin); }
    template <typename T, size_t S, class M>
    sarr_citer<T> FixedArray<T, S, M>::begin() const { return SArrayIterator<T>(_begin); }
    template <typename T, size_t S, class M>
    sarr_iter<T> FixedArray<T, S, M>::end() { return SArrayIterator<T>(_end); }
    template <typename T, size_t S, class M>
    sarr_citer<T> FixedArray<T, S, M>::end() const { return SArrayIterator<T>(_end); }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::size() const { return S; }
    template <typename T, size_t S, class M>
	size_t FixedArray<T, S, M>::capacity() const { return S + 1; }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::reset(const T &val) { sforeach(*this) E_ = val; }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::copy(const T *ptr) { M::copy(_begin, ptr, S); }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::copyTo(FixedArray<T, S, M> &array) const { array.copy(_begin); }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::moveTo(FixedArray<T, S, M> &array) {
        array._begin = _begin;
		array._end = _end;
        _begin = nullptr;
		_end = nullptr;
    }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::swap(FixedArray<T, S, M> &array) {
        auto tmpb = _begin; _begin = array._begin; array._begin = tmpb;
		auto tmpe = _end; _end = array._end; array._end = tmpe;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::contain(const T &val) const { return find(val) != NOT_FOUND; }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::find(const T &val, size_t off) const {
		auto p = _begin + off;
        while(p < _end) { if (*p == val) return p - _begin; else ++p; }
        return NOT_FOUND;
    }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::rfind(const T &val, size_t off) const {
		auto p = _end - off - 1;
        while(_begin <= p) { if (*p == val) return p - _begin; else --p; }
        return NOT_FOUND;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::operator < (const FixedArray<T, S, M> &array) const {
		if (_begin == nullptr || array._begin == nullptr) throw SException(ERR_INFO, SLIB_NULL_ERROR);
		sforeach2(*this, array) {
			if (E1_ != E2_) return E1_ < E2_;
		}
        return false;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::operator == (const FixedArray<T, S, M> &array) const {
		if (_begin == nullptr || array._begin == nullptr) throw SException(ERR_INFO, SLIB_NULL_ERROR);
		sforeach2(*this, array) {
			if (E1_ != E2_) return false;
		}
        return true;
    }
}

#endif
