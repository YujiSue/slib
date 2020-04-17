#ifndef SMATH_CALC_H
#define SMATH_CALC_H

#include "sbasic/range.h"
#include "smath/mathbasic.h"

namespace slib {
    namespace smath {
        
        extern inline bool isInteger(const float &f) { return f == floorf(f) || f == ceilf(f); }
        extern inline bool isInteger(const double &d) { return d == floor(d) || d == ceil(d); }
        template<typename T1, typename T2>
        extern inline sinteger quot(T1 t1, T2 t2) {
            if (t2 == 0) throw SMathException(ERR_INFO, DIV_ZERO_ERR, "t2");
            if (t1 < 0) {
                if (t2 < 0) return (-t1)/(-t2);
                else return -((-t1)/t2);
            }
            else {
                if (t2 < 0) return -(t1/(-t2));
                else return t1/t2;
            }
        }
        extern inline int power(int val, int n) {
			int init = 1;
			sforin(i, 0, n) init *= val;
			return init;
		}
        extern inline float power(float val, int n) {
			float init = 1.f;
			if (n < 0) {
				n *= -1;
				sforin(i, 0, n) init /= (float)val;
			}
			else sforin(i, 0, n) init *= (float)val;
			return init;
		}
        extern inline double power(double val, int n) {
			double init = 1.0;
			if (n < 0) {
				n *= -1;
				sforin(i, 0, n) init /= val;
			}
			else sforin(i, 0, n) init *= val;
			return init;
		}
        extern inline double rootN(double val, int n) {
			double r = pow(val, 1.0 / n);
			return (r * (n - 1) + val / (pow(r, n - 1))) / n;
		}
        extern inline size_t digit(int i) {
			std::string str = std::to_string(i);
			return str.length() - (i < 0 ? 1 : 0);
		}
        extern inline size_t decim(double d) {
			std::string str = std::to_string(d);
			size_t pos = str.find(".");
			return pos == std::string::npos ? str.length() : pos;
		}
        extern inline int gcd(int n1, int n2) {
			int ori = n1, div = n2, res = n1 % n2;
			while (res) { ori = div; div = res; res = ori % div; }
			return div;
		}
        extern inline int lcm(int n1, int n2) { return n1 * n2 / gcd(n1, n2); }
        extern inline void reduction(int &n1, int &n2) { int g = gcd(n1, n2); n1 /= g; n2 /= g; }
        extern inline int factorial(int val, int n) {
			int f = 1;
			sforin(i, 0, n) f *= (val - i);
			return f;
		}
        extern inline int factorial(int val) { return factorial(val, val); }
        extern inline int combination(int n, int k) { return factorial(n) / (factorial(k) * factorial(n - k)); }
        template<typename T>
		extern inline int kdelta(const T &v1, const T &v2) { return v1==v2?1:0; }
        
        extern inline double deg2rad(double deg) { return deg * smath::PI / 180.0; }
        extern inline double rad2deg(double rad) { return rad * 180.0 / smath::PI; }
        
        extern inline double sigmoid(double n) { return 0.5 * (tanh(0.5 * n) + 1.0); }
        extern inline double splus(double n) { return log(1.0 + exp(n)); }
        extern inline double relu(double n) { return 0.0 < n ? n : 0.0; }
        
        class SOBJ_DLL SRandom {
            std::random_device _seed;
            std::mt19937 _engine;

        public:
            SRandom() {
				_engine = std::mt19937(_seed());
			}
            ~SRandom() {}
            
			sint iruni(int beg, int end) {
				std::uniform_int_distribution<> dist(beg, end);
				return dist(_engine);
			}
            double runi(double beg = 0.0, double end = 1.0) {
				std::uniform_real_distribution<> dist(beg, end);
				return dist(_engine);
			}
            double rnorm(double m = 0.0, double s = 1.0) {
				std::normal_distribution<> dist(m, s);
				return dist(_engine);
			}
            double rlognorm(double m = 0.0, double s = 1.0) {
				std::lognormal_distribution<> dist(m, s);
				return dist(_engine);
			}
            double rchisq(double n) {
				std::chi_squared_distribution<> dist(n);
				return dist(_engine);
			}
            int rpois(double m) {
				std::poisson_distribution<> dist(m);
				return dist(_engine);
			}
            double rexp(double l) {
				std::exponential_distribution<> dist(l);
				return dist(_engine);
			}
            double rgamma(double a, double b) {
				std::gamma_distribution<> dist(a, b);
				return dist(_engine);
			}
            double rweib(double a, double b) {
				std::weibull_distribution<> dist(a, b);
				return dist(_engine);
			}
            double rbern(double p) {
				std::bernoulli_distribution dist(p);
				return dist(_engine);
			}
            double rbinom(int n, double p) {
				std::binomial_distribution<> dist(n, p);
				return dist(_engine);
			}
            double rgeom(double p) {
				std::geometric_distribution<> dist(p);
				return dist(_engine);
			}
            double rnbinom(int n, double p) {
				std::negative_binomial_distribution<> dist(n, p);
				return dist(_engine);
			}
            
            double rrayleigh(double s) {
				double r = runi();
				return s * sqrt(-2.0 * log(r));
			}
            double rvonmises(double m, double k) {
				double r = 1 + std::pow((1 + 4 * k * k), 0.5);
				double rho = 0.5 * (r - std::pow(2 * r, 0.5)) / k;
				double s = 0.5 * (1 + rho * rho) / rho;
				bool b = false;
				double phase;
				while (!b) {
					double c = std::cos(smath::PI * runi());
					phase = (1 + s * c) / (s + c);
					double tmp = k * (s - phase);
					double r1 = runi();
					b = tmp * (2 - tmp) - r1 > 0;
					if (!b) b = std::log(tmp / r1) + 1 - tmp >= 0;
				}
				double r2 = runi() - 0.5;
				double sig = ((r2 >= 0) - (r2 <= 0));
				return sig * std::acos(phase) + std::fmod(std::fmod(m, 2 * smath::PI) + 2 * smath::PI, 2 * smath::PI);
			}
        };
    }
}
#endif
