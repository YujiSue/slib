#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"

using namespace slib;
using namespace slib::sio;

STable::STable() : SObject() {}
STable::STable(const char *name, const Array<scolumn> &cols, const SArray &rows) : STable() {
    _name = name;
    sforeach(cols) addColumn(E_);
    sforeach(rows) addRow(E_);
}
STable::STable(const smat<sobj> &mat) : STable() {
    sforin(i, 0, mat.col) {
        auto val = mat[0][i];
        auto col_name = SString("col")+(i+1);
        if(val.isNum()) addColumn(scolumn(SColumn::NUMBER_COLUMN, col_name));
        else if(val.isStr()) addColumn(scolumn(SColumn::STRING_COLUMN, col_name));
        else if(val.isStr()) addColumn(scolumn(SColumn::TEXT_COLUMN, col_name));
        else if(val.isDate()) addColumn(scolumn(SColumn::DATE_COLUMN, col_name));
        else if(val.isDat()) addColumn(scolumn(SColumn::DATA_COLUMN, col_name));
        else if(val.isArray()) addColumn(scolumn(SColumn::ARRAY_COLUMN, col_name));
        else if(val.isDict()) addColumn(scolumn(SColumn::DICT_COLUMN, col_name));
        else if(val.isImg()) addColumn(scolumn(SColumn::IMG_COLUMN, col_name));
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
        if (obj["col"]) { sforeach(obj["col"]) addColumn(scolumn(E_["type"], E_["name"])); }
        if (obj["row"]) _rows = obj["row"];
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

void STable::load(const SDictionary &dict) {
    clear();
    if (!dict["path"]) return;
    if (dict["type"]) {
        if (dict["type"] == "TSV") loadTxt(dict["path"], "\t", dict["header"]);
        else if (dict["type"] == "CSV") loadTxt(dict["path"], ",", dict["header"]);
        else if (dict["type"] == "JSON") {
            SJson tbl; tbl.load(dict["path"]);
            
            
            
        }
        //else if (param["type"] == "XLS" || param["type"] == "XLSX") loadXls();
        //else if (dict["type"] == "SOML")
    }
}

void STable::load(const char *path) {
    clear();
    auto ext = SFile(path).extension();
    if (ext == "csv") loadTxt(path, ",", true);
    else if (ext == "txt") loadTxt(path, "\t", true);
    else if (ext == "json") {
        
    }
    //else if (ext == "xls" || ext == "xlsx") loadXls();
    //else if (dict["type"] == "soml")
}

void STable::loadTxt(const char *path, const char *sep, bool header) {
    clear();
    String row;
    SFile file(path);
    file.open();
    file.readLine(row);
    auto values = row.split(sep);
    if (header) {
        sforeach(values) addColumn(scolumn(E_.isNumeric()?SColumn::NUMBER_COLUMN:SColumn::TEXT_COLUMN, E_));
    }
    else {
        sforeachi(values) addColumn(scolumn(values[i].isNumeric()?SColumn::NUMBER_COLUMN:SColumn::TEXT_COLUMN, SString("col")+(i+1)));
        addRow(sarray(values));
    }
    while (!file.eof()) {
        file.readLine(row);
        values = row.split(sep);
        auto obj = sarray(values);
        addRow(sarray(values));
    }
}

void STable::save(const char *path) {
    clear();
    auto ext = SFile(path).extension();
    if (ext == "csv") saveTxt(path, ",");
    else if (ext == "txt") saveTxt(path, "\t");
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

void STable::addColumn(const scolumn &col) {
	_columns.add(col); _columns.last().column().setTable(this);
    if (rowCount()) { sforeach(_rows) E_.add(snull); }
    if (col->size()) {
        auto row_ = col->size()<rowCount()?col->size():rowCount();
        sforin(r, 0, row_) _rows[r][-1] = col->at(r);
        if (rowCount() < col->size()) {
            sforin(r, rowCount(), col->size()) { addRow(); _rows.last()[-1] = col->at(r); }
        }
    }
	_columns.last().clear();
}
void STable::insertColumn(size_t idx, const scolumn &col) {
	_columns.insert(idx, col); 
	column(idx).setTable(this);
    if (rowCount()) { sforeach(_rows) E_.insert(idx, snull); }
    if (col.size()) {
        auto row_ = col.size()<rowCount()?col.size():rowCount();
        sforin(r, 0, row_) _rows[r][idx] = col->at(r);
        if (rowCount() < col.size()) {
            sforin(r, rowCount(), col.size()) { addRow(); _rows.last()[idx] = col->at(r); }
        }
    }
	_columns[idx].clear();
}
void STable::setColumn(size_t idx, const scolumn &col) {
	_columns.set(idx, col); 
	column(idx).setTable(this);
    if (rowCount()) { sforeach(_rows) E_.set(idx, snull); }
    if (col.size()) {
        auto row_ = col.size()<rowCount()?col.size():rowCount();
        sforin(r, 0, row_) _rows[r][idx] = col[r];
        if (rowCount() < col.size()) {
            sforin(r, rowCount(), col.size()) { addRow(); _rows.last()[idx] = col->at(r); }
        }
    }
	_columns[idx].clear();
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
void STable::resizeColumn(size_t s) { _columns.resize(s); }
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
sdict STable::namedRow(int idx) const {
    sdict dict;
    sforeachi(_columns) dict[column(idx).name()] = _rows[idx][i];
    return dict;
}

//void STable::addRow() { _rows.add(SArray(columnCount())); }
void STable::_addRow(const SArray &array) {
    addRow(); setRow(_rows.size()-1, array);
}
void STable::_addRow(const stringarray &array) {
    addRow(); setRow(_rows.size()-1, array);
}
void STable::_addRow(const SDictionary &dict) {
    addRow(); setRow(_rows.size()-1, dict);
}
void STable::addRow(const sobj &obj) {
    if (obj.isNull()) _rows.add(SArray(columnCount()));
    else if (obj.isArray()) _addRow(obj.array());
    else if (obj.isDict()) _addRow(obj.dict());
}
void STable::insertRow(size_t idx, const sobj &obj) {
	_rows.insert(idx, sarray(columnCount()));
    auto &row = _rows[idx].array();
    if (obj.isArray()) {
        auto &row_ = obj.array();
        auto col_ = columnCount()<obj.size()?columnCount():obj.size();
        sforin(c, 0, col_) row[c] = row_[c];
    }
    else if (obj.isDict()) {
        auto &row_ = obj.dict();
        sforeach(row_) {
            auto idx = columnIndex(E_.key);
            if (idx != NOT_FOUND) row[idx] = E_.value;
        }
    }
    else { row.reset(obj); }
}
void STable::setRow(size_t idx, const SArray &array) {
    auto &row = _rows[idx];
    auto size = columnCount() < array.size()?columnCount():array.size();
    auto it = row.begin(); auto it_ = array.begin();
    sforin(i, 0, size) { E_ = *it_; NEXT_; ++it_; }
}
void STable::setRow(size_t idx, const stringarray &array) {
    auto &row = _rows[idx];
    auto size = columnCount() < array.size()?columnCount():array.size();
    auto col = _columns.begin(); auto it = row.begin(); auto it_ = array.begin();
    sforin(i, 0, size) {
        auto type = col->type();
        if (type & SColumn::NUMBER_COLUMN) E_ = it_->number();
        else if (type & SColumn::STRING_COLUMN) {
            if (it_->isQuoted()) E_ = String::dequot(*it_);
            else E_ = SString(*it_);
        }
        /*
         *
         */
        else {
            if (it_->isQuoted()) E_ = String::dequot(*it_);
            else E_ = SString(*it_);
        }
        E_ = *it_; NEXT_; ++it_; ++col;
    }
}
void STable::setRow(size_t idx, const SDictionary &dict) {
    auto &row = _rows[idx];
    sforeach(dict) {
        auto idx = columnIndex(E_.key);
        if (idx == NOT_FOUND) continue;
        row[idx] = E_.value;
    }
}
void STable::setRow(size_t idx, const sobj &obj) {
    if (obj.isNull()) sforeach(_rows[idx]) E_ = snull;
    else if (obj.isArray()) setRow(idx, obj.array());
    else if (obj.isDict()) setRow(idx, obj.dict());
}

void STable::removeRow(int idx) { _rows.removeAt(idx); }
void STable::removeRows(size_t off, size_t len) { _rows.remove(off, len); }
void STable::removeRows(const srange &range) { _rows.remove(range); }
void STable::clearRows() { _rows.clear(); }
void STable::swapRow(size_t i1, size_t i2) { _rows[i1].swap(_rows[i2]); }
void STable::resizeRows(size_t s) { _rows.resize(s); }

sobj &STable::value(int row, int col) { return _rows[row][col]; }
const sobj &STable::value(int row, int col) const { return _rows[row][col]; }
void STable::setValue(int row, int col, const sobj &obj) { _rows[row][col] = obj; }
void STable::clear() {
    _columns.clear();
	_rows.clear();
    _name = "";
}

std::pair<size_t, size_t> STable::find(const sobj &obj, smath::DIRECTION dir, size_t offset) {
    if (dir == smath::HORIZONTAL) {
		sforin(r, offset, rowCount()) {
			sforin(c, 0, columnCount()) { if (value(r, c) == obj) return std::pair<size_t, size_t>(r, c); }
		}
	}
	else {
		sforin(c, offset, columnCount()) {
			sforin(r, 0, rowCount()) { if (value(r, c) == obj) return std::pair<size_t, size_t>(r, c); }
		}
	}
	return std::pair<size_t, size_t>(NOT_FOUND, NOT_FOUND);
}
smat<sobj> STable::toMatrix() {
    smat<sobj> mat(rowCount(), columnCount());
    sforin(r, 0, rowCount()) {
        sforin(c, 0, columnCount()) mat[r][c] = value(r, c);
    }
    return mat;
}

String STable::getClass() const { return "table"; }
String STable::toString() const {
    String str = String("table:{name=")<<_name<<";col=[";
    if (columnCount()) {
        sforeach(_columns)
        str<<E_.column().name()<<":"<<SColumn::colTypeStr(E_.type())<<",";
        str.last() = ']';
    }
    else str<<"]";
    str<<";row=";
    if (rowCount()) {
        sforeach(_rows) str<<"["<<E_.array().toString()<<"],";
        if (str.size()) str.resize(str.length()-1);
    }
    else str<<"[]";
    str<<"}";
    return str;
}
SObject * STable::clone() const { return new STable(*this); }