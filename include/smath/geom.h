#ifndef SMATH_GEOM_H
#define SMATH_GEOM_H

#include "smath/calc.h"
#include "smath/la.h"

namespace slib {
    namespace smath {
		namespace sgeom {
			typedef enum {
				XYZ = 1,
				POLAR = 2,
				CYLINDER = 3,
				SPHERE = 4,
			} COORDINATE;

			typedef enum {
				ZERO_ORI = 0,
				UP_LEFT = 1,
				DOWN_LEFT = 2,
				UP_RIGHT = 3,
				DOWN_RIGHT = 4,
				MASS_CENTER = 5,
			} ORIGIN;

			extern inline double length(const v2i& p) { return sqrt(p.x * p.x + p.y * p.y); }
			extern inline double length(const v2f& p) { return sqrt(p.x * p.x + p.y * p.y); }
			extern inline double length(const v2d& p) { return sqrt(p.x * p.x + p.y * p.y); }
			extern inline double distance(const v2i& p1, const v2i& p2) { return length(p1 - p2); }
			extern inline double distance(const v2f& p1, const v2f& p2) { return length(p1 - p2); }
			extern inline double distance(const v2d& p1, const v2d& p2) { return length(p1 - p2); }
			extern inline double slope(const v2i& p) {
				if (0.0 < abs(p.x)) return (double)p.y / p.x;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline double slope(const v2f& p) {
				if (0.0 < abs(p.x)) return p.y / p.x;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline double slope(const v2d& p) {
				if (0.0 < abs(p.x)) return p.y / p.x;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}

			extern inline double argument(const v2i& p) {
				auto len = length(p);
				if (0.0 < abs(len)) return p.y < 0 ? 2 * smath::PI - acos((double)p.x / len) : acos((double)p.x / len);
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline double argument(const v2f& p) {
				auto len = length(p);
				if (0.0 < abs(len)) return p.y < 0 ? 2 * smath::PI - acos(p.x / len) : acos(p.x / len);
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline double argument(const v2d& p) {
				auto len = length(p);
				if (0.0 < abs(len)) return p.y < 0 ? 2 * smath::PI - acos(p.x / len) : acos(p.x / len);
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline double direction(const v2i& p1, const v2i& p2) { return argument(p2) - argument(p1); }
			extern inline double direction(const v2f& p1, const v2f& p2) { return argument(p2) - argument(p1); }
			extern inline double direction(const v2d& p1, const v2d& p2) { return argument(p2) - argument(p1); }
			extern inline void normalize(v2f& p) {
				auto len = length(p); if (0.0 < abs(len)) p /= (float)len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline void normalize(v2d& p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
			extern inline v2f xy2polar(const v2f& xy) { return v2f(length(xy), argument(xy)); }
			extern inline v2d xy2polar(const v2d& xy) { return v2d(length(xy), argument(xy)); }
			extern inline v2f polar2xy(const v2f& pol) { return v2f(pol.x * cos(pol.y), pol.x * sin(pol.y)); }
			extern inline v2d polar2xy(const v2d& pol) { return v2d(pol.x * cos(pol.y), pol.x * sin(pol.y)); }

			extern inline v2f trans(const v2f& p, const v2f& dif) { return p + dif; }
			extern inline v2d trans(const v2d& p, const v2d& dif) { return p + dif; }
			extern inline v2f scale(const v2f& p, const v2f& ratio) { return v2f(p.x * ratio.x, p.y * ratio.y); }
			extern inline v2d scale(const v2d& p, const v2d& ratio) { return v2f(p.x * ratio.x, p.y * ratio.y); }
			extern inline v2f rotate(const v2f& p, const float& rad) {
				return mat2f((float)cos(rad), (float)-sin(rad), (float)sin(rad), (float)cos(rad)) * p;
			}
			extern inline v2d rotate(const v2d& p, const double& rad) {
				return mat2d(cos(rad), -sin(rad), sin(rad), cos(rad)) * p;
			}
			extern inline v2f skew(const v2f& p, const float& ratio, DIRECTION dir) {
				if (dir == HORIZONTAL) return v2f(p.x + p.y * ratio, p.y);
				else return v2f(p.x, p.x * ratio + p.y);
			}
			extern inline v2d skew(const v2d& p, const double& ratio, DIRECTION dir) {
				if (dir == HORIZONTAL) return v2f(p.x + p.y * ratio, p.y);
				else return v2f(p.x, p.x * ratio + p.y);
			}
			extern inline v2f affine2d(const v2f& pos, const v2f& trans, const v2f& scale, const float& rot, const float& ratio, DIRECTION dir) {
				return mat2f((float)cos(rot), (float)-sin(rot), (float)sin(rot), (float)cos(rot)) *
					mat2f(scale.x, 0, 0, scale.y) *
					mat2f(1.0f, dir == HORIZONTAL ? ratio : 0.0f, dir == VERTICAL ? ratio : 0.0f, 1.0f) *
					pos + trans;
			}
			extern inline v2d affine2d(const v2d& pos, const v2d& trans, const v2d& scale, const double& rot, const double& ratio, DIRECTION dir) {
				return mat2d({ cos(rot), -sin(rot), sin(rot), cos(rot) }) *
					mat2d({ scale.x, 0, 0, scale.y }) *
					mat2d({ 1.0, dir == HORIZONTAL ? ratio : 0.0, dir == VERTICAL ? ratio : 0.0, 1.0 }) * pos + trans;
			}

			extern inline double length(const v3i& p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z); }
			extern inline double length(const v3f& p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z); }
			extern inline double length(const v3d& p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z); }
			extern inline double azimuth(const v3i& p) { return argument(p.projection(XY_PLANE)); }
			extern inline double azimuth(const v3f& p) { return argument(p.projection(XY_PLANE)); }
			extern inline double azimuth(const v3d& p) { return argument(p.projection(XY_PLANE)); }
			extern inline double altitude(const v3i& p) { return (p.z < 0.0 ? -1.0 : 1.0) * acos(length(p.projection(XY_PLANE)) / length(p)); }
			extern inline double altitude(const v3f& p) { return (p.z < 0.0 ? -1.0 : 1.0) * acos(length(p.projection(XY_PLANE)) / length(p)); }
			extern inline double altitude(const v3d& p) { return (p.z < 0.0 ? -1.0 : 1.0) * acos(length(p.projection(XY_PLANE)) / length(p)); }
			extern inline v2d direction(const v3i& p1, const v3i& p2) { return v2d(azimuth(p2 - p1), altitude(p2 - p1)); }
			extern inline v2d direction(const v3f& p1, const v3f& p2) { return v2d(azimuth(p2 - p1), altitude(p2 - p1)); }
			extern inline v2d direction(const v3d& p1, const v3d& p2) { return v2d(azimuth(p2 - p1), altitude(p2 - p1)); }
			extern inline void normalize(v3f& p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
            extern inline void normalize(v3d &p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
            extern inline double distance(const v3i &p1, const v3i &p2) { return length(p1 - p2); }
            extern inline double distance(const v3f &p1, const v3f &p2) { return length(p1 - p2); }
            extern inline double distance(const v3d &p1, const v3d &p2) { return length(p1 - p2); }
            
            extern inline v3f xyz2sphere(const v3f &xyz) { return v3f(length(xyz), azimuth(xyz), altitude(xyz)); }
            extern inline v3d xyz2sphere(const v3d &xyz) { return v3d(length(xyz), azimuth(xyz), altitude(xyz)); }
            extern inline v3f sphere2xyz(const v3f &sph) { return v3f(sph.x * cos(sph.y) * cos(sph.z), sph.x * sin(sph.y) * cos(sph.z), sph.x * sin(sph.z)); }
            extern inline v3d sphere2xyz(const v3d &sph) { return v3d(sph.x * cos(sph.y) * cos(sph.z), sph.x * sin(sph.y) * cos(sph.z), sph.x * sin(sph.z)); }
            extern inline v3f xyz2tube(const v3f &xyz) { return v3f(length(xyz), azimuth(xyz), xyz.z); }
            extern inline v3d xyz2tube(const v3d &xyz) { return v3d(length(xyz), azimuth(xyz), xyz.z); }
            extern inline v3f tube2xy(const v3f &tb) { return v3f(tb.x * cos(tb.y), tb.x * sin(tb.y), tb.z); }
            extern inline v3d tube2xy(const v3d &tb) { return v3d(tb.x * cos(tb.y), tb.x * sin(tb.y), tb.z); }
			
            extern inline double length(const v4i &p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w); }
            extern inline double length(const v4f &p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w); }
            extern inline double length(const v4d &p) { return sqrt(p.x * p.x + p.y * p.y + p.z * p.z + p.w * p.w); }
            extern inline void normalize(v4f &p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
            extern inline void normalize(v4d &p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "p");
			}
            extern inline double distance(const v4i &p1, const v4i &p2) { return length(p1 - p2); }
            extern inline double distance(const v4f &p1, const v4f &p2) { return length(p1 - p2); }
            extern inline double distance(const v4d &p1, const v4d &p2) { return length(p1 - p2); }
            
            extern inline double length(const sveci &p) {
				double len = 0.0; sforeach(p) { len += E_ * E_; } return sqrt(len);
			}
            extern inline double length(const svecf &p) {
				double len = 0.0; sforeach(p) { len += E_ * E_; } return sqrt(len);
			}
            extern inline double length(const svecd &p) {
				double len = 0.0; sforeach(p) { len += E_ * E_; } return sqrt(len);
			}
            extern inline double distance(const sveci&p1, const sveci&p2) {
				if (p1.size() != p2.size())
					throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "p1, p2", DIMENSION_SIZE_DIFF(p1.size(), p2.size()));
				return length(p2 - p1);
			}
            extern inline double distance(const svecf &p1, const svecf &p2) {
				if (p1.size() != p2.size())
					throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "p1, p2", DIMENSION_SIZE_DIFF(p1.size(), p2.size()));
				return length(p2 - p1);
			}
            extern inline double distance(const svecd &p1, const svecd &p2) {
				if (p1.size() != p2.size())
					throw SMathException(ERR_INFO, DIMENSION_SIZE_ERR, "p1, p2", DIMENSION_SIZE_DIFF(p1.size(), p2.size()));
				return length(p2 - p1);
			}
            extern inline double lpnorm(const sveci &p) {
				double len = 0.0; sforeach(p) { len += pow(abs(E_), p.size()); } return rootN(len, (int)p.size());
			}
            extern inline double lpnorm(const svecf &p) {
				double len = 0.0; sforeach(p) { len += pow(abs(E_), p.size()); } return rootN(len, (int)p.size());
			}
            extern inline double lpnorm(const svecd &p) {
				double len = 0.0; sforeach(p) { len += pow(abs(E_), p.size()); } return rootN(len, (int)p.size());
			}
            extern inline void normalize(svecf &p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "len");
			}
            extern inline void normalize(svecd &p) {
				auto len = length(p); if (0.0 < abs(len)) p /= len;
				throw SMathException(ERR_INFO, DIV_ZERO_ERR, "len");
			}
            
            extern inline double length(const v2ivec &v) {
				double len = 0.0;
				sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
				return len;
			}
            extern inline double length(const v2fvec &v) {
				double len = 0.0;
				sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
				return len;
			}
            extern inline double length(const v2dvec &v) {
				double len = 0.0;
				sforin(it, v.begin() + 1, v.end()) len += length((*it) - (*(it - 1)));
				return len;
			}
        };
    }
}

#endif
