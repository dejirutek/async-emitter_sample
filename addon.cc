#include <napi.h>
#include "async-emitter.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    AsyncEmitter::Init(env, exports);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
