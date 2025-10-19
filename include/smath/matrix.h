#ifndef SMATH_MATRIX_H
#define SMATH_MATRIX_H
#include "smath/vector.h"
template<typename T, size_t S>
inline void initMatND(T* e) {}
template<typename T, size_t S, class V, class... Remain>
inline void initMatND(T* e, const V& v, const Remain&... r) { *e = v; if (0 < S) { initMatND<T, S - 1>(e + 1, r...); } }
namespace slib {
	namespace smath {
		template<typename T, size_t D>
		class MatrixND {
		public:
			T elements[D * D];
		public:
			MatrixND() { memset(elements, 0, D * D * sizeof(T)); }
			template<class... Args>
			MatrixND(const Args&... args) { initMatND<T, (D * D)>(elements, args...); }
			MatrixND(const MatrixND<T, D>& m) {
				sforin(d, 0_u, D * D) { elements[d] = m.elements[d]; }
			}
			~MatrixND() {}

			static MatrixND unitMat() {
				auto mat = MatrixND<T, D>();
				sforin(d, 0_u, D) { mat.elements[(d + 1) * (d + 1) - 1] = 1; }
				return mat;
			}
			MatrixND& operator=(const MatrixND<T, D>& m) {
				sforin(d, 0_u, D * D) { elements[d] = m.elements[d]; }
				return *this;
			}
			MatrixND& operator+=(const MatrixND<T, D>& m) {
				sforin(d, 0_u, D * D) { elements[d] += m.elements[d]; }
				return *this;
			}
			MatrixND& operator-=(const MatrixND<T, D>& m) {
				sforin(d, 0_u, D * D) { elements[d] -= m.elements[d]; }
				return *this;
			}
			MatrixND& operator*=(const T val) {
				sforin(d, 0_u, D * D) { elements[d] *= val; }
				return *this;
			}
			MatrixND& operator*=(const MatrixND<T, D>& m) {
				sforin(d, 0_u, D * D) { elements[d] *= m.elements[d]; }
				return *this;
			}
			MatrixND& operator/=(const T val) {
				sforin(d, 0_u, D * D) { elements[d] /= val; }
				return *this;
			}
			MatrixND<T, D> operator + (const MatrixND<T, D>& m) const {
				MatrixND<T, D> mat(*this);
				sforin(d, 0_u, D * D) { mat.elements[d] += m.elements[d]; }
				return mat;
			}
			MatrixND<T, D> operator - (const MatrixND<T, D>& m) const {
				MatrixND<T, D> mat(*this);
				sforin(d, 0_u, D * D) { mat.elements[d] -= m.elements[d]; }
				return mat;
			}
			MatrixND<T, D> operator - () {
				MatrixND<T, D> mat(*this);
				sforin(d, 0_u, D * D) { mat.elements[d] = -mat.elements[d]; }
				return mat;
			}
			VectorND<T, D> operator * (const VectorND<T, D>& v) const {
				VectorND<T, D> vec;
				auto p1 = elements; auto p2 = v.elements;
				auto p3 = vec.elements;
				sforin(r, 0_u, D) {
					p2 = v.elements;
					sforin(c, 0_u, D) { (*p3) += (*p1) * (*p2); ++p1; ++p2; }
					++p3;
				}
				return vec;
			}
			MatrixND<T, D> operator * (const MatrixND<T, D>& m) const {
				MatrixND<T, D> mat;
				auto p1 = elements, p2 = m.elements;
				auto p3 = mat.elements;
				sforin(r, 0_u, D) {
					sforin(c, 0_u, D) {
						p2 = &m.elements[c];
						sforin(r_, 0_u, D) { (*p3) += p1[r_] * (*p2); p2 += D; }
						++p3;
					}
					p1 += D;
				}
				return mat;
			}
			MatrixND<T, D> operator * (const T val) const {
				MatrixND<T, D> mat(*this);
				sforin(d, 0_u, D * D) { mat.elements[d] *= val; }
				return mat;
			}
			MatrixND<T, D> operator / (const T val) const {
				MatrixND<T, D> mat(*this);
				sforin(d, 0_u, D * D) { mat.elements[d] /= val; }
				return mat;
			}
			MatrixND<T, D> transpose() const {
				MatrixND<T, D> mat;
				sforin(i, 0_u, D * D) {
					mat.elements[i] = elements[(i % D) * D + (i / D)];
				}
				return mat;
			}
			MatrixND<T, D> inverse() const {
				auto det = determinant();
				if (det == 0) throw DivZeroException("The determinant of the matrix is zero. Inverse matrix cannot be defined.");
				if (D == 2) return MatrixND<T, D>(elements[3], -elements[1], -elements[2], elements[0]) / det;
				else {
					MatrixND<T, D> inv;
					auto p = &inv.elements[0];
					sforin(r, 0_u, D) {
						sforin(c, 0_u, D) { *p = ((r + c) % 2 ? -1 : 1) * cofactor(r, c).determinant(); ++p; }
					}
					return inv.transpose() / det;
				}
			}
			MatrixND<T, D - 1> cofactor(const size_t r, const size_t c) const {
				MatrixND<T, D - 1> mat;
				auto p1 = elements;
				auto p2 = mat.elements;
				sforin(r_, 0_u, r) {
					sforin(c_, 0_u, c) { (*p2) = (*p1); ++p1; ++p2; }
					++p1;
					sforin(c_, c + 1, D) { (*p2) = (*p1); ++p1; ++p2; }
				}
				p1 += D;
				sforin(r_, r + 1, D) {
					sforin(c_, 0_u, c) { (*p2) = (*p1); ++p1; ++p2; }
					++p1;
					sforin(c_, c + 1, D) { (*p2) = (*p1); ++p1; ++p2; }
				}
				return mat;
			}
			T determinant() const {
				if (D == 2) return elements[0] * elements[3] - elements[1] * elements[2];
				else if (D == 3) return elements[0] * elements[4] * elements[8] + elements[1] * elements[5] * elements[6] + elements[2] * elements[3] * elements[7]
					- elements[2] * elements[4] * elements[6] - elements[1] * elements[3] * elements[8] - elements[0] * elements[5] * elements[7];
				else {
					T det = 0;
					auto p = elements;
					sforin(i, 0_u, D) { det += ((i % 2 ? -1 : 1) * (*p) * cofactor(i, 0).determinant()); p += D; }
					return det;
				}
			}
			T* operator[](const int idx) { return &elements[(idx < 0 ? (int)D + idx : idx) * D]; }
			const T* operator[](int idx) const { return &elements[(idx < 0 ? (int)D + idx : idx) * D]; }
			bool operator == (const MatrixND<T, D>& m) const { return memcmp(elements, m.elements) == 0; }
			bool operator != (const MatrixND<T, D>& m) const { return !(*this == m); }
		};
		template<typename T>
		using Matrix2D = MatrixND<T, 2>;
		template<typename T>
		using Matrix3D = MatrixND<T, 3>;
		template<typename T>
		using Matrix4D = MatrixND<T, 4>;

		template <typename T, size_t D>
		extern VectorND<T, D> operator*(const VectorND<T, D>& v, const MatrixND<T, D>& m) {

		}

		template<typename T>
		class Matrix : public Vector<T> {
			typedef Vector<T> vector;
		public:
			size_t row;
			size_t col;
		private:
			size_t _index(const int r, const int c) const {
				return (size_t)(col * (r < 0 ? (int)row + r : r) + (c < 0 ? (int)col + c : c));
			}
		public:
			Matrix() : Vector<T>(), row(0), col(0) {}
			Matrix(const size_t r, const size_t c) : Matrix() { resize(r, c); }
			Matrix(const size_t r, const size_t c, const T& val) : Matrix() { resize(r, c); }
			Matrix(const size_t r, const size_t c, std::initializer_list<T> li) : Matrix() {
				resize(r, c);
				if (li.size() != Vector<T>::values.size()) throw ShapeMismatchException("");
				sfor2(Vector<T>::values, li) $_1 = $_2;
			}
			Matrix(Matrix<T>&& mat) noexcept { vector::swap(mat); }
			Matrix(const Matrix<T>& mat) : Matrix() {
				resize(mat.row, mat.col);
				sfor2(Vector<T>::values, mat.values) $_1 = $_2;
			}
			~Matrix() {}
			Matrix<T>& operator=(const Matrix<T>& mat) {
				resize(mat.row, mat.col);
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(Vector<T>::values, mat.values) $_1 = $_2;
				return *this;
			}
			Matrix<T>& operator += (const Matrix<T>& mat) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(Vector<T>::values, mat.values) $_1 += $_2;
				return *this;
			}
			Matrix<T>& operator -= (const Matrix<T>& mat) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(Vector<T>::values, mat.values) $_1 -= $_2;
				return *this;
			}
			Matrix<T>& operator *= (const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(Vector<T>::values) $_ *= val;
				return *this;
			}
			Matrix<T>& operator /= (const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(Vector<T>::values) $_ /= val;
				return *this;
			}
			Matrix<T> operator + (const Matrix<T>& mat) { return Matrix<T>(*this) += mat; }
			Matrix<T> operator - (const Matrix<T>& mat) { return Matrix<T>(*this) -= mat; }
			Matrix<T> operator - () {
				Matrix<T> mat(row, col);
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(Vector<T>::values, mat.values) $_2 = -$_1;
				return *this;
			}
			Vector<T> operator * (const Vector<T>& v) {
				//if (col != v.size())
				//	throw slib::SMathException(slib::DIMENSION_SIZE_ERR);
				Vector<T> vec(row, 0);
				auto p1 = vector::data(); auto p2 = v.data(); auto p3 = vec.data();
				sforin(r, 0_u, row) {
					p2 = v.data();
					sforin(c, 0_u, col) { (*p3) += (*p1) * (*p2); ++p1; ++p2; }
					++p3;
				}
				return vec;
			}
			Matrix<T> operator * (const Matrix& m) {
				//if (col != m.row)
				//	throw slib::SMathException(slib::DIMENSION_SIZE_ERR);
				Matrix<T> mat(row, m.col, 0);
				auto p1 = vector::data(), p1_ = p1; auto p2 = m.data(); auto p3 = mat.data();
				sforin(r, 0, (int)row) {
					sforin(c, 0, (int)col) {
						p1_ = p1;
						p2 = &m[0][c];
						sforin(r_, 0, row) { (*p3) += (*p1_) * (*p2); ++p1_; p2 += m.col; }
						++p3;
					}
					p1 += row;
				}
				return mat;
			}
			Matrix<T> operator * (const T val) { return Matrix<T>(*this) *= val; }
			Matrix<T> operator / (const T val) { return Matrix<T>(*this) /= val; }
			void resize(const size_t r, const size_t c) {
				if (r == 0 || c == 0) {
					Vector<T>::values.clear();
					row = r; col = c;
				}
				else if (MAX_ARRAY_CAPACITY <= r || 
					(MAX_ARRAY_CAPACITY / r) <= c) throw OverFlowException(overflowErrorText("Matrix size (row * col)", MAX_ARRAY_CAPACITY));
				if (row == 0 || col == 0) {
					row = r; col = c; vector::values.resize(row * col);
				}
				else if (row * col == r * c) { row = r; col = c; }
				else {
					Matrix<T> tmp(r, c);
					sforin(i, 0, (int)(row < r ? row : r)) {
						sforin(j, 0, (int)(col < c ? col : c)) tmp[i][j] = at(i, j);
					}
					swap(tmp);
				}
			}
			void swap(Matrix& mat) {
				Memory<size_t>::swap(&row, &mat.row);
				Memory<size_t>::swap(&col, &mat.col);
				vector::swap(mat);
			}
			T* operator[](const int idx) { return vector::data() + _index(idx, 0); }
			const T* operator[](const int idx) const { return vector::data() + _index(idx, 0); }
			T& at(const int r, const int c) { return vector::at((int)_index(r, c)); }
			const T& at(const int r, const int c) const { return vector::at(_index(r, c)); }
			ArrayIterator<T> begin() { return vector::begin(); }
			ArrayCIterator<T> begin() const { return vector::begin(); }
			ArrayIterator<T> end() { return vector::end(); }
			ArrayCIterator<T> end() const { return vector::end(); }
			Matrix submat(const int r, const int c, const int rn, const int cn) const;
			//Matrix submat(const sarea& a) const;
			T determinant() const;
			Matrix inverse() const;
			Matrix transpose() const;
			Matrix cofactor(const sinteger& r, const sinteger& c) const;

			void rsum(const size_t idx1, const size_t idx2) {
				auto p1 = vector::ptr(idx1 * col), p2 = vector::ptr(idx2 * col);
				sforin(c, 0_u, col) { (*p1) += (*p2); ++p1; ++p2; }
			}
			void csum(const size_t idx1, const size_t idx2) {
				auto p1 = vector::ptr(idx1), p2 = vector::ptr(idx2);
				sforin(r, 0_u, row) { (*p1) += (*p2); p1 += col; p2 += col; }
			}
			void sum(const size_t idx1, const size_t idx2, DIRECTION dir) {
				if (dir == DIRECTION::COLUMN) csum(idx1, idx2);
				else rsum(idx1, idx2);
			}
			void rsubtract(const size_t idx1, const size_t idx2) {
				auto p1 = vector::ptr(idx1 * col), p2 = vector::ptr(idx2 * col);
				sforin(c, 0_u, col) { (*p1) -= (*p2); ++p1; ++p2; }
			}
			void csubtract(const size_t idx1, const size_t idx2) {
				auto p1 = vector::ptr(idx1), p2 = vector::ptr(idx2);
				sforin(r, 0_u, row) { (*p1) -= (*p2); p1 += col; p2 += col; }
			}
			void subtract(const size_t idx1, const size_t idx2, DIRECTION dir) {
				if (dir == DIRECTION::COLUMN) csubtract(idx1, idx2);
				else rsubtract(idx1, idx2);
			}
			void rtimes(const size_t idx, const T& ratio) {
				auto p = vector::ptr(idx * col);
				sforin(c, 0_u, col) { (*p) *= ratio; ++p; }
			}
			void ctimes(const size_t idx, const T& ratio) {
				auto p = vector::ptr(idx);
				sforin(r, 0_u, row) { (*p) *= ratio; p += col; }
			}
			void times(const size_t idx, const T& ratio, DIRECTION dir) {
				if (dir == DIRECTION::COLUMN) ctime(idx, ratio);
				else rtime(idx, ratio);
			}
			void rswap(const size_t idx1, const size_t idx2) {
				if (idx1 == idx2) return;
				T tmp, * p1 = vector::ptr(idx1 * col), * p2 = vector::ptr(idx2 * col);
				sforin(c, 0_u, col) {
					tmp = *p1; *p1 = *p2; *p2 = tmp;
					++p1; ++p2;
				}
			}
			void cswap(const size_t idx1, const size_t idx2) {
				if (idx1 == idx2) return;
				T tmp, * p1 = vector::ptr(idx1), * p2 = vector::ptr(idx2);
				sforin(r, 0_u, row) {
					tmp = *p1; *p1 = *p2; *p2 = tmp;
					p1 += col; p2 += col;
				}
			}
			void rsweep(const size_t idx1, const size_t idx2, const T& ratio);
			void csweep(const size_t idx1, const size_t idx2, const T& ratio);

			bool operator<(const Matrix<T>& mat) const {
				if (row != mat.row || col != mat.col) return vector::size() < mat.size();
				sfor2(*this, mat) {
					if ($_1 != $_2) return $_1 < $_2;
				}
				return false;
			}
			bool operator==(const Matrix<T>& mat) const {
				if (row != mat.row || col != mat.col) return false;
				sfor2(*this, mat) {
					if ($_1 != $_2) return false;
				}
				return true;
			}
		};

		template<typename T>
		extern inline void toMat(Matrix<T> &mat, const Vector<Vector<T>>& vecs, T na = (T)snum::NaN) {
			auto maxcount = 0;
			sfor(vecs) {
				if (maxcount < $_.size()) maxcount = (int)$_.size();
			}
			mat = Matrix<T>(maxcount, vecs.size(), na);
			sforin(c, 0_u, mat.col) {
				auto p = &mat[0][c];
				sfor(vecs[c]) { *p = $_; p += mat.col; }
			}
		}
	}
	template<typename T>
	extern std::ostream& operator<<(std::ostream& os, const slib::smath::Matrix<T>& mat) {
		auto maxlen = 0;
		sfor(mat) {
			if (maxlen < String($_).size()) maxlen = String($_).size();
		}
		++maxlen;
		sforin(r, 0_u, mat.row) {
			sforin(c, 0_u, mat.col) { os << sstr::lfill(String(mat[r][c]), ' ', maxlen); }
			os << NL;
		}
		return os;
	}

	template<typename T>
	extern String toString(const smath::Matrix<T>& mat, const char *format = nullptr) {
		slib::String str;
		if (!format) {
			size_t maxlen = 0;
			sfor(mat) {
				if (maxlen < String($_).size()) maxlen = String($_).size();
			}
			++maxlen;
			sforin(r, 0_u, mat.row) {
				sforin(c, 0_u, mat.col) { str << sstr::lfill(String(mat[r][c]), ' ', maxlen); }
				str << NL;
			}
		}
		else {
			slib::String f(format), sep = "";
			if (f == "csv") sep = ",";
			else sep = "\t";
			auto p = &mat[0][0];
			sforin(r, 0_u, mat.row) {
				sforin(c, 0_u, mat.col) { str << mat[r][c] << sep; }
				str.resize(str.size() - 1);
				str << NL;
			}
		}
		return str;
	}

	using smat2b = smath::Matrix2D<subyte>;
	using smat2i = smath::Matrix2D<sint>;
	using smat2l = smath::Matrix2D<sinteger>;
	using smat2f = smath::Matrix2D<float>;
	using smat2d = smath::Matrix2D<double>;
	using smat3b = smath::Matrix3D<subyte>;
	using smat3i = smath::Matrix3D<sint>;
	using smat3l = smath::Matrix3D<sinteger>;
	using smat3f = smath::Matrix3D<float>;
	using smat3d = smath::Matrix3D<double>;
	using smat4b = smath::Matrix4D<subyte>;
	using smat4i = smath::Matrix4D<sint>;
	using smat4l = smath::Matrix4D<sinteger>;
	using smat4f = smath::Matrix4D<float>;
	using smat4d = smath::Matrix4D<double>;
	template<typename T>
	using smat = smath::Matrix<T>;
	using smatb = smath::Matrix<subyte>;
	using smati = smath::Matrix<sint>;
	using smatl = smath::Matrix<sinteger>;
	using smatf = smath::Matrix<float>;
	using smatd = smath::Matrix<double>;
}

#endif