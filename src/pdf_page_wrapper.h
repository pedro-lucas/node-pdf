//
//  node_pdf_page.hpp
//  binding
//
//  Created by Pedro Lucas on 15/07/17.
//
//

#ifndef node_pdf_page_hpp
#define node_pdf_page_hpp

#include <nan.h>
#include <ApplicationServices/ApplicationServices.h>

using namespace v8;
using namespace std;

typedef enum {
    kImageTypePNG = 1,
    kImageTypeJPG = 2
} kImageType;

class PDFDocumentWrapper;

class BoxSize {
    
public:
    double width = 0;
    double height = 0;
    BoxSize(double w, double h) : width(w), height(h) {}
    
};

class BoxPoint {
    
public:
    double x = 0;
    double y = 0;
    BoxPoint(double xx, double yy) : x(xx), y(yy) {}
    
};

class BoxRect {
    
public:
    BoxPoint point;
    BoxSize size;
    BoxRect(BoxPoint p, BoxSize s) : point(p), size(s) {}
    
};

class PDFPageWrapper : public Nan::ObjectWrap {
    
public:
    static NAN_MODULE_INIT(Init);
    static MaybeLocal<Object> NewInstance(Local<Value> arg1, Local<Value> arg2);
    
    BoxRect getCropbox();
    MaybeLocal<Object> getImageBuffer(double width, double height, kImageType type);
    
    PDFDocumentWrapper *_document = NULL;
    unsigned int _pageIndex = 0;
    
private:
    
    explicit PDFPageWrapper(PDFDocumentWrapper *document, unsigned int pageIndex);
    ~PDFPageWrapper();
    
    static Nan::Persistent<Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(Size);
    static NAN_METHOD(GetImageBuffer);
    
};

#endif /* node_pdf_page_hpp */
