#ifndef SMATH_EXCEPTION_H
#define SMATH_EXCEPTION_H
#include "sbasic/exception.h"
#include "sbasic/string.h"
#include "smath/mbasic.h"
namespace slib {
	namespace smath {
		constexpr int DIV_ZERO_ERR = 0x0011;
		constexpr int UNSIGNED_ERR = 0x0012;
		constexpr int SHAPE_MISMATCH_ERR = 0x0013;
	}
	
	class SLIB_DLL SMathException : public Exception {
	public:
		SMathException();
		SMathException(const int i, const char* m = nullptr, const char* d = nullptr);
		~SMathException();
	};
	
	extern SLIB_DLL String divZeroErrorText(const char* s);
#define DivZeroException(X) SMathException(smath::DIV_ZERO_ERR,"Divided by zero error.",X)
#define UnsignedException(X) SMathException(smath::UNSIGNED_ERR,"Unsigned number error.",X)
#define ShapeMismatchException(X) SMathException(smath::SHAPE_MISMATCH_ERR,"Shape (vector dimension or matrix size) mismatch error",X)

}

#endif