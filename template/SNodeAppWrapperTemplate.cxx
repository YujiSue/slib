#include "$CLASS_NAME.h"

$CLASS_NAME::$CLASS_NAME() : sapp::SNodeApp<$CLASS_NAME>() {}
$CLASS_NAME::~$CLASS_NAME() {}
String $CLASS_NAME::className() { return "$CLASS_NAME"; }
Array<napi_property_descriptor> $CLASS_NAME::descriptor() {
	return Array<napi_property_descriptor>({
		DECLARE_NAPI_METHOD("$JS_METHOD", $JS_METHOD),
		$APP_METHODS...
	});
}

napi_value $CLASS_NAME::$JS_METHOD(napi_env env, napi_callback_info info) {
	size_t argc = $JS_ARG_COUNT;
	napi_value args[$JS_ARG_COUNT];
	$CLASS_NAME* instance = sapp::SNodeUtil::toClass<$CLASS_NAME>(env, info, &argc, args);
	/* function codes */
	return $JS_RETURN;
}
void $CLASS_NAME::init() {
	$CPP_INITIALIZER	
}
$CPP_RETURN $CLASS_NAME::$CPP_METHOD($CPP_ARGS) {
	$CPP_CODE
}