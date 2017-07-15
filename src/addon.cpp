//
//  addon.cpp
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#include <nan.h>
#include "node_pdf.h"

using namespace v8;

//
//std::map<std::string, CGPDFDocumentRef> documents;
//
//void Open(const FunctionCallbackInfo<Value>& args) {
//    
//    Isolate* isolate = args.GetIsolate();
//    
//    if (args.Length() < 1) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument")));
//        return;
//    }
//    
//    const char *c_arg = *v8::String::Utf8Value(args[0]->ToString());
//    const std::string key = std::string(c_arg);
//    
//    if (documents.find(key) != documents.end()) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The document is opened")));
//        return;
//    }
//    
//    CFStringRef filePath = CFStringCreateWithCString(NULL, c_arg, kCFStringEncodingUTF8);
//    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, filePath, kCFURLPOSIXPathStyle, true);
//    
//    if(url == NULL) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid path")));
//        return;
//    }
//    
//    CGPDFDocumentRef pdf = CGPDFDocumentCreateWithURL(url);
//    
//    if(pdf == NULL) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid PDF file")));
//        return;
//    }
//    
//    documents[key] = pdf;
//    
//    args.GetReturnValue().Set(args[0]->ToString());
//    
//}
//
//void Close(const Nan::FunctionCallbackInfo<v8::Value>& info) {
//    //CGPDFDocumentRelease(pdf);
//}
//
//void PagesCount(const FunctionCallbackInfo<Value>& args) {
//    
//    Isolate* isolate = args.GetIsolate();
//    
//    if (args.Length() < 1) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument")));
//        return;
//    }
//    
//    const char *c_arg = *v8::String::Utf8Value(args[0]->ToString());
//    const std::string key = std::string(c_arg);
//    std::map<std::string, CGPDFDocumentRef>::iterator it = documents.find(key);
//    
//    if (it == documents.end()) {
//        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid pdf reference")));
//        return;
//    }
//    
//    CGPDFDocumentRef pdf = it->second;
//    
//    size_t pagesSizeT = CGPDFDocumentGetNumberOfPages(pdf);
//    
//    Local<Integer> pages = Integer::New(isolate, pagesSizeT);
//    
//    args.GetReturnValue().Set(pages->ToString());
//    
//}

void Check(const Nan::FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(String::NewFromUtf8(isolate, "Everything working right!"));
}

void Init(Local<Object> exports) {
    PDFWrapper::Init(exports);
    exports->Set(Nan::New("check").ToLocalChecked(), Nan::New<FunctionTemplate>(Check)->GetFunction());
    exports->Set(Nan::New("create").ToLocalChecked(), Nan::New<FunctionTemplate>(PDFWrapper::NewInstance)->GetFunction());
}

NODE_MODULE(node_pdf, Init)
