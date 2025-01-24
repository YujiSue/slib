#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"

slib::STable::STable() : slib::SObject() {}
slib::STable::STable(const size_t row, const size_t col) : slib::STable(){ resize(row, col); }
slib::STable::STable(const Array<SColumn> &cols, const SArray &rows) : STable() {
	resize(rows.size(), cols.size());
	sfor2(_columns, cols) $_1 = $_2;
	sfor2(_rows, rows) $_1 = $_2;
}
slib::STable::STable(const sobj &obj) : STable() {
	if (!obj) return;
	if (obj.isTable()) *this = obj.table();
	else if (obj.isArray()) {
		if (obj.empty()) return;
		else if (!obj.isArray()) throw FormatException(formatErrorText("Table array", "array[" + obj.getClass() + "]", "array[array]"));
		resizeColumn(obj[0].size());
		sforeach(row, obj) addRow(row);
	}
    else if (obj.isDict()) {
        if (obj.hasKey("cols")) { sfor(obj["cols"]) addColumn(SColumn($_["name"], $_)); }
        if (obj.hasKey("rows")) _rows = obj["rows"];
		if (obj.hasKey("attribute")) attribute = obj["attribute"];
    }
}
slib::STable::STable(const STable &table) : STable() {
	_columns = table._columns;
	_rows = table._rows;
	attribute = table.attribute;
}
slib::STable::~STable() {}
slib::STable & slib::STable::operator=(const slib::STable &table) {
    clearAll();
    _columns = table._columns;
	_rows = table._rows;
	attribute = table.attribute;
	return *this;
}

inline void _addStr(const stringarray& vals, slib::STable& tbl) {
	if (vals.size() != tbl.ncol())
		throw FormatException(formatErrorText("Cell count of a single row", S(vals.size()), S(tbl.ncol())));
	tbl.addRow();
	auto cellit = tbl[-1].begin();
	sfor2(tbl.columns(), vals) {
		if ($_1.attribute.hasKey("type")) {
			auto type = slib::sstr::toLower($_1.attribute["type"]);
			if (type == "auto") *cellit = slib::SObjPtr::toObj($_2);
			else if (type.match("int")) *cellit = $_2.integer();
			else if (type.match("num")) *cellit = slib::SNumber($_2);
			else if (type.match("date")) *cellit = slib::SDate((const char *)$_2);
			else *cellit = $_2;
		}
		else *cellit = $_2;
		++cellit;
	}
}

slib::STable slib::STable::load(const char* path, sobj pref) {
	auto ext = slib::sstr::toLower(slib::sfs::extension(path));
	bool header = pref.hasKey("header") ? (bool)pref["header"] : false;
	STable tbl;
	if (ext == "csv" || ext == "tsv" || ext == "txt") {
		SFile f(path);
		String ln,
			sep = pref.hasKey("sep") ? pref["sep"] : (ext == "csv" ? "," : "\t"),
			note = pref.hasKey("note") ? pref["note"] : "";
		f.readLine(ln);
		if (note.size() && ln.beginWith(note)) {
			tbl.attribute["note"] = SArray();
			while (ln.beginWith(note)) {
				tbl.attribute["note"].add(ln.substring(note.size()));
			}
		}
		if (header) {
			auto vals = ln.split(sep);
			sfor(vals) tbl.addColumn(SColumn($_));
		}
		else {
			auto vals = ln.split(sep);
			if (pref.hasKey("cols")) {
				tbl.resizeColumn(pref["cols"].size());
				sfor2(tbl._columns, pref["cols"]) $_1 = $_2;
			}
			else sfori(vals) tbl.addColumn(SColumn("Column " + S(i)));
			_addStr(vals, tbl);
		}
		while (f) {
			f.readLine(ln);
			auto vals = ln.split(sep);
			_addStr(vals, tbl);
		}
	}
	else if (ext == "json") {
		auto obj = sjson::load(path);
		if (obj.hasKey("cols") && obj.hasKey("rows")) {
			tbl.resizeColumn(obj["cols"].size());
			sfor2(tbl._columns, obj["cols"]) $_1 = $_2;
			tbl._rows.append(obj["rows"]);
		}
		else throw FormatException(formatErrorText("Table JSON", obj.toString(), "{\n  cols:{...},\n  rows:{...},\n  (attribute:{...})\n}"));
		if (obj.hasKey("attribute")) tbl.attribute = obj["attribute"];
	}
	return tbl;
}
void slib::STable::save(const STable& tbl, const char* path, sobj pref) {
	auto ext = slib::sstr::toLower(slib::sfs::extension(path));
	bool header = pref.hasKey("header") ? (bool)pref["header"] : false;
	if (ext == "csv" || ext == "tsv" || ext == "txt") {
		SFile f(path, sio::WRITE);
		if (tbl.attribute.size() && pref["attribute"]) {
			auto note = pref["note"] ? pref["note"] : "#";
			if (tbl.attribute.hasKey("note") && tbl.attribute["note"].isArray()) {
				
			}
			else {

			}
		}



	}
	else if (ext == "json") { sjson::save(tbl.toObj(), path); }
}

/*
void slib::STable::initWithArray(SArray& array, bool header) {
	if (array.empty()) return;
	auto it = array.begin();
	if (header) {
		sforeach_(col, E_) addColumn(col->toString()); ++it;
	}
	else {
		auto cnum = E_.size();
		sforin(c, 0, cnum) addColumn();
	}
	while (it < array.end()) addRow(E_.array());
}
void STable::initWithDict(SDictionary& dict) {
	sforeach(dict["columns"]) {
		addColumn(SColumn(E_["type"], E_["name"]));
	}
	sforeach(dict["rows"]) { addRow(E_.array()); }
}
void STable::load(sobj obj) {
    clearAll();
    if (!obj["path"]) return;
    if (obj["type"]) {
        if (obj["type"] == "tsv") loadTxt(obj["path"], "\t", obj["header"]);
        else if (obj["type"] == "csv") loadTxt(obj["path"], ",", obj["header"]);
        else if (obj["type"] == "json") {
            SJson tbl; 
			tbl.load(obj["path"]);
			initWithDict(tbl.dict());
        }
        //else if (param["type"] == "XLS" || param["type"] == "XLSX") loadXls();
    }
}
void STable::load(const char *path) {
    clearAll();
    auto ext = String::lower(SFile(path).extension());
    if (ext == "csv") loadTxt(path, ",", true);
	else if (ext == "tsv") loadTxt(path, ",", true);
    else if (ext == "json") {
		SJson tbl;
		tbl.load(path);
		initWithDict(tbl.dict());
    }
    //else if (ext == "xls" || ext == "xlsx") loadXls();
}
void STable::loadTxt(const char *path, const char *sep, bool header) {
    clearAll();
    String row;
    SFile file(path);
    file.open();
    file.readLine(row);
    auto values = row.split(sep);
    if (header) {
        sforeach(values) addColumn(SColumn(E_.isNumeric()?NUMBER_COLUMN:TEXT_COLUMN, E_));
    }
    else {
		sforeachi(values) addColumn(SColumn(values[i].isNumeric() ? NUMBER_COLUMN : TEXT_COLUMN, String("col") + (i + 1)));
        addRow(sarray(values));
    }
    while (!file.eof()) {
        file.readLine(row);
        values = row.split(sep);
        auto obj = sarray(values);
        addRow(sarray(values));
    }
}
void STable::save(sobj obj) {
	if (!obj["path"]) return;
	if (obj["type"]) {
		if (obj["type"] == "tsv") saveTxt(obj["path"], "\t");
		else if (obj["type"] == "csv") saveTxt(obj["path"], ",");
		else if (obj["type"] == "json") {
			SJson tbl(*this);
			tbl.save(obj["path"]);
		}
		//else if (param["type"] == "XLS" || param["type"] == "XLSX") loadXls();
	}
}
void STable::save(const char *path) {
	auto ext = SFile(path).extension();
    if (ext == "csv") saveTxt(path, ",");
    else if (ext == "txt") saveTxt(path, "\t");
	else if (ext == "json") {
		SJson tbl(*this);
		tbl.save(path);
	}
    //else if (ext == "xls" || ) saveXls();
}
void STable::saveTxt(const char *path, const char *sep) {
    SFile file(path, sio::CREATE);
    if(_columns.empty()) return;
	sforeach(_columns) file << E_.name();
	file << NEW_LINE; file.flush();
	if (_rows.empty()) return;
    sforeach(_rows) {

	
    for (int c = 0; c < columnCount(); ++c) {
            if(!c) file<<SString::dquot(value(r, c));
            else file<<sep<<SString::dquot(value(r, c));
        }
        if(r < rowCount()-1) file<<NEW_LINE;

		}
    file.flush();
}
*/
size_t slib::STable::ncol() const { return _columns.size(); }
size_t slib::STable::nrow() const { return _rows.size(); }

/*
bool STable::hasColumn(const char* name) const { return columnIndex(name) != NOT_FOUND; }

*/
size_t slib::STable::colIndex(const char *s) const {
    sfor(_columns) { if($_.name == s) return $INDEX(_columns); }
    return NOT_FOUND;
}
slib::SRow slib::STable::operator[](const int idx) { return SRow(_rows.data(idx), this); }
slib::SColumn& slib::STable::operator[](const char* name) { return _columns[(int)colIndex(name)]; }
slib::SRow slib::STable::row(const int idx) { return SRow(_rows.data(idx), this); }
const slib::SRow slib::STable::row(const int idx) const { return SRow(const_cast<SObjPtr*>(_rows.data(idx)), const_cast<STable *>(this)); }
slib::SColumn& slib::STable::column(const int idx) { return _columns[idx]; }
const slib::SColumn& slib::STable::column(const int idx) const { return _columns[idx]; }


/*
const slib::SColumn& slib::STable::operator[](const char* name) const { return _columns[colIndex(name)]; }
slib::SColumn& slib::STable::columnAt(sinteger idx) { return _columns[idx].column(); }
const slib::SColumn & slib::STable::columnAt(sinteger idx) const { return _columns[idx].column(); }
slib::SColumn & slib::STable::column(const char* name) {
	sforeach(_columns) { if (E_.name() == name) return E_.column(); }
	throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR, name, "columns");
}
const SColumn &STable::column(const char *name) const {
	sforeach(_columns) { if (E_.name() == name) return E_.column(); }
	throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR, name, "columns");
}
SArray& STable::columns() { return _columns; }
*/
const slib::Array<slib::SColumn>& slib::STable::columns() const { return _columns; }
const slib::SArray& slib::STable::rows() const { return _rows; }

slib::RowIterator slib::STable::begin() { return slib::RowIterator(_rows.begin(), this); }
//slib::RowCIterator slib::STable::begin() const;
slib::RowIterator slib::STable::end() { return slib::RowIterator(_rows.end(), this); }
//slib::RowCIterator slib::STable::end() const;

void slib::STable::resizeColumn(const size_t sz) {
	auto cn = ncol();
	if (cn < sz) {
		sforin(i, cn, sz) addColumn(SColumn());
	}
	else if (sz < cn) {
		_columns.resize(sz);
		sfor(_rows) { $_.resize(sz); }
	}
}
slib::STable& slib::STable::addColumn(const SColumn& col) {
	_columns.add(col);
	_columns[-1].setTable(this);
	auto size = _columns.size();
	sfor(_rows) $_.resize(size);
	return *this;
}
slib::STable& slib::STable::appendColumn(const Array<SColumn>& cols) {
	auto cs = (int)_columns.size();
	_columns.append(cols);
	sforin(i, 0, (int)cols.size()) _columns[cs + i].setTable(this);
	auto size = _columns.size();
	sfor(_rows) $_.resize(size);
	return *this;
}

void slib::STable::resizeRow(const size_t sz) {
	auto rn = nrow();
	if (rn < sz) {
		sforin(i, rn, sz) addRow();
	}
	else if (sz < rn) { _rows.resize(sz); }
}
slib::STable& slib::STable::addRow(SObjPtr obj) {
	_rows.add(SArray(ncol()));
	if (obj) {
		auto& row = _rows[-1].array();
		if (obj.isArray()) {
			sfor2(row, obj.array()) { $_1 = $_2; }
		}
		else if (obj.isDict()) {
			size_t idx;
			sfor(obj.dict()) {
				if ((idx = colIndex($_.key())) != NOT_FOUND) row[(int)idx] = $_.value();
			}
		}
	}
	return *this;
}

/*
void slib::STable::addColumns(const Array<SColumn>& cols) {
	if (cols.empty()) return;
	sforeach(cols) { _columns.add(E_); _columns.last().column().setTable(this); }
	auto size = _columns.size();
	sforeach(_rows) E_.resize(size);
}
void slib::STable::insertColumn(sinteger idx, const SColumn& col) {
	_columns.insert(idx, col);
	_columns[idx].column().setTable(this);
	sforeach(_rows) E_.insert(idx, snull);
}
void STable::setColumn(sinteger idx, const SColumn& col) {
	_columns[idx] = col; 
	_columns[idx].column().setTable(this);
	_columns[idx].convert(col._type);
}
void STable::removeColumn(sinteger idx) {
	_columns.removeAt(idx);
    sforeach(_rows) E_.removeAt(idx);
}
void STable::removeColumns(size_t off, size_t len) {
	_columns.remove(off, len);
    sforeach(_rows) E_.remove(off, len);
}
void STable::swapColumns(size_t i1, size_t i2) {
	_columns.exchange(i1, i2);
	sforeach(_rows) { E_.array().exchange(i1, i2); }
}
void STable::resizeColumn(size_t s) {
	auto count = columnCount();
	if (s < count) _columns.resize(s);
	else if (count < s) {
		sforin(c, count, s) {
			addColumn(SColumn(OBJECT_COLUMN, String("column") + (_lastcol + 1)));
			++_lastcol;
		}
	}
	sforeach(_rows) E_.resize(s, snull);
}*/
//const slib::SRow& slib::STable::operator[](const int idx) const { return SRow(_rows.ptr(idx), this); }
/*
SArray& STable::rowAt(int idx) { return _rows[idx].array(); }
const SArray& STable::rowAt(int idx) const { return _rows[idx].array(); }
const SArray& STable::rows() const { return _rows; }
void STable::addRow() { _rows.add(SArray(columnCount())); }
void STable::addRow(const SArray& array) { 
	_rows.add(array);
	_rows.last().resize(columnCount());
}
void STable::addRows(const SArray& array) {
	sforeach(array) addRow(E_.array());
}
void STable::insertRow(size_t idx, const SArray& array) {
	_rows.insert(idx, array); 
	_rows[idx].resize(columnCount());
}
void STable::setRow(size_t idx, const SArray& array) {
	auto& row = _rows[idx].array();
	sforeach2(row, array) E1_ = E2_;
}
void STable::updateRow(size_t idx, const SDictionary& dict) {
	auto& row = _rows[idx];
	sforeach(dict) row[columnIndex(E_.key)] = E_.value;
}
void STable::removeRow(int idx) { _rows.removeAt(idx); }
void STable::removeRows(size_t off, size_t len) { _rows.remove(off, len); }
void STable::clearRows() { _rows.clear(); }
void STable::swapRows(size_t i1, size_t i2) { _rows[i1].swap(_rows[i2]); }
void STable::resizeRow(size_t s) { _rows.resize(s); }
void STable::add(sobj obj, smath::DIRECTION dir) {
	if (dir == HORIZONTAL) addRow(obj.array());
	else {
		if (obj.isColumn()) addColumn(obj.column());
		else if (obj.isDict()) addColumn(obj.dict());
		else addColumn(obj.toString());
	}
}
void STable::insert(sint idx, sobj obj, smath::DIRECTION dir) {
	if (dir == HORIZONTAL) insertRow(idx, obj.array());
	else {
		if (obj.isColumn()) insertColumn(idx, obj.column());
		else if (obj.isDict()) insertColumn(idx, obj.dict());
		else insertColumn(idx, obj.toString().cstr());
	}
}
void STable::set(sint idx, sobj obj, smath::DIRECTION dir) {
	if (dir == HORIZONTAL) setRow(idx, obj.array());
	else {
		if (obj.isColumn()) setColumn(idx, obj.column());
		else if (obj.isDict()) setColumn(idx, obj.dict());
		else setColumn(idx, obj.toString().cstr());
	}
}
sobj& STable::at(sint r, sint c) { return _rows[r][c]; }
const sobj& STable::at(sint r, sint c) const { return _rows[r][c]; }
sobj STable::getValue(sint r, sint c) const { return _rows[r][c]; }
SArray STable::getValues(sint r, sint c, sint h, sint w) const {
	SArray array;
	auto rit = _rows.begin() + r;
	sforin(i, 0, h) {
		SArray cells;
		auto cit = rit->begin() + c;
		sforin(j, 0, w) { cells.add(*cit); ++cit; }
		array.add(cells); ++rit;
	}
	return array;
}
void STable::clearValue(sint r, sint c) { at(r, c) = snull; }
void STable::clearValues(sint r, sint c, sint h, sint w) {
	auto rit = _rows.begin() + r;
	sforin(i, 0, h) {
		SArray cells;
		auto cit = rit->begin() + c;
		sforin(j, 0, w) { (*cit) = snull; ++cit; }
		++rit;
	}
}
void STable::setValue(sint r, sint c, sobj val) { at(r, c) = val; }
void STable::updateValues(sint r, const SDictionary& values) {
	auto& row = _rows[r];
	sforeach(values) row[columnIndex(E_.key)] = E_.value;
}
void STable::setValues(sint r, sint c, const SArray& values) {
	auto rit = _rows.begin() + r;
	sforeach(values) {
		auto cit = rit->begin() + c;
		sforeach_(vit, E_) { (*cit) = (*vit); ++cit; }
		++rit;
	}
}
*/
void slib::STable::resize(const size_t r, const size_t c) { 
	_columns.resize(c);
	_rows.resize(r);
	sfor(_rows) { $_ = SArray(c); }
}
slib::SObjPtr slib::STable::toObj() const {
	slib::SObjPtr obj = SDictionary();
	sfor(_columns) { obj["cols"].add($_.toObj()); }
	obj["rows"] = _rows;
	if (!attribute.empty()) obj["attribute"] = attribute;
	return obj;
}

void slib::STable::clear() {
	_rows.clear();
}
/*
Range<size_t> STable::find(const sobj& obj, smath::DIRECTION dir, srange offset) {
	if (dir == smath::HORIZONTAL) {
		auto rbeg = _rows.begin() + offset.begin, rit = rbeg;
		auto cbeg = rbeg->begin() + offset.end, cit = cbeg;
		while (cit < rit->end()) { 
			if ((*cit) == obj) return Range<size_t>(rit - _rows.begin(), cit - rit->begin()); 
			++cit;
		}
		++rit;
		if (rit == _rows.end()) rit = _rows.begin();
		while (rit != rbeg) {
			sforeach_(cit, *rit) {
				if ((*cit) == obj) return Range<size_t>(rit - _rows.begin(), cit - rit->begin());
			}
			++rit;
			if (rit == _rows.end()) rit = _rows.begin();
		}
		cit = rbeg->begin();
		while (cit < cbeg) {
			if ((*cit) == obj) return Range<size_t>(rbeg - _rows.begin(), cit - rit->begin());
			++cit;
		}
	}
	else {
		auto rbeg = _rows.begin() + offset.begin, rit = rbeg;
		auto cbeg = offset.end, current = cbeg;
		while (rit < _rows.end()) {
			if (rit->at(current) == obj) return Range<size_t>(rit - _rows.begin(), current);
			++rit;
		}
		++current;
		if (current == columnCount()) current = 0;
		while (current != cbeg) {
			rit = _rows.begin();
			while (rit < _rows.end()) {
				if (rit->at(current) == obj) return Range<size_t>(rit - _rows.begin(), current);
				++rit;
			}
			++current;
			if (current == columnCount()) current = 0;
		}
		rit = _rows.begin();
		while (rit < rbeg) {
			if (rit->at(cbeg) == obj) return Range<size_t>(rit - _rows.begin(), cbeg);
			++rit;
		}
	}
	return Range<size_t>(NOT_FOUND, NOT_FOUND);
}
void STable::sortBy(const char* name, ORDER order) {
	auto idx = columnIndex(name);
	if (idx == NOT_FOUND) throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
	_rows.sort([idx, order](const sobj& so1, const sobj& so2) {
		if (order == ASC) return so1[idx] < so2[idx];
		else return so2[idx] < so1[idx];
	});
}
*/
void slib::STable::clearAll() {
    _columns.clear();
	_rows.clear();
}
slib::String slib::STable::getClass() const { return "table"; }
slib::String slib::STable::toString(const char *format) const {
	String f(format);
	if (f == "json") return sjson::toString(toObj());
	else {
		String str, sep;
		if (f == "csv") sep = ",";
		else sep = TAB;
		if (ncol()) {
			sforeach(col, _columns) { str << sstr::dquote(col.name) << sep; }
			str.resize(str.size() - 1);
			str << NL;
		}
		if (nrow()) {
			sfor(_rows) {
				sforeach(cell, $_) { str << sstr::dquote(cell.toString()) << sep; }
				str.resize(str.size() - 1);
				str << NL;
			}
		}
		return str;
	}
}
slib::SObject * slib::STable::clone() const { return new STable(*this); }
std::ostream& slib::operator<<(std::ostream& os, const STable& tbl) {
	return os << tbl.toString("csv");
}