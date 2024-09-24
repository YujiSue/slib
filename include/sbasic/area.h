#ifndef SLIB_AREA_H
#define SLIB_AREA_H
#include "sconfig.h"
namespace slib {
	/**
	* @class Area
	* \~english @brief 2D area class 
	* \~japanese @brief 二次元領域クラス
	*/
	template<typename T>
    class Area {
	public:
		/**
		* \~english @brief X-coordinate of the origin
		* \~japanese @brief 始点のX座標
		*/
		T ori_x;
		/**
		* \~english @brief Y-coordinate of the origin
		* \~japanese @brief 始点のY座標
		*/
		T ori_y;
		/**
		* \~english @brief Width
		* \~japanese @brief 幅
		*/
		T width;
		/**
		* \~english @brief Height
		* \~japanese @brief 高さ
		*/
		T height;
	public:
        Area() : ori_x(T()), ori_y(T()), width(T()), height(T()) {}
        Area(const T x, const T y, const T w, const T h) : ori_x(x), ori_y(y), width(w), height(h) {}
        Area(const Area<T> &a) : ori_x(a.ori_x), ori_y(a.ori_y), width(a.width), height(a.height) {}
        ~Area() {}
        Area<T>&operator = (const Area<T>&a) {
			ori_x = a.ori_x; ori_y = a.ori_y; width = a.width; height = a.height; return *this;
		}
		Area<T>& operator |= (const Area<T>& area) { return this->merge(area); }
		Area<T>& operator &= (const Area<T>& area) { return this->mask(area); }
		Area<T> operator | (const Area<T>& area) const { return Area<T>(*this).merge(area); }
		Area<T> operator & (const Area<T>& area) const { return Area<T>(*this).mask(area); }
		T area() const { return width * height; }
        bool include(const T &x, const T &y) const {
			return ori_x <= x && x < (ori_x + width) && ori_y <= y && y < (ori_y + height);
		}
        bool include(const Area<T>&a) const {
			return ori_x <= a.ori_x && (a.ori_x + a.width) <= (ori_x + width) &&
				ori_y <= a.ori_y && (a.ori_y + a.height) <= (ori_y + height);
		}
		bool overlap(const Area<T>&a) const {
			return ori_x <= (a.ori_x + a.width) && a.ori_x <= (ori_x + width) &&
				ori_y <= (a.ori_y + a.height) && a.ori_y <= (ori_y + height);
		}
		void shift(const T& s1, const T& s2) { ori_x += s1; ori_y += s2; }
		void expand(const T& e1, const T& e2) { width += e1; height += e2; }
		void merge(const Area<T>& a) {
			T x1_ = ori_x < a.ori_x ? ori_x : a.ori_x,
				y1_ = ori_y < a.ori_y ? ori_y : a.ori_y,
				x2_ = (ori_x + width) < (a.ori_x + a.width) ? (a.ori_x + a.width) : (ori_x + width),
				y2_ = (ori_y + height) < (a.ori_y + a.height) ? (a.ori_y + a.height) : (ori_y + height);
			ori_x = x1_; ori_y = y1_; width = x2_ - x1_; height = y2_ - y1_;
		}
		void mask(const Area<T>& a) {
			if (include(a)) *this = a;
			else if (overlap(a)) {
				T x1_ = ori_x < a.ori_x ? a.ori_x : ori_x,
					y1_ = ori_y < a.ori_y ? a.ori_y : ori_y,
					x2_ = (ori_x + width) < (a.ori_x + a.width) ? (ori_x + width) : (a.ori_x + a.width),
					y2_ = (ori_y + height) < (a.ori_y + a.height) ? (ori_y + height) : (a.ori_y + a.height);
				ori_x = x1_; ori_y = y1_; width = x2_ - x1_; height = y2_ - y1_;
			}
			else *this = Area<T>();
		}
        bool operator < (const Area<T>&a) const {
			if (ori_x != a.ori_x) return ori_x < a.ori_x;
			if (ori_y != a.ori_y) return ori_y < a.ori_y;
			if (width != a.width) return width < a.width;
			if (height != a.height) return height < a.height;
			return false;
		}
        bool operator == (const Area<T>&a) const {
			return ori_x == a.ori_x && ori_y == a.ori_y && width == a.width && height == a.height;
		}
        bool operator != (const Area<T>&a) const { return !(*this == a); }
    };
	
	template<typename T>
	Area<T> shift(const Area<T>& area, const T& s1, const T& s2) { Area<T> a(area); a.shift(s1, s2); return a; }
	template<typename T>
	Area<T> expand(const Area<T>& area, const T& m1, const T& m2) { Area<T> a(area); a.expand(m1, m2); return a; }
	template<typename T>
	Area<T> merge(const Area<T>& area1, const Area<T>& area2) { Area<T> a(area1); a.merge(area2); return a; }
	template<typename T>
	Area<T> mask(const Area<T>& area1, const Area<T>& area2) { Area<T> a(area1); a.mask(area2); return a; }

	using sarea = Area<sint>;
	using sareal = Area<sinteger>;
	using sareaf = Area<float>;
	using saread = Area<double>;
	using sareas = Area<size_t>;
}
template<typename T>
extern std::ostream& operator<<(std::ostream& os, const slib::Area<T>& area) { return os << "(" << area.ori_x << ", " << area.ori_y << ", " << area.width << ", " << area.height << ")"; }

#endif
