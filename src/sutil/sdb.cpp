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

search_query::search_query() : operation(0) {}
search_query::search_query(const char* k, suint op, const sobj& v) : key(k), operation(op), value(v) {}
search_query::search_query(const search_query& que) : key(que.key), operation(que.operation), value(que.value) {}
search_query::~search_query() {}
search_query& search_query::operator=(const search_query& que) {
	key = que.key; operation = que.operation; value = que.value; return *this;
}
search_sorter::search_sorter() : order(DESC) {}
search_sorter::search_sorter(const char* k, slib::ORDER o) : key(k), order(o) {}
search_sorter::search_sorter(const search_sorter& sorter) : key(sorter.key), order(sorter.order) {}
search_sorter::~search_sorter() {}
search_sorter& search_sorter::operator=(const search_sorter& sorter) {
	key = sorter.key; order = sorter.order; return *this;
}
SSearchQuery::SSearchQuery() : _andor(false) {}
SSearchQuery::~SSearchQuery() {}
void SSearchQuery::addQuery(const search_query& que) {
	if (_andor) _queries.add({ que });
	else {
		if (_queries.empty())  _queries.add({ que });
		else _queries.last().add(que);
	}
}
void SSearchQuery::andQuery() { _andor = false; }
void SSearchQuery::orQuery() { _andor = true; }
void SSearchQuery::setQueries(SDictionary& que) {

}
void SSearchQuery::addSorter(const search_sorter& sorter) { _sorters.add(sorter); }
void SSearchQuery::addKey(const char* key) { _keys.add(key); }
void SSearchQuery::addKeys(const stringarray& keys) { _keys.append(keys); }
void SSearchQuery::setKeys(SArray& keys) { sforeach(keys) _keys.add(E_); }
void SSearchQuery::setOffset(suinteger i) { _range.begin = i; }
void SSearchQuery::setLimit(suinteger i) { _range.end = _range.begin + i; }
void SSearchQuery::setRange(Range<suinteger> r) { _range = r; }
void SSearchQuery::setConditions(SDictionary& cond) {
	/*
	*/
}
String SSearchQuery::toString(DB_MODE m) const {
	String str;
	switch (m)
	{
	case SQLITE_DB:
	{

		break;
	}
	default:
		break;
	}
	return str;
}

String sql::colTypeName(int type) {
    String name = String::upper(SColumn::colTypeStr(type&0x0FFF));
    if(type & KEY_COLUMN) name += " PRIMARY KEY";
    if(type & UNIQUE_COLUMN) name += " UNIQUE";
    if(type & AUTO_INCREMENT_COLUMN) name += " AUTOINCREMENT";
    return name;
}
String sql::colInfo(const SColumn &col) {
	return col.name() + " " + sql::colTypeName(col.type());
}
String sql::colInfos(const Array<SColumn>& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) str << colInfo(E_) << ",";
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::colInfos(const SArray& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) str << colInfo(E_.column()) << ",";
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::colNames(const Array<SColumn>& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) { str << E_.name() << ","; }
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::colNames(const SArray& cols) {
	String str;
	if (cols.empty()) return str;
	sforeach(cols) { str << E_["name"] << ","; }
	if (str.length()) str.resize(str.length() - 1);
	return str;
}
String sql::escaped(const char *que) { String str(que); str.replace("\'", "\'\'"); return str; }
String sql::value(sobj obj, bool like, subyte match) {
	if (obj.isNull()) return "NULL";
	else if (obj.isNum()) return obj.toString();
	else {
		if (!match) return String::dquot(escaped(obj));
		else {
			String que = escaped(obj);
			if (match & 0x01) que = (like ? "%" : "*") + que;
			if (match & 0x02) que << (like ? "%" : "*");
			return String::squot(que);
		}
	}
}
inline void _addCondQue(String& str, sobj obj) {
	if (obj["op"] == "in") str << (obj["not"] ? " NOT IN (" : " IN (") << sql::listQue(obj["val"]) << ")";
	else if (obj["op"] == "b/w") str << obj["col"] << (obj["not"] ? " NOT BETWEEN " : " BETWEEN ") << obj["val"][0] << " AND " << obj["val"][1];
	else if (obj["op"] == "like") str << obj["col"] << " LIKE " << sql::value(obj["val"], true, obj["match"]);
	else if (obj["op"] == "glob") str << obj["col"] << " GLOB " << sql::value(obj["val"], false, obj["match"]);
	else str << obj["col"] << " " << obj["op"] << " " << sql::value(obj["val"]);
}
inline void _addLogicCondQue(String& str, sobj obj) {
	if (obj["and"]) {
		sforeach(obj["and"]) {
			str << " (";
			_addLogicCondQue(str, E_);
			str << ") AND";
		}
		str.resize(str.length() - 4);
	}
	else if (obj["or"]) {
		sforeach(obj["or"]) {
			str << " (";
			_addLogicCondQue(str, E_);
			str << ") OR";
		}
		str.resize(str.length() - 3);
	}
	else if (obj["not"]) {
		str << " NOT (";
		_addLogicCondQue(str, obj["NOT"]);
		str << ")";
	}
	else _addCondQue(str, obj);
}
String sql::condQue(const sobj& conds, bool join) {
	String que = join?" ON ":" WHERE ";
	_addLogicCondQue(que, conds);
	return que;
}
String sql::caseBy(const char* name, const sattribute& attr, const char* exception, const char* as) {
	String que;
	que << " CASE " << name;
	for (auto it = attr.begin(); it != attr.end(); ++it)
		que << " WHEN " << it->key << " THEN " << it->value;
	if (exception) que << " ELSE " << exception;
	que << " END";
	if (as) que << " AS " << as;
	return que;
}
inline void _addCaseQue(String& str, const sobj& obj) {
	str << " WHEN ";
	_addCondQue(str, obj);
	str << " THEN " << sql::value(obj["res"]);
}
String sql::caseQue(const sobj& obj) {
	String que;
	auto& cases = obj.dict();
	que << " CASE";
	if (cases.hasKey("case")) que << " " << cases["case"];
	if (cases.hasKey("if")) _addCaseQue(que, cases["if"]);
	else if (cases.hasKey("ifs")) {
		sforeach(cases["ifs"]) _addCaseQue(que, E_);
	}
	if (cases.hasKey("else")) que << " ELSE " << cases["else"];
	que << " END";
	if (cases.hasKey("as")) que << " AS " << cases["as"];
	return que;
}
String sql::order(const Array<std::pair<String, ORDER>>& orders) {
	if (orders.empty()) return "";
	String oque;
	sforeach(orders) oque << E_.first << " " << (E_.second == ASC ? "ASC" : "DESC") << ",";
	oque.resize(oque.length() - 1);
	return String(" ORDER BY ") << oque;
}
String sql::orderQue(const sobj& obj) {
	auto& orders = obj.dict();
	if (orders.empty()) return "";
	String que;
	sforeach(orders) que << E_.key << " " << (E_.value.isNum()?String(E_.value == (int)ASC?"ASC":"DESC"):E_.value.toString()) << ",";
	que.resize(que.length() - 1);
	return String(" ORDER BY ") << que;
}
String sql::limit(int l, int o) {
	if (0 < l) {
		if (o < 0) return String(" LIMIT ") << l;
		else return String(" LIMIT ") << o << "," << l;
	}
	else if (-1 < o) return String(" OFFSET ") << o;
	else return "";
}
String sql::limitQue(const sobj& obj) {
	auto& lim = obj.dict();
	if (lim.hasKey("lim")) {
		if (lim.hasKey("off")) return String(" LIMIT ") << lim["off"] << "," << lim["lim"];
		else return String(" LIMIT ") << lim["lim"];
	}
	else if (lim.hasKey("off")) return String(" OFFSET ") << lim["off"];
	else return "";
}
String sql::strListQue(const stringarray& list) {
	String str;
	sforeach(list) str += String::squot(sql::escaped(E_)) + ",";
	str.resize(str.length() - 1);
	return str;
}
String sql::intListQue(const intarray& list) { return toString(list, ","); }
String sql::listQue(const sobj& obj) {
	String que;
	sforeach(obj) que << sql::value(E_) << ",";
	que.resize(que.length() - 1);
	return que;
}
String sql::selectQue(const char *tbl, const SDictionary& sel) {
	String que = "SELECT";
	if (sel.hasKey("distinct") && sel["distinct"]) que << " DISTINCT";
	if (sel.hasKey("cols")) que << " "<< slib::toString(sel["cols"].array(), ",");
	else que << " *";
	if (sel.hasKey("join")) que << " FROM " << sql::joinQue(tbl, sel["join"]);
	else que << " FROM " << tbl;
	if (sel.hasKey("case")) que << sql::caseQue(sel["case"]);
	if (sel.hasKey("condition")) que << sql::condQue(sel["condition"]);
	if (sel.hasKey("order")) que << sql::orderQue(sel["order"]);
	if (sel.hasKey("limit")) que << sql::limitQue(sel["limit"]);
	return que;
}
String sql::joinQue(const char* tbl, const sobj& obj) {
	String que = tbl;
	switch (obj["type"].ubyteValue()) {
	case INNER_JOIN:
		que << " INNER JOIN ";
		break;
	case OUTER_JOIN:
		que << " LEFT OUTER JOIN ";
		break;
	case CROSS_JOIN:
		que << " CROSS JOIN ";
		break;
	default:
		break;
	}
	que << obj["tbl2"] << " ON " << sql::condQue(obj["condition"]);
	return que;
}
String sql::selectQuery(const char* name, const stringarray& cols, const char* condition,
	const char* order, const char* limit, bool distinct) {
	return String(distinct ? "SELECT DISTINCT " : "SELECT ") << toString(cols, ",") << " FROM " << name <<
		(condition ? condition : "") << (order ? order : "") << (limit ? limit : "");
}
String sql::joinedSelectQuery(subyte type,
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
void SDBTable::create(sobj obj) {
	String que = "CREATE TABLE IF NOT EXISTS " + _table;
	que << "(";
	if (obj["columns"]) que << sql::colInfos(obj["columns"].array()) << ")";
	else que << ")";
	_db->begin();
	_db->sqlexec(que);
	if (obj["rows"]) {
		addRecordPrepare(obj["columns"].size());
		sforeach(obj["rows"]) {
			_db->bindRow(E_);
			_db->step();
			_db->reset();
		}
		_db->commit();
	}
	_db->commit();
}
void SDBTable::rename(const char* name) {
	_db->begin();
	_db->sqlexec(String("ALTER TABLE ") << _table << " RENAME TO " << name);
	_db->commit();
	_table = name;
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
void SDBTable::addColumn(const SDictionary& col) {
	_db->begin();
	_db->sqlexec(String("ALTER TABLE ") << _table << " ADD COLUMN " << col["name"] << " " << (col["type"].isNum() ? sql::colTypeName(col["type"]) : col["type"].toString()));
	_db->commit();
}
void SDBTable::removeColumn(const char* name) {
	String ori = _table;
	rename(_table + "_tmp");
	auto &cols = columnInfo();
	Array<SColumn> newcol;
	sforeach(cols) {
		if (E_["name"] != name) newcol.add(SColumn(E_["type"].intValue(), E_["name"]));
	}
	_db->begin();
	_db->createTable(ori.cstr(), newcol);
	_db->sqlexec(String("INSERT INTO ") << ori << "(" << sql::colNames(newcol) << ") SELECT " << sql::colNames(newcol) << " FROM " << _table);
	_db->sqlexec(String("DROP TABLE ") << _table);
	_db->commit();
	_db->release();

}
void SDBTable::addRecordArray(const SArray& row) {
	String que = "INSERT INTO " + _table;
	que << " VALUES(";
	if (row.empty()) que << ")";
	else {
		sforeach(row) que << sql::value(E_) << ",";
		que.last() = ')';
	}
	_db->begin();
	_db->sqlexec(que);
	_db->commit();
}
void SDBTable::addRecordDict(const SDictionary& row) {
	String colque = "(", valque = " VALUES(";
	if (row.empty()) { colque << ")"; valque << ")"; }
	else {
		sforeach(row) {
			colque << E_.key << ",";
			valque << sql::value(E_.value) << ",";
		}
		colque.last() = ')';
		valque.last() = ')';
	}
	_db->begin();
	_db->sqlexec(String("INSERT INTO ") << _table << colque << valque);
	_db->commit();
}
void SDBTable::addRecord(const sobj& row) {
	if (row.isArray()) addRecordArray(row.array());
	else if (row.isDict()) addRecordDict(row.dict());
}
void SDBTable::addRecordPrepare(size_t num) {
	String valque = "(";
	sforin(i, 0, num) valque << "?,";
	if (valque.size()) valque.last() = ')';
	_db->begin();
	_db->sqlprepare(String("INSERT INTO ") << _table << " VALUES" << valque);
}
void SDBTable::addRecordPrepare(const SArray& cols) {
	String colque = "(", valque = " VALUES(";
	sforeach(cols) {
		colque << E_ << ",";
		valque << "?,";
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
void SDBTable::removeRecordAt(size_t idx, const char* key) {
	_db->begin();
	_db->sqlexec(String("DELETE FROM ") << _table << " WHERE "<< key << "=" << idx);
	_db->commit();
}
void SDBTable::setRecord(const sobj& obj) { setRecord(obj["data"], obj.hasKey("condition")?sql::condQue(obj["condition"]):nullptr); }
void SDBTable::setRecord(const SDictionary& row, const char* condition) {
	String que = "UPDATE " + _table + " SET ";
	sforeach(row) que << E_.key << "=" << sql::value(E_.value) << ",";
	if (!row.empty()) que.resize(que.length() - 1);
	_db->begin();
	_db->sqlexec(que << (condition ? condition : ""));
	_db->commit();
}
void SDBTable::setRecordAt(size_t idx, const SDictionary& row, const char* key) {
	String que = "UPDATE " + _table + " SET ";
	sforeach(row) que << E_.key << "=" << sql::value(E_.value) << ",";
	if (!row.empty()) que.resize(que.length() - 1);
	_db->begin();
	_db->sqlexec(que << " WHERE "<< key << "=" << idx);
	_db->commit();
}
void SDBTable::setRecordPrepare(const SArray& cols, const char* key) {
	String que = "UPDATE " + _table + " SET ";
	sforeach(cols) que << E_ << "=?,";
	if (!cols.empty()) que.resize(que.length() - 1);
	_db->begin();
	_db->sqlprepare(que << " WHERE " << key << "=?");
}
int SDBTable::count(const char* condition) {
	_db->begin();
	_db->sqlprepare(String("SELECT COUNT(*) FROM ") << _table << (condition ? condition : ""));
	auto& row = _db->getRow();
	_db->commit();
	return row.begin()->value;
}
SArray& SDBTable::countGroup(const SArray& cols) {
	auto cname = sql::colNames(cols);
	_db->begin();
	_db->sqlprepare(String("SELECT ") << cname << ",COUNT(*) FROM " << _table << " GROUP BY " << cname);
	auto& rows = _db->getRows();
	_db->commit();
	return rows;
}
SDictionary& SDBTable::getRecord(const stringarray& cols,
	const char* condition,
	const char* order,
	SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, sql::limit(1), false));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getRecord(const SDictionary& info, SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQue(_table, info));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
SDictionary& SDBTable::getRecordAt(size_t idx, const stringarray& cols, const char* key, SDictionary* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, String(" WHERE ID=") << idx, nullptr, sql::limit(1)));
	auto& dict = _db->getRow(result);
	_db->commit();
	return dict;
}
void SDBTable::getRecordPrepare(const stringarray& cols,
	const char* condition,
	const char* order,
	const char* limit,
	bool distinct) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, limit, distinct));
}
void SDBTable::getRecordPrepare(const SDictionary& info) {
	_db->begin();
	_db->sqlprepare(sql::selectQue(_table, info));
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
	_db->sqlprepare(sql::selectQue(_table, info));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}
STable& SDBTable::getTable(const stringarray& cols, const char* condition,
	const char* order, const char* limit, bool distinct,
	STable* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQuery(_table, cols, condition, order, limit, distinct));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}
STable& SDBTable::getTable(const SDictionary& info, STable* result) {
	_db->begin();
	_db->sqlprepare(sql::selectQue(_table, info));
	auto& table = _db->getResult(result);
	_db->commit();
	return table;
}
SArray& SDBTable::getJoinedRecords(subyte type,
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
	_db->begin();
	_db->sqlprepare(sql::selectQue(_table, info));
	auto& array = _db->getRows(result);
	_db->commit();
	return array;
}
void SDBTable::getJoinedRecordPrepare(subyte type,
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
void SDBTable::getJoinedRecordPrepare(const SDictionary& info) {
	_db->begin();
	_db->sqlprepare(sql::selectQue(_table, info));
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
void SDataBase::createTable(const char* name, const Array<SColumn>& columns, const SArray& rows) {
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
}
void SDataBase::createTable(const SDictionary& dic) {
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
void SDataBase::createTable(const char* name, const STable& tbl) {
	String que = String("CREATE TABLE IF NOT EXISTS ") + name;
	if (tbl.columnCount()) {
		que << "(" << sql::colInfos(tbl.columns()) << ")";
		sqlexec(que);
		if (tbl.rowCount()) {
			auto sqltbl = table(name);
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
void SDataBase::release() { sqlexec("VACUUM"); }
void SDataBase::step() {
	if ((_res = sqlite3_step(_stmt)) != SQLITE_DONE)
		throw SDBException(ERR_INFO, SQL_ERROR, "sqlite3_step", SQL_ERR_TEXT(_res, sqlite3_errmsg(_db)));
}
STable& SDataBase::getResult(STable* table) {
	if (!table) table = &_result;
	table->clearRows();
	SDictionary row;
	while ((_res = sqlite3_step(_stmt)) == SQLITE_ROW) {
		auto size = sqlite3_column_count(_stmt);
		bool col = size == table->columnCount();
		if (!col) table->clearAll();
		sforin(i, 0, size) {
			auto name = sqlite3_column_name(_stmt, i);
			auto type = sqlite3_column_type(_stmt, i);
			switch (type) {
			case SQLITE_INTEGER:
			{
				row[name] = sqlite3_column_int(_stmt, i);
				if (!col) table->addColumn(SColumn(INTEGER_COLUMN, name));
				break;
			}
			case SQLITE_FLOAT:
				row[name] = sqlite3_column_double(_stmt, i);
				if (!col) table->addColumn(SColumn(REAL_COLUMN, name));
				break;
			case SQLITE_TEXT:
				row[name] = (const char*)sqlite3_column_text(_stmt, i);
				if (!col) table->addColumn(SColumn(TEXT_COLUMN, name));
				break;
			case SQLITE_BLOB:
			{
				SData tmp(sqlite3_column_bytes(_stmt, i), nullptr);
				memcpy(&tmp[0], sqlite3_column_blob(_stmt, i), tmp.size());
				row[name] = tmp;
				if (!col) table->addColumn(SColumn(DATA_COLUMN, name));
				break;
			}
			case SQLITE_NULL:
				row[name] = snull;
				if (!col) table->addColumn(SColumn(INTEGER_COLUMN, name));
				break;
			default:
				break;
			}
			table->addRow();
			table->updateRow(table->rowCount() - 1, row);
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
				SData tmp(size, nullptr);
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
