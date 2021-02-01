#ifndef SLIB_AREA_H
#define SLIB_AREA_H

#include "sconfig.h"

namespace slib {
    template<typename T>
    struct Area {
        T ori_x, ori_y, width, height;
        
        Area();
        Area(T x, T y, T w, T h);
        Area(const Area &a);
        ~Area();
        Area &operator = (const Area &a);
		Area& operator |= (const Area& area);
		Area& operator &= (const Area& area);
		Area operator | (const Area& area) const;
		Area operator & (const Area& area) const;
        T area() const;
        bool include(const T &x, const T &y) const;
        bool include(const Area &a) const;
        bool overlap(const Area &a) const;
		Area& shift(const T& s1, const T& s2);
		Area& magnify(const T& m1, const T& m2);
		Area& merge(const Area& rng);
		Area& mask(const Area& rng);
        bool operator < (const Area &a) const;
        bool operator == (const Area &a) const;
        bool operator != (const Area &a) const;
    };
	template<typename T>
	extern std::ostream& operator<<(std::ostream& os, const Area<T>& area) { return os << "(" << area.ori_x << ", " << area.ori_y << ", " << area.width << ", " << area.height << ")"; }

	using sarea = Area<sint>;
	using sareaf = Area<float>;
	using saread = Area<double>;

    /*============================================================*/
    template<typename T>
    Area<T>::Area() : ori_x(initVal<T>()), ori_y(initVal<T>()), width(initVal<T>()), height(initVal<T>()) {}
    template<typename T>
    Area<T>::Area(T x, T y, T w, T h) : ori_x(x), ori_y(y), width(w), height(h) {}
    template<typename T>
    Area<T>::Area(const Area &a) : ori_x(a.ori_x), ori_y(a.ori_y), width(a.width), height(a.height) {}
    template<typename T>
    Area<T>::~Area() {}
    template<typename T>
    Area<T> &Area<T>::operator =(const Area<T> &a) {
        ori_x = a.ori_x; ori_y = a.ori_y; width = a.width; height = a.height; return *this;
    }
	template<typename T>
	Area<T>& Area<T>::operator |= (const Area<T>& area) { return this->merge(area); }
	template<typename T>
	Area<T>& Area<T>::operator &= (const Area<T>& area) { return this->mask(area); }
	template<typename T>
	Area<T> Area<T>::operator | (const Area& area) const { return Area<T>(*this).merge(area); }
	template<typename T>
	Area<T> Area<T>::operator & (const Area& area) const { return Area<T>(*this).mask(area); }
    template<typename T>
	T Area<T>::area() const { return width * height; }
    template<typename T>
	bool Area<T>::include(const T &x, const T &y) const {
		return ori_x <= x && x < (ori_x + width) && ori_y <= y && y < (ori_y + height);
    }
    template<typename T>
	bool Area<T>::include(const Area<T> &a) const {
		return ori_x <= a.ori_x && (a.ori_x + a.width) <= (ori_x + width) &&
			ori_y <= a.ori_y && (a.ori_y + a.height) <= (ori_y + height);
    }
    template<typename T>
	bool Area<T>::overlap(const Area<T> &a) const {
		return ori_x <= (a.ori_x + a.width) && a.ori_x <= (ori_x + width) &&
			ori_y <= (a.ori_y + a.height) && a.ori_y <= (ori_y + height);
    }
	template<typename T>
	Area<T>& Area<T>::shift(const T& s1, const T& s2) {
		ori_x += s1; ori_y += s2; return *this;
	}
	template<typename T>
	Area<T>& Area<T>::magnify(const T& m1, const T& m2) {
		width *= m1; height *= m2; return *this;
	}
    template<typename T>
	Area<T> &Area<T>::merge(const Area<T> &a) {
		T x1_ = ori_x < a.ori_x ? ori_x : a.ori_x,
			y1_ = ori_y < a.ori_y ? ori_y : a.ori_y,
			x2_ = (ori_x + width) < (a.ori_x + a.width) ? (a.ori_x + a.width) : (ori_x + width),
			y2_ = (ori_y + height) < (a.ori_y + a.height) ? (a.ori_y + a.height) : (ori_y + height);
		ori_x = x1_; ori_y = y1_; width = x2_ - x1_; height = y2_ - y1_;
		return *this;
    }
    template<typename T>
	Area<T> &Area<T>::mask(const Area<T> &a) {
		if (include(a)) *this = a;
        else if (overlap(a)) {
			T x1_ = ori_x < a.ori_x ? a.ori_x : ori_x,
				y1_ = ori_y < a.ori_y ? a.ori_y : ori_y,
				x2_ = (ori_x + width) < (a.ori_x + a.width) ? (ori_x + width) : (a.ori_x + a.width),
				y2_ = (ori_y + height) < (a.ori_y + a.height) ? (ori_y + height) : (a.ori_y + a.height);
			ori_x = x1_; ori_y = y1_; width = x2_ - x1_; height = y2_ - y1_;
        }
        else *this = Area<T>();
		return *this;
    }
    template<typename T>
	bool Area<T>::operator < (const Area<T> &a) const {
        if (ori_x != a.ori_x) return ori_x < a.ori_x;
        if (ori_y != a.ori_y) return ori_y < a.ori_y;
        if (width != a.width) return width < a.width;
        if (height != a.height) return height < a.height;
        return false;
    }
    template<typename T>
	bool Area<T>::operator == (const Area<T> &a) const {
		return ori_x == a.ori_x && ori_y == a.ori_y && width == a.width && height == a.height;
    }
    template<typename T>
    bool Area<T>::operator != (const Area<T> &a) const { return !(*this == a); }
}
#endif
