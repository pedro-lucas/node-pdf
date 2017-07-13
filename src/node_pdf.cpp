//
//  node_pdf.cpp
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#include "node_pdf.h"

using v8::String;

NAN_METHOD(PDFWrapper::New) {
    if (info.IsConstructCall()) {
        double value = info[0]->IsUndefined() ? 0 : Nan::To<double>(info[0]).FromJust();
        PDFWrapper *obj = new PDFWrapper("teste");
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}
