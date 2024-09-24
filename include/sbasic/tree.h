#ifndef SLIB_TREE_H
#define SLIB_TREE_H

#include "sbasic/memory.h"
#include "sbasic/range.h"
#include "sbasic/exception.h"
#include "sbasic/treeiter.h"

namespace slib {
	template<class Key, class Val>
	class Tree {
	public:
		typedef Array<Key> keyarray;
    protected:
		size_t _size;
		TreeData<Key, Val>* _root;

	private:
		void _rearrange(TreeData<Key, Val>* node) {
			auto p = node->parent;
			if (!p || p->isBlack()) return;
			else {
				auto g = p->parent, u = p->isLeft() ? g->child[1] : g->child[0];
				if (u && u->isRed()) {
					p->changeColor(); g->changeColor(); u->changeColor();
					_rearrange(g);
				}
				else {
					if (p->isLeft()) {
						if (node->isRight()) {
							p->lrot();
							auto tmp = p; p = node; node = tmp;
						}
						g->rrot(); p->changeColor(); g->changeColor();
					}
					else {
						if (node->isLeft()) {
							p->rrot();
							auto tmp = p; p = node; node = tmp;
						}
						g->lrot(); p->changeColor(); g->changeColor();
					}
				}
			}
		}
		TreeData<Key, Val>* _insert(TreeData<Key, Val>* parent, bool left, const Key& k, const Val& v = initVal<Val>()) {
			auto node = parent->insert(new TreeData<Key, Val>(parent, k, v), left);
			_rearrange(node);
			++_size;
			return node;
		}
		TreeData<Key, Val>* _find(const Key& k, TreeData<Key, Val>* ori) {
			if (!_root) {
				_root = new TreeData<Key, Val>();
				_root->data.key = k;
				_size = 1;
				ori = _root;
			}
			if ((*ori) == k) return ori;
			else {
				if ((*ori) < k) {
					if (ori->child[1]) return _find(k, ori->child[1]);
					else return _insert(ori, false, k);
				}
				else {
					if (ori->child[0]) return _find(k, ori->child[0]);
					else return _insert(ori, true, k);
				}
			}
		}
		const TreeData<Key, Val>* _cfind(const Key& k, TreeData<Key, Val>* ori) {
			if (!ori) return nullptr;
			if ((*ori) == k) return ori;
			else {
				if ((*ori) < k) return _cfind(k, ori->child[1]);
				else return _cfind(k, ori->child[0]);
			}
		}
		TreeData<Key, Val>* _begin() const {
			if (!_root) return nullptr;
			TreeData<Key, Val>* p = _root;
			while (p->child[0]) { p = p->child[0]; }
			return p;
		}
    public:
        Tree() : _size(0), _root(nullptr) {}
		Tree(Tree&& tree) : Tree() { swap(tree); }
		Tree(const Tree& tree) {}
		~Tree() { if (_root) delete _root; }
		Tree& operator = (const Tree& tree) {

			return *this;
		}
		Val& operator[](const Key& key) { return at(key); }
		const Val& operator[](const Key& key) const { return at(key); }
		Val& at(const Key& key) { return _find(key, _root)->data.value; }
		const Val& at(const Key& key) const {
			auto node = _cfind(key, _root);
			if (node) return node->data.value;
			//else throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
		}
		TreeIterator<Key, Val> begin() { return TreeIterator<Key, Val>(_begin()); }
		TreeCIterator<Key, Val> begin() const { return TreeCIterator<Key, Val>(_begin()); }
		TreeIterator<Key, Val> end() { return TreeIterator<Key, Val>(nullptr); }
		TreeCIterator<Key, Val> end() const { return TreeCIterator<Key, Val>(nullptr); }
		TreeData<Key, Val>* root() { return _root; }
		const TreeData<Key, Val>* root() const { return _root; }
		bool empty() const { return !_size; }
		size_t size() const { return _size; }
		void swap(Tree& map) { _size = map.size; }
		keyarray keyset() const {
			keyarray keys;
			sfor(*this) { keys.add(E_.key()); }
			return keys;
		}
		Key rlookup(const Val& v) const {
			sfor(*this) {
				if (E_.value() == v) return E_.key();
			}
			//throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
		}
		bool hasKey(const Key& k) const { return _cfind(k) != nullptr; }
		void set(const Key& key, const Val& val) {
			auto node = _find(key, _root);
			node->data.value = val;
		}
		void insert(const std::pair<Key, Val>& pair) { set(pair.first, pair.second); }
		void insert(const KVPair<Key, Val>& pair) { set(pair.key(), pair.value()); }
		void append(const Tree<Key, Val>& tree) { sfor(tree) insert(E_); }
		void remove(const Key& key) {




		}
		void clear() { if (_size) { delete _root; discard(); } }
		void discard() { _root = nullptr; _size = 0; }
    };
	//template<typename T>
	//extern inline std::ostream& operator<<(std::ostream& os, const Tree<T>& tree) { return os << toString(tree); }

    /*============================================================*/
    /*
	template <typename T>
	void List<T>::_expand(size_t s) {
		if (!_capacity) reserve(s + 2);
		else {
			auto cap = _capacity;
			while (cap <= s) cap <<= 1;
			reserve(cap);
		}
	}
	template <typename T>
	list_data<T>* List<T>::_fill() {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init();
		return p;
	}
	template <typename T>
	list_data<T>* List<T>::_fill(T&& val) {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init(std::forward<T&&>(val));
		return p;
	}
	template <typename T>
	list_data<T>* List<T>::_fill(const T& val) {
		auto p = _vacant;
		_vacant = p->next;
		_vacant->prev = nullptr;
		p->init(val);
		return p;
	}
	template <typename T>
	void List<T>::_vacate(list_data<T>* p) {
		p->release();
		p->next->prev = p->prev;
		if (p->prev) p->prev->next = p->next;
		else _begin = p->next;
		_vacant->insertPrev(p);
		_vacant = p;
	}
	template <typename T>
	size_t List<T>::_index(sli_iter<T> iter) {
		auto idx = 0;
		while (iter._ptr != _begin) { ++idx; --iter; }
		return idx;
	}
    template <typename T>
    List<T>::List() : _capacity(0), _size(0), _begin(nullptr), _end(nullptr), _vacant(nullptr) {}
    template <typename T>
    List<T>::List(size_t s) : List<T>() { resize(s); }
    template <typename T>
    List<T>::List(size_t s, const T &val) : List<T>(s) {
        if (s) { 
			auto p = _begin; 
			while (p < _end) { *p = val; p = p->next; }
		}
    }
    template <typename T>
    List<T>::List(std::initializer_list<T> li) : List<T>(li.size()) {
		if (_size) {
			sforeach2(*this, li) E1_ = E2_;
		}
    }
    template <typename T>
    List<T>::List(List<T> &&list) : List<T>() {
        _capacity = list._capacity;
        _size = list._size;
        _begin = list._begin;
        _end = list._end;
        _vacant = list._vacant;
        list.discard();
    }
    template <typename T>
    List<T>::List(const List<T> &list) : List<T>(list.size()) {
        if (!list.empty()) {
			sforeach2(*this, list) E1_ = E2_;
        }
    }
    template <typename T>
    List<T>::~List() { release(); }
    template <typename T>
    List<T> &List<T>::operator = (const List<T> &list) {
        resize(list.size());
        if (_size) {
			sforeach2(*this, list) E1_ = E2_;
        }
		return *this;
    }
    template <typename T>
    List<T> &List<T>::operator = (List<T> &&list) {
        release();
        _capacity = list._capacity;
        _size = list._size;
        _begin = list._begin;
        _end = list._end;
        _vacant = list._vacant;
        list.discard();
        return *this;
    }
    template <typename T>
    T &List<T>::operator[] (sinteger idx) { return at(idx); }
    template <typename T>
    const T &List<T>::operator[] (sinteger idx) const { return at(idx); }
    template <typename T>
    T &List<T>::at(sinteger idx) {
        if (_size) {
			if (idx < 0 && -idx <= _size) return *(end() - (-idx));
			else if (idx < _size) return *(begin() + idx);
        }
        throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    const T &List<T>::at(sinteger idx) const {
        if (_size) {
			if (idx < 0 && -idx < _size) return *(end() - (-idx));
			else if (idx < _size) return *(begin() + idx);
        }
        throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    T &List<T>::first() { return at(0); }
    template <typename T>
    const T &List<T>::first() const { return at(0); }
    template <typename T>
    T &List<T>::last() { return at(-1); }
    template <typename T>
    const T &List<T>::last() const { return at(-1); }
    template <typename T>
    sli_iter<T> List<T>::begin() { return sli_iter<T>(_begin); }
    template <typename T>
    sli_citer<T> List<T>::begin() const { return sli_citer<T>(_begin); }
    template <typename T>
    sli_iter<T> List<T>::end() { return sli_iter<T>(_end); }
    template <typename T>
    sli_citer<T> List<T>::end() const { return sli_citer<T>(_end); }
    template <typename T>
    List<T> List<T>::sublist(size_t off, size_t len) {
		return sublist(begin() + off, len == -1 ? end() : begin() + off + len);
    }
    template <typename T>
    List<T> List<T>::sublist(sli_iter<T> beg, sli_iter<T> end) {
        List<T> tmp;
        while (beg < end) { tmp.add(*beg); ++beg; }
        return tmp;
    }
    template <typename T>
    bool List<T>::empty() const { return !_size; }
    template <typename T>
    size_t List<T>::size() const { return _size; }
    template <typename T>
    size_t List<T>::capacity() const { return _capacity; }
    template <typename T>
	void List<T>::add(T&& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(std::forward<T&&>(val));
		_end->insertPrev(p);
		if (!_size) _begin = _end->prev;
		++_size;
	}
    template <typename T>
	void List<T>::add(const T& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(val);
		_end->insertPrev(p);
		if (!_size) _begin = _end->prev;
		++_size;
	}
	template <typename T>
	void List<T>::put(T&& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(std::forward<T&&>(val));
		_begin->insertPrev(p);
		_begin = p;
		++_size;
	}
	template <typename T>
	void List<T>::put(const T& val) {
		if (_capacity <= _size + 2) _expand(_size + 2);
		auto p = _fill(val);
		_begin->insertPrev(p);
		_begin = p;
		++_size;
	}
	template <typename T>
	void List<T>::append(const T* val, size_t s) {
		if (val && s) {
			sforin(i, 0, s) { add(*val); ++val; }
		}
	}
	template <typename T>
	void List<T>::pile(const T* val, size_t s) {
		if (val && s) {
			sforin(i, 0, s) { put(*val); ++val; }
		}
	}
    template <typename T>
	void List<T>::append(const List<T>& list) {
		if (list._size) {
			sforeach(list) add(E_);
		}
	}
	template <typename T>
	void List<T>::pile(const List<T>& list) {
		if (list._size) {
			sforeach(list) put(E_);
		}
	}
    template <typename T>
	void List<T>::set(sinteger idx, const T& val) { set((idx < 0 ? end() : begin()) + idx, val); }
    template <typename T>
	void List<T>::set(sli_iter<T> iter, const T& val) { 
		iter._ptr->release();
		iter._ptr->init(val);
	}
    template <typename T>
	void List<T>::exchange(sli_iter<T> iter1, sli_iter<T> iter2) { 
		iter1.swap(iter1, iter2);
	}
    template <typename T>
    void List<T>::exchange(size_t idx1, size_t idx2) {
		exchange(begin() + idx1, begin() + idx2);
	}
    template <typename T>
	sli_iter<T> List<T>::insert(sinteger idx, const T& val) { return insert((idx < 0 ? end() : begin()) + idx, val); }
	template <typename T>
	sli_iter<T> List<T>::insert(sli_iter<T> iter, const T& val) {
		if (iter < end()) {
			if (_capacity <= _size + 2) {
				auto idx = _index(iter);
				_expand(_size + 2);
				iter = begin() + idx;
			}
			auto p = iter.ptr();
			auto p_ = _fill(val);
			p->insertPrev(p_);
			if (!(p_->prev)) _begin = p_;
			++_size;
			return sli_iter<T>(p_);
		}
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
    template <typename T>
	sli_iter<T> List<T>::remove(sli_iter<T> beg, sli_iter<T> end) {
		auto p = beg._ptr;
		while (beg < end) {
			auto p_ = beg._ptr;
			++beg;
			_vacate(p_);
			--_size;
		}
        return sli_iter<T>(p->next);
	}
	template <typename T>
	sli_iter<T> remove(const T& val);
    template <typename T>
	sli_iter<T> List<T>::remove(size_t off, size_t len) {
		return remove(begin() + off, len == -1 ? end() : begin() + off + len);
	}
    template <typename T>
	sli_iter<T> List<T>::removeAt(sinteger idx) { return remove((idx < 0 ? end() : begin()) + idx, (idx < 0 ? end() : begin()) + idx + 1); }
    template <typename T>
    void List<T>::trim(size_t s) { 
		if (s <= _size) remove(end() - s, end());
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
	}
    template <typename T>
    void List<T>::cut(size_t s) {
		if (s <= _size) remove(begin(), begin() + s);
		else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
	template <typename T>
	T List<T>::get(sinteger idx) const { return at(idx); }
	template <typename T>
    T List<T>::pop() {
        if (!empty()) { 
			auto val = last();
			trim(1);
			return val;
		}
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
    template <typename T>
    T List<T>::pick() {
        if (!empty()) {
			auto val = first();
			cut(1);
			return val;
		}
        else throw SException(ERR_INFO, SLIB_RANGE_ERROR);
    }
	template <typename T>
    void List<T>::clear() { resize(0); }
    template <typename T>
    void List<T>::reset(const T &val) { sforeach(*this) E_ = val; }
    template <typename T>
    void List<T>::sort(Comparer comp) { std::sort(begin(), end(), comp); }
    template <typename T>
    void List<T>::copy(const T *ptr, size_t s) {
        resize(s);
        sforeach(*this) { E_ = *ptr; ++ptr; }
    }
    template <typename T>
    void List<T>::copyTo(List<T> &list) const {
        if (_size) {
            list.resize(_size);
			sforeach2(list, *this) E1_ = E2_;
        }
    }
    template <typename T>
    void List<T>::moveTo(List<T> &list) {
		list.release();
        list._capacity = _capacity;
        list._size = _size;
        list._begin = _begin;
        list._end = _end;
        list._vacant = _vacant;
        discard();
    }
    template <typename T>
    void List<T>::swap(List<T> &list) {
        auto tmpc = _capacity; _capacity = list._capacity; list._capacity = tmpc;
        auto tmps = _size; _size = list._size; list._size = tmps;
        auto tmpb = _begin; _begin = list._begin; list._begin = tmpb ;
        auto tmpe = _end; _end = list._end; list._end = tmpe;
        auto tmpv = _vacant; _vacant = list._vacant; list._vacant = tmpv;
    }
    template <typename T>
    bool List<T>::contain(const T &val) const { return find(val) != NOT_FOUND; }
	template <typename T>
	sli_iter<T> List<T>::locate(const T& val, size_t off) {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it; else NEXT_; }
		return end();
	}
	template <typename T>
	sli_citer<T> List<T>::locate(const T& val, size_t off) const {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it; else NEXT_; }
		return end();
	}
	template <typename T>
    size_t List<T>::find(const T &val, size_t off) const {
		auto it = begin() + off;
		while (it < end()) { if (E_ == val) return it - begin(); else NEXT_; }
        return NOT_FOUND;
    }
    template <typename T>
    size_t List<T>::rfind(const T &val, size_t off) const {
		auto it = end() - off - 1;
		while (begin() <= it) { if (E_ == val) return it - begin(); else PREV_; }
        return NOT_FOUND;
    }
    template <typename T>
    void List<T>::resize(size_t s) {
		if (s) {
			if (!_capacity) reserve(s + 2);
			else if (_capacity + 1 <= s) _expand(s);
		}
        auto s_ = _size;
        if (s < s_) {
			auto it = begin() + s;
			auto p = it._ptr;
			_end->prev = p->prev;
			if (p->prev) p->prev->next = _end;
			else _begin = _end;
			while (p < _end) {
				auto p_ = p;
				p = p->next;
				_vacate(p_);
				--_size;
			}
        }
		else if (s_ < s) {
			sforin(i, s_, s) {
				auto p = _fill();
				_end->insertPrev(p);
				if (!_begin->next) _begin = p;
				++_size;
			}
		}
    }
    template <typename T>
    void List<T>::resize(size_t s, const T &val) {
        auto s_ = _size;
        resize(s);
        if (s_ < s) {
			auto it = begin() + s_;
            while (it < end()) { E_ = val; NEXT_; }
        }
    }
    template <typename T>
    void List<T>::reserve(size_t s) {
        if (_capacity < s) {
            if (_capacity) {
				List<T> tmp;
				tmp.reserve(s);
				if (_size) tmp.append(*this);
				tmp.swap(*this);
			}
            else {
				_vacant = Memory<list_data<T>>::alloc(s);
				_vacant->prev = nullptr;
				auto p = _vacant;
				sforin(i, 0, s - 1) { p->next = p + 1; p = p->next; }
				_end = p;
				_end->prev = nullptr;
				_end->next = nullptr;
				_begin = _end;
			}
            _capacity = s;
        }
    }
    template <typename T>
    void List<T>::release() {
		if (_size) { 
			auto p = _begin;
			while (p->next && p < _end) { p->release(); p = p->next; }
		}
		if (_capacity) Memory<list_data<T>>::dealloc(_end - _capacity + 1);
		discard();
    }
    template <typename T>
    void List<T>::discard() {
        _capacity = 0; _size = 0; _begin = nullptr; _end = nullptr; _vacant = nullptr;
    }
    template <typename T>
    bool List<T>::operator < (const List &list) const {
		if (size() != list.size()) return size() < list.size();
		sforeach2(*this, list) {
			if (!(E1_ == E2_)) return E1_ < E2_;
		}
		return false;
	}
    template <typename T>
	bool List<T>::operator == (const List& list) const {
		if (size() != list.size()) return false;
		sforeach2(*this, list) {
			if (!(E1_ == E2_)) return false;
		}
		return true;
	}
	*/
}
#endif