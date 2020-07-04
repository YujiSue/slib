#ifndef SMATH_LA_H
#define SMATH_LA_H

#include "sconfig.h"
#include "smath/mathbasic.h"
#include "sbasic/array.h"
#include "sbasic/cyciter.h"

namespace slib {
    namespace smath {
        
#define svec2d SVector2D
#define svec3d SVector3D
#define svec4d SVector4D
#define svec SVector

#define v2i SVector2D<sint>
#define v2f SVector2D<float>
#define v2d SVector2D<double>
#define v3i SVector3D<sint>
#define v3f SVector3D<float>
#define v3d SVector3D<double>
#define v4i SVector4D<sint>
#define v4f SVector4D<float>
#define v4d SVector4D<double>
#define svecb SVector<sbyte, CMemory<sbyte>>
#define svecub SVector<subyte, CMemory<subyte>>
#define sveci SVector<sint, CMemory<sint>>
#define svecu SVector<suint, CMemory<suint>>
#define svecl SVector<sinteger, CMemory<sinteger>>
#define svecs SVector<suinteger, CMemory<suinteger>>
#define svecf SVector<float, CMemory<float>>
#define svecd SVector<double, CMemory<double>>
#define v2ivec SVector<v2i, RMemory<v2i>>
#define v2fvec SVector<v2f, RMemory<v2f>>
#define v2dvec SVector<v2d, RMemory<v2d>>
#define v3ivec SVector<v3i, RMemory<v3i>>
#define v3fvec SVector<v3f, RMemory<v3f>>
#define v3dvec SVector<v3d, RMemory<v3d>>
#define v4ivec SVector<v4i, RMemory<v4i>>
#define v4fvec SVector<v4f, RMemory<v4f>>
#define v4dvec SVector<v4d, RMemory<v4d>>
#define svivec SVector<sveci, SMemory<sveci>>
#define svfvec SVector<svecf, SMemory<svecf>>
#define svdvec SVector<svecd, SMemory<svecd>>

        template <typename T>
        struct SVector2D {
            T x, y;
            
			SVector2D(const T &val = initVal<T>());
			SVector2D(const T &x_, const T &y_);
            template<typename S>
			SVector2D(const SVector2D<S> &v) : SVector2D((T)v.x, (T)v.y) {}
			SVector2D(const SVector2D<T> &v);
            ~SVector2D();
            
			SVector2D<T> &operator = (const SVector2D<T> &v);
            SVector2D<T> &operator += (const SVector2D<T> &v);
            SVector2D<T> &operator += (const T &val);
            SVector2D<T> &operator -= (const SVector2D<T> &v);
            SVector2D<T> &operator -= (const T &val);
            SVector2D<T> &operator *= (const T &val);
            SVector2D<T> &operator /= (const T &val);
            SVector2D<T> operator + (const SVector2D<T> &v) const;
            SVector2D<T> operator - (const SVector2D<T> &v) const;
            SVector2D<T> operator-() const;
            SVector2D<T> reflect(subyte r) const;
            T operator * (const SVector2D<T> &v) const;
            T cross(const SVector2D<T> &v) const;
            SVector2D<T> operator * (const T &val);
            SVector2D<T> operator / (const T &val);
            SVector2D<T> orthogonal(ROTATION rot);
            void reset(const T &val = initVal<T>());
            
            T &operator[](int i);
            const T &operator[](int i) const;
            bool operator<(const SVector2D<T> &v) const;
            bool operator==(const SVector2D<T> &v) const;
        };
        template <typename T>
        extern inline SVector2D<T> operator*(const T &val, const SVector2D<T> &v) { return v*val; }
        
        template <typename T>
        struct SVector3D  {
            T x, y, z;
            
            SVector3D(const T &val = initVal<T>());
            SVector3D(const T &x_, const T &y_, const T &z_);
            SVector3D(const SVector2D<T> &v);
            SVector3D(const SVector3D<T> &v);
            ~SVector3D();
            
            SVector3D &operator = (const SVector3D<T> &v);
            SVector3D &operator += (const SVector3D<T> &v);
            SVector3D &operator += (const T &val);
            SVector3D &operator -= (const SVector3D<T> &v);
            SVector3D &operator -= (const T &val);
            SVector3D &operator *= (const T &val);
            SVector3D &operator /= (const T &val);
            SVector3D operator + (const SVector3D<T> &v) const;
            SVector3D operator - (const SVector3D<T> &v) const;
            SVector3D operator-() const;
            T operator * (const SVector3D<T> &v) const;
            SVector3D cross(const SVector3D<T> &v) const;
            SVector3D operator * (const T &val) const;
            SVector3D operator / (const T &val) const;
            SVector2D<T> projection(PLANE p) const;
            void reset(const T &val = initVal<T>());
            
            T &operator[](int i);
            const T &operator[](int i) const;
            bool operator<(const SVector3D &v) const;
            bool operator==(const SVector3D &v) const;
        };
        
        template <typename T>
        extern inline SVector3D<T> operator*(const T &val, const SVector3D<T> &v) { return v*val; }
        
        template <typename T>
        struct SVector4D {
            T x, y, z, w;
            
            SVector4D(const T &val = initVal<T>());
            SVector4D(const T &x_, const T &y_, const T &z_, const T &w_);
            SVector4D(const SVector2D<T> &v);
            SVector4D(const SVector3D<T> &v);
            SVector4D(const SVector4D<T> &v);
            ~SVector4D();
            
            SVector4D<T> &operator = (const SVector4D<T> &v);
            SVector4D<T> &operator += (const SVector4D<T> &v);
            SVector4D<T> &operator += (const T &val);
            SVector4D<T> &operator -= (const SVector4D<T> &v);
            SVector4D<T> &operator -= (const T &val);
            SVector4D<T> &operator *= (const T &val);
            SVector4D<T> &operator /= (const T &val);
            SVector4D<T> operator + (const SVector4D<T> &v) const;
            SVector4D<T> operator - (const SVector4D<T> &v) const;
            SVector4D<T> operator-() const;
            T operator * (const SVector4D<T> &v) const;
            SVector4D<T> operator * (const T &val) const;
            SVector4D<T> operator / (const T &val) const;
            void reset(const T &val = initVal<T>());
            
            T &operator[](int i);
            const T &operator[](int i) const;
            bool operator<(const SVector4D &v) const;
            bool operator==(const SVector4D &v) const;
        };
        
        template <typename T>
        extern inline SVector4D<T> operator*(const T &val, const SVector4D<T> &v) { return v*val; }
        
        template <typename T, class M = SMemory<T>>
		struct SVector : public Array<T, M> {
            typedef Array<T, M> array;
            
            SVector();
            SVector(size_t s, const T &val = initVal<T>());
            SVector(std::initializer_list<T> li);
            SVector(const SVector<T, M> &v);
            ~SVector();
            
            SVector &operator = (const SVector<T, M> &v);
            SVector &operator += (const SVector<T, M> &v);
            SVector &operator += (const T &val);
            SVector &operator -= (const SVector<T, M> &v);
            SVector &operator -= (const T &val);
            SVector &operator *= (const T &val);
            SVector &operator /= (const T &val);
            SVector operator + (const SVector<T, M> &v) const;
            SVector operator - (const SVector<T, M> &v) const;
            SVector operator - () const;
            SVector operator * (const T &val) const;
            SVector operator / (const T &val) const;
            T operator *(const svec &v) const;

            scyc_iter<T> cycle(int i = 0);
            scyc_citer<T> cycle(int i = 0) const;
        };
        
        template <typename T, class M>
        extern inline SVector<T, M> operator*(const T &val, const SVector<T, M> &v) { return v*val; }
        
        
        #define smat2d SMatrix2D
        #define smat3d SMatrix3D
        #define smat4d SMatrix4D
        #define smat SMatrix
        
#define smatb SMatrix<sbyte, CMemory<sbyte>>
#define smati SMatrix<int, CMemory<int>>
#define smatf SMatrix<float, CMemory<float>>
#define smatd SMatrix<double, CMemory<double>>
        
#define v2imat SMatrix<v2i, RMemory<v2i>>
#define v2fmat SMatrix<v2f, RMemory<v2f>>
#define v2dmat SMatrix<v2d, RMemory<v2d>>
#define v3imat SMatrix<v3i, RMemory<v3i>>
#define v3fmat SMatrix<v3f, RMemory<v3f>>
#define v3dmat SMatrix<v3d, RMemory<v3d>>
#define v4imat SMatrix<v4i, RMemory<v4i>>
#define v4fmat SMatrix<v4f, RMemory<v4f>>
#define v4dmat SMatrix<v4d, RMemory<v4d>>
#define svimat SMatrix<sveci>
#define svfmat SMatrix<svecf>
#define svdmat SMatrix<svecd>
        
#define mat2i SMatrix2D<int>
#define mat2f SMatrix2D<float>
#define mat2d SMatrix2D<double>
#define mat3i SMatrix3D<int>
#define mat3f SMatrix3D<float>
#define mat3d SMatrix3D<double>
#define mat4i SMatrix4D<int>
#define mat4f SMatrix4D<float>
#define mat4d SMatrix4D<double>
        

        template<typename T>
        inline void addMatElement(T *current) { return; }
        template<typename T, class... Args>
        inline void addMatElement(T *current, T &val, Args... args) {
            *current = val; ++current;
            addMatElement(current, args...);
        }
        template<typename T>
        struct SMatrix2D {
            T element[4];
            
            static SMatrix2D uniMat();
            
            SMatrix2D();
            SMatrix2D(const T &t1, const T &t2, const T &t3, const T &t4);
            ~SMatrix2D();
            
            SMatrix2D &operator=(const SMatrix2D &m);
            SMatrix2D &operator += (const SMatrix2D &m);
            SMatrix2D &operator -= (const SMatrix2D &m);
            SMatrix2D &operator *= (const T &val);
            SMatrix2D &operator /= (const T &val);
            SMatrix2D operator + (const SMatrix2D &m) const;
            SMatrix2D operator - (const SMatrix2D &m) const;
            SMatrix2D operator - ();
            SVector2D<T> operator * (const SVector2D<T> &v) const;
            SMatrix2D operator * (const SMatrix2D &m) const;
            SMatrix2D operator * (const T &val) const;
            SMatrix2D operator / (const T &val) const;
            
			T determinant() const;
            T *operator[](int idx);
            const T *operator[](int idx) const;
        };
        
        template<typename T>
        struct SMatrix3D {
            T element[9];
            
            static SMatrix3D uniMat();
            SMatrix3D();
            SMatrix3D(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9);
            ~SMatrix3D();
            
            SMatrix3D &operator=(const SMatrix3D &m);
            SMatrix3D &operator += (const SMatrix3D &m);
            SMatrix3D &operator -= (const SMatrix3D &m);
            SMatrix3D &operator *= (const T &val);
            SMatrix3D &operator /= (const T &val);
            SMatrix3D operator + (const SMatrix3D &m) const;
            SMatrix3D operator - (const SMatrix3D &m) const;
            SMatrix3D operator - ();
            SVector3D<T> operator * (const SVector3D<T> &v) const;
            SMatrix3D operator * (const SMatrix3D &m) const;
            SMatrix3D operator * (const T &val) const;
            SMatrix3D operator / (const T &val) const;
			SMatrix2D<T> cofactor(sint r, sint c) const;
			T determinant() const;
			T *operator[](int idx);
            const T *operator[](int idx) const;
        };
        
        template<typename T>
        struct SMatrix4D {
            T element[16];
            
            static SMatrix4D uniMat();
            SMatrix4D();
            template<typename... Args>
            SMatrix4D(Args... args) : SMatrix4D() { addMatElement<T, Args...>(element, args...); }
            ~SMatrix4D();
            
            SMatrix4D &operator=(const SMatrix4D &m);
            SMatrix4D &operator += (const SMatrix4D &m);
            SMatrix4D &operator -= (const SMatrix4D &m);
            SMatrix4D &operator *= (const T &val);
            SMatrix4D &operator /= (const T &val);
            SMatrix4D operator + (const SMatrix4D &m);
            SMatrix4D operator - (const SMatrix4D &m);
            SMatrix4D operator - ();
            SVector4D<T> operator * (const SVector4D<T> &v);
            SMatrix4D operator * (const SMatrix4D &m);
            SMatrix4D operator * (const T &val);
            SMatrix4D operator / (const T &val);
            
			T determinant() const;
			T *operator[](int idx);
            const T *operator[](int idx) const;
        };
        
        template<typename T, class M = SMemory<T>>
		struct SMatrix : public Array<T, M> {
            typedef Array<T, M> array;
            size_t row, col;
            
            inline int matIndex(int r, int c) const;
            
        public:
            SMatrix();
            SMatrix(int r, int c);
            SMatrix(int r, int c, const T &val);
            SMatrix(int r, int c, std::initializer_list<T> li);
            SMatrix(const SMatrix &mat);
            ~SMatrix();
            
            SMatrix &operator=(const SMatrix &mat);
            SMatrix &operator += (const SMatrix &m);
            SMatrix &operator -= (const SMatrix &m);
            SMatrix &operator *= (const T &val);
            SMatrix &operator /= (const T &val);
            SMatrix operator + (const SMatrix &m);
            SMatrix operator - (const SMatrix &m);
            SMatrix operator - ();
            SVector<T> operator * (const SVector<T> &v);
            SMatrix operator * (const SMatrix &m);
            SMatrix operator * (const T &val);
            SMatrix operator / (const T &val);
            bool comparable(const SMatrix &m);
			T *operator[](int row);
            const T *operator[](int row) const;
            T &at(int row, int col);
            const T &at(int row, int col) const;
            void resize(int row, int col);
			SMatrix inverse() const;
			T determinant() const;
			SMatrix cofactor(sint r, sint c) const;
			SMatrix submat(int roff, int coff, int rnum, int cnum);
            SMatrix transpose();
            bool operator<(const SMatrix<T, M> &mat) const;
            bool operator==(const SMatrix<T, M> &mat) const;
        };
        
        template <typename T, class M>
        extern inline SMatrix<T, M> operator*(const T &val, const SMatrix<T, M> &m) {
            SMatrix<T, M> mat(m.row, m.col); auto p = mat.ptr();
            sforeach(m) { *p = E_ * val; ++p; }
            return mat;
        }
        
        template <typename T, class M>
        extern inline SVector<T, M> operator*(const SVector<T, M> &v, const SMatrix<T, M> &m) {
            SVector<T> arr(m.col());
            sforin(c, 0, m.col()) {
                arr[c] = initVal<T>();
                sforin(r, 0, m.row()) arr[c] += m.value(r, c)*v[r];
            }
            return arr;
        }

        extern inline v2i operator*(const v2i &v, const mat2i &m) {
			return v2i(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
        extern inline v2f operator*(const v2f &v, const mat2f &m) {
			return v2f(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
        extern inline v2d operator*(const v2d & v, const mat2d &m) {
			return v2d(v.x * m[0][0] + v.y * m[1][0], v.x * m[0][1] + v.y * m[1][1]);
		}
        extern inline v3i operator*(const v3i & v, const mat3i &m) {
			return v3i(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
        extern inline v3f operator*(const v3f & v, const mat3f &m) {
			return v3f(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
        extern inline v3d operator*(const v3d & v, const mat3d &m) {
			return v3d(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]);
		}
        extern inline v4i operator*(const v4i & v, const mat4i &m) {
			return v4i(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}
        extern inline v4f operator*(const v4f & v, const mat4f &m) {
			return v4f(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}
        extern inline v4d operator*(const v4d & v, const mat4d &m) {
			return v4d(
				v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + v.w * m[3][0],
				v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + v.w * m[3][1],
				v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + v.w * m[3][2],
				v.x * m[0][3] + v.y * m[1][3] + v.z * m[2][3] + v.w * m[3][3]);
		}
        
        /*============================================================*/
        template <typename T>
		SVector2D<T>::SVector2D(const T &val) : x(val), y(val) {}
        template <typename T>
		SVector2D<T>::SVector2D(const T &x_, const T &y_) : x(x_), y(y_) {}
        template <typename T>
		SVector2D<T>::SVector2D(const SVector2D<T> &v) : SVector2D(v.x, v.y) {}
        template <typename T>
        SVector2D<T>::~SVector2D() {}
        template <typename T>
		SVector2D<T> &SVector2D<T>::operator = (const SVector2D<T> &v) { x = v.x; y = v.y; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator += (const SVector2D<T> &v) { x += v.x; y += v.y; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator += (const T &val) { x += val; y += val; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator -= (const SVector2D<T> &v) { x -= v.x; y -= v.y; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator -= (const T &val) { x -= val; y -= val; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator *= (const T &val) { x *= val; y *= val; return *this; }
        template <typename T>
        SVector2D<T> &SVector2D<T>::operator /= (const T &val) { x /= val; y /= val; return *this; }
        template <typename T>
        SVector2D<T> SVector2D<T>::operator + (const SVector2D<T> &v) const { SVector2D<T> sum = *this; sum += v; return sum; }
        template <typename T>
        SVector2D<T> SVector2D<T>::operator - (const SVector2D<T> &v) const { SVector2D<T> diff = *this; diff -= v; return diff; }
        template <typename T>
        SVector2D<T> SVector2D<T>::operator-() const { SVector2D<T> v(*this); v.x = -v.x; v.y = -v.y; return v; }
        template <typename T>
        SVector2D<T> SVector2D<T>::reflect(subyte r) const { return SVector2D<T>(r&HORIZONTAL?-x:x, r&VERTICAL?-y:y); }
        template <typename T>
        T SVector2D<T>::operator * (const SVector2D<T> &v) const { return x*v.x+y*v.y; }
        template <typename T>
        T SVector2D<T>::cross(const SVector2D<T> &v) const { return x*v.y-y*v.x; }
        template <typename T>
        SVector2D<T> SVector2D<T>::operator * (const T &val) { SVector2D<T> prod = *this; prod *= val; return prod; }
        template <typename T>
        SVector2D<T> SVector2D<T>::operator / (const T &val) { SVector2D<T> div = *this; div /= val; return div; }
        template <typename T>
        SVector2D<T> SVector2D<T>::orthogonal(ROTATION rot) { return rot==CW?SVector2D<T>(y, -x):SVector2D(-y, x); }
        template <typename T>
        void SVector2D<T>::reset(const T &val) { x = val; y = val; }
        template <typename T>
        T &SVector2D<T>::operator[](int i) { return i==0?x:y; }
        template <typename T>
        const T &SVector2D<T>::operator[](int i) const { return i==0?x:y; }
        template <typename T>
        bool SVector2D<T>::operator<(const SVector2D<T> &v) const { return x!=v.x?x<v.x:y<v.y; }
        template <typename T>
        bool SVector2D<T>::operator==(const SVector2D<T> &v) const { return x == v.x && y == v.y; }
        
        
        template <typename T>
        SVector3D<T>::SVector3D(const T &val) : x(val), y(val), z(val) {}
        template <typename T>
        SVector3D<T>::SVector3D(const T &x_, const T &y_, const T &z_) : x(x_), y(y_), z(z_) {}
        template <typename T>
        SVector3D<T>::SVector3D(const SVector2D<T> &v) : x(v.x), y(v.y), z(initVal<T>()) {}
        template <typename T>
        SVector3D<T>::SVector3D(const SVector3D<T> &v) : x(v.x), y(v.y), z(v.z) {}
        template <typename T>
        SVector3D<T>::~SVector3D() {}
        
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator = (const SVector3D<T> &v) { x = v.x; y = v.y; z = v.z; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator += (const SVector3D<T> &v) { x += v.x; y += v.y; z += v.z; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator += (const T &val) { x += val; y += val; z += val; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator -= (const SVector3D<T> &v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator -= (const T &val) { x -= val; y -= val; z -= val; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator *= (const T &val) { x *= val; y *= val; z *= val; return *this; }
        template <typename T>
        SVector3D<T> &SVector3D<T>::operator /= (const T &val) { x /= val; y /= val; z /= val; return *this; }
        template <typename T>
        SVector3D<T> SVector3D<T>::operator + (const SVector3D<T> &v) const { SVector3D<T> sum = *this; sum += v; return sum; }
        template <typename T>
        SVector3D<T> SVector3D<T>::operator - (const SVector3D<T> &v) const { SVector3D<T> diff = *this; diff -= v; return diff; }
        template <typename T>
        SVector3D<T> SVector3D<T>::operator-() const { SVector3D<T> v(*this); v.x = -v.x; v.y = -v.y; v.z = -v.z; return v; }
        template <typename T>
        T SVector3D<T>::operator * (const SVector3D<T> &v) const { return x*v.x+y*v.y+z*v.z; }
        template <typename T>
        SVector3D<T> SVector3D<T>::cross(const SVector3D<T> &v) const { return SVector3D<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
        template <typename T>
        SVector3D<T> SVector3D<T>::operator * (const T &val) const { SVector3D<T> prod = *this; prod *= val; return prod; }
        template <typename T>
        SVector3D<T> SVector3D<T>::operator / (const T &val) const { SVector3D<T> div = *this; div /= val; return div; }
        template <typename T>
        SVector2D<T> SVector3D<T>::projection(PLANE p) const {
            switch (p) {
                case XY_PLANE:
                    return SVector2D<T>(x, y);
                case YZ_PLANE:
                    return SVector2D<T>(y, z);
                default:
                    return SVector2D<T>(x, z);
            }
        }
        template <typename T>
        void SVector3D<T>::reset(const T &val) { x = val; y = val; z = val; }
        template <typename T>
        T &SVector3D<T>::operator[](int i) { return i==0?x:(i==1?y:z); }
        template <typename T>
        const T &SVector3D<T>::operator[](int i) const { return i==0?x:(i==1?y:z); }
        template <typename T>
        bool SVector3D<T>::operator<(const SVector3D &v) const { return x!=v.x?x<v.x:(y!=v.y?y<v.y:z<v.z); }
        template <typename T>
        bool SVector3D<T>::operator==(const SVector3D &v) const { return x == v.x && y == v.y && z == v.z; }
        
        template <typename T>
        SVector4D<T>::SVector4D(const T &val) : x(val), y(val), z(val), w(val) {}
        template <typename T>
        SVector4D<T>::SVector4D(const T &x_, const T &y_, const T &z_, const T &w_) : x(x_), y(y_), z(z_), w(w_) {}
        template <typename T>
        SVector4D<T>::SVector4D(const SVector2D<T> &v) : SVector4D(v.x, v.y, initVal<T>(), initVal<T>()) {}
        template <typename T>
        SVector4D<T>::SVector4D(const SVector3D<T> &v) : SVector4D(v.x, v.y, v.z, initVal<T>()) {}
        template <typename T>
        SVector4D<T>::SVector4D(const SVector4D<T> &v) : SVector4D(v.x, v.y, v.z, v.w) {}
        template <typename T>
        SVector4D<T>::~SVector4D() {}
        
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator = (const SVector4D<T> &v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator += (const SVector4D<T> &v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator += (const T &val) { x += val; y += val; z += val; w += val; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator -= (const SVector4D<T> &v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator -= (const T &val) { x -= val; y -= val; z -= val; w -= val; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator *= (const T &val) { x *= val; y *= val; z *= val; w *= val; return *this; }
        template <typename T>
        SVector4D<T> &SVector4D<T>::operator /= (const T &val) { x /= val; y /= val; z /= val; w /= val; return *this; }
        template <typename T>
        SVector4D<T> SVector4D<T>::operator + (const SVector4D<T> &v) const { SVector4D<T> sum = *this; sum += v; return sum; }
        template <typename T>
        SVector4D<T> SVector4D<T>::operator - (const SVector4D<T> &v) const { SVector4D<T> diff = *this; diff -= v; return diff; }
        template <typename T>
        SVector4D<T> SVector4D<T>::operator-() const { SVector4D<T> v(*this); v.x = -v.x; v.y = -v.y; v.z = -v.z; v.w = -v.w; return v; }
        template <typename T>
        T SVector4D<T>::operator * (const SVector4D<T> &v) const { return x*v.x+y*v.y+z*v.z+w*v.w; }
        template <typename T>
        SVector4D<T> SVector4D<T>::operator * (const T &val) const { SVector4D<T> prod = *this; prod *= val; return prod; }
        template <typename T>
        SVector4D<T> SVector4D<T>::operator / (const T &val) const { SVector4D<T> div = *this; div /= val; return div; }
        template <typename T>
        void SVector4D<T>::reset(const T &val) { x = val; y = val; z = val; w = val; }
        
        template <typename T>
        T &SVector4D<T>::operator[](int i) { return i==0?x:(i==1?y:(i==2?z:w)); }
        template <typename T>
        const T &SVector4D<T>::operator[](int i) const { return i==0?x:(i==1?y:(i==2?z:w)); }
        template <typename T>
        bool SVector4D<T>::operator<(const SVector4D &v) const { return x!=v.x?x<v.x:(y!=v.y?y<v.y:(z!=v.z?z<v.z:w<v.w)); }
        template <typename T>
        bool SVector4D<T>::operator==(const SVector4D &v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        
        template <typename T, class M>
        SVector<T, M>::SVector() : Array<T, M>() {}
        template <typename T, class M>
        SVector<T, M>::SVector(size_t s, const T &val) : Array<T, M>(s, val) {}
        template <typename T, class M>
        SVector<T, M>::SVector(std::initializer_list<T> li) : Array<T, M>(li) {}
        template <typename T, class M>
        SVector<T, M>::SVector(const SVector<T, M> &v) : Array<T, M>(v) {}
        template <typename T, class M>
        SVector<T, M>::~SVector() {}
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator = (const SVector<T, M> &v) {
            if (v.size()) array::copy(v.ptr(), v.size());
            else array::clear();
            return (*this);
        }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator += (const SVector<T, M> &v) {
            if(array::size() != v.size())
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
            auto p = array::ptr(), p_ = v.ptr();
            sforin(i, 0, array::size()) { *p += *p_; ++p; ++p_; }
            return *this;
        }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator += (const T &val) { sforeach(*this) E_ += val; return *this; }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator -= (const SVector<T, M> &v) {
            if(array::size() != v.size())
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
            auto p = v.ptr();
            sforeach(*this) { E_ -= *p; ++p; }
            return *this;
        }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator -= (const T &val) { sforeach(*this) { E_ -= val; } return *this; }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator *= (const T &val) { sforeach(*this) { E_ *= val; } return *this; }
        template <typename T, class M>
        SVector<T, M> &SVector<T, M>::operator /= (const T &val) { sforeach(*this) { E_ /= val; } return *this; }
        template <typename T, class M>
        SVector<T, M> SVector<T, M>::operator + (const SVector<T, M>&v) const { SVector<T, M> sum = *this; sum += v; return sum; }
        template <typename T, class M>
        SVector<T, M> SVector<T, M>::operator - (const SVector<T, M>&v) const { SVector<T, M> diff = *this; diff -= v; return diff; }
        template <typename T, class M>
        SVector<T, M> SVector<T, M>::operator - () const {
            SVector v(*this);
            sforeach(v) E_ = -E_;
            return v;
        }
        template <typename T, class M>
        SVector<T, M> SVector<T, M>::operator * (const T &val) const { SVector<T, M> prod = *this; prod *= val; return prod; }
        template <typename T, class M>
        SVector<T, M> SVector<T, M>::operator / (const T &val) const { SVector<T, M> div = *this; div /= val; return div; }
        template <typename T, class M>
        T SVector<T, M>::operator *(const svec &v) const {
            if (array::size() != v.size() || array::empty())
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this, v", DIMENSION_SIZE_DIFF(array::size(), v.size()));
            T prod = initVal<T>();
            sforeach(*this) prod += E_*E_;
            return prod;
        }
        template <typename T, class M>
        scyc_iter<T> SVector<T, M>::cycle(int i) { return scyc_iter<T>(array::ptr(i), i, array::size()); }
        template <typename T, class M>
        scyc_citer<T> SVector<T, M>::cycle(int i) const { return scyc_citer<T>(aarrayrr::ptr(i), i, array::size()); }
        
        
        
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::uniMat() {
            return SMatrix2D(unitVal<T>(), initVal<T>(), initVal<T>(), unitVal<T>());
        }
        template <typename T>
        SMatrix2D<T>::SMatrix2D() { auto p = element; sforin(i, 0, 4) { *p = initVal<T>(); ++p; } }
        template <typename T>
        SMatrix2D<T>::SMatrix2D(const T &t1, const T &t2, const T &t3, const T &t4) {
            element[0] = t1; element[1] = t2; element[2] = t3; element[3] = t4;
        }
        template <typename T>
        SMatrix2D<T>::~SMatrix2D() {}
        
        template <typename T>
        SMatrix2D<T> &SMatrix2D<T>::operator=(const SMatrix2D<T> &m) {
            auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p = *p_; ++p; ++p_; }
            return *this;
        }
        template <typename T>
        SMatrix2D<T> &SMatrix2D<T>::operator += (const SMatrix2D<T> &m) {
            auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p += *p_; ++p; ++p_; }
            return *this;
        }
        template <typename T>
        SMatrix2D<T> &SMatrix2D<T>::operator -= (const SMatrix2D<T> &m) {
            auto p = element; auto p_ = m.element; sforin(i, 0, 4) { *p -= *p_; ++p; ++p_; }
            return *this;
        }
        template <typename T>
        SMatrix2D<T> &SMatrix2D<T>::operator *= (const T &val) {
            auto p = element; sforin(i, 0, 4) { *p *= val; ++p; }
            return *this;
        }
        template <typename T>
        SMatrix2D<T> &SMatrix2D<T>::operator /= (const T &val) {
            auto p = element; sforin(i, 0, 4) { *p /= val; ++p; }
            return *this;
        }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator + (const SMatrix2D<T> &m) const { return SMatrix2D<T>(*this) += m; }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator - (const SMatrix2D<T> &m) const { return SMatrix2D<T>(*this) -= m; }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator - () { SMatrix2D<T> m(*this); sforin(i, 0, 4) m.element[i] = -m.element[i]; return m; }
        template <typename T>
        SVector2D<T> SMatrix2D<T>::operator * (const SVector2D<T> &v) const {
            SVector2D<T> res;
            auto p = element;
            res.x = (*p)*v.x; ++p;
            res.x += (*p)*v.y; ++p;
            res.y = (*p)*v.x; ++p;
            res.y += (*p)*v.y;
            return res;
        }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator * (const SMatrix2D<T> &m) const {
            SMatrix2D res;
            sforin(r, 0, 2) {
                sforin(c, 0, 2) {
                    sforin(v, 0, 2) res[r][c] += element[2*r+c]*m[v][c];
                }
            }
            return res;
        }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator * (const T &val) const { return SMatrix2D<T>(*this) *= val; }
        template <typename T>
        SMatrix2D<T> SMatrix2D<T>::operator / (const T &val) const { return SMatrix2D<T>(*this) /= val; }
		template <typename T>
		T SMatrix2D<T>::determinant() const {
			return element[0] * element[3] - element[1] * element[2];
		}
        template <typename T>
        T *SMatrix2D<T>::operator[](int idx) { return &element[idx*2]; }
        template <typename T>
        const T *SMatrix2D<T>::operator[](int idx) const { return &element[idx*2]; }
        
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::uniMat() {
            return SMatrix3D(unitVal<T>(), initVal<T>(), initVal<T>(),
                                 initVal<T>(), unitVal<T>(), initVal<T>(),
                                 initVal<T>(), initVal<T>(), unitVal<T>());
        }
        template <typename T>
        SMatrix3D<T>::SMatrix3D() { auto p = element; sforin(i, 0, 9) *p = initVal<T>(); }
        template <typename T>
        SMatrix3D<T>::SMatrix3D(T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9) : SMatrix3D() {
            element[0] = t1; element[1] = t2; element[2] = t3;
            element[3] = t4; element[4] = t5; element[5] = t6;
            element[6] = t7; element[7] = t8; element[8] = t9;
        }
        template <typename T>
        SMatrix3D<T>::~SMatrix3D() {}
        
        template <typename T>
        SMatrix3D<T> &SMatrix3D<T>::operator=(const SMatrix3D<T> &m) {
            auto p = element; auto p_ = m.element;
            sforin(i, 0, 9) *p = *p_;
        }
        template <typename T>
        SMatrix3D<T> &SMatrix3D<T>::operator += (const SMatrix3D<T> &m) {
            
            sforin(i, 0, 9) element[i] += m.element[i];
            return *this;
        }
        template <typename T>
        SMatrix3D<T> &SMatrix3D<T>::operator -= (const SMatrix3D<T> &m) {
            sforin(i, 0, 4) element[i] -= m.element[i];
            return *this;
        }
        template <typename T>
        SMatrix3D<T> &SMatrix3D<T>::operator *= (const T &val) {
            sforin(i, 0, 4) element[i]*=val;
            return *this;
        }
        template <typename T>
        SMatrix3D<T> &SMatrix3D<T>::operator /= (const T &val) {
            sforin(i, 0, 4) element[i]/=val;
            return *this;
        }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator + (const SMatrix3D<T> &m) const { SMatrix3D<T> mat = *this; mat += m; return mat; }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator - (const SMatrix3D<T> &m) const { SMatrix3D<T> mat = *this; mat -= m; return mat; }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator - () { SMatrix3D<T> m(*this); sforin(i, 0, 9) m.element[i] = -m.element[i]; return m; }
        template <typename T>
        SVector3D<T> SMatrix3D<T>::operator * (const SVector3D<T> &v) const {
            SVector3D<T> res;
            sforin(r, 0, 3) {
                sforin(c, 0, 3) res[r] += element[3*r+c]*v[c];
            }
            return res;
        }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator * (const SMatrix3D<T> &m) const {
            SMatrix3D res;
            sforin(r, 0, 3) {
                sforin(c, 0, 3) {
                    sforin(v, 0, 3) res[r][c] += element[3*r+c]*m[v][c];
                }
            }
            return res;
        }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator * (const T &val) const { SMatrix3D<T> prod = *this; prod *= val; return prod; }
        template <typename T>
        SMatrix3D<T> SMatrix3D<T>::operator / (const T &val) const { SMatrix3D<T> div = *this; div /= val; return div; }
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
        T *SMatrix3D<T>::operator[](int idx) { return &element[idx*3]; }
        template <typename T>
        const T *SMatrix3D<T>::operator[](int idx) const { return &element[idx*3]; }
        
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::uniMat() {
            return SMatrix4D<T>(unitVal<T>(), initVal<T>(), initVal<T>(), initVal<T>(),
                                 initVal<T>(), unitVal<T>(), initVal<T>(), initVal<T>(),
                                 initVal<T>(), initVal<T>(), unitVal<T>(), initVal<T>(),
                                 initVal<T>(), initVal<T>(), initVal<T>(), unitVal<T>());
        }
        template <typename T>
        SMatrix4D<T>::SMatrix4D() { sforin(i, 0, 16) element[i] = initVal<T>(); }
        template <typename T>
        SMatrix4D<T>::~SMatrix4D() {}
        template <typename T>
        SMatrix4D<T> &SMatrix4D<T>::operator=(const SMatrix4D<T> &m) {
            sforin(i, 0, 4) element[i] = m[i];
        }
        template <typename T>
        SMatrix4D<T> &SMatrix4D<T>::operator += (const SMatrix4D<T> &m) {
            sforin(i, 0, 4) element[i] += m.element[i];
            return *this;
        }
        template <typename T>
        SMatrix4D<T> &SMatrix4D<T>::operator -= (const SMatrix4D<T> &m) {
            sforin(i, 0, 4) element[i] -= m.element[i];
            return *this;
        }
        template <typename T>
        SMatrix4D<T> &SMatrix4D<T>::operator *= (const T &val) {
            sforin(i, 0, 4) element[i]*=val;
            return *this;
        }
        template <typename T>
        SMatrix4D<T> &SMatrix4D<T>::operator /= (const T &val) {
            sforin(i, 0, 4) element[i]/=val;
            return *this;
        }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator + (const SMatrix4D<T> &m) { SMatrix4D<T> mat = *this; mat += m; return mat; }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator - (const SMatrix4D<T> &m) { SMatrix4D<T> mat = *this; mat -= m; return mat; }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator - () { SMatrix4D<T> m(*this); sforin(i, 0, 16) m.element[i] = -m.element[i]; return m; }
        template <typename T>
        SVector4D<T> SMatrix4D<T>::operator * (const SVector4D<T> &v) {
            SVector4D<T> res;
            sforin(r, 0, 4) {
                sforin(c, 0, 4) res[r] += element[4*r+c]*v[c];
            }
            return res;
        }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator * (const SMatrix4D<T> &m) {
            SMatrix4D<T> res;
            sforin(r, 0, 4) {
                sforin(c, 0, 4) {
                    sforin(v, 0, 4) res[r][c] += element[4*r+c]*m[v][c];
                }
            }
            return res;
        }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator * (const T &val) { SMatrix4D<T> prod = *this; prod *= val; return prod; }
        template <typename T>
        SMatrix4D<T> SMatrix4D<T>::operator / (const T &val) { SMatrix4D<T> div = *this; div /= val; return div; }
		template <typename T>
		T SMatrix4D<T>::determinant() const {



		}
		template <typename T>
        T *SMatrix4D<T>::operator[](int idx) { return &element[idx*4]; }
        template <typename T>
        const T *SMatrix4D<T>::operator[](int idx) const { return &element[idx*4]; }
        
        template<typename T, class M>
        inline int SMatrix<T, M>::matIndex(int r, int c) const { return col*r+c; }
        
        template<typename T, class M>
        SMatrix<T, M>::SMatrix() : row(0), col(0), array() {}
        template<typename T, class M>
        SMatrix<T, M>::SMatrix(int r, int c) : row(r), col(c), array(r*c) {}
        template<typename T, class M>
        SMatrix<T, M>::SMatrix(int r, int c, const T &val) : row(r), col(c), array(r*c, val) {}
        template<typename T, class M>
        SMatrix<T, M>::SMatrix(int r, int c, std::initializer_list<T> li) : row(r), col(c), array(li) {}
        template<typename T, class M>
        SMatrix<T, M>::SMatrix(const SMatrix<T, M> &mat) : SMatrix<T, M>(mat.row, mat.col) { array::copy(mat.ptr(), mat.size()); }
        template<typename T, class M>
        SMatrix<T, M>::~SMatrix() {}
        
        template<typename T, class M>
        SMatrix<T, M> &SMatrix<T, M>::operator=(const SMatrix<T, M> &mat) {
            array::clear(); row = mat.row; col = mat.col;
            array::copy(mat.ptr(), mat.size()); return *this;
        }
        template<typename T, class M>
        SMatrix<T, M> &SMatrix<T, M>::operator += (const SMatrix<T, M> &m) {
            if (!comparable(m))
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this, m", "");
            auto p = m.aoffset; sforeach(*this) { E_ += *p; ++p; } return *this;
        }
        template<typename T, class M>
        SMatrix<T, M> &SMatrix<T, M>::operator -= (const SMatrix<T, M> &m) {
            if (!comparable(m))
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this, m", "");
            auto p = m.aoffset; sforeach(*this) { E_ -= *p; ++p; } return *this;
        }
        template<typename T, class M>
        SMatrix<T, M> &SMatrix<T, M>::operator *= (const T &val) { sforeach(*this)  E_*=val;return *this; }
        template<typename T, class M>
        SMatrix<T, M> &SMatrix<T, M>::operator /= (const T &val) { sforeach(*this) E_/=val; return *this; }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator + (const SMatrix<T, M> &m) { smat mat = *this; mat += m; return mat; }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator - (const SMatrix<T, M> &m) { smat mat = *this; mat -= m; return mat; }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator - () { SMatrix<T, M> m(*this); sforeach(*this) E_ = -E_; return m; }
        template<typename T, class M>
        SVector<T> SMatrix<T, M>::operator * (const SVector<T> &v) {
            if (col != v.size())
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "this.col, v.size", DIMENSION_SIZE_DIFF(col, v.size()));
            svec<T> arr(row);
            sforin(r, 0,row) {
                sforin(c, 0, col) arr[r] += array::at(matIndex(r, c))*v[c];
            }
            return arr;
        }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator *(const SMatrix<T, M> &m) {
            if (col != m.row)
                throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR ,"matrix", "m.row");
            smat m_;
            sforin(r, 0, row) {
                sforin(c, 0, m.col) {
                    sforin(v, 0, col) m_[r][c] += array::at(matIndex(r, c))*m[v][c];
                }
            }
            return m_;
        }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator * (const T &val) { SMatrix<T, M> prod = *this; prod *= val; return prod; }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::operator / (const T &val) { SMatrix<T, M> div = *this; div /= val; return div; }
        template<typename T, class M>
        bool SMatrix<T, M>::comparable(const SMatrix &m) { return col == m.col && row == m.row; }
        template<typename T, class M>
        T *SMatrix<T, M>::operator[](int row) { return array::ptr(matIndex(row, 0)); }
        template<typename T, class M>
        const T *SMatrix<T, M>::operator[](int row) const { return array::ptr(matIndex(row, 0)); }
        template<typename T, class M>
        T &SMatrix<T, M>::at(int row, int col) { return array::at(matIndex(row, col)); }
        template<typename T, class M>
        const T &SMatrix<T, M>::at(int row, int col) const { return array::at(matIndex(row, col)); }
        template<typename T, class M>
        void SMatrix<T, M>::resize(int row, int col) {
            auto tmp = smat(row, col);
            if (row && col) {
                sforin(r, 0, row<row?row:row) {
                    sforin(c, 0, col<col?col:col) tmp[r][c] = at(r, c);
                }
            }
            array::swap(tmp);
            row = row; col = col;
        }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::submat(int roff, int coff, int rnum, int cnum) {
            smat m(rnum, cnum);
            sforin(r, 0, rnum) { sforin(c, 0, cnum) m.at(r, c) = at(roff+r, coff+c); }
            return m;
        }
        template<typename T, class M>
        SMatrix<T, M> SMatrix<T, M>::transpose() {
            smat m(col, row);
            sforin(r, 0, row) { sforin(c, 0, col) m[c][r] = at(r, c); }
            return m;
        }
        template<typename T, class M>
        bool SMatrix<T, M>::operator<(const SMatrix<T, M> &mat) const {
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
        bool SMatrix<T, M>::operator==(const SMatrix<T, M> &mat) const {
            if (row != mat.row || col != mat.col) return false;
            auto p = array::ptr(), p_ = mat.ptr();
            sforeachi(*this) {
                if (*p != *p_) return false;
                ++p; ++p_;
            }
            return true;
        }

		template<typename T, class M>
		extern inline void JacobiMethod(SMatrix<T, M> &mat, smatd& evec, svecd &evalue, sint iter = 100, double threshold = 1e-10) {
			sint lcount = 0;
			
			while (lcount < iter) {
				auto loop = false;
				sforin(r, 0, mat.row) {
					sforin(auto c, 0, mat.col) {
						
						
					
					}
				}
				if (!loop) break;
				++lcount;
			}
		}
		//QR method
		template<typename T, class M>
		extern inline void eigen(SMatrix<T, M>& mat, smatd& evec, svecd& evalue) {




		}

    }
}
#endif  