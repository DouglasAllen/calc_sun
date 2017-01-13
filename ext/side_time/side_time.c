#include <ruby.h>
#include <math.h>
#include <time.h>
/* if PI's not defined, define it */
#ifndef PI
#define PI 3.14159265358979323846
#endif
#ifndef DBL2NUM
# define DBL2NUM(dbl) rb_float_new(dbl)
#endif
# define R2D 57.295779513082320876798154814105
# define D2R 0.017453292519943295769236907684886
# define M2PI M_PI * 2.0
# define INV24 1.0 / 24.0
# define INV360 1.0 / 360.0
# define DJ00 2451545.0L

/*
 * call-seq:
 *  initialize()
 *
 * Create CalcSun class Ruby object.
 *
 */
static VALUE t_init(VALUE self){
  return self;
}
/*
 * call-seq:
 *  date('yyyy-mm-dd')
 *
 * convert input string to Date object.
 *
 */
static VALUE func_set_datetime(VALUE self, VALUE vdate){
  VALUE cDate = rb_const_get(rb_cObject, rb_intern("DateTime"));
  VALUE day = rb_funcall(cDate, rb_intern("parse"), 1, vdate);
  return day;
}
/*
 * call-seq:
 *  jd(date)
 *
 * convert Date or DateTime object to JD number.
 *
 */
static VALUE func_get_jd(VALUE self, VALUE vdate){
  double jd = NUM2DBL(rb_funcall(vdate, rb_intern("jd"), 0));
  return DBL2NUM(jd);
}
/*
 * call-seq:
 *  ajd(date)
 *
 * convert Date or DateTime object to AJD number.
 *
 */
static VALUE func_get_ajd(VALUE self, VALUE vdate){
  double ajd = NUM2DBL(rb_funcall(vdate, rb_intern("ajd"), 0));
  return DBL2NUM(ajd);
}
/*
 * call-seq:
 *  gmst(date)
 *
 * calculate Greenwhich Mean Sidereal Time.
 *
 */
static VALUE func_mean_sidetime(VALUE self, VALUE vdate){
  double ajd = NUM2DBL(func_get_ajd(self, vdate));
  long double sidereal;
  long double t;
  t = (ajd - 2451545.0) / 36525.0;
  /* calc mean angle */
  sidereal =
  280.46061837 +
  (360.98564736629 * (ajd - 2451545.0)) +
  (0.000387933 * t * t) -
  (t * t * t / 38710000.0);
  sidereal = fmod(sidereal, 360.0);
  /* change to hours */
  sidereal *= 24.0 / 360.0;
  return DBL2NUM(sidereal);
}
/*
 * call-seq:
 *  lmst(date)
 *
 * calculate Local Mean Sidereal Time.
 *
 */
static VALUE func_local_sidetime(VALUE self, VALUE vdate, VALUE vlon){
  double sidereal;
  double ls;
  double lon = NUM2DBL(vlon);
  sidereal = NUM2DBL(func_mean_sidetime(self, vdate));
  sidereal *= 15.0;
  ls = sidereal + lon;
  ls *= 24.0 / 360.0;
  return DBL2NUM(ls);
}


void Init_side_time(void){
  VALUE cSideTime = rb_define_class("SideTime", rb_cObject);
  rb_require("date");
  rb_define_method(cSideTime, "initialize", t_init, 0);
  rb_define_attr(cSideTime, "date", 1, 1);
  rb_define_method(cSideTime, "ajd", func_get_ajd, 1);
  rb_define_method(cSideTime, "s_datetime", func_set_datetime, 1);
  rb_define_method(cSideTime, "jd", func_get_jd, 1);
  //rb_define_method(cSideTime, "jd2000_dif", func_jd_from_2000, 1);
  //rb_define_method(cSideTime, "jd2000_dif_lon", func_days_from_2000, 2);
  rb_define_method(cSideTime, "lmst", func_local_sidetime, 2);
  rb_define_method(cSideTime, "gmst", func_mean_sidetime, 1);
}