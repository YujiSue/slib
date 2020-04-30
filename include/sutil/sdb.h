#ifndef SLIB_SDB_H
#define SLIB_SDB_H

#include "sio/sfile.h"
#include "sobj/stable.h"
extern "C" {
#include "sqlite/sqlite3.h"
}

namespace slib {
	class SOBJ_DLL SDBException : public SException {
	public:
		SDBException(const char* f, sint l, const char* func, sint e = 0, const char* target = nullptr, const char* note = nullptr);
		~SDBException();
	};
	constexpr suint SQL_ERROR = 0x0021;

#define SQL_ERR_TEXT(N, T) (u8"sqlite3 error #"+std::to_string(N)+u8": "+std::string(T)).c_str()

#define SDB_ORDER(X,Y) std::pair<String, slib::ORDER>((X),(Y))


	namespace sql {
		
		typedef enum {
			EQ = 0,
			NEQ = 1,
			LT = 2,
			MT = 3,
			ELT = 4,
			EMT = 5,
			LIKE = 6,
			GLOB = 7,
			IN_RANGE = 8,
		} OPERATOR;
		typedef enum {
			EXACT = 0,
			CONTAIN = 1,
			BEGIN = 2,
			END = 3,
		} MATCH_TYPE;
		typedef enum {
			INNER_JOIN = 1,
			OUTER_JOIN = 2,
			CROSS_JOIN = 3,
		} JOIN_TYPE;

		constexpr int KEY_COLUMN = 0x1000;
		constexpr int UNIQUE_COLUMN = 0x2000;
		constexpr int AUTO_INCREMENT_COLUMN = 0x4000;

		extern String colTypeName(int type);
		extern String colInfo(const sobj& col);
		extern String colInfos(const SArray& cols);
		extern String colNames(const SArray& cols);
		extern String escaped(const char* que);

		extern String textQue(const char* val, MATCH_TYPE match = EXACT, OPERATOR op = LIKE);

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
		extern String condQue(const sobj& conds);
		extern String caseQue(const char* name, const sattribute& attr, const char* exception, const char* as);
		extern String caseQue(const sobj& obj);
		extern String orderQue(const Array<std::pair<String, ORDER>>& orders);
		extern String orderQue(const sobj& orders);
		extern String limitQue(int l, int o = -1);
		extern String limitQue(const sobj& obj);
		extern String listQue(const stringarray& list);
		extern String listQue(const intarray& list);
		extern String listQue(const sobj& obj);
		extern String selectQue(const sobj& obj);
		extern String selectQuery(const char* name, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
		extern String joinedSelectQuery(sql::JOIN_TYPE type, const char* name1, const char* name2,
			const char* join, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
	}
	class SOBJ_DLL SDataBase;
	class SOBJ_DLL SDBTable {
	private:
		SDataBase* _db;
		String _table;

	public:
		SDBTable(const char* t = nullptr, SDataBase* d = nullptr);
		~SDBTable();

		bool exist();
		void create(const SArray& columns = {});
		void rename(const char* name);
		void copy(const char* src, const stringarray& cols = { "*" }, const SDictionary& conditions = {});
		void remove();
		stringarray columns();
		SArray& columnInfo();
		void addColumn(int type, const char* name);
		void addColumn(const SDictionary& dict);
		void addColumn(const sobj& obj);
		void removeColumn(const char* name);
		void addRecord(const SArray& row);
		void addRecord(const SDictionary& row);
		void addRecord(const sobj &obj);
		void addRecordPrepare(size_t num);
		void addRecordPrepare(const stringarray& cols);
		void removeRecord(const char* condition);
		void removeRecord(const SDictionary &condition);
		void removeRecord(const sobj& obj);
		void removeRecordAt(size_t idx, const char *key = "ID");
		void setRecord(const sobj& obj);
		void setRecord(const SDictionary& row, const char* condition);
		void setRecordAt(size_t idx, const SDictionary& row, const char* key = "ID");
		void setRecordPrepare(const stringarray& cols, const char* condition);
		int count(const char* condition = nullptr);
		SArray& countGroup(const stringarray& cols);
		void getRecordPrepare(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false);
		STable& getTable(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			STable* result = nullptr);
		STable& getTable(const SDictionary& info, STable* result = nullptr);
		SDictionary& getRecord(const stringarray& cols = { "*" }, const char* condition = nullptr,
			const char* order = nullptr, SDictionary* result = nullptr);
		SDictionary& getRecord(const SDictionary& info, SDictionary* result = nullptr);
		SDictionary& getRecordAt(size_t idx, const stringarray& cols = { "*" }, const char* key = "ID", SDictionary* result = nullptr);

		SArray& getRecords(const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			SArray* result = nullptr);
		SArray& getRecords(const SDictionary& info, SArray* result = nullptr);

		void getJoinedRecordPrepare(sql::JOIN_TYPE type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false);
		STable& getJoinedTable(sql::JOIN_TYPE type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			STable* result = nullptr);
		STable& getJoinedTable(const SDictionary& info, STable* result = nullptr);

		SDictionary& getJoinedRecord(sql::JOIN_TYPE type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			SDictionary* result = nullptr);
		SDictionary& getJoinedRecord(const SDictionary& info, SDictionary* result = nullptr);
		SDictionary& getJoinedRecordAt(size_t idx,
			sql::JOIN_TYPE type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			SDictionary* result = nullptr);

		SArray& getJoinedRecords(sql::JOIN_TYPE type,
			const char* table2,
			const char* join,
			const stringarray& cols = { "*" },
			const char* condition = nullptr,
			const char* order = nullptr,
			const char* limit = nullptr,
			bool distinct = false,
			SArray* result = nullptr);
		SArray& getJoinedRecords(const SDictionary& info, SArray* result = nullptr);
	};

	class SOBJ_DLL SDataBase : public SObject {
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

	protected:
		void _createTable(const SDictionary& dic);
		void _createTable(const STable& table);

	public:
		void createTable(const char* name, const SArray& columns = {}, const SArray& rows = {});
		void createTable(const sobj& obj);
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