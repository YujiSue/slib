#ifndef SLIB_SDB_H
#define SLIB_SDB_H

#include "sio/sfile.h"
#include "sobj/stable.h"
extern "C" {
#include "sqlite/sqlite3.h"
}

namespace slib {
	class SLIB_DLL SDBException : public SException {
	public:
		SDBException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
		~SDBException();
	};
	constexpr suint SQL_ERROR = 0x0021;

#define SQL_ERR_TEXT(N, T) (u8"sqlite3 error #"+std::to_string(N)+u8": "+std::string(T)).c_str()

#define SDB_ORDER(X,Y) std::pair<String, slib::ORDER>((X),(Y))


	namespace sql {
		constexpr subyte INNER_JOIN = 1;
		constexpr subyte OUTER_JOIN = 2;
		constexpr subyte CROSS_JOIN = 3;

		constexpr int KEY_COLUMN = 0x1000;
		constexpr int UNIQUE_COLUMN = 0x2000;
		constexpr int AUTO_INCREMENT_COLUMN = 0x4000;

		extern String colTypeName(int type);
		extern String colInfo(const sobj& col);
		extern String colInfos(const SArray& cols);
		extern String colNames(const SArray& cols);
		extern String escaped(const char* que);
		extern String value(sobj obj, bool like = true, subyte match = 0);

		inline void addConditionQuery(String& que) {}
		template <class First, class... Args>
		inline void addConditionQuery(String& que, First first, Args... args) {
			que << " " << first;
			addConditionQuery(que, args...);
		}
		template <class... Args>
		inline String condition(Args... args) {
			String query = " WHERE";
			addConditionQuery(query, args...);
			return query;
		}
		extern String condQue(const sobj& conds, bool join = false);
		extern String caseBy(const char* name, const sattribute& attr, const char* exception, const char* as);
		extern String caseQue(const sobj& obj);
		extern String order(const Array<std::pair<String, ORDER>>& orders);
		extern String orderQue(const sobj& orders);
		extern String limit(int l, int o = -1);
		extern String limitQue(const sobj& obj);
		extern String strListQue(const stringarray& list);
		extern String intListQue(const intarray& list);
		extern String listQue(const sobj& obj);
		extern String joinQue(const char* tbl, const sobj& obj);
		extern String selectQue(const char* tbl, const SDictionary& obj);
		extern String selectQuery(const char* name, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
		extern String joinedSelectQuery(subyte type, const char* name1, const char* name2,
			const char* join, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
	}
	class SLIB_DLL SDataBase;
	class SLIB_DLL SDBTable {
	private:
		SDataBase* _db;
		String _table;

	public:
		SDBTable(const char* t = nullptr, SDataBase* d = nullptr);
		~SDBTable();

		bool exist();
		void create(sobj obj);
		void rename(const char* name);
		void remove();
		stringarray columns();
		SArray& columnInfo();
		void addColumn(int type, const char* name);
		void addColumn(const SDictionary& col);
		void removeColumn(const char* name);
		void addRecordArray(const SArray& row);
		void addRecordDict(const SDictionary& row);
		void addRecord(const sobj& obj);
		void addRecordPrepare(size_t num);
		void addRecordPrepare(const SArray& cols);
		void removeRecord(const char* condition);
		void removeRecordAt(size_t idx, const char *key = "ID");
		void setRecord(const sobj& obj);
		void setRecord(const SDictionary& row, const char* condition);
		void setRecordAt(size_t idx, const SDictionary& row, const char* key = "ID");
		void setRecordPrepare(const SArray& cols, const char* key = "ID");
		int count(const char* condition = nullptr);
		SArray& countGroup(const SArray& cols);
		SDictionary& getRecord(const stringarray& cols = { "*" }, const char* condition = nullptr,
			const char* order = nullptr, SDictionary* result = nullptr);
		SDictionary& getRecord(const SDictionary& info, SDictionary* result = nullptr);
		SDictionary& getRecordAt(size_t idx, const stringarray& cols = { "*" }, const char* key = "ID", SDictionary* result = nullptr);
		void getRecordPrepare(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false);
		void getRecordPrepare(const SDictionary& info);
		SArray& getRecords(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			SArray* result = nullptr);
		SArray& getRecords(const SDictionary& info, SArray* result = nullptr);
		STable& getTable(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			STable* result = nullptr);
		STable& getTable(const SDictionary& info, STable* result = nullptr);
		SArray& getJoinedRecords(subyte type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			SArray* result = nullptr);
		SArray& getJoinedRecords(const SDictionary& info, SArray* result = nullptr);
		void getJoinedRecordPrepare(subyte type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false);
		void getJoinedRecordPrepare(const SDictionary& info);
	};

	class SLIB_DLL SDataBase : public SObject {
		friend SDBTable;

	private:
		sqlite3* _db;
		sqlite3_stmt* _stmt;
		String _path;
		STable _result;
		SDictionary _row;
		SArray _rows;
		char* _err;
		bool _open, _transaction;
		int _res;

	public:
		SDataBase();
		SDataBase(const char* path);
		~SDataBase();

		void open(const char* path);
		void close();
		bool isOpened() const;
		bool isTransacting() const;
		const char* path() const;

		int tableCount();
		sobj tables();

	public:
		void createTable(const char* name, const SArray& columns = {}, const SArray& rows = {});
		void createTable(const SDictionary& dic);
		void createTable(const STable& table);
		void removeTable(const char* name);
		void clearTables();
		SDBTable table(const char* name);
		SDBTable operator[](const char* name);

		void bindi(sint val, int i);
		void bindl(sinteger val, int i);
		void bindd(double val, int i);
		void bindt(const char* val, int i);
		void bindtt(const char* val, int i);
		void bindb(void* val, int size, int i);
		void bindbt(void* val, int size, int i);
		void bindNull(int i);
		void bind(const sobj& obj, int i);
		void bindRow(const sobj& obj);
		void sqlexec(const char* sql);
		void sqlprepare(const char* sql);
		void reset();
		void begin();
		void commit();
		void release();
		void step();

		STable& getResult(STable* table = nullptr);
		SDictionary& getRow(SDictionary* dict = nullptr);
		SArray& getRows(SArray* array = nullptr);

		String getClass() const;
		String toString() const;
		SObject* clone() const;
	};
}

#endif