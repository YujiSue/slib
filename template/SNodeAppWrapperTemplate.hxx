#include "sapp/snodeapp.h"

using namespace slib;

class $CLASS_NAME : public sapp::SNodeApp<$CLASS_NAME> {
public:
	$CLASS_NAME();
	~$CLASS_NAME();

public:
	static String className();
	static Array<napi_property_descriptor> descriptor();

	static napi_value $JS_METHOD(napi_env env, napi_callback_info info);
	
	void init();
	$CPP_RETURN $CPP_METHOD($CPP_ARGS);
}
