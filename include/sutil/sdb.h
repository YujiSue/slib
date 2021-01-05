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

	typedef enum {
		SQLITE_DB = 0x01,
		//MYSQL_DB = 0x02,
		
	} DB_MODE;
	/*
	typedef enum {
		EQ = 0x01,
		NOT = 0x02,
		NEQ = 0x04,
		LT = 0x10,
		ELT = 0x11,
		GT = 0x20,
		EGT = 0x21,
		//IN = 0x40,

	} OPERATION;
	*/


	

	namespace sql {
		constexpr subyte INNER_JOIN = 1;
		constexpr subyte OUTER_JOIN = 2;
		constexpr subyte CROSS_JOIN = 3;

		constexpr int KEY_COLUMN = 0x1000;
		constexpr int UNIQUE_COLUMN = 0x2000;
		constexpr int AUTO_INCREMENT_COLUMN = 0x4000;

		extern SLIB_DLL String colTypeName(int type);
		extern SLIB_DLL String colInfo(const SColumn& col);
		extern SLIB_DLL String colInfos(const Array<SColumn>& cols);
		extern SLIB_DLL String colInfos(const SArray& cols);
		extern SLIB_DLL String colNames(const  Array<SColumn>& cols);
		extern SLIB_DLL String colNames(const SArray& cols);
		extern SLIB_DLL String escaped(const char* que);
		extern SLIB_DLL String value(sobj obj, bool like = true, subyte match = 0);
		extern inline String sqlValue(const SearchQuery& que) {
			if (que.value.isNull()) return "NULL";
			else if (que.value.isNum()) return que.value.toString();
			else if (que.value.isArray()) {
				if (que.operation & BETWEEN) return que.value[0].replace("\'", "\'\'") + " AND " + que.value[1].replace("\'", "\'\'");
				else if (que.operation & CONTAIN) return "(" + slib::toString(que.value.array(), ",").replaced("\'", "\'\'") + ")";
				else return SQUOT + slib::toString(que.value.array(), ",").replaced("\'", "\'\'") + SQUOT;
			}
			else if (que.value.isDict()) return slib::toString(que.value.dict(), "&", "=").replaced("\'", "\'\'");
			else {
				if (que.operation & MATCH) {
					if (que.operation & PREFIX) return String::dquot(que.value.replace("\'", "\'\'") + "*");
					else if (que.operation & POSTFIX) return String::dquot("*" + que.value.replace("\'", "\'\'"));
					else if (que.operation & PARTIAL) return String::dquot("*" + que.value.replace("\'", "\'\'") + "*");
					else return String::dquot(que.value.replace("\'", "\'\'"));
				}
				else if (que.operation & LIKE) {
					if (que.operation & PREFIX) return String::squot(que.value.replace("\'", "\'\'") + "%");
					else if (que.operation & POSTFIX) return String::squot("%" + que.value.replace("\'", "\'\'"));
					else if (que.operation & PARTIAL) return String::squot("%" + que.value.replace("\'", "\'\'") + "%");
					else return String::squot(que.value.replace("\'", "\'\'"));
				}
				else return String::squot(que.value.replace("\'", "\'\'"));
			}
		}
		extern inline String sqlQuery(const SearchQuery& que) {
			String str = que.key.replaced("\'", "\'\'");
			if (que.operation == EQ) {
				if (que.value.isNull()) str << " IS NULL";
				else str << " == " << sqlValue(que);
			}
			else if (que.operation == LT) str << " < " << sqlValue(que);
			else if (que.operation == GT) str << " > " << sqlValue(que);
			else if (que.operation == ELT) str << " <= " << sqlValue(que);
			else if (que.operation == EGT) str << " >= " << sqlValue(que);
			else if (que.operation == NEQ) str << " != " << sqlValue(que);
			else if (que.operation & BETWEEN) {
				if (que.operation & NOT) str << " NOT BETWEEN " << sqlValue(que);
				else str << " BETWEEN " << sqlValue(que);
			}
			else if (que.operation & CONTAIN) {
				if (que.operation & NOT) str << " NOT IN " << sqlValue(que);
				else str << " IN " << sqlValue(que);
			}
			else if (que.operation & LIKE) {
				if (que.operation & NOT) str << " NOT LIKE " << sqlValue(que);
				else str << " LIKE " << sqlValue(que);
			}
			else if (que.operation & MATCH) {
				if (que.operation & NOT) str << " NOT GLOB " << sqlValue(que);
				else str << " GLOB " << sqlValue(que);
			}
			return str;
		}
		extern inline String sqlCondition(const SSearchCondition &sc) {
			String str;
			if (sc.queries[0].size()) {
				str << " WHERE ";
				sforeach(sc.queries) {
					if (E_.empty()) continue;
					str << "(";
					sforeach_(ait, E_) {
						str << sqlQuery(*ait);
						if (ait < E_.end() - 1) str << " AND ";
					}
					str << ")";
					if (it < sc.queries.end() - 1) str << " OR ";
				}
			}
			if (!sc.sorters.empty()) {
				str << " ORDER BY ";
				sforeach(sc.sorters) { 
					str << E_.key << SPACE << (E_.order == ASC ? "ASC" : "DESC");
					if (it < sc.sorters.end() - 1) str << ",";
				}
			}
			if (sc.range.end == -1) {
				if (sc.range.begin) str << " OFFSET" << sc.range.begin;
			}
			else {
				if (sc.range.begin) str << " LIMIT " << sc.range.begin << "," << sc.range.length();
				else str << " LIMIT " << sc.range.length();
			}
			return str;
		}
		/*
		extern inline String sqlCase(const SCaseCondition& sc) {
			if (sc.queries.empty()) return "";
			else {
				String str = " CASE";
				sforeach2(sc.queries, sc.values) {
					str << " WHEN " << sqlQuery(E1_) << " THEN " << (E2_.isNum() ? E2_.toString() : String::squot(E2_));
				}
				if (sc.exception.size()) str << " ELSE " << sc.exception;
				str << " END";
				if (sc.as.size()) str << " AS " << sc.as;
			}
		}
		*/
		class SLIB_DLL SJoinCondition {
		public:

		};


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
		extern SLIB_DLL String condQue(const sobj& conds, bool join = false);
		extern SLIB_DLL String caseBy(const char* name, const sattribute& attr, const char* exception, const char* as);
		extern SLIB_DLL String caseQue(const sobj& obj);
		extern SLIB_DLL String order(const Array<std::pair<String, ORDER>>& orders);
		extern SLIB_DLL String orderQue(const sobj& orders);
		extern SLIB_DLL String limit(int l, int o = -1);
		extern SLIB_DLL String limitQue(const sobj& obj);
		extern SLIB_DLL String strListQue(const stringarray& list);
		extern SLIB_DLL String intListQue(const intarray& list);
		extern SLIB_DLL String listQue(const sobj& obj);
		extern SLIB_DLL String joinQue(const char* tbl, const sobj& obj);
		extern SLIB_DLL String selectQue(const char* tbl, const SDictionary& obj);
		extern SLIB_DLL String selectQuery(const char* name, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
		extern SLIB_DLL String joinedSelectQuery(subyte type, const char* name1, const char* name2,
			const char* join, const stringarray& cols = { "*" },
			const char* condition = nullptr, const char* order = nullptr,
			const char* limit = nullptr, bool distinct = false);
	}
	class SLIB_DLL SDataBase;

	//class SJoinQuery
	class SLIB_DLL SRecord : public SObject {
		friend SDataBase;
	private:
		suint _type;
		SDataBase* _db;
		String _name;
		sobj _value;
		
	public:
		SRecord();
		~SRecord();

		/*
		sobj get(size_t count = -1);
		sobj get(const stringarray& columns, size_t count = -1);
		sobj set(sobj data);
		SRecord& incerement(sint i = 1);
		SRecord& decrement(sint i = 1);
		SRecord& insert(sobj data);
		SRecord& transaction();
		*/
		//SRecord& from(size_t off);
		//SRecord& where();
		//SRecord& sort();
		//SRecord& join();
	};
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
		DB_MODE _mode;
		
		sqlite3* _db;
		sqlite3_stmt* _stmt;

		//Array<SRecord> _records;
		
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
		void createTable(const char* name, const Array<SColumn>& columns = {}, const SArray& rows = {});
		void createTable(const char* name, const STable& table);
		void createTable(const SDictionary& dic);
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