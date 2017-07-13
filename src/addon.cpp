//
//  addon.cpp
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#include <node.h>
#include <iostream>
#include <string>
#include <map>
#include <nan.h>
#include <ApplicationServices/ApplicationServices.h>
#include "node_pdf.h"

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Integer;
using v8::Value;

std::map<std::string, CGPDFDocumentRef> documents;

void Check(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Everything working right!"));
}

void Open(const FunctionCallbackInfo<Value>& args) {
    
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument")));
        return;
    }
    
    const char *c_arg = *v8::String::Utf8Value(args[0]->ToString());
    const std::string key = std::string(c_arg);
    
    if (documents.find(key) != documents.end()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "The document is opened")));
        return;
    }
    
    CFStringRef filePath = CFStringCreateWithCString(NULL, c_arg, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, filePath, kCFURLPOSIXPathStyle, true);
    
    if(url == NULL) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid path")));
        return;
    }
    
    CGPDFDocumentRef pdf = CGPDFDocumentCreateWithURL(url);
    
    if(pdf == NULL) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid PDF file")));
        return;
    }
    
    documents[key] = pdf;
    
    args.GetReturnValue().Set(args[0]->ToString());
    
}

void Close(const FunctionCallbackInfo<Value>& args) {
    //CGPDFDocumentRelease(pdf);
}

void PagesCount(const FunctionCallbackInfo<Value>& args) {
    
    Isolate* isolate = args.GetIsolate();
    
    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid argument")));
        return;
    }
    
    const char *c_arg = *v8::String::Utf8Value(args[0]->ToString());
    const std::string key = std::string(c_arg);
    std::map<std::string, CGPDFDocumentRef>::iterator it = documents.find(key);
    
    if (it == documents.end()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Invalid pdf reference")));
        return;
    }
    
    CGPDFDocumentRef pdf = it->second;
    
    size_t pagesSizeT = CGPDFDocumentGetNumberOfPages(pdf);
    
    Local<Integer> pages = Integer::New(isolate, pagesSizeT);
    
    args.GetReturnValue().Set(pages->ToString());
    
}

void Create(const FunctionCallbackInfo<Value>& args) {
    
    Isolate* isolate = args.GetIsolate();
    
    if(args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong argument")));
        return;
    }
    
//    Local<Object> obj = Object::New(isolate);
    
    Local<PDFWrapper> obj = PDFWrapper("teste").handle();
    
    args.GetReturnValue().Set(obj);
    
    
}

void Init(Local<Object> exports) {
    PDFWrapper::Init(exports);
    NODE_SET_METHOD(exports, "check", Check);
    NODE_SET_METHOD(exports, "create", Create);
}

NODE_MODULE(node_pdf, Init)
