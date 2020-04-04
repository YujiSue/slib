#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"

using namespace slib;
using namespace slib::sio;

SColumn::SColumn() : _type(0), _name(""), _table(nullptr), SArray() {}
SColumn::SColumn(int type, const char *name) : _type(type), _name(name?name:""), _table(nullptr), SArray() {}
SColumn::SColumn(int type, const char *name, const SArray &array) : _type(type), _name(name?name:""), _table(nullptr), SArray(array) {}
SColumn::SColumn(int type, const char *name, SArray &&array)
: _type(type), _name(name?name:""), _table(nullptr), SArray(std::forward<SArray>(array)) {}
SColumn::SColumn(SColumn *col, STable *table) : _type(col->_type), _name(col->_name), _table(col->_table) {}
SColumn::SColumn(const SColumn &column) : SColumn(column.type(), column.name()) {
    _table = column._table;
    if (!column.empty()) copy(column.ptr(), column.size());
}
SColumn::~SColumn() {}

sushort SColumn::colType(const sobj &obj) {
    if (obj.isNum()) return NUMBER_COLUMN;
    else if (obj.isStr()) return STRING_COLUMN;
    else if (obj.isDate()) return DATE_COLUMN;
    else if (obj.isDat()) return DATA_COLUMN;
    else if (obj.isArray()) return ARRAY_COLUMN;
    else return DICT_COLUMN;
}
String SColumn::colTypeStr(int t) {
    switch (t) {
        case NUMBER_COLUMN:
            return "number";
            break;
        case INTEGER_COLUMN:
            return "integer";
            break;
        case REAL_COLUMN:
            return "real";
            break;
        case BOOL_COLUMN:
            return "boolean";
            break;
        case STRING_COLUMN:
            return "string";
            break;
        case TEXT_COLUMN:
            return "text";
            break;
        case DATE_COLUMN:
            return "date";
            break;
        case DATA_COLUMN:
            return "data";
            break;
        case ARRAY_COLUMN:
            return "array";
            break;
        case DICT_COLUMN:
            return "dict";
            break;
        default:
            return "unknown";
            break;
    }
}
sushort SColumn::colTypeIndex(const char *t) {
    auto type = SString::lower(t);
    if (type == "number") return NUMBER_COLUMN;
    else if (type == "integer") return INTEGER_COLUMN;
    else if (type == "real") return REAL_COLUMN;
    else if (type == "boolean") return BOOL_COLUMN;
    else if (type == "string") return STRING_COLUMN;
    else if (type == "text") return TEXT_COLUMN;
    else if (type == "date") return DATE_COLUMN;
    else if (type == "data") return DATA_COLUMN;
    else if (type == "array") return ARRAY_COLUMN;
    else if (type == "dict") return DICT_COLUMN;
    else return 0;
}

sushort SColumn::type() const { return _type; }
const String &SColumn::name() const { return _name; }

void SColumn::setType(sushort t) {
    if (_type == t || empty()) return;
    if (_table) {
        auto idx = _table->columnIndex(_name);
        sforeach(_table->_rows) {
            
        }
    }
    else if (!empty()) {
        
        
    }
    _type = t;
}
void SColumn::setName(const char *n) { _name = n?n:""; }
void SColumn::setTable(STable *tbl) { _table = tbl; }
String SColumn::getClass() const { return "column"; }
String SColumn::toString() const {
    String str = "column:{type="+SColumn::colTypeStr(_type)+";name="+_name+";data=[";
    if (_table) {
        auto idx = _table->columnIndex(_name);
        if (_table->_rows.empty()) str += "]}";
        else {
            sforeach(_table->_rows) str<<E_[idx]<<",";
            str.last() = ']'; str += "}";
        }
    }
    else {
        if (empty()) str += "]}";
        else {
            sforeach(*this) str<<E_<<",";
            str.last() = ']'; str += "}";
        }
    }
    return str;
}
SObject *SColumn::clone() const { return new SColumn(*this); }