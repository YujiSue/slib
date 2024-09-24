#ifndef SMATH_OPERATOR
#define SMATH_OPERATOR
#include "sconfig.h"
namespace slib {
	namespace soperator {
		constexpr suint LT = 0x0001;
		constexpr suint GT = 0x0002;
		constexpr suint ELT = 0x0011;
		constexpr suint EGT = 0x0012;
		constexpr suint EQ = 0x0010;
		constexpr suint NOT = 0x0020;
		constexpr suint NEQ = 0x0030;
		constexpr suint BETWEEN = 0x0003;
		constexpr suint CONTAIN = 0x0080;

		constexpr suint EXACT = 0x0100;
		constexpr suint PREFIX = 0x0200;
		constexpr suint POSTFIX = 0x0400;
		constexpr suint PARTIAL = 0x0800;
		constexpr suint LIKE = 0x1000;
		constexpr suint MATCH = 0x2000;
	}
}
#endif
