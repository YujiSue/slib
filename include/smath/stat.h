#ifndef SMATH_STAT_H
#define SMATH_STAT_H
#include "smath/vector.h"
#include "smath/matrix.h"
namespace slib {
	namespace sstat {		
		template<typename T>
		extern inline size_t argmax(T* val, size_t s) {
			size_t idx = 0; T max = *val;
			sforin(i, 1_u, s) {
				if (max < *val) { max = *val; idx = i; }
				++val;
			}
			return idx;
		}
		template<typename T, size_t D>
		extern inline size_t argmax(const smath::VectorND<T, D>& vec) {
			size_t idx = 0; T max = vec[0];
			sforin(i, 1_u, D) {
				if (max < vec[i]) { max = vec[i]; idx = i; }
			}
			return idx;
		}
		template<typename T>
		extern inline size_t argmax(const smath::Vector<T>& vec) {
			size_t idx = 0; T max = vec[0];
			sfor(vec) { if (max < $_) { idx = INDEX_($, vec); max = $_; } }
			return idx;
		}
		template<typename T, typename... Args>
		inline T _max(T &max) { return max; }
		template<typename T, typename... Args>
		inline T _max(T& max, T first, Args... args) {
			if (max < first) return _max(first, args...);
			else return _max(max, args...);
		}
		template<typename T, typename... Args>
		extern inline T getMax(T first, Args... args) {
			T max = first;
			return _max(max, args...);
		}
		template<typename T>
		extern inline T maxValue(T* val, size_t s) {
			T max = *val;
			sforin(i, 1_u, s) {
				if (max < *val) max = *val;
				++val;
			}
			return max;
		}
		template<typename T>
		extern inline T maxValue(const smath::Vector<T>& vec) {
			T max = vec[0];
			sfor(vec) { if (max < $_) max = $_; }
			return max;
		}
		/*
		template<typename T>
		extern inline T maxValue(sarr_citer<T> beg, sarr_citer<T> end) {
			T max = *beg;
			sforin(it, beg + 1, end) { if (max < E_) max = E_; }
			return max;
		}
		template<typename T>
		extern inline T maxValue(scyc_citer<T> beg, scyc_citer<T> end) {
			T max = *beg;
			sfortill(it, beg + 1, end) { if (max < E_) max = E_; }
			return max;
		}
		template<size_t D, typename T>
		extern inline sinteger argmin(const sla::SVectorND<D, T>& vec) {
			sinteger i = 0; T min = vec[0];
			sforin(d, 1, D) { if (vec[d] < min) { i = d; min = vec[d]; } }
			return i;
		}
		*/
		template<typename T>
		extern inline size_t argmin(T* val, size_t s) {
			size_t idx = 0; T min = *val;
			sforin(i, 1_u, s) {
				if ((*val) < min) { min = *val; idx = i; }
				++val;
			}
			return idx;
		}
		template<typename T>
		extern inline sinteger argmin(const smath::Vector<T>& vec) {
			size_t i = 0; T min = vec[0];
			sfor(vec) { if ($_ < min) { i = INDEX_($, vec); min = $_; } }
			return i;
		}
		template<typename T, typename... Args>
		inline T _min(T& min) { return min; }
		template<typename T, typename... Args>
		inline T _min(T& min, T first, Args... args) {
			if (first < min) return _min(first, args...);
			else return _min(min, args...);
		}
		template<typename T, typename... Args>
		extern inline T getMin(T first, Args... args) {
			T min = first;
			return _min(min, args...);
		}
		template<typename T>
		extern inline T minValue(T* val, size_t s) {
			T min = *val;
			sforin(i, 1_u, s) {
				if (*val < min) min = *val;
				++val;
			}
			return min;
		}
		template<typename T>
		extern inline T minValue(const smath::Vector<T>& vec) {
			T min = vec[0];
			sfor(vec) { if ($_ < min) min = $_; }
			return min;
		}
		template<typename T>
		extern inline T sum(T* val, const size_t sz) {
			T s = smath::zero<T>();
			sforin(i, 0_u, sz) { s += (*val); ++val; }
			return s;
		}
		template<typename T>
		extern inline T sum(const smath::Vector<T>& vec) {
			T s = smath::zero<T>();
			sfor(vec) { s += $_; }
			return s;
		}
		template<typename T>
		extern inline T average(T* val, const size_t sz) {
			return sum<T>(val, sz) / sz;
		}
		template<typename T>
		extern inline T average(const smath::Vector<T>& vec) {
			if (vec.empty()) throw DivZeroException(divZeroErrorText("Vector size"));
			return sum<T>(vec) / vec.size();
		}
		/*
		template<typename T>
		extern inline T average(sarr_citer<T> beg, sarr_citer<T> end) {
			if (end - beg) return (sumIn(beg, end)) / (end - beg);
			throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
		}
		template<typename T>
		extern inline T average(scyc_citer<T> beg, scyc_citer<T> end) {
			if (end - beg) return (sumIn(beg, end)) / (end - beg);
			throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
		}		
		template<size_t D, typename T>
		extern inline T angaverage(const sla::SVectorND<D, T>& vec) {
			sla::svec2d<T> sum;
			sforin(d, 0, D) sum += sla::svec2d<T>(cos(vec[d]), sin(vec[d]));
			return sla::argument(sum);
		}
		*/
		template<typename T>
		extern inline T angaverage(const smath::Vector<T>& vec) {
			smath::Vector2D<T> sum;
			sfor(vec) sum += smath::Vector2D<T>(std::cos($_), std::sin($_));
			return smath::argument(sum);
		}
		/*
		template<typename T>
		extern inline T angaverage(sarr_citer<T> beg, sarr_citer<T> end) {
			v2d sum;
			sforin(it, beg, end) {
				sum += v2d(cos(E_), sin(E_));
				sla::normalize(sum);
			}
			return sla::argument(sum);
		}
		template<typename T>
		extern inline T angaverage(scyc_citer<T> beg, scyc_citer<T> end) {
			v2d sum;
			sfortill(it, beg, end) {
				sum += v2d(cos(E_), sin(E_));
				sla::normalize(sum);
			}
			return sla::argument(sum);
		}
		template<size_t D, typename T>
		extern inline T product(const sla::SVectorND<D, T>& vec) {
			T prod = unitVal<T>();
			sforin(d, 0, D) { prod *= vec[d]; }
			return prod;
		}
		*/
		template<typename T>
		extern inline T product(const smath::Vector<T>& vec) {
			T prod = smath::unit<T>();
			sfor(vec) { prod *= $_; }
			return prod;
		}
		/*
		template<typename T>
		extern inline T product(sarr_citer<T> beg, sarr_citer<T> end) {
			T prod = smath::unit<T>();
			sforin(it, beg, end) prod *= E_;
			return prod;
		}
		template<typename T>
		extern inline T product(scyc_citer<T> beg, scyc_citer<T> end) {
			T prod = unitVal<T>();
			sfortill(it, beg, end) prod *= E_;
			return prod;
		}
		*/
		template<typename T, size_t D, size_t S>
		extern inline smath::VectorND<T, D - S> difference(const smath::VectorND<T, D>& vec) {
			smath::VectorND<T, D - S> dif;
			sforin(d, S, D) { dif[d - S] = vec[d] - vec[d - S]; }
			return dif;
		}
		template<typename T>
		extern inline smath::VectorND<T, 0> difference(const smath::VectorND<T, 1>& vec, sint i = 1) {
			return smath::VectorND<T, 0>();
		}
		template<typename T>
		extern inline smath::Vector<T> difference(const smath::Vector<T>& vec, sint i = 1) {
			smath::Vector<T> dif(vec.size() - i);
			sfor2(dif, vec) { $_1 = *(it.second + i) - $_2; }
			return dif;
		}
		template<typename T>
		extern inline void maverage(const smath::Vector<T>& vec, smath::Vector<T>& ma, size_t bin, bool imputation = false) {
			if (imputation) {
				auto init = vec.begin(), last = init, end = vec.end();
				auto s = T();
				sforin(i, 0, bin - 1) {
					s += (*last);
					ma.add(s / (i + 1));
					++last;
				}
				while (last < end) {
					s += (*last);
					ma.add(s / bin);
					s -= (*init);
					++init; ++last;
				}
				srforin(i, 0, bin - 1) {
					s -= (*init);
					ma.add(s / (i + 1));
					++init;
				}
			}
			else {
				auto init = vec.begin(), last = init + bin, end = vec.end();
				auto s = sum(init, last - 1);
				while (last < end) {
					s += (*last);
					ma.add(s / bin);
					s -= (*init);
					++init; ++last;
				}
			}
		}
		template<size_t D, typename T>
		extern inline T moment(const smath::Vector<T>& vec, const size_t d) {
			T mom = 0;
			sfor(vec) mom += smath::power($_, D);
			return mom;
		}
		/*
		template<size_t D, typename T>
		extern inline T moment(sarr_citer<T> beg, sarr_citer<T> end) {
			T mom = initVal<T>();
			sforin(it, beg, end) mom += smath::power(E_, D);
			return mom;
		}
		template<size_t D, typename T>
		extern inline T moment(scyc_citer<T> beg, scyc_citer<T> end) {
			T mom = initVal<T>();
			sfortill(it, beg, end) mom += smath::power(E_, D);
			return mom;
		}
		*/
		//extern inline double Var(const sveci& vec, bool unbiased = false) {
		//	return ((double)moment<2, sint, CMemory<sint>>(vec) - smath::power((vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		//}
		template<typename T>
		extern inline T variance(const smath::Vector<T>& vec, bool unbiased = false) {
			return (moment<2, T>(vec) - smath::power(average(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		}
		template<typename T>
		extern inline double covariance(const smath::Vector<T>& v1, const smath::Vector<T>& v2) {
			if (v1.empty() || v2.empty() || v1.size() != v2.size()) throw Exception();// smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
			auto ave1 = average(v1), ave2 = average(v2);
			double cov = 0;
			sfor2(v1, v2) cov += ((double)$_1 - ave1) * ((double)$_2 - ave2);
			return cov / v1.size();
		}
		template<typename T, size_t D>
		extern inline smath::MatrixND<T, D> covmat(const smath::Vector<smath::VectorND<T, D>>& vec) {
			auto sum = sstat::sum(vec);
			smath::VectorND<T, D> ave(sum[0] / vec.size(), sum[1] / vec.size()), diff;
			smath::MatrixND<T, D> mat;
			T dx, dy;
			sfor(vec) {
				sforin(i, 0, D) diff[i] = $_[i] - ave[i];
				sforin(j, 0, D) {
					sforin(k, j, D) { mat[j][k] += diff[j] * diff[k]; }
				}
			}
			sforin(l, 1, D) {
				sforin(m, 0, l) { mat[l][m] = mat[m][l]; }
			}
			return mat / vec.size();
		}
		template<typename T>
		extern inline smath::Matrix<T> covmat(const smath::Vector<smath::Vector<T>>& vec) {
			double sum = sstat::sum(vec);
			auto count = vec.size();
			auto dim = vec[0].size();
			svecd ave(dim, 0), diff(dim);
			sfor2(vec, ave) $_2 = (double)$_1 / count;
			smat<T> mat(dim, dim, 0);
			double dx, dy, dz, dw;
			sfor(vec) {
				sforin(d, 0, dim) diff[d] = (double)$_[d] - ave[d];
				sforin(i, 0, dim) {
					sforin(j, 0, i + 1) mat[i][j] += diff[i] * diff[j];
				}
			}
			sforin(i, 0, dim) {
				sforin(j, i + 1, dim) mat[i][j] = mat[j][i];
			}
			return mat / dim;
		}
		template<typename T>
		extern inline T stddev(const smath::Vector<T>& vec, bool corrected = false) {
			return sqrt(variance(vec, corrected));
		}
		template<typename T>
		extern inline T skew(const smath::Vector<T>& vec) {
			return moment<3, T>(vec) / smath::power(stddev(vec, true), 3);
		}
		template<typename T>
		extern inline T kurtosis(const smath::Vector<T>& vec) {
			return moment<4, T>(vec) / smath::power(stddev(vec, true), 4);
		}
		template<typename T>
		extern inline T median(const smath::Vector<T>& vec) {
			svec<T> v;
			vec.copyTo(v);
			//v.sort();
			return v[vec.size() / 2];
		}
		template<typename T, class M>
		extern inline smath::VectorND<T, 3> quartile(const smath::Vector<T>& vec) {
			svec<T> v;
			vec.copyTo(v);
			//v.sort();
			return smath::VectorND<T, 3>(v[vec.size() / 4], v[vec.size() / 2], v[3 * vec.size() / 4]);
		}

		template<typename T>
		extern sint count(const smath::Vector<T>& vec,
			const std::function<bool(const T&)>& condition = [](const T& v) { return true; }) {
			sint i = 0;
			sfor(vec) { if (condition($_)) { ++i; } }
			return i;
		}

		extern inline std::function<double(double)> normFunc(double m = 0.0, double s = 1.0) {
			return [m, s](double x) {
				return 1.0 / (sqrt(2 * snum::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
			};
		}
		extern inline double dnorm(double x, double m = 0.0, double s = 1.0) {
			return 1.0 / (sqrt(2 * snum::PI) * s) * exp(-smath::power(x - m, 2) / (2.0 * smath::power(s, 2)));
		}
		extern inline double pnorm(double x, double m = 0.0, double s = 1.0);
		extern inline double qnorm(double x, double m = 0.0, double s = 1.0);

		extern inline double dbinom(sinteger n, sinteger k, double p) {
			double d = std::log(p) * k + std::log(1.0 - p) * (n - k);
			sforin(i, 0, k) { d += std::log(n); --n; }
			while(1 < k) { d -= std::log(k); --k; }
			return exp(d);
		}
		extern inline double pbinom(double x, double m = 0.0, double s = 1.0);
		extern inline double qbinom(double x, double m = 0.0, double s = 1.0);
		extern inline double binomTest(sinteger n, sinteger k, double p) {
			double pval = 0.0;
			if (k < n - k) {
				sforin(i, 0, k) { pval += dbinom(n, i, p); }
				pval = pval < 1.0 ? 1.0 - pval : 0.0;
			}
			else { sforin(i, k, n) { pval += dbinom(n, i, p); } }
			return pval;
		}

		extern inline std::function<double(double)> chisqFunc(double x, double n);
		extern inline double dchisq(double x, double n);
		extern inline double pchisq(double x, double n);
		extern inline double qchisq(double x, double n);

		/*
		extern inline std::function<double(double)> binomFunc(double m = 0.0, double s = 1.0) {
				return [m, s](double x) {
						return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
				};
		}
		extern inline double pbinom(int x, int n, double p) {

		}
		*/

		extern inline std::function<double(int)> poisFunc(double l) {
			return [l](int x) { return (pow(l, x) * exp(-l)) / smath::factorial(x); };
		}
		extern inline double dpois(int x, double l) { 
			if (12 < x) {
				double p = x * std::log(l) - l;
				sforin(i, 2, x + 1) p -= std::log(i);
				return exp(p);
			}
			else return (pow(l, x) * exp(-l)) / smath::factorial(x); 
		}
		extern inline double ppois(int x, double l);
		extern inline double qpois(int x, double l);

		extern inline double gammaFunc(double d);
		extern inline double ibetaFunc(double d, double m, double n);
		extern inline double betaFunc(double m, double n);

	}
}
#endif