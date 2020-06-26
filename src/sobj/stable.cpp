#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"

using namespace slib;
using namespace slib::sio;

STable::STable() : SObject() {}
STable::STable(int row, int col) : STable(){
	resize(row, col);
}
STable::STable(const Array<scolumn> &cols, const SArray &rows) : STable() {
    sforeach(cols) addColumn(E_);
    sforeach(rows) addRow(E_);
}
STable::STable(const smat<sobj> &mat) : STable() {
    sforin(i, 0, mat.col) {
        auto val = mat[0][i];
        auto col_name = SString("col")+(i+1);
        if(val.isNum()) addColumn(scolumn(NUMBER_COLUMN, col_name));
        else if(val.isStr()) addColumn(scolumn(STRING_COLUMN, col_name));
        else if(val.isStr()) addColumn(scolumn(TEXT_COLUMN, col_name));
        else if(val.isDate()) addColumn(scolumn(DATE_COLUMN, col_name));
        else if(val.isDat()) addColumn(scolumn(DATA_COLUMN, col_name));
        else if(val.isArray()) addColumn(scolumn(ARRAY_COLUMN, col_name));
        else if(val.isDict()) addColumn(scolumn(DICT_COLUMN, col_name));
        else if(val.isImg()) addColumn(scolumn(IMG_COLUMN, col_name));
    }
    auto *values = mat.ptr();
    sforin(r, 0, mat.row) {
        addRow(); auto &row = _rows.last();
        sforeach(row) { E_ = *values; ++values; }
    }
}
STable::STable(const sobj &obj) : STable() {
    if (obj.isTable()) *this = obj.table();
    if (obj.isDict()) {
        _name = obj["name"];
        if (obj.hasKey("columns")) { sforeach(obj["columns"]) addColumn(scolumn(E_["type"], E_["name"])); }
        if (obj.hasKey("rows")) _rows = obj["rows"];
    }
}
STable::STable(const STable &table) : STable() {
    _name = table._name;
	_columns = table.columns();
	_rows = table.rows();
}
STable::~STable() {}
STable &STable::operator=(const STable &table) {
    clear();
    _name = table._name;
    _columns = table._columns;
	_rows = table._rows;
    return *this;
}

void STable::load(sobj obj) {
    clear();
    if (!obj["path"]) return;
    if (obj["type"]) {
        if (obj["type"] == "TSV") loadTxt(obj["path"], "\t", obj["header"]);
        else if (obj["type"] == "CSV") loadTxt(obj["path"], ",", obj["header"]);
        else if (obj["type"] == "JSON") {
            SJson tbl; 
			tbl.load(obj["path"]);
			*this = tbl;
        }
        //else if (param["type"] == "XLS" || param["type"] == "XLSX") loadXls();
    }
}
void STable::load(const char *path) {
    clear();
    auto ext = SFile(path).extension();
    if (ext == "csv") loadTxt(path, ",", true);
    else if (ext == "txt") loadTxt(path, "\t", true);
    else if (ext == "json") {
		SJson tbl;
		tbl.load(path);
		*this = tbl;
    }
    //else if (ext == "xls" || ext == "xlsx") loadXls();
}
void STable::loadTxt(const char *path, const char *sep, bool header) {
    clear();
    String row;
    SFile file(path);
    file.open();
    file.readLine(row);
    auto values = row.split(sep);
    if (header) {
        sforeach(values) addColumn(scolumn(E_.isNumeric()?NUMBER_COLUMN:TEXT_COLUMN, E_));
    }
    else {
        sforeachi(values) addColumn(scolumn(values[i].isNumeric()?NUMBER_COLUMN:TEXT_COLUMN, SString("col")+(i+1)));
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
		if (obj["type"] == "TSV") saveTxt(obj["path"], "\t");
		else if (obj["type"] == "CSV") saveTxt(obj["path"], ",");
		else if (obj["type"] == "JSON") {
			SJson tbl(*this);
			tbl.save(obj["path"]);
		}
		//else if (param["type"] == "XLS" || param["type"] == "XLSX") loadXls();
	}
}
void STable::save(const char *path) {
    clear();
    auto ext = SFile(path).extension();
    if (ext == "csv") saveTxt(path, ",");
    else if (ext == "txt") saveTxt(path, "\t");
	else if (ext == "json") {

	}
    //else if (ext == "xls" || ) saveXls();
}
void STable::saveTxt(const char *path, const char *sep) {
    SFile file(path, sio::CREATE);
    if(_columns.empty()) return;
    sforin(c, 0, columnCount()) file<<SString::dquot(column(c).name());
    file<<NEW_LINE;
    file.flush();
    if(!rowCount()) return;
    for (int r = 0; r < rowCount(); ++r) {
        for (int c = 0; c < columnCount(); ++c) {
            if(!c) file<<SString::dquot(value(r, c));
            else file<<sep<<SString::dquot(value(r, c));
        }
        if(r < rowCount()-1) file<<NEW_LINE;
    }
    file.flush();
}
String& STable::name() { return _name; }
const String &STable::name() const { return _name; }
void STable::setName(const char *name) { _name = name; }

bool STable::hasColumn(const char *name) const {
    return columnIndex(name) != NOT_FOUND;
}
size_t STable::columnCount() const { return _columns.size(); }
size_t STable::columnIndex(const char *name) const {
    sforeach(_columns) { if(E_.column().name() == name) return it-_columns.begin(); }
    return NOT_FOUND;
}
SColumn& STable::column(int idx) {
	return _columns[idx].column();
}
const SColumn &STable::column(int idx) const {
    return _columns[idx].column();
}
SColumn &STable::column(const char* name) {
	auto idx = columnIndex(name);
	if (idx != NOT_FOUND) return column(idx);
	else throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
}
const SColumn &STable::column(const char *name) const {
    auto idx = columnIndex(name);
	if (idx != NOT_FOUND) return column(idx);
	else throw SException(ERR_INFO, SLIB_NOT_FOUND_ERROR);
}
SArray& STable::columns() { return _columns; }
const SArray &STable::columns() const { return _columns; }
void STable::addColumn(const SColumn& col) {
	_columns.add(snull);
	setColumn(columnCount() - 1, col);
}
void STable::addColumnDict(const SDictionary& dict) {
	_columns.add(snull);
	setColumnDict(columnCount() - 1, dict);
}
void STable::addColumn(const sobj& obj) {
	if (obj.isNull()) _columns.add(SColumn(OBJECT_COLUMN, String("column") + (columnCount() + 1)));
	else if (obj.isColumn()) addColumn(obj.column());
	else if (obj.isDict()) addColumnDict(obj.dict());
}
void STable::insertColumn(size_t idx, const SColumn& col) {
	_columns.insert(idx, snull);
	setColumn(idx, col);
}
void STable::insertColumnDicr(size_t idx, const SDictionary& dict) {
	_columns.insert(idx, snull);
	setColumnDict(idx, dict);
}
void STable::insertColumn(size_t idx, const sobj& obj) {
	if (obj.isDict()) setColumnDict(idx, obj.dict());
	else if (obj.isColumn()) setColumn(idx, obj.column());
}
void STable::setColumn(size_t idx, const SColumn& col) {
	_columns[idx].column() = col;
	auto& newcol = column(idx);
	if (newcol.name().empty()) newcol.setName(String("column ") + (columnCount() + 1));
	newcol.setTable(this);
	auto rnum = rowCount(), size = col.size();
	if (rnum < size) {
		sforin(i, rnum, size) addRow();
		auto cit = newcol.begin();
		sforeach(col) { *cit = E_; ++cit; }
	}
	else if (size){
		auto cit = newcol.begin();
		sforeach(col) { *cit = E_; ++cit; }
		sforin(i, size, rnum) { *cit = snull; ++cit; }
	}
	newcol.clear();
}
void STable::setColumnDict(size_t idx, const SDictionary& dict) {
	_columns[idx].column() = SColumn(dict["type"], dict["name"]);
	auto& newcol = column(idx);
	if (newcol.name().empty()) newcol.setName(String("column ") + (columnCount() + 1));
	newcol.setTable(this);
	column(idx).setTable(this);
	if (dict.hasKey("values")) {
		auto& col = dict["values"];
		auto rnum = rowCount(), size = col.size();
		if (rnum < size) {
			sforin(i, rnum, size) addRow();
			auto cit = newcol.begin();
			sforeach(col) { *cit = E_; ++cit; }
		}
		else if (size) {
			auto cit = newcol.begin();
			sforeach(col) { *cit = E_; ++cit; }
			sforin(i, size, rnum) { *cit = snull; ++cit; }
		}
	}
	newcol.clear();
}
void STable::setColumn(size_t idx, const sobj& obj) {
	if (obj.isDict()) setColumnDict(idx, obj.dict());
	else if (obj.isColumn()) setColumn(idx, obj.column());
}
void STable::removeColumn(size_t idx) {
	_columns.removeAt(idx);
    sforeach(_rows) E_.removeAt(idx);
}
void STable::removeColumns(size_t off, size_t len) {
	_columns.remove(off, len);
    sforeach(_rows) E_.remove(off, len);
}
void STable::removeColumns(const srange &range) {
	_columns.remove(range);
    sforeach(_rows) E_.remove(range);
}
void STable::clearColumns() { _columns.clear(); }
void STable::resizeColumn(size_t s) {
	if (s < columnCount()) _columns.resize(s);
	else if (columnCount() < s) {
		sforin(r, rowCount(), s) addColumn();
	}
}
void STable::sortBy(size_t idx, ORDER order) {
    std::sort(_rows.begin(), _rows.end(),
              [idx, order] (const sobj &so1, const sobj &so2) {
                  if (order == ASC) return so1[idx] < so2[idx];
                  else return so2[idx] < so1[idx];
              });
}
size_t STable::rowCount() const { return _rows.size(); }
sobj &STable::operator[](int idx) { return _rows[idx]; }
const sobj &STable::operator[](int idx) const { return _rows[idx]; }
SArray &STable::row(int idx) { return _rows[idx].array(); }
const SArray &STable::row(int idx) const { return _rows[idx].array(); }
SArray& STable::rows() { return _rows; }
const SArray &STable::rows() const { return _rows; }
sobj STable::namedRow(int idx) const {
    sdict dict;
    sforeachi(_columns) dict[column(idx).name()] = _rows[idx][i];
    return dict;
}
void STable::addRowArray(const SArray &array) {
	_rows.add(SArray(columnCount()));
	setRowArray(_rows.size()-1, array);
}
void STable::addRowDict(const SDictionary &dict) {
	_rows.add(SArray(columnCount()));
	setRowDict(_rows.size()-1, dict);
}
void STable::addRow(const sobj &obj) {
    if (obj.isNull()) _rows.add(SArray(columnCount()));
    else if (obj.isArray()) addRowArray(obj.array());
    else if (obj.isDict()) addRowDict(obj.dict());
}
void STable::insertRowArray(size_t idx, const SArray& array) {
	_rows.insert(idx, SArray(columnCount()));
	setRowArray(idx, array);
}
void STable::insertRowDict(size_t idx, const SDictionary& dict) {
	_rows.insert(idx, SArray(columnCount()));
	setRowDict(idx, dict);
}
void STable::insertRow(size_t idx, const sobj &obj) {
	if (obj.isNull()) _rows.insert(idx, SArray(columnCount()));
	else if (obj.isArray()) insertRowArray(idx, obj.array());
    else if (obj.isDict()) insertRowDict(idx, obj.dict());
}
void STable::setRowArray(size_t idx, const SArray &array) {
    auto &row = _rows[idx];
	auto cnum = columnCount(), size = array.size();
	if (cnum < size) {
		sforin(i, cnum, size) addColumn();
		auto rit = row.begin();
		sforeach(array) { *rit = E_; ++rit; }
	}
	else {
		auto rit = row.begin();
		sforeach(array) { *rit = E_; ++rit; }
		sforin(i, size, cnum) { *rit = snull; ++rit; }
	}
}
void STable::setRowDict(size_t idx, const SDictionary &dict) {
    auto &row = _rows[idx];
    sforeach(dict) {
        auto idx = columnIndex(E_.key);
        if (idx == NOT_FOUND) continue;
        row[idx] = E_.value;
    }
}
void STable::setRow(size_t idx, const sobj &obj) {
    if (obj.isNull()) sforeach(_rows[idx]) E_ = snull;
    else if (obj.isArray()) setRowArray(idx, obj.array());
    else if (obj.isDict()) setRowDict(idx, obj.dict());
}
void STable::removeRow(int idx) { _rows.removeAt(idx); }
void STable::removeRows(size_t off, size_t len) { _rows.remove(off, len); }
void STable::removeRows(const srange &range) { _rows.remove(range); }
void STable::clearRows() { _rows.clear(); }
void STable::swapRow(size_t i1, size_t i2) { _rows[i1].swap(_rows[i2]); }
void STable::resizeRow(size_t s) { 
	if (s < rowCount()) _rows.resize(s);
	else if (rowCount() < s) {
		sforin(r, rowCount(), s) addRow();
	}
}
void STable::resize(size_t r, size_t c) {
	resizeColumn(c);
	resizeRow(r);
}
sobj &STable::value(int row, int col) { return _rows[row][col]; }
const sobj &STable::value(int row, int col) const { return _rows[row][col]; }
void STable::setValue(int row, int col, const sobj &obj) { _rows[row][col] = obj; }
void STable::clear() {
    _columns.clear();
	_rows.clear();
    _name = "";
}
svec2d<size_t> STable::find(const sobj &obj, smath::DIRECTION dir, size_t offset) {
    if (dir == smath::HORIZONTAL) {
		sforin(r, offset, rowCount()) {
			sforin(c, 0, columnCount()) { if (value(r, c) == obj) return svec2d<size_t>(r, c); }
		}
	}
	else {
		sforin(c, offset, columnCount()) {
			sforin(r, 0, rowCount()) { if (value(r, c) == obj) return svec2d<size_t>(r, c); }
		}
	}
	return svec2d<size_t>(NOT_FOUND, NOT_FOUND);
}
void STable::toMatrix(smat<sobj>& mat) {
	mat.resize(rowCount(), columnCount());
    sforin(r, 0, rowCount()) {
        sforin(c, 0, columnCount()) mat[r][c] = value(r, c);
    }
}
String STable::getClass() const { return "table"; }
String STable::toString() const {
	String str = _name == "" ? "No name" : _name;
	str << NEW_LINE;
	if (columnCount()) {
		sforeach(_columns) str << E_.column().name() << "(" << SColumn::colTypeStr(E_.type()) << "),";
		str << NEW_LINE;
    }
    if (rowCount()) {
		sforeach(_rows) str << E_.array().toString() << NEW_LINE;
    }
    return str;
}
SObject * STable::clone() const { return new STable(*this); }