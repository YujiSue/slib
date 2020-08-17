#include "sobj/sfunc.h"
#include "sobj/stable.h"
#include "sio/sfile.h"
#include "sutil/sjson.h"
#include "smedia/simage.h"

using namespace slib;
using namespace slib::sio;

SColumn::SColumn(int type, const char *name, const SArray &array) : _type(type), _name(name?name:""), _table(nullptr), SArray(array) {}
SColumn::SColumn(SArray &&array) : _type(0), _name(""), _table(nullptr), SArray(std::forward<SArray>(array)) {}
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
	else if (obj.isDict()) return DICT_COLUMN;
	else if (obj.isFunc()) return FUNC_COLUMN;
	else if (obj.isImg()) return IMG_COLUMN;
    else return OBJECT_COLUMN;
}
String SColumn::colTypeStr(int t) {
    switch (t) {
		case OBJECT_COLUMN:
			return "auto";
			break;
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
	if (type == "auto") return OBJECT_COLUMN;
	else if (type.beginWith("num")) return NUMBER_COLUMN;
	else if (type.beginWith("int")) return INTEGER_COLUMN;
    else if (type == "real") return REAL_COLUMN;
    else if (type.beginWith("bool")) return BOOL_COLUMN;
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
inline void _setInitObj(sobj& obj, sushort to) {
	switch (to)
	{
	case DATE_COLUMN:
		obj = SDate();
		break;
	case DATA_COLUMN:
		obj = SData();
		break;
	case ARRAY_COLUMN:
		obj = SArray();
		break;
	case DICT_COLUMN:
		obj = SDictionary();
		break;
	case FUNC_COLUMN:
		obj = SFunction<sobj(sobj)>();
		break;
	case IMG_COLUMN:
		obj = smedia::SImage();
		break;
	default:
		break;
	}
}
inline void _convert(sobj &obj, sushort from, sushort to) {
	if (from & NUMBER_COLUMN) {
		if (to & NUMBER_COLUMN) {
			if (to == INTEGER_COLUMN) obj.number().setType(SNumber::INTEGER);
			else if (to == REAL_COLUMN) obj.number().setType(SNumber::REAL);
			else if (to == BOOL_COLUMN) obj.number().setType(SNumber::BOOL);
		}
		else if (to & STRING_COLUMN) obj = obj.toString();
		else _setInitObj(obj, to);
	}
	else if (from & STRING_COLUMN) {
		if (to & NUMBER_COLUMN) {
			if (obj.toString().isNumeric()) obj = SNumber::toNumber(obj);
			else obj = 0;
			if (to == INTEGER_COLUMN) obj.number().setType(SNumber::INTEGER);
			else if (to == REAL_COLUMN) obj.number().setType(SNumber::REAL);
			else if (to == BOOL_COLUMN) obj.number().setType(SNumber::BOOL);
		}
		else if (to & STRING_COLUMN) obj = obj.toString();
		else if (to == DATE_COLUMN) obj = SDate(obj.toString(), slib::sstyle::YMDHMS);
		else if (to == DATA_COLUMN) obj = SData(obj.toString().length(), (void *)obj.toString().cstr());
		else _setInitObj(obj, to);
	}
	else if (to & STRING_COLUMN) obj = obj.toString();
	else _setInitObj(obj, to);
}
void SColumn::convert(sushort t) {
    if (_type == t || empty()) return;
    if (_table) {
        auto idx = _table->columnIndex(_name);
        sforeach(_table->_rows) _convert(E_[idx], _type, t);
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