#ifndef SLIB_SDOC_H
#define SLIB_SDOC_H

#include "sobj/sobject.h"
#include "sio/sio.h"

namespace slib {
	template<class Node>
	class SDocument : public SObject {
	protected:
		suint _type;
		String _title, _author;
		SDate _created, _revised;
		SDictionary _attribute;
		Node _root;

	public:
		SDocument();
		SDocument(suint t, const char* ti = nullptr, const char* a = nullptr);
		SDocument(const SDocument&doc);
		virtual ~SDocument();

		virtual void load(const char* path);
		virtual void save(const char* path);
		suinteger type() const;
		String& title();
		String& author();
		SDate& created();
		SDate& revised();
		SDictionary& attribute();
		Node& root();
		
		void setTitle(const char* s);
		void setAuthor(const char* s);
		void setRoot(Node&& elem);
		void setRoot(const Node& elem);

		virtual String getClass() const;
		virtual String toString() const;
		virtual SObject* clone() const;
	};

	template<class Node>
	SDocument<Node>::SDocument() : SObject() {
		_created = SDate(); _revised = _created;
	}
	template<class Node>
	SDocument<Node>::SDocument(suint t, const char* ti, const char* a) : SDocument() {
		_type = t;
		if (ti) _title = ti;
		if (a) _author = a;
	}
	template<class Node>
	SDocument<Node>::SDocument(const SDocument<Node>& doc) : SObject() {
		_type = doc._type;
		_title = doc._title;
		_author = doc._author;
		_created = doc._created;
		_revised = doc._revised;
		_attribute = doc._attribute;
		_root = doc._root;
	}
	template<class Node>
	SDocument<Node>::~SDocument() {}
	template<class Node>
	void SDocument<Node>::load(const char *path) {}
	template<class Node>
	void SDocument<Node>::save(const char* path) {}
	template<class Node>
	suinteger SDocument<Node>::type() const { return _type; }
	template<class Node>
	String& SDocument<Node>::title() { return _title; }
	template<class Node>
	String& SDocument<Node>::author() { return _author; }
	template<class Node>
	SDate& SDocument<Node>::created() { return _created; }
	template<class Node>
	SDate& SDocument<Node>::revised() { return _revised; }
	template<class Node>
	SDictionary& SDocument<Node>::attribute() { return _attribute; }
	template<class Node>
	Node& SDocument<Node>::root() { return _root; }
	template<class Node>
	void SDocument<Node>::setTitle(const char* s) { _title = s; }
	template<class Node>
	void SDocument<Node>::setAuthor(const char* s) { _author = s; }
	template<class Node>
	String SDocument<Node>::getClass() const { return "doc"; }
	template<class Node>
	String SDocument<Node>::toString() const { return  "";  }
	template<class Node>
	SObject* SDocument<Node>::clone() const { return new SDocument(*this); }
}

#endif