#ifndef SLIB_LIST_H
#define SLIB_LIST_H
#include "sbasic/listiter.h"
#include "sbasic/exception.h"
namespace slib {
    template <typename T>
    class List {
        typedef ListIterator<T> Iter;
        typedef ListCIterator<T> CIter;
        typedef std::function<bool(const T& t1, const T& t2)> Comparer;
    private:
        void _expand(size_t s);
        ListData<T>* _fill();
        ListData<T>* _fill(T&& val);
        ListData<T>* _fill(const T& val);
        void _vacate(ListData<T>* p);
        size_t _index(ListIterator<T> iter);
    
    
    
    protected:
        size_t _capacity, _size;
        ListData<T>* _begin, * _end, * _vacant;
    
    public:
        List();
        List(size_t s);
        List(size_t s, const T& val);
        List(std::initializer_list<T> li);
        List(List<T>&& list) noexcept;
        List(const List<T>& list);
        ~List();
        List& operator = (const List<T>& list);
        List& operator = (List<T>&& list);
        T& operator[] (sinteger idx);
        const T& operator[] (sinteger idx) const;
        T& at(sinteger idx);
        const T& at(sinteger idx) const;
        T& first();
        const T& first() const;
        T& last();
        const T& last() const;
        ListIterator<T> begin();
        ListCIterator<T> begin() const;
        ListIterator<T> end();
        ListCIterator<T> end() const;
        List<T> sublist(size_t off, size_t len = -1);
        List<T> sublist(ListIterator<T> beg, ListIterator<T> end);
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        void add(T&& val);
        void add(const T& val);
        template<class... Args>
        void add(Args... args) { add(T(args...)); }
        void put(T&& val);
        void put(const T& val);
        template<class... Args>
        void put(Args... args) { put(T(args...)); }
        void append(const T* val, size_t s);
        void pile(const T* val, size_t s);
        void append(const List<T>& list);
        void pile(const List<T>& list);
        void set(sinteger idx, const T& val);
        void set(ListIterator<T> iter, const T& val);
        void exchange(ListIterator<T> iter1, ListIterator<T> iter2);
        void exchange(size_t idx1, size_t idx2);
        ListIterator<T> insert(sinteger idx, const T& val);
        ListIterator<T> insert(ListIterator<T> iter, const T& val);
        ListIterator<T> remove(const T& val);
        ListIterator<T> remove(ListIterator<T> beg, ListIterator<T> end);
        ListIterator<T> remove(size_t off, size_t len);
        ListIterator<T> removeAt(sinteger idx);
        void trim(size_t s);
        void cut(size_t s);
        T get(sinteger idx) const;
        T pop();
        T pick();
        void clear();
        void reset(const T& val);
        void sort(Comparer comp = sortAsc<T>);
        void copy(const T* ptr, size_t s);
        void copyTo(List& list) const;
        void moveTo(List& list);
        void swap(List& list);
        bool contain(const T& val) const;
        ListIterator<T> locate(const T& val, size_t off = 0);
        ListCIterator<T> locate(const T& val, size_t off = 0) const;
        size_t find(const T& val, size_t off = 0) const;
        size_t rfind(const T& val, size_t off = 0) const;
        void resize(size_t s);
        void resize(size_t s, const T& val);
        void reserve(size_t s);
        void release();
        void discard();
        bool operator < (const List<T>& list) const;
        bool operator == (const List<T>& list) const;
    };
    template<typename T>
    extern inline String toString(const List<T>& list, const char *sep = ",") { 
        if (list.empty()) return "";
        String str;
        str << list[0];
        sforin(it, list.begin() + 1, list.end()) str << sep << $_;
        return str;
    }
}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::List<T>& list) {
    if (list.empty()) return os;
    os << list[0];
    sforin(it, list.begin() + 1, list.end()) os << "," << $_;
    return os;
}

    /*
    
	template <typename T>
	void List<T>::_expand(size_t s) {
		if (!_capacity) reserve(s + 2);
		else {
			auto cap = _capacity;
			while (cap <= s) cap <<= 1;
			reserve(cap);
		}
	}
	template <typename T>
	ListData<T>* List<T>::_fill() {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init();
		return p;
	}
	template <typename T>
	ListData<T>* List<T>::_fill(T&& val) {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init(std::forward<T&&>(val));
		return p;
	}
	template <typename T>
	ListData<T>* List<T>::_fill(const T& val) {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init(val);
		return p;
	}
	template <typename T>
	void List<T>::_vacate(ListData<T>* p) {
		p->release();
		p->next->prev = p->prev;
		if (p->prev) p->prev->next = p->next;
		else _begin = p->next;
		_vacant->insertPrev(p);
		_vacant = p;
	}
	template <typename T>
	size_t List<T>::_index(ListIterator<T> iter) {
		auto idx = 0;
		while (iter._ptr != _begin) { ++idx; --iter; }
		return idx;
	}
    template <typename T>
    List<T>::List() : _capacity(0), _size(0), _begin(nullptr), _end(nullptr), _vacant(nullptr) {}
    template <typename T>
    List<T>::List(size_t s) : List<T>() { resize(s); }
    template <typename T>
    List<T>::List(size_t s, const T &val) : List<T>(s) {
        if (s) { 
			auto p = _begin; 
			while (p < _end) { *p = val; p = p->next; }
		}
    }
    template <typename T>
    List<T>::List(std::initializer_list<T> li) : List<T>(li.size()) {
		if (_size) {
			sforeach2(*this, li) E1_ = E2_;
		}
    }
    template <typename T>
    List<T>::List(List<T> &&list) : List<T>() {
        _capacity = list._capacity;
        _size = list._size;
        _begin = list._begin;
        _end = list._end;
        _vacant = list._vacant;
        list.discard();
    }
    template <typename T>
    List<T>::List(const List<T> &list) : List<T>(list.size()) {
        if (!list.empty()) {
			sforeach2(*this, list) E1_ = E2_;
        }
    }
    template <typename T>
    List<T>::~List() { release(); }
    template <typename T>
    List<T> &List<T>::operator = (const List<T> &list) {
        resize(list.size());
        if (_size) {
			sforeach2(*this, list) E1_ = E2_;
        }
		return *this;
    }
    template <typename T>
    List<T> &List<T>::operator = (List<T> &&list) {
        release();
        _capacity = list._capacity;
        _size = list._size;
        _begin = list._begin;
        _end = list._end;
        _vacant = list._vacant;
        list.discard();
        return *this;
    }
    template <typename T>
    T &List<T>::operator[] (sinteger idx) { return at(idx); }
    template <typename T>
    const T &List<T>::operator[] (sinteger idx) const { return at(idx); }
    template <typename T>
    T &List<T>::at(sinteger idx) {
        if (_size) {
			if (idx < 0 && -idx <= _size) return *(end() - (-idx));
			else if (idx < _size) return *(begin() + idx);
        }
        throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    const T &List<T>::at(sinteger idx) const {
        if (_size) {
			if (idx < 0 && -idx < _size) return *(end() - (-idx));
			else if (idx < _size) return *(begin() + idx);
        }
        throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    T &List<T>::first() { return at(0); }
    template <typename T>
    const T &List<T>::first() const { return at(0); }
    template <typename T>
    T &List<T>::last() { return at(-1); }
    template <typename T>
    const T &List<T>::last() const { return at(-1); }
    template <typename T>
    ListIterator<T> List<T>::begin() { return ListIterator<T>(_begin); }
    template <typename T>
    ListCIterator<T> List<T>::begin() const { return ListCIterator<T>(_begin); }
    template <typename T>
    ListIterator<T> List<T>::end() { return ListIterator<T>(_end); }
    template <typename T>
    ListCIterator<T> List<T>::end() const { return ListCIterator<T>(_end); }
    template <typename T>
    List<T> List<T>::sublist(size_t off, size_t len) {
		return sublist(begin() + off, len == -1 ? end() : begin() + off + len);
    }
    template <typename T>
    List<T> List<T>::sublist(ListIterator<T> beg, ListIterator<T> end) {
        List<T> tmp;
        while (beg < end) { tmp.add(*beg); ++beg; }
        return tmp;
    }
    template <typename T>
    bool List<T>::empty() const { return !_size; }
    template <typename T>
    size_t List<T>::size() const { return _size; }
    template <typename T>
    size_t List<T>::capacity() const { return _capacity; }
    template <typename T>
	void List<T>::add(T&& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(std::forward<T&&>(val));
		_end->insertPrev(p);
		if (!_size) _begin = _end->prev;
		++_size;
	}
    template <typename T>
	void List<T>::add(const T& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(val);
		_end->insertPrev(p);
		if (!_size) _begin = _end->prev;
		++_size;
	}
	template <typename T>
	void List<T>::put(T&& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(std::forward<T&&>(val));
		_begin->insertPrev(p);
		_begin = p;
		++_size;
	}
	template <typename T>
	void List<T>::put(const T& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(val);
		_begin->insertPrev(p);
		_begin = p;
		++_size;
	}
	template <typename T>
	void List<T>::append(const T* val, size_t s) {
		if (val && s) {
			sforin(i, 0, s) { add(*val); ++val; }
		}
	}
	template <typename T>
	void List<T>::pile(const T* val, size_t s) {
		if (val && s) {
			sforin(i, 0, s) { put(*val); ++val; }
		}
	}
    template <typename T>
	void List<T>::append(const List<T>& list) {
		if (list._size) {
			sforeach(list) add(E_);
		}
	}
	template <typename T>
	void List<T>::pile(const List<T>& list) {
		if (list._size) {
			sforeach(list) put(E_);
		}
	}
    template <typename T>
	void List<T>::set(sinteger idx, const T& val) { set((idx < 0 ? end() : begin()) + idx, val); }
    template <typename T>
	void List<T>::set(ListIterator<T> iter, const T& val) { 
		iter._ptr->release();
		iter._ptr->init(val);
	}
    template <typename T>
	void List<T>::exchange(ListIterator<T> iter1, ListIterator<T> iter2) { 
		iter1.swap(iter1, iter2);
	}
    template <typename T>
    void List<T>::exchange(size_t idx1, size_t idx2) {
		exchange(begin() + idx1, begin() + idx2);
	}
    template <typename T>
	ListIterator<T> List<T>::insert(sinteger idx, const T& val) { return insert((idx < 0 ? end() : begin()) + idx, val); }
	template <typename T>
	ListIterator<T> List<T>::insert(ListIterator<T> iter, const T& val) {
		if (iter < end()) {
			if (_capacity <= _size + 2) {
				auto idx = _index(iter);
				_expand(_size + 2);
				iter = begin() + idx;
			}
			auto p = iter.ptr();
			auto p_ = _fill(val);
			p->insertPrev(p_);
			if (!(p_->prev)) _begin = p_;
			++_size;
			return ListIterator<T>(p_);
		}
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
    template <typename T>
	ListIterator<T> List<T>::remove(ListIterator<T> beg, ListIterator<T> end) {
		auto p = beg._ptr;
		while (beg < end) {
			auto p_ = beg._ptr;
			++beg;
			_vacate(p_);
			--_size;
		}
        return ListIterator<T>(p->next);
	}
	template <typename T>
	ListIterator<T> remove(const T& val);
    template <typename T>
	ListIterator<T> List<T>::remove(size_t off, size_t len) {
		return remove(begin() + off, len == -1 ? end() : begin() + off + len);
	}
    template <typename T>
	ListIterator<T> List<T>::removeAt(sinteger idx) { return remove((idx < 0 ? end() : begin()) + idx, (idx < 0 ? end() : begin()) + idx + 1); }
    template <typename T>
    void List<T>::trim(size_t s) { 
		if (s <= _size) remove(end() - s, end());
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
    template <typename T>
    void List<T>::cut(size_t s) {
		if (s <= _size) remove(begin(), begin() + s);
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
	template <typename T>
	T List<T>::get(sinteger idx) const { return at(idx); }
	template <typename T>
    T List<T>::pop() {
        if (!empty()) { 
			auto val = last();
			trim(1);
			return val;
		}
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    T List<T>::pick() {
        if (!empty()) {
			auto val = first();
			cut(1);
			return val;
		}
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
	template <typename T>
    void List<T>::clear() { resize(0); }
    template <typename T>
    void List<T>::reset(const T &val) { sforeach(*this) E_ = val; }
    template <typename T>
    void List<T>::sort(Comparer comp) { std::sort(begin(), end(), comp); }
    template <typename T>
    void List<T>::copy(const T *ptr, size_t s) {
        resize(s);
        sforeach(*this) { E_ = *ptr; ++ptr; }
    }
    template <typename T>
    void List<T>::copyTo(List<T> &list) const {
        if (_size) {
            list.resize(_size);
			sforeach2(list, *this) E1_ = E2_;
        }
    }
    template <typename T>
    void List<T>::moveTo(List<T> &list) {
		list.release();
        list._capacity = _capacity;
        list._size = _size;
        list._begin = _begin;
        list._end = _end;
        list._vacant = _vacant;
        discard();
    }
    template <typename T>
    void List<T>::swap(List<T> &list) {
        auto tmpc = _capacity; _capacity = list._capacity; list._capacity = tmpc;
        auto tmps = _size; _size = list._size; list._size = tmps;
        auto tmpb = _begin; _begin = list._begin; list._begin = tmpb ;
        auto tmpe = _end; _end = list._end; list._end = tmpe;
        auto tmpv = _vacant; _vacant = list._vacant; list._vacant = tmpv;
    }
    template <typename T>
    bool List<T>::contain(const T &val) const { return find(val) != NOT_FOUND; }
	template <typename T>
	ListIterator<T> List<T>::locate(const T& val, size_t off) {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it; else NEXT_; }
		return end();
	}
	template <typename T>
	ListCIterator<T> List<T>::locate(const T& val, size_t off) const {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it; else NEXT_; }
		return end();
	}
	template <typename T>
    size_t List<T>::find(const T &val, size_t off) const {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it - begin(); else NEXT_; }
        return NOT_FOUND;
    }
    template <typename T>
    size_t List<T>::rfind(const T &val, size_t off) const {
		auto it = end() - off - 1;
		while (begin() <= it) { if (E_ == val) return it - begin(); else PREV_; }
        return NOT_FOUND;
    }
    template <typename T>
    void List<T>::resize(size_t s) {
		if (s) {
			if (!_capacity) reserve(s + 2);
			else if (_capacity + 1 <= s) _expand(s);
		}
        auto s_ = _size;
        if (s < s_) {
			auto it = begin() + s;
			auto p = it._ptr;
			_end->prev = p->prev;
			if (p->prev) p->prev->next = _end;
			else _begin = _end;
			while (p < _end) {
				auto p_ = p;
				p = p->next;
				_vacate(p_);
				--_size;
			}
        }
		else if (s_ < s) {
			sforin(i, s_, s) {
				auto p = _fill();
				_end->insertPrev(p);
				if (!_begin->next) _begin = p;
				++_size;
			}
		}
    }
    template <typename T>
    void List<T>::resize(size_t s, const T &val) {
        auto s_ = _size;
        resize(s);
        if (s_ < s) {
			auto it = begin() + s_;
            while (it < end()) { E_ = val; NEXT_; }
        }
    }
    template <typename T>
    void List<T>::reserve(size_t s) {
        if (_capacity < s) {
            if (_capacity) {
				List<T> tmp;
				tmp.reserve(s);
				if (_size) tmp.append(*this);
				tmp.swap(*this);
			}
            else {
				_vacant = Memory<ListData<T>>::alloc(s);
				_vacant->prev = nullptr;
				auto p = _vacant;
				sforin(i, 0, s - 1) { p->next = p + 1; p = p->next; }
				_end = p;
				_end->prev = nullptr;
				_end->next = nullptr;
				_begin = _end;
			}
            _capacity = s;
        }
    }
    template <typename T>
    void List<T>::release() {
		if (_size) { 
			auto p = _begin;
			while (p->next && p < _end) { p->release(); p = p->next; }
		}
		if (_capacity) Memory<ListData<T>>::dealloc(_end - _capacity + 1);
		discard();
    }
    template <typename T>
    void List<T>::discard() {
        _capacity = 0; _size = 0; _begin = nullptr; _end = nullptr; _vacant = nullptr;
    }
    template <typename T>
    bool List<T>::operator < (const List &list) const {
		if (size() != list.size()) return size() < list.size();
		sforeach2(*this, list) {
			if (!(E1_ == E2_)) return E1_ < E2_;
		}
		return false;
	}
    template <typename T>
	bool List<T>::operator == (const List& list) const {
		if (size() != list.size()) return false;
		sforeach2(*this, list) {
			if (!(E1_ == E2_)) return false;
		}
		return true;
	}
}
*/
#endif