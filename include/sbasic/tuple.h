#ifndef SLIB_TUPLE_H
#define SLIB_TUPLE_H
#include "sconfig.h"
#include "sbasic/string.h"
namespace slib {
	///@cond
	template<size_t Size, class... Args>
	struct tuple_element;
	template<size_t Size>
	struct tuple_element<Size> {
		tuple_element() {}
		tuple_element(const tuple_element<Size>& elem) {}
		tuple_element<Size>& operator=(const tuple_element<Size>& elem) { return *this; }
		void init() {}
		void addValue(String& str) const { str.resize(str.size() - 1); }
	};
	template<size_t Size, class First, class... Args>
	struct tuple_element<Size, First, Args...> {
		static constexpr size_t index = Size;
		using type = First;
		First val;
		tuple_element<Size + 1, Args...> element;

		tuple_element() {}
		tuple_element(const tuple_element<Size, First, Args...>& elem) { val = elem.val; element = elem.element; }
		tuple_element<Size, First, Args...>& operator=(const tuple_element<Size, First, Args...>& elem) { val = elem.val; element = elem.element; return *this; }
		void init(First f, Args... args) { val = f; element.init(args...); }
		void addValue(String& str) const { str << val << ","; element.addValue(str); }
	};
	template<size_t Size, class Type, class Cls>
	struct tuple_value;
	template<class Type, class Cls>
	struct tuple_value<0, Type, Cls> {
		static Type& at(Cls& cls) { return cls.val; }
		static Type get(Cls& cls) { return cls.val; }
		static void set(Cls& cls, const Type& val) { cls.val = val; }
	};
	template<size_t Size, class Type, class Cls>
	struct tuple_value {
		static Type& at(Cls& cls) { return slib::tuple_value<Size - 1, Type, decltype(cls.element)>::at(cls.element); }
		static Type get(Cls& cls) { return slib::tuple_value<Size - 1, Type, decltype(cls.element)>::get(cls.element); }
		static void set(Cls& cls, const Type& val) { slib::tuple_value<Size - 1, Type, decltype(cls.element)>::set(cls.element, val); }
	};
	template<size_t Size, class... Args>
	struct tuple_type;
	template<class First, class... Args>
	struct tuple_type<0, First, Args...> { typedef First type; };
	template<size_t Size, class T, class... Elem>
	struct tuple_type<Size, T, Elem...> : public tuple_type<Size - 1, Elem...> {};
	template<size_t Size, class... Args>
	struct arg_name { void addName(String& str); };
	template<class... Args>
	struct arg_name<0, Args...> { 
		void addName(String& str) { str.resize(str.length() - 1); }
	};
	template<size_t Size, class First, class... Args>
	struct arg_name<Size, First, Args...> {
		arg_name<Size - 1, Args...> rest;
		void addName(String& str) { str << typeid(First).name() << ","; rest.addName(str); }
	};
	///@condend
	/**
	* @class Tuple
	* \~english @brief Tuple class
	* \~japanese @brief タプルクラス
	*/
	template<class... Args>
	class Tuple {
	private:
		tuple_element<0, Args...> _args;
	public:
		Tuple(const Args&... args) { _args.init(args...); }
		Tuple(const Tuple<Args...>& tpl) : _args(tpl._args) {}
		~Tuple() {}
		Tuple& operator=(const Tuple<Args...>& tpl) { _args = tpl._args; return *this; }
		template<size_t Size>
		typename tuple_type<Size, Args...>::type& at() {
			return tuple_value<Size, typename tuple_type<Size, Args...>::type, tuple_element<0, Args...>>::at(_args);
		}
		template<size_t Size>
		typename tuple_type<Size, Args...>::type& at() const {
			return tuple_value<Size, typename tuple_type<Size, Args...>::type, tuple_element<0, Args...>>::at(*const_cast<tuple_element<0, Args...> *>(&_args));
		}
		template<size_t Size>
		typename tuple_type<Size, Args...>::type get() {
			return tuple_value<Size, typename tuple_type<Size, Args...>::type, tuple_element<0, Args...>>::get(_args);
		}
		template<size_t Size>
		typename tuple_type<Size, Args...>::type get() const {
			return tuple_value<Size, typename tuple_type<Size, Args...>::type, tuple_element<0, Args...>>::get(*const_cast<tuple_element<0, Args...> *>(&_args));
		}
		template<size_t Size>
		void set(const typename tuple_type<Size, Args...>::type &val) {
			tuple_value<Size, typename tuple_type<Size, Args...>::type, tuple_element<0, Args...>>::set(_args, val);
		}
		const tuple_element<0, Args...>& elements() const { return _args; }
	};
	///@cond
	template<class... Args>
	extern slib::String argnames() { slib::String args; arg_name<sizeof...(Args), Args...> names; names.addName(args); return args; }
	template<class... Args>
	extern slib::String argvalues(const slib::Tuple<Args...> &tpl) { slib::String values; tpl.elements().addValue(values); return values; }
	///@condend
	template<class... Args>
	extern slib::String toString(const slib::Tuple<Args...>& tpl, const char* f = nullptr) { return String("<") << slib::argnames<Args...>() << ">(" << argvalues(tpl) << ")"; }
}
///@cond
template<class... Args>
extern inline std::ostream& operator<<(std::ostream& os, const slib::Tuple<Args...>& tpl) { return os << slib::toString(tpl); }
///@condend
#endif