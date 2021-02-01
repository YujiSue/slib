#ifndef SMATH_LA_H
#define SMATH_LA_H

#include "sconfig.h"
#include "smath/calc.h"
#include "smath/frac.h"
#include "smath/comp.h"
#include "sbasic/array.h"
#include "sbasic/cyciter.h"
#include "sbasic/string.h"

#define v2b slib::sla::SVectorND<2, sbyte>
#define v2ub slib::sla::SVectorND<2, subyte>
#define v2i slib::sla::SVectorND<2, sint>
#define v2l slib::sla::SVectorND<2, sinteger>
#define v2f slib::sla::SVectorND<2, float>
#define v2d slib::sla::SVectorND<2, double>
#define v2n slib::sla::SVectorND<2, sfrac>
#define v2x slib::sla::SVectorND<2, scomp>
#define v3b slib::sla::SVectorND<3, sbyte>
#define v3ub slib::sla::SVectorND<3, subyte>
#define v3i slib::sla::SVectorND<3, sint>
#define v3l slib::sla::SVectorND<3, sinteger>
#define v3f slib::sla::SVectorND<3, float>
#define v3d slib::sla::SVectorND<3, double>
#define v3n slib::sla::SVectorND<3, sfrac>
#define v3x slib::sla::SVectorND<3, scomp>
#define v4b slib::sla::SVectorND<4, sbyte>
#define v4ub slib::sla::SVectorND<4, subyte>
#define v4i slib::sla::SVectorND<4, sint>
#define v4l slib::sla::SVectorND<4, sinteger>
#define v4f slib::sla::SVectorND<4, float>
#define v4d slib::sla::SVectorND<4, double>
#define v4n slib::sla::SVectorND<4, sfrac>
#define v4x slib::sla::SVectorND<4, scomp>
#define mat2i slib::sla::SMatrixND<2, sint>
#define mat2l slib::sla::SMatrixND<2, sinteger>
#define mat2f slib::sla::SMatrixND<2, float>
#define mat2d slib::sla::SMatrixND<2, double>
#define mat2n slib::sla::SMatrixND<2, sfrac>
#define mat2x slib::sla::SMatrixND<2, scomp>
#define mat3i slib::sla::SMatrixND<3, sint>
#define mat3l slib::sla::SMatrixND<3, sinteger>
#define mat3f slib::sla::SMatrixND<3, float>
#define mat3d slib::sla::SMatrixND<3, double>
#define mat3n slib::sla::SMatrixND<3, sfrac>
#define mat3x slib::sla::SMatrixND<3, scomp>
#define mat4i slib::sla::SMatrixND<4, sint>
#define mat4l slib::sla::SMatrixND<4, sinteger>
#define mat4f slib::sla::SMatrixND<4, float>
#define mat4d slib::sla::SMatrixND<4, double>
#define mat4n slib::sla::SMatrixND<4, sfrac>
#define mat4x slib::sla::SMatrixND<4, scomp>

#define svec slib::sla::SVector
#define smat slib::sla::SMatrix

#define svecb slib::sla::SVector<sbyte, CMemory<sbyte>>
#define svecub slib::sla::SVector<subyte, CMemory<subyte>>
#define sveci slib::sla::SVector<sint, CMemory<sint>>
#define svecu slib::sla::SVector<suint, CMemory<suint>>
#define svecl slib::sla::SVector<sinteger, CMemory<sinteger>>
#define svecs slib::sla::SVector<suinteger, CMemory<suinteger>>
#define svecf slib::sla::SVector<float, CMemory<float>>
#define svecd slib::sla::SVector<double, CMemory<double>>
#define svecc slib::sla::SVector<scomp, RMemory<scomp>>

#define v2ivec slib::sla::SVector<v2i, RMemory<v2i>>
#define v2fvec slib::sla::SVector<v2f, RMemory<v2f>>
#define v2dvec slib::sla::SVector<v2d, RMemory<v2d>>
#define v2cvec slib::sla::SVector<v2x, RMemory<v2x>>
#define v3ivec slib::sla::SVector<v3i, RMemory<v3i>>
#define v3fvec slib::sla::SVector<v3f, RMemory<v3f>>
#define v3dvec slib::sla::SVector<v3d, RMemory<v3d>>
#define v3cvec slib::sla::SVector<v3x, RMemory<v3x>>
#define v4ivec slib::sla::SVector<v4i, RMemory<v4i>>
#define v4fvec slib::sla::SVector<v4f, RMemory<v4f>>
#define v4dvec slib::sla::SVector<v4d, RMemory<v4d>>
#define v4cvec slib::sla::SVector<v4x, RMemory<v4x>>

#define svivec slib::sla::SVector<sveci, SMemory<sveci>>
#define svfvec slib::sla::SVector<svecf, SMemory<svecf>>
#define svdvec slib::sla::SVector<svecd, SMemory<svecd>>
#define svcvec slib::sla::SVector<svecc, SMemory<svecc>>

#define smatb slib::sla::SMatrix<sbyte, slib::CMemory<sbyte>>
#define smatub slib::sla::SMatrix<subyte, slib::CMemory<subyte>>
#define smati slib::sla::SMatrix<int, slib::CMemory<int>>
#define smatl slib::sla::SMatrix<sinteger, slib::CMemory<sinteger>>
#define smatf slib::sla::SMatrix<float, slib::CMemory<float>>
#define smatd slib::sla::SMatrix<double, slib::CMemory<double>>
#define smatn slib::sla::SMatrix<sfrac, slib::RMemory<sfrac>>
#define smatx slib::sla::SMatrix<scomp, slib::RMemory<scomp>>

#define v2imat slib::sla::SMatrix<v2i, slib::RMemory<v2i>>
#define v2fmat slib::sla::SMatrix<v2f, slib::RMemory<v2f>>
#define v2dmat slib::sla::SMatrix<v2d, slib::RMemory<v2d>>
#define v3imat slib::sla::SMatrix<v3i, slib::RMemory<v3i>>
#define v3fmat slib::sla::SMatrix<v3f, slib::RMemory<v3f>>
#define v3dmat slib::sla::SMatrix<v3d, slib::RMemory<v3d>>
#define v4imat slib::sla::SMatrix<v4i, slib::RMemory<v4i>>
#define v4fmat slib::sla::SMatrix<v4f, slib::RMemory<v4f>>
#define v4dmat slib::sla::SMatrix<v4d, slib::RMemory<v4d>>
#define svimat slib::sla::SMatrix<sveci>
#define svfmat slib::sla::SMatrix<svecf>
#define svdmat slib::sla::SMatrix<svecd>
#define svcmat slib::sla::SMatrix<svecc>

#define m2ivec slib::sla::SVector<mat2i, slib::RMemory<mat2i>>
#define m2fvec slib::sla::SVector<mat2f, slib::RMemory<mat2f>>
#define m2dvec slib::sla::SVector<mat2d, slib::RMemory<mat2d>>
#define m3ivec slib::sla::SVector<mat3i, slib::RMemory<mat3i>>
#define m3fvec slib::sla::SVector<mat3f, slib::RMemory<mat3f>>
#define m3dvec slib::sla::SVector<mat3d, slib::RMemory<mat3d>>
#define m4ivec slib::sla::SVector<mat4i, slib::RMemory<mat4i>>
#define m4fvec slib::sla::SVector<mat4f, slib::RMemory<mat4f>>
#define m4dvec slib::sla::SVector<mat4d, slib::RMemory<mat4d>>
#define smivec slib::sla::SVector<smati>
#define smfvec slib::sla::SVector<smatf>
#define smdvec slib::sla::SVector<smatd>

namespace slib {
	namespace sla {
		//Initialization of VectorND
		template<size_t D, typename T, class... Args>
		void _initVecND(T* e, Args... args);
		template<size_t D, typename T>
		void _initVecND(T* e) {}
		template<size_t D, typename T, class Val, class... Remain>
		void _initVecND(T* e, Val v, Remain... r) { 
			*e = v; 
			if(0 < D) _initVecND<D - 1, T>(e + 1, r...);
		}
		//Fixed size vector class 
		template<size_t D, typename T>
		class SVectorND {
		public:
			T elements[D];
		public:
			SVectorND() { auto p = elements; sforin(d, 0, D) { *p = initVal<T>(); ++p; } }
			template<class... Args>
			SVectorND(Args... args) : SVectorND() { _initVecND<D, T>(elements, args...); }
			SVectorND(const SVectorND &vec) :SVectorND() { memcpy(elements, vec.elements, D * sizeof(T)); }
			template<size_t E, typename S, class N>
			SVectorND(const SVectorND<E, S>& vec) : SVectorND() {
				if (D < E) { sforin(i, 0, D) elements[i] = (T)vec[i]; }
				else { sforin(i, 0, E) elements[i] = (T)vec[i]; }
			}
			~SVectorND() {}
			SVectorND& operator = (const SVectorND<D, T>& v) { memcpy(elements, v.elements, D * sizeof(T)); return *this; }
			SVectorND& operator += (const SVectorND<D, T>& v) {
				auto p1 = elements;
				auto p2 = v.elements;
				sforin(d, 0, D) { (*p1) += (*p2); ++p1; ++p2; }
				return *this;
			}
			SVectorND& operator += (const T& val) {
				auto p = elements;
				sforin(d, 0, D) { (*p) += val; ++p; }
				return *this;
			}
			SVectorND& operator -= (const SVectorND<D, T>& v) {
				auto p1 = elements;
				auto p2 = v.elements;
				sforin(d, 0, D) { (*p1) -= (*p2); ++p1; ++p2; }
				return *this;
			}
			SVectorND& operator -= (const T& val) {
				auto p = elements;
				sforin(d, 0, D) { (*p) -= val; ++p; }
				return *this;
			}
			SVectorND& operator *= (const T& val) {
				auto p = elements;
				sforin(d, 0, D) { (*p) *= val; ++p; }
				return *this;
			}
			SVectorND& operator /= (const T& val) {
				auto p = elements;
				sforin(d, 0, D) { (*p) /= val; ++p; }
				return *this;
			}
			SVectorND operator + (const SVectorND<D, T>& v) const {
				SVectorND<D, T> vec;
				auto p1 = elements, p2 = v.elements;
				auto p3 = vec.elements;
				sforin(d, 0, D) { *p3 = (*p1) + (*p2); ++p1; ++p2; ++p3; }
				return vec;
			}
			SVectorND operator - (const SVectorND<D, T>& v) const {
				SVectorND<D, T> vec;
				auto p1 = elements, p2 = v.elements;
				auto p3 = vec.elements;
				sforin(d, 0, D) { *p3 = (*p1) - (*p2); ++p1; ++p2; ++p3; }
				return vec;
			}
			SVectorND<D, T> operator * (const T& val) const {
				SVectorND<D, T> vec;
				auto p1 = elements;
				auto p2 = vec.elements;
				sforin(d, 0, D) { *p2 = (*p1) * val; ++p1; ++p2; }
				return vec;
			}
			SVectorND<D, T> operator / (const T& val) const {
				SVectorND<D, T> vec;
				auto p1 = elements;
				auto p2 = vec.elements;
				sforin(d, 0, D) { *p2 = (*p1) / val; ++p1; ++p2; }
				return vec;
			}
			SVectorND operator-() const {
				SVectorND<D, T> vec;
				auto p1 = elements;
				auto p2 = vec.elements;
				sforin(d, 0, D) { *p2 = -(*p1); ++p1; ++p2; }
			}
			T operator * (const SVectorND<D, T>& v) const {
				T dot = initVal<T>();
				auto p1 = elements, p2 = v.elements;
				sforin(d, 0, D) { dot += (*p1) * (*p2); ++p1; ++p2; }
				return dot;
			}
			T& x() { if (0 < D) return elements[0]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			const T& x() const { if (0 < D) return elements[0]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			T& y() { if (1 < D)return elements[1]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			const T& y() const { if (1 < D)return elements[1]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			T& z() { if (2 < D)return elements[2]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			const T& z() const { if (2 < D)return elements[2]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			T& w() { if (3 < D)return elements[3]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			const T& w() const { if (3 < D)return elements[3]; else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR); }
			size_t dimension() const { return D; }
			size_t size() const { return D; }
			void reset(const T& val = initVal<T>()) { sforin(i, 0, D) elements[i] = val; }
			T& operator[](int i) { return elements[i]; }
			const T& operator[](int i) const { return elements[i]; }
			bool operator<(const SVectorND& v) const { return memcmp(elements, v.elements, D * sizeof(T)) < 0; }
			bool operator==(const SVectorND& v) const { return memcmp(elements, v.elements, D * sizeof(T)) == 0; }
		};
		//Euclid norm 
		template<size_t D, typename T>
		extern double length(const SVectorND<D, T>& v) {
			double len = 0;
			auto p = v.elements;
			sforin(d, 0, D) { len += ((*p) * (*p)); ++p; }
			return sqrt(len);
		}
		//Lp-norm
		template<size_t D, typename T>
		extern double lpnorm(const SVectorND<D, T>& v) {
			double len = 0.0; 
			sforin(d, 0, D) { len += pow(abs(v[d]), v.dimension()); }
			return smath::rootN(len, v.dimension());
		}
		//Normalize
		template<size_t D, typename T>
		extern void normalize(SVectorND<D, T>& v) {
			auto len = sla::length(v);
			if (len == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			v /= len;
		}
		template<size_t D, typename T>
		extern SVectorND<D, T> log(const SVectorND<D, T>& v, double base) {
			SVectorND<D, T> vec(v);
			if (base == smath::E) { sforin(d, 0, D) { vec[d] = log(vec[d]); } }
			else { auto b = std::log(base); sforin(d, 0, D) { vec[d] = log(vec[d]) / b; } }
			return vec;
		}
		//Vector product
		template<typename T>
		extern T cross(const SVectorND<2, T> &v1, const SVectorND<2, T>& v2) { return v1.x() * v2.y() - v1.y() * v2.x(); }
		template<typename T>
		extern SVectorND<2, T> orthogonal(const SVectorND<2, T> &v, smath::ROTATION rot) { 
			return rot == smath::CW ? SVectorND<2, T>(v.y(), -v.x()) : SVectorND<2, T>(-v.y(), v.x()); 
		}
		template<typename T>
		extern double slope(const SVectorND<2, T>& v) {
			if (v.x() == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return (double)v.y() / v.x();
		}
		template<typename T>
		extern double argument(const SVectorND<2, T>& v) {
			auto len = sla::length(v);
			if (len == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return v.y() < 0 ? 2 * smath::PI - acos((double)v.x() / len) : acos((double)v.x() / len);
		}
		template<typename T>
		extern SVectorND<2, T> projection(const SVectorND<3, T>& v, smath::PLANE p) { 
			switch (p) {
			case smath::XY_PLANE:
				return SVectorND<2, T>(v.x(), v.y());
			case smath::YZ_PLANE:
				return SVectorND<2, T>(v.y(), v.z());
			default:
				return SVectorND<2, T>(v.x(), v.z());
			}
		}
		template<typename T>
		extern double azimuth(const SVectorND<3, T>& v) { return sla::argument(sla::projection(v, smath::XY_PLANE)); }
		template<typename T>
		extern double altitude(const SVectorND<3, T>& v) { return (v.z() < 0.0 ? -1.0 : 1.0) * acos(sla::length(sla::projection(v, smath::XY_PLANE)) / sla::length(v)); }
		template<typename T>
		extern SVectorND<3, T> cross(SVectorND<3, T>& v1, const SVectorND<3, T>& v2) {
			return SVectorND<3, T>(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
		}
		template<size_t D, typename T>
		extern SVectorND<D, T> operator*(const T& val, const SVectorND<D, T>& v) { return v * val; }
		template <typename T, class M = SMemory<T>>
		struct SVector : public Array<T, M> {
			typedef Array<T, M> array;

			SVector() : array() {}
			SVector(size_t s, const T& val = initVal<T>()) : array(s, val) {}
			SVector(std::initializer_list<T> li) : array(li) {}
			SVector(const SVector<T, M>& v) : array(v) {}
			~SVector() {}
			SVector& operator = (const SVector<T, M>& v) {
				if (v.size()) array::copy(v.ptr(), v.size());
				else array::clear();
				return (*this);
			}
			SVector& operator += (const SVector<T, M>& v) {
				if (array::size() != v.size())
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				sforeach2(*this, v) E1_ += E2_;
				return *this;
			}
			SVector& operator += (const T& val) { sforeach(*this) { E_ += val; } return *this; }
			SVector& operator -= (const SVector<T, M>& v) {
				if (array::size() != v.size())
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
				sforeach2(*this, v) E1_ -= E2_;
				return *this;
			}
			SVector& operator -= (const T& val) { sforeach(*this) { E_ -= val; } return *this; }
			SVector& operator *= (const T& val) { sforeach(*this) { E_ *= val; } return *this; }
			SVector& operator /= (const T& val) { sforeach(*this) { E_ /= val; } return *this; }
			SVector operator + (const SVector<T, M>& v) const { SVector<T, M> v_(*this); v_ += v; return v_; }
			SVector operator - (const SVector<T, M>& v) const { SVector<T, M> v_(*this); v_ -= v; return v_; }
			SVector operator - () const {
				SVector v(*this);
				sforeach(v) E_ = -E_;
				return v;
			}
			SVector operator * (const T& val) const { SVector<T, M> v(*this); v *= val; return v; }
			SVector operator / (const T& val) const { SVector<T, M> v(*this); v /= val; return v; }
			T operator *(const SVector<T, M>& v) const {
				if (array::size() != v.size() || array::empty())
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				T prod = initVal<T>();
				sforeach(*this) prod += E_ * E_;
				return prod;
			}
			scyc_iter<T> cycle(int i = 0) { return scyc_iter<T>(array::ptr(i), i, array::size()); }
			scyc_citer<T> cycle(int i = 0) const { return scyc_citer<T>(array::ptr(i), i, array::size()); }
		};
		template <typename T, class M>
		extern inline SVector<T, M> operator*(const T& val, const SVector<T, M>& v) { return v * val; }
		template <typename T, class M>
		extern inline double length(const SVector<T, M>& vec) {
			double len = 0.0; 
			sforeach(vec) { len += (double)(E_ * E_); } 
			return sqrt(len);
		}
		template <typename T, class M>
		extern inline double lpnorm(const SVector<T, M>& vec) {
			double len = 0.0;
			sforeach(vec) { len += pow(abs(E_), vec.size()); } 
			return smath::rootN(len, vec.size());
		}
		template <typename T, class M>
		extern inline void normalize(SVector<T, M>& vec) {
			auto len = sla::length(vec);
			if (len) vec /= len;
			else throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
		}
		template <typename T, class M>
		extern SVector<T, M> log(const SVector<T, M>& v, double base) {
			SVector<T> vec(v.size());
			if (base == smath::E) { sforeach2(vec, v) { E1_ = log(E2_); } }
			else { auto b = std::log(base); sforeach2(vec, v) { E1_ = log(E2_) / b; } }
			return vec;
		}
		template<typename T>
		extern inline SVector<T, CMemory<T>> arrange(T init, T end, T diff = 1) {
			SVector<T, CMemory<T>> vec((end - init + 1) / diff);
			sforeach(vec) { E_ = init; init += diff; }
			return vec;
		}
		template<typename T>
		extern inline SVector<T, CMemory<T>> arithmetric(T init, T diff, sinteger num) {
			SVector<T, CMemory<T>> vec(num);
			sforeach(vec) { E_ = init; init += diff; }
			return vec;
		}
		template<typename T>
		extern inline SVector<T, CMemory<T>> geometric(T init, T ratio, sinteger num) {
			SVector<T, CMemory<T>> vec(num);
			sforeach(vec) { E_ = init; init *= ratio; }
			return vec;
		}
		template <typename T, class M>
		extern inline SVector<T, M> repeat(SVector<T, M>& vec, const sveci &count) {
			SVector<T, M> rep;
			auto cit = count.cycle();
			sforeach(vec) {
				sforin(i, 0, *cit) rep.add(E_);
				++cit;
			}
			return rep;
		}
		template <typename T, class M>
		extern inline SVector<T, M> repeat(SVector<T, M>& vec, size_t num) {
			SVector<T, M> rep;
			sforin(i, 0, num) rep.append(vec);
			return rep;
		}
		template <typename T, class M>
		extern inline svecd toReal(const SVector<T, M> &v) {
			svecd vd(v.size());
			sforeach2(v, vd) E2_ = (double)E1_;
			return vd;
		}
		
		template<size_t D, typename T>
		class SMatrixND {
		public:
			T elements[D * D];

			SMatrixND() { 
				if (D < 2) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
				auto p = elements; sforin(d, 0, D * D) { *p = initVal<T>(); ++p; } 
			}
			template<class... Args>
			SMatrixND(Args... args) { 
				if (D < 2) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
				_initVecND<D * D, T>(elements, args...);
			}
			SMatrixND(const SMatrixND<D, T>& m) { 
				if (D < 2) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
				memcpy(elements, m.elements, D * D * sizeof(T)); 
			}
			~SMatrixND() {}
			static SMatrixND unitMat() { 
				return SMatrixND<D, T>(); 
			
			
			}
			SMatrixND& operator=(const SMatrixND<D, T>& m) { memcpy(elements, m.elements, D * D * sizeof(T)); return *this; }
			SMatrixND& operator += (const SMatrixND<D, T>& m) {
				auto p1 = elements; auto p2 = m.elements;
				sforin(i, 0, D * D) { (*p1) += (*p2); ++p1; ++p2; }
				return *this;
			}
			SMatrixND& operator -= (const SMatrixND<D, T>& m) {
				auto p1 = elements; auto p2 = m.elements;
				sforin(i, 0, D * D) { (*p1) -= (*p2); ++p1; ++p2; }
				return *this;
			}
			SMatrixND& operator *= (const T& val) {
				auto p = elements;
				sforin(i, 0, D * D) { (*p) *= val; ++p; }
				return *this;
			}
			SMatrixND& operator *= (const SMatrixND<D, T>& m) {
				(*this) = (*this) * m;
				return *this;
			}
			SMatrixND& operator /= (const T& val) {
				auto p = elements;
				sforin(i, 0, D * D) { (*p) /= val; ++p; }
				return *this;
			}
			SMatrixND<D, T> operator + (const SMatrixND<D, T>& m) const {
				SMatrixND<D, T> mat;
				auto p1 = elements; auto p2 = m.elements;
				auto p3 = mat.elements;
				sforin(i, 0, D * D) { (*p3) = (*p1) + (*p2); ++p1; ++p2; ++p3; }
				return mat;
			}
			SMatrixND<D, T> operator - (const SMatrixND<D, T>& m) const {
				SMatrixND<D, T> mat;
				auto p1 = elements; auto p2 = m.elements;
				auto p3 = mat.elements;
				sforin(i, 0, D * D) { (*p3) = (*p1) - (*p2); ++p1; ++p2; ++p3; }
				return mat;
			}
			SMatrixND<D, T> operator - () {
				SMatrixND<D, T> mat;
				auto p1 = elements; auto p2 = mat.elements;
				sforin(i, 0, D * D) { (*p2) = -(*p1); ++p1; ++p2; }
				return mat;
			}
			SVectorND<D, T> operator * (const SVectorND<D, T>& v) const {
				SVectorND<D, T> vec;
				auto p1 = elements; auto p2 = v.elements;
				auto p3 = vec.elements;
				sforin(r, 0, D) {
					p2 = v.elements;
					sforin(c, 0, D) { (*p3) += (*p1) * (*p2); ++p1; ++p2; }
					++p3;
				}
				return vec;
			}
			SMatrixND<D, T> operator * (const SMatrixND<D, T>& m) const {
				SMatrixND<D, T> mat;
				auto p1 = elements, p2 = m.elements;
				auto p3 = mat.elements;
				sforin(r, 0, D) {
					sforin(c, 0, D) {
						p2 = &m.elements[c];
						sforin(r_, 0, D) { (*p3) += p1[r_] * (*p2); p2 += D; }
						++p3;
					}
					p1 += D;
				}
				return mat;
			}
			SMatrixND<D, T> operator * (const T& val) const {
				SMatrixND<D, T> mat;
				auto p1 = elements; auto p2 = mat.elements;
				sforin(i, 0, D * D) { (*p2) = (*p1) * val; ++p1; ++p2; }
				return mat;
			}
			SMatrixND<D, T> operator / (const T& val) const {
				SMatrixND<D, T> mat;
				auto p1 = elements; auto p2 = mat.elements;
				sforin(i, 0, D * D) { (*p2) = (*p1) / val; ++p1; ++p2; }
				return mat;
			}
			SMatrixND<D, T> transpose() const {
				SMatrixND<D, T> mat;
				sforin(i, 0, D * D) {
					mat.elements[i] = elements[(i % D) * D + (i / D)];
				}
				return mat;
			}
			SMatrixND<D, T> inverse() const {
				auto det = determinant();
				if (det == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
				if (D == 2) return SMatrixND<D, T>(elements[3], -elements[1], -elements[2], elements[0]) / det;
				else {
					SMatrixND<D, T> inv;
					auto p = inv.elements;
					sforin(r, 0, D) {
						sforin(c, 0, D) { *p = ((r + c) % 2 ? -1 : 1) * cofactor(r, c).determinant(); ++p; }
					}
					return inv.transpose() / det;
				}
			}
			SMatrixND<D - 1, T> cofactor(sint r, sint c) const {
				SMatrixND<D - 1, T> mat;
				auto p1 = elements; 
				auto p2 = mat.elements;
				sforin(r_, 0, r) {
					sforin(c_, 0, c) { (*p2) = (*p1); ++p1; ++p2; }
					++p1;
					sforin(c_, c+ 1, D) { (*p2) = (*p1); ++p1; ++p2; }
				}
				p1 += D;
				sforin(r_, r + 1, D) {
					sforin(c_, 0, c) { (*p2) = (*p1); ++p1; ++p2; }
					++p1;
					sforin(c_, c + 1, D) { (*p2) = (*p1); ++p1; ++p2; }
				}
				return mat;
			}
			T determinant() const {
				T det = initVal<T>();
				if (D == 2) return elements[0] * elements[3] - elements[1] * elements[2];
				else if (D == 3) return elements[0] * elements[4] * elements[8] + elements[1] * elements[5] * elements[6] + elements[2] * elements[3] * elements[7] 
					- elements[2] * elements[4] * elements[6] - elements[1] * elements[3] * elements[8] - elements[0] * elements[5] * elements[7];
				else {
					auto p = elements;
					sforin(i, 0, D) { det += ((i % 2 ? -1 : 1) * (*p) * cofactor(i, 0).determinant()); p += D; }
				}
				return det;
			}
			T* operator[](int idx) { return &elements[idx * D]; }
			const T* operator[](int idx) const { return &elements[idx * D]; }
			bool operator == (const SMatrixND<D, T>& m) const {
				return memcmp(elements, m.elements) == 0;
			}
			bool operator != (const SMatrixND<D, T>& m) const { return !((*this) == m); }
		};
		template <typename T>
		class SMatrixND<0, T> {
		public:
			T elements[1];
			T determinant() const { return initVal<T>(); }
		};
		template <typename T>
		class SMatrixND<1, T> {
		public:
			T elements[1];
			T determinant() const { return initVal<T>(); }
		};
		template <size_t D, typename T>
		extern inline SVectorND<D, T> operator*(const SVectorND<D, T>& v, const SMatrixND<D, T>& m) { 
			SVectorND<D, T> vec;
			auto p1 = v.elements; auto p2 = m.elements;
			auto p3 = vec.elements;
			sforin(c, 0, D) {
				p1 = v.elements;
				p2 = &m.elements[c];
				sforin(r, 0, D) { (*p3) += (*p1) * (*p2); ++p1; p2 += D; }
				++p3;
			}
			return vec;
		}
		
		template<typename T, class M = SMemory<T>>
		class SMatrix : public SVector<T, M> {
			typedef SVector<T, M> vector;
			inline size_t _index(int r, int c) const { return col * r + c; }
		public:
			size_t row, col;
		public:
			SMatrix() : row(0), col(0), vector() {}
			SMatrix(size_t r, size_t c) : row(r), col(c), vector(r * c) {}
			SMatrix(size_t r, size_t c, const T& val) : row(r), col(c), vector(r * c, val) {}
			SMatrix(size_t r, size_t c, std::initializer_list<T> li) : row(r), col(c), vector(li) {}
			SMatrix(SMatrix &&mat) : row(mat.row), col(mat.col), vector(std::forward<vector &&>(mat)) {}
			SMatrix(const SMatrix& mat) : SMatrix<T, M>(mat.row, mat.col) { vector::copy(mat.ptr(), mat.size()); }
			~SMatrix() {}
			SMatrix& operator=(const SMatrix& mat) {
				vector::clear();
				row = mat.row; col = mat.col;
				vector::copy(mat.ptr(), mat.size());
				return *this;
			}
			SMatrix& operator += (const SMatrix& m) {
				if (!comparable(m))
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				sforeach2(*this, m) E1_ += E2_;
				return *this;
			}
			SMatrix& operator -= (const SMatrix& m) {
				if (!comparable(m))
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				sforeach2(*this, m) E1_ -= E2_;
				return *this;
			}
			SMatrix& operator *= (const T& val) {
				sforeach(*this) E_ *= val;
				return *this;
			}
			SMatrix& operator /= (const T& val) {
				sforeach(*this) E_ /= val;
				return *this;
			}
			SMatrix operator + (const SMatrix& m) { SMatrix<T, M> mat(*this); mat += m; return mat; }
			SMatrix operator - (const SMatrix& m) { SMatrix<T, M> mat(*this); mat -= m; return mat; }
			SMatrix operator - () {
				SMatrix<T, M> mat(*this);
				sforeach(mat) E_ = -E_;
				return mat;
			}
			SVector<T, M> operator * (const SVector<T, M>& v) {
				if (col != v.size())
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				SVector<T, M> vec(row, 0);
				auto p1 = vector::ptr(); auto p2 = v.ptr(); auto p3 = vec.ptr();
				sforin(r, 0, row) {
					p2 = v.ptr();
					sforin(c, 0, col) { (*p3) += (*p1) * (*p2); ++p1; ++p2; }
					++p3;
				}
				return vec;
			}
			SMatrix<T, M> operator * (const SMatrix& m) {
				if (col != m.row)
					throw slib::smath::SMathException(ERR_INFO, slib::smath::DIMENSION_SIZE_ERR);
				SMatrix<T, M> mat(row, m.col, 0);
				auto p1 = vector::ptr(), p1_ = p1; auto p2 = m.ptr(); auto p3 = mat.ptr();
				sforin(r, 0, row) {
					sforin(c, 0, col) {
						p1_ = p1;
						p2 = &m[0][c];
						sforin(r_, 0, row) { (*p3) += (*p1_) * (*p2); ++p1_; p2 += m.col; }
						++p3;
					}
					p1 += row;
				}
				return mat;
			}
			SMatrix operator * (const T& val) { SMatrix<T, M> m(*this); m *= val; return m; }
			SMatrix operator / (const T& val) { SMatrix<T, M> m(*this); m /= val; return m; }
			bool comparable(const SMatrix& m) { return col == m.col && row == m.row; }
			void resize(size_t r, size_t c) {
				SMatrix<T, M> tmp(r, c);
				sforin(i, 0, row < r ? row : r) {
					sforin(j, 0, col < c ? col : c) tmp[i][j] = at(i, j);
				}
				this->swap(tmp);
			}
			void swap(const SMatrix& mat) {
				row = mat.row; col = mat.col;
				

				
				//vector::swap(mat);
			}
			T* operator[](size_t row) { return vector::ptr(_index(row, 0)); }
			const T* operator[](size_t row) const { return vector::ptr(_index(row, 0)); }
			T& at(size_t row, size_t col) { return vector::at(_index(row, col)); }
			const T& at(size_t row, size_t col) const { return vector::at(_index(row, col)); }
			SMatrix submat(size_t r, size_t c, size_t rn, size_t cn) const {
				SMatrix<T, M> mat(rn, cn);
				auto p1 = vector::ptr(_index(r, c)), *p1_;
				auto p2 = mat.ptr();
				sforin(r, 0, rn) { 
					p1_ = p1;
					sforin(c, 0, cn) { *p2 = *p1_; ++p1_; ++p2; }
					p1 += row;
				}
				return mat;
			}
			T determinant() const {
				if (row != col) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
				SMatrix<T, M> mat(*this);
				T det = 1, tmp;
				sforin(i, 0, row - 1) {
					if (mat[i][i] == 0) {
						auto i_ = i + 1;
						while (i_ < row) {
							if (mat[i_][i] != 0) break;
							++i_;
						}
						if (i_ < row) {
							mat.swap(i, i_, smath::VERTICAL);
							det *= -1;
						}
					}
					sforin(j, i + 1, row) {
						tmp = mat[j][i] / mat[i][i];
						sforin(k, i, col) mat[j][k] -= tmp * mat[i][k];
					}
					det *= mat[i][i];
				}
				det *= mat[row - 1][row - 1];
				return det;
			}
			SMatrix inverse() const {
				SMatrix<T, M> ori = *this, mat(row, col, initVal<T>());
				sforin(i, 0, row) mat[i][i] = unitVal<T>();
				T val;
				double tmp = 0;
				sforin(r, 0, row) {
					val = ori[r][r];
					if (val == 0) {
						sforin(r_, r + 1, row) {
							if (r == r_) continue;
							val = ori[r_][r];
							if (val != 0) {
								ori.swap(r, r_, smath::ROW);
								mat.swap(r, r_, smath::ROW);
								break;
							}
						}
					}
					if (val == 0) throw slib::smath::SMathException(ERR_INFO, slib::smath::DIV_ZERO_ERR);
					tmp = 1.0 / val;
					sforin(c, 0, col) {
						ori[r][c] *= tmp; mat[r][c] *= tmp;
					}
					sforin(r_, 0, row) {
						if (r == r_) continue;
						val = ori[r_][r];
						ori.sweep(r_, r, val, smath::ROW);
						mat.sweep(r_, r, val, smath::ROW);
					}
				}
				return mat;
			}
			SMatrix transpose() const {
				SMatrix<T, M> m(col, row);
				sforin(r, 0, row) { sforin(c, 0, col) m[c][r] = at(r, c); }
				return m;
			}
			SMatrix cofactor(size_t r, size_t c) const {
				SMatrix<T, M> mat(row - 1, col - 1);
				auto p1 = this->ptr(); auto p2 = mat.ptr();
				sforin(r_, 0, row) {
					if (r_ == r) { p1 += col; continue; }
					sforin(c_, 0, col) {
						if (c_ == c) { ++p1; continue; }
						*p1 = *p2; ++p1; ++p2;
					}
				}
				return mat;
			}

			void rsum(const size_t& idx1, const size_t& idx2) {
				auto p1 = vector::ptr(idx1 * col), p2 = vector::ptr(idx2 * col);
				sforin(c, 0, col) { (*p1) += (*p2); ++p1; ++p2; }
			}
			void csum(const size_t& idx1, const size_t& idx2) {
				auto p1 = vector::ptr(idx1), p2 = vector::ptr(idx2);
				sforin(r, 0, row) { (*p1) += (*p2); p1 += col; p2 += col; }
			}
			void sum(const size_t& idx1, const size_t& idx2, smath::DIRECTION dir) {
				if (dir == smath::COLUMN) csum(idx1, idx2);
				else rsum(idx1, idx2);
			}
			void rsubtract(const size_t& idx1, const size_t& idx2) {
				auto p1 = vector::ptr(idx1 * col), p2 = vector::ptr(idx2 * col);
				sforin(c, 0, col) { (*p1) -= (*p2); ++p1; ++p2; }
			}
			void csubtract(const size_t& idx1, const size_t& idx2) {
				auto p1 = vector::ptr(idx1), p2 = vector::ptr(idx2);
				sforin(r, 0, row) { (*p1) -= (*p2); p1 += col; p2 += col; }
			}
			void subtract(const size_t& idx1, const size_t& idx2, smath::DIRECTION dir) {
				if (dir == smath::COLUMN) csubtract(idx1, idx2);
				else rsubtract(idx1, idx2);
			}
			void rtimes(const size_t& idx, const T& ratio) {
				auto p = vector::ptr(idx * col);
				sforin(c, 0, col) { (*p) *= ratio; ++p; }
			}
			void ctimes(const size_t& idx, const T& ratio) {
				auto p = vector::ptr(idx);
				sforin(r, 0, row) { (*p) *= ratio; p += col; }
			}
			void times(const size_t& idx, const T& ratio, smath::DIRECTION dir) {
				if (dir == smath::COLUMN) ctime(idx, ratio);
				else rtime(idx, ratio);
			}
			void rswap(const size_t& idx1, const size_t& idx2) {
				if (idx1 == idx2) return;
				T tmp, *p1 = vector::ptr(idx1 * col), * p2 = vector::ptr(idx2 * col);
				sforin(c, 0, col) {
					tmp = *p1; *p1 = *p2; *p2 = tmp;
					++p1; ++p2;
				}
			}
			void cswap(const size_t& idx1, const size_t& idx2) {
				if (idx1 == idx2) return;
				T tmp, *p1 = vector::ptr(idx1), *p2 = vector::ptr(idx2);
				sforin(r, 0, row) {
					tmp = *p1; *p1 = *p2; *p2 = tmp;
					p1 += col; p2 += col;
				}
			}
			void swap(const size_t& idx1, const size_t& idx2, smath::DIRECTION dir) {
				if (dir == smath::COLUMN) cswap(idx1, idx2);
				else rswap(idx1, idx2);
			}
			void rsweep(const size_t& idx1, const size_t& idx2, const T& ratio) {
				auto p1 = vector::ptr(idx1 * col), p2 = vector::ptr(idx2 * col);
				sforin(c, 0, col) { (*p1) -= ratio * (*p2); ++p1; ++p2; }
			}
			void csweep(const size_t& idx1, const size_t& idx2, const T& ratio) {
				auto p1 = vector::ptr(idx1), p2 = vector::ptr(idx2);
				sforin(r, 0, row) { (*p1) -= ratio * (*p2); p1 += col; p2 += col; }
			}
			void sweep(const size_t& idx1, const size_t& idx2, const T& ratio, smath::DIRECTION dir) {
				if (dir == smath::COLUMN) csweep(idx1, idx2);
				else rsweep(idx1, idx2);
			}
			bool operator<(const SMatrix<T, M>& mat) const {
				if (!comparable(mat)) return vector::size() < mat.size();
				auto p = vector::ptr(), p_ = mat.ptr();
				sforeach2(*this, mat) {
					if (E1_ != E2_) return E1_ < E2_;
				}
				return false;
			}
			bool operator==(const SMatrix<T, M>& mat) const {
				if (row != mat.row || col != mat.col) return false;
				sforeach2(*this, mat) {
					if (E1_ != E2_) return false;
				}
				return true;
			}
		};
		template <typename T, class M>
		extern inline SMatrix<T, M> operator*(const T& val, const SMatrix<T, M>& m) {
			SMatrix<T, M> mat(m.row, m.col);
			sforeach2(m, mat) { E2_ = E1_ * val; }
			return mat;
		}
		template <typename T, class M>
		extern inline SVector<T, M> operator*(const SVector<T, M>& v, const SMatrix<T, M>& m) {
			if (v.size() != m.row) throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
			SVector<T, M> vec(m.col, 0);
			const T *p1, p2; auto p3 = vec.ptr();
			sforin(c, 0, m.col) {
				p1 = v.ptr(); p2 = m.ptr(c);
				sforin(r, 0, m.row()) { (+p3) += (*p1) * (*p2); ++p1; p2 += m.col; }
				++p3;
			}
			return vec;
		}
		template <typename T, size_t D>
		extern inline SMatrix<T, CMemory<T>> uniMat() {
			SMatrix<T, CMemory<T>> mat(D, D, 0);
			auto p = mat.ptr();
			sforin(d, 0, D) { *p = 1; p += (D + 1); }
			return mat;
		}
		template<size_t D>
		extern inline smati uniMati() { return uniMat<sint, D>(); }
		template<size_t D>
		extern inline smatl uniMatl() { return uniMat<sinteger, D>(); }
		template<size_t D>
		extern inline smatf uniMatf() { return uniMat<float, D>(); }
		template<size_t D>
		extern inline smatd uniMatd() { return uniMat<double, D>(); }
		template<size_t D>

		extern inline smatd toReal(const smati& mat) {
			smatd matd(mat.row, mat.col);
			sforeach2(mat, matd) E2_ = (double)E1_;
			return matd;
		}
		template <typename T, class M>
		extern inline SVector<SVector<T, M>> toVec(const SMatrix<T, M>& mat) {
			SVector<SVector<T, M>> vec(mat.row);
			auto p1 = vec.ptr(); auto p2 = mat.ptr();
			sforin(r, 0, mat.row) { p1->copy(p2, mat.col); ++p1; p2 += mat.col; }
			return vec;
		}
		
		/*============================================================*/
		
		template<typename T>
		extern inline double solveEq1(const SVectorND<2, T> &coef) {
			if (coef.x() == 0) throw slib::smath::SMathException(ERR_INFO, slib::smath::DIV_ZERO_ERR);
			return -coef.y() / coef.x();
		}
		template<typename T>
		extern inline double discriminant(const SVectorND<3, T> &coef) {
			return pow(coef.y(), 2) - 4.0 * coef.x() * coef.z();
		}
		template<typename T>
		extern inline v2d solveEq2(const SVectorND<3, T>& coef) {
			if (coef.x() == 0) throw slib::smath::SMathException(ERR_INFO, slib::smath::DIV_ZERO_ERR);
			auto d = discriminant(coef);
			if (d < 0) throw slib::smath::SMathException(ERR_INFO);
			d = sqrt(d);
			return v2d((-coef.y() - d) / (2.0 * coef.x()), (-coef.y() + d) / (2.0 * coef.x()));
		}
		template<typename T>
		extern inline SVectorND<2, smath::Complex<double>> solveEq2c(const SVectorND<3, T> &coef) {
			if (coef.x() == 0) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			auto d = discriminant(coef), re = 0.0, im = 0.0;
			if (d < 0) {
				re = -coef.y() / (2.0 * coef.x()); im = sqrt(-d) / (2.0 * coef.x());
				return SVectorND<2, smath::Complex<double>>(smath::Complex<double>(re, -im), smath::Complex<double>(re, im));
			}
			else {
				re = -coef.y() / (2.0 * coef.x()); im = sqrt(d) / (2.0 * coef.x());
				return SVectorND<2, smath::Complex<double>>(smath::Complex<double>(re - im, 0), smath::Complex<double>(re + im, 0));
			}
		}
		template<typename T, class M>
		extern inline void Householder(SMatrix<T, M>& mat, smatd& q, smatd& r) {
			q.resize(mat.row, mat.col);
			r.resize(mat.col, mat.col);





		}
		template<typename T, class M>
		extern inline void GramSchmidt(SMatrix<T, M>& mat, smatd& qmat, smatd& rmat) {
			qmat = mat;
			rmat.resize(mat.col, mat.col, 0);
			sforin(c, 0, mat.col) {
				sforin(c_, 0, c) {
					sforin(r, 0, mat.row) rmat[c_][c] += qmat[r][c_] * qmat[r][c];
					sforin(r, 0, mat.row) qmat[r][c] -= rmat[c_][c] * qmat[r][c_];
				}
				sforin(r, 0, mat.row) rmat[c][c] += smath::power(qmat[r][c], 2);
				rmat[c][c] = sqrt(rmat[c][c]);
				sforin(r, 0, mat.row) qmat[r][c] /= rmat[c][c];
			}
		}
		template<typename T, class M>
		extern inline void QRDecomposition(SMatrix<T, M>& mat, smatd& qmat, smatd& rmat, const char* method = "gs") {
			String alg = String::lower(method);
			if (alg == "hh")  Householder(mat, qmat, rmat);
			else GramSchmidt(mat, qmat, rmat);
		}
		template<typename T, class M>
		extern inline void QRMethod(SMatrix<T, M>& mat, smatd& evec, svecd& evalue) {
			evec = mat;
			evalue.resize(mat.row, 0);
			smatd qmat = uniMat(mat.row);
			v2d ratio;
			sint r_ = mat.row;
			double mu, sub[4], tmp1, tmp2;
			sla::SVectorND<2, smath::Complex<double>> sol;
			while (r_ != 1) {
				if (evec[r_ - 1][r_ - 2] < smath::D_EPS) { --r_; continue; }
				sub[0] = evec[r_ - 2][r_ - 2];
				sub[1] = evec[r_ - 2][r_ - 1];
				sub[2] = evec[r_ - 1][r_ - 2];
				sub[3] = evec[r_ - 1][r_ - 1];
				sol = solveEq2c(v3d(1, sub[0] + sub[3], sub[0] * sub[3] - sub[1] * sub[2]));
				if (abs(sub[3] - sol[0].real) < abs(sub[3] - sol[1].real))
					mu = sub[3] - sol[0].real;
				else  mu = sub[3] - sol[1].real;
				sforeach(evec) E_ -= mu;
				sforin(r, 0, mat.row - 1) {
					tmp1 = sqrt(smath::power(evec[r][r], 2) + smath::power(evec[r + 1][r], 2));
					if (tmp1 < smath::D_EPS) ratio = v2d(0.0, 0.0);
					else ratio = v2d(evec[r + 1][r] / tmp1, evec[r][r] / tmp1);
					sforin(c, r + 1, mat.row) {
						tmp2 = evec[r][c] * ratio.y + evec[r + 1][c] * ratio.x;
						evec[r + 1][c] = -evec[r][c] * ratio.x + evec[r + 1][c] * ratio.y;
						evec[r][c] = tmp2;
					}
					evec[r + 1][r] = 0.0;
					evec[r][r] = tmp1;
					sforin(c, 0, mat.col) {
						tmp2 = qmat[c][r] * ratio.y + qmat[c][r] * ratio.x;
						qmat[c][r + 1] = qmat[c][r] * ratio.x + qmat[c][r + 1] * ratio.y;
						qmat[c][r] = tmp2;
					}

				}
				sforin(r, 0, mat.row) {
					sforin(c, r, mat.col) evalue[c] = evec[r][c];
					sforin(c, 0, mat.col) {
						tmp1 = 0.0;
						sforin(r_, r, mat.row) tmp1 += evalue[r_] * qmat[r_][c];
						evec[r][c] = tmp1;
					}
				}
				sforeach(evec) E_ += mu;
			}
			sforeachi(evalue) evalue[i] = evec[i][i];




		}
		template<typename T, class M>
		extern inline void eigen(SMatrix<T, M>& mat, smatd& evec, svecd& evalue, const char* method = "qr") {
			String alg = String::lower(method);
			if (alg == "dc") {}
			else QRMethod(mat, evec, evalue);
		}
		template<typename T, class M>
		extern inline SVector<T, M> solve(SMatrix<T, M>& mat, SVector<T, M>& val) {
			SVector<T, M> sol(val.size(), 0);
			smatd qmat, rmat;
			QRDecomposition(mat, qmat, rmat);
			auto tmp = qmat.transpose() * val;
			sol.last() = tmp.last() / rmat.last();




		}

		template <typename T>
		using svec2d = SVectorND<2, T>;
		template <typename T>
		using svec3d = SVectorND<3, T>;
		template <typename T>
		using svec4d = SVectorND<4, T>;
		template <typename T>
		using smat2d = SMatrixND<2, T>;
		template <typename T>
		using smat3d = SMatrixND<2, T>;
		template <typename T>
		using smat4d = SMatrixND<2, T>;
		

	}
	template<size_t D, typename T>
	extern slib::String toString(const sla::SVectorND<D, T>& v) {
		slib::String str;
		str << "(";
		if (D == 0) str << ")";
		else {
			sforin(d, 0, D - 1) { str << v.elements[d] << ","; }
			str << v.elements[D - 1] << ")";
		}
		return str;
	}
	template<size_t D, typename T>
	extern std::ostream& operator<<(std::ostream& os, const sla::SVectorND<D, T>& v) { return os << slib::toString(v); }
	
	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const sla::SVector<T, M>& vec) {
		sforeach(vec) {
			os << E_;
			if (it < vec.end() - 1) os << ",";
		}
		return os;
	}

	template<typename T, class M>
	extern inline std::ostream& operator<<(std::ostream& os, const sla::SMatrix<T, M>& m) {
		auto maxlen = 0;
		sforeach(m) {
			if (maxlen < String(E_).size()) maxlen = String(E_).size();
		}
		sforin(r, 0, m.row) {
			sforin(c, 0, m.col) {
				os << String(m[r][c]).filled(maxlen + 2, ' ', true).cstr();
			}
			if (r < m.row - 1) os << NEW_LINE.cstr();
		}
		return os;
	}



	template<size_t D, typename T>
	extern slib::String toString(const sla::SMatrixND<D, T>& m) {
		slib::String str;
		auto maxlen = 0;
		sforin(d, 0, D * D) {
			if (maxlen < slib::String(m.elements[d]).size()) maxlen = slib::String(m.elements[d]).size();
		}
		str << String({ (char)0xE2, (char)0x94, (char)0x8C }) << SPACE * (maxlen + 2) * D << String({ (char)0xE2, (char)0x94, (char)0x90 }) << NEW_LINE.cstr();
		sforin(r, 0, D) {
			str << String({ (char)0xE2, (char)0x94, (char)0x82 });
			sforin(c, 0, D) str << slib::String(m.elements[D * r + c]).filled(maxlen + 2, ' ', String::BOTH_PART);
			str << String({ (char)0xE2, (char)0x94, (char)0x82 }) << NEW_LINE.cstr();
		}
		str << String({ (char)0xE2, (char)0x94, (char)0x94 }) << SPACE * (maxlen + 2) * D << String({ (char)0xE2, (char)0x94, (char)0x98 });
		return str;
	}
	template<size_t D, typename T>
	extern inline std::ostream& operator<<(std::ostream& os, const sla::SMatrixND<D, T>& m) { return os << slib::toString(m); }
}


#endif