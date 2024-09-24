#ifndef SMATH_BASIC_H
#define SMATH_BASIC_H
#ifdef USE_OMP
#include <omp.h>
#endif
#include "sconfig.h"
namespace slib {
	namespace stype {
		constexpr subyte BYTE = 0x01;
		constexpr subyte UBYTE = 0xF1;
		constexpr subyte SHORT = 0x02;
		constexpr subyte USHORT = 0xF2;
		constexpr subyte INT = 0x04;
		constexpr subyte UINT = 0xF4;
		constexpr subyte INTEGER = 0x08;
		constexpr subyte UINTEGER = 0xF8;
		constexpr subyte FLOAT = 0x14;
		constexpr subyte DOUBLE = 0x18;
		constexpr subyte REAL = 0x20;
		//constexpr subyte DECIMAL = 0x21;
		constexpr subyte FRACTION = 0x44;
		constexpr subyte FRACTION64 = 0x48;
		constexpr subyte COMPLEX = 0x84;
		constexpr subyte COMPLEX64 = 0x88;

		constexpr subyte BOOL = 0xFF;
	}
	/**
	* @namespace snum
	* \~english @brief Math constant
	*/
	namespace snum {
		constexpr float NaNf = std::numeric_limits<float>::quiet_NaN();
		constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
		constexpr double Inff = std::numeric_limits<float>::infinity();
		constexpr double Inf = std::numeric_limits<double>::infinity();
		constexpr double nInff = -std::numeric_limits<float>::infinity();
		constexpr double nInf = -std::numeric_limits<double>::infinity();
		constexpr double PI = M_PI;
		constexpr double E = M_E;
		constexpr float F_EPS = FLT_EPSILON;
		constexpr double D_EPS = DBL_EPSILON;

		constexpr subyte MAX8B = 0xFF;
		constexpr sushort MAX16B = 0xFFFF;
		constexpr suint MAX24B = 0x00FFFFFF;
		constexpr suint MAX32B = 0xFFFFFFFF;
		constexpr suinteger MAX64B = 0xFFFFFFFFFFFFFFFF;
	}
	namespace smath {
		enum class ROUND {
			DEFAULT = 0,
			FLOOR = 1,
			CEIL = 2,
			TO_ZERO = 3,
			TO_INF = 4,
			TO_NINF = 5,
		};
		enum class AXIS : subyte {
			X = 0x01,
			Y = 0x02,
			Z = 0x04,
			W = 0x08,
			T = 0x08,
		};
		enum class PLANE : subyte {
			XY = 1,
			XZ = 2,
			YZ = 4,
		};
		enum class ROTATION : subyte {
			CW = 0,
			CCW = 1,
		};

		enum class ORIGIN : subyte {
			ZERO = 0x00,
			TOP_LEFT = 0x01,
			BOTTOM_LEFT = 0x02,
			BOTTOM_RIGHT = 0x03,
			TOP_RIGHT = 0x04,
			CENTER = 0x10,
		};


		/**
		* @cond
		*/
		template<typename T>
		extern T zero() { return 0; }
		
		template<typename T>
		extern T unit() { return 1; }
		/**
		* @endcond
		*/
	}
}
extern SLIB_DLL size_t operator "" _u(unsigned long long int v);
#endif