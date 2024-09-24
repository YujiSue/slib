#ifndef SMATH_CALC_H
#define SMATH_CALC_H
#include "sbasic/range.h"
#include "smath/mexception.h"
namespace slib {
	namespace smath {
		template<typename T>
		extern bool isInteger(const T v) { return v == (T)floor((double)v) || v == (T)ceil((double)v); }
		template<typename T>
		extern double rootN(const T v, const size_t n) {
			if (n == 0) return 1.0;
			else if (n == 1) return v;
			double r = pow(v, 1.0 / n);
			return (r * (n - 1) + v / (pow(r, n - 1))) / n;
		}
		template<typename T>
		extern size_t digit(const T v) {
			std::string str = std::to_string(v);
			return str.length() - (v < 0 ? 1 : 0);
		}
		template<typename T>
		extern size_t decim(const T v) {
			std::string str = std::to_string(v);
			size_t pos = str.find(".");
			return pos == std::string::npos ? str.length() : pos;
		}
		template<typename T>
		extern T gcd(T n1, T n2) {
			auto surp = n1 % n2;
			while (surp) { n1 = n2; n2 = surp; surp = n1 % n2; }
			return n2;
		}
		template<typename T>
		extern T lcm(T n1, T n2) { return n1 * n2 / gcd(n1, n2); }
		template<typename T>
		extern void reduction(T& n1, T& n2) { auto g = gcd<T>(n1, n2); n1 /= g; n2 /= g; }
		template<typename T>
		extern T factorial(T n, T k) {
			T f = (T)1;
			sforin(i, 0, k) { f *= n; n--; }
			return f;
		}
		template<typename T>
		extern T factorial(T n) { return factorial(n, n); }
		template<typename T>
		extern T combination(T n, T k) {
			if (k <= n - k) {
				Pair<T, T> frac(1, 1);
				while (1 <= k) {
					frac.first *= n;
					frac.second *= k;
					reduction(frac.first, frac.second);
					--n; --k;
				}
				return frac.first;
			}
			else return combination(n, n - k);
		}
		template<typename T>
		extern T deg2rad(T deg) { return (T)(deg * snum::PI / 180.0); }
		template<typename T>
		extern T rad2deg(T rad) { return (T)(rad * 180.0 / snum::PI); }
		template<typename T1, typename T2>
		extern inline sint quot(T1 t1, T2 t2) {
			if (t2 == 0) throw DivZeroException(divZeroErrorText("divider"));
			if (t1 < 0) {
				if (t2 < 0) return (-t1) / (-t2);
				else return -((-t1) / t2);
			}
			else {
				if (t2 < 0) return -(t1 / (-t2));
				else return t1 / t2;
			}
		}
		template<typename T>
		extern T power(const T val, int n) {
			if (!val) return val;
			if (!n) return 1;
			T pow = val;
			if (n < 0) {
				sforin(i, 1, -n) pow *= val;
				pow = (T)1 / pow;
			}
			else sforin(i, 1, n) pow *= val;
			return pow;
		}
		template<typename T>
		extern int kdelta(const T& v1, const T& v2) { return v1 == v2 ? 1 : 0; }
	}
}
#endif