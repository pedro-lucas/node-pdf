const matchers = require('./matchers');
const path = require('path');
const pdf = require('../index.js');
const fs = require('fs');

describe("Native async operations", () => {

  const sample = path.join(__dirname, 'files', 'sample2.pdf');
  const samplePNG = path.join(__dirname, 'tmp', 'sample3.png');

  beforeAll(() => {
    jasmine.addMatchers(matchers);
  });

  it('PDF get image buffer png', done => {
    const doc = pdf.create(sample);
    const page = doc.getPage(1);
    const img = page.getImageBuffer({
      scale: 2,
      format: pdf.PNG
    }, (err, img) => {
      if(err) {
        expect(false).toBeTruthy();
      }else{
        fs.writeFileSync(samplePNG, img);
        expect(true).toBeTruthy();
      }
      done();
    });
  });

});
