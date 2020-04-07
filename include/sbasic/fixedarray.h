#ifndef SLIB_FIXARRAY_H
#define SLIB_FIXARRAY_H

namespace slib {
    
    template <typename T, size_t S, class M=SMemory<T>>
    class FixedArray {
	public:
		typedef T value_type;
		typedef std::function<bool(const T & t1, const T & t2)> Comparer;

    private:
        void _reserve();
        
    protected:
        size_t _capacity;
        T *_begin;
        
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
        
        bool empty() const;
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
        
        bool operator < (const FixedArray &array) const;
        bool operator == (const FixedArray &array) const;
    };
    
    /*============================================================*/
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray() : _capacity(S+1) { _reserve(); }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(const T &val) : FixedArray<T, S, M>() {
        auto p = _begin;
		sforin(i, 0, S) M::assign(p, val);
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(std::initializer_list<T> li) : FixedArray<T, S, M>() {
        if (li.size() == S) {
            auto p = _begin; auto it = li.begin();
			sforin(i, 0, S) { M::assign(p, E_); ++p; NEXT_; }
        }
        else throw SException(ERR_INFO);
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::FixedArray(const FixedArray<T, S, M> &array) : FixedArray<T, S, M>() {
        auto p = _begin; auto p_ = array._begin;
		sforin(i, 0, S) { M::assign(p, *p_); ++p; ++p_; }
    }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M>::~FixedArray() { M::dealloc(_begin); }
    template <typename T, size_t S, class M>
    FixedArray<T, S, M> &FixedArray<T, S, M>::operator=(const FixedArray<T, S, M> &array) {
        auto p = _begin; auto p_ = array._begin;
		sforin(i, 0, S) { M::assign(p, *p_); ++p; ++p_; }
    }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::_reserve() {
        if (_capacity) _begin = M::alloc(_capacity);
    }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::operator[] (int idx) { return at(idx); }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::operator[] (int idx) const { return at(idx); }
    template <typename T, size_t S, class M>
    T &FixedArray<T, S, M>::at(int idx) {
		auto p = idx < 0 ? _begin + _capacity + idx : _begin + idx;
        if (p < _begin || _begin+_capacity <= p)
            throw SException(ERR_INFO, SLIB_RANGE_ERROR);
        return *p;
    }
    template <typename T, size_t S, class M>
    const T &FixedArray<T, S, M>::at(int idx) const {
		auto p = idx < 0 ? _begin + _capacity + idx : _begin + idx;
        if (p < _begin || _begin+_capacity <= p)
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
    T *FixedArray<T, S, M>::ptr(size_t idx) const{ return _begin; }
    template <typename T, size_t S, class M>
    sarr_iter<T> FixedArray<T, S, M>::begin() { return SArrayIterator<T>(_begin); }
    template <typename T, size_t S, class M>
    sarr_citer<T> FixedArray<T, S, M>::begin() const { return SArrayIterator<T>(_begin); }
    template <typename T, size_t S, class M>
    sarr_iter<T> FixedArray<T, S, M>::end() { return SArrayIterator<T>(_begin+_capacity); }
    template <typename T, size_t S, class M>
    sarr_citer<T> FixedArray<T, S, M>::end() const { return SArrayIterator<T>(_begin+_capacity); }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::empty() const { return !_capacity; }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::size() const { return 0<_capacity?_capacity-1:0; }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::capacity() const { return _capacity; }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::reset(const T &val) { sforeach(*this) E_ = val; }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::copy(const T *ptr) {
		auto p = _begin;
		sforin(i, 0, S) { M::assign(p, *ptr); ++p; ++ptr; }
    }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::copyTo(FixedArray<T, S, M> &array) const { array.copy(_begin); }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::moveTo(FixedArray<T, S, M> &array) {
        array._capacity = _capacity;
        array._begin = _begin;
        array._begin = nullptr;
    }
    template <typename T, size_t S, class M>
    void FixedArray<T, S, M>::swap(FixedArray<T, S, M> &array) {
        auto tmpc = _capacity; _capacity = array._capacity; array._capacity = tmpc;
        auto tmpb = _begin; _begin = array._begin; array._begin = tmpb;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::contain(const T &val) const { return find(val) != NOT_FOUND; }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::find(const T &val, size_t off) const {
		auto p = _begin + off;
        sforin(i, off, _capacity-1) { if (*p == val) return i; else ++p; }
        return NOT_FOUND;
    }
    template <typename T, size_t S, class M>
    size_t FixedArray<T, S, M>::rfind(const T &val, size_t off) const {
		auto p = _begin + _capacity + off;
        srforin(i, _capacity, off) { if (*p == val) return i; else ++p; }
        return NOT_FOUND;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::operator < (const FixedArray<T, S, M> &array) const {
        if (_capacity != array._capacity) return _capacity < array._capacity;
        auto p = _begin; auto p_ = array._begin;
        srforin(i, 0, _capacity) {
            if (!(*p == *p_)) return *p < *p_;
            ++p; ++p_;
        }
        return false;
    }
    template <typename T, size_t S, class M>
    bool FixedArray<T, S, M>::operator == (const FixedArray<T, S, M> &array) const {
        if (_capacity != array._capacity) return _capacity < array._capacity;
        auto p = _begin; auto p_ = array._begin;
        srforin(i, 0, _capacity) {
            if (!(*p == *p_)) return false;
            ++p; ++p_;
        }
        return true;
    }
}

#endif
