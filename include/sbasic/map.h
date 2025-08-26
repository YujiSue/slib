#ifndef SLIB_MAP_H
#define SLIB_MAP_H
#include "smath/numeric.h"
#include "sbasic/mapiter.h"
#include "sbasic/array.h"

namespace slib {
    template<class Key>
    inline size_t hashIdx(const Key& key, const size_t seed) { return std::hash<Key>{}(key) % seed; }
    
    /**
    * @class Memory
    *\~english @brief Hashmap class
    *\~japanese @brief ハッシュマップクラス
    */
    template<class Key, class Val>
    class Map {
    protected:
        size_t _capacity, _size, _seed;
        MapData<Key, Val>* _ptr, *_vacant;
    private:
        void setSeed(const size_t s = 0) { _seed = s ? s : _capacity * 3 / 4; }
        void remap(const size_t s) {
            Map tmp;
            tmp.reserve(s);
            sfor(*this) tmp.insert($_);
            swap(tmp);
        }
        
    public:
        Map() : _capacity(0), _size(0), _seed(0), _ptr(nullptr), _vacant(nullptr) {}
        Map(std::initializer_list<Pair<Key, Val>> li) : Map<Key, Val>() {
            reserve(li.size() * 4 / 3 + 1);
            sfor(li) insert($_);
        }
        Map(Map<Key, Val>&& map) noexcept { swap(map); }
        Map(const Map<Key, Val>& map) : Map<Key, Val>() {
            reserve(map.capacity());
            sfor(map) insert($_);
        }
        ~Map() {
            if (_ptr) {
                auto it = _ptr;
                sforin(i, 0_u, _capacity) { Memory<MapData<Key, Val>>::destroy($); $NEXT; }
                Memory<MapData<Key, Val>>::dealloc(_ptr);
            }
        }
        Map& operator=(Map<Key, Val>&& map) noexcept {
            swap(map);
            return *this;
        }
        Map& operator=(const Map<Key, Val>& map) {
            clear();
            reserve(map.capacity());
            sfor(map) insert($_);
            return *this;
        }
        bool empty() const { return !_size; }
        size_t size() const { return _size; }
        size_t capacity() const { return _capacity; }
        void reserve(const size_t s) {
            if (!_capacity) {
                _capacity = s < 5 ? 5 : s;
                _ptr = Memory<MapData<Key, Val>>::alloc(_capacity);
                setSeed();
                auto it = _ptr;
                sforin(i, 0_u, _capacity) { Memory<MapData<Key, Val>>::init($); $NEXT; }
                it = _ptr + _capacity - 1;
                _vacant = _ptr + _seed;
                (_vacant - 1)->_next = $;
                $_._next = nullptr;
                $_._flag = BUCKET_FLAG::END;
            }
            else if (_capacity <= s) {
                Map<Key, Val> tmp;
                tmp.reserve(s);
                if (_size) { sfor(*this) tmp.insert($_); }
                swap(tmp);
            }
        }
        MapData<Key, Val>* vacant() const { return _vacant; }
        MapIterator<Key, Val> iterAt(const Key& key) {
            if (!_ptr) reserve(5);
            while (_vacant->_flag == BUCKET_FLAG::END) {
                remap((capacity() * 4 / 3) + 1);
            }
            auto p = _ptr + hashIdx(key, _seed), it = p;
            if ($_.filled()) {
                if ($_.key() == key) return it;
                else $ = $_._next;
                while (_ptr + _seed <= $) {
                    if ($_.key() == key) return it;
                    $ = $_._next;
                }
                auto newp = _vacant;
                _vacant = _vacant->_next;
                newp->reserve(key);
                p->insert(newp);
                ++_size;
                return newp;
            }
            else {
                p->reserve(key);
                ++_size;
                return p;
            }
        }
        MapCIterator<Key, Val> iterAt(const Key& key) const {
            if (_capacity) {
                auto p = _ptr + hashIdx(key, _seed), it = p;
                if ($_.filled()) {
                    if ($_.key() == key) return $;
                    else $ = $_._next;
                    while (_ptr + _seed <= $) {
                        if ($_.key() == key) return $;
                        $ = $_._next;
                    }
                }
            }
            return end();
        }
        Val& operator[](const Key& key) { return at(key); }
        const Val& operator[](const Key& key) const { return at(key); }
        Val& at(const Key& key) { 
            auto it = iterAt(key);
            if (it == end() || !it->filled()) {
                set(key, Val());
                it = iterAt(key);
            }
            return it->value();
        }
        const Val& at(const Key& key) const {
            auto it = iterAt(key);
            if (it == end() || !it->filled()) throw NotFoundException(nofoundErrorText(key, "map"));
            return it->value();
        }
        MapIterator<Key, Val> begin() { return _ptr; }
        MapCIterator<Key, Val> begin() const { return _ptr; }
        MapIterator<Key, Val> end() { return _ptr ? _ptr + _capacity - 1 : _ptr; }
        MapCIterator<Key, Val> end() const { return _ptr ? _ptr + _capacity - 1 : _ptr; }

        Array<Key> keyset(bool sort = false) const {
            Array<Key> keys;
            sfor(*this) keys.add($_.key());
            if (sort) keys.sort();
            return keys;
        }
        Key rlookup(const Val& val) const {
            sfor(*this) { if ($_.value() == val) return $_.key(); }
            throw Exception();
        }
        bool hasKey(const Key& key) const { 
            auto it = iterAt(key); 
            return it != end() && it->filled(); 
        }
        void set(const Key& key, const Val& val) { 
            iterAt(key)->set(val);
        }
        void insert(const Pair<Key, Val>& pair) { set(pair.first, pair.second); }
        void insert(const MapData<Key, Val>& md) { set(md.key(), md.value()); }
        void append(const Map<Key, Val>& map) { sfor(map) { if (!hasKey($_.key())) insert($_); } }
        void update(const Map<Key, Val>& map) { sfor(map) { insert($_); } }
        void remove(const Key& key) {
            if (!hasKey(key)) throw NotFoundException("");
            auto it = iterAt(key);
            auto p = _ptr + hashIdx(key, _seed);
            if (p == $.ptr()) { 
                auto nxt = p->_next;
                if (nxt == p + 1 || nxt->_flag == BUCKET_FLAG::END) p->release();
                else if (nxt->_flag == BUCKET_FLAG::FILLED) {
                    p->_key = nxt->_key;
                    p->_val = nxt->_val;
                    p->_next = nxt->_next;
                    nxt->release();
                }
            }
            else {
                while (p->_next != $.ptr()) { p = p->_next; }
                $_.release(); p->_next = $_._next;
                $_._next = _vacant; _vacant = $.ptr(); 
            }
            --_size;
        }
        void clear() {
            if (empty()) return;
            sfor(*this) { $_.release(); }
            auto it = _ptr;
            sforin(i, 0_u, _capacity - 1) { $->_next = $ + 1; NEXT_($); }
            $->_next = nullptr;
            $->_flag = BUCKET_FLAG::END;
            _vacant = _ptr + _seed;
            (_vacant - 1)->_next = $;
            _size = 0;
        }
        void swap(Map<Key, Val>& map) {
            Memory<MapData<Key, Val>*>::swap(&_ptr, &map._ptr);
            Memory<MapData<Key, Val>*>::swap(&_vacant, &map._vacant);
            Memory<size_t>::swap(&_capacity, &map._capacity);
            Memory<size_t>::swap(&_size, &map._size);
            Memory<size_t>::swap(&_seed, &map._seed);
        }
        bool operator<(const Map<Key, Val>& map) const {
            if (size() != map.size()) return size() < map.size();
            sfor(*this) {
                if (!(map.hasKey($_.key()))) return false;
                else if (at($_.key()) < map.at($_.key())) return true;
            }
            return false;
        }
        bool operator==(const Map<Key, Val>& map) const {
            if (size() != map.size()) return false;
            sfor(*this) {
                if (!map.hasKey($_.key()) || !(at($_.key()) == map.at($_.key()))) return false;
            }
            return true;
        }
    };
    template<class Key, class Val>
    extern inline String toString(const Map<Key, Val>& map, const char* sep = ":", const char* del = ",") {
        if (map.empty()) return "";
        String str;
        auto it = map.begin();
        str << $_.key() << sep << $_.value(); NEXT_($);
        while (it < map.end()) {
            str << del << $_.key() << sep << $_.value(); NEXT_($);
        }
        return str;
    }
}
using sorder = slib::Map<int, int>;
using sindex = slib::Map<slib::String, int>;
using sattribute = slib::Map<slib::String, slib::String>;

#define SS_(X,Y) slib::Pair<slib::String,slib::String>((X),(Y))
#define SI_(X,Y) slib::Pair<slib::String,int>((X),(Y))
#define II_(X,Y) slib::Pair<int,int>((X),(Y))

template<class Key, class Val>
extern inline std::ostream& operator<<(std::ostream& os, const slib::Map<Key, Val>& map) {
    if (map.empty()) return os;
    auto it = map.begin();
    os << $_.key() << ":" << $_.value(); NEXT_($);
    while (it < map.end()) {
        os << "," << $_.key() << ":" << $_.value(); NEXT_($);
    }
    return os;
}

#endif