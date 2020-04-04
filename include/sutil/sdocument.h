#include "sobj/sobject.h"
#include "sio/sio.h"

namespace slib {
	template<typename E>
	class SDocument : public SObject {
	protected:
		suint _type;
		String _title, _author;
		SDate _created, _revised;
		SDictionary _attribute;
		E _root;

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
		E &root();
		
		void setTitle(const char* s);
		void setAuthor(const char* s);
		void setRoot(E&& elem);
		void setRoot(const E& elem);

		virtual String getClass() const;
		virtual String toString() const;
		virtual SObject* clone() const;
	};

	template<typename E>
	SDocument<E>::SDocument() : SObject() {
		_created = SDate(); _revised = _created;
	}
	template<typename E>
	SDocument<E>::SDocument(suint t, const char* ti, const char* a) : SDocument() {
		_type = t;
		if (ti) _title = ti;
		if (a) _author = a;
	}
	template<typename E>
	SDocument<E>::SDocument(const SDocument<E>& doc) : SObject() {
		_type = doc._type;
		_title = doc._title;
		_author = doc._author;
		_created = doc._created;
		_revised = doc._revised;
		_attribute = doc._attribute;
		_root = doc._root;
	}
	template<typename E>
	SDocument<E>::~SDocument() {}
	template<typename E>
	void SDocument<E>::load(const char *path) {}
	template<typename E>
	void SDocument<E>::save(const char* path) {}
	template<typename E>
	suinteger SDocument<E>::type() const { return _type; }
	template<typename E>
	String& SDocument<E>::title() { return _title; }
	template<typename E>
	String& SDocument<E>::author() { return _author; }
	template<typename E>
	SDate& SDocument<E>::created() { return _created; }
	template<typename E>
	SDate& SDocument<E>::revised() { return _revised; }
	template<typename E>
	SDictionary& SDocument<E>::attribute() { return _attribute; }
	template<typename E>
	E& SDocument<E>::root() { return _root; }
	template<typename E>
	void SDocument<E>::setTitle(const char* s) { _title = s; }
	template<typename E>
	void SDocument<E>::setAuthor(const char* s) { _author = s; }
	template<typename E>
	String SDocument<E>::getClass() const { return "doc"; }
	template<typename E>
	String SDocument<E>::toString() const { return  "";  }
	template<typename E>
	SObject* SDocument<E>::clone() const { return new SDocument(*this); }
}


