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

Nan::Persistent<Function> PDFPageWrapper::constructor;

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

MaybeLocal<Object> PDFPageWrapper::getImageBuffer(double width, double height, kImageType type) {
    
    CGImageRef image = CreatePDFPageImage(CGPDFDocumentGetPage(_document->_pdf, _pageIndex), CGSizeMake(width, height), false);
    
    if(image == NULL) {
        return MaybeLocal<Object>(Local<Object>());
    }
    
    CFMutableDataRef data = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, type == kImageTypePNG ? kUTTypePNG : kUTTypeJPEG, 1, NULL);
    CGImageDestinationAddImage(destination, image, nil);
    
    CGImageRelease(image);
    
    if(!CGImageDestinationFinalize(destination)) {
        return MaybeLocal<Object>(Local<Object>());
    }
    
    CFIndex length = CFDataGetLength(data);
    UInt8 *buffer = new UInt8[length];
    
    CFDataGetBytes(data, CFRangeMake(0, length), buffer);
    
    return Nan::NewBuffer((char *)buffer, length);
    
}

BoxRect PDFPageWrapper::getCropbox() {
    CGPDFPageRef page = CGPDFDocumentGetPage(_document->_pdf, _pageIndex);
    CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
    return {{rect.origin.x, rect.origin.y}, {rect.size.width, rect.size.height}};
}


NAN_MODULE_INIT(PDFPageWrapper::Init) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PDFPageWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "getSize", Size);
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
        Local<Value> argv[argc] = {info[0]};
        Local<Function> cons = Nan::New(constructor);
        auto obj = Nan::NewInstance(cons, argc, argv);
        if(!obj.IsEmpty()) {
            info.GetReturnValue().Set(obj.ToLocalChecked());
        }
    }
}

MaybeLocal<Object> PDFPageWrapper::NewInstance(Local<Value> arg1, Local<Value> arg2) {
    const int argc = 2;
    Local<Value> argv[argc] = {arg1, arg2};
    Local<Function> cons = Nan::New(constructor);
    return Nan::NewInstance(cons, argc, argv);
}

NAN_METHOD(PDFPageWrapper::Size) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    BoxRect rect = obj->getCropbox();
    Local<Object> ret = Nan::New<Object>();
    ret->Set(Nan::New("width").ToLocalChecked(), Nan::New(rect.size.width));
    ret->Set(Nan::New("height").ToLocalChecked(), Nan::New(rect.size.height));
    info.GetReturnValue().Set(ret);
}

NAN_METHOD(PDFPageWrapper::GetImageBuffer) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    BoxRect rect = obj->getCropbox();
    BoxSize size = {rect.size.width, rect.size.height};
    kImageType type = kImageTypePNG;
    
    if(info.Length() > 0) {
        if(info[0]->IsObject()) {
            Local<Object> input = info[0].As<Object>();
            auto nWidth = input->Get(Nan::New("width").ToLocalChecked());
            auto nHeight = input->Get(Nan::New("height").ToLocalChecked());
            auto nScale = input->Get(Nan::New("scale").ToLocalChecked());
            auto nFormat = input->Get(Nan::New("format").ToLocalChecked()); //1 - PNG, 2 - JPG
            
            if(nScale->IsNumber() && nScale->NumberValue() > 0) {
                double scale = nScale->NumberValue();
                size.width *= scale;
                size.height *= scale;
            }else if(nWidth->IsNumber() && nHeight->IsNumber() && nWidth->NumberValue() > 0 && nHeight->NumberValue() > 0) {
                size.width = nWidth->NumberValue();
                size.height = nHeight->NumberValue();
            }
            
            if(nFormat->NumberValue() == kImageTypeJPG) {
                type = kImageTypeJPG;
            }
            
        }
    }
    
    MaybeLocal<Object> buff = obj->getImageBuffer(size.width, size.height, type);
    
    if(buff.IsEmpty()) {
        Nan::ThrowError("Internal module error");
        return;
    }
    
    info.GetReturnValue().Set(buff.ToLocalChecked());
    
}
