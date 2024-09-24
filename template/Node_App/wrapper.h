#include "sapp/snodeapp.h"
using namespace slib;
using namespace slib::sapp;
class Wrapper : public SNodeApp<Wrapper> {
public:
	Wrapper();
	~Wrapper();
public:
	static String className();
	static Array<napi_property_descriptor> descriptor();
	static napi_value get(napi_env env, napi_callback_info info);
	static napi_value post(napi_env env, napi_callback_info info);
	/*
	* Add functions if required.
	* static napi_value $FUNC(napi_env env, napi_callback_info info);
	* ...
	*/
}
