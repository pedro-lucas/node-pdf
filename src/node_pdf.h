//
//  node_pdf.h
//  node-pdf
//
//  Created by Pedro Lucas on 3/5/16.
//  Copyright © 2016 Appmania. All rights reserved.
//

#ifndef NODE_PDF_MAIN_INCLUDED
#define NODE_PDF_MAIN_INCLUDED

#include <string>
#include <nan.h>

using std::string;

class PDFWrapper : public Nan::ObjectWrap {
    
public:
    static NAN_MODULE_INIT(Init);
    PDFWrapper(std::string path) : _path(path) {  }
    
private:
    
    explicit PDFWrapper(double path);
    ~PDFWrapper();

    static Nan::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);

    std::string _path;
    
};

#endif // NODE_PDF_MAIN_INCLUDED
