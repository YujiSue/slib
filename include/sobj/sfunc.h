#ifndef SLIB_SFUNC_H
#define SLIB_SFUNC_H

#include "sobj/sobject.h"
#include "sobj/sset.h"

namespace slib {
    template<class Return, class... Args>
    class SFunction<Return(Args...)> : public SObject {
	public:
		typedef Return return_type;
		typedef SSet<Args...> args_type;

	protected:
		std::function<Return(Args...)> _func;

	public:
		SFunction() : SObject() {}
		SFunction(const std::function<Return(Args...)>& func) : _func(func) {}
		template<class Func>
		SFunction(const Func &f) : _func(f) {}
		SFunction(const SFunction<Return(Args...)>& func) { _func = func._func; }
		~SFunction() {}

		SFunction<Return(Args...)>& operator=(const SFunction<Return(Args...)>& func) {
			_func = func._func;
			return *this;
		}
        Return exec(Args... args) {
			return _func(args...);
		}
		Return operator()(Args... args) {
			 return _func(args...);
		}
        
		String getClass() const { return "function"; }
		String toString() const {
			String args;
			arg_name<sizeof...(Args), Args...> names;
			names.addName(args);
			return String(typeid(Return).name()) << "(" << args << ")";
		}
		SObject* clone() const { return new SFunction<Return(Args...)>(*this); }
        
    };
    
    /*============================================================*/
}


#endif
