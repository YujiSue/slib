#ifndef SLIB_SNODE_H
#define SLIB_SNODE_H

#include "sobj/sobjptr.h"

namespace slib {
	template<class Cls>
	class SNode : public SObject {
	protected:
		Cls* _parent;
		SArray _children;

	public:
		SNode() : SObject(), _parent(nullptr) {}
		SNode(const SNode<Cls>& node) : SObject() {
			_parent = node._parent;
			_children = node._children;
		}
		virtual ~SNode() {}
		/*
		SNode<Cls>& operator=(const SNode<Cls>& node) {
			_parent = node._parent;
			_children = node._children;
			return *this;
		}
		bool isRoot() const { return _parent == nullptr; }
		bool isLeaf() const { return _children.empty(); }
		size_t childCount() const { return _children.size(); }
		Cls* parent() const { return _parent; }
		Cls* root() const {
			Cls* ptr = const_cast<Cls *>(dynamic_cast<const Cls *>(this));
			if (_parent == nullptr) return ptr;
			do { ptr = ptr->_parent; }
			while (ptr->_parent != nullptr);
			return ptr;
		}
		List<Cls>& children() { return _children; }
		const List<SClsPtr<Cls, T>>& children() const { return _children; }
		SClsPtr<Cls, T>& child(int idx) { return _children[idx]; }
		const SClsPtr<Cls, T>& child(int idx) const { return _children[idx]; }
		SClsPtr<Cls, T>& operator[](int idx) { return _children[idx]; }
		const SClsPtr<Cls, T>& operator[](int idx) const { return _children[idx]; }
		sli_iter<SClsPtr<Cls, T>> begin() { return _children.begin(); }
		sli_citer<SClsPtr<Cls, T>> begin() const { return _children.begin(); }
		sli_iter<SClsPtr<Cls, T>> end() { return _children.end(); }
		sli_citer<SClsPtr<Cls, T>> end() const { return _children.end(); }
		size_t index() const {
			if (!_parent) return 0;
			size_t i = 0;
			sforeach(_parent->_children) {
				if (E_.ptr() == (SObject *)this) return i;
				++i;
			}
			return NOT_FOUND;
		}
		size_t layer() const {
			size_t l = 0;
			auto ptr = _parent;
			while (ptr != nullptr) {
				++l;
				ptr = ptr->_parent;
			}
			return l;
		}
		intarray address() const {
			intarray addr(layer());
			auto current = this;
			srforeach(addr) {
				E_ = current->index();
				current = current->_parent;
			}
			return addr;
		}
		void setParent(Cls* p) { _parent = p; }
		void addChild(const SClsPtr<Cls, T>& node) {
			_children.add(node);
			_children.last()->setParent(dynamic_cast<Cls *>(this));
		}
		sli_iter<SClsPtr<Cls, T>> insertChild(size_t idx, const SClsPtr<Cls, T>& node) {
			auto it = _children.insert(idx, node);
			E_->setParent(dynamic_cast<Cls*>(this));
			return it;
		}
		sli_iter<SClsPtr<Cls, T>> insertChild(sli_iter<SClsPtr<Cls, T>> iter, const SClsPtr<Cls, T>& node) {
			auto it = _children.insert(iter, node);
			E_->setParent(dynamic_cast<Cls*>(this));
			return it;
		}
		void setChild(size_t idx, const SClsPtr<Cls, T>& node) {
			auto it = _children.begin() + idx;
			E_ = node;
			E_->setParent(dynamic_cast<Cls*>(this));
		}
		void set(sli_iter<SClsPtr<Cls, T>> iter, const SClsPtr<Cls, T>& node) {
			*iter = node;
			(*iter)->setParent(dynamic_cast<Cls*>(this));
		}
		sli_iter<SClsPtr<Cls, T>> removeChildAt(size_t idx) {
			return _children.removeAt(idx);
		}
		sli_iter<SClsPtr<Cls, T>> removeChild(sli_iter<SClsPtr<Cls, T>> iter) {
			return _children.remove(iter, iter + 1);
		}
		sli_iter<SClsPtr<Cls, T>> removeChild(const SClsPtr<Cls, T>& node) {
			sforeach(_children) { if (&E_ == &node) return _children.remove(it, it + 1); }
			return end();
		}
		void moveChildTo(sli_iter<SClsPtr<Cls, T>> iter) {
			if (isRoot() || (*iter)->isRoot()) throw SException(ERR_INFO, SLIB_EXEC_ERROR);
			auto node = _parent->begin() + index();			
			(*iter)->parent()->insertChild(iter, *node);
			_parent->removeChild(node);
		}
		*/
		void clearChildren() { _children.clear(); }
		String getClass() const { return "node"; }
		String toString(const char *format) const { return ""; }
		SObject* clone() const { return new Cls(*dynamic_cast<const Cls*>(this)); }
		
	};

	/*
	class SNode : public Node<SNode> {
	public:
		suint type, flag;
		String name;
		sobj value;
		sdict attribute;

	public:
		SNode();
		SNode(const char* name, bool dir = false);
		SNode(const SNode &node);
		~SNode();

		SNode(const SNode& node);

		static SNode rootNode();
		void loadChildren(bool recurrent = false);

		
		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
    

    template<class Cls, OBJ_TYPE T, class Container>
    SNode<Cls, T, Container>::SNode() : SObject(), Node<Cls, scobj<Cls, T>>() {}
    template<class Cls, OBJ_TYPE T, class Container>
    SNode<Cls, T, Container>::SNode(const SNode<Cls, T, Container> &node) : SObject(), Node<Cls, scobj<Cls, T>>(node) {}
    template<class Cls, OBJ_TYPE T, class Container>
    SNode<Cls, T, Container>::~SNode() {}
    
    template<class Cls, OBJ_TYPE T, class Container>
    String SNode<Cls, T, Container>::getClass() const { return "node"; }
    template<class Cls, OBJ_TYPE T, class Container>
    String SNode<Cls, T, Container>::toString() const { return ""; }
    template<class Cls, OBJ_TYPE T, class Container>
    SObject *SNode<Cls, T, Container>::clone() const { return new SNode<Cls, T>(*this); }
	*/
}

#endif
