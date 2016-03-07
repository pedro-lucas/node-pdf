(function() {
  'use strict';

  const gulp = require('gulp');
  const shell = require('shelljs');
  const gutil = require('gulp-util');
  const log = {
    success: function() {
      gutil.log(gutil.colors.green(this.format(arguments)));
    },
    error: function() {
      gutil.log(gutil.colors.red(this.format(arguments)));
    },
    info: function() {
      gutil.log(gutil.colors.blue(this.format(arguments)));
    },
    format: function(argss) {
      let args = Array.prototype.slice.call(argss);
      return args.join(' ');
    }
  };

  gulp.task('default', function(done) {

    shell.exec('node-gyp build --debug', {silent: false}, function(code, stdout, stderr) {
      if(code != 0) {
        log.error("ERROR:", stderr);
      }else{

        log.info("addon compiled... wait for execution...");

        const addon = require('./build/Debug/node-pdf');

        log.success(addon.check());

        const pdf1 = addon.open('/Users/Pedro/Desktop/br_163-Guia-em-PDF-para-Fevereiro.pdf');
        const pdf2 = addon.open('/Users/Pedro/Desktop/certidao/certidao.pdf');

        log.info("pages pdf 1:", addon.pagesCount(pdf1));
        log.info("pages pdf 2:", addon.pagesCount(pdf2));

      }

    });

  });

})();
