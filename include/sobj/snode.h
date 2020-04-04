#ifndef SLIB_SNODE_H
#define SLIB_SNODE_H

#include "sobj/sobject.h"

namespace slib {
    
    template<class Cls, OBJ_TYPE T = NODE_OBJ, class Container=Array<scobj<Cls, T>>>
    class SNode : public SObject, public Node<Cls, scobj<Cls, T>, Container> {
    public:
        SNode();
        SNode(const SNode &node);
        ~SNode();
        
        String getClass() const;
        String toString() const;
        SObject *clone() const;
    };
	/*
	class SFileNode : public SNode<SFileNode> {
	protected:
		String _name;
		bool _dir;

	public:
		SFileNode();
		SFileNode(const char* name, bool dir = false);
		SFileNode(const SFileNode &node);
		~SFileNode();

		SFileNode(const SFileNode& node);

		static SFileNode rootNode();
		void loadChildren(bool recurrent = false);

		sio::SFile fileAt(const char *name);

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
    */
    /*============================================================*/
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
}

#endif
