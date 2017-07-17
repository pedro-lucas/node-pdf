//
//  node_pdf.cpp
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#include "pdf_document_wrapper.h"
#include "pdf_page_wrapper.h"
#include <unistd.h>
#include <sys/stat.h>
#include <string>

inline bool file_exists(const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

Nan::Persistent<v8::Function> PDFDocumentWrapper::constructor;

PDFDocumentWrapper::~PDFDocumentWrapper() {
    if(_pdf) {
        CGPDFDocumentRelease(_pdf);
    }
}

PDFDocumentWrapper::PDFDocumentWrapper(std::string path) {
    
    _path = path;
    
    if(!file_exists(_path)) return;
    
    CFStringRef filePath = CFStringCreateWithCString(NULL, _path.c_str(), kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, filePath, kCFURLPOSIXPathStyle, true);
    
    if(url == NULL) return;
    
    _pdf = CGPDFDocumentCreateWithURL(url);
    
}

NAN_MODULE_INIT(PDFDocumentWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PDFDocumentWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "getPath", GetPath);
    Nan::SetPrototypeMethod(tpl, "getPage", GetPage);
    Nan::SetPrototypeMethod(tpl, "count", Count);
    
    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("PDFDocumentWrapper").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(PDFDocumentWrapper::New) {
    if (info.IsConstructCall()) {
        if(info.Length() < 1) {
            Nan::ThrowError("Invalid params");
            return;
        }
        v8::String::Utf8Value param(info[0]->ToString());
        PDFDocumentWrapper *obj = new PDFDocumentWrapper(std::string(*param));
        if(!obj->_pdf) {
            Nan::ThrowError("Invalid PDF");
            return;
        }
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor);
        auto obj = Nan::NewInstance(cons, argc, argv);
        if(!obj.IsEmpty()) {
            info.GetReturnValue().Set(obj.ToLocalChecked());
        }
    }
}

NAN_METHOD(PDFDocumentWrapper::NewInstance) {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    auto obj = Nan::NewInstance(cons, argc, argv);
    if(!obj.IsEmpty()) {
        info.GetReturnValue().Set(obj.ToLocalChecked());
    }
}

NAN_METHOD(PDFDocumentWrapper::GetPath) {
    PDFDocumentWrapper *obj = ObjectWrap::Unwrap<PDFDocumentWrapper>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->_path).ToLocalChecked());
}

NAN_METHOD(PDFDocumentWrapper::Count) {
    PDFDocumentWrapper *obj = ObjectWrap::Unwrap<PDFDocumentWrapper>(info.Holder());
    int count = (int)CGPDFDocumentGetNumberOfPages(obj->_pdf);
    info.GetReturnValue().Set(count);
}

NAN_METHOD(PDFDocumentWrapper::GetPage) {
    if(info.Length() < 1) {
        Nan::ThrowError("Invalid params");
        return;
    }
    v8::MaybeLocal<v8::Object> page = PDFPageWrapper::NewInstance(info.Holder(), info[0]);
    if(!page.IsEmpty()) {
        info.GetReturnValue().Set(page.ToLocalChecked());
    }
}
