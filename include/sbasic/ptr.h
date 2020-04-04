#ifndef SLIB_PTR_H
#define SLIB_PTR_H

#include "sbasic/exception.h"

#ifdef _MANAGED
using namespace System::Threading;
#endif

namespace slib {
    
    #define sptr Pointer
    
    //Pointer<T>
    template<typename T>
    class Pointer {
    protected:
#ifndef _MANAGED
		std::atomic<int>* _scope;
#else
		int* _scope;
#endif
        T *_ptr;
        
    public:
        Pointer();
        Pointer(T *obj);
        Pointer(const T &obj);
        template<class C>
        Pointer(C *obj) : _ptr(dynamic_cast<T *>(obj)) {
#ifndef _MANAGED
			_scope = new std::atomic<int>(1);
#else 
			_scope = new int(1);
#endif
		}
        template<class... Args>
        Pointer(Args... args) : _ptr(new T(args...)) {
#ifndef _MANAGED
			_scope = new std::atomic<int>(1);
#else 
			_scope = new int(1);
#endif
		}
        Pointer(Pointer &&ptr);
        Pointer(const Pointer &ptr);
        ~Pointer();
        
        Pointer &operator=(T *obj);
        Pointer &operator=(const T &obj);
        template<class C>
        Pointer &operator=(C *obj) {
            release();
#ifndef _MANAGED
			_scope = new std::atomic<int>(1);
#else 
			_scope = new int(1);
#endif
            _ptr = dynamic_cast<T *>(obj);
        }
        Pointer &operator=(const Pointer &ptr);
        Pointer &operator=(Pointer &&ptr);
        
        T *ptr() const;
        T *operator->() const;
        T &operator*();
        const T &operator*() const;
        template<class C>
        C &toPtr() { return dynamic_cast<C *>(_ptr); }
        template<class C>
        C &toClass() { return *dynamic_cast<C *>(_ptr); }
        
        void share();
        void release();
        void copyTo(Pointer &ptr) const;
        void moveTo(Pointer &ptr);
        void discard();
        void swap(Pointer &ptr);
        bool isNull() const;
        
        operator bool() const;
        operator T *() const;

		bool operator<(const Pointer& p) const;
		bool operator==(const Pointer& p) const;
    };
    
    /*============================================================*/
    template<typename T>
    Pointer<T>::Pointer() : _ptr(nullptr) {
#ifndef _MANAGED
		_scope = new std::atomic<int>(1);
#else 
		_scope = new int(1);
#endif
	}
    template<typename T>
    Pointer<T>::Pointer(T *obj) : _ptr(obj) {
#ifndef _MANAGED
		_scope = new std::atomic<int>(1);
#else 
		_scope = new int(1);
#endif
	}
    template<typename T>
    Pointer<T>::Pointer(const T &obj) : _ptr(new T(obj)) {
#ifndef _MANAGED
		_scope = new std::atomic<int>(1);
#else 
		_scope = new int(1);
#endif
	}
    template<typename T>
    Pointer<T>::Pointer(Pointer &&ptr) : _scope(ptr._scope), _ptr(ptr._ptr) { ptr.discard(); }
    template<typename T>
    Pointer<T>::Pointer(const Pointer &ptr) : _scope(ptr._scope), _ptr(ptr._ptr) { share(); }
    template<typename T>
    Pointer<T>::~Pointer() { release(); }
    template<typename T>
    Pointer<T> &Pointer<T>::operator=(T *obj) {
        release(); 
#ifndef _MANAGED
		_scope = new std::atomic<int>(1);
#else 
		_scope = new int(1);
#endif
		_ptr = obj; return *this;
    }
    template<typename T>
    Pointer<T> &Pointer<T>::operator=(const T &obj) {
        release();
#ifndef _MANAGED
		_scope = new std::atomic<int>(1);
#else 
		_scope = new int(1);
#endif
		_ptr = new T(obj); return *this;
    }
    template<typename T>
    Pointer<T> &Pointer<T>::operator=(const Pointer &ptr) {
        release(); _scope = ptr._scope; _ptr = ptr._ptr; share(); return *this;
    }
    template<typename T>
    Pointer<T> &Pointer<T>::operator=(Pointer &&ptr) {
        release(); _scope = ptr._scope; _ptr = ptr._ptr; ptr.discard(); return *this;
    }
    template<typename T>
    T *Pointer<T>::ptr() const { return _ptr; }
    template<typename T>
    T *Pointer<T>::operator->() const { return _ptr; }
    template<typename T>
    T &Pointer<T>::operator*() {
        if (_ptr) return *_ptr;
        throw slib::SException(ERR_INFO, SLIB_NULL_ERROR, "_ptr");
    }
    template<typename T>
    const T &Pointer<T>::operator*() const {
        if (_ptr) return *_ptr;
        throw slib::SException(ERR_INFO, SLIB_NULL_ERROR, "_ptr");
    }
    template<typename T>
    void Pointer<T>::share() { 
#ifdef _MANAGED
		if (_scope) Interlocked::Increment(*_scope);
#else
		if (_scope) ++(*_scope);
#endif
	}
    template<typename T>
    void Pointer<T>::release() {
#ifdef _MANAGED
		if (_scope && 0 < *_scope) {
			Interlocked::Decrement(*_scope);
			if (*_scope < 1) {
				delete _scope;
				if (_ptr) delete _ptr;
			}
			discard();
		}
#else
		if (_scope && 0 < _scope->load()) {
			--(*_scope);
			if (_scope->load() < 1) {
				delete _scope;
				if (_ptr) delete _ptr;
			}
			discard();
        }
#endif
    }
    template<typename T>
    void Pointer<T>::copyTo(Pointer<T> &ptr) const {
        ptr.release(); ptr._scope = _scope; ptr._ptr = _ptr; ptr.share();
    }
    template<typename T>
    void Pointer<T>::moveTo(Pointer<T> &ptr) {
        ptr.release(); ptr._scope = _scope; ptr._ptr = _ptr; discard();
    }
    template<typename T>
    void Pointer<T>::discard() { _scope = nullptr; _ptr = nullptr; }
    template<typename T>
    void Pointer<T>::swap(Pointer<T> &ptr) {
        auto s = _scope; _scope = ptr._scope; ptr._scope = s;
        auto p = _ptr; _ptr = ptr._ptr; ptr._ptr = p;
    }
    template<typename T>
    bool Pointer<T>::isNull() const { return !_ptr; }
    template<typename T>
    Pointer<T>::operator bool() const { return _ptr; }
    template<typename T>
    Pointer<T>::operator T *() const { return _ptr; }
	template<typename T>
	bool Pointer<T>::operator<(const Pointer& p) const { return (*_ptr) < (*p._ptr); }
	template<typename T>
	bool Pointer<T>::operator==(const Pointer& p) const { return (*_ptr) == (*p._ptr); }
}
#endif
