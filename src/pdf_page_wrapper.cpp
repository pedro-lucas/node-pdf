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

class CreateImageWorker : public Nan::AsyncWorker {
public:
    CreateImageWorker(Nan::Callback *callback, PDFPageWrapper *p, BoxSize s, kImageType t) : AsyncWorker(callback), page(p), size(s), type(t) {}
    ~CreateImageWorker() {}
    
    void Execute () {
        buff = page->getImageBuffer(size.width, size.height, type, &resultSize);
    }
    
    void HandleOKCallback () {
        if(callback != NULL) {
            Local<Value> argv[2];
            if(buff == NULL || resultSize == 0) {
                argv[0] = Nan::Error("Internal module error");
                argv[1] = Nan::Null();
            }else{
                argv[0] = Nan::Null();
                argv[1] = Nan::NewBuffer(buff, (uint32_t)resultSize).ToLocalChecked();
            }
            callback->Call(2, argv);
        }
    }
    
private:
    PDFPageWrapper *page;
    BoxSize size;
    kImageType type;
    char *buff;
    long resultSize = 0;
    
};

kImageType getImageType(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if(info.Length() > 0 && info[0]->IsObject()) {
        Local<Object> input = info[0].As<Object>();
        auto nFormat = input->Get(Nan::New("format").ToLocalChecked()); //1 - PNG, 2 - JPG
        if(nFormat->NumberValue() == kImageTypeJPG) {
            return kImageTypeJPG;
        }
    }
    return kImageTypePNG;
}

BoxSize getImageSize(const Nan::FunctionCallbackInfo<v8::Value>& info, BoxSize size) {
    if(info.Length() > 0 && info[0]->IsObject()) {
        Local<Object> input = info[0].As<Object>();
        auto nWidth = input->Get(Nan::New("width").ToLocalChecked());
        auto nHeight = input->Get(Nan::New("height").ToLocalChecked());
        auto nScale = input->Get(Nan::New("scale").ToLocalChecked());
        if(nScale->IsNumber() && nScale->NumberValue() > 0) {
            double scale = nScale->NumberValue();
            size.width *= scale;
            size.height *= scale;
        }else if(nWidth->IsNumber() && nHeight->IsNumber() && nWidth->NumberValue() > 0 && nHeight->NumberValue() > 0) {
            size.width = nWidth->NumberValue();
            size.height = nHeight->NumberValue();
        }
        
    }
    return size;
}

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

char* PDFPageWrapper::getImageBuffer(double width, double height, kImageType type, long *buffSize) {
    
#if defined(IS_MACOSX)
    CGImageRef image = CreatePDFPageImage(CGPDFDocumentGetPage(_document->_pdf, _pageIndex), CGSizeMake(width, height), true);
    
    if(image == NULL) {
        return NULL;
    }
    
    CFMutableDataRef data = CFDataCreateMutable(NULL, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, type == kImageTypePNG ? kUTTypePNG : kUTTypeJPEG, 1, NULL);
    CGImageDestinationAddImage(destination, image, nil);
    
    CGImageRelease(image);
    
    if(!CGImageDestinationFinalize(destination)) {
        return NULL;
    }
    
    CFIndex length = CFDataGetLength(data);
    UInt8 *buffer = new UInt8[length];
    
    CFDataGetBytes(data, CFRangeMake(0, length), buffer);
    
    *buffSize = length;
    
    CFRelease(data);
        
    return (char*)buffer;
#else
    *buffSize = 0;
    return NULL;
#endif
    
}

BoxRect PDFPageWrapper::getCropbox() {
#if defined(IS_MACOSX)
    CGPDFPageRef page = CGPDFDocumentGetPage(_document->_pdf, _pageIndex);
    CGRect rect = CGPDFPageGetBoxRect(page, kCGPDFCropBox);
    return {{rect.origin.x, rect.origin.y}, {rect.size.width, rect.size.height}};
#else
    return {{0,0}, {0,0}};
#endif
}


NAN_MODULE_INIT(PDFPageWrapper::Init) {
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("PDFPageWrapper").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    Nan::SetPrototypeMethod(tpl, "getSize", Size);
    Nan::SetPrototypeMethod(tpl, "getImageBufferSync", GetImageBufferSync);
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
    BoxSize size = getImageSize(info, obj->getCropbox().size);
    kImageType type = getImageType(info);
    Nan::Callback *callback = NULL;
    
    if(info.Length() > 1 && info[1]->IsFunction()) {
        callback = new Nan::Callback(info[1].As<Function>());
    }
    
    Nan::AsyncQueueWorker(new CreateImageWorker(callback, obj, size, type));
}

NAN_METHOD(PDFPageWrapper::GetImageBufferSync) {
    PDFPageWrapper *obj = ObjectWrap::Unwrap<PDFPageWrapper>(info.Holder());
    BoxSize size = getImageSize(info, obj->getCropbox().size);
    kImageType type = getImageType(info);
    
    long length = 0;
    char *buff = obj->getImageBuffer(size.width, size.height, type, &length);
    
    if(buff == NULL || length == 0) {
        Nan::ThrowError("Internal module error");
        return;
    }
    
    info.GetReturnValue().Set(Nan::NewBuffer(buff, (uint32_t)length).ToLocalChecked());
    
}
