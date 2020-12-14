#ifndef SLIB_MAPITER_H
#define SLIB_MAPITER_H

#include "sconfig.h"

#define smap_iter slib::SMapIterator
#define smap_citer slib::SMapCIterator
    
namespace slib {
	template<class Key, class Val>
	class Map;
    template<class Key, class Val>
    struct kvpair {
        Key key;
        Val value;
        
        kvpair();
        kvpair(const Key &k, const Val &v);
        kvpair(const std::pair<Key, Val> &pair);
        kvpair(const kvpair &pair);
        ~kvpair();
        kvpair &operator=(const kvpair &pair);
    };
    template<class Key, class Val>
    struct map_data {
		bool filled;
		subyte data[sizeof(kvpair<Key, Val>)];
        map_data *next;
        
        map_data();
        map_data(const map_data &dat);
        ~map_data();
        map_data &operator=(const map_data &dat);
		kvpair<Key, Val>& pair();
		const kvpair<Key, Val>& pair() const;
		void init(const Key& k);
		void release();
		void insert(map_data* dat);
    };
    template<class Key, class Val>
    class SMapIterator {
		friend Map<Key, Val>;
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef kvpair<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef kvpair<Key, Val>* pointer;
        typedef kvpair<Key, Val>& reference;

    private:
        map_data<Key, Val> *_ptr;
        
    public:
        SMapIterator(map_data<Key, Val> *p);
        SMapIterator(const SMapIterator &it);
        ~SMapIterator();
        SMapIterator& operator=(const SMapIterator &it);
        reference operator*();
        pointer operator->();
        reference operator[](std::ptrdiff_t diff);
        SMapIterator &operator ++();
        SMapIterator operator ++(int);
        SMapIterator &operator +=(std::ptrdiff_t diff);
        SMapIterator operator +(std::ptrdiff_t diff);
        void swap(SMapIterator it1, SMapIterator it2);
        bool operator <(const SMapIterator &it) const;
        bool operator ==(const SMapIterator &it) const;
        bool operator !=(const SMapIterator &it) const;
    };
    template<class Key, class Val>
    class SMapCIterator {
		friend Map<Key, Val>;
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef const kvpair<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const kvpair<Key, Val>* pointer;
        typedef const kvpair<Key, Val>& reference;
        
    private:
        const map_data<Key, Val> *_ptr;
        
    public:
        SMapCIterator(map_data<Key, Val> *p);
        SMapCIterator(const SMapCIterator &it);
        ~SMapCIterator();
        SMapCIterator& operator=(const SMapCIterator &it);
        reference operator *();
        pointer operator ->();
        reference operator [](std::ptrdiff_t diff);
        SMapCIterator &operator ++();
        SMapCIterator operator ++(int);
        SMapCIterator &operator +=(std::ptrdiff_t diff);
        SMapCIterator operator +(std::ptrdiff_t diff);
        void swap(SMapCIterator it1, SMapCIterator it2);
        bool operator <(const SMapCIterator &it) const;
        bool operator ==(const SMapCIterator &it) const;
        bool operator !=(const SMapCIterator &it) const;
    };
    
    /*============================================================*/
    template<class Key, class Val>
    kvpair<Key, Val>::kvpair() : key(initVal<Key>()), value(initVal<Val>()) {}
    template<class Key, class Val>
    kvpair<Key, Val>::kvpair(const Key &k, const Val &v) : key(k), value(v) {}
    template<class Key, class Val>
    kvpair<Key, Val>::kvpair(const std::pair<Key, Val> &pair) : key(pair.first), value(pair.second) {}
    template<class Key, class Val>
    kvpair<Key, Val>::kvpair(const kvpair &pair) : key(pair.key), value(pair.value) {}
    template<class Key, class Val>
    kvpair<Key, Val>::~kvpair() {}
    template<class Key, class Val>
    kvpair<Key, Val> &kvpair<Key, Val>::operator=(const kvpair<Key, Val> &pair) {
        key = pair.key; value = pair.value; return *this;
    }
    
    /*============================================================*/
    template<class Key, class Val>
	map_data<Key, Val>::map_data() : filled(false), next(nullptr) { memset(data, 0, sizeof(kvpair<Key, Val>));  next = this + 1; }
    template<class Key, class Val>
    map_data<Key, Val>::map_data(const map_data<Key, Val> &dat) : map_data() {
		filled = dat.filled; next = dat.next; 
		*reinterpret_cast<const kvpair<Key, Val>*>(&data[0]) = *reinterpret_cast<const kvpair<Key, Val>*>(&dat.data[0]);
	}
    template<class Key, class Val>
	map_data<Key, Val>::~map_data() { if (filled) release(); }
    template<class Key, class Val>
    map_data<Key, Val> &map_data<Key, Val>::operator=(const map_data<Key, Val> &dat) {
		filled = dat.filled; next = dat.next; 
		*reinterpret_cast<const kvpair<Key, Val>*>(&data[0]) = *reinterpret_cast<const kvpair<Key, Val>*>(&dat.data[0]);
		return *this;
    }
	template<class Key, class Val>
	inline kvpair<Key, Val>& map_data<Key, Val>::pair() { return *reinterpret_cast<kvpair<Key, Val>*>(&data[0]); }
	template<class Key, class Val>
	inline const kvpair<Key, Val>& map_data<Key, Val>::pair() const { return *reinterpret_cast<const kvpair<Key, Val>*>(&data[0]); }
	template<class Key, class Val>
	inline void map_data<Key, Val>::init(const Key& k) {
		filled = true;
		new(reinterpret_cast<kvpair<Key, Val>*>(&data[0])) kvpair<Key, Val>(k, Val());
	}
	template<class Key, class Val>
	inline void map_data<Key, Val>::release() {
		filled = false;
		(reinterpret_cast<kvpair<Key, Val>*>(&data[0]))->~kvpair<Key, Val>();
	}
	template<class Key, class Val>
	inline void map_data<Key, Val>::insert(map_data* dat) { dat->next = next; next = dat; }
    /*============================================================*/
    template<class Key, class Val>
    SMapIterator<Key, Val>::SMapIterator(map_data<Key, Val> *p) : _ptr(p) {
		while (_ptr->next && !_ptr->filled) _ptr = _ptr->next;
	}
    template<class Key, class Val>
    SMapIterator<Key, Val>::SMapIterator(const SMapIterator &it) : _ptr(it._ptr) {}
    template<class Key, class Val>
    SMapIterator<Key, Val>::~SMapIterator() {}
    template<class Key, class Val>
    SMapIterator<Key, Val> &SMapIterator<Key, Val>::operator=(const SMapIterator &it) { _ptr = it._ptr; return *this; }
    template<class Key, class Val>
	inline kvpair<Key, Val> &SMapIterator<Key, Val>::operator*() { return _ptr->pair(); }
    template<class Key, class Val>
	inline kvpair<Key, Val> *SMapIterator<Key, Val>::operator ->() { return reinterpret_cast<kvpair<Key, Val>*>(_ptr->data); }
    template<class Key, class Val>
	inline kvpair<Key, Val> &SMapIterator<Key, Val>::operator[](std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return _ptr->pair(); }
    template<class Key, class Val>
	inline SMapIterator<Key, Val> &SMapIterator<Key, Val>::operator ++() {
        if (_ptr->next) { do { _ptr = _ptr->next; } while (_ptr->next && !_ptr->filled); } return *this;
    }
    template<class Key, class Val>
	inline SMapIterator<Key, Val> SMapIterator<Key, Val>::operator ++(int) { return ++SMapIterator(_ptr); }
    template<class Key, class Val>
	inline SMapIterator<Key, Val> &SMapIterator<Key, Val>::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
    template<class Key, class Val>
	inline SMapIterator<Key, Val> SMapIterator<Key, Val>::operator +(std::ptrdiff_t diff) { auto it = SMapIterator(_ptr); sforin(i, 0, diff) ++it; return it; }
    template<class Key, class Val>
	inline void SMapIterator<Key, Val>::swap(SMapIterator it1, SMapIterator it2) { auto temp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = temp; }
    template<class Key, class Val>
	inline bool SMapIterator<Key, Val>::operator <(const SMapIterator<Key, Val> &it) const { return _ptr < it._ptr; }
    template<class Key, class Val>
	inline bool SMapIterator<Key, Val>::operator ==(const SMapIterator<Key, Val> &it) const { return _ptr == it._ptr; }
    template<class Key, class Val>
	inline bool SMapIterator<Key, Val>::operator !=(const SMapIterator<Key, Val> &it) const { return _ptr != it._ptr; }
    
    /*============================================================*/
    template<class Key, class Val>
    SMapCIterator<Key, Val>::SMapCIterator(map_data<Key, Val> *p) : _ptr(p) {
		while (_ptr->next && !_ptr->filled) _ptr = _ptr->next;
	}
    template<class Key, class Val>
    SMapCIterator<Key, Val>::SMapCIterator(const SMapCIterator &it) : _ptr(it._ptr) {}
    template<class Key, class Val>
    SMapCIterator<Key, Val>::~SMapCIterator() {}
    template<class Key, class Val>
    SMapCIterator<Key, Val> &SMapCIterator<Key, Val>::operator=(const SMapCIterator &it) { _ptr = it._ptr; return *this; }
    template<class Key, class Val>
	inline const kvpair<Key, Val> &SMapCIterator<Key, Val>::operator *() { return _ptr->pair(); }
    template<class Key, class Val>
	inline const kvpair<Key, Val> *SMapCIterator<Key, Val>::operator ->() { return reinterpret_cast<const kvpair<Key, Val>*>(_ptr->data); }
    template<class Key, class Val>
	inline const kvpair<Key, Val> &SMapCIterator<Key, Val>::operator [](std::ptrdiff_t diff) {  sforin(i, 0, diff) ++(*this); return _ptr->pair(); }
    template<class Key, class Val>
	inline SMapCIterator<Key, Val> &SMapCIterator<Key, Val>::operator ++() {
		if (_ptr->next) { do { _ptr = _ptr->next; } while (_ptr->next && !_ptr->filled); } return *this;
    }
    template<class Key, class Val>
	inline SMapCIterator<Key, Val> SMapCIterator<Key, Val>::operator ++(int) { return ++SMapCIterator(_ptr); }
    template<class Key, class Val>
	inline SMapCIterator<Key, Val> &SMapCIterator<Key, Val>::operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
    template<class Key, class Val>
	inline SMapCIterator<Key, Val> SMapCIterator<Key, Val>::operator +(std::ptrdiff_t diff) { auto it = SMapCIterator(_ptr); sforin(i, 0, diff) ++it; return it; }
    template<class Key, class Val>
	inline void SMapCIterator<Key, Val>::swap(SMapCIterator<Key, Val> it1, SMapCIterator<Key, Val> it2) { auto temp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = temp; }
    template<class Key, class Val>
	inline bool SMapCIterator<Key, Val>::operator <(const SMapCIterator<Key, Val> &it) const { return _ptr < it._ptr; }
    template<class Key, class Val>
	inline bool SMapCIterator<Key, Val>::operator ==(const SMapCIterator<Key, Val> &it) const { return _ptr == it._ptr; }
    template<class Key, class Val>
	inline bool SMapCIterator<Key, Val>::operator !=(const SMapCIterator<Key, Val> &it) const { return _ptr != it._ptr; }
}

#endif
