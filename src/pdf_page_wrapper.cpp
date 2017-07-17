//
//  node_pdf_page.cpp
//  binding
//
//  Created by Pedro Lucas on 15/07/17.
//
//

#include "pdf_page_wrapper.h"
#include "pdf_document_wrapper.h"

Nan::Persistent<v8::Function> PDFPageWrapper::constructor;

PDFPageWrapper::~PDFPageWrapper() {

}

PDFPageWrapper::PDFPageWrapper(PDFDocumentWrapper *document, unsigned int pageIndex) {
    
    if(!document->_pdf) {
        return;
    }
    
    if(CGPDFDocumentGetNumberOfPages(document->_pdf) < (size_t)pageIndex) {
        return;
    }
    
    _pageIndex = pageIndex;
    _document = document;
    
}

NAN_MODULE_INIT(PDFPageWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PDFPageWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "getCropbox", Cropbox);
    
    constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("PDFPageWrapper").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(PDFPageWrapper::New) {
    if (info.IsConstructCall()) {
        if(info.Length() < 2 || !info[0]->IsObject() || !info[1]->IsUint32()) {
            Nan::ThrowError("Invalid params");
            return;
        }
        PDFDocumentWrapper *param1 = Nan::ObjectWrap::Unwrap<PDFDocumentWrapper>(info[0]->ToObject());
        int param2 = info[1]->NumberValue();
        PDFPageWrapper *obj = new PDFPageWrapper(param1, param2);
        if(!obj->_document) {
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

v8::MaybeLocal<v8::Object> PDFPageWrapper::NewInstance(v8::Local<v8::Value> arg1, v8::Local<v8::Value> arg2) {
    const int argc = 2;
    v8::Local<v8::Value> argv[argc] = {arg1, arg2};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    return Nan::NewInstance(cons, argc, argv);
}

NAN_METHOD(PDFPageWrapper::Cropbox) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    CGPDFPageRef page = CGPDFDocumentGetPage(obj->_document->_pdf, obj->_pageIndex);
    CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
    v8::Local<v8::Object> ret = Nan::New<v8::Object>();
    ret->Set(Nan::New("width").ToLocalChecked(), Nan::New(rect.size.width));
    ret->Set(Nan::New("height").ToLocalChecked(), Nan::New(rect.size.height));
    info.GetReturnValue().Set(ret);
}
