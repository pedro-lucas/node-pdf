//
//  node_pdf.h
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#ifndef NODE_PDF_MAIN_INCLUDED
#define NODE_PDF_MAIN_INCLUDED

#include <nan.h>
#include <ApplicationServices/ApplicationServices.h>

class PDFWrapper : public Nan::ObjectWrap {
    
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(NewInstance);
    
private:
    
    explicit PDFWrapper(std::string path = "");    
    ~PDFWrapper();

    static Nan::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(GetPath);
    static NAN_METHOD(Count);

    std::string _path;
    CGPDFDocumentRef _pdf = NULL;

};

#endif // NODE_PDF_MAIN_INCLUDED
