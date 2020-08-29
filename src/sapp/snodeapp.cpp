#include "sapp/snodeapp.h"

using namespace slib;
using namespace slib::sapp;

bool SNodeUtil::nodeInstalled() {
	String result;
	SSystem::exec("node -v", result);
	if (result.equal(R(/ ^ v[0 - 9] + .[0 - 9] + .[0 - 9]/))) {
		SSystem::exec("npm -v", result);
		return result.equal(R(/ ^ [0 - 9] + .[0 - 9] + .[0 - 9] / ));
	}
	return false;
}
bool SNodeUtil::ngypInstalled() {
	String result;
	SSystem::exec("node-gyp -v", result);
	return result.equal(R(/ ^ v[0 - 9] + .[0 - 9] + .[0 - 9] / ));
}
//void SNodeUtil::installNode() {}
void SNodeUtil::installNGyp() {
	SSystem::exec("npm install -g node-gyp");
}
SNodeUtil::JS_OBJ_TYPE SNodeUtil::jsType(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	napi_valuetype type;
	bool b;
	st = napi_is_array(env, val, &b);
	if (b) return JS_ARRAY;
	st = napi_typeof(env, val, &type);
	switch (type) {
	case napi_null:
		return JS_NULL;
	case napi_number:
		return JS_NUMBER;
	case napi_boolean:
		return JS_BOOL;
	case napi_string:
		return JS_STRING;
	case napi_object:
		return JS_DICT;
	default:
		break;
	}
	return JS_NULL;
}
size_t SNodeUtil::jsStrlen(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	size_t len;
	st = napi_get_value_string_utf8(env, val, NULL, 0, &len);
	return len;
}
napi_value SNodeUtil::jsnull(napi_env env) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_get_null(env, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsbool(napi_env env, bool b) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_get_boolean(env, b, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsint(napi_env env, int i) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_int32(env, i, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsuint(napi_env env, unsigned int ui) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_uint32(env, ui, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jslong(napi_env env, sint i) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_int64(env, i, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsreal(napi_env env, double d) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_double(env, d, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsnum(napi_env env, const SNumber& n) {
	napi_value val;
	switch (n.type()) {
	case SNumber::INTEGER:
		val = SNodeUtil::jsint(env, n.intValue());
		break;
	case SNumber::UINTEGER:
		val = SNodeUtil::jsuint(env, n.uintValue());
		break;
	case SNumber::REAL:
		val = SNodeUtil::jsreal(env, n.real());
		break;
	case SNumber::BOOL:
		val = SNodeUtil::jsbool(env, n.boolean());
		break;
	default:
		val = SNodeUtil::jsint(env, 0);
		break;
	}
	return val;
}
napi_value SNodeUtil::jsstr(napi_env env, const char* s) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_string_utf8(env, s, NAPI_AUTO_LENGTH, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return val;
}
napi_value SNodeUtil::jsarray(napi_env env, const SArray& array) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_array_with_length(env, array.size(), &val);
	if (st != napi_ok) napi_throw_error(env, "jsarray", "JSArray create error.");
	if (!array.empty()) {
		sforeachi(array) {
			st = napi_set_element(env, val, i, SNodeUtil::jsobj(env, array[i]));
		}
	}
	return val;
}
napi_value SNodeUtil::jsdict(napi_env env, const SDictionary& dict) {
	napi_status st = napi_generic_failure;
	napi_value val;
	st = napi_create_object(env, &val);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	if (!dict.empty()) {
		sforeachk(dict) {
			st = napi_set_property(env, val, SNodeUtil::jsstr(env, E_.key), SNodeUtil::jsobj(env, E_.value));
		}
	}
	return val;
}
napi_value SNodeUtil::jsobj(napi_env env, sobj obj) {
	napi_value val;
	if (obj.isNull()) napi_get_null(env, &val);
	else if (obj.isNum()) val = SNodeUtil::jsnum(env, obj.number());
	else if (obj.isStr()) val = SNodeUtil::jsstr(env, obj.string());
	else if (obj.isArray()) val = SNodeUtil::jsarray(env, obj.array());
	else if (obj.isDict()) val = SNodeUtil::jsdict(env, obj.dict());
	else val = SNodeUtil::jsstr(env, obj.toString());
	return val;
}

bool SNodeUtil::toBool(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	bool b;
	st = napi_get_value_bool(env, val, &b);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return b;
}

int SNodeUtil::toInt(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	int i;
	st = napi_get_value_int32(env, val, &i);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return i;
}

unsigned int SNodeUtil::toUint(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	unsigned int ui;
	st = napi_get_value_uint32(env, val, &ui);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return ui;
}

sinteger SNodeUtil::toLong(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	sinteger i;
	st = napi_get_value_int64(env, val, &i);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return i;
}

sreal SNodeUtil::toReal(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	sreal r;
	st = napi_get_value_double(env, val, &r);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return r;
}

SNumber SNodeUtil::toNum(napi_env env, napi_value val) {
	SNumber num;
	auto type = SNodeUtil::jsType(env, val);
	switch (type) {
	case JS_NUMBER:
		num = SNodeUtil::toReal(env, val);
		break;
	case JS_BOOL:
		num = SNodeUtil::toBool(env, val);
		break;
	default:
		break;
	}
	return num;
}
String SNodeUtil::toStr(napi_env env, napi_value val) {
	napi_status st = napi_generic_failure;
	size_t len = jsStrlen(env, val);
	SString str(len, '\0');
	st = napi_get_value_string_utf8(env, val, str.ptr(), len+1, &len);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	return str;
}
SArray SNodeUtil::toArray(napi_env env, napi_value val) {
	SArray array;
	napi_status st = napi_generic_failure;
	uint32_t size;
	st = napi_get_array_length(env, val, &size);
	if (st != napi_ok) napi_throw_error(env, "toArray", "Get array error.");
	if (size) {
		sforin(i, 0, size) {
			napi_value e;
			st = napi_get_element(env, val, i, &e);
			if (st != napi_ok) napi_throw_error(env, "toArray", "Get array element error.");
			array.add(SNodeUtil::toObj(env, e));
		}
	}
	return array;
}
intarray SNodeUtil::toIArray(napi_env env, napi_value val) {
	intarray array;
	napi_status st = napi_generic_failure;
	uint32_t size;
	st = napi_get_array_length(env, val, &size);
	if (st != napi_ok) napi_throw_error(env, "toArray", "Get array error.");
	if (size) {
		sforin(i, 0, size) {
			napi_value e;
			st = napi_get_element(env, val, i, &e);
			if (st != napi_ok) napi_throw_error(env, "toArray", "Get array element error.");
			array.add(SNodeUtil::toInt(env, e));
		}
	}
	return array;
}
stringarray SNodeUtil::toStrArray(napi_env env, napi_value val) {
	stringarray array;
	napi_status st = napi_generic_failure;
	uint32_t size;
	st = napi_get_array_length(env, val, &size);
	if (st != napi_ok) napi_throw_error(env, "toArray", "Get array error.");
	if (size) {
		sforin(i, 0, size) {
			napi_value e;
			st = napi_get_element(env, val, i, &e);
			if (st != napi_ok) napi_throw_error(env, "toArray", "Get array element error.");
			array.add(SNodeUtil::toStr(env, e));
		}
	}
	return array;
}
SDictionary SNodeUtil::toDict(napi_env env, napi_value val) {
	SDictionary dict;
	napi_status st = napi_generic_failure;
	napi_value keys;
	st = napi_get_property_names(env, val, &keys);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	uint32_t size;
	st = napi_get_array_length(env, keys, &size);
	if (st != napi_ok) napi_throw_error(env, NULL, "");
	if (size) {
		sforin(i, 0, size) {
			napi_value key, obj;
			st = napi_get_element(env, keys, i, &key);
			if (st != napi_ok) napi_throw_error(env, NULL, "");
			st = napi_get_property(env, val, key, &obj);
			if (st != napi_ok) napi_throw_error(env, NULL, "");
			dict[SNodeUtil::toStr(env, key)] = SNodeUtil::toObj(env, obj);
		}
	}
	return dict;
}
sobj SNodeUtil::toObj(napi_env env, napi_value val) {
	sobj obj;
	switch (SNodeUtil::jsType(env, val)) {
	case JS_NULL:
		obj = snull;
		break;
	case JS_NUMBER:
		obj = SNodeUtil::toNum(env, val);
		break;
	case JS_BOOL:
		obj = SNodeUtil::toBool(env, val);
		break;
	case JS_STRING:
		obj = SNodeUtil::toStr(env, val);
		break;
	case JS_ARRAY:
		obj = SNodeUtil::toArray(env, val);
		break;
	case JS_DICT:
		obj = SNodeUtil::toDict(env, val);
		break;
	default:
		break;
	}
	return obj;
}
