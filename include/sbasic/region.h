#ifndef SLIB_REGION_H
#define SLIB_REGION_H

#include "sbasic/range.h"
#include "sbasic/array.h"

#define sregion slib::Region<sint>
#define sregionf slib::Region<float>
#define sregiond slib::Region<double>

namespace slib {
    template<typename T>
    class Region : public Array<Range<T>, RMemory<Range<T>>> {
		typedef Array<Range<T>, RMemory<Range<T>>> array;
    public:
        Region();
        Region(const Range<T> &rng);
		Region(std::initializer_list<Range<T>> li);
        Region(const Region &reg);
        ~Region();
        
        Region &operator=(const Range<T> &rng);
        Region &operator=(const Region &reg);
        T length(bool closed = false) const;
        Range<T> range() const;
        bool include(const T &val) const;
        bool include(const Range<T> &rng) const;
        bool overlap(const Range<T> &rng) const;
        bool overlap(const Region &reg) const;
        Region subregion(const Range<T> &rng) const;
        void shift(const T &len);
        void expand(const T &off, const T &len);
        void merge(const Range<T> &rng);
        void merge(const Region &reg);
        void conjunction(const Range<T> &rng);
        void exclude(const Range<T> &rng);
        srange find(const T &val) const;
        srange find(const Range<T> &rng) const;
        bool operator < (const Region &reg) const;
        bool operator == (const Region &reg) const;
        bool operator != (const Region<T> &reg) const;
    };
    template <typename T>
    extern Region<T> complement(const Range<T> &rng, const Region<T> &region);
    
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
    Region<T>::Region(const Region &reg) : array(reg) {}
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
		sforeach(*this) { 
			if(reg.overlap(E_)) return true; 
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
    void Region<T>::shift(const T &len) {
        if (array::empty()) return;
        sforeach(*this) E_.shift(len);
    }
    template <typename T>
    void Region<T>::expand(const T &off, const T &len) {
        if (array::empty()) return;
        sforeach(*this) {
            if (off <= E_.begin) E_.shift(len);
            else if (off <= E_.end) E_.expand(len);
        }
    }
    template <typename T>
    void Region<T>::merge(const Range<T> &rng) {
		array::add(rng);
		array::sort();
		auto it = array::begin(), end = array::end();
		while(it < end - 1) {
			auto it_ = it + 1;
            while (it_ < end && E_.overlap(E__)) { E_.merge(E__); ++it_; }
			if (it + 1 < it_) it = array::remove(it + 1, it_);
			else NEXT_;
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
    void Region<T>::conjunction(const Range<T> &rng) { *this = subregion(rng); }
    template <typename T>
    void Region<T>::exclude(const Range<T> &rng) {
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
    }
    template <typename T>
    srange Region<T>::find(const T &val) const {
        if (array::empty()) return srange(-1, -1);
        if (val < array::first().begin) return srange(-1, 0);
        else if (array::last().end < val)
            return srange(array::size(), array::size());
        sforeach(*this) {
            if (E_.include(val)) return srange(INDEX_(*this), INDEX_(*this));
			if (array::begin() < it && E_PREV.end < val && val < E_.begin) return srange(INDEX_(*this) - 1, INDEX_(*this));
        }
        return srange(-1, -1);
    }
    template <typename T>
    srange Region<T>::find(const Range<T> &rng) const {
        srange r1 = find(rng.begin), r2 = find(rng.end);
        return srange(r1.begin, r2.end);
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
    template <typename T>
    Region<T> complement(const Range<T> &rng, const Region<T> &region) {
        Region<T> comp;
        if (rng.begin < region.first().begin)
            comp.add(Range<T>(rng.begin, region.first().begin-1));
		sforin(it, region.begin(), region.end() - 1) {
			if (E_.end + 1 <= E_NEXT.begin - 1) 
				comp.add(Range<T>(E_.end + 1, E_NEXT.begin - 1));
		}
		if (region.last().end < rng.end) comp.add(Range<T>(region.last().end + 1, rng.end));
        return comp;
    }
}

#endif
