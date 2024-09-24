#ifndef SLIB_MAPITER_H
#define SLIB_MAPITER_H
#include "sbasic/container.h"
namespace slib {
    enum class BUCKET_FLAG : subyte {
        EMPTY = 0x00,
        FILLED = 0x01,
        END = 0x10,
    };
    /**
    * @class MapData
    *\~english @brief Hashmap data bucket class.
    *\~japanese @brief ハッシュマップ用データコンテナクラス
    */
    template<class Key, class Val>
    class MapData {
        friend Map<Key, Val>;
        friend MapIterator<Key, Val>;
        friend MapCIterator<Key, Val>;

    protected:
        BUCKET_FLAG _flag;
        MapData<Key, Val>* _next;
        Key _key;
        Val _val;

    public:
        MapData() : _flag(BUCKET_FLAG::EMPTY), _next(this + 1) {}
        MapData(const std::pair<Key, Val>& p) : MapData<Key, Val>() {
            _flag = BUCKET_FLAG::FILLED;
            _key = p.first; _val = p.second;
        }
        MapData(const MapData<Key, Val>& dat) {
            _flag = dat._flag; _key = dat._key; _val = dat._val; _next = dat._next;
        }
        ~MapData() {}
        MapData<Key, Val>& operator=(const MapData<Key, Val>& dat) {
            _flag = dat._flag; _next = dat._next; 
            _key = dat._key; _val = dat._val;
            return *this;
        }
        Key& key() { return _key; }
        const Key& key() const { return _key; }
        Val& value() { return _val; }
        const Val& value() const { return _val; }
        const MapData<Key, Val>* next() const { return _next; }
        bool filled() const { return _flag == BUCKET_FLAG::FILLED; }
        void reserve(const Key& k) { _flag = BUCKET_FLAG::FILLED; _key = k; _val = Val(); }
        void set(Val&& v) noexcept { _val = std::forward<Val&&>(v); }
        void set(const Val& v) { _val = v; }
        void release() { _flag = BUCKET_FLAG::EMPTY; }
		void insert(MapData* dat) { dat->_next = _next; _next = dat; }
    };

    /**
    * @class MapIterator
    *\~english @brief Iterator for map object
    *\~japanese @brief マップ用のイテレータクラス
    */
    template<class Key, class Val>
    class MapIterator {
		friend Map<Key, Val>;
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef MapData<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef MapData<Key, Val>* pointer;
        typedef MapData<Key, Val>& reference;
    public:
        MapData<Key, Val> *_ptr;
    public:
        MapIterator(MapData<Key, Val> *p) : _ptr(p) {
            while (_ptr && _ptr->_next && !_ptr->filled()) _ptr = const_cast<MapData<Key, Val> *>(_ptr->_next);
        }
        MapIterator(const MapIterator &it) : _ptr(it._ptr) {}
        ~MapIterator() {}
        MapIterator& operator=(const MapIterator &it) { _ptr = it._ptr; return *this; }
        reference operator*() { return *_ptr; }
        pointer operator->() { return _ptr; }
        reference operator[](std::ptrdiff_t diff) { 
            auto it = *this; 
            sforin(i, 0, diff) ++it;
            return *(it._ptr);
        }
        MapIterator &operator ++() {
            if (_ptr->_next) { do { _ptr = _ptr->_next; } while (_ptr->_next && !_ptr->filled()); }
            return *this;
        }
        MapIterator operator ++(int) { return ++MapIterator(_ptr); }
        MapIterator &operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
        MapIterator operator +(std::ptrdiff_t diff) { auto it = MapIterator(_ptr); sforin(i, 0, diff) ++it; return it; }
        void swap(MapIterator it1, MapIterator it2) { auto temp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = temp; }
        MapData<Key, Val>* ptr() { return _ptr; }
        bool operator <(const MapIterator& it) const { return _ptr < it._ptr; }
        bool operator ==(const MapIterator &it) const { return _ptr == it._ptr; }
        bool operator !=(const MapIterator &it) const { return _ptr != it._ptr; }
    };
    /**
    * @class MapCIterator
    *\~english @brief Const iterator for map object
    *\~japanese @brief マップ用の不変イテレータクラス
    */
    template<class Key, class Val>
    class MapCIterator {
		friend Map<Key, Val>;
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef const MapData<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const MapData<Key, Val>* pointer;
        typedef const MapData<Key, Val>& reference;
    private:
        const MapData<Key, Val> *_ptr;
    public:
        MapCIterator(const MapData<Key, Val> *p) : _ptr(p) {
            while (_ptr && _ptr->_next && !_ptr->filled()) _ptr = _ptr->_next;
        }
        MapCIterator(const MapCIterator &it) : _ptr(it._ptr) {}
        ~MapCIterator() {}
        MapCIterator& operator=(const MapCIterator &it) { _ptr = it._ptr; return *this; }
        reference operator *() { return *_ptr; }
        pointer operator ->() { return _ptr; }
        reference operator [](std::ptrdiff_t diff) {
            auto it = *this;
            sforin(i, 0, diff) ++it;
            return *(it._ptr);
        }
        MapCIterator &operator ++() {
            if (_ptr->_next) { do { _ptr = _ptr->_next; } while (_ptr->_next && !_ptr->filled()); } 
            return *this;
        }
        MapCIterator operator ++(int) { return ++MapCIterator(_ptr); }
        MapCIterator &operator +=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
        MapCIterator operator +(std::ptrdiff_t diff) { auto it = MapCIterator(_ptr); sforin(i, 0, diff) ++it; return it; }
        void swap(MapCIterator it1, MapCIterator it2) { auto temp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = temp; }
        const MapData<Key, Val>* ptr() { return _ptr; }
        bool operator <(const MapCIterator &it) const { return _ptr < it._ptr; }
        bool operator ==(const MapCIterator &it) const { return _ptr == it._ptr; }
        bool operator !=(const MapCIterator &it) const { return _ptr != it._ptr; }
    };
}

#endif
