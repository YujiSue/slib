#ifndef SMATH_GEOM_H
#define SMATH_GEOM_H
#include "sbasic/area.h"
#include "sbasic/zone.h"
#include "smath/calc.h"
#include "smath/stat.h"
#include "smath/matrix.h"
namespace slib {
	namespace smath {
		template<typename T>
		extern Vector2D<T> toPolar(const Vector2D<T>& xy) {
			return Vector2D<T>(smath::length(xy), smath::argument(xy));
		}
		template<typename T>
		extern Vector2D<T> toOrthogonal(const Vector2D<T>& pol) {
			return Vector2D<T>(pol[0] * cos(pol[1]), pol[0] * sin(pol[1]));
		}
		template<typename T>
		extern Vector3D<T> toSphere(const Vector3D<T>& xyz) {
			return Vector3D<T>(length(xyz), azimuth(xyz),altitude(xyz));
		}
		template<typename T>
		extern Vector3D<T> toOrthogonal(const Vector3D<T>& sph) {
			return Vector3D<T>(sph[0] * cos(sph[1]) * cos(sph[2]), sph[0] * sin(sph[1]) * cos(sph[2]), sph[0] * sin(sph[2]));
		}
		template<typename T>
		extern Pair<Vector2D<T>, T> toTube(const Vector3D<T>& xyz) {
			return Pair<Vector2D<T>, T>(Vector2D<T>(length(xyz), azimuth(xyz)), xyz[2]);
		}
		template<typename T>
		extern Vector3D<T> toOrthogonal(const Pair<Vector2D<T>, T>& tb) {
			return Vector3D<T>(tb.first[0] * cos(tb.first[1]), tb.first[0] * sin(tb.first[1]), tb.second);
		}

		template<typename T>
		class AffineTransform2D {
		public:
			static void translate(const Vector2D<T>& vec, Vector2D<T>& pt) { pt += vec; }
			static void translate(const Vector2D<T>& vec, Vector<Vector2D<T>>& pts) { sfor(pts) { $_ += vec; } }
			static void expand(const Vector2D<T>& vec, const Vector2D<T>& ori, Vector2D<T>& pt) {
				pt -= ori; pt[0] *= vec[0]; pt[1] *= vec[1]; pt += ori;
			}
			static void expand(const Vector2D<T>& vec, const Vector2D<T>& ori, Vector<Vector2D<T>>& pts) { sfor(pts) { AffineTransform2D::expand(vec, ori, $_); } }
			static void rotate(const T v, const Vector2D<T>& ori, Vector2D<T>& pt) {
				pt -= ori; 
				Vector2D<T> tmp;
				tmp[0] = pt[0] * cos(v) - pt[1] * sin(v);
				tmp[1] = pt[0] * sin(v) + pt[1] * cos(v);
				pt = tmp;
				pt += ori;
			}
			static void rotate(const T v, const Vector2D<T>& ori, Vector<Vector2D<T>>& pts) { sfor(pts) { AffineTransform2D::rotate(v, ori, $_); } }
			static void shear(const T v, smath::AXIS ax, const Vector2D<T>& ori, Vector2D<T>& pt) {
				pt -= ori;

				pt += ori;
			}
			static void shear(const T v, smath::AXIS ax, const Vector2D<T>& ori, Vector<Vector2D<T>>& pts) { sfor(pts) { AffineTransform2D::shear(v, ax, ori, $_); } }
			static void reflect(smath::AXIS ax, const Vector2D<T>& ori, Vector2D<T>& pt) {
				pt -= ori; if (ax == AXIS::X) { pt[1] *= -1; } else { pt[0] *= -1; } pt += ori;
			}
			static void reflect(smath::AXIS ax, const Vector2D<T>& ori, Vector<Vector2D<T>>& pts) { sfor(pts) { AffineTransform2D::reflect(ax, ori, $_); } }
		};
		template<typename T>
		extern void bernsteinPolynom(int n, const T d, Vector<T>& vec) {
			auto i = 0;
			sfor(vec) {
				$_ = (double)smath::combination(n, i) * smath::power(d, i) * smath::power(1 - d, n - i); 
				++i;
			}
		}
		template<typename T>
		extern void bezier2(Vector<Vector2D<T>>& curve, const Vector2D<T>& v1, const Vector2D<T>& v2, const Vector2D<T>& v3) {
			Vector<T> coeff(3);
			auto num = curve.size() - 1;
			T init = (T)0, d = (T)1.0 / num;
			sfor(curve) {
				bernsteinPolynom(2, init, coeff);
				$_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3;
				init += d;
			}
		}
		template<typename T>
		extern void bezier3(Vector<Vector2D<T>>& curve, const Vector2D<T>& v1, const Vector2D<T>& v2, const Vector2D<T>& v3, const Vector2D<T>& v4) {
			Vector<T> coeff(4);
			auto num = curve.size() - 1;
			T init = (T)0, d = (T)1.0 / num;
			sfor(curve) {
				bernsteinPolynom(3, init, coeff);
				$_ = coeff[0] * v1 + coeff[1] * v2 + coeff[2] * v3 + coeff[3] * v4;
				init += d;
			}
		}


		template<typename T>
		size_t humming(const T* vec1, const T* vec2, const size_t s) {
			size_t dist = 0;
			sforin(i, 0, s) {
				dist += (*vec1) == (*vec2) ? 0 : 1;
				++vec1; ++vec2;
			}
			return dist;
		}
		template<typename T>
		size_t hummingDist(const Vector<T>& vec1, const Vector<T>& vec2) {
			if (vec1.size() != vec2.size()) throw ShapeMismatchException("");
			return humming(vec1.data(), vec2.data(), vec1.size());
		}

		template<typename T>
		inline T selectMin(T v1, T v2, T v3) {
			return (v1 <= v2 ? (v2 <= v3 ? v1 : (v1 <= v3 ? v1 : v3)) : (v1 <= v3 ? v2 : (v2 <= v3 ? v2 : v3)));

		}
		template<typename T>
		size_t levenshtein(const T* vec1, const size_t s1, const T* vec2, const size_t s2) {
			int cost;
			Matrix<size_t> mat(s1 + 1, s2 + 1);
			sforin(r, 0, mat.row) mat[r][0] = r;
			sforin(c, 0, mat.col) mat[0][c] = c;
			sforin(i1, 1, mat.row) {
				sforin(i2, 1, mat.col) {
					cost = vec1[i1] == vec2[i2] ? 0 : 1;
					mat[i1][i2] = selectMin(mat[i1 - 1][i2] + 1, mat[i1][i2 - 1] + 1, mat[i1 - 1][i2 - 1] + cost);
				}
			}
			return mat[s1][s2];
		}

		template<typename T>
		size_t levenshteinDist(const Vector<T>& vec1, const Vector<T>& vec2) {
			return levenshtein(vec1.data(), vec1.size(), vec2.data(), vec2.size());
		}

		template<typename T>
		size_t codeDistance(const Vector<T>& vec1, const Vector<T>& vec2, const char* method = "humming") {
			String m(method);
			if (m == "humming") return hummingDist(vec1, vec2);
			else if (m == "leven") return levenshteinDist(vec1, vec2);
			else throw FormatException(formatErrorText("Method name", method, "humming or leven"));
		}
	}
	namespace sgeom {
		template<typename T, size_t D>
		class Shape {
		public:
			smath::Vector<smath::VectorND<T, D>> vertex;
		public:
			Shape() {}
			Shape(const Shape<T, D>& shp) : Shape() { vertex = shp.vertex; }
			virtual ~Shape() {}
			virtual smath::VectorND<T, D> center() const {
				return vertex.empty() ? smath::VectorND<T, D>() : sstat::average(vertex); 
			}
			virtual T length() const { return (T)0; }
			virtual T area() const { return (T)0; }
			virtual T volume() const { return (T)0; }
		};
		template<typename T, size_t D>
		class Point : public Shape<T, D> {
		public:
			Point() : Shape<T, D>() { Shape<T, D>::vertex.resize(1); }
			Point(const smath::VectorND<T, D>& pt) : Shape<T, D>() { Shape<T, D>::vertex << pt; }
			Point(const Point &pt) : Shape<T, D>(pt) {}
			~Point() {}
			smath::VectorND<T, D> point() const { return Shape<T, D>::vertex[0]; }
		};
		template<typename T, size_t D>
		class Line : public Shape<T, D> {
		public:
			Line() : Shape<T, D>() { Shape<T, D>::vertex.resize(2); }
			Line(const smath::VectorND<T, D>& p1, const smath::VectorND<T, D>& p2) : Shape<T, D>() { Shape<T, D>::vertex << p1 << p2; }
			Line(const Line &ln) : Shape<T, D>(ln) {}
			~Line() {}
			smath::Vector2D<smath::VectorND<T, D>> vector() const {
				return smath::Vector2D<smath::VectorND<T, D>>(Shape<T, D>::vertex[0], Shape<T, D>::vertex[1]);
			}
			T length() const { return smath::length(Shape<T, D>::vertex[1] - Shape<T, D>::vertex[0]); }
		};
		template<typename T, size_t D>
		class Curve : public Shape<T, D> {
		public:
			Curve() : Shape<T, D>() { Shape<T, D>::vertex.resize(4); }
			Curve(const smath::VectorND<T, D>& p1, const smath::VectorND<T, D>& p2, const smath::VectorND<T, D>& p3, const smath::VectorND<T, D>& p4) 
				: Shape<T, D>() { Shape<T, D>::vertex << p1 << p2 << p3 << p4; }
			Curve(const Curve& cv) : Shape<T, D>(cv) {}
			~Curve() {}
		};
		enum class PATH_TYPE : subyte {
			LINE = 2,
			BEZIER2 = 3,
			BEZIER3 = 4,
		};
		template<typename T, size_t D>
		class Path : public Shape<T, D> {
			
		protected:
			smath::VectorND<T, D> _current;
			ubytearray _type;
			bool _loop;
		public:
			Path() { _loop = false; }
			Path(const Path<T, D> &path) : Shape<T, D>(path) {
				_current = path._current;
				_type = path._type;
				_loop = path._loop;
			}
			~Path() {}

			const ubytearray& pathtype() const { return _type; }
			bool isClosed() const { return _loop; }
			void moveTo(const smath::VectorND<T, D>& p) { _current = p; }
			void lineTo(const smath::VectorND<T, D>& p) { 
				Shape<T, D>::vertex << _current << p;
				_type.add((subyte)PATH_TYPE::LINE);
				_current = p; 
			}
			void curveTo(const smath::VectorND<T, D>& p, const smath::VectorND<T, D>& c1, const smath::VectorND<T, D>& c2) {
				Shape<T, D>::vertex << _current << c1 << c2 << p;
				_type.add((subyte)PATH_TYPE::BEZIER3);
				_current = p;
			}
			void backTo(const smath::VectorND<T, D>& p) {


			}
			void setClosed(const bool b) { _loop = b; }
		};
		template<typename T>
		class Rectangle : public Shape<T, 2> {
		public:
			Rectangle() : Shape<T, 2>() { Shape<T, 2>::vertex.resize(4); }
			Rectangle(const T x, const T y, const T w, const T h) : Shape<T, 2>() {
				Shape<T, 2>::vertex <<
					smath::VectorND<T, 2>(x, y) <<
					smath::VectorND<T, 2>(x, y + h) <<
					smath::VectorND<T, 2>(x + w, y + h) <<
					smath::VectorND<T, 2>(x + w, y);
			}
			Rectangle(const Area<T> &area) : Shape<T, 2>() {
				Shape<T, 2>::vertex <<
					smath::VectorND<T, 2>(area.ori_x, area.ori_y) <<
					smath::VectorND<T, 2>(area.ori_x, area.ori_y + area.height) <<
					smath::VectorND<T, 2>(area.ori_x + area.width, area.ori_y + area.height) <<
					smath::VectorND<T, 2>(area.ori_x + area.width, area.ori_y);
			}
			Rectangle(const Rectangle<T>& rect) : Shape<T, 2>(rect) {}
			~Rectangle() {}
			T width() const { return smath::length(Shape<T, 2>::vertex[3] - Shape<T, 2>::vertex[0]); }
			T height() const { return smath::length(Shape<T, 2>::vertex[1] - Shape<T, 2>::vertex[0]); }

			//T length() const;
			//T area() const;

		};
		/*
		template<typename T>
		class Plane : public Shape<T, 3> {
		public:
			Plane();
			Plane(const Plane<T>& pl);
			~Plane();
		};
		template<typename T>
		class Cube : public Shape<T, 3> {
		public:
			Cube();
			Cube(const Cube<T>& cube);
			~Cube();
		};
		*/
		template<typename T>
		class Ellipse : public Shape<T, 2> {
		public:
			Ellipse() : Shape<T, 2>() { Shape<T, 2>::vertex.resize(4); }
			Ellipse(const T x, const T y, const T w, const T h) : Shape<T, 2>() {
				Shape<T, 2>::vertex <<
					smath::VectorND<T, 2>(x, y) <<
					smath::VectorND<T, 2>(x, y + h) <<
					smath::VectorND<T, 2>(x + w, y + h) <<
					smath::VectorND<T, 2>(x + w, y);
			}
			Ellipse(const Area<T>& area) : Shape<T, 2>() {
				Shape<T, 2>::vertex <<
					smath::VectorND<T, 2>(area.ori_x, area.ori_y) <<
					smath::VectorND<T, 2>(area.ori_x, area.ori_y + area.height) <<
					smath::VectorND<T, 2>(area.ori_x + area.width, area.ori_y + area.height) <<
					smath::VectorND<T, 2>(area.ori_x + area.width, area.ori_y);
			}
			Ellipse(const Ellipse& elps) : Shape<T, 2>(elps) {}
			~Ellipse() {}
			//T length() const;
			//T area() const;
			smath::Vector2D<T> center() const {
				return (Shape<T, 2>::vertex[0] + Shape<T, 2>::vertex[2]) / 2;
			}
			smath::Vector2D<smath::Vector2D<T>> axis(smath::AXIS ax) {
				if (ax == smath::AXIS::X) return smath::Vector2D<smath::Vector2D<T>>(
					(Shape<T, 2>::vertex[0] + Shape<T, 2>::vertex[1]) / 2, 
					(Shape<T, 2>::vertex[2] + Shape<T, 2>::vertex[3]) / 2);
				else if (ax == smath::AXIS::Y) return smath::Vector2D<smath::Vector2D<T>>(
					(Shape<T, 2>::vertex[0] + Shape<T, 2>::vertex[3]) / 2, 
					(Shape<T, 2>::vertex[1] + Shape<T, 2>::vertex[2]) / 2);
				else throw RangeException(outRangeErrorText("index", (int)ax, 1, 2));
			}
			T width() const { return smath::length(Shape<T, 2>::vertex[3] - Shape<T, 2>::vertex[0]); }
			T height() const { return smath::length(Shape<T, 2>::vertex[1] - Shape<T, 2>::vertex[0]); }
		};
		/*
		template<typename T>
		class Tube : public Shape<T, 3> {
		protected:
			Shape _base;
		public:
			Tube();
			Tube(const Tube<T>& tube);
			~Tube();
		};
		template<typename T>
		class Sphere : public Shape<T, 3> {
		public:
			Sphere();
			Sphere(const Sphere<T>& sp);
			~Sphere();
		};
		*/
		template<typename T>
		class Arc : public Shape<T, 2> {
		public:
			smath::Vector2D<T> phase;
		public:
			Arc() : Shape<T, 2>() {
				Shape<T, 2>::vertex.resize(4);
				phase = smath::Vector2D<T>(0, 1);
			}
			Arc(const T x, const T y, const T w, const T h, const T start = 0, const T end = 1) : Shape<T, 2>() {
				Shape<T, 2>::vertex <<
					smath::VectorND<T, 2>(x, y) <<
					smath::VectorND<T, 2>(x, y + h) <<
					smath::VectorND<T, 2>(x + w, y + h) <<
					smath::VectorND<T, 2>(x + w, y);
				phase = smath::Vector2D<T>(start, end);
			}
			Arc(const Arc& arc) : Shape<T, 2>(arc) { phase = arc.phase; }
			~Arc() {}

			smath::Vector2D<T> center() const {
				return (Shape<T, 2>::vertex[0] + Shape<T, 2>::vertex[2]) / 2;
			}
			T width() const { return smath::length(Shape<T, 2>::vertex[3] - Shape<T, 2>::vertex[0]); }
			T height() const { return smath::length(Shape<T, 2>::vertex[1] - Shape<T, 2>::vertex[0]); }
		};
		template<typename T>
		class Polygon : public Shape<T, 2> {
		public:
			Polygon() : Shape<T, 2>() {}
			Polygon(const size_t s) : Shape<T, 2>() { resize(s); }
			Polygon(std::initializer_list<smath::VectorND<T, 2>> li) : Shape<T, 2>() {
				sfor(li) Shape<T, 2>::vertex << $_;
			}
			Polygon(const Polygon<T>& poly) : Shape<T, 2>(poly) {}
			~Polygon() {}
			void resize(const size_t sz) { Shape<T, 2>::vertex.resize(sz); }
			void addVertex(const smath::VectorND<T, 2>& pt) { Shape<T, 2>::vertex.add(pt); }
			void insertVertex(const int idx, const smath::VectorND<T, 2>& pt) { Shape<T, 2>::vertex.insert(idx, pt); }
			void removeVertex(const int idx) { Shape<T, 2>::vertex.removeAt(idx); }
			//smath::VectorND<T, D> center() const;
			//T length() const;
			//T area() const;
		};		
		/*
		template<typename T>
		class Polyhedron : public Shape<T, 3> {
		public:
			Polyhedron();
			Polyhedron(const Polygon<T>& rect);
			~Polyhedron();
		};
		*/
	}
	template<typename T>
	using Point2D = sgeom::Point<T, 2>;
	template<typename T>
	using Point3D = sgeom::Point<T, 3>;
	template<typename T>
	using Line2D = sgeom::Line<T, 2>;
	template<typename T>
	using Line3D = sgeom::Line<T, 3>;
	template<typename T>
	using Curve2D = sgeom::Curve<T, 2>;
	template<typename T>
	using Curve3D = sgeom::Curve<T, 3>;
	template<typename T>
	using Path2D = sgeom::Path<T, 2>;
	template<typename T>
	using Path3D = sgeom::Path<T, 3>;

}

/*

template<size_t D, typename T>
		extern inline double distance(const smath::SVectorND<T, D>& v1, const smath::SVectorND<T, D>& v2) {
			return sla::length(v1 - v2);
		}
		template<typename T>
		extern inline double direction(const smath::SVectorND<2, T>& vec,
			const smath::SVectorND<2, T>& axis = smath::SVectorND<2, T>(1, 0)) {
			return sla::argument(vec) - sla::argument(axis);
		}
		template<typename T>
		extern smath::SVectorND<2, double> direction(const smath::SVectorND<3, T>& vec,
			const smath::SVectorND<3, T>& axis = smath::SVectorND<3, T>(1, 0)) {
			return smath::SVectorND<2, double>(sla::azimuth(vec) - sla::azimuth(axis), sla::altitude(vec) - sla::altitude(axis));
		}
		template<typename T, class M>
		extern inline double distance(const smath::SVector<T, M>& v1, const smath::SVector<T, M>& v2) {
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
		
		typedef enum {
			ZERO_ORIGIN = 0,
			UPPER_LEFT = 1,
			LOWER_LEFT = 2,
			UPPER_RIGHT = 3,
			LOWER_RIGHT = 4,
			MASS_CENTER = 5,
		} ORIGIN;
		template<size_t D, typename T>
		extern inline smath::SVectorND<T, D> midpoint(const smath::SVectorND<T, D>& v1, const smath::SVectorND<T, D>& v2) { return (v1 + v2) / 2; }
		template<typename T>
		extern inline smath::SVectorND<2, T> center(const Area<T>& area) {
			return smath::SVectorND<2, T>(area.ori_x + area.width / 2, area.ori_y + area.height / 2);
		}
		template<typename T>
		extern inline smath::SVectorND<3, T> center(const Zone<T>& zone) {
			return smath::SVectorND<3, T>(zone.ori_x + zone.width / 2, zone.ori_y + zone.height / 2, zone.ori_z + zone.depth / 2);
		}
		

		template<typename T>
		extern inline void rotate(smath::SVectorND<3, T>& vec, const T& r, smath::AXIS ax) {
			smath::SVectorND<3, T> v_;
			switch (ax) {
			case smath::X_AXIS:
				v_ = smath::SVectorND<3, T>(vec[0], cos(r) * vec[1] - sin(r) * vec[2], sin(r) * vec[1] + cos(r) * vec[2]);
				break;
			case smath::Y_AXIS:
				v_ = smath::SVectorND<3, T>(cos(r) * vec[0] + sin(r) * vec[2], vec[1], -sin(r) * vec[0] + cos(r) * vec[2]);
				break;
			case smath::Z_AXIS:
				v_ = smath::SVectorND<3, T>(cos(r) * vec[0] - sin(r) * vec[1], sin(r) * vec[0] + cos(r) * vec[1], vec[2]);
				break;
			}
			vec = v_;
		}
		template<size_t D, typename T>
		extern inline void shear(smath::SVectorND<T, D>& vec, const smath::SVectorND<T, D>& sh) {
			sforin(d, 0, D) {
				sforin(e, 0, D) {
					if (d == e) continue;
					else vec[d] += vec[e] * sh[e];
				}
				
			}
			vec[0] += vec[1] * sh[0]; vec[1] += vec[0] * sh[1];
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

		
}
*/
#endif