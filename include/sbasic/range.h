#ifndef SLIB_RANGE_H
#define SLIB_RANGE_H
#include "sconfig.h"
namespace slib {
	/**
	* @class Range
	* \~english @brief Range class.
	* \~japanese @brief 範囲クラス
	*/
	template<typename T>
	class Range {
	public:
		/**
		 * \~english @brief Begin of rage
		 * \~japanese @brief 範囲開始値
		 */
		T begin;
		/**
		 * \~english @brief End of rage
		 * \~japanese @brief 範囲終了値
		 */
		T end;

	public:
		Range() : begin(T()), end(T()) {}
		Range(const T b, const T e) : begin(b), end(e) {}
		Range(const Range<T>& range) : begin(range.begin), end(range.end) {}
		~Range() {}
		Range& operator = (const Range<T>& range) { begin = range.begin; end = range.end; return *this; }
		Range<T>& operator|=(const Range<T>& range) { merge(range); return *this; }
		Range<T>& operator&=(const Range<T>& range) { mask(range); return *this; }
		Range<T>& operator^=(const Range<T>& range) { exclude(range); return *this; }
		Range<T> operator|(const Range<T>& rng) const { Range<T> range(*this); range.merge(rng); return range; }
		Range<T> operator&(const Range<T>& rng) const { Range<T> range(*this); range.mask(rng); return range; }
		Range<T> operator^(const Range<T>& rng) const { Range<T> range(*this); range.exclude(rng); return range; }
		T length(bool closed = false) const { return end - begin + (closed ? 1 : 0); }
		bool include(const T val) const { return (begin <= val && val <= end) ? true : false; }
		bool include(const Range<T>& range) const { return begin <= range.begin && range.end <= end; }
		bool overlap(const Range<T>& range) const { return begin <= range.end && range.begin <= end; }
		void shift(const T val) { begin += val; end += val; }
		void expand(const T val) { end += val; }
		void merge(const Range<T>& range) {
			begin = range.begin < begin ? range.begin : begin;
			end = end < range.end ? range.end : end;
		}
		void exclude(const Range<T>& range) {
			if (overlap(range)) *this = Range<T>((include(range.end) ? range.end + 1 : begin), (include(range.begin) ? range.begin - 1 : end));
		}
		void mask(const Range<T>& range) {
			if (include(range)) *this = range;
			else if (overlap(range))
				*this = Range<T>((begin < range.begin ? range.begin : begin), (range.end < end ? range.end : end));
			else *this = Range<T>();
		}
		bool operator < (const T val) const { return begin < val; }
		bool operator < (const Range& range) const {
			return begin != range.begin ? begin < range.begin : end < range.end;
		}
		bool operator == (const Range& range) const {
			return begin == range.begin && end == range.end;
		}
		bool operator != (const Range& range) const {
			return begin != range.begin || end != range.end;
		}
	};
	template<typename T>
	Range<T> shift(const Range<T>& rng, const T val) { Range<T> r(rng); r.shift(val); return r; }
	template<typename T>
	Range<T> expand(const Range<T>& rng, const T val) { Range<T> r(rng); r.expand(val); return r; }
	template<typename T>
	Range<T> merge(const Range<T>& rng1, const Range<T>& rng2) { Range<T> r(rng1); r.merge(rng2); return r; }
	template<typename T>
	Range<T> exclude(const Range<T>& rng1, const Range<T>& rng2) { Range<T> r(rng1); r.exclude(rng2); return r; }
	template<typename T>
	Range<T> mask(const Range<T>& rng1, const Range<T>& rng2) { Range<T> r(rng1); r.mask(rng2); return r; }

	/**
	* @cond
	*/
	template<typename T>
	extern bool operator<(const T n, const Range<T>& range) { return n < range.begin; }	
	/**
	* @endcond
	*/
}
/**
* @cond
*/
using srange = slib::Range<slib::sint>;
using srangel = slib::Range<slib::sinteger>;
using srangeb = slib::Range<slib::sbyte>;
using srangef = slib::Range<float>;
using sranged = slib::Range<double>;
using srangeu = slib::Range<size_t>;

template<typename T>
extern inline std::ostream& operator<<(std::ostream& os, const slib::Range<T>& range) { return os << "(" << range.begin << "," << range.end << ")"; }
/**
* @endcond
*/

#endif