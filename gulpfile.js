const gulp = require('gulp');
const jasmine = require('gulp-jasmine');
const clean = require('gulp-clean');
const filter = require('gulp-filter');
const sequence = require('run-sequence');
const shell = require('gulp-shell')

gulp.task('clear-test', () => {
  return gulp.src('spec/tmp/*.*', {
      read: false
  })
  .pipe(filter(['*', '!spec/tmp/.gitignore']))
  .pipe(clean());
});

gulp.task('compile', shell.task(['node-gyp rebuild']));

gulp.task('test', ['clear-test'], () => {
  return gulp.src('spec/*-test.js')
  .pipe(jasmine({
    verbose: true
  }));
});

gulp.task('default', done => {
	sequence('compile', 'test', done);
});
