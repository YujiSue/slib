#ifndef SLIB_ZONE_H
#define SLIB_ZONE_H
#include "sconfig.h"
namespace slib {
    /**
    * @class Zone
    * \~english @brief 3D zone class
    * \~japanese @brief 三次元領域クラス
    */
    template<typename T>
    class Zone {
    public:
        T ori_x, ori_y, ori_z, width, height, depth;
    public:
        Zone() : ori_x(0), ori_y(0), ori_z(0), width(0), height(0), depth(0) {}
        Zone(const T x, const T y, const T z, const T w, const T h, const T d)
            : ori_x(x), ori_y(y), ori_z(z), width(w), height(h), depth(d) {}
        Zone(const Zone<T> &z)
            : ori_x(z.ori_x), ori_y(z.ori_y), ori_z(z.ori_z), width(z.width), height(z.height), depth(z.depth) {}
        ~Zone() {}
        Zone<T>&operator =(const Zone<T>&z) {
            ori_x = z.ori_x; ori_y = z.ori_y; ori_z = z.ori_z;
            width = z.width; height = z.height; depth = z.depth;
            return *this;
        }
        T volume() const { return width * height * depth; }
        bool include(const T &x, const T &y, const T &z) const {
            return ori_x <= x && x <= (ori_x + width) &&
                ori_y <= y && y <= (ori_y + height) &&
                ori_z <= z && z <= (ori_z + depth);
        }
        bool include(const Zone<T>&z) const {
            return ori_x <= z.ori_x && (z.ori_x + z.width) <= (ori_x + width) &&
                ori_y <= z.ori_y && (z.ori_y + z.height) <= (ori_y + height) &&
                ori_z <= z.ori_z && (z.ori_z + z.depth) <= (ori_z + depth);
        }
        bool overlap(const Zone<T>&z) const {
            return ori_x <= (z.ori_x + z.width) && z.ori_x <= (ori_x + width) &&
                ori_y <= (z.ori_y + z.height) && z.ori_y <= (ori_y + height) &&
                ori_z <= (z.ori_z + z.depth) && z.ori_z <= (ori_z + depth);
        }
		void shift(const T s1, const T s2, const T s3) {
            ori_x += s1; ori_y += s2; ori_z += s3;
        }
        void expand(const T m1, const T m2, const T m3) {
            width += m1; height += m2; depth += m3; 
        }
        void merge(const Zone<T>& z) {
            T x1_ = ori_x < z.ori_x ? ori_x : z.ori_x,
                y1_ = ori_y < z.ori_y ? ori_y : z.ori_y,
                z1_ = ori_z < z.ori_z ? ori_z : z.ori_z,
                x2_ = (ori_x + width) < (z.ori_x + z.width) ? (z.ori_x + z.width) : (ori_x + width),
                y2_ = (ori_y + height) < (z.ori_y + z.height) ? (z.ori_y + z.height) : (ori_y + height),
                z2_ = (ori_z + depth) < (z.ori_z + z.depth) ? (z.ori_z + z.depth) : (ori_z + depth);
            ori_x = x1_; ori_y = y1_; ori_z = z1_; width = x2_ - x1_; height = y2_ - y1_; depth = z2_ - z1_;
        }
        void mask(const Zone<T>& z) {
            if (include(z)) *this = z;
            else if (overlap(z)) {
                T x1_ = ori_x < z.ori_x ? z.ori_x : ori_x,
                    y1_ = ori_y < z.ori_y ? z.ori_y : ori_y,
                    z1_ = ori_z < z.ori_z ? z.ori_z : ori_z,
                    x2_ = (ori_x + width) < (z.ori_x + z.width) ? (ori_x + width) : (z.ori_x + z.width),
                    y2_ = (ori_y + height) < (z.ori_y + z.height) ? (ori_y + height) : (z.ori_y + z.height),
                    z2_ = (ori_z + depth) < (z.ori_z + z.depth) ? (ori_z + depth) : (z.ori_z + z.depth);
                ori_x = x1_; ori_y = y1_; ori_z = z1_; width = x2_ - x1_; height = y2_ - y1_; depth = z2_ - z1_;
            }
            else *this = Zone<T>();
        }
        bool operator < (const Zone<T>&z) const {
            if (ori_x != z.ori_x) return ori_x < z.ori_x;
            if (ori_y != z.ori_y) return ori_y < z.ori_y;
            if (ori_z != z.ori_z) return ori_z < z.ori_z;
            if (width != z.width) return width < z.width;
            if (height != z.height) return height < z.height;
            if (depth != z.depth) return depth < z.depth;
        }
        bool operator == (const Zone<T>&z) const {
            return ori_x == z.ori_x && ori_y == z.ori_y && ori_z == z.ori_z &&
                width == z.width && height == z.height && depth == z.depth;
        }
        bool operator != (const Zone<T>&z) const { return !((*this) == z); }
    };
    template<typename T>
    Zone<T> shift(const Zone<T>& zone, const T s1, const T s2, const T s3) {
        Zone<T> z(zone); z.shift(s1, s2, s3); return z;
    }
    template<typename T>
    Zone<T> expand(const Zone<T>& zone, const T e1, const T e2, const T e3) {
        Zone<T> z(zone); z.expand(e1, e2, e3); return z;
    }
    template<typename T>
    Zone<T> merge(const Zone<T>& zone1, const Zone<T>& zone2) {
        Zone<T> z(zone1); z.merge(zone2); return z;
    }
    template<typename T>
    Zone<T> mask(const Zone<T>& zone1, const Zone<T>& zone2) {
        Zone<T> z(zone1); z.mask(zone2); return z;
    }
	using szone = Zone<sint>;
	using szonef = Zone<float>;
	using szoned = Zone<double>;
}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::Zone<T>& zone) { return os << "(" << zone.ori_x << ", " << zone.ori_y << ", " << zone.ori_z << ", " << zone.width << ", " << zone.height << ", " << zone.depth << ")"; }

#endif
