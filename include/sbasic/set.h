#ifndef SLIB_SET_H
#define SLIB_SET_H

#include "sbasic/string.h"
#include "sconfig.h"

namespace slib {
	template<size_t Size, class... Args>
	struct set_element;
	template<size_t Size>
	struct set_element<Size> { 
		void init() {} 
		set_element<Size>& operator=(const set_element<Size>& elem) { return *this; }
	};
	template<size_t Size, class First, class... Args>
	struct set_element<Size, First, Args...> {
		static constexpr size_t index = Size;
		using type = First;
		First val;
		set_element<Size + 1, Args...> element;
		void init(First f, Args... args) {
			val = f; element.init(args...);
		}
		set_element<Size, First, Args...>& operator=(const set_element<Size, First, Args...>& elem) {
			val = elem.val; element = elem.element; return *this;
		}
	};
	template<size_t Size, class Type, class Cls>
	struct set_value;
	template<class Type, class Cls>
	struct set_value<0, Type, Cls> {
		static Type get(Cls& cls) { return cls.val; }
		static void set(Cls& cls, const Type &val) { cls.val = val; }
	};
	template<size_t Size, class Type, class Cls>
	struct set_value {
		static Type get(Cls& cls) { return set_value<Size - 1, Type, decltype(cls.element)>::get(cls.element); }
		static void set(Cls &cls, const Type &val) { set_value<Size - 1, Type, decltype(cls.element)>::set(cls.element, val); }
	};
	template<size_t Size, class... Args>
	struct set_type;
	template<class First, class... Args>
	struct set_type<0, First, Args...> { typedef First type; };
	template<size_t Size, class T, class... Elem>
	struct set_type<Size, T, Elem...> : public set_type<Size - 1, Elem...> {};

	template<size_t Size, class... Args>
	struct arg_name { void addName(String& str); };
	template<class... Args>
	struct arg_name<0, Args...> {
		void addName(String& str) { str.resize(str.length() - 1); }
	};
	template<size_t Size, class First, class... Args>
	struct arg_name<Size, First, Args...> {
		arg_name<Size - 1, Args...> rest;
		void addName(String& str) {
			str << typeid(First).name() << ","; rest.addName(str);
		}
	};
	template<class... Args>
	class Set {
	protected:
		set_element<0, Args...> _args;
	public:
		Set();
		Set(Args... args);
		Set(const Set<Args...> & set);
		~Set();
		Set& operator=(const Set<Args...>& set);
		template<size_t Size>
		typename set_type<Size, Args...>::type getValue() {
			return set_value<Size, typename set_type<Size, Args...>::type, set_element<0, Args...>>::get(_args);
		}
		template<size_t Size>
		typename set_type<Size, Args...>::type getValue() const {
			return set_value<Size, typename set_type<Size, Args...>::type, set_element<0, Args...>>::get(*const_cast<set_element<0, Args...> *>(&_args));
		}
		template<size_t Size>
		void setValue(const typename set_type<Size, Args...>::type &val) {
			set_value<Size, typename set_type<Size, Args...>::type, set_element<0, Args...>>::set(_args, val);
		}
	};

	template<class... Args>
	Set<Args...>::Set() {}
	template<class... Args>
	Set<Args...>::Set(Args... args) : Set<Args...>() { _args.init(args...); }
	template<class... Args>
	Set<Args...>::Set(const Set<Args...>& set) : Set<Args...>() { _args = set._args; }
	template<class... Args>
	Set<Args...>::~Set() {}
	template<class... Args>
	Set<Args...>& Set<Args...>::operator=(const Set<Args...>& set) { _args = set._args; return *this; }
}

#endif