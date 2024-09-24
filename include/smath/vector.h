#ifndef SMATH_VECTOR_H
#define SMATH_VECTOR_H
#include "sbasic/cyciter.h"
#include "sbasic/array.h"
#include "sbasic/string.h"
#include "smath/calc.h"

template<typename T, size_t D>
inline void initVecND(T* e) {}
template<typename T, size_t D, class V, class... Remain>
inline void initVecND(T* e, V v, Remain... r) { *e = (T)v; if (0 < D) { initVecND<T, D - 1, Remain...>(e + 1, r...); } }

namespace slib {
	namespace smath {
		template<typename T, size_t D>
		class VectorND {
		public:
			T elements[D];
			
		public:
			VectorND() { memset(elements, 0, D * sizeof(T)); }
			template<class... Args>
			VectorND(const Args... args) : VectorND() { initVecND<T, D, Args...>(&elements[0], args...); }
			VectorND(const VectorND& vec) : VectorND() { memcpy(elements, vec.elements, D * sizeof(T)); }
			template<typename S>
			VectorND(const VectorND<S, D>& vec) : VectorND() { sforin(i, 0_u, D) elements[i] = (T)vec[i]; }
			~VectorND() {}
			VectorND<T, D>& operator=(const VectorND<T, D>& v) { memcpy(elements, v.elements, D * sizeof(T)); return *this; }
			VectorND<T, D>& operator+=(const VectorND<T, D>& v) {
				auto p1 = elements;
				auto p2 = v.elements;
				sforin(d, 0_u, D) { (*p1) += (*p2); ++p1; ++p2; }
				return *this;
			}
			VectorND<T, D>& operator+=(const T val) {
				auto p = elements;
				sforin(d, 0_u, D) { (*p) += val; ++p; }
				return *this;
			}
			VectorND<T, D>& operator-=(const VectorND<T, D>& v) {
				auto p1 = elements;
				auto p2 = v.elements;
				sforin(d, 0_u, D) { (*p1) -= (*p2); ++p1; ++p2; }
				return *this;
			}
			VectorND<T, D>& operator-=(const T val) {
				auto p = elements;
				sforin(d, 0_u, D) { (*p) -= val; ++p; }
				return *this;
			}
			VectorND<T, D>& operator*=(const T val) {
				auto p = elements;
				sforin(d, 0_u, D) { (*p) *= val; ++p; }
				return *this;
			}
			VectorND<T, D>& operator/=(const T val) {
				auto p = elements;
				sforin(d, 0_u, D) { (*p) /= val; ++p; }
				return *this;
			}
			VectorND<T, D> operator+(const VectorND<T, D>& v) const {
				VectorND<T, D> vec(*this);
				auto p1 = vec.elements;
				auto p2 = v.elements;
				sforin(d, 0_u, D) { (*p1) += (*p2); ++p1; ++p2; }
				return vec;
			}
			VectorND<T, D> operator-(const VectorND<T, D>& v) const {
				VectorND<T, D> vec(*this);
				auto p1 = vec.elements;
				auto p2 = v.elements;
				sforin(d, 0_u, D) { (*p1) -= (*p2); ++p1; ++p2; }
				return vec;
			}
			VectorND<T, D> operator*(const T val) const {
				VectorND<T, D> vec(*this);
				auto p = vec.elements;
				sforin(d, 0_u, D) { (*p) *= val; ++p; }
				return vec;
			}
			VectorND<T, D> operator/(const T val) const {
				VectorND<T, D> vec;
				auto p1 = elements;
				auto p2 = vec.elements;
				sforin(d, 0_u, D) { *p2 = (*p1) / val; ++p1; ++p2; }
				return vec;
			}
			VectorND<T, D> operator-() const {
				VectorND<T, D> vec(*this);
				auto p = vec.elements;
				sforin(d, 0_u, D) { (*p) *= -1; ++p; }
			}
			T operator * (const VectorND<T, D>& v) const {
				auto dot = T();
				auto p1 = elements, p2 = v.elements;
				sforin(d, 0_u, D) { dot += (*p1) * (*p2); ++p1; ++p2; }
				return dot;
			}
			size_t size() const { return D; }
			void reset(const T val) { sforin(i, 0, D) elements[i] = val; }
			T& operator[](const int i) { return elements[(i < 0 ? (int)D : 0) + i]; }
			const T& operator[](const int i) const { return  elements[(i < 0 ? (int)D : 0) + i]; }
			bool operator<(const slib::smath::VectorND<T, D>& v) const { 
				return memcmp(elements, v.elements, D * sizeof(T)) < 0; 
			}
			bool operator==(const slib::smath::VectorND<T, D>& v) const { 
				return memcmp(elements, v.elements, D * sizeof(T)) == 0; 
			};
			bool operator!=(const slib::smath::VectorND<T, D>& v) const {
				return memcmp(elements, v.elements, D * sizeof(T)) != 0;
			};
		};

		template<typename T, size_t D>
		extern inline T length(const slib::smath::VectorND<T, D>& v) {
			T len = 0;
			auto p = v.elements;
			sforin(d, 0_u, D) { len += ((*p) * (*p)); ++p; }
			return (T)std::sqrt(len);
		}
		template<typename T, size_t D>
		extern inline T lpnorm(const slib::smath::VectorND<T, D>& v) {
			double len = 0.0;
			sforin(d, 0_u, D) { len += smath::power(abs(v[d]), v.dimension()); }
			return smath::rootN(len, v.dimension());
		}
		template<typename T, size_t D>
		extern inline void normalize(slib::smath::VectorND<T, D>& v) {
			auto len = smath::length(v);
			if (len == 0) throw DivZeroException(divZeroErrorText("Vector length"));
			v /= len;
		}
		template<typename T, size_t D>
		extern inline slib::smath::VectorND<T, D> log(const slib::smath::VectorND<T, D>& v, double base) {
			slib::smath::VectorND<T, D> vec(v.size());
			if (base == snum::E) { sforin(d, 0_u, D) { vec[d] = std::log(vec[d]); } }
			else { auto b = std::log(base); sforin(d, 0_u, D) { vec[d] = std::log(vec[d]) / b; } }
			return vec;
		}
		template<typename T>
		extern inline T cross(const slib::smath::VectorND<T, 2>& v1, const slib::smath::VectorND<T, 2>& v2) { return v1.x() * v2.y() - v1.y() * v2.x(); }
		template<typename T>
		extern inline slib::smath::VectorND<T, 2> orthogonal(const slib::smath::VectorND<T, 2>& v, slib::DIRECTION rot) {
			return rot == slib::DIRECTION::CW ? slib::smath::VectorND<T,2>(v.y(), -v.x()) : slib::smath::VectorND<T,2>(-v.y(), v.x());
		}
		template<typename T>
		extern inline T slope(const slib::smath::VectorND<T, 2>& v) {
			//if (v.x() == 0) throw 
			return (double)v.y() / v.x();
		}
		template<typename T>
		extern inline T argument(const slib::smath::VectorND<T, 2>& v) {
			auto len = length(v);
			//if (len == 0) throw 
			return v.y() < 0 ? 2 * snum::PI - acos((double)v.x() / len) : acos((double)v.x() / len);
		}



		template <typename T>
		using Vector2D = VectorND<T, 2>;
		template <typename T>
		using Vector3D = VectorND<T, 3>;
		template <typename T>
		using Vector4D = VectorND<T, 4>;

		template <typename T>
		class Vector {
		protected:
			Array<T> values;
		public:
			Vector() : values() {}
			Vector(std::initializer_list<T> li) : values(li.size()) { sfor2(*this, li) { $_1 = $_2; } }
			Vector(const size_t s, const T& val = T()) : values(s, val) {}
			Vector(Vector<T>&& v) noexcept { values.swap(v.values); }
			Vector(const Vector<T>& v) : values(v.values) {}
			~Vector() {}
			Vector<T>& operator=(const Vector<T>& v) {
				values = v.values; return *this;
			}
			Vector<T>& operator+=(const Vector<T>& v) {
				if (size() != v.size()) throw ShapeMismatchException("Dimensions of vectors are mismatched.");
#ifdef USE_OMP
				#pragma omp parallel for
#endif
				sfor2(*this, v) { $_1 += $_2; }
				return *this;
			}
			Vector<T>& operator+=(const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(*this) { $_ += val; }
				return *this;
			}
			Vector<T>& operator-=(const Vector<T>& v) {
				if (size() != v.size()) throw ShapeMismatchException("Dimensions of vectors are mismatched.");
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(*this, v) { $_1 -= $_2; }
				return *this;
			}
			Vector<T>& operator-=(const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(*this) { $_ -= val; }
				return *this;
			}
			Vector<T>& operator*=(const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(*this) { $_ *= val; }
				return *this;
			}
			Vector<T>& operator/=(const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(*this) { $_ /= val; }
				return *this;
			}
			Vector<T> operator+(const Vector<T>& v) const { Vector<T> v_(*this); v_ += v; return v_; }
			Vector<T> operator-(const Vector<T>& v) const { Vector<T> v_(*this); v_ -= v; return v_; }
			Vector<T> operator-() const {
				Vector<T> v_(*this);
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(v_) { $_ = -$_; }
				return *this;
			}
			Vector<T> operator*(const T val) const { Vector<T> v_(*this); v_ *= val; return v_; }
			Vector<T> operator/(const T val) const { Vector<T> v_(*this); v_ /= val; return v_; }
			Vector<T>& operator<<(const T& val) { values.add(val); return *this; }
			Vector<T>& operator<<(const Vector<T>& vec) { values.append(vec.values); return *this; }
			T operator *(const Vector<T>& v) const {
				if (size() != v.size()) throw ShapeMismatchException("Dimensions of vectors are mismatched.");
				auto dot = T();
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(*this, v) { dot += $_1 * $_2; }
				return dot;
			}
			bool empty() const { return values.empty(); }
			size_t size() const { return values.size(); }
			void reserve(const size_t sz) { values.reserve(sz); }
			void resize(const size_t sz) { values.resize(sz); }
			void resize(const size_t sz, const T val) { values.resize(sz, val); }
			T* data() { return values.data(); }
			const T* data() const { return values.data(); }
			T& operator[](const int i) { return values[i]; }
			const T& operator[](const int i) const { return values[i]; }
			T& at(const int i) { return values[i]; }
			const T& at(const int i) const { return values[i]; }
			ArrayIterator<T> begin() { return values.begin(); }
			ArrayCIterator<T> begin() const { return values.begin(); }
			ArrayIterator<T> end() { return values.end(); }
			ArrayCIterator<T> end() const { return values.end(); }
			CycleIterator<T> cycle(const int i = 0) { return CycleIterator<T>(values.data(i), begin(), size()); }
			CycleCIterator<T> cycle(const int i = 0) const { return CycleCIterator<T>(values.data(i), begin(), size()); }
			void add(const T& v) { values.add(v); }
			void insert(const int i, const T& v) { values.insert(i, v); }
			void removeAt(const int i) { values.removeAt(i); }
			void swap(Vector<T>& v) { values.swap(v.values); }
			void reset(const T val) {
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor(*this) { $_ = val; }
			}
			void clear() { resize(0); }
			template<typename S>
			Vector<S> as() const {
				Vector<S> vec(size());
#ifdef USE_OMP
#pragma omp parallel for
#endif
				sfor2(vec, *this) { $_1 = (S)$_2; }
				return vec;
			}
			Array<T>& array() { return values; }
			const Array<T>& array() const { return values; }
			bool operator <(const Vector<T>& vec) { return values < vec.values; }
			bool operator ==(const Vector<T>& vec) { return values == vec.values; }

		};	
	}
	template<typename T, size_t D>
	extern slib::String toString(const slib::smath::VectorND<T, D>& vec, const char* format = nullptr) {
		slib::String str("(");
		sforin(i, 0, D) str << vec[i] << ",";
		str[-1] = ')';
		return str;
	}
	template<typename T>
	extern slib::String toString(const slib::smath::Vector<T>& vec, const char* format = nullptr) {
		slib::String str("(");
		if (vec.empty()) str << ")";
		else {
			sfor(vec) str << $_ << ",";
			str[-1] = ')';
		}
		return str;
	}
	using svec2b = smath::Vector2D<slib::subyte>;
	using svec2i = smath::Vector2D<slib::sint>;
	using svec2l = smath::Vector2D<slib::sinteger>;
	using svec2f = smath::Vector2D<float>;
	using svec2d = smath::Vector2D<double>;
	using svec3b = smath::Vector3D<slib::subyte>;
	using svec3i = smath::Vector3D<slib::sint>;
	using svec3l = smath::Vector3D<slib::sinteger>;
	using svec3f = smath::Vector3D<float>;
	using svec3d = smath::Vector3D<double>;
	using svec4b = smath::Vector4D<slib::subyte>;
	using svec4i = smath::Vector4D<slib::sint>;
	using svec4l = smath::Vector4D<slib::sinteger>;
	using svec4f = smath::Vector4D<float>;
	using svec4d = smath::Vector4D<double>;
	template<typename T>
	using svec = smath::Vector<T>;
	using svecb = smath::Vector<slib::subyte>;
	using sveci = smath::Vector<slib::sint>;
	using svecl = smath::Vector<slib::sinteger>;
	using svecu = smath::Vector<slib::suinteger>;
	using svecs = smath::Vector<size_t>;
	using svecf = smath::Vector<float>;
	using svecd = smath::Vector<double>;
}

template<typename T, size_t D>
extern inline std::ostream& operator<<(std::ostream& os, const slib::smath::VectorND<T, D>& vec) { return os << slib::toString(vec); }
template<typename T>
extern inline std::ostream& operator<<(std::ostream& os, const slib::smath::Vector<T>& vec) { return os << slib::toString(vec); }

#endif