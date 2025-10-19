#ifndef SLIB_SDB_H
#define SLIB_SDB_H
#include "sbasic/search.h"
#include "sutil/ssqlite.h"

namespace slib {
	inline void _extendCondQue(slib::String& str) {}
	template<class First, class... Remain>
	inline void _extendCondQue(slib::String& str, First& f, Remain&... args) {
		str << (str.empty() ? "" : " ") << f;
		_extendCondQue(str, args...);
	}
	inline void _extendSortQue(String& str) {}
	template<class... Args>
	inline void _extendSortQue(String& str, const String& key, slib::ORDER order, Args&... args) {
		str << (str.empty() ? "" : ",") << sdb::orderQuery(key, order);
		_extendSortQue(str, args...);
	}
	/**
	* @class SDBException
	* \~english @brief Database exception class
	*/
	class SLIB_DLL SDBException : public Exception {
	public:
		SDBException();
		SDBException(int i, const char *msg, const char *desc);
		~SDBException();
	};
	class SLIB_DLL SDataBase;
	//extern SLIB_DLL void joinSchema(SDataBase &db, const String &name1, const String& name2, SDictionary &schema);
	extern SLIB_DLL void joinSchema(SDataBase& db, const stringarray& names, SDictionary& schema);

	/**
	* @class SDBTable
	* \~english @brief Database table class
	*/
	class SLIB_DLL SDBTable : public STable {
		SDataBase* _db;
		String _condition, _order, _join, _group, _range;
		stringarray _joined;
		SDictionary _schema;
		bool _prepare;

	public:
		String name;
		
	public:
		SDBTable();
		SDBTable(const char* name, SDataBase* db);
		~SDBTable();

		const SDictionary &schema() const;
		
		SDBTable& prepare();
		void complete();

		SDBTable& rename(const char *name);
		void drop();

		SDBTable& addColumn(const SColumn &col);
		SDBTable& addColumns(const Array<SColumn>& col);
		SDBTable& renameColumn(const char *ori, const char *dest);
		SDBTable& removeColumn(const char* name);
		SDBTable& insert(const SObjPtr& obj = snull, bool returning = false, const char *retkeys = nullptr);
		SDBTable& insertAll(const SArray& array, bool returning = false, const char* retkeys = nullptr);
		SDBTable& addRecord();
		SDBTable& addRecord(const SObjPtr& obj);
		SDBTable& addRecords(const SArray& array);

		SDBTable& update(const stringarray &keys);
		SDBTable& update(const SDictionary& dict);
		SDBTable& setRecord(const SObjPtr& obj);
		SDBTable& setRecords(const SArray& array);
		
		SDBTable& upsert(const char *uniq_key, const SDictionary& dict);

		size_t count(bool reset_condition = true);
		SDBTable& countBy(const stringarray& keys);
		SDBTable& select(const stringarray& keys = { "*" });

		SDBTable& erase();
		
		SDBTable& deleteRecord();

		void clearCondition();
		void reset();


		template<class... Args>
		SDBTable& join(const char* name2, sdb::JOIN mode, Args... args) {
			String on;
			_extendCondQue(on, args...);
			if (_joined.empty()) _joined.add(name);
			_joined.add(name2);
			_join << sdb::joinQuery(name2, mode, on);
			_schema.clear();
			joinSchema(*_db, _joined, _schema);
			return *this;
		}
		template<class... Args>
		SDBTable& where(Args... args) {
			if (_condition.size()) _condition << " AND";
			_extendCondQue(_condition, args...);
			return *this;
		}
		template<class... Args>
		SDBTable& sortBy(Args... args) {
			_extendSortQue(_order, args...);
			return *this;
		}
		SDBTable& grouping(const stringarray& keys);
		SDBTable& limit(const size_t lim, const size_t off = 0);
	};
	
	/**
	* @class SDataBase
	* \~english @brief Database class
	*/
	class SLIB_DLL SDataBase {
		friend SDBTable;
	protected:
		sqlite3* _db;
		sqlite3_stmt* _stmt;
		String _sql;
		SDictionary _buffer;
		STable _result;
		bool _connect, _trans;
	public:
		String path;
		int res;
		char* err;

	public:
		SDataBase();
		SDataBase(const char* s);
		~SDataBase();
		void open(const char* s);
		//void connect(const char* s);
		void close();
		SArray tables();
		SDictionary schema(const char* name);
		bool exist(const char* name);
		void create(const char* name, const Array<SColumn>& columns, const SArray& rows = {});
		SDBTable table(const char* tbl);
		SDBTable operator[](const char* tbl);
		STable& result();
		void exec(const char* sql);
		void prepare(const char* sql);
		void begin();
		void commit();
		void vacuum();
		void bindi(const int val, const int i);
		void bindl(const sinteger val, const int i);
		void bindd(const double val, const int i);
		void bindt(const char* val, const int i);
		void bindtt(const char* val, const int i);
		void bindb(void* val, const int size, const int i);
		void bindbt(void* val, const int size, const int i);
		void bindNull(const int i);
		void bindObj(const SObjPtr& obj, int i);
		void bindValues(const SArray& values);
		void bindValues(const SDictionary& values, const stringarray& keys);
		void step(STable *result = nullptr, SDictionary* schema = nullptr);
		void fetch(STable* result = nullptr, SDictionary* schema = nullptr);
		void reset();
		void finalize();
		void clear();
	};
}

#endif