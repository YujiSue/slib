#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"

using namespace slib;
using namespace slib::sio;

STable::STable() : SObject() { _lastcol = 0; }
STable::STable(int row, int col) : STable(){ resize(row, col); }
STable::STable(const Array<SColumn> &cols, const SArray &rows) : STable() {
	addColumns(cols); addRows(rows);
}
STable::STable(const sobj &obj) : STable() {
    if (obj.isTable()) *this = obj.table();
    if (obj.isDict()) {
        if (obj.hasKey("columns")) { sforeach(obj["columns"]) addColumn(SColumn(E_["type"], E_["name"])); }
        if (obj.hasKey("rows")) _rows = obj["rows"];
    }
}
STable::STable(const STable &table) : STable() {
	_columns = table.columns();	
	_rows = table.rows();
}
STable::~STable() {}
STable &STable::operator=(const STable &table) {
    clearAll();
    _columns = table._columns;
	_rows = table._rows;
    return *this;
}
void STable::initWithArray(SArray& array, bool header) {
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

		/*
        for (int c = 0; c < columnCount(); ++c) {
            if(!c) file<<SString::dquot(value(r, c));
            else file<<sep<<SString::dquot(value(r, c));
        }
        if(r < rowCount()-1) file<<NEW_LINE;
		*/
    }
    file.flush();
}

size_t STable::columnCount() const { return _columns.size(); }
bool STable::hasColumn(const char* name) const { return columnIndex(name) != NOT_FOUND; }
size_t STable::columnIndex(const char *name) const {
    sforeach(_columns) { if(E_.name() == name) return INDEX_(_columns); }
    return NOT_FOUND;
}
SColumn& STable::operator[](const char* name) { return column(name); }
const SColumn& STable::operator[](const char* name) const { return column(name); }
SColumn& STable::columnAt(sinteger idx) { return _columns[idx].column(); }
const SColumn &STable::columnAt(sinteger idx) const { return _columns[idx].column(); }
SColumn &STable::column(const char* name) {
	sforeach(_columns) { if (E_.name() == name) return E_.column(); }
	throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR, name, "columns");
}
const SColumn &STable::column(const char *name) const {
	sforeach(_columns) { if (E_.name() == name) return E_.column(); }
	throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR, name, "columns");
}
SArray& STable::columns() { return _columns; }
const SArray& STable::columns() const { return _columns; }
void STable::addColumn(const char* s) {
	if (s) _columns.add(SColumn(OBJECT_COLUMN, s));
	else { _columns.add(SColumn(OBJECT_COLUMN, String("column") + (_lastcol + 1))); ++_lastcol; }
}
void STable::addColumn(const SColumn& col) {
	_columns.add(col);
	_columns.last().column().setTable(this);
	auto size = _columns.size();
	sforeach(_rows) E_.resize(size);
}
void STable::addColumns(const Array<SColumn>& cols) {
	if (cols.empty()) return;
	sforeach(cols) { _columns.add(E_); _columns.last().column().setTable(this); }
	auto size = _columns.size();
	sforeach(_rows) E_.resize(size);
}
void STable::insertColumn(sinteger idx, const SColumn& col) {
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
}
size_t STable::rowCount() const { return _rows.size(); }
SArray& STable::operator[](int idx) { return _rows[idx].array(); }
const SArray& STable::operator[](int idx) const { return _rows[idx].array(); }
SArray& STable::rowAt(int idx) { return _rows[idx].array(); }
const SArray& STable::rowAt(int idx) const { return _rows[idx].array(); }
SArray& STable::rows() { return _rows; }
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
void STable::resize(size_t r, size_t c) { resizeColumn(c); resizeRow(r); }
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
void STable::clearAll() {
    _columns.clear();
	_rows.clear();
	_lastcol = 0;
}
String STable::getClass() const { return "table"; }
String STable::toString() const {
	String str;
	if (columnCount()) {
		sforeach(_columns) str << E_.name() << TAB;
		str << NEW_LINE;
    }
    if (rowCount()) {
		sforeach(_rows) str << slib::toString(E_.array(), TAB) << NEW_LINE;
    }
    return str;
}
SObject * STable::clone() const { return new STable(*this); }