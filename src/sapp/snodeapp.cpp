#include "snodeapp.h"

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
bool SNodeUtil::gypInstalled() {
	String result;
	SSystem::exec("node-gyp -v", result);
	return result.equal(R(/ ^ v[0 - 9] + .[0 - 9] + .[0 - 9] / ));
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
napi_value SNodeUtil::jsnum(napi_env env, SNumber& n) {
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
napi_value SNodeUtil::jsarray(napi_env env, SArray& array) {
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
napi_value SNodeUtil::jsdict(napi_env env, SDictionary& dict) {
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
/*
void SNodeApp::makeMain(const char *path, const char *cname) {
	sio::SFile main(path);
	main << "#include \"wrapper.h\"\n" <<
		"using namespace slib::sapp;\n" <<
		"napi_value Init(napi_env env, napi_value exports) {\n\treturn " <<
		cname << "::Init(env, exports);\n" << "NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)";
}
*/
/*
void SNodeApp::makeWrapper() {
	SString header, source;
	header << "#include \"sapp/snodeapp.h\"\n" <<
		"using namespace slib;\n" <<
		"using namespace slib::sapp;\n" <<
		"class " << profile["class"]["name"] << " {\n" <<
		"private:\n" <<
		"\tnapi_env _env;\n" <<
		"\tnapi_ref _wrapper;\n" <<
		"\tstatic napi_ref constructor;\n" <<
		"public:\n" <<
		"\tstatic napi_value Init(napi_env env, napi_value exports);\n" <<
		"\tstatic void Destructor(napi_env env, void* object, void* finalize);\n" <<
		"private:\n" <<
		"\texplicit " << profile["class"]["name"] << "();\n" <<
		"\tvirtual ~" << profile["class"]["name"] << "();\n" <<
		"\tstatic napi_value New(napi_env env, napi_callback_info info);\n";
	if (!profile["class"]["method"].empty()) {
		sforeach(profile["class"]["method"]) {
			header << "\tstatic napi_value " << E_ << "(napi_env env, napi_callback_info info);\n";
		}
	}
	header << "};";
	header.save(sio::SFile(profile["code"]["include"]).child("wrapper.h"));

	source << "#include \"wrapper.h\"\n" <<
		"napi_ref " << profile["class"]["name"] << "::constructor;\n" <<
		profile["class"]["name"] << "::" << profile["class"]["name"] << "() : _env(nullptr), _wrapper(nullptr) {\n" <<
		"\t// Insert C++ class construction code.\n}\n" <<
		profile["class"]["name"] << "::~" << profile["class"]["name"] << "() {\n" <<
		"\tnapi_delete_reference(_env, _wrapper);\n" <<
		"\t// Insert C++ class destruction code.\n}\n" <<
		"napi_value " << profile["class"]["name"] << "::Init(napi_env env, napi_value exports) {\n" <<
		"\tnapi_status status;\n" <<
		"\tnapi_property_descriptor properties[] = {\n";
	if (!profile["class"]["method"].empty()) {
		sforeach(profile["class"]["method"]) {
			source << "\t\tDECLARE_NAPI_METHOD(\"" << E_ << "\", " << E_ <<"),\n";
		}
	}
	source << "\t};\n" <<
		"\tnapi_value cons;\n" <<
		"\tstatus = napi_define_class(env, \"" << profile["class"]["name"] << "\", NAPI_AUTO_LENGTH, New, nullptr, " << profile["class"]["method"].size() << ", properties, &cons);\n" <<
		"\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\tstatus = napi_create_reference(env, cons, 1, &constructor);\n" <<
		"\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\tstatus = napi_set_named_property(env, exports, \"" << profile["class"]["name"] << "\", cons);\n" <<
		"\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\treturn exports;\n" <<
		"}\n" <<
		"void " << profile["class"]["name"] << "::Destructor(napi_env env, void* object, void* finalize) {\n" <<
		"\treinterpret_cast<" << profile["class"]["name"] << "*>(object)->~" << profile["class"]["name"] << "();\n" <<
		"napi_value " << profile["class"]["name"] << "::New(napi_env env, napi_callback_info info) {\n" <<
		"\tnapi_status status;\n" <<
		"\tnapi_value target;\n" <<
		"\tstatus = napi_get_new_target(env, info, &target);\n" <<
		"\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\tbool is_constructor = target != nullptr;\n" <<
		"\tif (is_constructor) {\n" <<
		"\t\tsize_t argc = 1;\n" <<
		"\t\tnapi_value args[1];\n" <<
		"\t\tnapi_value jsthis;\n" <<
		"\t\tstatus = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);\n" <<
		"\t\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\t\t" << profile["class"]["name"] << "* instance = new " << profile["class"]["name"] << "();\n" <<
		"\t\tinstance->_env = env;\n" <<
		"\t\tstatus = napi_wrap(env, jsthis, reinterpret_cast<void*>(instance), " << profile["class"]["name"] << "::Destructor, nullptr, &instance->_wrapper);\n" <<
		"\t\treturn jsthis;\n" <<
		"\t}\n" <<
		"\telse {\n" <<
		"\t\tsize_t argc_ = 1;\n" <<
		"\t\tnapi_value args[1];\n" <<
		"\t\tstatus = napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr);\n" <<
		"\t\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\t\tconst size_t argc = 1;\n" <<
		"\t\tnapi_value argv[argc] = { args[0] };\n" <<
		"\t\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\t\tnapi_value cons;\n" <<
		"\t\tstatus = napi_get_reference_value(env, constructor, &cons);\n" <<
		"\t\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\t\tnapi_value instance;\n" <<
		"\t\tstatus = napi_new_instance(env, cons, argc, argv, &instance);\n" <<
		"\t\tif (status != napi_ok) napi_throw_error(env, NULL, "");\n" <<
		"\t\treturn instance;\n" <<
		"\t}\n}\n";
	if (!profile["class"]["method"].empty()) {
		sforeach(profile["class"]["method"]) {
			source << "napi_value " << profile["class"]["name"] << "::" << E_ << "(napi_env env, napi_callback_info info) {\n" <<
				"\t// Insert code. \n}\n";
		}
	}
}
*/
/*
void SNodeApp::makeGypBind(const char *path, sobj profile) {
	sio::SJson gyp;
	gyp.set("targets", SArray());
	SDictionary appinfo =
	{
		kv("target_name", profile["app"]["name"]),
		kv("sources", profile["code"]["src"]),
		kv("include_dirs", profile["code"]["include"]),
		kv("libraries", profile["library"]),
		kv("conditions", profile["platform"])
	};
	gyp["targets"].add(appinfo);
	gyp.save(path);
}
*/

