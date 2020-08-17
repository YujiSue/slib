#ifndef SLIB_SFUNC_H
#define SLIB_SFUNC_H

#include "sobj/sobject.h"
#include "sbasic/set.h"

namespace slib {
    template<class Return, class... Args>
    class SFunction<Return(Args...)> : public SObject {
	public:
		typedef Set<Args...> args_type;
	protected:
		std::function<Return(Args...)> _func;
	public:
		SFunction();
		SFunction(const std::function<Return(Args...)>& func);
		template<class Func>
		SFunction(const Func &f) : _func(f) {}
		SFunction(const SFunction<Return(Args...)>& func);
		~SFunction();

		SFunction<Return(Args...)>& operator=(const SFunction<Return(Args...)>& func);
		Return exec(Args... args);
		Return operator()(Args... args);
		String getClass() const;
		String toString() const;
		SObject* clone() const;
    };

	template<class Return, class... Args>
	SFunction<Return(Args...)>::SFunction() : SObject() {}
	template<class Return, class... Args>
	SFunction<Return(Args...)>::SFunction(const std::function<Return(Args...)>& func) : _func(func) {}
	template<class Return, class... Args>
	SFunction<Return(Args...)>::SFunction(const SFunction<Return(Args...)>& func) { _func = func._func; }
	template<class Return, class... Args>
	SFunction<Return(Args...)>::~SFunction() {}
	template<class Return, class... Args>
	SFunction<Return(Args...)>& SFunction<Return(Args...)>::operator=(const SFunction<Return(Args...)>& func) {
		_func = func._func;
		return *this;
	}
	template<class Return, class... Args>
	Return SFunction<Return(Args...)>::exec(Args... args) {
		return _func(args...);
	}
	template<class Return, class... Args>
	Return SFunction<Return(Args...)>::operator()(Args... args) {
		return _func(args...);
	}
	template<class Return, class... Args>
	String SFunction<Return(Args...)>::getClass() const { return "function"; }
	template<class Return, class... Args>
	String SFunction<Return(Args...)>::toString() const {
		String args;
		arg_name<sizeof...(Args), Args...> names;
		names.addName(args);
		return String(typeid(Return).name()) << "(" << args << ")";
	}
	template<class Return, class... Args>
	SObject* SFunction<Return(Args...)>::clone() const { return new SFunction<Return(Args...)>(*this); }
}
#endif
