#ifndef SLIB_SSQLITE_H
#define SLIB_SSQLITE_H
#include "sbasic/search.h"
#include "sobj/sstring.h"
#include "sobj/stable.h"
extern "C" {
#include "sqlite/sqlite3.h"
#include "sqlite/sqlite3ext.h"
}
namespace slib {
	namespace sdb {
		constexpr size_t MAX_EXEC_TRIAL = 3;
		enum class JOIN : subyte {
			INNER = 0x00,
			LEFT = 0x01,
			RIGHT = 0x02,
			FULL = 0x03,
		};

		extern SLIB_DLL String sqlValue(const SObjPtr& obj);
		extern SLIB_DLL String sqlValues(const SArray& array);
		extern SLIB_DLL String sqlValues(const SDictionary& dict, const stringarray &keys);

		extern SLIB_DLL String tableListQuery();
		extern SLIB_DLL String searchTableQuery(const String& name);

		extern SLIB_DLL String createTableQuery(const String& name, const Array<SColumn>& columns);
		extern SLIB_DLL String renameTableQuery(const String& oldname, const String& newname);
		extern SLIB_DLL String dropTableQuery(const String& name);
		extern SLIB_DLL String tableSchemaQuery(const String& name);

		extern SLIB_DLL String addColumnQuery(const char* table, const SColumn &col);
		extern SLIB_DLL String renameColumnQuery(const char* table, const char* oldname, const char* newname);
		extern SLIB_DLL String removeColumnQuery(const char* table, const char* name);

		extern SLIB_DLL String insertQuery(const String& table, const SArray& array, bool returning = false, const char* retkeys = nullptr);
		extern SLIB_DLL String insertQuery(const String& table, const SDictionary& dict, bool returning = false, const char* retkeys = nullptr);
		extern SLIB_DLL String insertPrepareQuery(const String& table, const size_t num, bool returning = false, const char* retkeys = nullptr);
		extern SLIB_DLL String insertPrepareQuery(const String& table, const stringarray& keys, bool returning = false, const char* retkeys = nullptr);

		extern SLIB_DLL String updateQuery(const String& table, const SDictionary& dict, const String& condition);
		extern SLIB_DLL String updatePrepareQuery(const String& table, const stringarray& keys, const String& condition);
		extern SLIB_DLL String upsertQuery(const String& table, const char* uniq, const SDictionary& dict, const String& condition);

		extern SLIB_DLL String countQuery(const String& table, const String& condition);
		extern SLIB_DLL String groupCountQuery(const String& table, const stringarray& keys, const String& condition);
		extern SLIB_DLL String selectQuery(const String& table, const stringarray& target, const String& join, const String& order, const String& condition, const String& group, const String& limit, bool distinct = false);

		extern SLIB_DLL String deleteQuery(const String& table, const String& condition);

		
		
		inline void _andQue(String& q) {}
		template<typename First, typename... Args>
		inline void _andQue(String& q, First& first, Args&... args) {
			q << first << " AND ";
			_andQue(q, args...);
		}
		template<typename... Args>
		extern String andQuery(Args... args) {
			String que("(");
			_andQue(que, args...);
			que.resize(que.size() - 5);
			return que << ")";
		}
		inline void _orQue(String& q) {}
		template<typename Arg, typename... Args>
		inline void _orQue(String& q, Arg& a, Args&... args) {
			q << " OR " << a; _orQue(q, args...);
		}
		template<typename... Args>
		extern String orQuery(Args... args) {
			String que = "(";
			_orQue(que, args...);
			return que << ")";
		}
		inline void _conditionQue(String& q) {}
		template<typename Arg, typename... Args>
		inline void _conditionQue(String &q, Arg &a, Args&... args) {
			q << SP << a; _conditionQue(q, args...);
		}
		template<typename... Args>
		extern String conditionQuery(Args... args) {
			String que;
			_conditionQue(que, args...);
			return que;
		}

		extern SLIB_DLL String formulaQuery(const char* operation, const String& key, const slib::SObjPtr &value);
		
		extern SLIB_DLL String textQuery(const char* key, const char* value, slib::MATCH m = slib::MATCH::EXACT, bool glob = false);
		extern SLIB_DLL String textsQuery(const char* key, const stringarray &values, slib::MATCH m = slib::MATCH::EXACT, bool a = false, bool glob = false);
		extern SLIB_DLL String textInQuery(const char* key, const char* value, const char* sep = ",", slib::MATCH m = slib::MATCH::EXACT);
		extern SLIB_DLL String textsInQuery(const char* key, const stringarray& values, const char* sep = ",", slib::MATCH m = slib::MATCH::EXACT, bool a = false);
		extern SLIB_DLL String inQuery(const char* key, const intarray& array);
		extern SLIB_DLL String inQuery(const char* key, const stringarray& array);
		extern SLIB_DLL String inQuery(const char* key, const SArray &array);


		extern SLIB_DLL String joinQuery(const String& table, sdb::JOIN mode, const String& formula);
		extern SLIB_DLL String groupQuery(const stringarray& keys);
		extern SLIB_DLL String orderQuery(const String& key, slib::ORDER order);
		extern SLIB_DLL String orderListQuery(Array<Pair<String, ORDER>> orders);		
		extern SLIB_DLL String limitQuery(const size_t limit, const size_t offset);
	}
}
#endif