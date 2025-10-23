#include "sobj/snumber.h"
#include "sobj/sstring.h"
#include "sobj/sdate.h"
#include "sobj/sdata.h"
#include "sobj/sarray.h"
#include "sobj/sdict.h"
#include "sutil/sdb.h"
#include "sutil/sjson.h"

slib::SDBException::SDBException() : Exception() { prefix = "DB"; }
slib::SDBException::SDBException(int i, const char* msg, const char* desc) : Exception(i, msg, desc) { prefix = "DB"; }
slib::SDBException::~SDBException() {}

void slib::joinSchema(slib::SDataBase& db, const stringarray& names, slib::SDictionary& schema) {
	sfor(names) {
		auto dic = db.schema($_);
		sforeach(elem, dic) schema.set($_ + "." + elem.key(), elem.value());
	}
}
slib::SDBTable::SDBTable() : STable() {
	_db = nullptr;
	_prepare = false;
}
slib::SDBTable::SDBTable(const char* s, SDataBase *db) : SDBTable() {
	SArray rules;
	stringarray types;
	name = s;
	_db = db;
	_schema = _db->schema(name);
}
slib::SDBTable::~SDBTable() { name.clear(); _db = nullptr; }
const slib::SDictionary &slib::SDBTable::schema() const { return _schema; }
slib::SDBTable& slib::SDBTable::prepare() { _prepare = true; return *this; }
void slib::SDBTable::complete() { 
	_db->commit(); 
	reset();
	_prepare = false;
}

slib::SDBTable& slib::SDBTable::rename(const char* s) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	if (!s) throw NullException("Targe name");
	_db->exec(sdb::renameTableQuery(name, s));
	name = s;
	return *this;
}
void slib::SDBTable::drop() {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	_db->exec(sdb::dropTableQuery(name));
	reset(); name.clear(); _db = nullptr;
}

slib::SDBTable& slib::SDBTable::insert(const SObjPtr& obj, bool returning, const char* retkeys) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	if (obj.isNull() || obj.empty()) {
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, _schema.size(), returning, retkeys));
		if (_prepare) return *this;
		sfori(_schema) { _db->bindNull(i + 1); }
		_db->step(this, &_schema);
		_db->reset();
	}
	else if (obj.isArray()) {
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, obj.size(), returning, retkeys));
		if (_prepare) return *this;
		_db->bindValues(obj.array());
		_db->step(this, &_schema);
		_db->reset();
	}
	else if (obj.isDict()) {
		auto keys = obj.keyset();
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, keys, returning, retkeys));
		if (_prepare) return *this;
		_db->bindValues(obj.dict(), keys);
		_db->step(this, &_schema);
		_db->reset();
	}
	else throw Exception();
	if (_db->_trans) _db->commit();
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::insertAll(const SArray& array, bool returning, const char* retkeys) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	if (array.empty()) throw Exception();
	if (array[0].isNull() || array[0].empty()) {
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, _schema.size(), returning, retkeys));
		sfori(array) {
			sforin(j, 0_u, _schema.size()) { _db->bindNull(j + 1); }
			_db->step(this, &_schema);
			_db->reset();
		}
	}
	else if (array[0].isArray()) {
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, array[0].size(), returning, retkeys));
		sfor(array) {
			_db->bindValues($_.array());
			_db->step(this, &_schema);
			_db->reset();
		}
	}
	else if (array[0].isDict()) {
		auto keys = array[0].keyset();
		_db->begin();
		_db->prepare(sdb::insertPrepareQuery(name, keys, returning, retkeys));
		sfor(array) {
			_db->bindValues($_.dict(), keys);
			_db->step(this, &_schema);
			_db->reset();
		}
	}
	else throw Exception();
	if (_db->_trans) _db->commit();
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::addRecord() {
	_db->bindNull(1);
	_db->step(this, &_schema);
	_db->reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::addRecord(const SObjPtr& obj) {
	if (obj) {
		if (obj.isArray()) _db->bindValues(obj.array());
		else if (obj.isDict()) {
			auto keys = obj.keyset();
			_db->bindValues(obj.dict(), keys);
		}
	}
	else _db->bindNull(1);
	_db->step(this, &_schema);
	_db->reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::addRecords(const SArray& array) {
	sfor(array) { addRecord($_); }
	return *this;
}
slib::SDBTable& slib::SDBTable::update(const stringarray &keys) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database object is null.");
	if (keys.empty()) {
		_db->begin();
		_db->prepare(sdb::updatePrepareQuery(name, _schema.keyset(), _condition));
		if (_prepare) return *this;
		sfori(_schema) { _db->bindNull(i + 1); }
		_db->step(this, &_schema);
		_db->reset();
	}
	else {
		_db->begin();
		_db->prepare(sdb::updatePrepareQuery(name, keys, _condition));
		if (_prepare) return *this;
		sfori(keys) { _db->bindNull(i + 1); }
		_db->step(this, &_schema);
		_db->reset();
	}
	if (_db->_trans) _db->commit();
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::update(const SDictionary& dict) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database object is null.");
	if (dict.empty()) {
		auto keys = _schema.keyset();
		_db->begin();
		_db->prepare(sdb::updatePrepareQuery(name, keys, _condition));
		if (_prepare) return *this;
		sfori(keys) _db->bindNull(i + 1);
		_db->step(this, &_schema);
		_db->reset();
	}
	else {
		auto keys = dict.keyset();
		_db->begin();
		_db->prepare(sdb::updatePrepareQuery(name, keys, _condition));
		if (_prepare) return *this;
		_db->bindValues(dict, keys);
		_db->step(this, &_schema);
		_db->reset();
	}
	if (_db->_trans) _db->commit();
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::setRecord(const SObjPtr& obj) {
	if (obj) {
		if (obj.isArray()) _db->bindValues(obj.array());
		else if (obj.isDict()) {
			auto keys = obj.keyset();
			_db->bindValues(obj.dict(), keys);
		}
	}
	else _db->bindNull(1);
	_db->step(this, &_schema);
	_db->reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::setRecords(const SArray& array) {
	sfor(array) { setRecord($_); }
	return *this;
}

slib::SDBTable& slib::SDBTable::upsert(const char *uniq_key, const SDictionary& dict) {
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	if (dict.empty()) throw Exception();
	if (!dict.hasKey(uniq_key)) throw Exception();
	_db->exec(sdb::upsertQuery(name, uniq_key, dict, _condition));
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::grouping(const stringarray& keys) {
	_group = slib::sdb::groupQuery(keys);
	return *this;
}
size_t slib::SDBTable::count(bool reset_condition) {
	clearAll();
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	_db->prepare(sdb::countQuery(name, _condition));
	_db->fetch();
	if (reset_condition) reset();
	return _db->_result[0][0].intValue();
}
slib::SDBTable& slib::SDBTable::countBy(const stringarray& keys) {
	clearAll();
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	_db->prepare(sdb::groupCountQuery(name, keys, _condition));
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::select(const stringarray& keys) {
	clearAll();
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	_db->prepare(sdb::selectQuery(name, keys, _join, _order, _condition, _group, _range));
	_db->fetch(this, &_schema);
	reset();
	return *this;
}
slib::SDBTable& slib::SDBTable::deleteRecord() {
	clearAll();
	if (name.empty()) throw Exception();
	if (!_db) throw NullException("Database");
	_db->exec(sdb::deleteQuery(name, _condition));
	reset();
	return *this;
}
void slib::SDBTable::reset() { 
	_condition.clear();
	_order.clear();
	if (!_joined.empty()) {
		_joined.clear();
		_join.clear();
		_schema = _db->schema(name);
	}
	_group.clear();
	_range.clear();
}
slib::SDBTable& slib::SDBTable::limit(const size_t lim, const size_t off) {
	if (off == 0 && lim == (size_t)-1) _range.clear();
	else _range = sdb::limitQuery(lim, off);
	return *this;
}

slib::SDataBase::SDataBase() {
	_db = nullptr;
	_stmt = nullptr;
	_connect = false;
	_trans = false;
	res = 0;
	err = nullptr;
}
slib::SDataBase::SDataBase(const char* s) : SDataBase() { open(s); }
slib::SDataBase::~SDataBase() { close(); }
void slib::SDataBase::open(const char* s) { 
	if (_connect) close();
	res = sqlite3_open_v2(sfs::absolutePath(s), &_db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, 0);
	if (res == SQLITE_OK) _connect = true;
}
void slib::SDataBase::close() {
	if (!_connect) return;
	if (_trans) commit();
	if (_stmt) sqlite3_finalize(_stmt);
	if (_db) sqlite3_close(_db);
	_connect = false;
}
bool slib::SDataBase::exist(const char* name) {
	prepare(sdb::searchTableQuery(name));
	fetch();
	auto count = _result[0][0].intValue();
	clear();
	return 0 < count;
}
slib::SArray slib::SDataBase::tables() { 
	SArray array;
	prepare("PRAGMA table_list");
	fetch();
	sfor(_result) {
		if ($_[1].beginWith("sqlite_")) continue;
		array.add($_[1]);
	}
	return array;
}
slib::SDBTable slib::SDataBase::table(const char* name) { return SDBTable(name, this); }
slib::SDBTable slib::SDataBase::operator[](const char* name) { return SDBTable(name, this); }
slib::SDictionary slib::SDataBase::schema(const char* name) {
	SArray rules;
	stringarray types;
	slib::SDictionary dict;
	prepare(sdb::tableSchemaQuery(name));
	fetch();
	sfor(_result) {
		rules.clear();
		if ($_["pk"]) rules.add("key");
		if ($_["notnull"]) rules.add("!null");
		if ($_["type"]) types = sstr::toLower($_["type"]).split(SP);
		dict[$_["name"]] = {
			D_("type", types[0])
		};
		if (rules.size()) dict[$_["name"]]["rule"] = rules;
		if ($_["default"]) dict[$_["name"]]["default"] = $_["default"];
		if (1 < types.size()) dict[$_["name"]]["format"] = types[1];
	}
	return dict;
}
slib::STable& slib::SDataBase::result() { return _result; }
void slib::SDataBase::create(const char* name, const Array<SColumn>& columns, const SArray& rows) {
	exec(sdb::createTableQuery(name, columns));
	if (!rows.empty()) {
		begin();
		prepare(sdb::insertPrepareQuery(name, columns.size()));
		sfor(rows) {
			bindValues($_);
			step();
			reset();
		}
		commit();
	}
}
void slib::SDataBase::exec(const char* sql) {
	_sql = sql;
	res = sqlite3_exec(_db, sql, NULL, NULL, &err);
	if (res) throw SDBException(res, "sqlite3 exec error.", _sql + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::prepare(const char* sql) {
	_sql = sql;
	res = sqlite3_prepare_v2(_db, sql, -1, &_stmt, NULL);
	if(res == SQLITE_OK) return;
	throw SDBException(res, "sqlite3 prepare error.", _sql + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::begin() {
	if (!_trans) exec("BEGIN");
	_trans = true;
}
void slib::SDataBase::commit() {
	if (_trans) exec("COMMIT");
	_trans = false;
}
void slib::SDataBase::vacuum() {
	exec("VACUUM");
}
void slib::SDataBase::bindi(const int val, const int i) {
	res = sqlite3_bind_int(_stmt, i, val);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : " + S(val) + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindl(const sinteger val, const int i) {
	res = sqlite3_bind_int64(_stmt, i, val);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : " + S(val) + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindd(const double val, const int i) {
	res = sqlite3_bind_double(_stmt, i, val);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : " + S(val) + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindt(const char* val, const int i) {
	res = sqlite3_bind_text(_stmt, i, val, -1, SQLITE_STATIC);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : " + S(val) + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindtt(const char* val, const int i) {
	res = sqlite3_bind_text(_stmt, i, val, -1, SQLITE_TRANSIENT);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : " + S(val) + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindb(void* val, int size, int i) {
	res = sqlite3_bind_blob(_stmt, i, val, size, SQLITE_STATIC);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : void*" + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindbt(void* val, int size, int i) {
	res = sqlite3_bind_blob(_stmt, i, val, size, SQLITE_TRANSIENT);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : void*" + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindNull(int i) {
	res = sqlite3_bind_null(_stmt, i);
	if (res != SQLITE_OK) throw SDBException(res, "sqlite3 bind error.", "Value : null" + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::bindObj(const SObjPtr& obj, int i) {
	if (obj.isNull()) bindNull(i);
	else if (obj.isNum()) {
		auto& num = obj.number();
		auto type = num.type();
		switch (type) {
		case stype::INTEGER:
			bindl(num.integer(), i);
			break;
		case stype::UINTEGER:
			bindl(num.uinteger(), i);
			break;
		case stype::DOUBLE:
			bindd(num.doubleValue(), i);
			break;
		case stype::BOOL:
			bindi(num.boolean(), i);
			break;

		default:
			break;
		}
	}
	else if (obj.isStr()) bindt(obj.string().cstr(), i);
	else if (obj.isDate()) bindl(obj.date().integer(), i);
	else if (obj.isData()) bindb((void*)obj.data().data(), (int)obj.size(), i);
	else bindtt(obj.toString().cstr(), i);
}
void slib::SDataBase::bindValues(const SArray& values) {
	auto count = 1;
	sfor(values) { bindObj($_, count); ++count; }
}
void slib::SDataBase::bindValues(const SDictionary& values, const stringarray& keys) {
	auto count = 1; 
	sfor(keys) { bindObj(values[$_], count); ++count; }
}
void setRecordValue(int i, int type, sqlite3_stmt* stmt, slib::SColumn& col, slib::SRow& row) {
	switch (type) {
	case SQLITE_INTEGER:
	{
		auto val = sqlite3_column_int64(stmt, i);
		if (col.attribute.hasKey("type")) {
			if (col.attribute["type"] == "integer") row[i] = val;
			else if (col.attribute["type"] == "real") row[i] = (double)val;
			else if (col.attribute["type"] == "bool") row[i] = (val == 1);
			else if (col.attribute["type"] == "date") row[i] = slib::SDate(val);
			else if (col.attribute["type"] == "time") row[i] = slib::SDate(val);
			else row[i] = slib::String((int64_t)val);
		}
		else {
			col.attribute["type"] = "integer";
			row[i] = val;
		}
		break;
	}
	case SQLITE_FLOAT:
	{
		auto val = sqlite3_column_double(stmt, i);
		if (col.attribute.hasKey("type")) {
			if (col.attribute["type"] == "integer") row[i] = (int)val;
			else if (col.attribute["type"] == "real") row[i] = val;
			
		}
		else {
			col.attribute["type"] = "real";
			row[i] = val;
		}
		break;
	}
	case SQLITE_TEXT:
	{
		slib::String val = (const char*)sqlite3_column_text(stmt, i);
		if (col.attribute.hasKey("type")) {
			if (col.attribute["type"] == "integer") row[i] = val.intValue();
			else if (col.attribute["type"] == "real") row[i] = val.doubleValue();
			else if (col.attribute["type"] == "date") {
				row[i] = val;
			}
			else if (col.attribute["type"] == "array") {
				if (val.empty()) row[i] = slib::SArray();
				else {
					if (col.attribute["format"]) {
						auto& f = col.attribute["format"].string();
						if (f == "csv") row[i] = val.split(",");
						else if (f == "tsv") row[i] = val.split(slib::TAB);
						else if (f == "json") row[i] = slib::sjson::parse(val);
					}
					else row[i] = val.split(",");
				}
			}
			else if (col.attribute["type"] == "dict") {
				if (val.empty()) row[i] = slib::SDictionary();
				else {
					if (col.attribute["format"]) {
						auto& f = col.attribute["format"].string();
						if (f == "url") row[i] = val.parse("&", "=");
						else if (f == "py") row[i] = val.parse(",", ":");
						else if (f == "json") row[i] = slib::sjson::parse(val);
					}
					else row[i] = val.parse("&", "=");
				}
			}
			else row[i] = val;
		}
		else {
			col.attribute["type"] = "string";
			row[i] = val;
		}
		break;
	}
	case SQLITE_BLOB:
	{
		row[i] = slib::SData();
		auto& dat = row[i].data();
		dat.resize(sqlite3_column_bytes(stmt, i));
		memcpy(dat.data(), sqlite3_column_blob(stmt, i), dat.size());
		if (col.attribute.hasKey("type")) {
			if (col.attribute["type"] == "data") row[i] = dat;
			//else if (col.attribute["type"] == "image") {}
		}
		else {
			col.attribute["type"] = "data";

		}
		break;
	}
	case SQLITE_NULL:
		break;
	default:
		break;
	}
}
void slib::SDataBase::step(STable* result, SDictionary *schema) {
	if (!result) result = &_result;
	res = sqlite3_step(_stmt);
	if (res == SQLITE_ROW) {
		auto size = sqlite3_column_count(_stmt);
		if (result->columns().empty()) {
			sforin(i, 0, size) {
				String colname = sqlite3_column_name(_stmt, i),
					fullname(sqlite3_column_table_name(_stmt, i));
				fullname << "." << colname;
				if (schema && schema->hasKey(colname)) {
					result->addColumn(SColumn(colname, schema->at(colname).dict()));
				}
				else if (schema && schema->hasKey(fullname)) {
					result->addColumn(SColumn(fullname, schema->at(fullname).dict()));
				}
				else {
					auto type = sqlite3_column_type(_stmt, i);
					result->addColumn(SColumn(colname, 
						{ D_("type", (type == SQLITE_INTEGER?"integer":
							(type == SQLITE_FLOAT? "real":"string")))}
					));
				}	
			}
		}
		result->addRow();
		auto row = result->row(-1);
		sforin(i, 0, size) {
			auto type = sqlite3_column_type(_stmt, i);
			setRecordValue(i, type, _stmt, result->column(i), row);
		}
	}
	else if (res == SQLITE_OK || res == SQLITE_DONE) return;
	else throw SDBException(res, "sqlite3 step error.", _sql + NL + sqlite3_errmsg(_db));
}
void slib::SDataBase::fetch(STable* result, SDictionary* schema) {
	if (!result) result = &_result;
	result->clearAll();
	do { step(result, schema); } while (res == SQLITE_ROW);
	reset();
}
void slib::SDataBase::reset() {
	if (_stmt) res = sqlite3_reset(_stmt);
	if (res == SQLITE_OK) return;
	throw SDBException(res, "sqlite3 reset error.", sqlite3_errmsg(_db));
}
void slib::SDataBase::finalize() {
	if (_stmt) res = sqlite3_finalize(_stmt);
	if (res == SQLITE_OK) return;
	throw SDBException(res, "sqlite3 statement finalize error.", sqlite3_errmsg(_db));
}
void slib::SDataBase::clear() { 
	_buffer.clear();
	_result.clearAll();
}
/*
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

\ SDataBase::tableCount() {
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
*/