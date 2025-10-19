#ifndef SLIB_LIITER_H
#define SLIB_LIITER_H
#include "sbasic/container.h"
#include "sbasic/exception.h"
namespace slib {
    template<typename T>
    class ListIterator;
    template<typename T>
    class List;
    template<typename T>
    class ListData {
        friend List<T>;
        friend ListIterator<T>;
        friend ListCIterator<T>;
    protected:
        T _data;
        ListData* _prev, * _next;
    public:
        ListData() : _prev(nullptr), _next(nullptr) {}
        ListData(T&& t) noexcept : ListData() { Memory<T>::assign(&_data, std::forward<T&&>(t)); }
        ListData(const T& t) : ListData() { Memory<T>::assign(&_data, t); }
        ListData(const ListData& li) {
            _data = li._data; _prev = li._prev; _next = li._next;
        }
        ~ListData() {}
        ListData& operator=(const ListData& li) {
            _data = li._data; _prev = li._prev; _next = li._next; return *this;
        }
        T& value() { return _data; };
        const T& value() const { return _data; };
        void release() { _data.~T(); }
        void insertNext(ListData* nxt) {
            nxt->_next = _next;
            if (_next) _next->_prev = nxt;
            _next = nxt; nxt->_prev = this;
        }
        void insertPrev(ListData* prv) {
            if (_prev) _prev->_next = prv;
            prv->_prev = _prev;
            _prev = prv;
            prv->_next = this;
        }
    };
    template<typename T>
    class ListIterator {
        friend class List<T>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;

    private:
        ListData<T>* _ptr;

    public:
        ListIterator(ListData<T>* p) : _ptr(p) {}
        ListIterator(const ListIterator<T>& it) : _ptr(it._ptr) {}
        ~ListIterator() {}
        ListIterator<T>& operator=(const ListIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator*() { return _ptr->_data; }
        pointer operator->() { return &_ptr->_data; }
        reference operator[](std::ptrdiff_t diff) {
            auto tmp = _ptr;
            sforin(i, 0, diff) {
                if (!tmp->next) throw RangeException("");
                tmp = tmp->next;
            }
            return tmp->_data;
        }
        ListIterator<T>& operator++() { _ptr = _ptr->next; return *this; }
        ListIterator<T> operator++(int) { return ListIterator<T>(_ptr->next); }
        ListIterator<T>& operator--() { _ptr = _ptr->prev; return *this; }
        ListIterator<T> operator--(int) { return ListIterator<T>(_ptr->prev); }
        ListIterator<T>& operator+=(std::ptrdiff_t diff) {
            sforin(i, 0, diff) { 
                if (!_ptr) throw RangeException(outRangeErrorText("ptrdiff", diff, 0, i - 1));
                _ptr = _ptr->next;
            }
            return *this;
        }
        ListIterator<T>& operator-=(std::ptrdiff_t diff) {
            sforin(i, 0, diff) {
                if (!_ptr) throw RangeException(outRangeErrorText("ptrdiff", diff, 0, i - 1));
                _ptr = _ptr->prev;
            }
            return *this;
        }
        ListIterator<T> operator +(std::ptrdiff_t diff) { return ListIterator<T>(*this) += diff; }
        ListIterator<T> operator -(std::ptrdiff_t diff) { return ListIterator<T>(*this) -= diff; }
        std::ptrdiff_t operator-(const ListIterator<T> it) const {
            if (_ptr == it._ptr) return 0;
            std::ptrdiff_t dif[2] = { 0, 0 };
            ListData<T>* p[2] = { it._ptr, it._ptr };
            while (true) {
                if (p[0]) { p[0] = p[0]->_prev; --dif[0]; }
                if (p[1]) { p[1] = p[1]->_next; ++dif[1]; }
                if (p[0] == _ptr) return dif[0];
                else if (p[1] == _ptr) return dif[1];
                else if (!p[0] && !p[1]) break;
            }
            throw NotFoundException(nofoundErrorText(S(*it), "List"));
        }
        ListData<T>* ptr() { return _ptr; }
        void swap(ListIterator<T> it1, ListIterator<T> it2) {
            Memory<ListData<T>*>::swap(&it1._ptr, it2._ptr);
        }
        bool operator<(const ListIterator<T>& it) const {
            auto p = _ptr;
            while (p) {
                p = p->_next;
                if (p == it._ptr) return true;
            }
            return false;
        }
        bool operator<=(const ListIterator<T>& it) const { return _ptr == it._ptr || (*this) < it; }
        bool operator>(const ListIterator<T>& it) const { return it < (*this); }
        bool operator>=(const ListIterator<T>& it) const { return _ptr == it._ptr || it < (*this); }
        bool operator==(const ListIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const ListIterator<T>& it) const { return _ptr != it._ptr; }
    };
    template<typename T>
    class ListCIterator {
        friend class List<T>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;

    private:
        const ListData<T>* _ptr;

    public:
        ListCIterator(const ListData<T>* p) : _ptr(p) {}
        ListCIterator(const ListCIterator& it) : _ptr(it._ptr) {}
        ~ListCIterator() {}
        ListCIterator<T>& operator=(const ListCIterator<T>& it) { _ptr = it._ptr; return *this; }
        reference operator*() { return _ptr->_data; }
        pointer operator->() { return &_ptr->_data; }
        reference operator[](std::ptrdiff_t diff) {
            auto tmp = _ptr;
            sforin(i, 0, diff) {
                if (!tmp->next) throw RangeException("");
                tmp = tmp->next;
            }
            return tmp->_data;
        }
        ListCIterator<T>& operator++() { _ptr = _ptr->next; return *this; }
        ListCIterator<T> operator++(int) { return ListCIterator<T>(_ptr->next); }
        ListCIterator<T>& operator--() { _ptr = _ptr->prev; return *this; }
        ListCIterator<T> operator--(int) { return ListCIterator<T>(_ptr->prev); }
        ListCIterator<T>& operator+=(std::ptrdiff_t diff) {
            sforin(i, 0, diff) {
                if (!_ptr) throw RangeException("");
                _ptr = _ptr->next;
            }
            return *this;
        }
        ListCIterator<T>& operator-=(std::ptrdiff_t diff) {
            sforin(i, 0, diff) {
                if (!_ptr) throw RangeException("");
                _ptr = _ptr->prev;
            }
            return *this;
        }
        ListCIterator<T> operator +(std::ptrdiff_t diff) { return ListCIterator<T>(*this) += diff; }
        ListCIterator<T> operator -(std::ptrdiff_t diff) { return ListCIterator<T>(*this) -= diff; }
        sinteger operator-(const ListCIterator<T> it) const {
            if (_ptr == it._ptr) return 0;
            std::ptrdiff_t dif = 0;
            auto p = it._ptr;
            while (p && p != _ptr) { p = p->_next; ++dif; }
            if (p == _ptr) return dif;
            dif = 0; p = it._ptr;
            while (p && p != _ptr) { p = p->_prev; --dif; }
            if (p == _ptr) return dif;
            throw NotFoundException("");
        }
        const ListData<T>* ptr() { return _ptr; }
        void swap(ListCIterator<T> it1, ListCIterator<T> it2) {
            Memory<const ListData<T>*>::swap(&it1._ptr, it2._ptr);
        }
        bool operator<(const ListCIterator<T>& it) const {
            auto p = _ptr;
            while (p) {
                p = p->_next;
                if (p == it._ptr) return true;
            }
            return false;
        }
        bool operator<=(const ListCIterator<T>& it) const { return _ptr == it._ptr || (*this) < it; }
        bool operator>(const ListCIterator<T>& it) const { return it < (*this); }
        bool operator>=(const ListCIterator<T>& it) const { return _ptr == it._ptr || it < (*this); }
        bool operator==(const ListCIterator<T>& it) const { return _ptr == it._ptr; }
        bool operator!=(const ListCIterator<T>& it) const { return _ptr != it._ptr; }
    };
}
#endif
