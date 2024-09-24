#ifndef SLIB_NODE_H
#define SLIB_NODE_H
#include "sbasic/parray.h"
namespace slib {
	template<class Cls>
	class Node {
	protected:
		Node<Cls>* _parent;
		Array<Cls*> _children;
	public:
		Node() : _parent(nullptr) {}
		Node(const Cls &cls) {
			const Node<Cls> *node = dynamic_cast<const Node<Cls>*>(&cls);
			_parent = node->_parent;
			_children = node->_children;
		}
		virtual ~Node() {}
		Node<Cls>& operator=(Node<Cls>&& node) noexcept { swap(node); return *this; }
		Node<Cls>& operator=(const Node<Cls>& node) {
			_parent = node._parent;
			_children = node._children;
			return *this;
		}
		bool isRoot() const { return !_parent; }
		bool isLeaf() const { return _children.empty(); }
		size_t count() const { return _children.size(); }
		const Array<Cls*>& children() const { return _children; }
		Cls* root() {
			auto p = this;
			while (p->_parent) { p = p->_parent; }
			return dynamic_cast<Cls*>(p);
		}
		const Cls* root() const {
			auto p = this;
			while (p->_parent) { p = p->_parent; }
			return dynamic_cast<const Cls*>(p);
		}
		Cls *parent() {
			if (_parent) return dynamic_cast<Cls*>(_parent);
			else return nullptr;
		}
		const Cls *parent() const {
			if (_parent) return dynamic_cast<const Cls*>(_parent);
			else return nullptr;
		}
		void setParent(Cls* p) { _parent = dynamic_cast<Node<Cls> *>(p); }
		size_t index() const {
			if (!_parent) return NOT_FOUND;
			else {
				sfor(_parent->_children) {
					if ($_.ptr() == this) return $INDEX(_parent->_children);
				}
				return NOT_FOUND;
			}
		}
		size_t layer() const {
			size_t l = 0;
			auto p = _parent;
			while (p) { p = p->_parent; ++l; }
			return l;
		}
		Pair<size_t, size_t> address() const { return Pair<size_t, size_t>(layer(), index()); }
		Cls *child(const int i) { return dynamic_cast<Cls *>(_children[i].ptr()); }
		const Cls* child(const int i) const { return dynamic_cast<const Cls*>(_children[i].ptr()); }
		Cls* operator[](const int i) { return dynamic_cast<Cls*>(_children[i].ptr()); }
		const Cls* operator[](const int i) const { return dynamic_cast<const Cls*>(_children[i].ptr()); }

		ArrayIterator<Cls*> begin() { return _children.begin(); }
		ArrayCIterator<Cls*> begin() const { return _children.begin(); }
		ArrayIterator<Cls*> end() { return _children.end(); }
		ArrayCIterator<Cls*> end() const { return _children.end(); }

		void addChild(Cls* node) {
			_children.add(_child(node));
			_children[-1]->setParent(dynamic_cast<Cls *>(this));
		}
		void insertChild(const int i, const Cls* node) {
			_children.insert(i, _child(node));
			_children[i]->setParent(*dynamic_cast<Cls*>(this));
		}
		void insertChildBefore(const Cls* node) {
			auto idx = dynamic_cast<const Node<Cls>*>(&node)->index();
			insertChild(idx, node);
		}
		void insertChildAfter(const Cls* node) {
			auto idx = dynamic_cast<const Node<Cls>*>(&node)->index();
			insertChild(idx + 1, node);
		}
		void removeChildAt(const int i) { _children.removeAt(i); }
		void removeChild(const size_t off, const size_t len = -1) { _children.remove(off, len); }
		void removeChild(const Range<int>& range) { _children.remove(range); }
		void eraseChild(const Cls *node) { 
			auto idx = indexOf(node);
			if (idx == NOT_FOUND) throw NotFoundException(nofoundErrorText("child", "this node"));
			else _children.removeAt(idx); 
		}
		void reloc(Cls *parent, const int idx) {
			if (isRoot()) throw UndefinedException(undefErrorText("node relocation", "root node"));
			auto cidx = index();
			SPointer<Node<Cls>> &self = _parent->_children[cidx];
			dynamic_cast<Node<Cls>*>(parent)->_children.add(self);
			_parent->removeChildAt(cidx);
			self->setParent(parent);
		}
		virtual void clear() { return _children.clear(); }
		virtual void swap(Node<Cls>& node) {
			Memory<Node<Cls>*>::swap(&_parent, &node._parent);
			_children.swap(node._children);
		}
		bool hasElement(const Cls* node, bool recur = false) const {
			auto nf = indexOf(node) != NOT_FOUND;
			if (recur) {
				sfor(_children) {
					auto b = $_->hasElement(node, recur);
					if (b) return true;
				}
				return false;
			}
			else return nf;
		}
		size_t indexOf(const Cls* node, const size_t off = 0) {
			sforin(it, _children.begin() + off, _children.end()) {
				if ($_.ptr() == dynamic_cast<Node<Cls>*>(node)) return $INDEX(_children);
			}
			return NOT_FOUND;
		}
		operator Cls*() { return dynamic_cast<Cls*>(this); }
		operator const Cls*() const { return dynamic_cast<const Cls*>(this); }
	};

	template<class Cls>
	class SNode {
	public:
		SNode<Cls>* _parent;
		PArray<Cls> _children;
	public:
		SNode() : _parent(nullptr) {}
		SNode(const Cls& cls) {
			const SNode<Cls>* node = dynamic_cast<const SNode<Cls>*>(&cls);
			_parent = node->_parent;
			_children = node->_children;
			sfor(_children) $_.setParent(this);
		}
		virtual ~SNode() {}
		SNode<Cls>& operator=(SNode<Cls>&& node) noexcept { swap(node); return *this; }
		SNode<Cls>& operator=(const SNode<Cls>& node) {
			_parent = node._parent;
			_children = node._children;
			return *this;
		}
		bool isRoot() const { return !_parent; }
		bool isLeaf() const { return _children.empty(); }
		size_t count() const { return _children.size(); }
		const PArray<Cls>& children() const { return _children; }
		Cls& root() {
			auto p = this;
			while (p->_parent) { p = p->_parent; }
			return *dynamic_cast<Cls*>(p);
		}
		const Cls& root() const {
			auto p = this;
			while (p->_parent) { p = p->_parent; }
			return *dynamic_cast<const Cls*>(p);
		}
		Cls& parent() {
			if (_parent) return *dynamic_cast<Cls*>(_parent);
			else throw NullException(nullErrorText("Parent node"));
		}
		const Cls& parent() const {
			if (_parent) return *dynamic_cast<const Cls*>(_parent);
			else throw NullException(nullErrorText("Parent node"));
		}
		void setParent(SNode<Cls>* p) { _parent = p; }
		size_t index() const {
			if (!_parent) return NOT_FOUND;
			else {
				auto it = _parent->_children.begin();
				sfor(_parent->_children) {
					if (&$_ == dynamic_cast<const Cls*>(this)) return $INDEX(_parent->_children);
				}
				return NOT_FOUND;
			}
		}
		size_t layer() const {
			size_t l = 0;
			auto p = _parent;
			while (p) { p = p->_parent; ++l; }
			return l;
		}
		Pair<size_t, size_t> address() const { return Pair<size_t, size_t>(layer(), index()); }
		Cls& child(const int i) { return _children[i]; }
		const Cls& child(const int i) const { return _children[i]; }
		Cls& operator[](const int i) { return _children[i]; }
		const Cls& operator[](const int i) const { return _children[i]; }
		PArrayIterator<Cls> begin() { return _children.begin(); }
		PArrayCIterator<Cls> begin() const { return _children.begin(); }
		PArrayIterator<Cls> end() { return _children.end(); }
		PArrayCIterator<Cls> end() const { return _children.end(); }
		
		void addChild(SPointer<Cls> node) {
			dynamic_cast<SNode<Cls>*>(node.ptr())->setParent(this);
			_children.add(node);
		}
		void insertChild(const int i, SPointer<Cls> node) {
			_children.insert(i, node);
			dynamic_cast<SNode<Cls>*>(&_children[i])->setParent(this);
		}
		void removeChildAt(const int i) { _children.removeAt(i); }
		void removeChild(const size_t off, const size_t len = -1) { _children.remove(off, len); }
		void removeChild(const Range<int>& range) { _children.remove(range); }
		void eraseChild(const Cls& node) {
			auto idx = indexOf(node);
			if (idx == NOT_FOUND) throw NotFoundException(nofoundErrorText("child", "this node"));
			else _children.removeAt(idx);
		}
		void reloc(Cls* parent) {
			if (isRoot()) throw UndefinedException(undefErrorText("node relocation", "root node"));
			SPointer<Cls> self = *_parent->_children.iterAt(index()).ptr();
			_parent->removeChildAt(index());
			dynamic_cast<SNode<Cls>*>(parent)->addChild(self);
		}
		virtual void clear() { _children.clear(); }
		virtual void swap(SNode<Cls>& node) {
			Memory<SNode<Cls>*>::swap(&_parent, &node._parent);
			_children.swap(node._children);
		}
		bool hasElement(const Cls& node, bool recur = false) const {
			auto nf = indexOf(node) != NOT_FOUND;
			if (recur) {
				sfor(_children) {
					auto b = dynamic_cast<const SNode<Cls>*>(&$_)->hasElement(node, recur);
					if (b) return true;
				}
				return false;
			}
			else return nf;
		}
		size_t indexOf(const Cls& node, const size_t off = 0) const {
			sforin(it, _children.begin() + off, _children.end()) {
				if (&$_ == &node) return $INDEX(_children);
			}
			return NOT_FOUND;
		}
		operator Cls& () { return *dynamic_cast<Cls*>(this); }
		operator const Cls& () const { return *dynamic_cast<const Cls*>(this); }
	};
}
template<class Elem>
std::ostream& operator<<(std::ostream& os, const slib::Node<Elem>& node) {
	return os << node.toString();
}
#endif