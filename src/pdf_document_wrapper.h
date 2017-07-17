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

class PDFDocumentWrapper : public Nan::ObjectWrap {
    
public:
    static NAN_MODULE_INIT(Init);
    static NAN_METHOD(NewInstance);
    
    std::string _path;
    
    /* Document pointer
     *
     */
    CGPDFDocumentRef _pdf = NULL;

private:
    
    explicit PDFDocumentWrapper(std::string path = "");
    ~PDFDocumentWrapper();

    static Nan::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(GetPath);
    static NAN_METHOD(Count);
    static NAN_METHOD(GetPage);

};

#endif // NODE_PDF_MAIN_INCLUDED
