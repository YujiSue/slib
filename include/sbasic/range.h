#ifndef SLIB_RANGE_H
#define SLIB_RANGE_H

#include "sconfig.h"

#define srange slib::Range<sint>
#define srangeb slib::Range<sbyte>
#define srangef slib::Range<float>
#define sranged slib::Range<double>
#define sranges slib::Range<size_t>

namespace slib {
	template<typename T>
	struct Range {
		T begin, end;

		Range();
		Range(const T& b, const T& e);
		Range(const Range& rng);
		~Range();
		Range& operator = (const Range& rng);
		Range operator + (const Range& rng) const;
		Range operator * (const Range& rng) const;
		T length(bool closed = false) const;
		bool include(const T& val) const;
		bool include(const Range& rng) const;
		bool overlap(const Range& rng) const;
		void shift(const T& s);
		void expand(const T& e);
		void merge(const Range& rng);
		void difference(const Range& rng);
		void conjunction(const Range& rng);
		Range shifted(const T& s) const;
		Range expanded(const T& e) const;
		Range merged(const Range& rng) const;
		Range differenced(const Range& rng) const;
		Range conjunctioned(const Range& rng) const;
		bool operator < (const T& pos) const;
		bool operator < (const Range& rng) const;
		bool operator == (const Range& rng) const;
		bool operator != (const Range& rng) const;
	};
	template<typename T>
	extern bool operator<(const T& n, const Range<T>& range) { return n < range.begin; }
	template<typename T>
	extern std::ostream& operator<<(std::ostream& os, const Range<T>& range) { return os << "(" << range.begin << ", " << range.end << ")"; }

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
	Range<T> Range<T>::operator+(const Range<T>& rng) const {
		Range rng_ = *this; rng_.merge(rng); return rng_;
	}
	template<typename T>
	Range<T> Range<T>::operator*(const Range<T>& rng) const {
		Range rng_ = *this; rng_.conjunction(rng); return rng_;
	}
	template<typename T>
	T Range<T>::length(bool closed) const { return end - begin + (closed ? 1 : 0); }
	template<typename T>
	bool Range<T>::include(const T& val) const { return (begin <= val && val <= end) ? true : false; }
	template<typename T>
	bool Range<T>::include(const Range<T>& rng) const { return begin <= rng.begin && rng.end <= end; }
	template<typename T>
	bool Range<T>::overlap(const Range<T>& rng) const { return begin <= rng.end && rng.begin <= end; }
	template<typename T>
	void Range<T>::shift(const T& s) { begin += s; end += s; }
	template<typename T>
	void Range<T>::expand(const T& e) { end += e; }
	template<typename T>
	void Range<T>::merge(const Range& rng) {
		if (rng.begin < begin) begin = rng.begin;
		if (end < rng.end) end = rng.end;
	}
	template<typename T>
	void Range<T>::difference(const Range& rng) {
		if (!overlap(rng) || include(rng)) return;
		if (include(rng.begin)) end = rng.begin - 1;
		else if (include(rng.end)) begin = rng.end + 1;
	}
	template<typename T>
	void Range<T>::conjunction(const Range<T>& rng) {
		if (include(rng)) *this = rng;
		else if (overlap(rng)) {
			if (begin < rng.begin) begin = rng.begin;
			if (rng.end < end) end = rng.end;
		}
		else *this = Range();
	}
	template<typename T>
	Range<T> Range<T>::shifted(const T& s) const { return Range(begin + s, end + s); }
	template<typename T>
	Range<T> Range<T>::expanded(const T& e) const { return Range(begin, end + e); }
	template<typename T>
	Range<T> Range<T>::merged(const Range<T>& rng) const {
		Range rng_ = *this;
		if (rng.begin < begin) rng_.begin = rng.begin;
		if (end < rng.end) rng_.end = rng.end;
		return rng_;
	}
	template<typename T>
	Range<T> Range<T>::differenced(const Range& rng) const {
		Range<T> r(*this);
		r.difference(rng);
		return r;
	}
	template<typename T>
	Range<T> Range<T>::conjunctioned(const Range<T>& rng) const {
		Range rng_ = *this;
		if (include(rng)) rng_ = rng;
		else if (overlap(rng)) {
			if (begin < rng.begin) rng_.begin = rng.begin;
			if (rng.end < end) rng_.end = rng.end;
		}
		return rng_;
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