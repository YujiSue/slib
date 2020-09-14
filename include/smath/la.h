#ifndef SMATH_LA_H
#define SMATH_LA_H

#include "sconfig.h"
#include "smath/mathbasic.h"
#include "smath/comp.h"
#include "sbasic/array.h"
#include "sbasic/cyciter.h"
#include "sbasic/string.h"

#define svec2d slib::sla::SVector2D
#define svec3d slib::sla::SVector3D
#define svec4d slib::sla::SVector4D
#define svec slib::sla::SVector

#define v2i slib::sla::SVector2D<sint>
#define v2f slib::sla::SVector2D<float>
#define v2d slib::sla::SVector2D<double>
#define v2c slib::sla::SVector2D<scomp>
#define v3i slib::sla::SVector3D<sint>
#define v3f slib::sla::SVector3D<float>
#define v3d slib::sla::SVector3D<double>
#define v3c slib::sla::SVector3D<scomp>
#define v4i slib::sla::SVector4D<sint>
#define v4f slib::sla::SVector4D<float>
#define v4d slib::sla::SVector4D<double>
#define v4c slib::sla::SVector4D<scomp>
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
#define v2cvec slib::sla::SVector<v2c, RMemory<v2c>>
#define v3ivec slib::sla::SVector<v3i, RMemory<v3i>>
#define v3fvec slib::sla::SVector<v3f, RMemory<v3f>>
#define v3dvec slib::sla::SVector<v3d, RMemory<v3d>>
#define v3cvec slib::sla::SVector<v3c, RMemory<v3c>>
#define v4ivec slib::sla::SVector<v4i, RMemory<v4i>>
#define v4fvec slib::sla::SVector<v4f, RMemory<v4f>>
#define v4dvec slib::sla::SVector<v4d, RMemory<v4d>>
#define v4cvec slib::sla::SVector<v4c, RMemory<v4c>>
#define svivec slib::sla::SVector<sveci, SMemory<sveci>>
#define svfvec slib::sla::SVector<svecf, SMemory<svecf>>
#define svdvec slib::sla::SVector<svecd, SMemory<svecd>>
#define svcvec slib::sla::SVector<svecc, SMemory<svecc>>

#define smat2d slib::sla::SMatrix2D
#define smat3d slib::sla::SMatrix3D
#define smat4d slib::sla::SMatrix4D
#define smat slib::sla::SMatrix

#define smatb slib::sla::SMatrix<sbyte, slib::CMemory<sbyte>>
#define smatub slib::sla::SMatrix<subyte, slib::CMemory<subyte>>
#define smati slib::sla::SMatrix<int, slib::CMemory<int>>
#define smatf slib::sla::SMatrix<float, slib::CMemory<float>>
#define smatd slib::sla::SMatrix<double, slib::CMemory<double>>
#define smatc slib::sla::SMatrix<scomp, slib::RMemory<scomp>>

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

#define mat2i slib::sla::SMatrix2D<int>
#define unimat2i slib::sla::SMatrix2D<int>(1, 0, 0, 1)
#define mat2f slib::sla::SMatrix2D<float>
#define unimat2f slib::sla::SMatrix2D<float>(1.f, 0.f, 0.f, 1.f)
#define mat2d slib::sla::SMatrix2D<double>
#define unimat2d slib::sla::SMatrix2D<double>(1.0, 0.0, 0.0, 1.0)
#define mat3i slib::sla::SMatrix3D<int>
#define unimat3i slib::sla::SMatrix3D<int>(1, 0, 0, 0, 1, 0, 0, 0, 1)
#define mat3f slib::sla::SMatrix3D<float>
#define unimat3f slib::sla::SMatrix3D<float>(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f)
#define mat3d slib::sla::SMatrix3D<double>
#define unimat3d slib::sla::SMatrix3D<double>(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)
#define mat4i slib::sla::SMatrix4D<int>
#define unimat4i slib::sla::SMatrix4D<int>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)
#define mat4f slib::sla::SMatrix4D<float>
#define unimat4f slib::sla::SMatrix4D<float>(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f)
#define mat4d slib::sla::SMatrix4D<double>
#define unimat4d slib::sla::SMatrix4D<double>(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)

#define m2ivec slib::sla::SVector<slib::sla::SMatrix2D<int>, slib::RMemory<slib::sla::SMatrix2D<int>>>
#define m2fvec slib::sla::SVector<slib::sla::SMatrix2D<float>, slib::RMemory<SMatrix2D<float>>>
#define m2dvec slib::sla::SVector<slib::sla::SMatrix2D<double>, slib::RMemory<slib::sla::SMatrix2D<double>>>
#define m3ivec slib::sla::SVector<slib::sla::SMatrix3D<int>, slib::RMemory<slib::sla::SMatrix3D<int>>>
#define m3fvec slib::sla::SVector<slib::sla::SMatrix3D<float>, slib::RMemory<slib::sla::SMatrix3D<float>>>
#define m3dvec slib::sla::SVector<slib::sla::SMatrix3D<double>, slib::RMemory<slib::sla::SMatrix3D<double>>>
#define m4ivec slib::sla::SVector<slib::sla::SMatrix4D<int>, slib::RMemory<slib::sla::SMatrix4D<int>>>
#define m4fvec slib::sla::SVector<slib::sla::SMatrix4D<float>, slib::RMemory<slib::sla::SMatrix4D<float>>>
#define m4dvec slib::sla::SVector<slib::sla::SMatrix4D<double>, slib::RMemory<slib::sla::SMatrix4D<double>>>
#define smivec slib::sla::SVector<smati>
#define smfvec slib::sla::SVector<smatf>
#define smdvec slib::sla::SVector<smatd>

namespace slib {
	namespace sla {

		template <typename T>
		struct SVector2D {
			T x, y;

			SVector2D(const T& val = initVal<T>());
			SVector2D(const T& x_, const T& y_);
			template<typename S>
			SVector2D(const SVector2D<S>& v) : SVector2D((T)v.x, (T)v.y) {}
			SVector2D(const SVector2D<T>& v);
			~SVector2D();

			SVector2D<T>& operator = (const SVector2D<T>& v);
			SVector2D<T>& operator += (const SVector2D<T>& v);
			SVector2D<T>& operator += (const T& val);
			SVector2D<T>& operator -= (const SVector2D<T>& v);
			SVector2D<T>& operator -= (const T& val);
			SVector2D<T>& operator *= (const T& val);
			SVector2D<T>& operator /= (const T& val);
			SVector2D<T> operator + (const SVector2D<T>& v) const;
			SVector2D<T> operator - (const SVector2D<T>& v) const;
			SVector2D<T> operator * (const T& val) const;
			SVector2D<T> operator / (const T& val) const;
			SVector2D<T> operator-() const;
			SVector2D<T> reflect(subyte r) const;
			T operator * (const SVector2D<T>& v) const;
			T cross(const SVector2D<T>& v) const;
			SVector2D<T> operator * (const T& val);
			SVector2D<T> operator / (const T& val);
			SVector2D<T> orthogonal(smath::ROTATION rot);
			void reset(const T& val = initVal<T>());

			T& operator[](int i);
			const T& operator[](int i) const;
			bool operator<(const SVector2D<T>& v) const;
			bool operator==(const SVector2D<T>& v) const;
		};
		template <typename T>
		extern inline SVector2D<T> operator*(const T& val, const SVector2D<T>& v) { return v * val; }
		extern inline v2d operator*(double d, const v2i& v) { return v2d((double)v.x * d, (double)v.y * d); }
		extern inline v2d operator*(double d, const v2f& v) { return v2d((double)v.x * d, (double)v.y * d); }
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SVector2D<T>& v) {
			os << "(" << v.x << "," << v.y << ")";
			return os;
		}
		template <typename T>
		struct SVector3D {
			T x, y, z;

			SVector3D(const T& val = initVal<T>());
			SVector3D(const T& x_, const T& y_, const T& z_);
			SVector3D(const SVector2D<T>& v);
			SVector3D(const SVector3D<T>& v);
			~SVector3D();

			SVector3D& operator = (const SVector3D<T>& v);
			SVector3D& operator += (const SVector3D<T>& v);
			SVector3D& operator += (const T& val);
			SVector3D& operator -= (const SVector3D<T>& v);
			SVector3D& operator -= (const T& val);
			SVector3D& operator *= (const T& val);
			SVector3D& operator /= (const T& val);
			SVector3D operator + (const SVector3D<T>& v) const;
			SVector3D operator - (const SVector3D<T>& v) const;
			SVector3D<T> operator * (const T& val) const;
			SVector3D<T> operator / (const T& val) const;
			SVector3D operator-() const;
			T operator * (const SVector3D<T>& v) const;
			SVector3D cross(const SVector3D<T>& v) const;
			SVector2D<T> projection(smath::PLANE p) const;
			void reset(const T& val = initVal<T>());

			T& operator[](int i);
			const T& operator[](int i) const;
			bool operator<(const SVector3D& v) const;
			bool operator==(const SVector3D& v) const;
		};

		template <typename T>
		extern inline SVector3D<T> operator*(const T& val, const SVector3D<T>& v) { return v * val; }
		extern inline v3d operator*(double d, const v3i& v) { return v3d((double)v.x * d, (double)v.y * d, (double)v.z * d); }
		extern inline v3d operator*(double d, const v3f& v) { return v3d((double)v.x * d, (double)v.y * d, (double)v.z * d); }
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SVector3D<T>& v) {
			os << "(" << v.x << "," << v.y << "," << v.z << ")";
			return os;
		}
		template <typename T>
		struct SVector4D {
			T x, y, z, w;

			SVector4D(const T& val = initVal<T>());
			SVector4D(const T& x_, const T& y_, const T& z_, const T& w_);
			SVector4D(const SVector2D<T>& v);
			SVector4D(const SVector3D<T>& v);
			SVector4D(const SVector4D<T>& v);
			~SVector4D();

			SVector4D<T>& operator = (const SVector4D<T>& v);
			SVector4D<T>& operator += (const SVector4D<T>& v);
			SVector4D<T>& operator += (const T& val);
			SVector4D<T>& operator -= (const SVector4D<T>& v);
			SVector4D<T>& operator -= (const T& val);
			SVector4D<T>& operator *= (const T& val);
			SVector4D<T>& operator /= (const T& val);
			SVector4D<T> operator + (const SVector4D<T>& v) const;
			SVector4D<T> operator - (const SVector4D<T>& v) const;
			SVector4D<T> operator-() const;
			T operator * (const SVector4D<T>& v) const;
			SVector4D<T> operator * (const T& val) const;
			SVector4D<T> operator / (const T& val) const;
			void reset(const T& val = initVal<T>());

			T& operator[](int i);
			const T& operator[](int i) const;
			bool operator<(const SVector4D& v) const;
			bool operator==(const SVector4D& v) const;
		};

		template <typename T>
		extern inline SVector4D<T> operator*(const T& val, const SVector4D<T>& v) { return v * val; }
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SVector4D<T>& v) {
			os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
			return os;
		}
		template <typename T, class M = SMemory<T>>
		struct SVector : public Array<T, M> {
			typedef Array<T, M> array;

			SVector();
			SVector(size_t s, const T& val = initVal<T>());
			SVector(std::initializer_list<T> li);
			SVector(const SVector<T, M>& v);
			~SVector();

			SVector& operator = (const SVector<T, M>& v);
			SVector& operator += (const SVector<T, M>& v);
			SVector& operator += (const T& val);
			SVector& operator -= (const SVector<T, M>& v);
			SVector& operator -= (const T& val);
			SVector& operator *= (const T& val);
			SVector& operator /= (const T& val);
			SVector operator + (const SVector<T, M>& v) const;
			SVector operator - (const SVector<T, M>& v) const;
			SVector operator - () const;
			SVector operator * (const T& val) const;
			SVector operator / (const T& val) const;
			T operator *(const svec<T, M>& v) const;
			scyc_iter<T> cycle(int i = 0);
			scyc_citer<T> cycle(int i = 0) const;
		};

		template <typename T, class M>
		extern inline SVector<T, M> operator*(const T& val, const SVector<T, M>& v) { return v * val; }
		template<typename T, class M>
		extern inline std::ostream& operator<<(std::ostream& os, const SVector<T, M>& vec) {
			sforeach(vec) {
				os << E_;
				if (it < vec.end() - 1) os << ",";
			}
			return os;
		}
		template<typename T>
		inline void addMatElement(T* current) { return; }
		template<typename T, class... Args>
		inline void addMatElement(T* current, T& val, Args... args) {
			*current = val; ++current;
			addMatElement(current, args...);
		}
		template<typename T>
		struct SMatrix2D {
			T element[4];

			SMatrix2D();
			SMatrix2D(const T& t1, const T& t2, const T& t3, const T& t4);
			~SMatrix2D();

			SMatrix2D& operator=(const SMatrix2D& m);
			SMatrix2D& operator += (const SMatrix2D& m);
			SMatrix2D& operator -= (const SMatrix2D& m);
			SMatrix2D& operator *= (const T& val);
			SMatrix2D& operator /= (const T& val);
			SMatrix2D operator + (const SMatrix2D& m) const;
			SMatrix2D operator - (const SMatrix2D& m) const;
			SMatrix2D operator - ();
			SVector2D<T> operator * (const SVector2D<T>& v) const;
			SMatrix2D operator * (const SMatrix2D& m) const;
			SMatrix2D operator * (const T& val) const;
			SMatrix2D operator / (const T& val) const;
			SMatrix2D inverse() const;
			T determinant() const;
			T* operator[](int idx);
			const T* operator[](int idx) const;
		};
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SMatrix2D<T>& m) {
			auto maxlen = 0;
			sforin(e, 0, 4) {
				if (maxlen < slib::String(m.element[e]).size()) maxlen = slib::String(m.element[e]).size();
			}
			sforin(r, 0, 2) {
				sforin(c, 0, 2) {
					os << slib::String(m.element[2 * r + c]).filled(maxlen + 2, ' ', true);
				}
				if (r < 1) os << NEW_LINE.cstr();
			}
			return os;
		}
		template<typename T>
		struct SMatrix3D {
			T element[9];

			SMatrix3D();
			SMatrix3D(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9);
			~SMatrix3D();
			SMatrix3D& operator=(const SMatrix3D& m);
			SMatrix3D& operator += (const SMatrix3D& m);
			SMatrix3D& operator -= (const SMatrix3D& m);
			SMatrix3D& operator *= (const T& val);
			SMatrix3D& operator /= (const T& val);
			SMatrix3D operator + (const SMatrix3D& m) const;
			SMatrix3D operator - (const SMatrix3D& m) const;
			SMatrix3D operator - ();
			SVector3D<T> operator * (const SVector3D<T>& v) const;
			SMatrix3D operator * (const SMatrix3D& m) const;
			SMatrix3D operator * (const T& val) const;
			SMatrix3D operator / (const T& val) const;
			SMatrix3D inverse() const;
			T determinant() const;
			SMatrix2D<T> cofactor(sint r, sint c) const;
			T* operator[](int idx);
			const T* operator[](int idx) const;
		};
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SMatrix3D<T>& m) {
			auto maxlen = 0;
			sforin(e, 0, 9) {
				if (maxlen < String(m.element[e]).size()) maxlen = String(m.element[e]).size();
			}
			sforin(r, 0, 3) {
				sforin(c, 0, 3) {
					os << String(m.element[3 * r + c]).filled(maxlen + 2, ' ', true);
				}
				if (r < 2) os << NEW_LINE.cstr();
			}
			return os;
		}
		template<typename T>
		struct SMatrix4D {
			T element[16];

			SMatrix4D();
			template<typename... Args>
			SMatrix4D(Args... args) : SMatrix4D() { addMatElement<T, Args...>(element, args...); }
			~SMatrix4D();
			SMatrix4D& operator=(const SMatrix4D& m);
			SMatrix4D& operator += (const SMatrix4D& m);
			SMatrix4D& operator -= (const SMatrix4D& m);
			SMatrix4D& operator *= (const T& val);
			SMatrix4D& operator /= (const T& val);
			SMatrix4D operator + (const SMatrix4D& m);
			SMatrix4D operator - (const SMatrix4D& m);
			SMatrix4D operator - ();
			SVector4D<T> operator * (const SVector4D<T>& v);
			SMatrix4D operator * (const SMatrix4D& m);
			SMatrix4D operator * (const T& val);
			SMatrix4D operator / (const T& val);
			SMatrix4D inverse() const;
			T determinant() const;
			SMatrix3D<T> cofactor(sint r, sint c) const;
			T* operator[](int idx);
			const T* operator[](int idx) const;
		};
		template<typename T>
		extern inline std::ostream& operator<<(std::ostream& os, const SMatrix4D<T>& m) {
			auto maxlen = 0;
			sforin(e, 0, 16) {
				if (maxlen < String(m.element[e]).size()) maxlen = String(m.element[e]).size();
			}
			sforin(r, 0, 4) {
				sforin(c, 0, 4) {
					os << String(m.element[4 * r + c]).filled(maxlen + 2, ' ', true);
				}
				if (r < 3) os << NEW_LINE.cstr();
			}
			return os;
		}
		template<typename T, class M = SMemory<T>>
		struct SMatrix : public Array<T, M> {
			typedef Array<T, M> array;
			size_t row, col;

			inline int matIndex(int r, int c) const;

		public:
			SMatrix();
			SMatrix(size_t r, size_t c);
			SMatrix(size_t r, size_t c, const T& val);
			SMatrix(size_t r, size_t c, std::initializer_list<T> li);
			SMatrix(const SMatrix& mat);
			~SMatrix();
			SMatrix& operator=(const SMatrix& mat);
			template<typename S, class N>
			SMatrix& operator=(const SMatrix<S, N>& mat) {
				resize(mat.row, mat.col);
				sforeach2(*this, mat) E1_ = (T)E2_;
			}
			SMatrix& operator += (const SMatrix& m);
			SMatrix& operator -= (const SMatrix& m);
			SMatrix& operator *= (const T& val);
			SMatrix& operator /= (const T& val);
			SMatrix operator + (const SMatrix& m);
			SMatrix operator - (const SMatrix& m);
			SMatrix operator - ();
			SVector<T, M> operator * (const SVector<T, M>& v);
			SMatrix operator * (const SMatrix& m);
			SMatrix operator * (const T& val);
			SMatrix operator / (const T& val);
			void expand(sint i, double r, smath::DIRECTION dir);
			void subtract(sint i1, sint i2, smath::DIRECTION dir);
			void swap(sint i1, sint i2, smath::DIRECTION dir);
			void sweep(sint i1, sint i2, double r, smath::DIRECTION dir);
			bool comparable(const SMatrix& m);
			T* operator[](int row);
			const T* operator[](int row) const;
			T& at(int row, int col);
			const T& at(int row, int col) const;
			SMatrix operator[](std::pair<Range<size_t>, Range<size_t>> pair);
			const SMatrix operator[](std::pair<Range<size_t>, Range<size_t>> pair) const;
			void resize(int row, int col);
			SMatrix inverse() const;
			T determinant() const;
			SMatrix cofactor(sint r, sint c) const;
			SMatrix submat(int roff, int coff, int rnum, int cnum);
			SMatrix transpose();
			bool operator<(const SMatrix<T, M>& mat) const;
			bool operator==(const SMatrix<T, M>& mat) const;
		};
		template <typename T, class M>
		extern inline SMatrix<T, M> operator*(const T& val, const SMatrix<T, M>& m) {
			SMatrix<T, M> mat(m.row, m.col); auto p = mat.ptr();
			sforeach(m) { *p = E_ * val; ++p; }
			return mat;
		}
		template <typename T, class M>
		extern inline SVector<T, M> operator*(const SVector<T, M>& v, const SMatrix<T, M>& m) {
			SVector<T> arr(m.col());
			sforin(c, 0, m.col()) {
				arr[c] = initVal<T>();
				sforin(r, 0, m.row()) arr[c] += m.value(r, c) * v[r];
			}
			return arr;
		}
		template<typename T, class M>
		extern inline std::ostream& operator<<(std::ostream& os, const SMatrix<T, M>& m) {
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
		extern inline v2i operator*(const v2i& v, const mat2i& m) {
			return v2i(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
		extern inline v2f operator*(const v2f& v, const mat2f& m) {
			return v2f(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
		extern inline v2d operator*(const v2d& v, const mat2d& m) {
			return v2d(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
		extern inline v3i operator*(const v3i& v, const mat3i& m) {
			return v3i(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
		extern inline v3f operator*(const v3f& v, const mat3f& m) {
			return v3f(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
		extern inline v3d operator*(const v3d& v, const mat3d& m) {
			return v3d(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
		extern inline v4i operator*(const v4i& v, const mat4i& m) {
			return v4i(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}
		extern inline v4f operator*(const v4f& v, const mat4f& m) {
			return v4f(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}
		extern inline v4d operator*(const v4d& v, const mat4d& m) {
			return v4d(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}

		/*============================================================*/
		template <typename T>
		SVector2D<T>::SVector2D(const T& val) : x(val), y(val) {}
		template <typename T>
		SVector2D<T>::SVector2D(const T& x_, const T& y_) : x(x_), y(y_) {}
		template <typename T>
		SVector2D<T>::SVector2D(const SVector2D<T>& v) : SVector2D(v.x, v.y) {}
		template <typename T>
		SVector2D<T>::~SVector2D() {}
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator = (const SVector2D<T>& v) { x = v.x; y = v.y; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator += (const SVector2D<T>& v) { x += v.x; y += v.y; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator += (const T& val) { x += val; y += val; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator -= (const SVector2D<T>& v) { x -= v.x; y -= v.y; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator -= (const T& val) { x -= val; y -= val; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator *= (const T& val) { x *= val; y *= val; return *this; }
		template <typename T>
		SVector2D<T>& SVector2D<T>::operator /= (const T& val) { x /= val; y /= val; return *this; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator + (const SVector2D<T>& v) const { SVector2D<T> sum = *this; sum += v; return sum; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator - (const SVector2D<T>& v) const { SVector2D<T> diff = *this; diff -= v; return diff; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator * (const T& v) const { SVector2D<T> prod = *this; prod *= v; return prod; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator / (const T& v) const { SVector2D<T> div = *this; div /= v; return div; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator-() const { SVector2D<T> v(*this); v.x = -v.x; v.y = -v.y; return v; }
		template <typename T>
		SVector2D<T> SVector2D<T>::reflect(subyte r) const { return SVector2D<T>(r & smath::HORIZONTAL ? -x : x, r & smath::VERTICAL ? -y : y); }
		template <typename T>
		T SVector2D<T>::operator * (const SVector2D<T>& v) const { return x * v.x + y * v.y; }
		template <typename T>
		T SVector2D<T>::cross(const SVector2D<T>& v) const { return x * v.y - y * v.x; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator * (const T& val) { SVector2D<T> prod = *this; prod *= val; return prod; }
		template <typename T>
		SVector2D<T> SVector2D<T>::operator / (const T& val) { SVector2D<T> div = *this; div /= val; return div; }
		template <typename T>
		SVector2D<T> SVector2D<T>::orthogonal(smath::ROTATION rot) { return rot == smath::CW ? SVector2D<T>(y, -x) : SVector2D(-y, x); }
		template <typename T>
		void SVector2D<T>::reset(const T& val) { x = val; y = val; }
		template <typename T>
		T& SVector2D<T>::operator[](int i) { return i == 0 ? x : y; }
		template <typename T>
		const T& SVector2D<T>::operator[](int i) const { return i == 0 ? x : y; }
		template <typename T>
		bool SVector2D<T>::operator<(const SVector2D<T>& v) const { return x != v.x ? x < v.x : y < v.y; }
		template <typename T>
		bool SVector2D<T>::operator==(const SVector2D<T>& v) const { return x == v.x && y == v.y; }

		template <typename T>
		SVector3D<T>::SVector3D(const T& val) : x(val), y(val), z(val) {}
		template <typename T>
		SVector3D<T>::SVector3D(const T& x_, const T& y_, const T& z_) : x(x_), y(y_), z(z_) {}
		template <typename T>
		SVector3D<T>::SVector3D(const SVector2D<T>& v) : x(v.x), y(v.y), z(initVal<T>()) {}
		template <typename T>
		SVector3D<T>::SVector3D(const SVector3D<T>& v) : x(v.x), y(v.y), z(v.z) {}
		template <typename T>
		SVector3D<T>::~SVector3D() {}

		template <typename T>
		SVector3D<T>& SVector3D<T>::operator = (const SVector3D<T>& v) { x = v.x; y = v.y; z = v.z; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator += (const SVector3D<T>& v) { x += v.x; y += v.y; z += v.z; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator += (const T& val) { x += val; y += val; z += val; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator -= (const SVector3D<T>& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator -= (const T& val) { x -= val; y -= val; z -= val; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator *= (const T& val) { x *= val; y *= val; z *= val; return *this; }
		template <typename T>
		SVector3D<T>& SVector3D<T>::operator /= (const T& val) { x /= val; y /= val; z /= val; return *this; }
		template <typename T>
		SVector3D<T> SVector3D<T>::operator + (const SVector3D<T>& v) const { SVector3D<T> sum = *this; sum += v; return sum; }
		template <typename T>
		SVector3D<T> SVector3D<T>::operator - (const SVector3D<T>& v) const { SVector3D<T> diff = *this; diff -= v; return diff; }
		template <typename T>
		SVector3D<T> SVector3D<T>::operator-() const { SVector3D<T> v(*this); v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }
		template <typename T>
		T SVector3D<T>::operator * (const SVector3D<T>& v) const { return x * v.x + y * v.y + z * v.z; }
		template <typename T>
		SVector3D<T> SVector3D<T>::cross(const SVector3D<T>& v) const { return SVector3D<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
		template <typename T>
		SVector3D<T> SVector3D<T>::operator * (const T& val) const { SVector3D<T> prod = *this; prod *= val; return prod; }
		template <typename T>
		SVector3D<T> SVector3D<T>::operator / (const T& val) const { SVector3D<T> div = *this; div /= val; return div; }
		template <typename T>
		SVector2D<T> SVector3D<T>::projection(smath::PLANE p) const {
			switch (p) {
			case smath::XY_PLANE:
				return SVector2D<T>(x, y);
			case smath::YZ_PLANE:
				return SVector2D<T>(y, z);
			default:
				return SVector2D<T>(x, z);
			}
		}
		template <typename T>
		void SVector3D<T>::reset(const T& val) { x = val; y = val; z = val; }
		template <typename T>
		T& SVector3D<T>::operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
		template <typename T>
		const T& SVector3D<T>::operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
		template <typename T>
		bool SVector3D<T>::operator<(const SVector3D& v) const { return x != v.x ? x < v.x : (y != v.y ? y < v.y : z < v.z); }
		template <typename T>
		bool SVector3D<T>::operator==(const SVector3D& v) const { return x == v.x && y == v.y && z == v.z; }

		template <typename T>
		SVector4D<T>::SVector4D(const T& val) : x(val), y(val), z(val), w(val) {}
		template <typename T>
		SVector4D<T>::SVector4D(const T& x_, const T& y_, const T& z_, const T& w_) : x(x_), y(y_), z(z_), w(w_) {}
		template <typename T>
		SVector4D<T>::SVector4D(const SVector2D<T>& v) : SVector4D(v.x, v.y, initVal<T>(), initVal<T>()) {}
		template <typename T>
		SVector4D<T>::SVector4D(const SVector3D<T>& v) : SVector4D(v.x, v.y, v.z, initVal<T>()) {}
		template <typename T>
		SVector4D<T>::SVector4D(const SVector4D<T>& v) : SVector4D(v.x, v.y, v.z, v.w) {}
		template <typename T>
		SVector4D<T>::~SVector4D() {}

		template <typename T>
		SVector4D<T>& SVector4D<T>::operator = (const SVector4D<T>& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator += (const SVector4D<T>& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator += (const T& val) { x += val; y += val; z += val; w += val; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator -= (const SVector4D<T>& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator -= (const T& val) { x -= val; y -= val; z -= val; w -= val; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator *= (const T& val) { x *= val; y *= val; z *= val; w *= val; return *this; }
		template <typename T>
		SVector4D<T>& SVector4D<T>::operator /= (const T& val) { x /= val; y /= val; z /= val; w /= val; return *this; }
		template <typename T>
		SVector4D<T> SVector4D<T>::operator + (const SVector4D<T>& v) const { SVector4D<T> sum = *this; sum += v; return sum; }
		template <typename T>
		SVector4D<T> SVector4D<T>::operator - (const SVector4D<T>& v) const { SVector4D<T> diff = *this; diff -= v; return diff; }
		template <typename T>
		SVector4D<T> SVector4D<T>::operator-() const { SVector4D<T> v(*this); v.x = -v.x; v.y = -v.y; v.z = -v.z; v.w = -v.w; return v; }
		template <typename T>
		T SVector4D<T>::operator * (const SVector4D<T>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
		template <typename T>
		SVector4D<T> SVector4D<T>::operator * (const T& val) const { SVector4D<T> prod = *this; prod *= val; return prod; }
		template <typename T>
		SVector4D<T> SVector4D<T>::operator / (const T& val) const { SVector4D<T> div = *this; div /= val; return div; }
		template <typename T>
		void SVector4D<T>::reset(const T& val) { x = val; y = val; z = val; w = val; }

		template <typename T>
		T& SVector4D<T>::operator[](int i) { return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w)); }
		template <typename T>
		const T& SVector4D<T>::operator[](int i) const { return i == 0 ? x : (i == 1 ? y : (i == 2 ? z : w)); }
		template <typename T>
		bool SVector4D<T>::operator<(const SVector4D& v) const { return x != v.x ? x < v.x : (y != v.y ? y < v.y : (z != v.z ? z < v.z : w < v.w)); }
		template <typename T>
		bool SVector4D<T>::operator==(const SVector4D& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }

		template <typename T, class M>
		SVector<T, M>::SVector() : Array<T, M>() {}
		template <typename T, class M>
		SVector<T, M>::SVector(size_t s, const T& val) : Array<T, M>(s, val) {}
		template <typename T, class M>
		SVector<T, M>::SVector(std::initializer_list<T> li) : Array<T, M>(li) {}
		template <typename T, class M>
		SVector<T, M>::SVector(const SVector<T, M>& v) : Array<T, M>(v) {}
		template <typename T, class M>
		SVector<T, M>::~SVector() {}
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator = (const SVector<T, M>& v) {
			if (v.size()) array::copy(v.ptr(), v.size());
			else array::clear();
			return (*this);
		}
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator += (const SVector<T, M>& v) {
			if (array::size() != v.size())
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
			auto p = array::ptr(), p_ = v.ptr();
			sforin(i, 0, array::size()) { *p += *p_; ++p; ++p_; }
			return *this;
		}
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator += (const T& val) { sforeach(*this) E_ += val; return *this; }
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator -= (const SVector<T, M>& v) {
			if (array::size() != v.size())
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
			auto p = v.ptr();
			sforeach(*this) { E_ -= *p; ++p; }
			return *this;
		}
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator -= (const T& val) { sforeach(*this) { E_ -= val; } return *this; }
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator *= (const T& val) { sforeach(*this) { E_ *= val; } return *this; }
		template <typename T, class M>
		SVector<T, M>& SVector<T, M>::operator /= (const T& val) { sforeach(*this) { E_ /= val; } return *this; }
		template <typename T, class M>
		SVector<T, M> SVector<T, M>::operator + (const SVector<T, M>& v) const { SVector<T, M> sum = *this; sum += v; return sum; }
		template <typename T, class M>
		SVector<T, M> SVector<T, M>::operator - (const SVector<T, M>& v) const { SVector<T, M> diff = *this; diff -= v; return diff; }
		template <typename T, class M>
		SVector<T, M> SVector<T, M>::operator - () const {
			SVector v(*this);
			sforeach(v) E_ = -E_;
			return v;
		}
		template <typename T, class M>
		SVector<T, M> SVector<T, M>::operator * (const T& val) const { SVector<T, M> prod = *this; prod *= val; return prod; }
		template <typename T, class M>
		SVector<T, M> SVector<T, M>::operator / (const T& val) const { SVector<T, M> div = *this; div /= val; return div; }
		template <typename T, class M>
		T SVector<T, M>::operator *(const svec<T, M>& v) const {
			if (array::size() != v.size() || array::empty())
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this, v", smath::DIMENSION_SIZE_DIFF(array::size(), v.size()));
			T prod = initVal<T>();
			sforeach(*this) prod += E_ * E_;
			return prod;
		}
		template <typename T, class M>
		scyc_iter<T> SVector<T, M>::cycle(int i) { return scyc_iter<T>(array::ptr(i), i, array::size()); }
		template <typename T, class M>
		scyc_citer<T> SVector<T, M>::cycle(int i) const { return scyc_citer<T>(array::ptr(i), i, array::size()); }

		template <typename T>
		SMatrix2D<T>::SMatrix2D() { auto p = element; sforin(i, 0, 4) { *p = initVal<T>(); ++p; } }
		template <typename T>
		SMatrix2D<T>::SMatrix2D(const T& t1, const T& t2, const T& t3, const T& t4) {
			element[0] = t1; element[1] = t2; element[2] = t3; element[3] = t4;
		}
		template <typename T>
		SMatrix2D<T>::~SMatrix2D() {}

		template <typename T>
		SMatrix2D<T>& SMatrix2D<T>::operator=(const SMatrix2D<T>& m) {
			auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p = *p_; ++p; ++p_; }
			return *this;
		}
		template <typename T>
		SMatrix2D<T>& SMatrix2D<T>::operator += (const SMatrix2D<T>& m) {
			auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p += *p_; ++p; ++p_; }
			return *this;
		}
		template <typename T>
		SMatrix2D<T>& SMatrix2D<T>::operator -= (const SMatrix2D<T>& m) {
			auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p -= *p_; ++p; ++p_; }
			return *this;
		}
		template <typename T>
		SMatrix2D<T>& SMatrix2D<T>::operator *= (const T& val) {
			auto p = element; sforin(i, 0, 4) { *p *= val; ++p; }
			return *this;
		}
		template <typename T>
		SMatrix2D<T>& SMatrix2D<T>::operator /= (const T& val) {
			auto p = element; sforin(i, 0, 4) { *p /= val; ++p; }
			return *this;
		}
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator + (const SMatrix2D<T>& m) const { return SMatrix2D<T>(*this) += m; }
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator - (const SMatrix2D<T>& m) const { return SMatrix2D<T>(*this) -= m; }
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator - () { SMatrix2D<T> m(*this); sforin(i, 0, 4) m.element[i] = -m.element[i]; return m; }
		template <typename T>
		SVector2D<T> SMatrix2D<T>::operator * (const SVector2D<T>& v) const {
			SVector2D<T> res;
			auto p = element;
			res.x = (*p) * v.x; ++p;
			res.x += (*p) * v.y; ++p;
			res.y = (*p) * v.x; ++p;
			res.y += (*p) * v.y;
			return res;
		}
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator * (const SMatrix2D<T>& m) const {
			SMatrix2D res;
			sforin(r, 0, 2) {
				sforin(c, 0, 2) {
					sforin(v, 0, 2) res[r][c] += element[2 * r + c] * m[v][c];
				}
			}
			return res;
		}
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator * (const T& val) const { return SMatrix2D<T>(*this) *= val; }
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::operator / (const T& val) const { return SMatrix2D<T>(*this) /= val; }
		template <typename T>
		SMatrix2D<T> SMatrix2D<T>::inverse() const {
			auto det = determinant();
			if (det == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return SMatrix2D<T>(element[3] / det, -element[1] / det, -element[2] / det, element[0] / det);
		}
		template <typename T>
		T SMatrix2D<T>::determinant() const {
			return element[0] * element[3] - element[1] * element[2];
		}
		template <typename T>
		T* SMatrix2D<T>::operator[](int idx) { return &element[idx * 2]; }
		template <typename T>
		const T* SMatrix2D<T>::operator[](int idx) const { return &element[idx * 2]; }

		template <typename T>
		SMatrix3D<T>::SMatrix3D() { auto p = element; sforin(i, 0, 9)* p = initVal<T>(); }
		template <typename T>
		SMatrix3D<T>::SMatrix3D(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9) : SMatrix3D() {
			element[0] = t1; element[1] = t2; element[2] = t3;
			element[3] = t4; element[4] = t5; element[5] = t6;
			element[6] = t7; element[7] = t8; element[8] = t9;
		}
		template <typename T>
		SMatrix3D<T>::~SMatrix3D() {}

		template <typename T>
		SMatrix3D<T>& SMatrix3D<T>::operator=(const SMatrix3D<T>& m) {
			auto p = element; auto p_ = m.element;
			sforin(i, 0, 9)* p = *p_;
		}
		template <typename T>
		SMatrix3D<T>& SMatrix3D<T>::operator += (const SMatrix3D<T>& m) {

			sforin(i, 0, 9) element[i] += m.element[i];
			return *this;
		}
		template <typename T>
		SMatrix3D<T>& SMatrix3D<T>::operator -= (const SMatrix3D<T>& m) {
			sforin(i, 0, 4) element[i] -= m.element[i];
			return *this;
		}
		template <typename T>
		SMatrix3D<T>& SMatrix3D<T>::operator *= (const T& val) {
			sforin(i, 0, 4) element[i] *= val;
			return *this;
		}
		template <typename T>
		SMatrix3D<T>& SMatrix3D<T>::operator /= (const T& val) {
			sforin(i, 0, 4) element[i] /= val;
			return *this;
		}
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator + (const SMatrix3D<T>& m) const { SMatrix3D<T> mat = *this; mat += m; return mat; }
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator - (const SMatrix3D<T>& m) const { SMatrix3D<T> mat = *this; mat -= m; return mat; }
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator - () { SMatrix3D<T> m(*this); sforin(i, 0, 9) m.element[i] = -m.element[i]; return m; }
		template <typename T>
		SVector3D<T> SMatrix3D<T>::operator * (const SVector3D<T>& v) const {
			SVector3D<T> res;
			sforin(r, 0, 3) {
				sforin(c, 0, 3) res[r] += element[3 * r + c] * v[c];
			}
			return res;
		}
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator * (const SMatrix3D<T>& m) const {
			SMatrix3D res;
			sforin(r, 0, 3) {
				sforin(c, 0, 3) {
					sforin(v, 0, 3) res[r][c] += element[3 * r + c] * m[v][c];
				}
			}
			return res;
		}
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator * (const T& val) const { SMatrix3D<T> prod = *this; prod *= val; return prod; }
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::operator / (const T& val) const { SMatrix3D<T> div = *this; div /= val; return div; }
		template <typename T>
		SMatrix3D<T> SMatrix3D<T>::inverse() const {
			auto det = determinant();
			if (det == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return SMatrix3D<T>(
				(element[4] * element[8] - element[5] * element[7]) / det,
				-(element[1] * element[8] - element[2] * element[7]) / det,
				(element[1] * element[5] - element[2] * element[4]) / det,
				-(element[3] * element[8] - element[5] * element[6]) / det,
				(element[0] * element[8] - element[2] * element[6]) / det,
				-(element[0] * element[5] - element[2] * element[3]) / det,
				(element[3] * element[7] - element[4] * element[6]) / det,
				-(element[0] * element[7] - element[1] * element[6]) / det,
				(element[0] * element[4] - element[1] * element[3]) / det
				);
		}
		template <typename T>
		SMatrix2D<T> SMatrix3D<T>::cofactor(sint r, sint c) const {
			SMatrix2D<T> mat;
			switch (r)
			{
			case 0:
			{
				switch (c)
				{
				case 0:
					return SMatrix2D<T>(element[4], element[5], element[7], element[8]);
				case 1:
					return SMatrix2D<T>(element[3], element[5], element[6], element[8]);
				case 2:
					return SMatrix2D<T>(element[3], element[4], element[6], element[7]);
				default:
					break;
				}
			}
			case 1:
			{
				switch (c)
				{
				case 0:
					return SMatrix2D<T>(element[1], element[2], element[7], element[8]);
				case 1:
					return SMatrix2D<T>(element[0], element[2], element[6], element[8]);
				case 2:
					return SMatrix2D<T>(element[0], element[1], element[6], element[7]);
				default:
					break;
				}
			}
			case 2:
			{
				switch (c)
				{
				case 0:
					return SMatrix2D<T>(element[1], element[2], element[4], element[5]);
				case 1:
					return SMatrix2D<T>(element[0], element[2], element[3], element[5]);
				case 2:
					return SMatrix2D<T>(element[1], element[2], element[3], element[4]);
				default:
					break;
				}
			}
			default:
				break;
			}
			throw SMathException(ERR_INFO, SLIB_RANGE_ERROR);
		}
		template <typename T>
		T SMatrix3D<T>::determinant() const {
			return element[0] * element[4] * element[8] + element[1] * element[5] * element[6] + element[2] * element[3] * element[7] -
				element[2] * element[4] * element[6] - element[1] * element[3] * element[8] - element[0] * element[5] * element[7];
		}
		template <typename T>
		T* SMatrix3D<T>::operator[](int idx) { return &element[idx * 3]; }
		template <typename T>
		const T* SMatrix3D<T>::operator[](int idx) const { return &element[idx * 3]; }

		template <typename T>
		SMatrix4D<T>::SMatrix4D() { sforin(i, 0, 16) element[i] = initVal<T>(); }
		template <typename T>
		SMatrix4D<T>::~SMatrix4D() {}
		template <typename T>
		SMatrix4D<T>& SMatrix4D<T>::operator=(const SMatrix4D<T>& m) {
			sforin(i, 0, 4) element[i] = m[i];
		}
		template <typename T>
		SMatrix4D<T>& SMatrix4D<T>::operator += (const SMatrix4D<T>& m) {
			sforin(i, 0, 4) element[i] += m.element[i];
			return *this;
		}
		template <typename T>
		SMatrix4D<T>& SMatrix4D<T>::operator -= (const SMatrix4D<T>& m) {
			sforin(i, 0, 4) element[i] -= m.element[i];
			return *this;
		}
		template <typename T>
		SMatrix4D<T>& SMatrix4D<T>::operator *= (const T& val) {
			sforin(i, 0, 4) element[i] *= val;
			return *this;
		}
		template <typename T>
		SMatrix4D<T>& SMatrix4D<T>::operator /= (const T& val) {
			sforin(i, 0, 4) element[i] /= val;
			return *this;
		}
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator + (const SMatrix4D<T>& m) { SMatrix4D<T> mat = *this; mat += m; return mat; }
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator - (const SMatrix4D<T>& m) { SMatrix4D<T> mat = *this; mat -= m; return mat; }
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator - () { SMatrix4D<T> m(*this); sforin(i, 0, 16) m.element[i] = -m.element[i]; return m; }
		template <typename T>
		SVector4D<T> SMatrix4D<T>::operator * (const SVector4D<T>& v) {
			SVector4D<T> res;
			sforin(r, 0, 4) {
				sforin(c, 0, 4) res[r] += element[4 * r + c] * v[c];
			}
			return res;
		}
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator * (const SMatrix4D<T>& m) {
			SMatrix4D<T> res;
			sforin(r, 0, 4) {
				sforin(c, 0, 4) {
					sforin(v, 0, 4) res[r][c] += element[4 * r + c] * m[v][c];
				}
			}
			return res;
		}
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator * (const T& val) { SMatrix4D<T> prod = *this; prod *= val; return prod; }
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::operator / (const T& val) { SMatrix4D<T> div = *this; div /= val; return div; }
		/*
		template <typename T>
		SMatrix4D<T> SMatrix4D<T>::inverse() const {
				auto det = determinant();
				if (det == 0) throw SMathException(SLIB_ERR, DIV_ZERO_ERR);
				return SMatrix4D<T>(
						(element[5]*element[10]*element[15]+)/det,

						);
		}
		*/
		template <typename T>
		T SMatrix4D<T>::determinant() const {
			return element[0] * cofactor(0, 0).determinant() - element[4] * cofactor(1, 0).determinant()
				+ element[8] * cofactor(2, 0).determinant() - element[12] * cofactor(3, 0).determinant();
		}
		template <typename T>
		SMatrix3D<T> SMatrix4D<T>::cofactor(sint r, sint c) const {
			SMatrix2D<T> mat;
			switch (r)
			{
			case 0:
			{
				switch (c)
				{
				case 0:
					return SMatrix3D<T>(element[5], element[6], element[7],
						element[9], element[10], element[11],
						element[13], element[14], element[15]);
				case 1:
					return SMatrix3D<T>(element[4], element[6], element[7],
						element[8], element[10], element[11],
						element[12], element[14], element[15]);
				case 2:
					return SMatrix3D<T>(element[4], element[5], element[7],
						element[8], element[9], element[11],
						element[12], element[13], element[15]);
				case 3:
					return SMatrix3D<T>(element[4], element[5], element[6],
						element[8], element[9], element[10],
						element[12], element[13], element[14]);
				default:
					break;
				}
			}
			case 1:
			{
				switch (c)
				{
				case 0:
					return SMatrix3D<T>(element[1], element[2], element[3],
						element[9], element[10], element[11],
						element[13], element[14], element[15]);
				case 1:
					return SMatrix3D<T>(element[0], element[2], element[3],
						element[8], element[10], element[11],
						element[12], element[14], element[15]);
				case 2:
					return SMatrix3D<T>(element[0], element[1], element[3],
						element[8], element[9], element[11],
						element[12], element[13], element[15]);
				case 3:
					return SMatrix3D<T>(element[0], element[1], element[2],
						element[8], element[9], element[10],
						element[12], element[13], element[14]);
				default:
					break;
				}
			}
			case 2:
			{
				switch (c)
				{
				case 0:
					return SMatrix3D<T>(element[1], element[2], element[3],
						element[5], element[6], element[7],
						element[13], element[14], element[15]);
				case 1:
					return SMatrix3D<T>(element[0], element[2], element[3],
						element[4], element[6], element[7],
						element[12], element[14], element[15]);
				case 2:
					return SMatrix3D<T>(element[0], element[1], element[3],
						element[4], element[5], element[7],
						element[12], element[13], element[15]);
				case 3:
					return SMatrix3D<T>(element[0], element[1], element[2],
						element[4], element[5], element[6],
						element[12], element[13], element[14]);
				default:
					break;
				}
			}
			case 3:
			{
				switch (c)
				{
				case 0:
					return SMatrix3D<T>(element[1], element[2], element[3],
						element[5], element[6], element[7],
						element[9], element[10], element[11]);
				case 1:
					return SMatrix3D<T>(element[0], element[2], element[3],
						element[4], element[6], element[7],
						element[8], element[10], element[11]);
				case 2:
					return SMatrix3D<T>(element[0], element[1], element[3],
						element[4], element[5], element[7],
						element[8], element[9], element[11]);
				case 3:
					return SMatrix3D<T>(element[0], element[1], element[2],
						element[4], element[5], element[6],
						element[8], element[9], element[10]);
				default:
					break;
				}
			}
			default:
				break;
			}
			throw SMathException(ERR_INFO, SLIB_RANGE_ERROR);
		}
		template <typename T>
		T* SMatrix4D<T>::operator[](int idx) { return &element[idx * 4]; }
		template <typename T>
		const T* SMatrix4D<T>::operator[](int idx) const { return &element[idx * 4]; }

		template<typename T, class M>
		inline sint SMatrix<T, M>::matIndex(int r, int c) const { return (sint)col * r + c; }

		template<typename T, class M>
		SMatrix<T, M>::SMatrix() : row(0), col(0), array() {}
		template<typename T, class M>
		SMatrix<T, M>::SMatrix(size_t r, size_t c) : row(r), col(c), array(r* c) {}
		template<typename T, class M>
		SMatrix<T, M>::SMatrix(size_t r, size_t c, const T& val) : row(r), col(c), array(r* c, val) {}
		template<typename T, class M>
		SMatrix<T, M>::SMatrix(size_t r, size_t c, std::initializer_list<T> li) : row(r), col(c), array(li) {}
		template<typename T, class M>
		SMatrix<T, M>::SMatrix(const SMatrix<T, M>& mat) : SMatrix<T, M>(mat.row, mat.col) { array::copy(mat.ptr(), mat.size()); }
		template<typename T, class M>
		SMatrix<T, M>::~SMatrix() {}

		template<typename T, class M>
		SMatrix<T, M>& SMatrix<T, M>::operator=(const SMatrix<T, M>& mat) {
			array::clear(); row = mat.row; col = mat.col;
			array::copy(mat.ptr(), mat.size()); return *this;
		}
		template<typename T, class M>
		SMatrix<T, M>& SMatrix<T, M>::operator += (const SMatrix<T, M>& m) {
			if (!comparable(m))
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this, m", "");
			sforeach2(*this, m) E1_ += E2_;
			return *this;
		}
		template<typename T, class M>
		SMatrix<T, M>& SMatrix<T, M>::operator -= (const SMatrix<T, M>& m) {
			if (!comparable(m))
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this, m", "");
			sforeach2(*this, m) E1_ -= E2_;
			return *this;
		}
		template<typename T, class M>
		SMatrix<T, M>& SMatrix<T, M>::operator *= (const T& val) {
			sforeach(*this) E_ *= val;
			return *this;
		}
		template<typename T, class M>
		SMatrix<T, M>& SMatrix<T, M>::operator /= (const T& val) {
			sforeach(*this) E_ /= val;
			return *this;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator + (const SMatrix<T, M>& m) { smat<T, M> mat(*this); mat += m; return mat; }
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator - (const SMatrix<T, M>& m) { smat<T, M> mat(*this); mat -= m; return mat; }
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator - () {
			smat<T, M> mat(*this);
			sforeach(mat) E_ = -E_;
			return mat;
		}
		template<typename T, class M>
		SVector<T, M> SMatrix<T, M>::operator * (const SVector<T, M>& v) {
			if (col != v.size())
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "this.col, v.size", DIMENSION_SIZE_DIFF(col, v.size()));
			SVector<T, M> prod(row, 0);
			sforeach2(prod, *this) {
				sforeach_(vit, v) E1_ += E2_ * (*vit);
			}
			return prod;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator *(const SMatrix<T, M>& m) {
			if (col != m.row)
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR, "matrix", "m.row");
			smat<T,M> m_(row, row, 0);
			sforin(r, 0, row) {
				sforin(c, 0, m.col) {
					sforin(v, 0, col) m_[r][c] += at(r, v) * m[v][c];
				}
			}
			return m_;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator * (const T& val) { SMatrix<T, M> prod = *this; prod *= val; return prod; }
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::operator / (const T& val) { SMatrix<T, M> div = *this; div /= val; return div; }
		template<typename T, class M>
		void SMatrix<T, M>::expand(sint i, double r, smath::DIRECTION dir) {
			if (dir == smath::COLUMN) {
				auto v = &at(0, i);
				sforin(r, 0, row) {
					(*v) *= r;
					v += col;
				}
			}
			else {
				auto v = &at(i, 0);
				sforin(c, 0, col) {
					(*v) *= r;
					++v;
				}
			}
		}
		template<typename T, class M>
		void SMatrix<T, M>::subtract(sint i1, sint i2, smath::DIRECTION dir) {
			if (dir == smath::COLUMN) {
				auto v1 = &at(0, i1), v2 = &at(0, i2);
				sforin(r, 0, row) {
					(*v1) -= (*v2);
					v1 += col; v2 += col;
				}
			}
			else {
				auto v1 = &at(i1, 0), v2 = &at(i2, 0);
				sforin(c, 0, col) {
					(*v1) -= (*v2);
					++v1; ++v2;
				}
			}
		}
		template<typename T, class M>
		void SMatrix<T, M>::swap(sint i1, sint i2, smath::DIRECTION dir) {
			T tmp;
			if (dir == smath::COLUMN) {
				auto v1 = &at(0, i1), v2 = &at(0, i2);
				sforin(r, 0, row) {
					if (*v1 != *v2) {
						tmp = *v1; *v1 = *v2; *v2 = tmp;
					}
					v1 += col; v2 += col;
				}
			}
			else {
				auto v1 = &at(i1, 0), v2 = &at(i2, 0);
				sforin(c, 0, col) {
					if (*v1 != *v2) {
						tmp = *v1; *v1 = *v2; *v2 = tmp;
					}
					++v1; ++v2;
				}
			}
		}
		template<typename T, class M>
		void SMatrix<T, M>::sweep(sint i1, sint i2, double r, smath::DIRECTION dir) {
			if (dir == smath::COLUMN) {
				auto v1 = &at(0, i1), v2 = &at(0, i2);
				sforin(r, 0, row) {
					(*v1) -= r * (*v2);
					v1 += col; v2 += col;
				}
			}
			else {
				auto v1 = &at(i1, 0), v2 = &at(i2, 0);
				sforin(c, 0, col) {
					(*v1) -= r * (*v2);
					++v1; ++v2;
				}
			}
		}
		template<typename T, class M>
		bool SMatrix<T, M>::comparable(const SMatrix& m) { return col == m.col && row == m.row; }
		template<typename T, class M>
		T* SMatrix<T, M>::operator[](int row) { return array::ptr(matIndex(row, 0)); }
		template<typename T, class M>
		const T* SMatrix<T, M>::operator[](int row) const { return array::ptr(matIndex(row, 0)); }
		template<typename T, class M>
		T& SMatrix<T, M>::at(int row, int col) { return array::at(matIndex(row, col)); }
		template<typename T, class M>
		const T& SMatrix<T, M>::at(int row, int col) const { return array::at(matIndex(row, col)); }
		template<typename T, class M>
		void SMatrix<T, M>::resize(int r, int c) {
			auto tmp = smat<T, M>(r, c);
			if (row && col) {
				sforin(i, 0, row < r ? row : r) {
					sforin(j, 0, col < c ? col : c) tmp[i][j] = at(i, j);
				}
			}
			array::swap(tmp);
			row = r; col = c;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::inverse() const {
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
				if (val == 0) throw smath::SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
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
		template<typename T, class M>
		T SMatrix<T, M>::determinant() const {
			if (row != col) throw SMathException(ERR_INFO, SLIB_FORMAT_ERROR);
			auto mat = *this;
			double det = 1.0, tmp;
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
					tmp = (double)mat[j][i] / mat[i][i];
					sforin(k, i, col) mat[j][k] -= tmp * mat[i][k];
				}
				det *= mat[i][i];
			}
			det *= mat[row - 1][row - 1];
			return det;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::cofactor(sint r, sint c) const {
			SMatrix<T, M> mat(row - 1, col - 1);
			auto ori = this->ptr(), ptr = mat.ptr();
			sforin(i, 0, row) {
				if (i == r) {
					ori += col; continue;
				}
				sforin(j, 0, col) {
					if (j == c) { ++ori; continue; }
					*ptr = *ori;
					++ori; ++ptr;
				}
			}
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::submat(int roff, int coff, int rnum, int cnum) {
			SMatrix<T, M> m(rnum, cnum);
			sforin(r, 0, rnum) { sforin(c, 0, cnum) m.at(r, c) = at(roff + r, coff + c); }
			return m;
		}
		template<typename T, class M>
		SMatrix<T, M> SMatrix<T, M>::transpose() {
			SMatrix<T, M> m(col, row);
			sforin(r, 0, row) { sforin(c, 0, col) m[c][r] = at(r, c); }
			return m;
		}
		template<typename T, class M>
		bool SMatrix<T, M>::operator<(const SMatrix<T, M>& mat) const {
			if (row != mat.row) return row < mat.row;
			if (col != mat.col) return col < mat.col;
			auto p = array::ptr(), p_ = mat.ptr();
			sforeachi(*this) {
				if (*p != *p_) return *p < *p_;
				++p; ++p_;
			}
			return false;
		}
		template<typename T, class M>
		bool SMatrix<T, M>::operator==(const SMatrix<T, M>& mat) const {
			if (row != mat.row || col != mat.col) return false;
			auto p = array::ptr(), p_ = mat.ptr();
			sforeachi(*this) {
				if (*p != *p_) return false;
				++p; ++p_;
			}
			return true;
		}
		extern inline sveci sequence(const srange& range) {
			sveci v(range.length(true));
			auto i = range.begin;
			sforeach(v) {
				E_ = i; ++i;
			}
			return v;
		}
		extern inline sveci sequence(size_t num) {
			return sequence(srange(1, (sint)num));
		}
		extern inline sveci arithsequence(int beg, int diff, size_t num) {
			sveci v(num);
			auto it = v.begin();
			E_ = beg; NEXT_;
			sforin(i, 1, num) {
				E_ = E_PREV + diff; NEXT_;
			}
			return v;
		}
		extern inline svecd arithsequence(double beg, double diff, size_t num) {
			svecd v(num);
			auto it = v.begin();
			E_ = beg; NEXT_;
			sforin(i, 1, num) {
				E_ = E_PREV + diff; NEXT_;
			}
			return v;
		}
		extern inline svecd arithsequence(srange range, size_t num) {
			double diff = (double)range.length(true);
			return arithsequence((double)range.begin, diff, num);
		}
		extern inline svecd geosequence(double beg, double ratio, size_t num) {
			svecd v(num);
			auto it = v.begin();
			E_ = beg; NEXT_;
			sforin(i, 1, num) {
				E_ = E_PREV * ratio; NEXT_;
			}
			return v;
		}
		template <typename T, class M>
		extern inline SVector<T, M> repeat(SVector<T, M>& vec, std::initializer_list<int> num) {
			SVector<T, M> rep;
			sveci count(num);
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
		extern inline svecd toReal(const sveci v) {
			svecd vd(v.size());
			sforeach2(v, vd) E2_ = (double)E1_;
			return vd;
		}
		template <typename T, class M>
		extern inline SMatrix<T, M> uniMat(size_t n) {
			SMatrix<T, M> mat(n, n, initVal<T>());
			sforin(i, 0, n) mat[i][i] = unitVal<T>();
			return mat;
		}
		extern inline smati uniMati(size_t n) {
			smati mat(n, n, 0);
			sforin(i, 0, n) mat[i][i] = 1;
			return mat;
		}
		extern inline smatf uniMatf(size_t n) {
			smatf mat(n, n, 0.0f);
			sforin(i, 0, n) mat[i][i] = 1.0f;
			return mat;
		}
		extern inline smatd uniMatd(size_t n) {
			smatd mat(n, n, 0.0);
			sforin(i, 0, n) mat[i][i] = 1.0;
			return mat;
		}
		extern inline smatd toReal(const smati& mat) {
			smatd matd(mat.row, mat.col);
			sforeach2(mat, matd) E2_ = (double)E1_;
			return matd;
		}
		template <typename T, class M>
		extern inline SVector<SVector<T, M>> toVec(const SMatrix<T, M>& mat) {
			SVector<SVector<T, M>> vec(mat.row);
			sforin(r, 0, mat.row) vec[r].copy(mat[r], mat.col);
			return vec;
		}
		template<typename T>
		extern inline double solveEq1(svec2d<T> coef) {
			if (coef.x == 0) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			return -coef.y / coef.x;
		}
		template<typename T>
		extern inline double discriminant(svec3d<T> coef) {
			return smath::power(coef.y, 2) - 4.0 * coef.x * coef.z;
		}
		template<typename T>
		extern inline v2d solveEq2(svec3d<T> coef) {
			if (coef.x == 0) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			auto d = discriminant(coef);
			if (d < 0) throw SMathException(ERR_INFO);
			d = sqrt(d);
			return v2d((-coef.y - d) / (2.0 * coef.x), (-coef.y + d) / (2.0 * coef.x));
		}
		template<typename T>
		extern inline svec2d<smath::Complex<double>> solveEq2c(svec3d<T> coef) {
			if (coef.x == 0) throw SMathException(ERR_INFO, smath::DIV_ZERO_ERR);
			auto d = discriminant(coef), re = 0.0, im = 0.0;
			if (d < 0) {
				re = -coef.y / (2.0 * coef.x); im = sqrt(-d) / (2.0 * coef.x);
				return svec2d<smath::Complex<double>>(smath::Complex<double>(re, -im), smath::Complex<double>(re, im));
			}
			else {
				re = -coef.y / (2.0 * coef.x); im = sqrt(d) / (2.0 * coef.x);
				return svec2d<smath::Complex<double>>(smath::Complex<double>(re - im, 0), smath::Complex<double>(re + im, 0));
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
			svec2d<smath::Complex<double>> sol;
			while (r_ != 1) {
				if (evec[r_ - 1][r_ - 2] < smath::D_EPS) { --r_; continue; }
				sub[0] = evec[r_ - 2][r_ - 2];
				sub[1] = evec[r_ - 2][r_ - 1];
				sub[2] = evec[r_ - 1][r_ - 2];
				sub[3] = evec[r_ - 1][r_ - 1];
				sol = solveEq2c(v3d(1, sub[0] + sub[3], sub[0] * sub[3] - sub[1] * sub[2]));
				if (abs(sub[3] - sol.x.real) < abs(sub[3] - sol.y.real))
					mu = sub[3] - sol.x.real;
				else  mu = sub[3] - sol.y.real;
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
	}
}
#endif