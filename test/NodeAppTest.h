#include "sapp/snodeapp.h"

using namespace slib;

class NodeAppTest : public sapp::SNodeApp<NodeAppTest> {
public:
	NodeAppTest();
	~NodeAppTest();

public:
	static String className();
	static Array<napi_property_descriptor> descriptor();

	static napi_value TestNum(napi_env env, napi_callback_info info);
	static napi_value TestStr(napi_env env, napi_callback_info info);
	static napi_value TestArrayI(napi_env env, napi_callback_info info);
	static napi_value TestArrayO(napi_env env, napi_callback_info info);
	static napi_value TestObjI(napi_env env, napi_callback_info info);
	static napi_value TestObjO(napi_env env, napi_callback_info info);
	void init();
};
