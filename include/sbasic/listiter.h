#ifndef SLIB_LIITER_H
#define SLIB_LIITER_H

#include "sconfig.h"

#define sli_iter slib::SListIterator
#define sli_citer slib::SListCIterator

namespace slib {
	template<typename T>
	class List;
    template<typename T>
    struct list_data {
		subyte data[sizeof(T)];
        list_data *prev, *next;
        
        list_data();
		list_data(T &&t);
        list_data(const T &t);
        list_data(const list_data &li);
        ~list_data();
		list_data& operator=(const list_data& li);
		T& value();
		const T& value() const;
		void init();
		void init(T&& t);
		void init(const T& t);
		void release();
		void insertNext(list_data* nxt);
		void insertPrev(list_data* prv);
    };
    template<typename T>
    class SListIterator {
		friend class List<T>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;
        
    private:
        list_data<T> *_ptr;
        
    public:
        SListIterator(list_data<T> *p);
        SListIterator(const SListIterator &it);
		~SListIterator();
		SListIterator& operator=(const SListIterator &it);
        reference operator*();
        pointer operator->();
        reference operator[](std::ptrdiff_t diff);
        SListIterator &operator++();
        SListIterator operator++(int);
        SListIterator &operator--();
        SListIterator operator--(int);
        SListIterator &operator+=(std::ptrdiff_t diff);
        SListIterator &operator-=(std::ptrdiff_t diff);
        SListIterator operator +(std::ptrdiff_t diff);
        SListIterator operator -(std::ptrdiff_t diff);
		sinteger operator-(const SListIterator it) const;
		list_data<T>* ptr();
        void swap(SListIterator it1, SListIterator it2);
        bool operator<(const SListIterator &it) const;
		bool operator<=(const SListIterator &it) const;
		bool operator>(const SListIterator& it) const;
		bool operator>=(const SListIterator& it) const;
        bool operator==(const SListIterator &it) const;
        bool operator!=(const SListIterator &it) const;
    };
    template<typename T>
    class SListCIterator {
		friend class List<T>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const T value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef const T& reference;
        
    private:
        list_data<T> *_ptr;
        
    public:
        SListCIterator(list_data<T> *p);
        SListCIterator(const SListCIterator &it);
		~SListCIterator();
		SListCIterator& operator=(const SListCIterator &it);
        reference operator*();
        pointer operator->();
        reference operator[](std::ptrdiff_t diff);
        SListCIterator &operator++();
        SListCIterator operator++(int);
        SListCIterator &operator--();
        SListCIterator operator--(int);
        SListCIterator &operator+=(std::ptrdiff_t diff);
        SListCIterator &operator-=(std::ptrdiff_t diff);
        SListCIterator operator +(std::ptrdiff_t diff);
        SListCIterator operator -(std::ptrdiff_t diff);
		sinteger operator-(const SListCIterator it) const;
		list_data<T>* ptr();
        void swap(SListCIterator it1, SListCIterator it2);
        bool operator<(const SListCIterator &it) const;
		bool operator<=(const SListCIterator& it) const;
		bool operator>(const SListCIterator& it) const;
		bool operator>=(const SListCIterator& it) const;
        bool operator==(const SListCIterator &it) const;
        bool operator!=(const SListCIterator &it) const;
    };
    /*============================================================*/
    template<typename T>
    list_data<T>::list_data() : prev(nullptr), next(nullptr) {}
	template<typename T>
	list_data<T>::list_data(T&& t) : list_data() { init(std::forward(t)); }
    template<typename T>
	list_data<T>::list_data(const T& t) : list_data() { init(t); }
    template<typename T>
    list_data<T>::list_data(const list_data<T> &li) : list_data() {
		prev = li.prev; next = li.next;
		*reinterpret_cast<T*>(&data[0]) = *reinterpret_cast<T*>(&li.data[0]);
	}
    template<typename T>
	list_data<T>::~list_data() { reinterpret_cast<T*>(&data[0])->~T(); prev = nullptr; next = nullptr; }
	template<typename T>
	list_data<T>& list_data<T>::operator=(const list_data<T>& li) {
		prev = li.prev; next = li.next;
		*reinterpret_cast<T*>(&data[0]) = *reinterpret_cast<T*>(&li.data[0]);
		return *this;
	}
	template<typename T>
	T& list_data<T>::value() { return *(reinterpret_cast<T*>(&data[0])); }
	template<typename T>
	const T& list_data<T>::value() const { return *(reinterpret_cast<T*>(&data[0])); }
	template<typename T>
	void list_data<T>::init() { new(reinterpret_cast<T*>(&data[0])) T(); }
	template<typename T>
	void list_data<T>::init(T&& t) { new(reinterpret_cast<T*>(&data[0])) T(std::forward<T&&>(t)); }
	template<typename T>
	void list_data<T>::init(const T& t) { new(reinterpret_cast<T*>(&data[0])) T(t); }
	template<typename T>
	void list_data<T>::release() { (reinterpret_cast<T*>(&data[0]))->~T(); }
	template<typename T>
	void list_data<T>::insertNext(list_data* nxt) {
		nxt->next = next; 
		if (next) next->prev = nxt;
		next = nxt; nxt->prev = this; 
	}
	template<typename T>
	void list_data<T>::insertPrev(list_data* prv) {
		if (prev) prev->next = prv; 
		prv->prev = prev;
		prev = prv;
		prv->next = this;
	}

    /*============================================================*/
    template<typename T>
    SListIterator<T>::SListIterator(list_data<T> *p) : _ptr(p) {}
    template<typename T>
    SListIterator<T>::SListIterator(const SListIterator<T> &it) : _ptr(it._ptr) {}
    template<typename T>
    SListIterator<T> &SListIterator<T>::operator=(const SListIterator<T> &it) { _ptr = it._ptr; return *this; }
    template<typename T>
    SListIterator<T>::~SListIterator() {}
    template<typename T>
	T &SListIterator<T>::operator*() { return _ptr->value(); }
    template<typename T>
	T *SListIterator<T>::operator->() { return reinterpret_cast<T*>(&(_ptr->data[0])); }
    template<typename T>
	T &SListIterator<T>::operator[](std::ptrdiff_t diff) {
        auto tmp = _ptr;
        sforin(i, 0, diff) {
            if (!tmp->next) break;
            tmp = tmp->next;
        }
        return tmp->value();
    }
    template<typename T>
	SListIterator<T> &SListIterator<T>::operator ++() { _ptr = _ptr->next; return *this; }
    template<typename T>
	SListIterator<T> SListIterator<T>::operator ++(int) { return SListIterator<T>(_ptr->next); }
    template<typename T>
	SListIterator<T> &SListIterator<T>::operator --() { _ptr = _ptr->prev; return *this; }
    template<typename T>
	SListIterator<T> SListIterator<T>::operator --(int) { return SListIterator<T>(_ptr->prev); }
    template<typename T>
	SListIterator<T> &SListIterator<T>::operator +=(std::ptrdiff_t diff) {
        if(_ptr) {
            sforin(i, 0, diff) { if(_ptr) _ptr = _ptr->next; else break; }
        }
        return *this;
    }
    template<typename T>
	SListIterator<T> &SListIterator<T>::operator -=(std::ptrdiff_t diff) {
        if(_ptr->prev) {
            sforin(i, 0, diff) { if(_ptr->prev) _ptr = _ptr->prev; else break; }
        }
        return *this;
    }
    template<typename T>
	SListIterator<T> SListIterator<T>::operator +(std::ptrdiff_t diff) { return SListIterator<T>(*this)+=diff; }
    template<typename T>
	SListIterator<T> SListIterator<T>::operator -(std::ptrdiff_t diff) { return SListIterator<T>(*this)-=diff; }
    template<typename T>
	sinteger SListIterator<T>::operator -(const SListIterator<T> it) const {
		sinteger count = 0;
        auto tmp = _ptr;
        while (tmp != it._ptr) {
            ++count; tmp = tmp->prev;
        }
        return count;
    }
	template<typename T>
	list_data<T>* SListIterator<T>::ptr() { return _ptr; }
    template<typename T>
	void SListIterator<T>::swap(SListIterator<T> it1, SListIterator<T> it2) {
		auto temp = *it1; *it1 = *it2; *it2 = temp;
    }
    template<typename T>
	bool SListIterator<T>::operator<(const SListIterator<T> &it) const { return _ptr < it._ptr; }
	template<typename T>
	bool SListIterator<T>::operator<=(const SListIterator<T>& it) const { return _ptr <= it._ptr; }
	template<typename T>
	bool SListIterator<T>::operator>(const SListIterator<T>& it) const { return it._ptr < _ptr; }
	template<typename T>
	bool SListIterator<T>::operator>=(const SListIterator<T>& it) const { return it._ptr <= _ptr; }
    template<typename T>
	bool SListIterator<T>::operator==(const SListIterator<T> &it) const { return _ptr == it._ptr; }
    template<typename T>
	bool SListIterator<T>::operator!=(const SListIterator<T> &it) const { return _ptr != it._ptr; }
    
    /*============================================================*/
    
    template<typename T>
    SListCIterator<T>::SListCIterator(list_data<T> *p) : _ptr(p) {}
    template<typename T>
    SListCIterator<T>::SListCIterator(const SListCIterator<T> &it) : _ptr(it._ptr) {}
    template<typename T>
    SListCIterator<T>::~SListCIterator() {}
    template<typename T>
    SListCIterator<T> &SListCIterator<T>::operator=(const SListCIterator<T> &it) { _ptr = it._ptr; return *this; }
    template<typename T>
	 const T &SListCIterator<T>::operator *() { return _ptr->value(); }
    template<typename T>
	const T *SListCIterator<T>::operator ->() { return reinterpret_cast<const T*>(&(_ptr->data[0])); }
    template<typename T>
	const T &SListCIterator<T>::operator [](std::ptrdiff_t diff) {
        auto tmp = _ptr;
        sforin(i, 0, diff) {
            if (!tmp->next) break;
            tmp = tmp->next;
        }
        return tmp->value();
    }
    template<typename T>
	SListCIterator<T> &SListCIterator<T>::operator ++() { _ptr = _ptr->next; return *this; }
    template<typename T>
	SListCIterator<T> SListCIterator<T>::operator ++(int) { return SListCIterator<T>(_ptr->next); }
    template<typename T>
	SListCIterator<T> &SListCIterator<T>::operator --() { _ptr = _ptr->prev; return *this; }
    template<typename T>
	SListCIterator<T> SListCIterator<T>::operator --(int) { return SListCIterator<T>(_ptr->prev); }
    template<typename T>
	SListCIterator<T> &SListCIterator<T>::operator +=(std::ptrdiff_t diff) {
        if(_ptr) {
            sforin(i, 0, diff) { if(_ptr->next) _ptr = _ptr->next; else break; }
        }
        return *this;
    }
    template<typename T>
	SListCIterator<T> &SListCIterator<T>::operator -=(std::ptrdiff_t diff) {
        if(_ptr->prev) {
            sforin(i, 0, diff) { if(_ptr->prev) _ptr = _ptr->prev; else break; }
        }
        return *this;
    }
    template<typename T>
	SListCIterator<T> SListCIterator<T>::operator +(std::ptrdiff_t diff) { return SListCIterator<T>(*this)+=diff; }
    template<typename T>
	SListCIterator<T> SListCIterator<T>::operator -(std::ptrdiff_t diff) { return SListCIterator<T>(*this)-=diff; }
    template<typename T>
	sinteger SListCIterator<T>::operator -(const SListCIterator<T> it) const {
		sinteger count = 0;
        auto tmp = _ptr;
        while (tmp != it._ptr) {
            ++count; tmp = tmp->prev;
        }
        return count;
    }
	template<typename T>
	list_data<T>* SListCIterator<T>::ptr() { return _ptr; }
	template<typename T>
	void SListCIterator<T>::swap(SListCIterator<T> it1, SListCIterator<T> it2) {
		auto temp = *it1; *it1 = *it2; *it2 = temp;
    }
    template<typename T>
	bool SListCIterator<T>::operator<(const SListCIterator<T> &it) const { return _ptr < it._ptr; }
	template<typename T>
	bool SListCIterator<T>::operator<=(const SListCIterator<T>& it) const { return _ptr <= it._ptr; }
	template<typename T>
	bool SListCIterator<T>::operator>(const SListCIterator<T>& it) const { return it._ptr < _ptr; }
	template<typename T>
	bool SListCIterator<T>::operator>=(const SListCIterator<T>& it) const { return it._ptr <= _ptr; }
    template<typename T>
	bool SListCIterator<T>::operator==(const SListCIterator<T> &it) const { return _ptr == it._ptr; }
    template<typename T>
	bool SListCIterator<T>::operator!=(const SListCIterator<T> &it) const { return _ptr != it._ptr; }
}
#endif
