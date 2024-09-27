#include "sbasic/arrayiter.h"
#include "sbasic/exception.h"
namespace slib {
    template<typename T>
    class RecycleArray {
        typedef std::function<bool(const T& t1, const T& t2)> Comparer;
    protected:
        /*
        *    |---------------|- - - |- - - - |
        *   _ptr           _end   _last
        *    <----        capacity       ---->
        *    <----     used     ---->
        *    <---- size ---->
        */
        T* _ptr, *_end, * _last;
        size_t _capacity;

    public:
        RecycleArray() : _ptr(nullptr), _end(nullptr), _last(nullptr), _capacity(0) {}
        RecycleArray(const size_t sz) : RecycleArray<T>() { resize(sz); }
        RecycleArray(RecycleArray<T>&& array) noexcept : RecycleArray<T>() { swap(array); }
        RecycleArray(const RecycleArray<T>& array) : RecycleArray<T>(array.size()) {
            Memory<T>::copy(_ptr, array.data(), array.size());
        }
        ~RecycleArray() {
            clear(); 
            release();
            if (_ptr) { Memory<T>::dealloc(_ptr); }
        }
        RecycleArray<T>& operator=(RecycleArray<T>&& array) noexcept { swap(array); return *this; }
        RecycleArray<T>& operator=(const RecycleArray<T>& array) {
            resize(array.size()); 
            Memory<T>::copy(_ptr, array.data(), array.size()); 
            return *this;
        }
        bool empty() const { return _ptr == _end; }
        size_t size() const { return _end - _ptr; }
        size_t used() const { return _last - _ptr; }
        size_t capacity() const { return _capacity; }
        void resize(const size_t s) {
            if (s == size()) return;
            if (!_capacity) {
                _capacity = s + 1;
                _ptr = Memory<T>::alloc(_capacity);
                if (!_ptr) throw MemAllocException();
                _end = _ptr;
                sforin(i, 0, s) { Memory<T>::init(_end); ++_end; }
                _last = _end;
            }
            else {
                if (s < _capacity) {
                    auto sz = size();
                    if (s < sz) {
                        sforin(i, s, sz) { (_end - 1)->clear(); --_end; }
                    }
                    else {
                        auto use = used();
                        if (use < s) {
                            sforin(i, use, s) { Memory<T>::init(_last); ++_last; }
                        }
                        _end = _ptr + s;
                    }
                }
                else {
                    RecycleArray<T> array;
                    auto cap = _capacity;
                    while (cap <= s) cap <<= 1;
                    array.reserve(cap);
                    array.resize(s);
                    Memory<T>::copy(array.data(), data(), size());
                    swap(array);
                }
            }
        }
        void reserve(const size_t sz) {
            if (sz <= _capacity) return;
            if (!_capacity) {
                _capacity = sz;
                _ptr = Memory<T>::alloc(_capacity);
                if (!_ptr) throw MemAllocException();
                _end = _ptr;
                _last = _ptr;
            }
            else {
                RecycleArray<T> array;
                array.reserve(sz);
                array.resize(used());
                array.resize(size());
                Memory<T>::copy(array.data(), data(), size());
                swap(array);
            }
        }

        ArrayIterator<T> iterAt(const int i) { 
            auto p = (i < 0 ? _end : _ptr) + i;
            if (p < _ptr || _end <= p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
            return ArrayIterator<T>(p); 
        }
        ArrayCIterator<T> iterAt(const int i) const { 
            auto p = (i < 0 ? _end : _ptr) + i;
            if (p < _ptr || _end <= p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
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
        ArrayIterator<T> end() { return ArrayIterator<T>(_end); }
        ArrayCIterator<T> end() const { return ArrayCIterator<T>(_end); }
        T& next() {
            resize(_end - _ptr + 1);
            return *(_end - 1);
        }
        void release() {
            sforin(p, _ptr, _last) Memory<T>::destroy(p);
        }
        void clear() {
            if (!_ptr) return;
            sforin(p, _ptr, _end) p->clear();
            _end = _ptr;
        }
        void sort(Comparer comp = sortAsc<T>) {
            std::sort(begin(), end(), [comp](const T& v1, const T& v2) { return comp(v1, v2); });
        }
        void swap(RecycleArray<T>& array) {
            Memory<T*>::swap(&_ptr, &array._ptr);
            Memory<T*>::swap(&_end, &array._end);
            Memory<T*>::swap(&_last, &array._last);
            Memory<size_t>::swap(&_capacity, &array._capacity);
        }
    };

    template<typename T>
    class RArray {
        typedef std::function<bool(const T& p1, const T& p2)> Comparer;
    protected:
        /*
        *    |---------------|- - - |- - - - |
        *   _ptr           _end   _last
        *    <----        capacity       ---->
        *    <----     used     ---->
        *    <---- size ---->
        */
        T* _ptr, * _end, * _last;
        size_t _capacity;

    public:
        RArray() : _ptr(nullptr), _end(nullptr), _last(nullptr), _capacity(0) {}
        RArray(const size_t sz) : RArray<T>() { resize(sz); }
        RArray(RArray<T>&& array) noexcept : RArray<T>() { swap(array); }
        RArray(const RArray<T>& array) : RArray<T>(array.size()) {
            Memory<T>::copy(_ptr, array.data(), array.size());
        }
        ~RArray() {
            clear();
            release();
            if (_ptr) { Memory<T>::dealloc(_ptr); }
        }
        RArray<T>& operator=(RArray<T>&& array) noexcept { swap(array); return *this; }
        RArray<T>& operator=(const RArray<T>& array) {
            resize(array.size());
            Memory<T>::copy(_ptr, array.data(), array.size());
            return *this;
        }
        bool empty() const { return _ptr == _end; }
        size_t size() const { return _end - _ptr; }
        size_t used() const { return _last - _ptr; }
        size_t capacity() const { return _capacity; }
        void resize(const size_t s) {
            if (s == size()) return;
            if (!_capacity) reserve(s + 1);
            else if (_capacity <= s + 1) {
                auto c = _capacity;
                while (c <= s + 1) { c << 1; }
                reserve(c);
            }
            if (s < used()) {
                auto sz = size();
                if (s < sz) {
                    sforin(i, s, sz) { (_end - 1)->clear(); --_end; }
                }
                else {
                    sforin(i, sz, s) { ++_end; }
                }
            }
            else {
                auto sz = used();
                sforin(i, sz, s) { Memory<T>::init(_last); ++_last; }
                _end = _last;
            }
        }
        void reserve(const size_t sz) {
            if (sz <= _capacity) return;
            if (!_capacity) {
                _capacity = sz;
                _ptr = Memory<T>::alloc(_capacity);
                if (!_ptr) throw MemAllocException();
                _end = _ptr;
                _last = _ptr;
            }
            else {
                RArray<T> array;
                array.reserve(sz);
                array.resize(used());
                array.resize(size());
                Memory<T>::copy(array.data(), data(), size());
                swap(array);
            }
        }
        
        ArrayIterator<T> iterAt(const int i) {
            auto p = (i < 0 ? _end : _ptr) + i;
            if (p < _ptr || _end <= p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
            return ArrayIterator<T>(p);
        }
        ArrayCIterator<T> iterAt(const int i) const {
            auto p = (i < 0 ? _end : _ptr) + i;
            if (p < _ptr || _end <= p) throw RangeException(outRangeErrorText("index", (int)(p - _ptr), 0, (int)(_end - _ptr)));
            return ArrayCIterator<T>(p);
        }
        T* data(const int i = 0) { return iterAt(i).ptr(); }
        const T* data(const int i = 0) const { return iterAt(i).ptr(); }
        T& at(const int i) { return *iterAt(i); }
        const T& at(const int i) const { return *iterAt(i); }
        T& operator[](const int i) { return at(i); }
        const T& operator[](const int i) const { return at(i); }
        ArrayIterator<T> begin() { return ArrayIterator<T>(_ptr); }
        ArrayCIterator<T> begin() const { return ArrayCIterator<T>(_ptr); }
        ArrayIterator<T> end() { return ArrayIterator<T>(_end); }
        ArrayCIterator<T> end() const { return ArrayCIterator<T>(_end); }
        
        void add(const T& val) {
            if (size() < used()) {
                Memory<T>::assign(_end, val);
                ++_end;
            }
            else {
                if (!_capacity) reserve(16);
                else if (size() + 1 == _capacity) reserve(_capacity << 1);
                Memory<T>::init(_end, val);
                ++_end; ++_last;
            }
        }
        void insert(const int idx, const T& val) {
            auto p = iterAt(idx).ptr();
            resize(size() + 1);
            p = iterAt(idx).ptr();
            auto e = _end - 1;
            while (p < e) {
                Memory<T>::swap(e - 1, e); --e;
            }
            Memory<T>::assign(p, val);
        }
        void release() {
            sforin(p, _ptr, _last) Memory<T>::destroy(p);
        }
        void clear() {
            if (!_ptr) return;
            sforin(p, _ptr, _end) p->clear();
            _end = _ptr;
        }
        void swap(RArray<T>& array) {
            Memory<T*>::swap(&_ptr, &array._ptr);
            Memory<T*>::swap(&_end, &array._end);
            Memory<T*>::swap(&_last, &array._last);
            Memory<size_t>::swap(&_capacity, &array._capacity);
        }
    };
}
