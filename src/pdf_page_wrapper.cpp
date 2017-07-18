//
//  node_pdf_page.cpp
//  binding
//
//  Created by Pedro Lucas on 15/07/17.
//
//

#include "pdf_page_wrapper.h"
#include "pdf_document_wrapper.h"
#include "pdf_utils.h"

Nan::Persistent<v8::Function> PDFPageWrapper::constructor;

PDFPageWrapper::~PDFPageWrapper() {
    
}

PDFPageWrapper::PDFPageWrapper(PDFDocumentWrapper *document, unsigned int pageIndex) {
    
    if(!document->_pdf) {
        return;
    }
    
    if(document->count() < (size_t)pageIndex) {
        return;
    }
    
    _pageIndex = pageIndex;
    _document = document;
    
}

CFDataRef PDFPageWrapper::getImage(double width, double height) {

    CGPDFPageRef page = CGPDFDocumentGetPage(_document->_pdf, _pageIndex);
    CGImageRef image = CreatePDFPageImage(page, CGSizeMake(width, height), false);
    
    CFMutableDataRef newImageData = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(newImageData, kUTTypePNG, 1, NULL);
    CGImageDestinationAddImage(destination, image, nil);
    
    CGImageRelease(image);
    
    if(!CGImageDestinationFinalize(destination)) {
        return NULL;
    }
    
    return newImageData;
    
}

BoxRect PDFPageWrapper::getCropbox() {
    CGPDFPageRef page = CGPDFDocumentGetPage(_document->_pdf, _pageIndex);
    CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
    return {{rect.origin.x, rect.origin.y}, {rect.size.width, rect.size.height}};
}


NAN_MODULE_INIT(PDFPageWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PDFPageWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "getSize", Size);
    Nan::SetPrototypeMethod(tpl, "saveImage", SaveImage);
    Nan::SetPrototypeMethod(tpl, "getImageBuffer", GetImageBuffer);
    
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

NAN_METHOD(PDFPageWrapper::Size) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    BoxRect rect = obj->getCropbox();
    v8::Local<v8::Object> ret = Nan::New<v8::Object>();
    ret->Set(Nan::New("width").ToLocalChecked(), Nan::New(rect.size.width));
    ret->Set(Nan::New("height").ToLocalChecked(), Nan::New(rect.size.height));
    info.GetReturnValue().Set(ret);
}

NAN_METHOD(PDFPageWrapper::GetImageBuffer) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    BoxRect rect = obj->getCropbox();
    CFDataRef data = obj->getImage(rect.size.width, rect.size.height);
    CFIndex length = CFDataGetLength(data);
    UInt8 *buffer = new UInt8[length];
    CFDataGetBytes(data, CFRangeMake(0, length), buffer);
    char *buff = (char *)buffer;
    info.GetReturnValue().Set(Nan::NewBuffer(buff, (uint32_t)length).ToLocalChecked());
}
