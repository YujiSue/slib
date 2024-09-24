#include "wrapper.h"
Wrapper::Wrapper() : SNodeApp<Wrapper>() {}
Wrapper::~Wrapper() {}
String Wrapper::className() { return "NodeNativePlugin"; }
Array<napi_property_descriptor> Wrapper::descriptor() {
	return Array<napi_property_descriptor>({
		DECLARE_NAPI_METHOD("get", get),
		DECLARE_NAPI_METHOD("post", post) //...
		/*
		* Add declarations f requred.
		* DECLARE_NAPI_METHOD("METHOD", METHOD),
		* ...
		*/
	});
}
#define GET_METHOD_ARGS_COUNT 1 // <= Replace the number with the appropriate value for your server app.
napi_value Wrapper::get(napi_env env, napi_callback_info info) {
	// Interpret wrapper class object and argments from server as c++ objects
	size_t argc = GET_METHOD_ARGS_COUNT;
	napi_value args[GET_METHOD_ARGS_COUNT];
	Wrapper* instance = slb::snapi::toClass<Wrapper>(env, info, &argc, args);
	// Prepare return object
	sobj response;
	/* 
	 * Any process
	 * 
	 * e.g. 	
	 *  response = {
	 *	  D_("code", 0),
	 *    D_("state", "OK"),
	 *    D_("value", instance->some_function(args))
	 *  };
	 *  
	 */
	return slib::snapi::jsobj(env, response);
}
#define POST_METHOD_ARGS_COUNT 1 // <= Replace the number with the appropriate value for your server app.
napi_value Wrapper::post(napi_env env, napi_callback_info info) {
	// Interpret wrapper class object and argments from server as c++ objects
	size_t argc = GET_METHOD_ARGS_COUNT;
	napi_value args[GET_METHOD_ARGS_COUNT];
	Wrapper* instance = slb::snapi::toClass<Wrapper>(env, info, &argc, args);
	// Prepare return object
	sobj response;
	/*
	 * Any process
	 *
	 * e.g.
	 *  response = {
	 *	  D_("code", 0),
	 *    D_("state", "OK"),
	 *    D_("value", instance->some_function(args))
	 *  };
	 *
	 */
	return slib::snapi::jsobj(env, response);
}