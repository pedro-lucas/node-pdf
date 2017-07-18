const matchers = require('./matchers');
const path = require('path');
const pdf = require('../index.js');
const fs = require('fs');

describe("Native async operations", () => {

  const sample1 = path.join(__dirname, 'files', 'sample1.pdf');
  const sample2 = path.join(__dirname, 'files', 'sample2.pdf');
  const samplePNG = path.join(__dirname, 'tmp', 'sample1.png');
  const sampleJPG = path.join(__dirname, 'tmp', 'sample2.jpg');

  beforeAll(() => {
    jasmine.addMatchers(matchers);
  });

  it('Check addon', () => {
    expect(pdf.check()).toEqual("Everything working right!");
  });

  it('Create document', () => {
    const doc = pdf.create(sample1);
    expect(doc instanceof pdf.PDFDocumentWrapper).toBeTruthy();
  });

  it('PDF pages count', () => {
    const doc = pdf.create(sample1);
    expect(doc.count() > 0).toBeTruthy();
  });

  it('PDF dispatch error', () => {
    try {
      pdf.create("invalid-path");
    }catch(e) {
      expect(e.message).toEqual("Invalid PDF");
    }
  });

  it('PDF get page', () => {
    const doc = pdf.create(sample2);
    const page = doc.getPage(1);
    expect(page instanceof pdf.PDFPageWrapper).toBeTruthy();
  });

  it('PDF get cropbox', () => {
    const doc = pdf.create(sample2);
    const page = doc.getPage(1);
    const size = page.getSize();
    expect(size.width).toEqual(595.27);
    expect(size.height).toEqual(841.89);
  });

  it('PDF get image buffer png', () => {
    const doc = pdf.create(sample2);
    const page = doc.getPage(1);
    try {
      const img = page.getImageBuffer({
        width: 768,
        height: 1024,
        format: pdf.PNG
      });
      fs.writeFileSync(samplePNG, img);
      expect(true).toBeTruthy();
    }catch(e) {
      expect(false).toBeTruthy();
    }
  });

  it('PDF get image buffer jpg', () => {
    const doc = pdf.create(sample2);
    const page = doc.getPage(1);
    try {
      const img = page.getImageBuffer({
        scale: 2,
        format: pdf.JPG
      });
      fs.writeFileSync(sampleJPG, img);
      expect(true).toBeTruthy();
    }catch(e) {
      expect(false).toBeTruthy();
    }
  });

});
