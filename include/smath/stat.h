#ifndef SMATH_STAT_H
#define SMATH_STAT_H

#include "smath/mathbasic.h"
#include "smath/geom.h"

namespace slib {
	namespace sstat {
		template<size_t D, typename T>
		extern inline sinteger argmax(const sla::SVectorND<D, T>& vec) { 
			sinteger i = 0; T max = vec[0];
			sforin(d, 1, D) { if (max < vec[d]) { i = d; max = vec[d]; } }
			return i;
		}
		template<typename T>
		extern inline sinteger argmax(T* val, size_t s) {
			sinteger i = 0; T max = *val;
			sforin(k, 0, s) {
				if (max < *val) { max = *val; i = k; }
				++val;
			}
			return i;
		}
		template<typename T, class M>
		extern inline sinteger argmax(const sla::SVector<T, M>& vec) {
			sinteger i = 0; T max = vec[0];
			sforeach(vec) { if (max < E_) { i = INDEX_(vec); max = E_; } }
			return i;
		}
		template<typename T>
		extern inline sinteger argmax(sarr_citer<T> beg, sarr_citer<T> end) {
			sint idx = 0; T max = *beg;
			sforin(it, beg + 1, end) { if (max < E_) { max = E_; idx = it - beg; } }
			return idx;
		}
		template<typename T>
		extern inline sinteger argmax(scyc_citer<T> beg, scyc_citer<T> end) {
			sint idx = 0; T max = *beg;
			sfortill(it, beg + 1, end) { if (max < E_) { max = E_; idx = E_.current; } }
			return idx;
		}
		template<size_t D, typename T>
		extern inline T maxValue(const sla::SVectorND<D, T>& vec) {
			T max = vec[0];
			sforin(d, 1, D) { if (max < vec[d]) max = vec[d]; }
			return max;
		}
		template<typename T>
		extern inline T maxValue(T* val, size_t s) {
			T max = *val;
			sforin(i, 0, s) {
				if (max < *val) max = *val;
				++val;
			}
			return max;
		}
		template<typename T, class M>
		extern inline T maxValue(const sla::SVector<T, M>& vec) {
			T max = vec[0];
			sforeach(vec) { if (max < E_) max = E_; }
			return max;
		}
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
		template<typename T>
		extern inline sinteger argmin(T* val, size_t s) {
			sint idx = 0; T min = *val;
			sforin(i, 0, s) {
				if ((*val) < min) { min = *val; idx = i; }
				++val;
			}
			return idx;
		}
		template<typename T, class M>
		extern inline sinteger argmin(const sla::SVector<T, M>& vec) {
			sinteger i = 0; T min = vec[0];
			sforeach(vec) { if (E_ < min) { i = INDEX_(vec); min = E_; } }
			return i;
		}
		template<typename T>
		extern inline sinteger argmin(sarr_citer<T> beg, sarr_citer<T> end) {
			sint idx = 0; T min = *beg;
			sforin(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
			return idx;
		}
		template<typename T>
		extern inline sinteger argmin(scyc_citer<T> beg, scyc_citer<T> end) {
			sint idx = 0; T min = *beg;
			sfortill(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
			return idx;
		}
		template<size_t D, typename T>
		extern inline T minValue(const sla::SVectorND<D, T>& vec) {
			T min = vec[0];
			sforin(d, 1, D) { if (vec[d] < min) min = vec[d]; }
			return min;
		}
		template<typename T>
		extern inline T minValue(T* val, size_t s) {
			T min = *val;
			sforin(i, 0, s) {
				if (*val < min) min = *val;
				++val;
			}
			return min;
		}
		template<typename T, class M>
		extern inline T minValue(const sla::SVector<T, M>& vec) {
			T min = vec[0];
			sforeach(vec) { if (E_ < min) min = E_; }
			return min;
		}
		template<typename T>
		extern inline T minValue(sarr_citer<T> beg, sarr_citer<T> end) {
			T min = *beg;
			sforin(it, beg + 1, end) { if (E_ < min) min = E_; }
			return min;
		}
		template<typename T>
		extern inline T minValue(scyc_citer<T> beg, scyc_citer<T> end) {
			T min = *beg;
			sfortill(it, beg + 1, end) { if (E_ < min) min = E_; }
			return min;
		}
		template<size_t D, typename T>
		extern inline T sum(const sla::SVectorND<D, T>& vec) {
			T s = initVal<T>();
			sforin(d, 0, D) s += vec[d];
			return s;
		}
		template<typename T>
		extern inline T sum(T* val, size_t size) {
			T s = initVal<T>();
			sforin(i, 0, size) { s += (*val); ++val; }
			return s;
		}
		template<typename T, class M>
		extern inline T sum(const sla::SVector<T, M>& vec) {
			T s = initVal<T>();
			sforeach(vec) s += E_;
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
		template<size_t D, typename T>
		extern inline T average(const sla::SVectorND<D, T>& vec) {
			if (!D) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return sum(vec) / D;
		}
		template<typename T>
		extern inline T average(T* val, size_t len) {
			return sum<T>(val, len) / len;
		}
		template<typename T, class M>
		extern inline T average(const sla::SVector<T, M>& vec) {
			if (vec.empty()) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return sum(vec) / vec.size();
		}
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
		template<typename T, class M>
		extern inline T angaverage(const sla::SVector<T, M>& vec) {
			sla::svec2d<T> sum;
			sforeach(vec) sum += sla::svec2d<T>(cos(E_), sin(E_));
			return sla::argument(sum);
		}
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
		template<typename T, class M>
		extern inline T product(const sla::SVector<T, M>& vec) {
			T prod = unitVal<T>();
			sforeach(vec) { prod *= E_; }
			return prod;
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
		template<size_t D, typename T>
		extern inline sla::SVectorND<D-1, T> difference(const sla::SVectorND<D, T>& vec, sint i = 1) {
			sla::SVectorND<D - i, T> dif;
			sforin(d, i, D) { dif[d - i] = vec[d] - vec[d - i]; }
			return dif;
		}
		template<typename T>
		extern inline sla::SVectorND<0, T> difference(const sla::SVectorND<1, T>& vec, sint i = 1) {
			return sla::SVectorND<0, T>();
		}
		template<typename T, class M>
		extern inline sla::SVector<T, M> difference(const sla::SVector<T, M>& vec, sint i = 1) {
			sla::SVector<T, M> dif(vec.size() - i);
			sforeach2(dif, vec) { E1_ = *(it2.second + i) - E2_; }
			return dif;
		}
		template<typename T, class M>
		extern inline void maverage(const sla::SVector<T, M>& vec, sla::SVector<T, M>& ma, size_t bin, bool imputation = false) {
			if (imputation) {
				auto init = vec.begin(), last = init, end = vec.end();
				auto s = initVal<T>();
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
		template<size_t D, typename T, class M>
		extern inline T moment(const sla::SVector<T, M>& vec, size_t d) {
			T mom = initVal<T>();
			sforeach(vec) mom += smath::power(E_, D);
			return mom;
		}
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
		//extern inline double Var(const sveci& vec, bool unbiased = false) {
		//	return ((double)moment<2, sint, CMemory<sint>>(vec) - smath::power((vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		//}
		template<typename T, class M>
		extern inline T variance(const sla::SVector<T, M>& vec, bool unbiased = false) {
			return (moment<2, T, M>(vec) - smath::power(average(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		}
		template<typename T, class M>
		extern inline double covariance(const sla::SVector<T, M>& v1, const sla::SVector<T, M>& v2) {
			if (v1.empty() || v2.empty() || v1.size() != v2.size()) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
			auto ave1 = sstat::average(v1), ave2 = sstat::average(v2);
			double cov = 0;
			sforeach2(v1, v2) cov += ((double)E1_ - ave1) * ((double)E2_ - ave2);
			return cov / v1.size();
		}
		/*
		template<size_t D, typename T>
		extern inline sla::MatrixND<D, T> covariance(const sla::SVector<sla::SVectorND<D, T>, RMemory<sla::SVectorND<2, T>>>& vec) {
			auto sum = sstat::sum(vec);
			v2d ave((double)sum.x / vec.size(), (double)sum.y / vec.size());
			mat2d mat;
			double dx, dy;
			sforeach(vec) {
				dx = (double)E_.x - ave.x;
				dy = (double)E_.y - ave.y;
				mat.elements[0] += dx * dx;
				mat.elements[1] += dx * dy;
				mat.elements[3] += dy * dy;
			}
			mat.elements[2] = mat.elements[1];
			return mat / vec.size();
		}
		*/

		template<size_t D, typename T>
		extern inline sla::SMatrixND<D, T> covmat(const sla::SVector<sla::SVectorND<D, T>, RMemory<sla::SVectorND<D, T>>>& vec) {
			auto sum = sstat::sum(vec);
			sla::SVectorND<D, T> ave(sum[0] / vec.size(), sum[1] / vec.size()), diff;
			sla::SMatrixND<D, T> mat;
			T dx, dy;
			sforeach(vec) {
				sforin(i, 0, D) diff[i] = E_[i] - ave[i];
				sforin(j, 0, D) {
					sforin(k, j, D) { mat[j][k] += diff[j] * diff[k]; }
				}
			}
			sforin(l, 1, D) {
				sforin(m, 0, l) { mat[l][m] = mat[m][l]; }
			}
			return mat / vec.size();
		}
		template<typename T, class M>
		extern inline sla::SMatrix<T, M> covmat(const sla::SVector<sla::SVector<T, M>, SMemory<sla::SVector<T, M>>>& vec) {
			double sum = sstat::sum(vec);
			auto count = vec.size();
			auto dim = vec[0].size();
			svecd ave(dim, 0), diff(dim);
			sforeach2(vec, ave) E2_ = (double)E1_ / count;
			smat<T, CMemory<T>> mat(dim, dim, 0);
			double dx, dy, dz, dw;
			sforeach(vec) {
				sforin(d, 0, dim) diff[d] = (double)E_[d] - ave[d];
				sforin(i, 0, dim) {
					sforin(j, 0, i + 1) mat[i][j] += diff[i] * diff[j];
				}
			}
			sforin(i, 0, dim) {
				sforin(j, i + 1, dim) mat[i][j] = mat[j][i];
			}
			return mat / dim;
		}
		template<typename T, class M>
		extern inline T stddev(const sla::SVector<T, M>& vec, bool corrected = false) {
			return sqrt(variance(vec, corrected));
		}
		template<typename T, class M>
		extern inline T skew(const sla::SVector<T, M>& vec) {
			return moment<3, T, M>(vec) / smath::power(stddev(vec, true), 3);
		}
		template<typename T, class M>
		extern inline T kurtosis(const sla::SVector<T, M>& vec) {
			return moment<4, T, M>(vec) / smath::power(stddev(vec, true), 4);
		}
		template<typename T, class M>
		extern inline T median(const sla::SVector<T, M>& vec) {
			svec<T, M> v;
			vec.copyTo(v);
			v.sort();
			return v[vec.size() / 2];
		}
		template<typename T, class M>
		extern sla::SVectorND<3, T> quartile(const sla::SVector<T, M>& vec) {
			svec<T, M> v;
			vec.copyTo(v);
			v.sort();
			return sla::SVectorND<3, T>(v[vec.size() / 4], v[vec.size() / 2], v[3 * vec.size() / 4]);
		}

		template<typename T, class M>
		extern sint count(const sla::SVector<T, M>& vec,
			const std::function<bool(const T&)>& condition = [](const T& v) { return true; }) {
			sint i = 0;
			sforeach(vec) {
				if (condition(E_)) ++i;
			}
			return i;
		}

		extern inline std::function<double(double)> normFunc(double m = 0.0, double s = 1.0) {
			return [m, s](double x) {
				return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
			};
		}
		extern inline double dnorm(double x, double m = 0.0, double s = 1.0) {
			return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-smath::power(x - m, 2) / (2.0 * smath::power(s, 2)));
		}
		extern inline double pnorm(double x, double m = 0.0, double s = 1.0);
		extern inline double qnorm(double x, double m = 0.0, double s = 1.0);

		extern inline double dbinom(sinteger n, sinteger k, double p) {
			double d = log(p) * k + log(1.0 - p) * (n - k);
			sforin(i, 0, k) { d += log(n); --n; }
			while(1 < k) { d -= log(k); --k; }
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
				double p = x * log(l) - l;
				sforin(i, 2, x + 1) p -= log(i);
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