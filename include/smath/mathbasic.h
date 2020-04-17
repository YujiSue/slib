#ifndef SMATH_BASIC_H
#define SMATH_BASIC_H

#include "sbasic/exception.h"

namespace slib {
	namespace smath {
		constexpr suint DIV_ZERO_ERR = 0x0011;
		constexpr suint UNSIGNED_MINUS_ERR = 0x0012;
		constexpr suint DIMENSION_SIZE_ERR = 0x0013;

#define DIMENSION_SIZE_DIFF(X, Y) String(X)+u8" ≠ "+String(Y)

		class SOBJ_DLL SMathException : public SException {
		public:
			SMathException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr) :
				SException(f, l, func, e, target, note) {
				prefix = "smath";
				switch (err) {
				case DIV_ZERO_ERR:
				{
					message = "Divided by zero.";
					description = TARGET_TEXT(target ? target : "") + u8" is zero.";
					break;
				}
				case UNSIGNED_MINUS_ERR:
				{
					message = "Unsigned value error.";
					description = TARGET_TEXT(target ? target : "") + u8" should be positive.";
					break;
				}
				case DIMENSION_SIZE_ERR:
				{
					message = "Dimension size error.";
					description = TARGET_TEXT(target ? target : "") + u8" " + (note ? note : "");
					break;
				}
				default:
					break;
				}
			}
			~SMathException() {}
		};

		typedef enum {
			HORIZONTAL = 1,
			VERTICAL = 2,
			DEPTH = 3,
		} DIRECTION;

		typedef enum {
			DEFAULT = 0,
			FLOOR = 1,
			CEIL = 2,
			TO_ZERO = 3,
			TO_INF = 4,
		} ROUND;

		typedef enum {
			X_AXIS = 1,
			Y_AXIS = 2,
			Z_AXIS = 4,
			W_AXIS = 8,
			T_AXIS = 8,
		} AXIS;

		typedef enum {
			XY_PLANE = 1,
			XZ_PLANE = 2,
			YZ_PLANE = 4,
		} PLANE;

		typedef enum {
			CW = 0,
			CCW = 1,
		} ROTATION;

		constexpr subyte MAX_UBYTE = 0xFF;
		constexpr sushort MAX_USHORT = 0xFFFF;
		constexpr suint MAX_UINT = 0xFFFFFFFF;
		constexpr suinteger MAX_ULONG = 0xFFFFFFFFFFFFFFFF;
		constexpr double PI = M_PI;
		constexpr float F_EPS = FLT_EPSILON;
		constexpr double D_EPS = DBL_EPSILON;
	}
}

#endif