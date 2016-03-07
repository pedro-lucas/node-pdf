#include <node.h>

namespace node_pdf {

  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void Check(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Everything working right!"));
  }

  void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "check", Check);
  }

  NODE_MODULE(node_pdf, Init)

}
