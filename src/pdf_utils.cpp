//
//  pdf_utils.cpp
//  binding
//
//  Created by Pedro Lucas on 18/07/17.
//
//

#include "pdf_utils.h"
#include "os.h"

#if defined(IS_MACOSX)

int PDFPageGetRotation(CGPDFPageRef page) {
    int rotationAngle = CGPDFPageGetRotationAngle(page);
    if (rotationAngle % 90 != 0)
        return 0;
    else
        return (4 + ((rotationAngle / 90) % 4)) % 4;
}

CGAffineTransform PDFPageGetDrawingTransform(CGPDFPageRef page, CGPDFBox box, float scale) {
    CGRect boxRect = CGPDFPageGetBoxRect(page, box);
    int rotation = PDFPageGetRotation(page);
    
    CGAffineTransform transform = CGAffineTransformMakeScale(scale, scale);
    
    transform = CGAffineTransformRotate(transform, -(rotation * M_PI_2));
    
    switch (rotation)
    {
        case 1:
            transform = CGAffineTransformTranslate(transform, -CGRectGetWidth(boxRect), 0);
            break;
        case 2:
            transform = CGAffineTransformTranslate(transform, -CGRectGetWidth(boxRect), 0);
            transform = CGAffineTransformTranslate(transform, 0, -CGRectGetHeight(boxRect));
            break;
        case 3:
            transform = CGAffineTransformTranslate(transform, 0, -CGRectGetHeight(boxRect));
            break;
        default:
            break;
    }
    
    transform = CGAffineTransformTranslate(transform, -boxRect.origin.x, -boxRect.origin.y);
    
    return transform;
}

CGSize PDFPageGetSize(CGPDFPageRef page, CGPDFBox box) {
    CGRect boxRect = CGPDFPageGetBoxRect(page, box);
    int rotation = PDFPageGetRotation(page);
    bool invertSize = (rotation % 2) == 1;
    
    CGFloat width = invertSize ? CGRectGetHeight(boxRect) : CGRectGetWidth(boxRect);
    CGFloat height = invertSize ? CGRectGetWidth(boxRect) : CGRectGetHeight(boxRect);
    
    return CGSizeMake(width, height);
}

CGImageRef CreatePDFPageImage(CGPDFPageRef page, CGSize scaledSize, bool transparentBackground) {
    CGSize pageSize = PDFPageGetSize(page, kCGPDFCropBox);
    
    CGFloat scaleWidth = scaledSize.width / pageSize.width;
    CGFloat scaleHeight = scaledSize.height / pageSize.height;
    CGFloat scale = fmax(scaleWidth, scaleHeight);
    
    size_t width = scale * pageSize.width;
    size_t height = scale * pageSize.height;
    size_t x = (width - scaledSize.width) / 2.0;
    size_t y = (height - scaledSize.height) / 2.0;
    
    size_t bytesPerLine = width * 4;
    uint64_t size = (uint64_t)height * (uint64_t)bytesPerLine;
    
    if ((size == 0) || (size > SIZE_MAX))
        return NULL;
    
    void *bitmapData = malloc(size);
    
    if (!bitmapData)
        return NULL;
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
    CGContextRef context = CGBitmapContextCreate(bitmapData, scaledSize.width, scaledSize.height, 8, bytesPerLine, colorSpace, kCGImageAlphaPremultipliedFirst);
    
    if (transparentBackground) {
        CGContextClearRect(context, CGRectMake(0, 0, scaledSize.width, scaledSize.height));
    }else{
        CGContextSetRGBFillColor(context, 1, 1, 1, 1); // white
        CGContextFillRect(context, CGRectMake(0, 0, scaledSize.width, scaledSize.height));
    }
    
    CGAffineTransform drawingTransform = PDFPageGetDrawingTransform(page, kCGPDFCropBox, scale);
    drawingTransform.tx -= x;
    drawingTransform.ty -= y;
    CGContextConcatCTM(context, drawingTransform);
    
    CGContextDrawPDFPage(context, page);
    CGContextClipToRect(context, CGRectMake(x, y, width, height));
    CGImageRef pdfImage = CGBitmapContextCreateImage(context);
    
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    free(bitmapData);
    
    return pdfImage;
}

#endif
