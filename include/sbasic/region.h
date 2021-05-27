#ifndef SLIB_REGION_H
#define SLIB_REGION_H

#include "sbasic/range.h"
#include "sbasic/array.h"

namespace slib {
    template<typename T>
    class Region : public Array<Range<T>, RMemory<Range<T>>> {
		typedef Array<Range<T>, RMemory<Range<T>>> array;
    public:
        Region();
        Region(const Range<T> &rng);
		Region(std::initializer_list<Range<T>> li);
		Region(Region&& reg);
        Region(const Region &reg);
        ~Region();
        Region &operator=(const Range<T> &rng);
        Region &operator=(const Region &reg);
		Region& operator >>= (const T& t);
		Region& operator <<= (const T& t);
		Region& operator |= (const Range<T>& rng);
		Region& operator &= (const Range<T>& rng);
		Region& operator ^= (const Range<T>& rng);
		Region operator >> (const T& t) const;
		Region operator << (const T& t) const;
		Region operator | (const Range<T>& rng) const;
		Region operator & (const Range<T>& rng) const;
		Region operator ^ (const Range<T>& rng) const;
        T length(bool closed = false) const;
        Range<T> range() const;
        bool include(const T &val) const;
        bool include(const Range<T> &rng) const;
        bool overlap(const Range<T> &rng) const;
        bool overlap(const Region &reg) const;
        Region subregion(const Range<T> &rng) const;
		void shift(const T& len);
		void expand(const T& off, const T& len);
		void merge(const Range<T>& rng);
		void merge(const Region& reg);
		void mask(const Range<T>& rng);
		void exclude(const Range<T>& rng);
        sarr_iter<Range<T>> seek(const T &val);
        Range<sarr_iter<Range<T>>> seek(const Range<T> &rng);
        bool operator < (const Region &reg) const;
        bool operator == (const Region &reg) const;
        bool operator != (const Region<T> &reg) const;
    };
    template <typename T>
	extern Region<T> operator^(const Range<T>& rng, const Region<T>& region);
	template<typename T>
	extern String toString(const Region<T>& reg) {
		String str;
		if (!reg.empty()) sforeach(reg) str << "(" << E_.begin << ".." << E_.end << "),";
		if (!str.empty()) str.resize(str.length() - 1);
		return str;
	}
	template<typename T>
	extern inline std::ostream& operator<<(std::ostream& os, const Region<T>& reg) { return os << toString(reg); }
	
	using sregion = Region<sint>;
	using sregionf = Region<float>;
	using sregiond = Region<double>;
	
	/*============================================================*/
    
    template <typename T>
    Region<T>::Region() : array() {}
    template <typename T>
    Region<T>::Region(const Range<T> &rng) : Region() { array::add(rng); }
	template <typename T>
	Region<T>::Region(std::initializer_list<Range<T>> li) : array(li.size()) {
		sforeach2(*this, li) E1_ = E2_;
	}
	template <typename T>
	Region<T>::Region(Region<T>&& reg) : array(std::forward<array&&>(*this)) {}
    template <typename T>
	Region<T>::Region(const Region<T>& reg) : array(reg) {}
    template <typename T>
    Region<T>::~Region() {}
    template <typename T>
    Region<T> &Region<T>::operator=(const Range<T> &rng) {
		array::clear();
		array::add(rng);
		return *this;
    }
    template <typename T>
    Region<T> &Region<T>::operator=(const Region<T> &reg) {
		array::resize(reg.size());
		sforeach2(*this, reg) E1_ = E2_;
        return *this;
    }
	template <typename T>
	Region<T>& Region<T>::operator >>= (const T& t) { return this->shift(t); }
	template <typename T>
	Region<T>& Region<T>::operator <<= (const T& t) { return this->shift(-t); }
	template <typename T>
	Region<T>& Region<T>::operator |= (const Range<T>& rng) { return this->merge(rng); }
	template <typename T>
	Region<T>& Region<T>::operator &= (const Range<T>& rng) { return this->mask(rng); }
	template <typename T>
	Region<T>& Region<T>::operator ^= (const Range<T>& rng) { return this->exclude(rng); }
	template <typename T>
	Region<T> Region<T>::operator >> (const T& t) const { return Region<T>(*this).shift(t); }
	template <typename T>
	Region<T> Region<T>::operator << (const T& t) const { return Region<T>(*this).shift(-t); }
	template <typename T>
	Region<T> Region<T>::operator | (const Range<T>& rng) const { return Region<T>(*this).merge(rng); }
	template <typename T>
	Region<T> Region<T>::operator & (const Range<T>& rng) const { return Region<T>(*this).mask(rng); }
	template <typename T>
	Region<T> Region<T>::operator ^ (const Range<T>& rng) const { return Region<T>(*this).exclude(rng); }
    template <typename T>
    T Region<T>::length(bool closed) const { 
		T len = initVal<T>(); 
		sforeach(*this) len += E_.length(closed); 
		return len; 
	}
    template <typename T>
    Range<T> Region<T>::range() const {
        if (!array::empty()) return Range<T>(array::first().begin, array::last().end);
        return Range<T>();
    }
    template <typename T>
    bool Region<T>::include(const T &val) const {
		sforeach(*this) {
			if(E_.include(val)) return true;
		}
		return false; }
    template <typename T>
    bool Region<T>::include(const Range<T> &rng) const {
		sforeach(*this) {
			if(E_.include(rng)) return true; 
		}
		return false;
	}
    template <typename T>
    bool Region<T>::overlap(const Range<T> &rng) const { 
		sforeach(*this) { 
			if(E_.overlap(rng)) return true; 
		} 
		return false; 
	}
    template <typename T>
    bool Region<T>::overlap(const Region &reg) const { 
		sforeach(reg) {
			if(this->overlap(E_)) return true; 
		} 
		return false; 
	}
    template <typename T>
    Region<T> Region<T>::subregion(const Range<T> &rng) const {
        Region reg;
        if (range().overlap(rng)) {
            sforeach(*this) {
                if (rng.end < E_.begin) break;
                else if (rng.include(E_)) reg.add(E_);
                else if (rng.overlap(E_))
                    reg.add(Range<T>((rng.begin<E_.begin?E_.begin:rng.begin), (E_.end<rng.end?E_.end:rng.end)));
            }
        }
        return reg;
    }
    template <typename T>
	void Region<T>::shift(const T& len) {
		if (!array::empty()) {
			sforeach(*this) E_.shift(len);
		}
    }
    template <typename T>
	void Region<T>::expand(const T &off, const T &len) {
		if (!array::empty()) {
			sforeach(*this) {
				if (off <= E_.begin) E_.shift(len);
				else if (off <= E_.end) E_.expand(len);
			}
		}
    }
    template <typename T>
	void Region<T>::merge(const Range<T> &rng) {
		if (array::empty() || array::last().end < rng.begin) array::add(rng);
		else if (rng.end < array::first().begin) array::insert(0, rng);
		else {
			sforeach(*this) {
				if (E_.include(rng)) break;
				else if (E_.overlap(rng)) {
					if (E_.include(rng.end)) E_.begin = rng.begin;
					else {
						auto it_ = it + 1;
						if (it_ == array::end() || rng.end < it_->begin) E_.end = rng.end;
						else {
							while (it_ < array::end() && it_->begin <= rng.end) {
								if (it_->end < rng.end) E_.end = rng.end;
								else if (it_->include(rng.end)) E_.end = it_->end;
								++it_;
							}
							array::remove(it_ - array::begin(), it_ - it);
						}
					}
					break;
				}
				else if (rng.end < E_.begin) { array::insert(INDEX_(*this), rng); break; }
			}
		}
    }
    template <typename T>
	void Region<T>::merge(const Region<T> &reg) {
		array::append(reg);
		array::sort();
		auto it = array::begin(), end = array::end();
		while (it < end - 1) {
			auto it_ = it + 1;
			while (it_ < end && E_.overlap(E__)) { E_.merge(E__); ++it_; }
			if (it + 1 < it_) it = array::remove(it + 1, it_);
			else NEXT_;
		}
    }
    template <typename T>
	void Region<T>::mask(const Range<T> &rng) {
		auto reg = subregion(rng);
		array::swap(reg);
	}
    template <typename T>
	void Region<T>::exclude(const Range<T> &rng) {
		auto rng_ = range();
		if (rng.include(rng_)) array::clear();
		else {
			auto init = array::begin(), end = array::end(), it = init;
			if (rng_.include(rng.begin)) {
				while(it < array::end()) {
					if (E_.include(rng.begin)) {
						E_.end = rng.begin - 1;
						init = E_.end < E_.begin?it : it + 1;
						break;
					}
					else if (rng.begin < E_.begin) {
						init = it;
						break;
					}
					NEXT_;
				}
			}
			if (rng_.include(rng.end)) {
				while (it < array::end()) {
					if (E_.include(rng.end)) {
						E_.begin = rng.end + 1;
						end = E_.end < E_.begin ? it + 1 : it;
						break;
					}
					else if (rng.end < E_.begin) {
						end = it;
						break;
					}
					NEXT_;
				}
			}
			if (init < end) array::remove(init, end);
		}
/*
        if (range().overlap(rng)) {
            sforeach(*this) {
				if (rng.include(E_)) it = array::remove(it, it + 1);
				if (E_.include(rng)) {
					if (E_.begin == rng.begin) E_.begin = rng.end + 1;
					else if (E_.end == rng.end) E_.end = rng.begin - 1;
					else {
						Range<T> tmp(E_.begin, rng.begin - 1);
						E_.begin = rng.end + 1;
						it = array::insert(it, tmp);
					}
				}
				else if (E_.overlap(rng)) {
					if (E_.begin < rng.begin) E_.end = rng.begin - 1;
					else E_.begin = rng.end + 1;
				}
				else if (rng.end < E_.begin) break;
            }
        }
		*/
    }
    template <typename T>
    sarr_iter<Range<T>> Region<T>::seek(const T &val) {
		sforeach(*this) {
			if (E_.include(val)) return it;
			else if (val < E_.begin) break;
		}
		return array::end();
		/*
		sarr_iter<Range<T>> it = array::begin();
        if (array::empty()) return srange(-1, -1);
        if (val < array::first().begin) return srange(-1, 0);
        else if (array::last().end < val)
            return srange((sint)array::size(), (sint)array::size());
        sforeach(*this) {
            if (E_.include(val)) return srange(INDEX_(*this), INDEX_(*this));
			if (array::begin() < it && E_PREV.end < val && val < E_.begin) return srange(INDEX_(*this) - 1, INDEX_(*this));
        }
        return srange(-1, -1);
		*/
    }
    template <typename T>
    Range<sarr_iter<Range<T>>> Region<T>::seek(const Range<T> &rng) {
		Range<sarr_iter<Range<T>>> rng_(array::end(), array::end());
		if (range().overlap(rng)) {
			sforeach(*this) {
				if (E_.include(rng)) {
					rng_.begin = it; rng_.end = it; break;
				}
				else if (E_.include(rng.begin)) rng_.begin = it;
				else if (E_.include(rng.end)) rng_.end = it;
				else if (E_.end < rng.begin) rng_.begin = it + 1;
				else if (E_.end < rng.end) rng_.end = it;
				else if (rng.end < E_.begin) break;
			}
		}
		return rng_;
		/*
        srange r1 = find(rng.begin), r2 = find(rng.end);
        return srange(r1.begin, r2.end);
		*/
    }
    template <typename T>
    bool Region<T>::operator < (const Region &reg) const { return range() < reg.range(); }
    template <typename T>
    bool Region<T>::operator == (const Region &reg) const {
        if (array::size() != reg.size()) return false;
        sforeach2(*this, reg) { 
			if(E1_ != E2_) return false; 
		}
        return true;
    }
    template <typename T>
    bool Region<T>::operator != (const Region<T> &reg) const { return !(*this==reg); }
}
template <typename T>
slib::Region<T> slib::operator^(const slib::Range<T>& rng, const slib::Region<T>& region) {
//Region<T> complement(const Range<T> &rng, const Region<T> &region) {
	slib::Region<T> comp;
	if (region.empty() || !rng.overlap(region.range())) comp.add(rng);
	else {
		if (rng.begin < region.first().begin) comp.add(Range<T>(rng.begin, region.first().begin - 1));
		auto it = region.begin(), end = region.end() - 1;
		if (1 < region.size()) {
			while (it < end) {
				if (E_.end + 1 <= E_NEXT.begin - 1) comp.add(Range<T>(E_.end + 1, E_NEXT.begin - 1));
				NEXT_;
			}
		}
		if (end->end < rng.end) comp.add(Range<T>(end->end + 1, rng.end));
	}
	return comp;
}
#endif
