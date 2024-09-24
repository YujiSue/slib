#ifndef SLIB_REGION_H
#define SLIB_REGION_H
#include "sbasic/range.h"
#include "sbasic/array.h"
namespace slib {
	/**
	* @class Region
	*/
	template<typename T>
	class Region : public Array<Range<T>> {
	public:
		Region() : Array<Range<T>>() {}
		Region(const size_t s) : Array<Range<T>>(s) {}
		Region(std::initializer_list<Range<T>> li) : Array<Range<T>>(li) {}
		Region(Region<T>&& reg) noexcept : Array<Range<T>>(std::forward<Array<Range<T>>&&>(*this)) {}
		Region(const Region<T>& reg) : Array<Range<T>>(reg) {}
		~Region() {}
		Region<T>& operator=(Region<T>&& reg) noexcept {
			Array<Range<T>>::swap(reg);
			return *this;
		}
		Region<T>& operator=(const Region<T>& reg) {
			Array<Range<T>>::resize(reg.size());
			sfor2(*this, reg) $_1 = $_2;
			return *this;
		}
		T length(bool closed = false) const {
			T len = 0;
			sfor(*this) len += $_.length(closed);
			return len;
		}
		Range<T> range() const {
			if (!Array<Range<T>>::empty()) return Range<T>(Array<Range<T>>::at(0).begin, Array<Range<T>>::at(-1).end);
			return Range<T>();
		}
		bool include(const T& val) const {
			sfor(*this) {
				if ($_.include(val)) return true;
			}
			return false;
		}
		bool include(const Range<T>& rng) const {
			sfor(*this) {
				if ($_.include(rng)) return true;
			}
			return false;
		}
		bool overlap(const Range<T>& rng) const {
			sfor(*this) {
				if ($_.overlap(rng)) return true;
			}
			return false;
		}
		bool overlap(const Region& reg) const {
			sfor(reg) {
				if (this->overlap($_)) return true;
			}
			return false;
		}
		Region<T> subregion(const Range<T>& rng) const {
			Region<T> reg;
			if (range().overlap(rng)) {
				sfor(*this) {
					if (rng.end < $_.begin) break;
					else if (rng.include($_)) reg.add($_);
					else if (rng.overlap($_))
						reg.add(Range<T>((rng.begin < $_.begin ? $_.begin : rng.begin), ($_.end < rng.end ? $_.end : rng.end)));
				}
			}
			return reg;
		}
		void shift(const T len) {
			if (!Array<Range<T>>::empty()) { sfor(*this) $_.shift(len); }
		}
		void expand(const T& off, const T& len) {
			if (!Array<Range<T>>::empty()) {
				sfor(*this) {
					if (off <= $_.begin) $_.shift(len);
					else if (off <= $_.end) $_.expand(len);
				}
			}
		}
		void merge(const Range<T>& rng) {
			Array<Range<T>>::add(rng);
			Array<Range<T>>::sort();
			auto it = Array<Range<T>>::begin() + 1, end = Array<Range<T>>::end();
			while (it < end) {
				if ($_.overlap($_PREV)) {
					$_PREV.merge($_);
					Array<Range<T>>::removeAt($INDEX(*this));
					end = Array<Range<T>>::end();
				}
				else $NEXT;
			}
		}
		void merge(const Region<T>& reg) {
			Array<Range<T>>::append(reg);
			Array<Range<T>>::sort();
			auto it = Array<Range<T>>::begin() + 1, end = Array<Range<T>>::end();
			while (it < end) {
				if ($_.overlap($_PREV)) {
					$_PREV.merge($_); 
					Array<Range<T>>::removeAt($INDEX(*this)); 
					end = Array<Range<T>>::end();
				}
				else $NEXT;
			}
		}
		void mask(const Range<T>& rng) {
			auto reg = subregion(rng);
			Array<Range<T>>::swap(reg);
		}
		void exclude(const Range<T>& rng) {
			auto it = Array<Range<T>>::begin();
			ArrayIterator<Range<T>> beg, end;
			while ($ < Array<Range<T>>::end()) {
				if ($_.begin < rng.begin) {
					if (rng.end <= $_.end) $_.end = rng.begin - 1;
					else {
						if (beg.ptr()) end = $;
						else { beg = $; end = $ + 1; }
					}
				}
				else {
					if (rng.end < $_.end) $_.begin = rng.end + 1;
					else {
						if (beg.ptr()) end = $ + 1;
						else { beg = $; end = $ + 1; }
					}
				}
				$NEXT;
			}
			if (beg.ptr()) Array<Range<T>>::remove(INDEX_(beg, *this), (end - beg));
		}
		ArrayIterator<Range<T>> seek(const T& val) {
			sfor(*this) {
				if ($_.include(val)) return $;
			}
			return Array<Range<T>>::end();
		}
		Range<ArrayIterator<Range<T>>> seek(const Range<T>& rng) {
			auto it = Array<Range<T>>::begin();
			ArrayIterator<Range<T>> beg, end;
			while ($ < Array<Range<T>>::end()) {
				if ($_.overlap(rng)) {
					if (beg.ptr()) end = $ + 1;
					else {
						beg = $; end = $ + 1;
					}
				}
				$NEXT;
			}
			return Range<ArrayIterator<Range<T>>>(beg, end);
		}

		bool operator < (const Region<T>& reg) const { return range() < reg.range(); }
		bool operator == (const Region<T>& reg) const {
			if (Array<Range<T>>::size() != reg.size()) return false;
			sfor2(*this, reg) {
				if ($_1 != $_2) return false;
			}
			return true;
		}
		bool operator != (const Region<T>& reg) const { return !(*this == reg); }
	};
	template<typename T>
	extern inline String toString(const Region<T>& region, const char* format = nullptr) {
		String str;
		sfor(region) {
			str << "(" << $_.begin << "," << $_.end << "),";
		}
		if (region.size()) str.resize(str.size() - 1);
		return str;
	}

	template<typename T>
	Region<T> shift(const Region<T>& region, const T len) {
		Region<T> r(region); r.shift(len); return r;
	}
	template<typename T>
	Region<T> expand(const Region<T>& region, const T& off, const T& len) {
		Region<T> r(region); r.expand(off, len); return r;
	}
	template<typename T>
	Region<T> merge(const Region<T>& region, const Range<T>& rng) {
		Region<T> r(region); r.merge(rng); return r;
	}
	template<typename T>
	Region<T> merge(const Region<T>& reg1, const Region<T>& reg2) {
		Region<T> r(reg1); r.merge(reg2); return r;
	}
	template<typename T>
	Region<T> mask(const Region<T>& region, const Range<T>& rng) {
		Region<T> r(region); r.mask(rng); return r;
	}
	template<typename T>
	Region<T> exclude(const Region<T>& region, const Range<T>& rng) {
		Region<T> r(region); r.exclude(rng); return r;
	}
}
using sregion = slib::Region<slib::sint>;
using sregionf = slib::Region<float>;
using sregiond = slib::Region<double>;
template <typename T>
extern inline std::ostream& operator<<(std::ostream& os, const slib::Region<T>& reg) {
	if (reg.empty()) return os;
	else {
		os << "(" << reg[0].begin << ".." << reg[0].end << ")";
		auto it = reg.begin() + 1, end = reg.end();
		while (it < end) {
			os << ",(" << $_.begin << ".." << $_.end << ")"; $NEXT;
		}
		return os;
	}
}

template <typename T>
extern slib::Region<T> operator^(const slib::Range<T>& rng, const slib::Region<T>& region) {
	slib::Region<T> comp;
	if (region.empty() || !rng.overlap(region.range())) comp.add(rng);
	else {
		if (rng.begin < region[0].begin) comp.add(slib::Range<T>(rng.begin, region[0].begin - 1));
		auto it = region.begin(), end = region.end() - 1;
		if (1 < region.size()) {
			while (it < end) {
				if ($_.end + 1 <= $_NEXT.begin - 1) comp.add(slib::Range<T>($_.end + 1, $_NEXT.begin - 1));
				$NEXT;
			}
		}
		if (end->end < rng.end) comp.add(slib::Range<T>(end->end + 1, rng.end));
	}
	return comp;
}

#endif
