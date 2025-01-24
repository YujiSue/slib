#include "sutil/ssqlite.h"
slib::String slib::sdb::sqlValue(const SObjPtr& obj) {
	if (obj.isNull()) return "NULL";
	else if (obj.isNum()) {
		switch (obj.type()) {
		case stype::BOOL:
			return obj ? "1" : "0";
			break;
		case stype::FRACTION:
			return sstr::squote(obj.toString());
			break;
		case stype::COMPLEX:
			return sstr::squote(obj.toString());
			break;
		default:
			return obj.toString();
			break;
		}
	}
	else return sstr::squote(obj);
}
slib::String slib::sdb::sqlValues(const SArray& array) {
	slib::String sql;
	sfor(array) sql << sqlValue($_) << ",";
	sql.resize(sql.size() - 1);
	return sql;
}
slib::String slib::sdb::sqlValues(const SDictionary& dict, const stringarray& keys) {
	slib::String sql;
	sfor(keys) sql << sqlValue(dict[$_]) << ",";
	sql.resize(sql.size() - 1);
	return sql;
}

slib::String slib::sdb::tableListQuery() {
	return "SELECT table FROM sqlite_master WHERE type IN('table', 'view')";
}
slib::String slib::sdb::searchTableQuery(const String& name) {
	return String("SELECT COUNT(*) FROM sqlite_master WHERE type IN('table', 'view') AND name LIKE '") << name << "'";
}
slib::String columnDefineQuery(const slib::SColumn& column) {
	slib::String info(column.name);
	auto type = slib::sstr::toUpper(column.attribute["type"]);
	info << " " << type;
	if (column.attribute.hasKey("format")) info << " " << slib::sstr::toUpper(column.attribute["format"]);
	if (column.attribute.hasKey("rule")) {
		auto rules = column.attribute["rule"];
		if (rules.isArray()) {
			sfor(rules) {
				if ($_ == "key") info << " PRIMARY KEY";
				else if ($_ == "unique") info << " UNIQUE";
				else if ($_ == "!null") info << " NOT NULL";
			}
		}
		else {
			auto& rule = rules.string();
			if (rule == "key") info << " PRIMARY KEY";
			else if (rule == "unique") info << " UNIQUE";
			else if (rule == "!null") info << " NOT NULL";
		}
	}
	if (column.attribute.hasKey("default")) {
		info << " DEFAULT " << column.attribute["default"];
	}
	return info;
}
inline slib::String columnDefineQuery(const slib::Array<slib::SColumn>& columns) {
	slib::String info;
	sfor(columns) info << columnDefineQuery($_) << ",";
	info.resize(info.size() - 1);
	return info;
}
slib::String slib::sdb::createTableQuery(const String& name, const Array<SColumn>& columns) {
	return String("CREATE TABLE IF NOT EXISTS ") << name << "(" << columnDefineQuery(columns) << ")";
}
slib::String slib::sdb::renameTableQuery(const String& oldname, const String& newname) {
	return String("ALTER TABLE ") << oldname << " RENAME TO " << newname;
}
slib::String slib::sdb::dropTableQuery(const String& name) {
	return String("DROP TABLE ") << name;
}
slib::String slib::sdb::tableSchemaQuery(const String& name) {
	return String("PRAGMA table_info('") << name << "')";
}

slib::String slib::sdb::insertQuery(const String& table, const SArray& array, bool returning, const char* retkeys) {
	return String("INSERT INTO ") << table << " VALUES(" << sqlValues(array) << ")" <<
		(returning ? String(" RETURNING ") + (retkeys ? retkeys : "*") : "");
}
slib::String slib::sdb::insertQuery(const String& table, const SDictionary& dict, bool returning, const char* retkeys) {
	auto keys = dict.keyset();
	return String("INSERT INTO ") << table << 
		" VALUES(" << sqlValues(dict, keys) << ")" <<
		(returning ? String(" RETURNING ") + (retkeys ? retkeys : "*") : "");
}
slib::String slib::sdb::insertPrepareQuery(const String& table, const size_t num, bool returning, const char* retkeys) {
	if (!num) throw Exception();
	return String("INSERT INTO ") << table << 
		" VALUES(?" << String(",?") * (int)(num - 1) << ")" << 
		(returning ? String(" RETURNING ") + (retkeys ? retkeys : "*") : "");
}
slib::String slib::sdb::insertPrepareQuery(const String& table, const stringarray& keys, bool returning, const char* retkeys) {
	if (keys.empty()) throw Exception();
	return String("INSERT INTO ") << table << "(" << toString(keys, ",") << ")" <<
		" VALUES(?" << String(",?") * (int)(keys.size() - 1) << ")" << 
		(returning ? String(" RETURNING ") + (retkeys ? retkeys : "*") : "");
}
slib::String slib::sdb::updateQuery(const String& table, const SDictionary& dict, const String& condition) {
	if (dict.empty()) throw Exception();
	String update;
	sfor(dict) update << $_.key() << "=" << sqlValue($_.value()) << ",";
	update.resize(update.size() - 1);
	return String("UPDATE ") << table << " SET " << update <<
		(condition.size() ? " WHERE " + condition : "");
} 
slib::String slib::sdb::updatePrepareQuery(const String& table, const stringarray& keys, const String& condition) {
	if (keys.empty()) throw Exception();
	String update;
	sfor(keys) update << $_ << "=?,";
	update.resize(update.size() - 1);
	return String("UPDATE ") << table << " SET " << update <<
		(condition.size() ? " WHERE " + condition : "");
}
slib::String slib::sdb::upsertQuery(const String& table, const char* uniq, const SDictionary& dict, const String& condition) {
	String update;
	sfor(dict) update << $_.key() << "=" << sqlValue($_.value()) << ",";
	auto keys = dict.keyset();
	return String("INSERT INTO ") << table << "(" << toString(keys, ",") << ")" <<
		" VALUES(" <<
		" ON CONFLICT(" << uniq << ")" <<
		" DO UPDATE SET " << update <<
		(condition.size() ? " WHERE " + condition : "");
}




slib::String slib::sdb::countQuery(const String& table, const String& condition) {
	return String("SELECT COUNT(*) FROM ") << table << 
		(condition.size() ? " WHERE " + condition : "");
}
slib::String slib::sdb::groupCountQuery(const String& table, const stringarray &keys, const String& condition) {
	return String("SELECT ") << toString(keys, ",") << 
		",COUNT(*) FROM " << table << 
		" GROUP BY " << toString(keys, ",") <<
		(condition.size() ? " WHERE " + condition : "");
}
slib::String slib::sdb::selectQuery(const String& table, const stringarray& keys, const String& join, 
	const String& order, const String& condition, const String& group, const String& limit, bool distinct) {
	return String("SELECT ") << toString(keys) <<
		" FROM " << table <<
		(join.size() ? SP + join : "") <<
		(condition.size() ? " WHERE " + condition : "") <<
		(group.size() ? " GROUP BY " + group : "") <<
		(order.size() ? String(" ORDER BY ") + order : "") <<
		(limit.size() ? limit : "");
}
slib::String slib::sdb::deleteQuery(const String& table, const String& condition) {
	return String("DELETE FROM ") << table <<
		(condition.size() ? " WHERE " + condition : "");
}

slib::String slib::sdb::joinQuery(const String& table, sdb::JOIN mode, const String& formula) {
	String que;
	switch (mode) {
	case JOIN::LEFT:
		que << " LEFT JOIN ";
		break;
	case JOIN::RIGHT:
		que << " RIGHT JOIN ";
		break;
	case JOIN::FULL:
		que << " FULL JOIN ";
		break;
	default:
		que << " INNER JOIN ";
		break;
	}
	return que << table << " ON (" << formula << ")";
}
slib::String slib::sdb::groupQuery(const stringarray& keys) { return toString(keys); }
slib::String slib::sdb::orderQuery(const String& key, slib::ORDER order) {
	return key + (order == ORDER::ASC ? " ASC" : " DESC");
}
slib::String slib::sdb::orderListQuery(Array<Pair<String, ORDER>> orders) {
	if (orders.empty()) return "";
	slib::String que;
	sfor(orders) { que << orderQuery($_.first, $_.second) << ","; }
	que.resize(que.size() - 1);
	return que;
}
slib::String slib::sdb::limitQuery(const size_t limit, const size_t offset) {
	return String(" LIMIT ") << limit << (offset ? String(" OFFSET ") + String(offset) : "");
}

slib::String slib::sdb::textQuery(const char* key, const char* value, slib::MATCH m, bool glob) {
	String que(key);
	switch (m) {
	case MATCH::EXACT:
	{
		que << "='" << S(value).replace("'", "''") << "'";
		break;
	}
	case MATCH::START:
	{
		que << (glob?" GLOB \"" : " LIKE '") << S(value).replace("'", "''") << (glob ? "*\"" : "%'");
		break;
	}
	case MATCH::END:
	{
		que << (glob ? " GLOB \"*" : " LIKE '%") << S(value).replace("'", "''") << (glob ? "\"" : "'");
		break;
	}
	case MATCH::PARTIAL:
	{
		que << (glob ? " GLOB \"*" : " LIKE '%") << S(value).replace("'", "''") << (glob ? "*\"" : "%'");
		break;
	}
	default:
		break;
	}
	return que;
}
slib::String slib::sdb::textsQuery(const char* key, const stringarray& values, slib::MATCH m, bool a, bool glob) {
	slib::String que;
	sfor(values) que << textQuery(key, $_, m, glob) << (a ? " AND " : " OR ");
	if (que.size()) que.resize(que.size() - (a ? 5 : 4));
	return que;
}
slib::String slib::sdb::textInQuery(const char* key, const char* value, const char* sep, slib::MATCH m) {
	String que(key);
	switch (m) {
	case MATCH::EXACT:
	{
		que << "='" << value << "' OR " << 
			key << " LIKE '" << value << sep << "%' OR " <<
			key << " LIKE '%" << sep << value << "' OR " << 
			key << " LIKE '%" << sep << value << sep << "%'";
		break;
	}
	case MATCH::START:
	{
		que << " LIKE '" << value << "%' OR " <<
			key << " LIKE '%" << sep << value << "%'";
		break;
	}
	case MATCH::END:
	{
		que << " LIKE '%" << value << "' OR " <<
			key << " LIKE '%" << value << sep << "%'";
		break;
	}
	case MATCH::PARTIAL:
	{
		que << " LIKE '%" << value << "%'";
		break;
	}
	default:
		break;
	}
	return que;
}
slib::String slib::sdb::textsInQuery(const char* key, const stringarray& values, const char* sep, slib::MATCH m, bool a) {
	slib::String que;
	sfor(values) que << "(" << textInQuery(key, $_, sep, m) << ")" << (a ? " AND " : " OR ");
	if (que.size()) que.resize(que.size() - (a ? 5 : 4));
	return que;
}
slib::String slib::sdb::inQuery(const char* key, const intarray& array) {
	if (array.empty()) return "";
	String que(key);
	que << " IN(" << toString(array) << ")";
	return que;
}
slib::String slib::sdb::inQuery(const char* key, const stringarray& array) {
	if (array.empty()) return "";
	String que(key);
	que << " IN(";
	sfor(array) que << sstr::squote($_) << ",";
	que[-1] = ')';
	return que;
}
slib::String slib::sdb::inQuery(const char* key, const SArray& array) {
	if (array.empty()) return "";
	String que(key);
	que << " IN(";	
	sfor(array) {
		if ($_.isNull()) que << "NULL,";
		else if ($_.isNum()) que << $_.toString() << ",";
		else que << "'" << $_.toString() << "',";
	}
	que[-1] = ')';
	return que;
}
