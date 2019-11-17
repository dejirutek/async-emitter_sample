#include "async-emitter.h"

Napi::FunctionReference AsyncEmitter::constructor;


Napi::Object AsyncEmitter::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "AsyncEmitter", {
        InstanceMethod("AsyncInit", &AsyncEmitter::AsyncInit),
        InstanceMethod("AsyncQueue", &AsyncEmitter::AsyncQueue)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("AsyncEmitter", func);
    return exports;
}


AsyncEmitter::AsyncEmitter(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<AsyncEmitter>(info)
{
    Napi::Env env = info.Env();
    Napi::Object _this = info.This().As<Napi::Object>();
    Napi::Function _emit = _this.Get("emit").As<Napi::Function>();
    int interval = info[0].As<Napi::Number>().Int32Value();
    LocalWorker *worker = new LocalWorker(env, interval);
    worker->SuppressDestruct();
    worker->refThis = Napi::Persistent(_this);
    worker->refThis.SuppressDestruct();
    worker->refEmit = Napi::Persistent(_emit);
    worker->refEmit.SuppressDestruct();
    LocalWorker::worker = worker;
}

AsyncEmitter::~AsyncEmitter() {
    LocalWorker::worker = nullptr;
}


Napi::Value AsyncEmitter::AsyncInit(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    int length = info[0].As<Napi::Number>().Int32Value();
    int count = info[1].As<Napi::Number>().Int32Value();
    LocalWorker::worker->InitPerm(length, count);

    return info.Env().Undefined();
}


Napi::Value AsyncEmitter::AsyncQueue(const Napi::CallbackInfo &info)
{
    LocalWorker::worker->Queue();
    return info.Env().Undefined();
}
