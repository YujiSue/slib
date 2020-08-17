#ifndef SMATH_STAT_H
#define SMATH_STAT_H

#include "smath/mathbasic.h"
#include "smath/geom.h"

namespace slib {
	namespace sstat {
		template<typename T>
		extern inline sint argmax(T* val, size_t s) {
			sint idx = 0; T max = *val;
			sforin(i, 0, s) {
				if (max < *val) { max = *val; idx = i; }
				++val;
			}
			return idx;
		}
		template<typename T>
		extern inline sint argmax(sarr_citer<T> beg, sarr_citer<T> end) {
			sint idx = 0; T max = *beg;
			sforin(it, beg + 1, end) { if (max < E_) { max = E_; idx = it - beg; } }
			return idx;
		}
		template<typename T>
		extern inline sint argmax(scyc_citer<T> beg, scyc_citer<T> end) {
			sint idx = 0; T max = *beg;
			sfortill(it, beg + 1, end) { if (max < E_) { max = E_; idx = E_.current; } }
			return idx;
		}
		template<typename T, class M>
		extern inline sint argmax(const sla::SVector<T, M>& vec) {
			return maxi(vec.begin(), vec.end());
		}
		template<typename T>
		extern inline T maxv(T* val, size_t s) {
			T max = *val;
			sforin(i, 0, s) {
				if (max < *val) max = *val;
				++val;
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
		extern inline T maxv(const sla::SVector<T, M>& vec) {
			return maxIn(vec.begin(), vec.end());
		}
		template<typename T>
		extern inline sint argmin(T* val, size_t s) {
			sint idx = 0; T min = *val;
			sforin(i, 0, s) {
				if ((*val) < min) { min = *val; idx = i; }
				++val;
			}
			return idx;
		}
		template<typename T>
		extern inline sint argmin(sarr_citer<T> beg, sarr_citer<T> end) {
			sint idx = 0; T min = *beg;
			sforin(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
			return idx;
		}
		template<typename T>
		extern inline sint argmin(scyc_citer<T> beg, scyc_citer<T> end) {
			sint idx = 0; T min = *beg;
			sfortill(it, beg + 1, end) { if (E_ < min) { min = E_; idx = it - beg; } }
			return idx;
		}
		template<typename T, class M>
		extern inline sint argmin(const sla::SVector<T, M>& vec) {
			return argmin(vec.begin(), vec.end());
		}
		template<typename T>
		extern inline T minv(T* val, size_t s) {
			T min = *val;
			sforin(i, 0, s) {
				if (*val < min) min = *val;
				++val;
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
		extern inline T minv(const sla::SVector<T, M>& vec) {
			return minv(vec.begin(), vec.end());
		}
		template<typename T>
		extern inline T sum(T* val, size_t size) {
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
		extern inline T sum(const sla::SVector<T, M>& vec) {
			return sum(vec.begin(), vec.end());
		}
		extern inline double Mean(int* val, size_t len) {
			return ((double)(sum(val, len))) / len;
		}
		template<typename T>
		extern inline T average(T* val, size_t len) {
			return sum<T>(val, len) / len;
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
		template<typename T, class M>
		extern inline T average(const sla::SVector<T, M>& vec) {
			if (vec.empty()) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return sum(vec) / vec.size();
		}
		extern inline double Mean(const sveci& vec) {
			if (vec.empty()) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return ((double)(sstat::sum(vec))) / vec.size();
		}
		template<typename T>
		extern inline T angaverage(sarr_citer<T> beg, sarr_citer<T> end) {
			v2d sum;
			sforin(it, beg, end) {
				sum += v2d(cos(E_), sin(E_));
				ssgeom::normalize(sum);
			}
			return ssgeom::argument(sum);
		}
		template<typename T>
		extern inline T angaverage(scyc_citer<T> beg, scyc_citer<T> end) {
			v2d sum;
			sfortill(it, beg, end) {
				sum += v2d(cos(E_), sin(E_));
				ssgeom::normalize(sum);
			}
			return ssgeom::argument(sum);
		}
		template<typename T, class M>
		extern inline T angaverage(const sla::SVector<T, M>& vec) {
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
		extern inline T product(const sla::SVector<T, M>& vec) {
			return product(vec.begin(), vec.end());
		}
		template<typename T, class M>
		extern inline void difference(const sla::SVector<T, M>& vec, sla::SVector<T, M>& diff) {
			sforin(it, vec.begin(), vec.end() - 1) diff.add(E_NEXT - E_);
		}
		template<typename T, class M>
		extern inline T maverage(const sla::SVector<T, M>& vec, sla::SVector<T, M>& ma, size_t bin, bool imputation = false) {
			if (imputation) {
				auto init = vec.begin(), last = init, end = vec.end();
				auto sum = initVal<T>();
				sforin(i, 0, bin - 1) {
					sum += (*last);
					ma.add(sum / (i + 1));
					++last;
				}
				while (last < end) {
					sum += (*last);
					ma.add(sum / bin);
					sum -= (*init);
					++init; ++last;
				}
				srforin(i, 0, bin - 1) {
					sum -= (*init);
					ma.add(sum / (i + 1));
					++init;
				}
			}
			else {
				auto init = vec.begin(), last = init + bin, end = vec.end();
				auto sum = sum(init, last - 1);
				while (last < end) {
					sum += (*last);
					ma.add(sum / bin);
					sum -= (*init);
					++init; ++last;
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
		extern inline T moment(const sla::SVector<T, M>& vec) {
			return moment<Dim, T>(vec.begin(), vec.end());
		}
		extern inline double Var(const sveci& vec, bool unbiased = false) {
			return ((double)moment<2, sint, CMemory<sint>>(vec) - smath::power(Mean(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		}
		template<typename T, class M>
		extern inline T variance(const sla::SVector<T, M>& vec, bool unbiased = false) {
			return (moment<2, T, M>(vec) - smath::power(average(vec), 2)) / (unbiased ? vec.size() - 1 : vec.size());
		}
		template<typename T>
		extern inline double covariance(const sla::SVector<svec2d<T>, RMemory<svec2d<T>>>& vec) {
			auto sum = sstat::sum(vec);
			v2d ave((double)sum.x / vec.size(), (double)sum.y / vec.size());
			auto cov = 0;
			sforeach(vec) cov += ((double)vec.x - ave.x) * ((double)vec.y - ave.y);
			return cov / vec.size();
		}
		template<typename T>
		extern inline mat2d covmat(const sla::SVector<svec2d<T>, RMemory<svec2d<T>>>& vec) {
			auto sum = sstat::sum(vec);
			v2d ave((double)sum.x / vec.size(), (double)sum.y / vec.size());
			mat2d mat;
			double dx, dy;
			sforeach(vec) {
				dx = (double)E_.x - ave.x;
				dy = (double)E_.y - ave.y;
				mat.element[0] += dx * dx;
				mat.element[1] += dx * dy;
				mat.element[3] += dy * dy;
			}
			mat.element[2] = mat.element[1];
			return mat / vec.size();
		}
		template<typename T>
		extern inline mat3d covmat(const sla::SVector<svec3d<T>, RMemory<svec3d<T>>>& vec) {
			auto sum = sstat::sum(vec);
			v3d ave((double)sum.x / vec.size(), (double)sum.y / vec.size(), (double)sum.z / vec.size());
			mat3d mat;
			double dx, dy, dz;
			sforeach(vec) {
				dx = (double)E_.x - ave.x;
				dy = (double)E_.y - ave.y;
				dz = (double)E_.z - ave.z;
				mat.element[0] += dx * dx;
				mat.element[1] += dx * dy;
				mat.element[2] += dx * dz;
				mat.element[4] += dy * dy;
				mat.element[5] += dy * dz;
				mat.element[8] += dz * dz;
			}
			mat.element[3] = mat.element[1];
			mat.element[6] = mat.element[2];
			mat.element[7] = mat.element[5];
			return mat / vec.size();
		}
		template<typename T>
		extern inline mat4d covmat(const sla::SVector<svec4d<T>, RMemory<svec4d<T>>>& vec) {
			auto sum = sstat::sum(vec);
			v4d ave((double)sum.x / vec.size(), (double)sum.y / vec.size(), (double)sum.z / vec.size(), (double)sum.w / vec.size());
			mat4d mat;
			double dx, dy, dz, dw;
			sforeach(vec) {
				dx = (double)E_.x - ave.x;
				dy = (double)E_.y - ave.y;
				dz = (double)E_.z - ave.z;
				dw = (double)E_.w - ave.w;
				mat.element[0] += dx * dx;
				mat.element[1] += dx * dy;
				mat.element[2] += dx * dz;
				mat.element[3] += dx * dw;
				mat.element[5] += dy * dy;
				mat.element[6] += dy * dz;
				mat.element[7] += dy * dw;
				mat.element[10] += dz * dz;
				mat.element[11] += dz * dw;
				mat.element[15] += dw * dw;
			}
			mat.element[4] = mat.element[1];
			mat.element[8] = mat.element[2];
			mat.element[9] = mat.element[6];
			mat.element[12] = mat.element[3];
			mat.element[13] = mat.element[7];
			mat.element[14] = mat.element[11];
			return mat / vec.size();
		}
		template<typename T>
		extern inline sla::SMatrix<T, CMemory<T>> covmat(const sla::SVector<sla::SVector<T, CMemory<T>>>& vec) {
			auto sum = sstat::sum(vec);
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
		extern inline double StdDev(const sveci& vec, bool corrected = false) {
			return sqrt(Var(vec, corrected));
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
			svec<T, M> vec_;
			vec.copyTo(vec_);
			vec_.sort();
			return vec_[vec.size() / 2];
		}
		template<typename T, class M>
		extern svec3d<T> quartile(const sla::SVector<T, M>& vec) {
			svec<T, M> vec_;
			vec.copyTo(vec_);
			vec_.sort();
			return svec3d<T>(vec_[vec.size() / 4], vec_[vec.size() / 2], vec_[3 * vec.size() / 4]);
		}

		template<typename T, class M>
		extern sint count(const sla::SVector<T, M>& vec,
			std::function<bool(sarr_citer<T> & iter)> condition = [](sarr_citer<T>&) { return true; }) {
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
		extern inline double norm(double x, double m = 0.0, double s = 1.0) {
			return 1.0 / (sqrt(2 * smath::PI) * s) * exp(-(x - m) * (x - m) / (2.0 * s));
		}
		extern inline double pnorm(double x, double m = 0.0, double s = 1.0);
		extern inline double qnorm(double x, double m = 0.0, double s = 1.0);

		extern inline std::function<double(double)> chisqFunc(double x, double n);
		extern inline double chisq(double x, double n);
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

#endif
