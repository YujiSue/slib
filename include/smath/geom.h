#ifndef SMATH_GEOM_H
#define SMATH_GEOM_H

#include "smath/calc.h"
#include "smath/la.h"
#include "sbasic/area.h"
#include "sbasic/zone.h"

#define SShape2D slib::sgeom::Shape<2, slib::Area<float>, float>
#define point2d slib::sgeom::Point<2, slib::Area<float>, float>
#define line2d slib::sgeom::Line<2, slib::Area<float>, float>
#define curve2d slib::sgeom::Curve<2, slib::Area<float>, float, 3>

namespace slib {
	namespace sshape {
		constexpr int MULTIPLE = 0x0100;
		constexpr int ROUND = 0x0200;
		constexpr int CIRCULAR = 0x0400;
		constexpr int REGULAR = 0x0800;
		constexpr int CONCAVE = 0x1000;
		constexpr int CLOSED = 0x2000;
		constexpr int TEXTURE = 0x4000;
		constexpr int GROUP = 0x8000;

		constexpr int POINT = 0x0001;
		constexpr int MULTI_POINT = MULTIPLE | POINT;
		constexpr int LINE = 0x0002;
		constexpr int CURVE = ROUND | LINE;
		constexpr int PATH = MULTIPLE | LINE;

		constexpr int RECTANGLE = 0x0004;
		constexpr int SQUARE = REGULAR | RECTANGLE;
		constexpr int PLANE = 0x0004;
		constexpr int POLYGON = 0x0008;
		constexpr int STAR = CONCAVE | POLYGON;
		constexpr int ELLIPSE = CIRCULAR | RECTANGLE;
		constexpr int CIRCLE = REGULAR | CIRCULAR | RECTANGLE;
		constexpr int ARC = CIRCULAR | LINE;

		constexpr int PICTURE = TEXTURE | RECTANGLE;
		constexpr int TEXT = 0x0010;
	}
	namespace sgeom {
		template<size_t D, typename T>
		extern inline double distance(const sla::SVectorND<D, T>& v1,
			const sla::SVectorND<D, T>& v2) {
			return sla::length(v1 - v2);
		}
		template<typename T>
		extern inline double direction(const sla::SVectorND<2, T>& vec,
			const sla::SVectorND<2, T>& axis = sla::SVectorND<2, T>(1, 0)) {
			return sla::argument(vec) - sla::argument(axis);
		}
		template<typename T>
		extern sla::SVectorND<2, double> direction(const sla::SVectorND<3, T>& vec,
			const sla::SVectorND<3, T>& axis = sla::SVectorND<3, T>(1, 0)) {
			return sla::SVectorND<2, double>(sla::azimuth(vec) - sla::azimuth(axis), sla::altitude(vec) - sla::altitude(axis));
		}
		template<typename T, class M>
		extern inline double distance(const sla::SVector<T, M>& v1, const sla::SVector<T, M>& v2) {
			if (v1.size() != v2.size())
				throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
			return sla::length(v2 - v1);
		}
		typedef enum {
			XYZ = 1,
			POLAR = 2,
			CYLINDER = 3,
			SPHERE = 4,
		} COORDINATE;
		template<typename T>
		extern inline sla::SVectorND<2, T> XYToPolar(const sla::SVectorND<2, T>& xy) { 
			return sla::SVectorND<2, T>(sla::length(xy), (float)argument(xy)); 
		}
		template<typename T>
		extern inline sla::SVectorND<2, T> PolarToXY(const sla::SVectorND<2, T>& pol) { 
			return sla::SVectorND<2, T>(pol.x() * cos(pol.y()), pol.x() * sin(pol.y())); 
		}
		template<typename T>
		extern inline sla::SVectorND<3, T> XYZToSphere(const sla::SVectorND<3, T>& xyz) {
			return sla::SVectorND<3, T>(sla::length(xyz), sla::azimuth(xyz), sla::altitude(xyz));
		}
		template<typename T>
		extern inline sla::SVectorND<3, T> SphereToXYZ(const sla::SVectorND<3, T>& sph) { 
			return sla::SVectorND<3, T>(sph.x() * cos(sph.y()) * cos(sph.z()), sph.x() * sin(sph.y()) * cos(sph.z()), sph.x() * sin(sph.z()));
		}
		template<typename T>
		extern inline sla::SVectorND<3, T> xyz2tube(const sla::SVectorND<3, T>& xyz) { 
			return sla::SVectorND<3, T>(sla::length(xyz), sla::azimuth(xyz), xyz.z()); 
		}
		template<typename T>
		extern inline sla::SVectorND<3, T> tube2xy(const sla::SVectorND<3, T>& tb) { 
			return sla::SVectorND<3, T>(tb.x() * cos(tb.y()), tb.x() * sin(tb.y()), tb.z());
		}
		typedef enum {
			ZERO_ORIGIN = 0,
			UPPER_LEFT = 1,
			LOWER_LEFT = 2,
			UPPER_RIGHT = 3,
			LOWER_RIGHT = 4,
			MASS_CENTER = 5,
		} ORIGIN;
		template<size_t D, typename T>
		extern inline sla::SVectorND<D, T> midpoint(const sla::SVectorND<D, T>& v1, const sla::SVectorND<D, T>& v2) { return (v1 + v2) / 2; }
		template<typename T>
		extern inline sla::SVectorND<2, T> center(const Area<T>& area) {
			return sla::SVectorND<2, T>(area.ori_x + area.width / 2, area.ori_y + area.height / 2);
		}
		template<typename T>
		extern inline sla::SVectorND<3, T> center(const Zone<T>& zone) {
			return sla::SVectorND<3, T>(zone.ori_x + zone.width / 2, zone.ori_y + zone.height / 2, zone.ori_z + zone.depth / 2);
		}
		typedef enum {
			NONE = 0x00,
			TRANSLATION = 0x01,
			SCALING = 0x02,
			ROTATION = 0x03,
			SHEARING = 0x04,
			REFLECTION = 0x05,
		} TRANSFORM;
		template<size_t D, typename T>
		extern inline void translate(sla::SVectorND<D, T>& vec, const sla::SVectorND<D, T>& trans) { vec += trans; }
		template<size_t D, typename T>
		extern inline void scale(sla::SVectorND<D, T>& vec, const sla::SVectorND<D, T>& sc) { sforin(d, 0, D) { vec[d] *= sc[d]; }}
		template<typename T>
		extern inline void rotate(sla::SVectorND<2, T>& vec, const T& r) {
			sla::SVectorND<2, T> v_(cos(r) * vec[0] - sin(r) * vec[1], sin(r) * vec[0] + cos(r) * vec[1]);
			vec = v_;
		}
		template<typename T>
		extern inline void rotate(sla::SVectorND<3, T>& vec, const T& r, smath::AXIS ax) {
			sla::SVectorND<3, T> v_;
			switch (ax) {
			case smath::X_AXIS:
				v_ = sla::SVectorND<3, T>(vec[0], cos(r) * vec[1] - sin(r) * vec[2], sin(r) * vec[1] + cos(r) * vec[2]);
				break;
			case smath::Y_AXIS:
				v_ = sla::SVectorND<3, T>(cos(r) * vec[0] + sin(r) * vec[2], vec[1], -sin(r) * vec[0] + cos(r) * vec[2]);
				break;
			case smath::Z_AXIS:
				v_ = sla::SVectorND<3, T>(cos(r) * vec[0] - sin(r) * vec[1], sin(r) * vec[0] + cos(r) * vec[1], vec[2]);
				break;
			}
			vec = v_;
		}
		template<size_t D, typename T>
		extern inline void shear(sla::SVectorND<D, T>& vec, const sla::SVectorND<D, T>& sh) {
			sforin(d, 0, D) {
				sforin(e, 0, D) {
					if (d == e) continue;
					else vec[d] += vec[e] * sh[e];
				}
				
			}
			vec[0] += vec[1] * sh[0]; vec[1] += vec[0] * sh[1];
		}
		template<size_t D, typename T>
		extern inline void reflect(sla::SVectorND<D, T>& vec, smath::DIRECTION dir) {
			if (dir == smath::HORIZONTAL) vec[0] *= -1;
			else if(dir == smath::VERTICAL)  vec[1] *= -1;
			else if (dir == smath::DEPTH)  vec[2] *= -1;
		}

		/*
		extern inline v2f affine2d(const v2f& pos, const v2f& trans, const v2f& scale, const float& rot, const float& ratio, smath::DIRECTION dir) {
			return mat2f((float)cos(rot), (float)-sin(rot), (float)sin(rot), (float)cos(rot)) *
				mat2f(scale.x, 0, 0, scale.y) *
				mat2f(1.0f, dir == smath::HORIZONTAL ? ratio : 0.0f, dir == smath::VERTICAL ? ratio : 0.0f, 1.0f) *
				pos + trans;
		}
		extern inline v2d affine2d(const v2d& pos, const v2d& trans, const v2d& scale, const double& rot, const double& ratio, smath::DIRECTION dir) {
			return mat2d(cos(rot), -sin(rot), sin(rot), cos(rot)) *
				mat2d(scale.x, 0, 0, scale.y) *
				mat2d(1.0, dir == smath::HORIZONTAL ? ratio : 0.0, dir == smath::VERTICAL ? ratio : 0.0, 1.0) * pos + trans;
		}
		*/


		/*
		extern inline v2f affine2d(const v2f& pos, const v2f& trans, const v2f& scale, const float& rot, const float& ratio, smath::DIRECTION dir) {
			return mat2f((float)cos(rot), (float)-sin(rot), (float)sin(rot), (float)cos(rot)) *
				mat2f(scale.x, 0, 0, scale.y) *
				mat2f(1.0f, dir == smath::HORIZONTAL ? ratio : 0.0f, dir == smath::VERTICAL ? ratio : 0.0f, 1.0f) *
				pos + trans;
		}
		extern inline v2d affine2d(const v2d& pos, const v2d& trans, const v2d& scale, const double& rot, const double& ratio, smath::DIRECTION dir) {
			return mat2d(cos(rot), -sin(rot), sin(rot), cos(rot)) *
				mat2d(scale.x, 0, 0, scale.y) *
				mat2d(1.0, dir == smath::HORIZONTAL ? ratio : 0.0, dir == smath::VERTICAL ? ratio : 0.0, 1.0) * pos + trans;
		}
		extern inline double length(const v2ivec& v) {
			double len = 0.0;
			sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
			return len;
		}
		extern inline double length(const v2fvec& v) {
			double len = 0.0;
			sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
			return len;
		}
		extern inline double length(const v2dvec& v) {
			double len = 0.0;
			sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
			return len;
		}
		*/
		extern inline void BernsteinPolynom(sinteger n, double d, svecd& vec) {
			sinteger i = 0;
			sforeach(vec) {
				E_ = (double)smath::combination(n, i) * smath::power(d, i) * smath::power(1 - d, n - i); ++i;
			}
		}
		/*
		template<size_t D, typename T>
		extern inline void Bezier(const sla::SVectorND<D, T>& v1, const sla::SVectorND<D, T>& v2, const sla::SVectorND<D, T>& v3, sla::SVector<sla::SVectorND<D, T>>& curve) {

		}
		using bezier2f = Bezier<2, float>;
		*/

		template<typename T>
		extern inline void bezier2(const sla::SVectorND<2, T>& v1, const sla::SVectorND<2, T>& v2, const sla::SVectorND<2, T>& v3, v2dvec& curve, sint n = 10) {
			svecd coeff(3);
			curve.resize(n + 1);
			double init = 0.0, d = 1.0 / n;
			sforeach(curve) {
				BernsteinPolynom(2, init, coeff);
				E_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3;
				init += d;
			}
		}
		template<typename T>
		extern inline void bezier2(const sla::SVectorND<3, T>& v1, const sla::SVectorND<3, T>& v2, const sla::SVectorND<3, T>& v3, v3dvec& curve, sint n = 10) {
			svecd coeff(3);
			curve.resize(n + 1);
			double init = 0.0, d = 1.0 / n;
			sforeach(curve) {
				BernsteinPolynom(2, init, coeff);
				E_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3;
				init += d;
			}
		}
		template<typename T>
		extern inline void bezier3(const sla::SVectorND<2, T>& v1, const sla::SVectorND<2, T>& v2, const sla::SVectorND<2, T>& v3, const sla::SVectorND<2, T>& v4, v2dvec& curve, sint n = 10) {
			svecd coeff(4);
			curve.resize(n + 1);
			double init = 0.0, d = 1.0 / n;
			sforeach(curve) {
				BernsteinPolynom(3, init, coeff);
				E_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3 + coeff[3] * v4;
				init += d;
			}
		}
		template<typename T>
		extern inline void bezier3(const sla::SVectorND<3, T>& v1, const sla::SVectorND<3, T>& v2, const sla::SVectorND<3, T>& v3, const sla::SVectorND<3, T>& v4, v3dvec& curve, sint n = 10) {
			svecd coeff(4);
			curve.resize(n + 1);
			double init = 0.0, d = 1.0 / n;
			sforeach(curve) {
				BernsteinPolynom(3, init, coeff);
				E_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3 + coeff[3] * v4;
				init += d;
			}
		}
		extern inline double BSplineBasis(sint i, sint dim, double k, svecd& knot) {
			if (dim) {
				double d = 0;
				if (knot[i + dim] != knot[i]) d += (k - knot[i]) / (knot[i + dim] - knot[i]) * BSplineBasis(i, dim - 1, k, knot);
				if (knot[i + dim + 1] != knot[i + 1]) d += (knot[i + dim + 1] - k) / (knot[i + dim + 1] - knot[i + 1]) * BSplineBasis(i + 1, dim - 1, k, knot);
				return d;
			}
			else {
				if (k < knot[i] || knot[i + 1] <= k) return 0;
				else return 1;
			}
		}
		extern inline void uniKnot(sint d, svecd& knot) {
			knot.first() = 0;
			knot.last() = 1.0;
			double diff = 1.0 / (knot.size() - 1);
			sforin(i, 1, knot.size() - 1) knot[i] = knot[i - 1] + diff;
		}
		extern inline void openUniKnot(sint d, svecd& knot) {
			auto it = knot.begin();
			sforin(i, 0, d + 1) { E_ = 0.0; NEXT_; }
			auto len = (sint)knot.size() - 2 * (d + 1);
			double diff = 1.0 / len;
			sforin(i, 0, len - 1) { E_ = E_PREV + diff; NEXT_; }
			sforin(i, 0, d + 1) { E_ = 1.0; NEXT_; }
		}
		extern inline void bspline(sint n, v2dvec& vec, v2dvec& curve, svecd& knot) {
			curve.resize(knot.size());
			sforeach2(curve, knot) {
				E1_ = v2d();
				sforeachi(vec) E1_ += vec[i] * BSplineBasis(i, n, E2_, knot);
			}
		}
		extern inline void bspline(sint n, v2dvec& vec, v2dvec& curve,
			std::function<void(sint, svecd&)> makeKnot = openUniKnot) {
			svecd knot(vec.size() + n + 1);
			makeKnot(n, knot);
			bspline(n, vec, curve, knot);
		}
		extern inline void _bspline2(v2dvec& vec, v2dvec& curve, svecd& knot) {
			sforeach2(curve, knot) {
				E1_ = v2d();
				sforeachi(vec) { E1_ += vec[i] * BSplineBasis(i, 2, E2_, knot); }
			}
		}
		extern inline void bspline2(v2dvec& vec, v2dvec& curve,
			std::function<void(sint, svecd&)> makeKnot = openUniKnot) {
			svecd knot(vec.size() + 3);
			curve.resize(vec.size());
			makeKnot(2, knot);
			_bspline2(vec, curve, knot);
		}
		extern inline void _bspline3(v2dvec& vec, v2dvec& curve, svecd& knot) {
			sforeach2(curve, knot) {
				E1_ = v2d();
				sforeachi(vec) { E1_ += vec[i] * BSplineBasis(i, 3, E2_, knot); }
			}
		}
		extern inline void bspline3(v2dvec& vec, v2dvec& curve,
			std::function<void(sint, svecd&)> makeKnot = openUniKnot) {
			svecd knot(vec.size() + 4);
			curve.resize(vec.size());
			makeKnot(3, knot);
			_bspline3(vec, curve, knot);
		}

		template<size_t D, typename T>
		struct affine_trans {
			sgeom::TRANSFORM _type;
			sla::SVectorND<D, T> _trans;
			affine_trans() : _type(sgeom::NONE) {}
			affine_trans(sgeom::TRANSFORM t, const sla::SVectorND<D, T>& v) : _type(t), _trans(v) {}
			affine_trans(const affine_trans& a) : _type(a._type), _trans(a._trans) {}
			~affine_trans() {}
		};
		template<size_t D, typename T>
		class SLIB_DLL Transform {
		public:
			static void translate(
				const sla::SVectorND<D, T>& trans,
				sla::SVector<sla::SVectorND<D, T>>& vertex) {
				sforeach(vertex) E_ += trans;
			}
			static void scale(
				const sla::SVectorND<D, T>& scale,
				sla::SVector<sla::SVectorND<D, T>>& vertex,
				const sla::SVectorND<D, T>& ori) {
				sforeach(vertex) {
					E_ -= ori; sforin(i, 0, D) { E_[i] = scale[i]; } E_ += ori;
				}
			}
			static void shear(const sla::SVectorND<D, T>& scale,
				sla::SVector<sla::SVectorND<D, T>>& vertex,
				const sla::SVectorND<D, T>& ori) {

			}
			static void rotate(float rot, v2fvec& vertex, v2f ori);
			static void reflect(subyte ref, v2fvec& vertex, v2f ori);
		};




		template<typename T>
		inline void _renewBoundary(Area<T>& a, const sla::SVectorND<2, T>& v) { a = Area<T>(v.x(), v.y(), 0, 0); }
		template<typename T>
		inline void _updateBoundary(Area<T>& a, const sla::SVectorND<2, T>& v) {
			if (v.x() < a.ori_x) {
				a.width = a.ori_x + a.width - v.x();
				a.ori_x = v.x();
			}
			else if (a.ori_x + a.width < v.x()) a.width = v.x() - a.ori_x;
			if (v.y() < a.ori_y) {
				a.height = a.ori_y + a.height - v.y();
				a.ori_y = v.y();
			}
			else if (a.ori_y + a.height < v.y()) a.height = v.y() - a.ori_y;
		}
		template<typename T>
		void _makeBoundary(Area<T>& a, const sla::SVector<sla::SVectorND<2, T>>& vec) {
			if (vec.empty()) a = Area<T>();
			else {
				_renewBoundary(a, vec[0]);
				if (1 < vec.size()) {
					auto it = vec.begin() + 1;
					while (it < vec.end()) { _updateBoundary(a, E_); NEXT_; }
				}
			}
		}
		template<typename T>
		inline void _renewBoundary(Zone<T>& z, const sla::SVectorND<3, T>& v) { z = Zone<T>(v.x(), v.y(), v.z(), 0, 0, 0); }
		template<typename T>
		inline void _updateBoundary(Zone<T>& z, const sla::SVectorND<3, T>& v) {
			if (v.x() < z.ori_x) {
				z.width = z.ori_x + z.width - v.x();
				z.ori_x = v.x();
			}
			else if (z.ori_x + z.width < v.x()) z.width = v.x() - z.ori_x;
			if (v.y() < z.ori_y) {
				z.height = z.ori_y + z.height - v.y();
				z.ori_y = v.y();
			}
			else if (z.ori_y + z.height < v.y()) z.height = v.y() - z.ori_y;
			if (v.z() < z.ori_z) {
				z.depth = z.ori_z + z.depth - v.z();
				z.ori_z = v.z();
			}
			else if (z.ori_z + z.depth < v.z()) z.depth = v.z() - z.ori_z;
		}
		template<typename T>
		void _makeBoundary(Zone<T>& z, const sla::SVector<sla::SVectorND<3, T>>& vec) {
			if (vec.empty()) z = Zone<T>();
			else {
				_renewBoundary(vec[0]);
				if (1 < vec.size()) {
					auto it = vec.begin() + 1;
					while (it < vec.end()) { _updateBoundary(z, E_); NEXT_; }
				}
			}
		}
		template<size_t D, class B, typename T>
		class SLIB_DLL Shape {
		protected:
			suint _type;
			B _boundary;
			sgeom::ORIGIN _oritype;
			sla::SVectorND<D, T> _origin;
			sla::SVector<sla::SVectorND<D, T>> _vertex;
			Array<affine_trans<D, T>> _transform;

		protected:
			void _resetBoundary() { _makeBoundary(_boundary, _vertex); }
		public:
			Shape() : _type(0) {}
			Shape(suint t, sinteger n = 0) : _type(t) { if (n) { _vertex.resize(n); _resetBoundary(); } }
			Shape(suint t, std::initializer_list<sla::SVectorND<D, T>> li) : _type(t) {
				if (li.size()) {
					_vertex.resize(li.size());
					sforeach2(_vertex, li) E1_ = E2_;
					_resetBoundary();
				}
			}
			Shape(const Shape& s) : _type(s._type), _boundary(s._boundary), _vertex(s._vertex) {}
			virtual ~Shape() {}
			suint type() const { return _type; }
			const B& boundary() const { return _boundary; }
			void addPoint(const sla::SVectorND<D, T>& p) {
				if (_vertex.empty()) _renewBoundary(_boundary, p);
				else _updateBoundary(_boundary, p);
				_vertex.add(p);
			}
			void setPoint(sinteger idx, const sla::SVectorND<D, T>& p) {
				_vertex[idx] = p;
				_resetBoundary(_boundary, _vertex);
			}
			void setOrigin(const sla::SVectorND<D, T>& o) { _origin = o; }
			void setTranslation(const sla::SVectorND<D, T>& t) { _transform.add(sgeom::TRANSLATION, t); }
			void setScale(const sla::SVectorND<D, T>& s) { _transform.add(sgeom::SCALING, s); }
			void setShear(const sla::SVectorND<D, T>& s) { _transform.add(sgeom::SHEARING, s); }
			void setRotation(const sla::SVectorND<D, T>& r) { _transform.add(sgeom::ROTATION, r); }
			void setReflect(const sla::SVectorND<D, T>& r) { _transform.add(sgeom::REFLECTION, r); }
			virtual size_t count() const { return _vertex.size(); }
			virtual sla::SVectorND<D, T> center() const = 0;
			virtual double length() const = 0;
			virtual T area() const = 0;
			virtual T volume() const = 0;
			virtual bool include(const sla::SVectorND<D, T>& p) const { return false; }// _boundary.include(p); }
			virtual bool cross(const Shape& s) const { return _boundary.overlap(s._boundary); }


		};
		template<size_t D, class B, typename T>
		class SLIB_DLL Point : public Shape<D, B, T> {
		public:
			Point() : shape(sshape::POINT, 1) {}
			Point(const sla::SVectorND<D, T>& p) : shape(sshape::POINT) { Shape<D, B, T>::addPoint(p); }
			Point(const Point& p) : shape(p) {}
			virtual ~Point() {}
			Point& operator=(const Point& p) {
				Shape<D, B, T>::_type = p._type;
				Shape<D, B, T>::_boundary = p._boundary;
				Shape<D, B, T>::_vertex = p._vertex;
				Shape<D, B, T>::_origin = p._origin;
				Shape<D, B, T>::_transform = p._transform;
				return *this;
			}
			virtual void dot(const sla::SVectorND<D, T>& p) {
				if (!(Shape<D, B, T>::_type& sshape::MULTIPLE)) Shape<D, B, T>::_type |= sshape::MULTIPLE;
				Shape<D, B, T>::addPoint(p);
			}
			virtual sla::SVectorND<D, T> center() const {
				if (Shape<D, B, T>::count() < 2) return Shape<D, B, T>::_vertex[0];
				else {
					auto c = sla::SVectorND<D, T>();
					sforeach(Shape<D, B, T>::_vertex) c += E_;
					c /= Shape<D, B, T>::count();
					return c;
				}
			}
			virtual double length() const { return 0; }
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }
			//virtual bool include(const sla::SVectorND<D, T>& p) const { return shape::_vertex.indexOf(p) != NOT_FOUND; }
		};
		template<size_t D, class B, typename T>
		class SLIB_DLL Line : public Shape<D, B, T> {
		public:
			Line() : Shape<D, B, T>(sshape::LINE, 2) {}
			Line(const sla::SVectorND<D, T>& p1, const sla::SVectorND<D, T>& p2) : Shape<D, B, T>(sshape::LINE) {
				Shape<D, B, T>::_vertex.add(p1); Shape<D, B, T>::_vertex.add(p2); Shape<D, B, T>::_resetBoundary();
			}
			Line(const Line& l) : Shape<D, B, T>(l) {}
			virtual ~Line() {}
			Line& operator=(const Line& l) {
				Shape<D, B, T>::_type = l._type;
				Shape<D, B, T>::_boundary = l._boundary;
				Shape<D, B, T>::_vertex = l._vertex;
				Shape<D, B, T>::_origin = l._origin;
				Shape<D, B, T>::_transform = l._transform;
				return *this;
			}
			virtual void setStart(const sla::SVectorND<D, T>& p) { Shape<D, B, T>::setPoint(0, p); }
			virtual void setEnd(const sla::SVectorND<D, T>& p) { Shape<D, B, T>::setPoint(-1, p); }
			virtual double length() const {
				return sgeom::distance(Shape<D, B, T>::_vertex[0], Shape<D, B, T>::_vertex[1]);
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }
			/*
			virtual bool include(const sla::SVectorND<D, T>& p) const {
				return (Shape<D, B, T>::_vertex[0] - p)*(Shape<D, B, T>::_vertex[1] - Shape<D, B, T>::_vertex[0])
			}
			*/
		};
		template<size_t D, class B, typename T, size_t R = 3>
		class SLIB_DLL Curve : public Shape<D, B, T> {
			typedef Shape<D, B, T> shape;
		public:
			Curve() : shape(sshape::CURVE, R) {}
			Curve(std::initializer_list<sla::SVectorND<D, T>> li) : shape(sshape::CURVE) {
				if (li.size() == R) {
					Shape<D, B, T>::_vertex.resize(li.size());
					sforeach2(Shape<D, B, T>::_vertex, li) E1_ = E2_;
					Shape<D, B, T>::_resetBoundary();
				}
				else throw smath::SMathException(ERR_INFO, smath::DIMENSION_SIZE_ERR);
			}
			Curve(const Curve& c) : shape(c) {}
			virtual ~Curve() {}
			size_t order() const { return R; }
			void interpolate(size_t n);
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }

		};
		template<size_t D, class B, typename T, size_t R = 3>
		class SLIB_DLL Path : public Shape<D, B, T> {
			ubytearray _track;
		public:
			Path() : Shape<D, B, T>(sshape::PATH) {}
			virtual ~Path() {}

			void moveTo(const sla::SVectorND<D, T>& p) {
				Shape<D, B, T>::_addPoint(p);
				_track.add(0);
			}
			void lineTo(const sla::SVectorND<D, T>& p) {
				Shape<D, B, T>::_addPoint(p);
				_track.add(sshape::LINE & 0x0F);
			}
			template<class... Args>
			void curveTo(Args... args) {


			}
			void closePath() {
				if (_track.empty()) return;
				_track.last() |= 0x10;
			}
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }
		};
		template<typename T>
		class SLIB_DLL Rectangle : public Shape<2, Area<T>, T> {
		protected:
			T _width, _height;
		public:
			Rectangle() : Shape<2, Area<T>, T>(sshape::RECTANGLE, 4) {}
			Rectangle(const T& x, const T& y, const T& w, const T& h)
				: Shape<2, Area<T>, T>(sshape::RECTANGLE,
					{
						sla::SVectorND<2, T>(x, y),
						sla::SVectorND<2, T>(x, y + h),
						sla::SVectorND<2, T>(x + w, y + h),
						sla::SVectorND<2, T>(x + w, y)
					}) {}
			Rectangle(const Rectangle<T>& rect) {}
			~Rectangle() {}

			void resize(const T& w, const T& h) {
				_width = w; _height = h;
				auto& p = Shape<2, Area<T>, T>::_vertex[0];

			}
			virtual T width() const { return _width; }
			virtual T height() const { return _height; }

			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }
		};
		template<typename T>
		class SLIB_DLL Plane : public Shape<3, Zone<T>, T> {
		protected:
			T _width, _height;
			sla::SVectorND<3, T> _horizontal, _vertical;
		public:
			Plane() : Shape<3, Zone<T>, T>(sshape::RECTANGLE, 4) {}
			Plane(const T& x, const T& y, const T& w, const T& h,
				const sla::SVectorND<3, T>& H, const sla::SVectorND<3, T>& V)
				: Shape<3, Zone<T>, T>(sshape::PLANE,
					{
						sla::SVectorND<2, T>(x, y),
						sla::SVectorND<2, T>(x, y + h),
						sla::SVectorND<2, T>(x + w, y + h),
						sla::SVectorND<2, T>(x + w, y)
					}) {}
			Plane(const Rectangle<T>& rect) {}
			~Plane() {}

			void resize(const T& w, const T& h) {
				_width = w; _height = h;
				auto& p = Shape<2, Area<T>, T>::_vertex[0];

			}
			virtual T width() const { return _width; }
			virtual T height() const { return _height; }
			virtual sla::SVectorND<3, T> horizontal() const { return _horizontal; }
			virtual sla::SVectorND<3, T> vertical() const { return _vertical; }
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }// sgeom::magnitude(sla::cross(_width * _horizontal, _height * _vertex)); }
			virtual T volume() const { return 0; }
		};
		template<size_t D, class B, typename T>
		class SLIB_DLL Polygon : public Shape<D, B, T> {
		public:
			Polygon() {}
			~Polygon() {}

			void resize(size_t n, const char* method = "lerp");
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }

		};

		template<typename T>
		class SLIB_DLL Ellipse : public Rectangle<T> {
		public:
			Ellipse() {}
			~Ellipse() {}
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }

		};
		template<typename T>
		class SLIB_DLL Arc : public Ellipse<T> {
		public:
			Arc() {}
			~Arc() {}
			virtual double length() const {
				/*
				 */
				return 0;
			}
			virtual T area() const { return 0; }
			virtual T volume() const { return 0; }

		};





	}

		
}

#endif