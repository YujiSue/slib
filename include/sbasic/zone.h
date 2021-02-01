#ifndef SLIB_ZONE_H
#define SLIB_ZONE_H

#include "sconfig.h"

namespace slib {
    template<typename T>
    struct Zone {
        T ori_x, ori_y, ori_z, width, height, depth;
        
        Zone();
        Zone(const T &x, const T &y, const T &z, const T &w, const T &h, const T &d);
        Zone(const Zone &z);
        ~Zone();
        Zone &operator =(const Zone &z);
        T volume() const;
        bool include(const T &x, const T &y, const T &z) const;
        bool include(const Zone &z) const;
        bool overlap(const Zone &z) const;
		Zone& shift(const T& s1, const T& s, const T& s3);
		Zone& magnify(const T& m1, const T& m2, const T& m3);
		Zone& merge(const Zone& zone);
		Zone& mask(const Zone& zone);
        bool operator < (const Zone &z) const;
        bool operator == (const Zone &z) const;
        bool operator != (const Zone &z) const;
    };
	template<typename T>
	extern std::ostream& operator<<(std::ostream& os, const Zone<T>& zone) { return os << "(" << zone.ori_x << ", " << zone.ori_y << ", " << zone.ori_z << ", " << zone.width << ", " << zone.height << ", " << zone.depth << ")"; }

	using szone = Zone<sint>;
	using szonef = Zone<float>;
	using szoned = Zone<double>;
    /*============================================================*/
    template<typename T>
    Zone<T>::Zone()
    : ori_x(initVal<T>()), ori_y(initVal<T>()), ori_z(initVal<T>()),
    width(initVal<T>()), height(initVal<T>()), depth(initVal<T>()) {}
    template<typename T>
    Zone<T>::Zone(const T &x, const T &y, const T &z, const T &w, const T &h, const T &d)
    : ori_x(x), ori_y(y), ori_z(z), width(w), height(h), depth(d) {}
    template<typename T>
    Zone<T>::Zone(const Zone &z)
    : ori_x(z.ori_x), ori_y(z.ori_y), ori_z(z.ori_z), width(z.width), height(z.height), depth(z.depth) {}
    template<typename T>
    Zone<T>::~Zone() {}
    template<typename T>
    Zone<T> &Zone<T>::operator=(const Zone<T> &z) {
        ori_x = z.ori_x; ori_y = z.ori_y; ori_z = z.ori_z;
        width = z.width; height = z.height; depth = z.depth;
        return *this;
    }
    template<typename T>
	inline T Zone<T>::volume() const { return width * height * depth; }
    template<typename T>
	inline bool Zone<T>::include(const T &x, const T &y, const T &z) const {
        return ori_x <= x && x <= (ori_x+width) &&
        ori_y <= y && y <= (ori_y+height) &&
        ori_z <= z && z <= (ori_z+depth);
    }
    template<typename T>
	inline bool Zone<T>::include(const Zone<T> &z) const {
        return ori_x <= z.ori_x && (z.ori_x + z.width) <= (ori_x + width) &&
        ori_y <= z.ori_y && (z.ori_y + z.height) <= (ori_y + height) &&
        ori_z <= z.ori_z && (z.ori_z + z.depth) <= (ori_z + depth);
    }
    template<typename T>
	inline bool Zone<T>::overlap(const Zone<T> &z) const {
        return ori_x <= (z.ori_x + z.width) && z.ori_x <= (ori_x + width) &&
        ori_y <= (z.ori_y + z.height) && z.ori_y <= (ori_y + height) &&
        ori_z <= (z.ori_z + z.depth) && z.ori_z <= (ori_z + depth);
    }
	template<typename T>
	Zone<T>& Zone<T>::shift(const T& s1, const T& s2, const T& s3) {
		ori_x += s1; ori_y += s2; ori_z += s3; return *this;
	}
	template<typename T>
	Zone<T>& Zone<T>::magnify(const T& m1, const T& m2, const T& m3) {
		width *= m1; height *= m2; depth *= m3; return *this;
	}
	template<typename T>
	Zone<T>& Zone<T>::merge(const Zone<T>& z) {
		T x1_ = ori_x < z.ori_x ? ori_x : z.ori_x,
			y1_ = ori_y < z.ori_y ? ori_y : z.ori_y,
			z1_ = ori_z < z.ori_z ? ori_z : z.ori_z,
			x2_ = (ori_x + width) < (z.ori_x + z.width) ? (z.ori_x + z.width) : (ori_x + width),
			y2_ = (ori_y + height) < (z.ori_y + z.height) ? (z.ori_y + z.height) : (ori_y + height),
			z2_ = (ori_z + depth) < (z.ori_z + z.depth) ? (z.ori_z + z.depth) : (ori_z + depth);
		ori_x = x1_; ori_y = y1_; ori_z = z1_; width = x2_ - x1_; height = y2_ - y1_; depth = z2_ - z1_;
		return *this;
	}
	template<typename T>
	Zone<T>& Zone<T>::mask(const Zone<T>& z) {
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
		return *this;
    }
    template<typename T>
	inline bool Zone<T>::operator < (const Zone<T> &z) const {
        if(ori_x != z.ori_x) return ori_x < z.ori_x;
        if(ori_y != z.ori_y) return ori_y < z.ori_y;
        if(ori_z != z.ori_z) return ori_z < z.ori_z;
        if(width != z.width) return width < z.width;
        if(height != z.height) return height < z.height;
        if(depth != z.depth) return depth < z.depth;
    }
    template<typename T>
	inline bool Zone<T>::operator == (const Zone<T> &z) const {
        return ori_x == z.ori_x && ori_y == z.ori_y && ori_z == z.ori_z &&
        width == z.width && height == z.height && depth == z.depth;
    }
    template<typename T>
	inline bool Zone<T>::operator != (const Zone<T> &z) const { return (*this) != z; }
}
#endif
