#ifndef SLIB_SFUNC_H
#define SLIB_SFUNC_H
#include "sbasic/tuple.h"
#include "sobj/sobjptr.h"
namespace slib {
	template<class Ret, class... Args>
	class SFunction : public SObject {
	protected:
		std::function<Ret(Args...)> _func;
	public:
		SFunction() : SObject() {}
		SFunction(const std::function<Ret(Args...)>& func) : _func(func) {}
		SFunction(const SFunction<Ret, Args...>& func) { _func = func._func; }
		~SFunction() {}
		SFunction<Ret, Args...>& operator=(const std::function<Ret(Args...)>& func) {
			_func = func; return *this;
		}
		SFunction<Ret, Args...>& operator=(const SFunction<Ret, Args...>& func) {
			_func = func._func; return *this;
		}
		Ret exec(Args... args) { return _func(args...); }
		Ret operator()(Args... args) { return _func(args...); }
		String getClass() const { return "function"; }
		String toString(const char *format = nullptr) const {
			String args;
			arg_name<sizeof...(Args), Args...> names;
			names.addName(args);
			return String(typeid(Ret).name()) << "(" << args << ")";
		}
		SObject* clone() const { return new SFunction<Ret, Args...>(*this); }
    };
}
#endif
