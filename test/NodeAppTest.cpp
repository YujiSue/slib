#include "NodeAppTest.h"

NodeAppTest::NodeAppTest() : slib::sapp::SNodeApp<NodeAppTest>() {}
NodeAppTest::~NodeAppTest() {}
slib::String NodeAppTest::className() { return "NodeAppTest"; }
Array<napi_property_descriptor> NodeAppTest::descriptor() {
	return Array<napi_property_descriptor>({
		DECLARE_NAPI_METHOD("TestNum", TestNum),
		DECLARE_NAPI_METHOD("TestStr", TestStr),
		DECLARE_NAPI_METHOD("TestArrayI", TestArrayI),
		DECLARE_NAPI_METHOD("TestArrayO", TestArrayO),
		DECLARE_NAPI_METHOD("TestObjI", TestObjI),
		DECLARE_NAPI_METHOD("TestObjO", TestObjO)
	});
}

napi_value NodeAppTest::TestNum(napi_env env, napi_callback_info info) {
	size_t argc = 1;
	napi_value args[1];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto num = sapp::SNodeUtil::toNum(env, args[0]);
	return sapp::SNodeUtil::jsnum(env, num*2);
}
napi_value NodeAppTest::TestStr(napi_env env, napi_callback_info info) {
	size_t argc = 2;
	napi_value args[2];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto s1 = sapp::SNodeUtil::toStr(env, args[0]), s2 = sapp::SNodeUtil::toStr(env, args[1]);
	return sapp::SNodeUtil::jsstr(env, s1+s2);
}
napi_value NodeAppTest::TestArrayI(napi_env env, napi_callback_info info) {
	size_t argc = 1;
	napi_value args[1];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto a = sapp::SNodeUtil::toArray(env, args[0]);
	return sapp::SNodeUtil::jsstr(env, slib::toString(a));
}
napi_value NodeAppTest::TestArrayO(napi_env env, napi_callback_info info) {
	size_t argc = 3;
	napi_value args[3];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto o1 = sapp::SNodeUtil::toObj(env, args[0]), 
	o2 = sapp::SNodeUtil::toObj(env, args[1]),
	o3 = sapp::SNodeUtil::toObj(env, args[2]);
	return sapp::SNodeUtil::jsarray(env, {o1, o2, o3});
}
napi_value NodeAppTest::TestObjI(napi_env env, napi_callback_info info) {
	size_t argc = 1;
	napi_value args[1];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto d = sapp::SNodeUtil::toDict(env, args[0]);
	return sapp::SNodeUtil::jsstr(env, slib::toString(d));
}
napi_value NodeAppTest::TestObjO(napi_env env, napi_callback_info info) {
	size_t argc = 6;
	napi_value args[6];
	NodeAppTest* instance = sapp::SNodeUtil::toClass<NodeAppTest>(env, info, &argc, args);
	auto k1 = sapp::SNodeUtil::toStr(env, args[0]), 
	k2 = sapp::SNodeUtil::toStr(env, args[2]),
	k3 = sapp::SNodeUtil::toStr(env, args[4]);
	auto v1 = sapp::SNodeUtil::toObj(env, args[1]), 
	v2 = sapp::SNodeUtil::toObj(env, args[3]), 
	v3 = sapp::SNodeUtil::toObj(env, args[5]);
	return sapp::SNodeUtil::jsdict(env, {kv(k1,v1), kv(k2,v2), kv(k3,v3)});
}
void NodeAppTest::init() {}