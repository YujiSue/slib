#ifndef SLIB_RANGE_H
#define SLIB_RANGE_H

#include "sconfig.h"

namespace slib {
	template<typename T>
	struct Range {
		T begin, end;

		Range();
		Range(const T& b, const T& e);
		Range(const Range& rng);
		~Range();
		Range& operator = (const Range& rng);
		Range& operator >>= (const T& t);
		Range& operator <<= (const T& t);
		Range& operator += (const T& t);
		Range& operator -= (const T& t);
		Range& operator |= (const Range& rng);
		Range& operator &= (const Range& rng);
		Range& operator ^= (const Range& rng);
		Range operator >> (const T& t) const;
		Range operator << (const T& t) const;
		Range operator + (const T& t) const;
		Range operator - (const T& t) const;
		Range operator | (const Range& rng) const;
		Range operator & (const Range& rng) const;
		Range operator ^ (const Range& rng) const;
		T length(bool closed = false) const;
		bool include(const T& val) const;
		bool include(const Range& rng) const;
		bool overlap(const Range& rng) const;
		void shift(const T& s);
		void expand(const T& e);
		void merge(const Range& rng);
		void exclude(const Range& rng);
		void mask(const Range& rng);

		bool operator < (const T& pos) const;
		bool operator < (const Range& rng) const;
		bool operator == (const Range& rng) const;
		bool operator != (const Range& rng) const;
	};
	template<typename T>
	extern bool operator<(const T& n, const Range<T>& range) { return n < range.begin; }
	template<typename T>
	extern std::ostream& operator<<(std::ostream& os, const Range<T>& range) { return os << "(" << range.begin << ".." << range.end << ")"; }

	using srange = Range<sint>;
	using srangeb = Range<sbyte>;
	using srangef = Range<float>;
	using sranged = Range<double>;
	using sranges = Range<size_t>;

	/*============================================================*/

	template<typename T>
	Range<T>::Range() : begin(T()), end(T()) {}
	template<typename T>
	Range<T>::Range(const T& b, const T& e) : begin(b), end(e) {}
	template<typename T>
	Range<T>::Range(const Range& rng) : begin(rng.begin), end(rng.end) {}
	template<typename T>
	Range<T>::~Range() {}
	template<typename T>
	Range<T>& Range<T>::operator = (const Range<T>& rng) { begin = rng.begin; end = rng.end; return *this; }
	template<typename T>
	Range<T>& Range<T>::operator >>=  (const T& t) { begin += t; end += t; return *this; }
	template<typename T>
	Range<T>& Range<T>::operator <<=  (const T& t) { begin -= t; end -= t; return *this; }
	template<typename T>
	Range<T>& Range<T>::operator += (const T& t) { end += t;  return *this; }
	template<typename T>
	Range<T>& Range<T>::operator -= (const T& t) { end -= t;  return *this; }
	template<typename T>
	Range<T>& Range<T>::operator |= (const Range<T>& rng) { 
		if (rng.begin < begin) begin = rng.begin;
		if (end < rng.end) end = rng.end; 
		return *this; 
	}
	template<typename T>
	Range<T>& Range<T>::operator &= (const Range<T>& rng) { 
		if (include(rng)) *this = rng;
		else if (overlap(rng)) {
			if (begin < rng.begin) begin = rng.begin;
			if (rng.end < end) end = rng.end;
		}
		else *this = Range<T>();
		return *this;
	}
	template<typename T>
	Range<T>& Range<T>::operator ^= (const Range<T>& rng) { 
		if (!overlap(rng)) return *this;
		if (include(rng.begin)) end = rng.begin - 1;
		else if (include(rng.end)) begin = rng.end + 1;
		return *this;
	}
	template<typename T>
	Range<T> Range<T>::operator >> (const T& t) const { return Range<T>(begin + t, end + t); }
	template<typename T>
	Range<T> Range<T>::operator << (const T& t) const { return Range<T>(begin - t, end - t); }
	template<typename T>
	Range<T> Range<T>::operator + (const T& t) const { return Range<T>(begin, end + t); }
	template<typename T>
	Range<T> Range<T>::operator - (const T& t) const { return Range<T>(begin, end - t); }
	template<typename T>
	Range<T> Range<T>::operator | (const Range& rng) const { return this->merge(rng); }
	template<typename T>
	Range<T> Range<T>::operator & (const Range& rng) const { return this->mask(rng); }
	template<typename T>
	Range<T> Range<T>::operator ^ (const Range& rng) const { return this->exclude(rng); }
	template<typename T>
	T Range<T>::length(bool closed) const { return end - begin + (closed ? 1 : 0); }
	template<typename T>
	bool Range<T>::include(const T& val) const { return (begin <= val && val <= end) ? true : false; }
	template<typename T>
	bool Range<T>::include(const Range<T>& rng) const { return begin <= rng.begin && rng.end <= end; }
	template<typename T>
	bool Range<T>::overlap(const Range<T>& rng) const { return begin <= rng.end && rng.begin <= end; }
	template<typename T>
	void Range<T>::shift(const T& t) { begin += t; end += t; }
	template<typename T>
	void Range<T>::expand(const T& t) { end += t; }
	template<typename T>
	void Range<T>::merge(const Range& rng) {
		*this = Range<T>((rng.begin < begin ? rng.begin : begin), (end < rng.end ? rng.end : end));
	}
	template<typename T>
	void Range<T>::exclude(const Range& rng) {
		if (overlap(rng)) *this = Range<T>((include(rng.end) ? rng.end + 1 : begin), (include(rng.begin) ? rng.begin - 1 : end));
	}
	template<typename T>
	void Range<T>::mask(const Range<T>& rng) {
		if (include(rng)) *this = rng;
		else if (overlap(rng))
			*this = Range<T>((begin < rng.begin ? begin : rng.begin), (rng.end < end ? rng.end : end));
		else *this = Range<T>();
	}
	template<typename T>
	bool Range<T>::operator < (const T& pos) const { return end < pos; }
	template<typename T>
	bool Range<T>::operator < (const Range<T>& rng) const {
		return begin != rng.begin ? begin < rng.begin : end < rng.end;
	}
	template<typename T>
	bool Range<T>::operator == (const Range<T>& rng) const {
		return begin == rng.begin && end == rng.end;
	}
	template<typename T>
	bool Range<T>::operator != (const Range<T>& rng) const {
		return begin != rng.begin || end != rng.end;
	}
}
#endif