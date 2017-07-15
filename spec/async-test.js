const matchers = require('./matchers');
const path = require('path');
const pdf = require('../index.js');

describe("Native async operations", () => {

  const sample1 = path.join(__dirname, 'files', 'sample1.pdf');
  const sample2 = path.join(__dirname, 'files', 'sample2.pdf');

  beforeAll(() => {
    jasmine.addMatchers(matchers);
  });

  it('Check addon', () => {
    expect(pdf.check()).toEqual("Everything working right!");
  });

  it('Create PDFWrapper', () => {
    const obj = pdf.create(sample1);
    expect(obj.isValid()).toBeTruthy();
  });

  it('PDF pages count', () => {
    const obj = pdf.create(sample1);
    expect(obj.count() > 0).toBeTruthy();
  });

});
