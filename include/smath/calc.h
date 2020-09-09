#ifndef SMATH_CALC_H
#define SMATH_CALC_H

#include "sbasic/range.h"
#include "smath/mathbasic.h"

namespace slib {
    namespace smath {
                extern SLIB_DLL bool isInteger(const float& f);
                extern SLIB_DLL bool isInteger(const double& d);
                extern SLIB_DLL double rootN(double val, int n);
                extern SLIB_DLL size_t digit(int i);
                extern SLIB_DLL size_t decim(double d);
                extern SLIB_DLL int gcd(int n1, int n2);
                extern SLIB_DLL int lcm(int n1, int n2);
                extern SLIB_DLL void reduction(int& n1, int& n2);
                extern SLIB_DLL int factorial(int val, int n);
                extern SLIB_DLL int factorial(int val);
                extern SLIB_DLL int combination(int n, int k);
                extern SLIB_DLL double deg2rad(double deg);
                extern SLIB_DLL double rad2deg(double rad);
                template<typename T1, typename T2>
                extern inline sint quot(T1 t1, T2 t2) {
                        if (t2 == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "Second argment of quot function");
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
                extern inline T power(T val, int n) {
                        if (!val) return val;
                        T pow = 1;
                        if (n < 0) {
                                sforin(i, 0, n) pow *= val;
                                pow = (T)1 / pow;
                        }
                        else sforin(i, 0, n) pow *= val;
                        return pow;
                }
                template<typename T>
                extern inline int kdelta(const T& v1, const T& v2) { return v1 == v2 ? 1 : 0; }
                class SLIB_DLL SRandom {
            std::random_device _seed;
            std::mt19937 _engine;

        public:
                        SRandom();
                        ~SRandom();

                        sint iruni(int beg, int end);
                        double runi(double beg = 0.0, double end = 1.0);
                        double rnorm(double m = 0.0, double s = 1.0);
                        double rlognorm(double m = 0.0, double s = 1.0);
                        double rchisq(double n);
                        int rpois(double m);
                        double rexp(double l);
                        double rgamma(double a, double b);
                        double rweib(double a, double b);
                        double rbern(double p);
                        double rbinom(int n, double p);
                        double rgeom(double p);
                        double rnbinom(int n, double p);
                        double rrayleigh(double s);
                        double rvonmises(double m, double k);
        };
    }
}
#endif