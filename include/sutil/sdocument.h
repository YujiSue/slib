#ifndef SLIB_SDOC_H
#define SLIB_SDOC_H

#include "sobj/sobject.h"
#include "sio/sio.h"

namespace slib {
	template<class ROOT>
	class SOBJ_DLL SDocument : public SObject {
	protected:
		suint _type;
		String _title, _author;
		SDate _created, _revised;
		SDictionary _attribute;
		ROOT _root;

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
		ROOT& root();
		
		void setTitle(const char* s);
		void setAuthor(const char* s);
		void setRoot(ROOT&& elem);
		void setRoot(const ROOT& elem);

		virtual String getClass() const;
		virtual String toString() const;
		virtual SObject* clone() const;
	};

	template<class ROOT>
	SDocument<ROOT>::SDocument() : SObject() {
		_created = SDate(); _revised = _created;
	}
	template<class ROOT>
	SDocument<ROOT>::SDocument(suint t, const char* ti, const char* a) : SDocument() {
		_type = t;
		if (ti) _title = ti;
		if (a) _author = a;
	}
	template<class ROOT>
	SDocument<ROOT>::SDocument(const SDocument<ROOT>& doc) : SObject() {
		_type = doc._type;
		_title = doc._title;
		_author = doc._author;
		_created = doc._created;
		_revised = doc._revised;
		_attribute = doc._attribute;
		_root = doc._root;
	}
	template<class ROOT>
	SDocument<ROOT>::~SDocument() {}
	template<class ROOT>
	void SDocument<ROOT>::load(const char *path) {}
	template<class ROOT>
	void SDocument<ROOT>::save(const char* path) {}
	template<class ROOT>
	suinteger SDocument<ROOT>::type() const { return _type; }
	template<class ROOT>
	String& SDocument<ROOT>::title() { return _title; }
	template<class ROOT>
	String& SDocument<ROOT>::author() { return _author; }
	template<class ROOT>
	SDate& SDocument<ROOT>::created() { return _created; }
	template<class ROOT>
	SDate& SDocument<ROOT>::revised() { return _revised; }
	template<class ROOT>
	SDictionary& SDocument<ROOT>::attribute() { return _attribute; }
	template<class ROOT>
	ROOT& SDocument<ROOT>::root() { return _root; }
	template<class ROOT>
	void SDocument<ROOT>::setTitle(const char* s) { _title = s; }
	template<class ROOT>
	void SDocument<ROOT>::setAuthor(const char* s) { _author = s; }
	template<class ROOT>
	String SDocument<ROOT>::getClass() const { return "doc"; }
	template<class ROOT>
	String SDocument<ROOT>::toString() const { return  "";  }
	template<class ROOT>
	SObject* SDocument<ROOT>::clone() const { return new SDocument(*this); }
}

#endif