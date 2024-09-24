#ifndef SLIB_TREEITER_H
#define SLIB_TREEITER_H
#include "sbasic/map.h"
namespace slib {
	template<class Key, class Val>
	class Tree;
	template<class Key, class Val>
    class TreeData {
		friend Tree<Key, Val>;
		friend TreeIterator<Key, Val>;
		friend TreeCIterator<Key, Val>;
	protected:
		bool _flag;
		Pair<Key, Val> _data;
		TreeData *_parent, *_child[2];
	public:
		TreeData() : flag(false), parent(nullptr) { memset(child, 0, sizeof(TreeData*) * 2); }
		TreeData(TreeData *p, const Key& k, const Val& v) : flag(true), parent(p), data(k, v) { memset(child, 0, sizeof(TreeData*) * 2); }
		TreeData(const TreeData& tr) : flag(tr.flag), parent(tr.parent), data(tr.data) { 
			memcpy(child, tr.child[0], sizeof(TreeData*) * 2);
		}
		~TreeData() { 
			if (child[0]) delete child[0]; 
			if (child[1]) delete child[1]; 
		}
		TreeData& operator=(const TreeData& tr) {
			flag = tr.flag;
			parent = tr.parent;
			data = tr.data;
			memcpy(child, tr.child, sizeof(TreeData*) * 2);
			return *this;
		}
		TreeData* insert(TreeData* node, bool left) {
			child[left ? 0 : 1] = node;
			return node;
		}
		TreeData* sibling() {
			if (!parent) return nullptr;
			return isLeft() ? parent->child[1] : parent->child[0];
		}
		void changeColor() { flag = !flag; }
		void setColor(bool b) { flag = b; }
		void lrot() {
			auto p = parent, rc = child[1];
			if (isLeft()) p->child[0] = rc;
			else p->child[1] = rc;
			child[1] = rc->child[0];
			rc->child[0] = this;
		}
		void rrot() {
			auto p = parent, lc = child[0];
			if (isLeft()) p->child[0] = lc;
			else p->child[1] = lc;
			child[0] = lc->child[1];
			lc->child[1] = this;
		}
		bool isRoot() { return !parent; }
		bool isLeaf() { return (!child[0]) && (!child[1]); }
		bool isLeft() {
			if (parent) { return parent->child[0] == this; }
			//throw SException(ERR_INFO, 0);
		}
		bool isRight() {
			if (parent) { return parent->child[1] == this; }
			//throw SException(ERR_INFO, 0);
		}
		bool isRed() { return flag; }
		bool isBlack() { return !flag; }
		Key& key() { return _data.first; }
		const Key& key() const { return _data.first; }
		Val& value() { return _data.second; }
		const Val& value() const { return _data.second; }
		bool operator < (const Key& k) const { return data.key < k; }
		bool operator == (const Key& k) const { return data.key == k; }
    };
	template<class Key, class Val>
	class TreeIterator {
		friend class Tree<Key, Val>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef Pair<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef Pair<Key, Val>* pointer;
        typedef Pair<Key, Val>& reference;
        
    private:
		TreeData<Key, Val> *_ptr;
        
    public:
		TreeIterator(TreeData<Key, Val> *p) : _ptr(p) {}
		TreeIterator(const TreeIterator& it) : _ptr(it._ptr) {}
		~TreeIterator() {}
		TreeIterator& operator=(const TreeIterator& it) {
			_ptr = it._ptr; return *this;
		}
		reference operator*() { return _ptr->data; }
		pointer operator->() { return &_ptr->data; }
        reference operator[](std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return _ptr->data; }
		TreeIterator& operator++() {
			if (_ptr->child[1]) {
				_ptr = _ptr->child[1];
				while (_ptr->child[0]) { _ptr = _ptr->child[0]; }
			}
			else {
				if (_ptr->parent) {
					if (_ptr->isLeft()) _ptr = _ptr->parent;
					else {
						while (_ptr->parent && _ptr->isRight()) { _ptr = _ptr->parent; }
						if (_ptr->isRoot()) _ptr = nullptr;
						else {
							_ptr = _ptr->parent->child[1];
							if (_ptr && _ptr->child[0]) {
								while (_ptr->child[0]) { _ptr = _ptr->child[0]; }
							}
						}
					}
				}
				else _ptr = nullptr;
			}
			return *this;
		}
		TreeIterator operator++(int) { return TreeIterator(*this) + 1; }
		TreeIterator& operator--() {
			if (_ptr->parent) {
				if (_ptr->child[0]) {
					_ptr = _ptr->child[0];
					while (_ptr->child[1]) { _ptr = _ptr->child[1]; }
				}
				else {
					if (_ptr->isRight()) _ptr = _ptr->parent;
					else {
						while (_ptr->parent && _ptr->isLeft()) { _ptr = _ptr->parent; }
						if (_ptr->isRoot()) _ptr = nullptr;
						else {
							_ptr = _ptr->parent->child[0];
							if (_ptr && _ptr->child[1]) {
								while (_ptr->child[1]) { _ptr = _ptr->child[0]; }
							}
						}
					}
				}
			}
			else {
				if (_ptr->child[0]) {
					_ptr = _ptr->child[0];
					while (_ptr->child[1]) { _ptr = _ptr->child[1]; }
				}
				else _ptr = nullptr;
			}
			return *this;
		}
		TreeIterator operator--(int) { return TreeIterator(*this) - 1; }
		TreeIterator&operator+=(std::ptrdiff_t diff) { sforin(i, 0, diff) ++(*this); return *this; }
		TreeIterator&operator-=(std::ptrdiff_t diff) { sforin(i, 0, diff) --(*this); return *this; }
		TreeIterator operator +(std::ptrdiff_t diff) { auto it = TreeIterator(*this); sforin(i, 0, diff) ++it; return it; }
		TreeIterator operator -(std::ptrdiff_t diff) { auto it = TreeIterator(*this); sforin(i, 0, diff) --it; return it; }
		sinteger operator-(const TreeIterator it) const {
			sinteger i = 0;
			auto cur = TreeIterator(*this);
			while (cur != it) { ++i; ++cur; }
			return i;
		}
		TreeData<Key, Val>* ptr() { return _ptr; }
        void swap(TreeIterator it1, TreeIterator it2) { auto temp = it1._ptr; it1._ptr = it2._ptr; it2._ptr = temp; }
        bool operator<(const TreeIterator&it) const { return _ptr < it._ptr; }
        bool operator==(const TreeIterator&it) const { return _ptr == it._ptr; }
        bool operator!=(const TreeIterator&it) const { return _ptr != it._ptr; }
    };
	template<class Key, class Val>
	class TreeCIterator {
		friend class Tree<Key, Val>;
    public:
        typedef std::random_access_iterator_tag iterator_category;
        typedef const KVPair<Key, Val> value_type;
        typedef std::ptrdiff_t difference_type;
        typedef const KVPair<Key, Val>* pointer;
        typedef const KVPair<Key, Val>& reference;
        
    private:
		TreeData<Key, Val> *_ptr;
        
    public:
		TreeCIterator(TreeData<Key, Val> *p);
		TreeCIterator(const TreeCIterator&it);
		~TreeCIterator();
		TreeCIterator& operator=(const TreeCIterator&it);
        reference operator*();
        pointer operator->();
        reference operator[](std::ptrdiff_t diff);
		TreeCIterator&operator++();
		TreeCIterator operator++(int);
		TreeCIterator&operator--();
		TreeCIterator operator--(int);
		TreeCIterator&operator+=(std::ptrdiff_t diff);
		TreeCIterator&operator-=(std::ptrdiff_t diff);
		TreeCIterator operator +(std::ptrdiff_t diff);
		TreeCIterator operator -(std::ptrdiff_t diff);
		sinteger operator-(const TreeCIterator it) const;
		const TreeData<Key, Val>* ptr();
        void swap(TreeCIterator it1, TreeCIterator it2);
        bool operator<(const TreeCIterator&it) const;
		bool operator<=(const TreeCIterator& it) const;
		bool operator>(const TreeCIterator& it) const;
		bool operator>=(const TreeCIterator& it) const;
        bool operator==(const TreeCIterator&it) const;
        bool operator!=(const TreeCIterator&it) const;

		template<class Key, class Val>
		using str_iter = TreeIterator<Key, Val>;
		template<class Key, class Val>
		using str_citer = TreeCIterator<Key, Val>;

    };
    /*============================================================*/
	
}
#endif
