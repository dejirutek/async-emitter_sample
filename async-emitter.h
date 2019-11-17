#include <napi.h>

#include <vector>

class AsyncEmitter : public Napi::ObjectWrap<AsyncEmitter>
{
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        AsyncEmitter(const Napi::CallbackInfo& info);
        ~AsyncEmitter();

    protected:
        class LocalWorker : public Napi::AsyncWorker
        {
            public:
                static LocalWorker *worker;
                static Napi::ObjectReference refThis;
                static Napi::FunctionReference refEmit;
                LocalWorker(Napi::Env env, int interval)
                    : Napi::AsyncWorker(env), interval(interval) {}
                ~LocalWorker() {}

                void InitPerm(int length, int count) {
                    this->length = length;
                    this->count = count;
                }

            protected:
                void Execute();
                void OnOK();

            private:
                int interval;
                int length;
                int count;
                std::vector<uint8_t> data;
        };

    private:
        static Napi::FunctionReference constructor;

        Napi::Value AsyncInit(const Napi::CallbackInfo &info);
        Napi::Value AsyncQueue(const Napi::CallbackInfo &info);
};
