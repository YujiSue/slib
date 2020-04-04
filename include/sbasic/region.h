#ifndef SLIB_REGION_H
#define SLIB_REGION_H

#include "sbasic/range.h"
#include "sbasic/array.h"

namespace slib {
    #define sregion Region<sint>
    #define sregionf Region<float>
    #define sregiond Region<double>
    
    template<typename T>
    class Region : public Array<Range<T>, RMemory<Range<T>>> {
    public:
        Region();
        Region(const Range<T> &rng);
        Region(const Region &reg);
        ~Region();
        
        Region &operator=(const Range<T> &rng);
        Region &operator=(const Region &reg);
        T length() const;
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
    Region<T>::Region() : Array<Range<T>, RMemory<Range<T>>>() {}
    template <typename T>
    Region<T>::Region(const Range<T> &rng) : Region() { Array<Range<T>, RMemory<Range<T>>>::add(rng); }
    template <typename T>
    Region<T>::Region(const Region &reg) : Array<Range<T>, RMemory<Range<T>>>(reg) {}
    template <typename T>
    Region<T>::~Region() {}
    
    template <typename T>
    Region<T> &Region<T>::operator=(const Range<T> &rng) {
        Array<Range<T>, RMemory<Range<T>>>::clear();
        Array<Range<T>, RMemory<Range<T>>>::add(rng); return *this;
    }
    template <typename T>
    Region<T> &Region<T>::operator=(const Region<T> &reg) {
        Array<Range<T>, RMemory<Range<T>>>::resize(reg.size());
        auto it = Array<Range<T>, RMemory<Range<T>>>::_begin;
        auto it_ = reg._begin;
        while (it < Array<Range<T>, RMemory<Range<T>>>::_end) { E_ = *it_; NEXT_; ++it_; }
        return *this;
    }
    template <typename T>
    T Region<T>::length() const { T len = initVal<T>(); sforeach(*this) { len += E_.length(); } return len; }
    template <typename T>
    Range<T> Region<T>::range() const {
        if (!Array<Range<T>, RMemory<Range<T>>>::empty())
            return Range<T>(Array<Range<T>, RMemory<Range<T>>>::first().begin,
                             Array<Range<T>, RMemory<Range<T>>>::last().end);
        return Range<T>();
    }
    template <typename T>
    bool Region<T>::include(const T &val) const { sforeach(*this) { if(E_.include(val)) return true; } return false; }
    template <typename T>
    bool Region<T>::include(const Range<T> &rng) const { sforeach(*this) { if(E_.include(rng)) return true; } return false; }
    template <typename T>
    bool Region<T>::overlap(const Range<T> &rng) const { sforeach(*this) { if(E_.overlap(rng)) return true; } return false; }
    template <typename T>
    bool Region<T>::overlap(const Region &reg) const { sforeach(*this) { if(reg.overlap(E_)) return true; } return false; }
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
        if (Array<Range<T>, RMemory<Range<T>>>::empty()) return;
        sforeach(*this) E_.shift(len);
    }
    template <typename T>
    void Region<T>::expand(const T &off, const T &len) {
        if (Array<Range<T>, RMemory<Range<T>>>::empty()) return;
        sforeach(*this) {
            if (off <= E_.begin) E_.shift(len);
            else if (off <= E_.end) E_.expand(len);
        }
    }
    template <typename T>
    void Region<T>::merge(const Range<T> &rng) {
        Array<Range<T>, RMemory<Range<T>>>::add(rng);
        std::sort(Array<Range<T>, RMemory<Range<T>>>::begin(),
                  Array<Range<T>, RMemory<Range<T>>>::end(),
                  [](const Range<T> &r1, const Range<T> &r2) { return r1 < r2; });
        sforeach(*this) {
            auto it_ = it+1;
            while (it_ < Array<Range<T>, RMemory<Range<T>>>::end() && E_.overlap(*it_)) { E_.merge(*it_); ++it_; }
            if (it+1 < it_) Array<Range<T>, RMemory<Range<T>>>::remove(it+1, it_);
        }
    }
    template <typename T>
    void Region<T>::merge(const Region<T> &reg) {
        Array<Range<T>, RMemory<Range<T>>>::append(reg);
        std::sort(Array<Range<T>, RMemory<Range<T>>>::begin(),
                  Array<Range<T>, RMemory<Range<T>>>::end(),
                  [](const Range<T> &r1, const Range<T> &r2) { return r1 < r2; });
        sforeach(*this) {
            auto it_ = it+1;
            while (it_ < Array<Range<T>, RMemory<Range<T>>>::end() && E_.overlap(*it_)) { E_.merge(*it_); ++it_; }
            if (it+1 < it_) {
                auto off = it-Array<Range<T>, RMemory<Range<T>>>::begin();
                Array<Range<T>, RMemory<Range<T>>>::remove(off, 1);
                it = Array<Range<T>, RMemory<Range<T>>>::begin()+off;
            }
        }
    }
    template <typename T>
    void Region<T>::conjunction(const Range<T> &rng) { *this = subregion(rng); }
    template <typename T>
    void Region<T>::exclude(const Range<T> &rng) {
        if (range().overlap(rng)) {
            sforeach(*this) {
                if (E_.include(rng)) {
                    if (E_.begin == rng.begin) E_.begin = rng.end+1;
                    else if (E_.end == rng.end) E_.end = rng.begin-1;
                    else {
                        auto tmp = E_.end;
                        E_.end = rng.begin-1;
                        Array<Range<T>, RMemory<Range<T>>>::insert(it-Array<Range<T>, RMemory<Range<T>>>::begin()+1,
                                                                     Range<T>(rng.end+1, tmp));
                    }
                }
                else if (rng.include(E_)) {
                    auto it_ = it+1;
                    while(it_ < Array<Range<T>, RMemory<Range<T>>>::end() && rng.include(*it_)) ++it_;
                    auto off = it-Array<Range<T>, RMemory<Range<T>>>::begin();
                    Array<Range<T>, RMemory<Range<T>>>::remove(off, 1);
                    it = Array<Range<T>, RMemory<Range<T>>>::begin()+off;
                }
                else if(E_.overlap(rng)) {
                    if (E_.begin < rng.begin) E_.end = rng.begin-1;
                    else E_.begin = rng.end+1;
                }
                else if(it+1 < Array<Range<T>, RMemory<Range<T>>>::end() && E_.end < rng.begin && rng.end < (it+1)->begin) break;
            }
        }
    }
    template <typename T>
    srange Region<T>::find(const T &val) const {
        if (Array<Range<T>, RMemory<Range<T>>>::empty()) return srange(-1, -1);
        if (val < Array<Range<T>, RMemory<Range<T>>>::first().begin) return srange(-1, 0);
        else if (Array<Range<T>, RMemory<Range<T>>>::last().end < val)
            return srange(Array<Range<T>, RMemory<Range<T>>>::size()-1, Array<Range<T>, RMemory<Range<T>>>::size());
        sforeachi(*this) {
            if (Array<Range<T>, RMemory<Range<T>>>::at(i).include(val)) return srange(i, i);
            if (i+1 < Array<Range<T>, RMemory<Range<T>>>::size() &&
                Array<Range<T>, RMemory<Range<T>>>::at(i).end < val &&
                val < Array<Range<T>, RMemory<Range<T>>>::at(i+1).begin) return srange(i, i+1);
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
        if (Array<Range<T>, RMemory<Range<T>>>::size() != reg.size()) return false;
        sforeachi(*this) { if(Array<Range<T>, RMemory<Range<T>>>::at(i) != reg[i]) return false; }
        return true;
    }
    template <typename T>
    bool Region<T>::operator != (const Region<T> &reg) const { return !(*this==reg); }
    template <typename T>
    Region<T> complement(const Range<T> &rng, const Region<T> &region) {
        Region<T> comp;
        if (rng.begin < region.first().begin)
            comp.add(Range<T>(rng.begin, region.first().begin-1));
        sforin(it, region.begin(), region.end()-1)
        comp.add(Range<T>(it->end, (it+1)->begin-1));
        if (region.last().end < rng.end)
            comp.add(Range<T>(region.last().end+1, rng.end));
        return comp;
    }
}

#endif
