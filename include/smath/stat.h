#ifndef SMATH_STAT_H
#define SMATH_STAT_H

#include "smath/mathbasic.h"
#include "smath/geom.h"

namespace slib {
    namespace smath {
        namespace sstat {
			template<typename T>
			extern inline size_t maxi(T* val, size_t s) {
				size_t idx = 0; T max = *val;
				sforin(i, 0, s) {
					if (max < *val) { max = *val; idx = i; }
				}
				return idx;
			}
			template<typename T>
			extern inline size_t maxi(sarr_citer<T> beg, sarr_citer<T> end) {
				size_t idx = 0; T max = *beg;
				sforin(it, beg + 1, end) { if (max < E_) { max = E_; idx = it - beg; } }
				return idx;
			}
			template<typename T>
			extern inline size_t maxi(scyc_citer<T> beg, scyc_citer<T> end) {
				size_t idx = 0; T max = *beg;
				sfortill(it, beg + 1, end) { if (max < E_) { max = E_; idx = E_.current; } }
				return idx;
			}
			template<typename T, class M>
			extern inline size_t maxi(const SVector<T, M>& vec) {
				return maxi(vec.begin(), vec.end());
			}
			template<typename T>
			extern inline T maxv(T* val, size_t s) {
				T max = *val;
				sforin(i, 0, s) {
					if (max < *val) max = *val;
				}
				return max;
			}
			template<typename T>
			extern inline T maxv(sarr_citer<T> beg, sarr_citer<T> end) {
				T max = *beg;
				sforin(it, beg + 1, end) { if (max < E_) max = E_; }
				return max;
			}
			template<typename T>
			extern inline T maxv(scyc_citer<T> beg, scyc_citer<T> end) {
				T max = *beg;
				sfortill(it, beg + 1, end) { if (max < E_) max = E_; }
				return max;
			}
			template<typename T, class M>
			extern inline T maxv(const SVector<T, M>& vec) {
				return maxIn(vec.begin(), vec.end());
			}
			template<typename T>
			extern inline size_t mini(T* val, size_t s) {
				size_t idx = 0; T min = *val;
				sforin(i, 0, s) {
					if ((*val) < min) { min = *val; idx = i; }
				}
				return idx;
			}
			template<typename T>
			extern inline size_t mini(sarr_citer<T> beg, sarr_citer<T> end) {
				size_t idx = 0; T min = *beg;
				sforin(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
				return idx;
			}
			template<typename T>
			extern inline size_t mini(scyc_citer<T> beg, scyc_citer<T> end) {
				size_t idx = 0; T min = *beg;
				sfortill(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
				return idx;
			}
			template<typename T, class M>
			extern inline size_t mini(const SVector<T, M>& vec) {
				return mini(vec.begin(), vec.end());
			}
			template<typename T>
			extern inline T minv(T* val, size_t s) {
				T min = *val;
				sforin(i, 0, s) {
					if (*val < min) min = *val;
				}
				return min;
			}
			template<typename T>
			extern inline T minv(sarr_citer<T> beg, sarr_citer<T> end) {
				T min = *beg;
				sforin(it, beg + 1, end) { if (E_ < min) min = E_; }
				return min;
			}
			template<typename T>
			extern inline T minv(scyc_citer<T> beg, scyc_citer<T> end) {
				T min = *beg;
				sfortill(it, beg + 1, end) { if (E_ < min) min = E_; }
				return min;
			}
			template<typename T, class M>
			extern inline T minv(const SVector<T, M>& vec) {
				return minv(vec.begin(), vec.end());
			}
			template<typename T>
			extern inline T sum(T *val, size_t size) {
				T s = initVal<T>();
				sforin(i, 0, size) { s += *val; ++val; }
				return s;
			}
			template<typename T>
			extern inline T sum(sarr_citer<T> beg, sarr_citer<T> end) {
				T s = initVal<T>();
				sforin(it, beg, end) s += E_;
				return s;
			}
			template<typename T>
			extern inline T sum(scyc_citer<T> beg, scyc_citer<T> end) {
				T s = initVal<T>();
				sfortill(it, beg, end) s += E_;
				return s;
			}
			template<typename T, class M>
			extern inline T sum(const SVector<T, M>& vec) {
				return sum(vec.begin(), vec.end());
			}
			extern inline double Mean(int *val, size_t len) {
				return ((double)(sum(val, len))) / len;
			}
			template<typename T>
			extern inline T average(T* val, size_t len) {
				return sum<T>(val, len) / len;
			}
			template<typename T>
			extern inline T average(sarr_citer<T> beg, sarr_citer<T> end) {
				if (end - beg) return (sumIn(beg, end)) / (end - beg);
				throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			}
			template<typename T>
			extern inline T average(scyc_citer<T> beg, scyc_citer<T> end) {
				if (end - beg) return (sumIn(beg, end)) / (end - beg);
				throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			}
			template<typename T, class M>
			extern inline T average(const SVector<T, M>& vec) {
				if (vec.empty()) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
				return sum(vec) / vec.size();
			}
			extern inline double Mean(const sveci &vec) {
				if (vec.empty()) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
				return ((double)(sum(vec))) / vec.size();
			}
			template<typename T>
			extern inline T angaverage(sarr_citer<T> beg, sarr_citer<T> end) {
				v2d sum;
				sforin(it, beg, end) {
					sum += v2d(cos(E_), sin(E_));
					sgeom::normalize(sum);
				}
				return sgeom::argument(sum);
			}
			template<typename T>
			extern inline T angaverage(scyc_citer<T> beg, scyc_citer<T> end) {
				v2d sum;
				sfortill(it, beg, end) {
					sum += v2d(cos(E_), sin(E_));
					sgeom::normalize(sum);
				}
				return sgeom::argument(sum);
			}
			template<typename T, class M>
			extern inline T angaverage(const SVector<T, M>& vec) {
				return angaverage(vec.begin(), vec.end());
			}
			template<typename T>
			extern inline T product(sarr_citer<T> beg, sarr_citer<T> end) {
				T prod = unitVal<T>();
				sforin(it, beg, end) prod *= E_;
				return prod;
			}
			template<typename T>
			extern inline T product(scyc_citer<T> beg, scyc_citer<T> end) {
				T prod = unitVal<T>();
				sfortill(it, beg, end) prod *= E_;
				return prod;
			}
			template<typename T, class M>
			extern inline T product(const SVector<T, M>& vec) {
				return product(vec.begin(), vec.end());
			}
			template<typename T, class M>
			extern inline void difference(const SVector<T, M>& vec, SVector<T, M>& diff) {
				sforin(it, vec.begin(), vec.end()-1) diff.add(E_NXT - E_);
			}
			template<typename T, class M>
			extern inline T maverage(const SVector<T, M>& vec, SVector<T, M>& ma, size_t bin, bool imputation = false) {
				if (imputation) {



				}
				else {
					auto init = vec.begin(), last = init + bin;
					auto ave = average(init, last);
					sforin(it, vec.begin(), vec.end() - bin) {



					}
				}
				return ma;
			}
			template<size_t Dim, typename T>
			extern inline T moment(sarr_citer<T> beg, sarr_citer<T> end) {
				T mom = initVal<T>();
				sforin(it, beg, end) mom += smath::power(E_, Dim);
				return mom;
			}
			template<size_t Dim, typename T>
			extern inline T moment(scyc_citer<T> beg, scyc_citer<T> end) {
				T mom = initVal<T>();
				sfortill(it, beg, end) mom += smath::power(E_, Dim);
				return mom;
			}
			template<size_t Dim, typename T, class M>
			extern inline T moment(const SVector<T, M>& vec) {
				return moment<Dim, T>(vec.begin(), vec.end());
			}
			extern inline double Var(const sveci& vec, bool unbiased = false) {
				return ((double)moment<2, sint, CMemory<sint>>(vec) - smath::power(Mean(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
			}
			template<typename T, class M>
			extern inline T variance(const SVector<T, M>& vec, bool unbiased = false) {
				return (moment<2, T, M>(vec) - smath::power(average(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
			}
			template<typename T, class M>
			extern inline T stddev(const SVector<T, M>& vec, bool corrected = false) {
				return sqrt(variance(vec, corrected));
			}
			extern inline double StdDev(const sveci& vec, bool corrected = false) {
				return sqrt(Var(vec, corrected));
			}
			template<typename T, class M>
			extern inline T skew(const SVector<T, M>& vec) {
				return moment<3, T, M>(vec) / smath::power(stddev(vec, true), 3);
			}
			template<typename T, class M>
			extern inline T kurtosis(const SVector<T, M>& vec) {
				return moment<4, T, M>(vec) / smath::power(stddev(vec, true), 4);
			}
			template<typename T, class M>
			extern inline T median(const SVector<T, M>& vec) {
				SVector<T, M> vec_;
				vec.copyTo(vec_);
				vec_.sort();
				return vec_[vec.size() / 2];
			}
			template<typename T, class M>
			extern svec3d<T> quartile(const SVector<T, M>& vec) {
				SVector<T, M> vec_;
				vec.copyTo(vec_);
				vec_.sort();
				return svec3d<T>(vec_[vec.size() / 4], vec_[vec.size() / 2], vec_[3 * vec.size() / 4]);
			}

            extern inline std::function<double(double)> normFunc(double m = 0.0, double s = 1.0) {
				return [m, s](double x) {
					return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
				};
			}
            extern inline double norm(double x, double m = 0.0, double s = 1.0) {
				return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
			}
            extern inline double pnorm(double x, double m = 0.0, double s = 1.0);
            extern inline double qnorm(double x, double m = 0.0, double s = 1.0);
            
            extern inline std::function<double(double)> chisqFunc(double x, double n);
            extern inline double chisq(double x, double n);
            extern inline double pchisq(double x, double n);
            extern inline double qchisq(double x, double n);
            
            extern inline std::function<double(int)> poisFunc(double l) {
				return [l](int x) {
					return (pow(l, x) * exp(-l)) / smath::factorial(x);
				};
			}
            extern inline double pois(int x, double l) { return (pow(l, x) * exp(-l)) / smath::factorial(x); }
            extern inline double ppois(int x, double l);
            extern inline double qpois(int x, double l);
            
			extern inline double gammaFunc(double d);
            extern inline double ibetaFunc(double d, double m, double n);
            extern inline double betaFunc(double m, double n);
            
        }
    }
}

#endif
