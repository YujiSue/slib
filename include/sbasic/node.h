#ifndef SLIB_NODE_H
#define SLIB_NODE_H

#include "sbasic/array.h"
#include "sbasic/list.h"

namespace slib {
	template<class Elem, typename Child = Elem>
	class Node {
	protected:
		Elem* _parent;
		List<Child> _children;

	public:
		Node();
		Node(const Node<Elem, Child>& node);
		virtual ~Node();
		Node<Elem, Child>& operator=(const Node<Elem, Child>& node);
		bool isRoot() const;
		bool isLeaf() const;
		size_t childCount() const;
		Elem& parent();
		const Elem& parent() const;
		Elem& root();
		const Elem& root() const;
		List<Child>& children();
		const List<Child>& children() const;
		Elem& child(int idx);
		const Elem& child(int idx) const;
		Elem& operator[](int idx);
		const Elem& operator[](int idx) const;
		sli_iter<Child> begin();
		sli_citer<Child> begin() const;
		sli_iter<Child> end();
		sli_citer<Child> end() const;
		size_t index() const;
		size_t layer() const;
		intarray address() const;
		void setParent(Elem* p);
		void addChild(const Child& node);
		sli_iter<Child> insertChild(size_t idx, const Child& node);
		sli_iter<Child> insertChild(sli_iter<Child> iter, const Child& node);
		void setChild(size_t idx, const Child& node);
		void set(sli_iter<Child> iter, const Child& node);
		sli_iter<Child> removeChildAt(size_t idx);
		sli_iter<Child> removeChild(sli_iter<Child> iter);
		sli_iter<Child> removeChild(const Child& node);
		void moveChildTo(sli_iter<Child> iter);
		void clearChildren();
	};

	template<class Elem, typename Child>
	Node<Elem, Child>::Node() : _parent(nullptr) {}
	template<class Elem, typename Child>
	Node<Elem, Child>::Node(const Node<Elem, Child>& node) : SObject() {
		_parent = node._parent;
		_children = node._children;
	}
	template<class Elem, typename Child>
	Node<Elem, Child>::~Node() {}
	template<class Elem, typename Child>
	Node<Elem, Child>& Node<Elem, Child>::operator=(const Node<Elem, Child>& node) {
		_parent = node._parent;
		_children = node._children;
		return *this;
	}
	template<class Elem, typename Child>
	bool Node<Elem, Child>::isRoot() const { return _parent == nullptr; }
	template<class Elem, typename Child>
	bool Node<Elem, Child>::isLeaf() const { return _children.empty(); }
	template<class Elem, typename Child>
	size_t Node<Elem, Child>::childCount() const { return _children.size(); }
	template<class Elem, typename Child>
	Elem& Node<Elem, Child>::parent() { return *_parent; }
	template<class Elem, typename Child>
	const Elem& Node<Elem, Child>::parent() const { return *_parent; }
	template<class Elem, typename Child>
	Elem& Node<Elem, Child>::root() {
		Elem* ptr = const_cast<Elem*>(dynamic_cast<const Elem*>(this));
		if (_parent == nullptr) return ptr;
		do { ptr = ptr->_parent; } while (ptr->_parent != nullptr);
		return *ptr;
	}
	template<class Elem, typename Child>
	const Elem& Node<Elem, Child>::root() const {
		Elem* ptr = const_cast<Elem*>(dynamic_cast<const Elem*>(this));
		if (_parent == nullptr) return ptr;
		do { ptr = ptr->_parent; } while (ptr->_parent != nullptr);
		return *ptr;
	}
	template<class Elem, typename Child>
	List<Child>& Node<Elem, Child>::children() { return _children; }
	template<class Elem, typename Child>
	const List<Child>& Node<Elem, Child>::children() const { return _children; }
	template<class Elem, typename Child>
	Elem& Node<Elem, Child>::child(int idx) { return _children[idx]; }
	template<class Elem, typename Child>
	const Elem& Node<Elem, Child>::child(int idx) const { return _children[idx]; }
	template<class Elem, typename Child>
	Elem& Node<Elem, Child>::operator[](int idx) { return _children[idx]; }
	template<class Elem, typename Child>
	const Elem& Node<Elem, Child>::operator[](int idx) const { return _children[idx]; }
	template<class Elem, typename Child>
	sli_iter<Child> Node<Elem, Child>::begin() { return _children.begin(); }
	template<class Elem, typename Child>
	sli_citer<Child> Node<Elem, Child>::begin() const { return _children.begin(); }
	template<class Elem, typename Child>
	sli_iter<Child> Node<Elem, Child>::end() { return _children.end(); }
	template<class Elem, typename Child>
	sli_citer<Child> Node<Elem, Child>::end() const { return _children.end(); }
	template<class Elem, typename Child>
	size_t Node<Elem, Child>::index() const {
		if (!_parent) return 0;
		size_t i = 0;
		sforeach(_parent->_children) {
			if (E_.ptr() == (SObject*)this) return i;
			++i;
		}
		return NOT_FOUND;
	}
	template<class Elem, typename Child>
	size_t Node<Elem, Child>::layer() const {
		size_t l = 0;
		auto ptr = _parent;
		while (ptr != nullptr) {
			++l;
			ptr = ptr->_parent;
		}
		return l;
	}
	template<class Elem, typename Child>
	intarray Node<Elem, Child>::address() const {
		intarray addr(layer());
		auto current = this;
		srforeach(addr) {
			E_ = current->index();
			current = current->_parent;
		}
		return addr;
	}
	template<class Elem, typename Child>
	void Node<Elem, Child>::setParent(Elem* p) { _parent = p; }
	template<class Elem, typename Child>
	void Node<Elem, Child>::addChild(const Child& node) {
		_children.add(node);
		(_children.last()).setParent(dynamic_cast<Elem*>(this));
	}
	template<class Elem, typename Child>
	sli_iter<Child> Node<Elem, Child>::insertChild(size_t idx, const Child& node) {
		auto it = _children.insert(idx, node);
		E_->setParent(dynamic_cast<Cls*>(this));
		return it;
	}
	template<class Elem, typename Child>
	sli_iter<Child> Node<Elem, Child>::insertChild(sli_iter<Child> iter, const Child& node) {
		auto it = _children.insert(iter, node);
		E_->setParent(dynamic_cast<Cls*>(this));
		return it;
	}
	template<class Elem, typename Child>
	void setChild(size_t idx, const Child& node) {
		auto it = _children.begin() + idx;
		E_ = node;
		E_->setParent(dynamic_cast<Cls*>(this));
	}
	template<class Elem, typename Child>
	void set(sli_iter<Child> iter, const Child& node) {
		*iter = node;
		(*iter)->setParent(dynamic_cast<Cls*>(this));
	}
	template<class Elem, typename Child>
	sli_iter<Child> removeChildAt(size_t idx) {
		return _children.removeAt(idx);
	}
	template<class Elem, typename Child>
	sli_iter<Child> removeChild(sli_iter<Child> iter) {
		return _children.remove(iter, iter + 1);
	}
	template<class Elem, typename Child>
	sli_iter<Child> removeChild(const Child& node) {
		sforeach(_children) { if (&E_ == &node) return _children.remove(it, it + 1); }
		return end();
	}
	template<class Elem, typename Child>
	void moveChildTo(sli_iter<Child> it) {
		if (isRoot() || (*iter)->isRoot()) throw SException(ERR_INFO, SLIB_EXEC_ERROR);
		auto node = _parent->begin() + index();
		(*iter)->parent()->insertChild(iter, *node);
		_parent->removeChild(node);
	}
	template<class Elem, typename Child>
	void Node<Elem, Child>::clearChildren() { _children.clear(); }
}

#endif
