//
//  addon.cpp
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#include <nan.h>
#include "pdf_document_wrapper.h"
#include "pdf_page_wrapper.h"

using namespace v8;

void Check(const Nan::FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(String::NewFromUtf8(isolate, "Everything working right!"));
}

void Init(Local<Object> exports) {
    PDFDocumentWrapper::Init(exports);
    PDFPageWrapper::Init(exports);
    exports->Set(Nan::New("check").ToLocalChecked(), Nan::New<FunctionTemplate>(Check)->GetFunction());
    exports->Set(Nan::New("create").ToLocalChecked(), Nan::New<FunctionTemplate>(PDFDocumentWrapper::NewInstance)->GetFunction());
}

NODE_MODULE(node_pdf, Init)
