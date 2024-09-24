#ifndef SLIB_PTR_H
#define SLIB_PTR_H
#include "sbasic/exception.h"
namespace slib {
	/**
	* @class FreePointer
	* @brief Pointer carrier class without ownership 
	*/
	template<typename T>
	class FreePointer {
		T* _ptr;
	public:
		FreePointer() { _ptr = nullptr; }
		FreePointer(T* p) { _ptr = p; }
		FreePointer(const FreePointer<T>& fp) { _ptr = fp._ptr; }
		~FreePointer() { _ptr = nullptr; }
		FreePointer<T>& operator=(const FreePointer<T>& fp) { _ptr = fp._ptr; return *this; }
		T* operator->() { return _ptr; }
		T& operator*() { return *_ptr; }
		operator T* () { return _ptr; }
		bool operator<(const FreePointer<T>& fp) const { return _ptr < fp._ptr; }
		bool operator==(const FreePointer<T>& fp) const { return _ptr == fp._ptr; }
	};
	/**
	* @class Pointer
	* @brief Pointer carrier class with ownership
	*/
	template<typename T>
	class Pointer {
		T* _ptr;
	public:
		Pointer() { _ptr = nullptr; }
		Pointer(T* p) { _ptr = p; }
		Pointer(const T&val) { _ptr = new T(val); }
		template<class... Args>
		Pointer(Args... args) { _ptr = new T(args...); }
		Pointer(const Pointer<T>& p) {
			_ptr = p._ptr;
			const_cast<Pointer<T>*>(&p)->_ptr = nullptr;
		}
		~Pointer() {
			if (_ptr) delete _ptr;
			_ptr = nullptr;
		}
		Pointer<T>& operator=(const Pointer<T>& p) { _ptr = p._ptr; const_cast<Pointer<T>*>(&p)->_ptr = nullptr; return *this; }
		T* operator->() { return _ptr; }
		T& operator*() { return *_ptr; }
		operator bool() { return _ptr != nullptr; }
		operator T* () { return _ptr; }
		operator const T* () const { return _ptr; }
		operator T& () { return *_ptr; }
		operator const T& () const { return *_ptr; }
		bool operator<(const Pointer<T>& p) const { return _ptr < p._ptr; }
		bool operator==(const Pointer<T>& p) const { return _ptr == p._ptr; }
	};
	/**
	* @class SPointer
	* @brief Pointer carrier class with sharable ownership
	*/
	template<typename T>
	class SPointer {
	protected:
		std::atomic<int>* _scope;
		T* _ptr;
	public:
		SPointer() : _scope(nullptr), _ptr(nullptr) {}
		SPointer(T* obj) : _scope(new std::atomic<int>(1)), _ptr(obj) {}
		SPointer(const T& obj) : SPointer(new T(obj)) {}
		template<class C>
		SPointer(C* obj) : SPointer(dynamic_cast<T*>(obj)) {}
		template<class... Args>
		SPointer(Args... args) : SPointer(new T(args...)) {}
		SPointer(SPointer<T>&& ptr) noexcept : _scope(ptr._scope), _ptr(ptr._ptr) { ptr.release(); }
		SPointer(const SPointer<T>& ptr) : _scope(ptr._scope), _ptr(ptr._ptr) { share(); }
		~SPointer() { destroy(); release(); }
		SPointer<T>& operator=(T* obj) {
			destroy();
			_scope = new std::atomic<int>(1);
			_ptr = obj; 
			return *this;
		}
		SPointer<T>& operator=(const T& obj) {
			destroy();
			_scope = new std::atomic<int>(1);
			_ptr = new T(obj); 
			return *this;
		}
		template<class C>
		SPointer<T>& operator=(C* obj) {
			destroy();
			_scope = new std::atomic<int>(1);
			_ptr = dynamic_cast<T*>(obj);
			return *this;
		}
		SPointer<T>& operator=(const SPointer<T>& ptr) {
			destroy(); 
			_scope = ptr._scope; 
			_ptr = ptr._ptr; 
			share(); 
			return *this;
		}
		T* ptr() const { return _ptr; }
		T* operator->() const { return _ptr; }
		T& operator*() {
			if (_ptr) return *_ptr;
			throw NullException(nullErrorText("instance"));
		}
		const T& operator*() const {
			if (_ptr) return *_ptr;
			throw NullException(nullErrorText("instance"));
		}
		template<class C>
		C* toPtr() { return dynamic_cast<C*>(_ptr); }
		template<class C>
		C& toClass() { return *dynamic_cast<C*>(_ptr); }
		size_t scope() const { return (size_t)_scope->load(); }
		void share() { if (_scope) ++(*_scope); }
		void destroy() {
			if (_scope && 0 < _scope->load()) {
				--(*_scope);
				if (_scope->load() < 1) {
					delete _scope;
					if (_ptr) delete _ptr;
				}
			}
		}
		void release() { _scope = nullptr; _ptr = nullptr; }
		void copyTo(SPointer<T>& ptr) const {
			ptr.destroy(); ptr._scope = _scope; ptr._ptr = _ptr; ptr.share();
		}
		void moveTo(SPointer<T>& ptr) {
			ptr.destroy(); ptr._scope = _scope; ptr._ptr = _ptr; release();
		}
		void swap(SPointer<T>& ptr) {
			Memory<std::atomic<int>*>::swap(&_scope, &(ptr._scope));
			Memory<T*>::swap(&_ptr, &(ptr._ptr));
		}
		bool isNull() const { return _ptr == nullptr; }
		operator bool() const { return _ptr != nullptr; }
		operator T* () const { return _ptr; }
		operator T& () { return *_ptr; }
		operator const T& () const { return *_ptr; }
		bool operator<(const T& val) const { return (*_ptr) < val; }
		bool operator<(const SPointer<T>& p) const { return (*_ptr) < (*p._ptr); }
		bool operator==(const T& val) const { return (*_ptr) == val; }
		bool operator==(const SPointer<T>& p) const { return _ptr == p._ptr || (*_ptr) == (*p._ptr); }
	};
}
/**
* @cond
*/
template<typename T>
using fptr = slib::FreePointer<T>;
template<typename T>
using sptr = slib::SPointer<T>;
/**
* @endcond
*/
#endif
