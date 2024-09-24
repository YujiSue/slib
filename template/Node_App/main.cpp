#include "wrapper.h"
napi_value Init(napi_env env, napi_value exports) {
    return Wrapper::Init(env, exports);
}
NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)