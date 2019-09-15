#include <node.h>
#include <node_buffer.h>


#include "libplatform/libplatform.h"
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
    printf("hello world! %d\n",args.Length());
    if (args.Length() < 1) {
        except("You must provide one argument.");
        return;
     }
    Local<Object> target = args[0]->ToObject();
     printf("a");
    if(!Buffer::HasInstance(target)) {
        except("Argument should be a buffer object.");
        return;
    }
    char * input = Buffer::Data(target);
    char * output = new char[32];
    printf("b");
    quark_hash(input, output);
//    scope.Escape(Buffer::Data(output);
      args.GetReturnValue().Set(
      v8::String::NewFromUtf8(args.GetIsolate(), output,
                              v8::NewStringType::kNormal).ToLocalChecked());
}

void Load(v8::Handle<v8::Object> exports) {
  NODE_SET_METHOD(exports, "digest", Digest);
} 

NODE_MODULE(method,Load);
