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

});

// log.info("pages pdf 1:", addon.pagesCount(pdf1));
// log.info("pages pdf 2:", addon.pagesCount(pdf2));
