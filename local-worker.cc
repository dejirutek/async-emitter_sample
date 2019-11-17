#include "async-emitter.h"

#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>

AsyncEmitter::LocalWorker *AsyncEmitter::LocalWorker::worker = nullptr;

Napi::ObjectReference AsyncEmitter::LocalWorker::refThis;
Napi::FunctionReference AsyncEmitter::LocalWorker::refEmit;


void AsyncEmitter::LocalWorker::Execute()
{
    std::this_thread::sleep_for(std::chrono::seconds(interval));

    std::srand( time(NULL) );
    data.clear();

    for(int i = 0; i < length; i++) {
        uint8_t rdata = rand() % 0x100;
        data.push_back(rdata);
    }

}


void AsyncEmitter::LocalWorker::OnOK()
{
    Napi::Function emit = refEmit.Value();
    Napi::Object _this = refThis.Value();
    Napi::Env env = Env();

    Napi::HandleScope scope(env);

    if (data.size()) {

        emit.Call(
            _this,
            {
                Napi::String::New(env, "data"),
                Napi::Buffer<uint8_t>::Copy(env, data.data(), data.size()),
                Napi::Number::New(env, length)
            }
        );

    }

    if (--count) {
        worker->Queue();
    }

}
