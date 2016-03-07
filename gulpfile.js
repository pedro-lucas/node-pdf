(function() {
  'use strict';

  const gulp = require('gulp');
  const shell = require('shelljs');
  const gutil = require('gulp-util');
  const log = {
    success: function(args) {
      gutil.log(gutil.colors.green(args));
    },
    error: function(args) {
      gutil.log(gutil.colors.red(args));
    },
    info: function(args) {
      gutil.log(gutil.colors.blue(args));
    }
  };

  gulp.task('default', function(done) {

    shell.exec('node-gyp build --debug', {silent: true}, function(code, stdout, stderr) {
      if(code != 0) {
        log.error("ERROR:", stderr);
      }else{

        log.info("addon compiled... wait for execution...");

        const addon = require('./build/Debug/node-pdf');
        log.success(addon.check());

      }

    });

  });

})();
