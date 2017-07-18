//
//  pdf_utils.hpp
//  binding
//
//  Created by Pedro Lucas on 18/07/17.
//
//

#ifndef pdf_utils_hpp
#define pdf_utils_hpp

#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>

int PDFPageGetRotation(CGPDFPageRef page);
CGAffineTransform PDFPageGetDrawingTransform(CGPDFPageRef page, CGPDFBox box, float scale);
CGSize PDFPageGetSize(CGPDFPageRef page, CGPDFBox box);
CGImageRef CreatePDFPageImage(CGPDFPageRef page, CGSize scaledSize, bool transparentBackground);

#endif /* pdf_utils_hpp */
