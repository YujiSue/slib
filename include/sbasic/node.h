#ifndef SLIB_NODE_H
#define SLIB_NODE_H

#include "sbasic/ptr.h"
#include "sbasic/array.h"

namespace slib {

    template<typename T, class Ptr=sptr<T>, class Container=Array<Ptr>>
    class Node {
    protected:
        T *_parent;
        Container _children;
        
    public:
        Node();
        Node(const Node &node);
        virtual ~Node();
        
        Node &operator=(const Node &node);
        
        T *parent();
		const T* parent() const;
		T* root();
        const T *root() const;
        size_t count() const;
        Container &children();
        const Container &children() const;
        bool isRoot() const;
        size_t index() const;
        size_t layer() const;
        intarray address() const;
        bool hasChild() const;
        
		Ptr &at(int idx);
		const Ptr &at(int idx) const;
		Ptr &operator[](int idx);
		const Ptr &operator[](int idx) const;
        auto begin() -> decltype(_children.begin());
        auto begin() const -> decltype(_children.begin());
        auto end() -> decltype(_children.begin());
        auto end() const -> decltype(_children.begin());
        
        void add(T *n);
        void add(Ptr n);
        void insert(size_t idx, T *n);
        void insert(size_t idx, Ptr n);
        void set(size_t idx, T *n);
        void set(size_t idx, Ptr n);
        void removeAt(size_t idx);
        void remove(Ptr n);
        void transfer(size_t idx1, size_t idx2);
        void transfer(size_t idx1, T *to, size_t idx2);
        void clear();
    };
    
    /*============================================================*/
    
    template<typename T, class Ptr, class Container>
    Node<T, Ptr, Container>::Node() : _parent(), _children() {}
    template<typename T, class Ptr, class Container>
    Node<T, Ptr, Container>::Node(const Node &node) : _parent(node._parent), _children(node._children) {}
    template<typename T, class Ptr, class Container>
    Node<T, Ptr, Container>::~Node() {}
    template<typename T, class Ptr, class Container>
    Node<T, Ptr, Container> &Node<T, Ptr, Container>::operator=(const Node<T, Ptr, Container> &node) {
        _parent = node._parent; _children = node._children; return *this;
    }
    
    template<typename T, class Ptr, class Container>
	T* Node<T, Ptr, Container>::parent() { return _parent; }
	template<typename T, class Ptr, class Container>
	const T* Node<T, Ptr, Container>::parent() const { return _parent; }
    template<typename T, class Ptr, class Container>
    T *Node<T, Ptr, Container>::root() {
		if (!_parent) return dynamic_cast<T *>(this);
        T *node = _parent;
        while (node->_parent != nullptr) node = node->_parent;
        return node;
    }
	template<typename T, class Ptr, class Container>
	const T* Node<T, Ptr, Container>::root() const {
		if (!_parent) return dynamic_cast<const T*>(this);
		T* node = _parent;
		while (node->_parent != nullptr) node = node->_parent;
		return node;
	}
    template<typename T, class Ptr, class Container>
    size_t Node<T, Ptr, Container>::count() const { return _children.size(); }
    template<typename T, class Ptr, class Container>
    Container &Node<T, Ptr, Container>::children() { return _children; }
    template<typename T, class Ptr, class Container>
    const Container &Node<T, Ptr, Container>::children() const { return _children; }
    template<typename T, class Ptr, class Container>
    bool Node<T, Ptr, Container>::isRoot() const { return !_parent; }
    template<typename T, class Ptr, class Container>
    size_t Node<T, Ptr, Container>::index() const {
        if (isRoot()) return 0;
        sforeach(_parent->_children) { if ((void *)it._ptr == (void *)this) return INDEX_(_parent->_children); }
        return NOT_FOUND;
    }
    template<typename T, class Ptr, class Container>
    size_t Node<T, Ptr, Container>::layer() const {
        size_t l = 0;
        T *node = _parent;
        while (node != nullptr) { node = node->_parent; ++l; }
        return l;
    }
    template<typename T, class Ptr, class Container>
    intarray Node<T, Ptr, Container>::address() const {
        intarray ia;
        const T *node = dynamic_cast<const T *>(this);
        while (node->_parent != nullptr) {
            sforeach(node->_parent->_children) {
                if (E_.ptr() == node) ia.add(INDEX_(node->_parent->_children));
            }
            node = node->_parent;
        }
        auto beg = ia.begin(), end = ia.end()-1;
        while (beg < end) {
            auto tmp = *beg;
            *beg = *end;
            *end = tmp;
        }
        return ia;
    }
    template<typename T, class Ptr, class Container>
    bool Node<T, Ptr, Container>::hasChild() const { return _children.size(); }
    
    template<typename T, class Ptr, class Container>
	Ptr&Node<T, Ptr, Container>::at(int idx) { return _children[idx]; }
	template<typename T, class Ptr, class Container>
	const Ptr& Node<T, Ptr, Container>::at(int idx) const { return _children[idx]; }
    template<typename T, class Ptr, class Container>
	Ptr& Node<T, Ptr, Container>::operator[](int idx) { return _children[idx]; }
	template<typename T, class Ptr, class Container>
	const Ptr& Node<T, Ptr, Container>::operator[](int idx) const { return _children[idx]; }
    template<typename T, class Ptr, class Container>
    auto Node<T, Ptr, Container>::begin() ->decltype(_children.begin()) { return _children.begin(); }
    template<typename T, class Ptr, class Container>
    auto Node<T, Ptr, Container>::begin() const ->decltype(_children.begin()) { return _children.begin(); }
    template<typename T, class Ptr, class Container>
    auto Node<T, Ptr, Container>::end() ->decltype(_children.begin()) { return _children.end(); }
    template<typename T, class Ptr, class Container>
    auto Node<T, Ptr, Container>::end() const ->decltype(_children.begin()) { return _children.end(); }
    
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::add(T *n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.add(Ptr(n));
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::add(Ptr n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.add(n);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::insert(size_t idx, T *n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.insert(idx, Ptr(n));
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::insert(size_t idx, Ptr n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.insert(idx, n);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::set(size_t idx, T *n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.set(idx, Ptr(n));
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::set(size_t idx, Ptr n) {
        n->_parent = dynamic_cast<T *>(this);
        _children.set(idx, n);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::removeAt(size_t idx) {
        _children.removeAt(idx);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::remove(Ptr n) {
        _children.removeAt(n->index());
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::transfer(size_t idx1, size_t idx2) {
        Ptr n = _children[idx1];
        _children.removeAt(idx1);
        _children.insert(idx2, n);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::transfer(size_t idx1, T *to, size_t idx2) {
        Ptr n = _children[idx1];
        _children.removeAt(idx1);
        to->insert(idx2, n);
    }
    template<typename T, class Ptr, class Container>
    void Node<T, Ptr, Container>::clear() { _children.clear(); }
    
}

#endif
