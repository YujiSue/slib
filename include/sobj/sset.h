#ifndef SLIB_SSET_H
#define SLIB_SSET_H

#include "sobj/sobject.h"
#include "sbasic/string.h"

namespace slib {
	template<size_t Size, class... Args>
	struct set_element;
	template<size_t Size>
	struct set_element<Size> { void init() {} };
	template<size_t Size, class First, class... Args>
	struct set_element<Size, First, Args...> {
		static constexpr size_t index = Size;
		using type = First;
		First val;
		set_element<Size + 1, Args...> element;
		void init(First f, Args... args) {
			val = f; element.init(args...);
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
	class SSet : public SObject {
	protected:
		set_element<0, Args...> _args;

	public:
		SSet();
		SSet(Args... args);
		SSet(const SSet<Args...> &strct);
		~SSet();

		template<size_t Size>
		typename set_type<Size, Args...>::type get() {
			return set_value<Size, typename set_type<Size, Args...>::type, set_element<0, Args...>>::get(_args);
		}
		template<size_t Size>
		void set(const typename set_type<Size, Args...>::type &val) {
			set_value<Size, typename set_type<Size, Args...>::type, set_element<0, Args...>>::set(_args, val);
		}

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};

	template<class... Args>
	SSet<Args...>::SSet() : SObject() {}
	template<class... Args>
	SSet<Args...>::SSet(Args... args) : SSet() { _args.init(args...); }
	template<class... Args>
	SSet<Args...>::SSet(const SSet<Args...>& set) {}
	template<class... Args>
	SSet<Args...>::~SSet() {}
	template<class... Args>
	String SSet<Args...>::getClass() const { return "struct"; }
	template<class... Args>
	String SSet<Args...>::toString() const {
		String args;
		arg_name<sizeof...(Args), Args...> names;
		names.addName(args);
		return "<"+args+">";
	}
	template<class... Args>
	SObject* SSet<Args...>::clone() const { return new SSet<Args...>(*this); }

}

#endif