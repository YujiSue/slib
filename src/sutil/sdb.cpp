#include "sutil/sdb.h"

using namespace slib;

SDBException::SDBException(const char* f, sint l, const char* func, sint e, const char* target, const char* note) 
	: SException(f, l, func, e, target, note) { 
	prefix = "sdb";
	if (err == SQL_ERROR) {
		message = "SQLite3 error.";
		description = TARGET_TEXT(target) + u8" process error. [" + std::string(note ? note : "") + "]";
	}
}
SDBException::~SDBException() {}

String sql::colTypeName(int type) {
    String name = String::upper(SColumn::colTypeStr(type&0x0FFF));
    if(type & KEY_COLUMN) name += " PRIMARY KEY";
    if(type & UNIQUE_COLUMN) name += " UNIQUE";
    if(type & AUTO_INCREMENT_COLUMN) name += " AUTOINCREMENT";
    return name;
}
String sql::colInfo(const sobj &col) {
	if (col.isColumn()) return col.name() + " " + sql::colTypeName(col.type());
	else if (col.isDict()) return col["name"] + " " + col["type"];
}
String sql::colInfos(const SArray& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) str << colInfo(E_) << ",";
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::colNames(const SArray& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) {
		if (E_.isDict()) str << E_["name"] << ",";
		else if (E_.isColumn()) str << E_.name() << ",";
	}
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::escaped(const char *que) { String str(que); str.replace("\'", "\'\'"); return str; }
String sql::textQue(const char* val, sql::MATCH_TYPE match, sql::OPERATOR op) {
	String str;
	switch (match) {
	case EXACT:
		str << sql::escaped(val);
		break;
	case CONTAIN:
		if (op == LIKE) str << "%" << sql::escaped(val) << "%";
		else str << "*" << sql::escaped(val) << "*";
		break;
	case BEGIN:
		if (op == LIKE) str << "%" << sql::escaped(val);
		else str << "*" << val;
		break;
	case END:
		if (op == LIKE) str << sql::escaped(val) << "%";
		else str << val << "*";
		break;
	default:
		break;
	}
	return String::squot(str);
}
String sql::condQue(const sobj& conds) {
	String que = " WHERE ";


	return que;
}
String sql::caseQue(const char* name, const sattribute& attr, const char* exception, const char* as) {
	String que;
	que << " CASE " << name;
	for (auto it = attr.begin(); it != attr.end(); ++it)
		que << " WHEN " << it->key << " THEN " << it->value;
	if (exception) que << " ELSE " << exception;
	que << " END";
	if (as) que << " AS " << as;
	return que;
}
extern inline void addCaseQue(String& str, const sobj& obj) {
	str << " WHEN ";
	if (obj.hasKey("col")) str << obj["col"] << obj["op"] << obj["val"] << " THEN " << obj["res"];
	else str << obj["val"] << " THEN " << obj["res"];
}
String sql::caseQue(const sobj& obj) {
	String que;
	auto& cases = obj.dict();
	que << " CASE";
	if (cases.hasKey("case")) que << " " << cases["case"];
	if (cases.hasKey("if")) addCaseQue(que, cases["if"]);
	else if (cases.hasKey("ifs")) {
		sforeach(cases["ifs"]) addCaseQue(que, E_);
	}
	if (cases.hasKey("else")) que << " ELSE " << cases["else"];
	que << "END";
	if (cases.hasKey("as")) que << " AS " << cases["as"];
	return que;
}
String sql::orderQue(const Array<std::pair<String, ORDER>>& orders) {
	if (orders.empty()) return "";
	String oque;
	sforeach(orders) oque << E_.first << " " << (E_.second == ASC ? "ASC" : "DESC") << ",";
	oque.resize(oque.length() - 1);
	return String(" ORDER BY ") << oque;
}
String sql::orderQue(const sobj& obj) {
	auto& orders = obj.array();
	if (orders.empty()) return "";
	String oque;
	sforeach(orders) oque << E_["name"] << " " << E_["order"] << ",";
	oque.resize(oque.length() - 1);
	return String(" ORDER BY ") << oque;
}
String sql::limitQue(int l, int o) {
	if (0 < l) {
		if (o < 0) return String(" LIMIT ") << l;
		else return String(" LIMIT ") << o << "," << l;
	}
	else if (-1 < o) return String(" OFFSET ") << o;
	else return "";
}
String sql::limitQue(const sobj& obj) {
	auto& lim = obj.dict();
	if (lim.hasKey("limit")) {
		if (lim.hasKey("offset")) return String(" LIMIT ") << lim["offset"] << "," << lim["limit"];
		else return String(" LIMIT ") << lim["limit"];
	}
	else if (lim.hasKey("offset")) return String(" OFFSET ") << lim["offset"];
	else return "";
}
String sql::listQue(const stringarray& list) {
	String str;
	sforeach(list) str += String::squot(sql::escaped(E_)) + ",";
	str.resize(str.length() - 1);
	return str;
}
String sql::listQue(const intarray& list) { return toString(list, ","); }
String sql::listQue(const sobj& obj) {
	String str;
	auto& list = obj.array();
	sforeach(list) {
		if (E_.isNull()) str += "null,";
		else if (E_.isNum()) str += E_.toString() + ",";
		else str += String::squot(escaped(E_.toString())) + ",";
	}
	str.resize(str.length() - 1);
	return str;
}
String sql::selectQue(const sobj& obj) {
	String que;
	auto& sel = obj.dict();


	return que;
}
String sql::selectQuery(const char* name, const stringarray& cols, const char* condition,
	const char* order, const char* limit, bool distinct) {
	return String(distinct ? "SELECT DISTINCT " : "SELECT ") << toString(cols, ",") << " FROM " << name <<
		(condition ? condition : "") << (order ? order : "") << (limit ? limit : "");
}
String sql::joinedSelectQuery(sql::JOIN_TYPE type,
	const char* name1, const char* name2,
	const char* join, const stringarray& cols,
	const char* condition, const char* order,
	const char* limit, bool distinct) {
	String joinque = name1;
	switch (type) {
	case INNER_JOIN:
		joinque += " INNER JOIN ";
		break;
	case OUTER_JOIN:
		joinque += " LEFT OUTER JOIN ";
		break;
	case CROSS_JOIN:
		joinque += " CROSS JOIN ";
		break;
	default:
		break;
	}
	joinque << name2 << " ON " << join;
	return String(distinct ? "SELECT DISTINCT " : "SELECT ") << toString(cols, ",") << " FROM " <<
		joinque << (condition ? condition : "") << (order ? order : "") << (limit ? limit : "");
}

SDBTable::SDBTable(const char* t, SDataBase* d) {
	if (t) _table = t;
	if (d) _db = d;
}
SDBTable::~SDBTable() {}
bool SDBTable::exist() {
	if (_table.empty()) return false;
	_db->begin();
	_db->sqlprepare(String("SELECT COUNT(*) FROM sqlite_master WHERE type=\'table\' AND tbl_name=\'") << _table << "\'");
	auto& row = _db->getRow();
	_db->commit();
	return row.begin()->value == 1;
}
void SDBTable::create(const SArray& columns) {
	String colque = "(";
	if (columns.empty()) colque += ")";
	else {
		sforeach(columns) colque += sql::colInfo(E_) + ",";
		colque.last() = ')';
	}
	_db->begin();
	_db->sqlexec(String("CREATE TABLE IF NOT EXISTS ") << _table << colque);
	_db->commit();
}
void SDBTable::rename(const char* name) {
	_db->begin();
	_db->sqlexec(String("ALTER TABLE ") << _table << " RENAME TO " << name);
	_db->commit();
	_table = name;
}
void SDBTable::copy(const char* src, const stringarray& cols, const SDictionary& conditions) {
	auto src_tbl = _db->table(src);
	auto src_cols = src_tbl.columnInfo();
	SArray newcol;
	if (cols[0] == "*") newcol = src_cols;
	else {
		sforeach(src_cols) {
			if (cols.contain(E_["name"])) newcol.add(E_);
		}
	}
	_db->begin();
	_db->createTable(_table, newcol);
	_db->sqlexec(String("INSERT INTO ") << _table << "(" << sql::colNames(newcol) << ") SELECT " << sql::colNames(newcol) << " FROM " << src << sql::condQue(conditions));
	_db->commit();
}
void SDBTable::remove() {
	_db->begin();
	_db->sqlexec(String("DROP TABLE ") << _table);
	_db->commit();
}
stringarray SDBTable::columns() {
	stringarray cols;
	_db->begin();
	_db->sqlprepare("PRAGMA table_info('" + _table + "')");
	auto& rows = _db->getRows();
	sforeach(rows) cols.add(E_["name"]);
	_db->commit();
	return cols;
}
SArray& SDBTable::columnInfo() {
	_db->begin();
	_db->sqlprepare("PRAGMA table_info('" + _table + "')");
	auto& rows = _db->getRows();
	_db->commit();
	return rows;
}
void SDBTable::addColumn(int type, const char* name) {
	_db->begin();
	_db->sqlexec(String("ALTER TABLE ") << _table << " ADD COLUMN " << name << " " << sql::colTypeName(type));
	_db->commit();
}
void SDBTable::addColumn(const SDictionary& dict) {
	_db->begin();
	_db->sqlexec(String("ALTER TABLE ") << _table << " ADD COLUMN " << dict["name"] << " " << dict["type"]);
	_db->commit();
}
void SDBTable::addColumn(const sobj& obj) {
	if (obj.isDict()) addColumn(obj.dict());
	else if (obj.isColumn()) addColumn(obj.type(), obj.name());
}
void SDBTable::removeColumn(const char* name) {
	rename(_table + "_tmp");
	auto &cols = columnInfo();
	SArray newcol;
	sforeach(cols) {
		if (E_["name"] != name) newcol.add(E_);
	}
	_db->begin();
	_db->createTable(_table, newcol);
	_db->sqlexec(String("INSERT INTO ") << _table << "(" << sql::colNames(newcol) << ") SELECT " << sql::colNames(newcol) << " FROM " << _table << "_tmp");
	_db->removeTable(_table + "_tmp");
	_db->release();
	_db->commit();
}
void SDBTable::addRecord(const SArray& row) {
	String que = "(";
	if (row.empty()) que += ")";
	else {
		sforeach(row) {
			if (E_.isNull()) que += "null,";
			else if (E_.isNum()) que += E_ + ",";
			else que += String::squot(sql::escaped(E_)) + ",";
		}
		que.last() = ')';
	}
	_db->begin();
	_db->sqlexec(String("INSERT INTO ") << _table << " VALUES" << que);
	_db->commit();
}
void SDBTable::addRecord(const SDictionary& row) {
	String colque = "(", valque = " VALUES(";
	if (row.empty()) { colque += ")"; valque += ")"; }
	else {
		sforeach(row) {
			colque += E_.key + ",";
			if (E_.value.isNull()) valque += "null,";
			else if (E_.value.isNum()) valque += E_.value + ",";
			else valque += String::squot(sql::escaped(E_.value)) + ",";
		}
		colque.last() = ')';
		valque.last() = ')';
	}
	_db->begin();
	_db->sqlexec(String("INSERT INTO ") << _table << colque << valque);
	_db->commit();
}
void SDBTable::addRecord(const sobj& obj) {
	if (obj.isArray()) addRecord(obj.array());
	else if (obj.isDict()) addRecord(obj.dict());
}
void SDBTable::addRecordPrepare(size_t num) {
	String valque = "(";
	sforin(i, 0, num) valque += "?,";
	if (valque.size()) valque.last() = ')';
	_db->begin();
	_db->sqlprepare(String("INSERT INTO ") << _table << " VALUES" << valque);
}
void SDBTable::addRecordPrepare(const stringarray& cols) {
	String colque = "(", valque = " VALUES(";
	sforeach(cols) {
		colque += E_ + ",";
		valque += "?,";
	}
	colque.last() = ')';
	valque.last() = ')';
	_db->begin();
	_db->sqlprepare(String("INSERT INTO ") << _table << colque << valque);
}
void SDBTable::removeRecord(const char* condition) {
	_db->begin();
	_db->sqlexec(String("DELETE FROM ") << _table << (condition ? condition : ""));
	_db->commit();
}
void SDBTable::removeRecord(const SDictionary& condition) { removeRecord(sql::condQue(condition).cstr()); }
void SDBTable::removeRecordAt(size_t idx, const char* key) {
	_db->begin();
	_db->sqlexec(String("DELETE FROM ") << _table << "WHERE "<< key << "=" << idx);
	_db->commit();
}
void SDBTable::setRecord(const SDictionary& row, const char* condition) {
	String que;
	sforeach(row) {
		que += E_.key + "=";
		if (E_.value.isNull()) que += "NULL,";
		else if (E_.value.isNum()) que += E_.value + ",";
		else que += String::squot(sql:: escaped(E_.value)) + ",";
	}
	if (!que.empty()) que.resize(que.length() - 1);
	_db->begin();
	_db->sqlexec(String("UPDATE ") << _table << " SET " << que << (condition ? condition : ""));
	_db->commit();
}
void SDBTable::setRecordAt(size_t idx, const SDictionary& row, const char* key) {
	String que;
	sforeach(row) {
		que += E_.key + "=";
		if (E_.value.isNull()) que += "NULL,";
		else if (E_.value.isNum()) que += E_.value + ",";
		else que += String::squot(sql::escaped(E_.value)) + ",";
	}
	if (!que.empty()) que.resize(que.length() - 1);
	_db->begin();
	_db->sqlexec(String("UPDATE ") << _table << " SET " << que << "WHERE "<< key << "=" << idx);
	_db->commit();
}
void SDBTable::setRecordPrepare(const stringarray& cols, const char* condition) {
	String que;
	sforeach(cols) que += E_ + "=?,";
	que.resize(que.length() - 1);
	_db->begin();
	_db->sqlprepare(String("UPDATE ") << _table << " SET " << que << (condition ? condition : ""));
}
int SDBTable::count(const char* condition) {
	_db->begin();
	_db->sqlprepare(String("SELECT COUNT(*) FROM ") << _table << (condition ? condition : ""));
	auto& row = _db->getRow();
	_db->commit();
	return row.begin()->value;
}
SArray& SDBTable::countGroup(const stringarray& cols) {
	_db->begin();
	_db->sqlprepare(String("SELECT ") << toString(cols, ",") << ",COUNT(*) FROM " << _table << " GROUP BY " << toString(cols, ","));
	auto& rows = _db->getRows();
	_db->commit();
	return rows;
}
void SDBTable::getRecordPrepare(const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, limit, distinct));
}
STable& SDBTable::getTable(const stringarray& cols, const char* condition,
	const char* order, const char* limit, bool distinct,
	STable* result) {
	if (!result) {
		result = &_db->_result;
		result->clear();
	}
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, limit, distinct));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}

STable& SDBTable::getTable(const SDictionary& info, STable* result) {
	if (!result) {
		result = &_db->_result;
		result->clear();
	}
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table,
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		info["limit"] ? (const char*)info["limit"] : nullptr,
		info["distinct"]));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}
SDictionary& SDBTable::getRecord(const stringarray& cols,
	const char* condition,
	const char* order,
	SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, sql::limitQue(1), false));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getRecord(const SDictionary& info,
	SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table,
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		sql::limitQue(1), false));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getRecordAt(size_t idx, const stringarray& cols, const char* key, SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, String(" WHERE ID=") << idx, nullptr, sql::limitQue(1)));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SArray& SDBTable::getRecords(const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct,
	SArray* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, limit, distinct));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}
SArray& SDBTable::getRecords(const SDictionary& info,
	SArray* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table,
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		info["limit"] ? (const char*)info["limit"] : nullptr,
		info["distinct"]));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}

void SDBTable::getJoinedRecordPrepare(sql::JOIN_TYPE type,
	const char* table2,
	const char* join,
	const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct) {
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(table2), join, cols, condition, order, limit, distinct));
}
STable& SDBTable::getJoinedTable(sql::JOIN_TYPE type,
	const char* table2,
	const char* join,
	const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct,
	STable* result) {
	if (!result) {
		result = &_db->_result;
		result->clear();
	}
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(table2), join, cols, condition, order, limit, distinct));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}
STable& SDBTable::getJoinedTable(const SDictionary& info, STable* result) {
	if (!result) {
		result = &_db->_result;
		result->clear();
	}
	sql::JOIN_TYPE type;
	if (info["mode"] == "inner") type = sql::INNER_JOIN;
	else if (info["mode"] == "outer") type = sql::OUTER_JOIN;
	else if (info["mode"] == "cross") type = sql::CROSS_JOIN;
	else return getTable(info, result);

	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(info["table"]),
		info["join"] ? info["join"] : "",
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		info["limit"] ? (const char*)info["limit"] : nullptr,
		info["distinct"]));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}
SDictionary& SDBTable::getJoinedRecord(sql::JOIN_TYPE type,
	const char* table2,
	const char* join,
	const stringarray& cols,
	const char* condition,
	const char* order,
	SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(table2), join, cols, condition, order, sql::limitQue(1)));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getJoinedRecord(const SDictionary& info, SDictionary* result) {
	sql::JOIN_TYPE type;
	if (info["mode"] == "inner") type = sql::INNER_JOIN;
	else if (info["mode"] == "outer") type = sql::OUTER_JOIN;
	else if (info["mode"] == "cross") type = sql::CROSS_JOIN;
	else return getRecord(info, result);
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(info["table"]),
		info["join"] ? info["join"] : "",
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		sql::limitQue(1)));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getJoinedRecordAt(size_t idx,
	sql::JOIN_TYPE type,
	const char* table2,
	const char* join,
	const stringarray& cols,
	SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(table2), join, cols,
		String("WHERE ID=") << idx, nullptr, sql::limitQue(1)));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}

SArray& SDBTable::getJoinedRecords(sql::JOIN_TYPE type,
	const char* table2,
	const char* join,
	const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct,
	SArray* result) {
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(table2), join, cols, condition, order, limit, distinct));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}
SArray& SDBTable::getJoinedRecords(const SDictionary& info, SArray* result) {
	sql::JOIN_TYPE type;
	if (info["mode"] == "inner") type = sql::INNER_JOIN;
	else if (info["mode"] == "outer") type = sql::OUTER_JOIN;
	else if (info["mode"] == "cross") type = sql::CROSS_JOIN;
	else return getRecords(info, result);
	_db->begin();
	_db->sqlprepare(sql::joinedSelectQuery(type, _table, sql::escaped(info["table"]),
		info["join"] ? info["join"] : "",
		info["column"] ? info["column"].split(",") : stringarray({ "*" }),
		info["condition"] ? (const char*)info["condition"] : nullptr,
		info["order"] ? (const char*)info["order"] : nullptr,
		info["limit"] ? (const char*)info["limit"] : nullptr,
		info["distinct"]));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}

SDataBase::SDataBase() {
	_db = nullptr;
	_stmt = nullptr;
	_open = false;
	_transaction = false;
	_res = 0;
}
SDataBase::SDataBase(const char* path) : SDataBase() { open(path); }
SDataBase::~SDataBase() {
	if (isTransacting()) commit();
	if (isOpened()) close();
}
void SDataBase::open(const char* path) {
	if (_open) close();
	_res = sqlite3_open_v2(path, &_db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, 0);
	if (_res != SQLITE_OK) throw SDBException(ERR_INFO, sio::FILE_OPEN_ERROR, path);
	_path = path;
	_open = true;
}
void SDataBase::close() {
	if (_stmt) sqlite3_finalize(_stmt);
	if (_db) sqlite3_close(_db);
	_open = false;
}
bool SDataBase::isOpened() const { return _open; }
bool SDataBase::isTransacting() const { return _transaction; }
const char* SDataBase::path() const { return _path; }
int SDataBase::tableCount() {
	sqlprepare("SELECT COUNT(*) FROM sqlite_master WHERE type=\'table\'");
	auto& row = getRow();
	return row.begin()->value;
}
sobj SDataBase::tables() {
	sarray tbls;
	auto& array = table("sqlite_master").getRecords({ "tbl_name" }, sql::condition("type=\'table\'"));
	if (!array.empty()) {
		sforeach(array) tbls.add(E_["tbl_name"]);
	}
	return tbls;
}
void SDataBase::createTable(const char* name, const SArray& columns, const SArray& rows) {
	String que = "CREATE TABLE IF NOT EXISTS " + String(name ? name : "table");
	if (columns.size()) {
		que << "(" << sql::colInfos(columns) << ")";
		sqlexec(que);
		if (rows.size()) {
			auto sqltbl = table(name ? name : "table");
			sqltbl.addRecordPrepare(columns.size());
			sforeach(rows) {
				bindRow(E_);
				step();
				reset();
			}
			commit();
		}
	}
	else {
		que << "()";
		sqlexec(que);
	}
	
	String tblname = name == nullptr ? "table" : name, colque = "(";
	if (!columns.empty()) {
		sforeach(columns) {
			if (E_.isColumn()) colque += sql::escaped(E_.name()) + " " + sql::colTypeName(E_.type()) + ",";
			else if (E_.isDict()) colque += sql::escaped(E_["name"]) + " " + E_["type"] + ",";
		}
	}
	sqlexec(String("CREATE TABLE IF NOT EXISTS ") << tblname << colque);
}
inline void SDataBase::_createTable(const SDictionary& dic) {
	sint colnum = 0;
	String que = "CREATE TABLE IF NOT EXISTS " + (dic.hasKey("name") ? dic["name"] : "table");
	if (dic.hasKey("columns") && (colnum = dic["columns"].size())) {
		que << "(" << sql::colInfos(dic["columns"].array()) << ")";
		sqlexec(que);
		if (dic.hasKey("rows") && dic["rows"].size()) {
			auto sqltbl = table(dic.hasKey("name") ? dic["name"] : "table");
			sqltbl.addRecordPrepare(colnum);
			sforeach(dic["rows"]) {
				bindRow(E_);
				step();
				reset();
			}
			commit();
		}
	}
	else {
		que << "()";
		sqlexec(que);
	}
}
inline void SDataBase::_createTable(const STable& tbl) {
	String que = "CREATE TABLE IF NOT EXISTS " + tbl.name();
	if (tbl.columnCount()) {
		que << "(" << sql::colInfos(tbl.columns()) << ")";
		sqlexec(que);
		if (tbl.rowCount()) {
			auto sqltbl = table(tbl.name());
			sqltbl.addRecordPrepare(tbl.columnCount());
			sforeach(tbl.rows()) {
				bindRow(E_);
				step();
				reset();
			}
			commit();
		}
	}
	else {
		que << "()";
		sqlexec(que);
	}
}
void SDataBase::createTable(const sobj& obj) {
	if (obj.isDict()) createTable(obj.dict());
	else if (obj.isTable()) createTable(obj.table());
	else sqlexec(obj);
}
void SDataBase::removeTable(const char* name) { table(name).remove(); }
void SDataBase::clearTables() {
	if (tableCount()) {
		auto list = tables();
		sforeach(list) table(E_).remove();
		release();
	}
}
SDBTable SDataBase::table(const char* name) {
	return SDBTable(name, this);
}
SDBTable SDataBase::operator[](const char* name) {
	return SDBTable(name, this);
}
void SDataBase::bindi(sint val, int i) {
	_res = sqlite3_bind_int(_stmt, i, val);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_int", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindl(sinteger val, int i) {
	_res = sqlite3_bind_int64(_stmt, i, val);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_int64", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindd(double val, int i) {
	_res = sqlite3_bind_double(_stmt, i, val);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_double", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindt(const char* val, int i) {
	_res = sqlite3_bind_text(_stmt, i, val, -1, SQLITE_STATIC);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_text", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindtt(const char* val, int i) {
	_res = sqlite3_bind_text(_stmt, i, val, -1, SQLITE_TRANSIENT);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_text", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindb(void* val, int size, int i) {
	_res = sqlite3_bind_blob(_stmt, i, val, size, SQLITE_STATIC);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_blob", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindbt(void* val, int size, int i) {
	_res = sqlite3_bind_blob(_stmt, i, val, size, SQLITE_TRANSIENT);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_blob", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bindNull(int i) {
	_res = sqlite3_bind_null(_stmt, i);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_bind_null", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::bind(const sobj& obj, int i) {
	if (obj.isNull()) bindNull(i);
	else if (obj.isNum()) {
		int type = obj.number().type();
		if (type == SNumber::INTEGER || type == SNumber::UINTEGER) bindl(obj.integer(), i);
		else if (type == SNumber::REAL) bindd(obj.real(), i);
		else bindtt(obj->toString(), i);
	}
	else if (obj.isStr()) bindt(obj.string(), i);
	else if (obj.isDat()) bindb(obj.data().ptr(), obj.size(), i);
	else bindtt(obj->toString(), i);
}
void SDataBase::bindRow(const sobj& obj) {
	int i = 1;
	auto& array = obj.array();
	sforeach(array) { bind(E_, i); ++i; }
}
void SDataBase::sqlexec(const char* sql) {
	_res = sqlite3_exec(_db, sql, NULL, NULL, &_err);
	if (_res != SQLITE_OK) {
		auto count = 0;
		while (count < 1000) {
			_res = sqlite3_exec(_db, sql, NULL, NULL, &_err);
			if (_res == SQLITE_OK) break;
			++count;
		}
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_exec", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
	}
}
void SDataBase::sqlprepare(const char* sql) {
	reset();
	_res = sqlite3_prepare_v2(_db, sql, -1, &_stmt, NULL);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_prepare_v2", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::reset() {
	if (_stmt) _res = sqlite3_reset(_stmt);
	if (_res != SQLITE_OK)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_reset", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
void SDataBase::begin() {
	if (!_transaction) sqlexec("BEGIN");
	_transaction = true;
}
void SDataBase::commit() {
	if (_transaction) sqlexec("COMMIT");
	_transaction = false;
}
void SDataBase::release() {
	begin();
	sqlexec("VACUUM");
	commit();
}
void SDataBase::step() {
	if ((_res = sqlite3_step(_stmt)) != SQLITE_DONE)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_step", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
STable& SDataBase::getResult(STable* table) {
	if (!table) table = &_result;
	table->clearRows();
	int r, c;
	while ((_res = sqlite3_step(_stmt)) == SQLITE_ROW) {
		r = table->rowCount();
		table->addRow();
		auto size = sqlite3_column_count(_stmt);
		sforin(i, 0, size) {
			auto type = sqlite3_column_type(_stmt, i);
			c = table->columnIndex(sqlite3_column_name(_stmt, i));
			switch (type) {
			case SQLITE_INTEGER:
				table->setValue(r, c, sqlite3_column_int(_stmt, i));
				break;
			case SQLITE_FLOAT:
				table->setValue(r, c, sqlite3_column_double(_stmt, i));
				break;
			case SQLITE_TEXT:
				table->setValue(r, c, (const char*)sqlite3_column_text(_stmt, i));
				break;
			case SQLITE_BLOB:
			{
				SData tmp(sqlite3_column_bytes(_stmt, i), '\0');
				memcpy(&tmp[0], sqlite3_column_blob(_stmt, i), tmp.size());
				table->setValue(r, c, tmp);
				break;
			}
			case SQLITE_NULL:
				table->setValue(r, c, snull);
				break;
			default:
				break;
			}
		}
	}
	if (_res != SQLITE_DONE)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_step", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
	return *table;
}
SDictionary& SDataBase::getRow(SDictionary* dict) {
	if (!dict) dict = &_row;
	dict->clear();
	if ((_res = sqlite3_step(_stmt)) == SQLITE_ROW) {
		auto size = sqlite3_column_count(_stmt);
		sforin(i, 0, size) {
			auto type = sqlite3_column_type(_stmt, i);
			auto name = sqlite3_column_name(_stmt, i);
			switch (type) {
			case SQLITE_INTEGER:
				dict->set(name, sqlite3_column_int(_stmt, i));
				break;
			case SQLITE_FLOAT:
				dict->set(name, sqlite3_column_double(_stmt, i));
				break;
			case SQLITE_TEXT:
				dict->set(name, (const char*)sqlite3_column_text(_stmt, i));
				break;
			case SQLITE_BLOB:
			{
				int size = sqlite3_column_bytes(_stmt, i);
				SData tmp(size, '\0');
				memcpy(&tmp[0], sqlite3_column_blob(_stmt, i), tmp.size());
				dict->set(name, tmp);
				break;
			}
			case SQLITE_NULL:
				dict->set(name, snull);
				break;
			default:
				break;
			}
		}
	}
	return *dict;
}
SArray& SDataBase::getRows(SArray* array) {
	if (!array) array = &_rows;
	array->clear();
	getRow();
	while (_res == SQLITE_ROW) {
		array->add(_row);
		getRow();
	}
	if (_res != SQLITE_DONE)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_step", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
	return *array;
}
String SDataBase::getClass() const { return "database"; }
String SDataBase::toString() const { return String("db://") + path(); }
SObject* SDataBase::clone() const { 
	SDataBase* db = new SDataBase();
	if (isOpened()) db->open(path());
	return db;
}