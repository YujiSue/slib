#ifndef SLIB_MAP_H
#define SLIB_MAP_H

#include "sbasic/memory.h"
#include "sbasic/array.h"
#include "sbasic/mapiter.h"
#include "sbasic/exception.h"

#define ii(X,Y) slib::kvpair<sinteger, sinteger>((X),(Y))
#define ki(X,Y) slib::kvpair<slib::String, suinteger>((X),(Y))
#define ks(X,Y) slib::kvpair<slib::String, slib::String>((X),(Y))

#define sorder slib::Map<sinteger, sinteger>
#define sindex slib::Map<slib::String, suinteger>
#define sattribute slib::Map<slib::String, slib::String>

namespace slib {
    template<class Key, class Val>
    class Map {
    public:
        typedef Array<Key> keyarray;
        
    protected:
        size_t _seed, _size;
		map_data<Key, Val>* _begin, * _end, * _vacant;
        
    private:
        void _remap(size_t s);
        map_data<Key, Val> *_find(const Key &key);
        const map_data<Key, Val> *_cfind(const Key &key) const;
        
    public:
        Map();
        Map(std::initializer_list<std::pair<Key, Val>> li);
        Map(std::initializer_list<kvpair<Key, Val>> li);
		Map(Map &&map) noexcept;
        Map(const Map &map);
        ~Map();
        Map &operator=(const Map &map);
        
        Val &operator[](const Key &key);
        const Val &operator[](const Key &key) const;
        Val &at(const Key &key);
        const Val &at(const Key &key) const;
        
        smap_iter<Key, Val> begin();
        smap_citer<Key, Val> begin() const;
        smap_iter<Key, Val> end();
        smap_citer<Key, Val> end() const;
        
        map_data<Key, Val> *ptr();
        const map_data<Key, Val> *ptr() const;
        bool empty() const;
        size_t size() const;
        size_t capacity() const;
        void swap(Map &map);
        
        keyarray keys() const;
        Key rlookup(const Val &val) const;
        bool hasKey(const Key &key) const;
        
        void set(const Key &key, const Val &val);
        void insert(const std::pair<Key, Val> &pair);
        void insert(const kvpair<Key, Val> &pair);
        void append(const Map<Key, Val> &map);
        void remove(const Key &key);
        void clear();
        void reserve(size_t s);
		void release();
		void discard();

        bool operator < (const Map &map) const;
        bool operator==(const Map &map) const;
    };
	extern inline sindex makeIndex(const stringarray& strarray, const intarray& iarray) {
		sindex map;
		auto kit = strarray.begin();
		auto vit = iarray.begin();
		while (kit < strarray.end()) {
			map.set(*kit, *vit);
			++kit; ++vit;
		}
		return map;
	}
	template<class Key, class Val>
	extern String toString(const Map<Key, Val>& map, const char* sep = ";", const char* part = "=") {
		String str;
		if (!map.empty()) sforeach(map) str << E_.key << part << E_.value << sep;
		if (!str.empty()) str.resize(str.length() - strlen(sep));
		return str;
	}
	template<class Key, class Val>
	extern inline std::ostream& operator<<(std::ostream& os, const Map<Key, Val>& map) { return os << toString(map); }
    /*============================================================*/
    template<class Key>
    inline size_t hashIdx(const Key &key, const size_t &seed) {
		return std::hash<Key>{}(key) % seed;
    }
    template<class Key, class Val>
    void Map<Key, Val>::_remap(size_t s) {
        Map tmp;
        tmp.reserve(s);
        sforeach(*this) tmp.insert(E_);
        swap(tmp);
    }
    template<class Key, class Val>
    map_data<Key, Val> *Map<Key, Val>::_find(const Key &key) {
		auto ptr = _begin + hashIdx(key, _seed);
		if (ptr->filled && ptr->pair().key == key) return ptr;
		if (ptr->next < _begin) {
			auto tmp = ptr;
			while (tmp->next && tmp->next < _begin) {
				tmp = tmp->next;
				if (tmp->pair().key == key) return tmp;
			}
		}
		if (_vacant->next == _begin) {
			_remap(((capacity() - 1) << 1) + 1);
			ptr = _begin + hashIdx(key, _seed);
		}
		if (ptr->filled) {
			auto tmp = _vacant;
			_vacant = tmp->next;
			tmp->init(key);
			ptr->insert(tmp);
			++_size;
			return tmp;
		}
		else {
			ptr->init(key);
			++_size;
			return ptr;
		}
    }
    template<class Key, class Val>
    const map_data<Key, Val> *Map<Key, Val>::_cfind(const Key &key) const {
        if (!_size) return nullptr;
		map_data<Key, Val>* ptr = _begin + hashIdx(key, _seed);
		if (ptr->filled && ptr->pair().key == key) return ptr;
		if (ptr->next < _begin) {
			auto tmp = ptr;
			while (tmp->next && tmp->next < _begin) {
				tmp = tmp->next;
				if (tmp->pair().key == key) return tmp;
			}
		}
        return nullptr;
    }
    template<class Key, class Val>
    Map<Key, Val>::Map()
        : _seed(0), _size(0), _begin(nullptr), _end(nullptr), _vacant(nullptr) {}
    template<class Key, class Val>
    Map<Key, Val>::Map(std::initializer_list<std::pair<Key, Val>> li) : Map() {
		if (li.size() < 8) reserve(17);
		else reserve(li.size() * 2 + 1);
        if (li.size()) sforeach(li) insert(E_);
    }
    template<class Key, class Val>
    Map<Key, Val>::Map(std::initializer_list<kvpair<Key, Val>> li) : Map() {
		if (li.size() < 8) reserve(17);
		else reserve(li.size() * 2 + 1);
        if (li.size()) sforeach(li) insert(E_);
    }
	template<class Key, class Val>
	Map<Key, Val>::Map(Map&& map) noexcept {
		_seed = map._seed;
		_size = map._size;
		_begin = map._begin;
		_end = map._end;
		_vacant = map._vacant;
		map.discard();
	}
	template<class Key, class Val>
    Map<Key, Val>::Map(const Map &map) : Map() {
        reserve(map.capacity());
        if (map.size()) sforeach(map) insert(E_);
    }
    template<class Key, class Val>
    Map<Key, Val>::~Map() { release(); }
    template<class Key, class Val>
    Map<Key, Val> &Map<Key, Val>::operator=(const Map &map) {
        clear();
        if (capacity() < map.capacity()) reserve(map.capacity());
        if (map.size()) sforeach(map) insert(E_);
        return *this;
    }
    template<class Key, class Val>
	Val& Map<Key, Val>::operator[](const Key& key) { return at(key); }
    template<class Key, class Val>
    const Val &Map<Key, Val>::operator[](const Key &key) const { return at(key); }
    template<class Key, class Val>
    Val &Map<Key, Val>::at(const Key &key) {
        if (!_seed) reserve(17);
        return _find(key)->pair().value;
    }
    template<class Key, class Val>
    const Val &Map<Key, Val>::at(const Key &key) const {
        if (_seed) {
            auto ptr = _cfind(key);
            if (ptr) return ptr->pair().value;
			else return _end->pair().value;
        }
        else throw SException(ERR_INFO, SLIB_NULL_ERROR);
    }
    template<class Key, class Val>
    smap_iter<Key, Val> Map<Key, Val>::begin() { return SMapIterator<Key, Val>(_begin); }
    template<class Key, class Val>
    smap_citer<Key, Val> Map<Key, Val>::begin() const { return SMapCIterator<Key, Val>(_begin); }
    template<class Key, class Val>
    smap_iter<Key, Val> Map<Key, Val>::end() { return SMapIterator<Key, Val>(_end); }
    template<class Key, class Val>
    smap_citer<Key, Val> Map<Key, Val>::end() const { return SMapCIterator<Key, Val>(_end); }
    template<class Key, class Val>
	map_data<Key, Val>* Map<Key, Val>::ptr() { return begin()._ptr; }
    template<class Key, class Val>
    const map_data<Key, Val> *Map<Key, Val>::ptr() const { return begin()._ptr; }
    template<class Key, class Val>
    bool Map<Key, Val>::empty() const { return !_size; }
    template<class Key, class Val>
    size_t Map<Key, Val>::size() const { return _size; }
    template<class Key, class Val>
	size_t Map<Key, Val>::capacity() const { return _seed ? 2 * _seed + 1 : 0; }
    template<class Key, class Val>
    void Map<Key, Val>::swap(Map &map) {
        auto tmps = _seed; _seed = map._seed; map._seed = tmps;
        auto tmpz = _size; _size = map._size; map._size = tmpz;
        auto tmpb = _begin; _begin = map._begin; map._begin = tmpb;
        auto tmpe = _end; _end = map._end; map._end = tmpe;
        auto tmpv = _vacant; _vacant = map._vacant; map._vacant = tmpv;
    }
    template<class Key, class Val>
    Array<Key> Map<Key, Val>::keys() const {
        keyarray array;
        sforeach(*this) array.add(E_.key);
        return array;
    }
    template<class Key, class Val>
    Key Map<Key, Val>::rlookup(const Val &val) const {
        sforeach(*this) { if (E_.value == val) return E_.key; }
		throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
    }
    template<class Key, class Val>
	bool Map<Key, Val>::hasKey(const Key& key) const { return _cfind(key) != nullptr; }
    template<class Key, class Val>
    void Map<Key, Val>::set(const Key &key, const Val &val) { at(key) = val; }
    template<class Key, class Val>
    void Map<Key, Val>::insert(const std::pair<Key, Val> &pair) { at(pair.first) = pair.second; }
    template<class Key, class Val>
    void Map<Key, Val>::insert(const kvpair<Key, Val> &pair) { at(pair.key) = pair.value; }
    template<class Key, class Val>
    void Map<Key, Val>::append(const Map<Key, Val> &map) { sforeach(map) insert(E_); }
    template<class Key, class Val>
    void Map<Key, Val>::remove(const Key &key) {
		auto ptr = _begin + hashIdx(key, _seed);
		if (ptr->filled && ptr->pair().key == key) {
			ptr->release(); --_size;
		}
		else if (ptr->next < _begin) {
			auto tmp = ptr;
			while (ptr->next < _begin) {
				ptr = ptr->next;
				if (ptr->pair().key == key) {
					ptr->release(); tmp->next = ptr->next;
					ptr->next = _vacant; _vacant = ptr;
					--_size; break;
				}
			}
		}
    }
    template<class Key, class Val>
    void Map<Key, Val>::clear() {
		if (_size) {
			sforeach(*this) it._ptr->release();
			_vacant = _end - 2 * _seed;
			auto ptr = _vacant;
			while (ptr < _end) { ptr->next = ptr + 1; ++ptr; }
			_end->next = nullptr; _size = 0;
		}
    }
    template<class Key, class Val>
    void Map<Key, Val>::reserve(size_t s) {
		if (capacity() < s) {
			if (_seed) _remap(s);
			else {
				_seed = s / 2;
				if (!(s % 2)) ++s;
				_vacant = SMemory<map_data<Key, Val>>::alloc(s);
				SMemory<map_data<Key, Val>>::init(_vacant, s);
				_begin = _vacant + _seed;
				_end = _begin + _seed;
				_end->init(Key());
				_end->next = nullptr;
			}
		}
    }
	template<class Key, class Val>
	void Map<Key, Val>::release() {
		clear();
		if (_seed) SMemory<map_data<Key, Val>>::dealloc(_vacant);
		discard();
	}
	template<class Key, class Val>
	void Map<Key, Val>::discard() {
		_seed = 0; _size = 0;
		_begin = nullptr; _end = nullptr; _vacant = nullptr;
	}
    template<class Key, class Val>
    bool Map<Key, Val>::operator < (const Map &map) const {
        if (size() != map.size()) return size() < map.size();
        auto keys = keyset();
        sforeach(keys) {
            if(!(map.hasKey(E_))) return false;
            else if(at(E_) < map.at(E_)) return true;
        }
        return false;
    }
    template<class Key, class Val>
    bool Map<Key, Val>::operator==(const Map &map) const {
        if (size() != map.size()) return false;
        auto keys = keyset();
        sforeach(keys) {
            if(!(map.hasKey(E_)) || !(at(E_) == map.at(E_))) return false;
        }
        return true;
    }
}

#endif
