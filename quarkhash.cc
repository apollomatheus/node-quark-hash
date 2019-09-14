#include <node.h>
#include <node_buffer.h>
#include <v8.h>

#include <memory>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
    #include "quark.h"
}

using namespace node;
using namespace v8;


void except(const char* msg) {
    printf("Exception: %s\n",msg);
}

void Digest(const FunctionCallbackInfo<v8::Value>& args) {
    Isolate* isolate = v8::Isolate::GetCurrent();
    EscapableHandleScope scope(isolate);
    if (args.Length() < 1) {
        except("You must provide one argument.");
        return;
     }
    Local<Object> target = args[0]->ToObject();

    if(!Buffer::HasInstance(target)) {
        except("Argument should be a buffer object.");
        return;
    }
    char * input = Buffer::Data(target);
    char * output = new char[32];

    quark_hash(input, output);
//    scope.Escape(Buffer::Data(output);
      args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), output,
                              v8::NewStringType::kNormal).ToLocalChecked());
}

Local<Context> CreateShellContext(const FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  Local<ObjectTemplate> global = ObjectTemplate::New(isolate);
  global->Set(
      String::NewFromUtf8(isolate, "digest", NewStringType::kNormal)
          .ToLocalChecked(),
      FunctionTemplate::New(isolate, Digest));
}

NODE_MODULE(quarkhash,CreateShellContext)
