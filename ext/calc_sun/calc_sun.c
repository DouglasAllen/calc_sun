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
static VALUE func_get_date(VALUE self, VALUE vdate){
  VALUE cDate = rb_const_get(rb_cObject, rb_intern("Date"));
  VALUE parsed = rb_funcall(cDate, rb_intern("parse"), 1, vdate);
  return parsed;
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
 *  mean_anomaly(date)
 *
 * convert Date or DateTime object to float
 * representing Solar Mean Anomaly in radians.
 *
 */
static VALUE func_mean_anomaly(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double vma =
  fmod(
    (357.5291  +
     0.98560028 * d
    ) * D2R, M2PI);
  return DBL2NUM(vma);
}

static VALUE func_eccentricity(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double ve =
  0.016709 -
  1.151e-9 * d;
  return DBL2NUM(ve);
}

static VALUE func_equation_of_center(VALUE self, VALUE vdate){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vdate));
  double ve =
  NUM2DBL(func_eccentricity(self, vdate));
  double ve2 = ve * 2.0;
  double vesqr = ve * ve;
  double vesqr54 = 5.0 / 4.0 * vesqr;
  double vecube12 = (vesqr * ve) / 12.0;
  double veoc =
  ve2 * sin(vma) +
  vesqr54 * sin(2 * vma) +
  vecube12 * (13.0 * sin(3 * vma) - 3.0 * sin(vma));
  return DBL2NUM(veoc);
}

static VALUE func_true_anomaly(VALUE self, VALUE vdate){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vdate));
  double veoc =
  NUM2DBL(func_equation_of_center(self, vdate));
  double vta = vma + veoc;
  return DBL2NUM(vta);
}

static VALUE func_mean_longitude(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double vml =
  fmod(
    (280.4664567 +
     0.9856473601037645 * d
    ) * D2R, M2PI);
  return DBL2NUM(vml);
}

static VALUE func_eccentric_anomaly(VALUE self, VALUE vdate){
  double ve =
  NUM2DBL(func_eccentricity(self, vdate));
  double vml =
  NUM2DBL(func_mean_longitude(self, vdate));
  double vea =
  vml + ve * sin(vml) * (1.0 + ve * cos(vml));
  return DBL2NUM(vea);
}

static VALUE func_obliquity_of_ecliptic(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double vooe =
  (23.439291 - 3.563E-7 * d) * D2R;
  return DBL2NUM(vooe);
}

static VALUE func_longitude_of_perihelion(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double vlop =
  fmod(
    (282.9404 +
     4.70935e-05 * d
    ) * D2R, M2PI);
  return DBL2NUM(vlop);
}

static VALUE func_xv(VALUE self, VALUE vdate){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vdate));
  double ve =
  NUM2DBL(func_eccentricity(self, vdate));
  double vxv = cos(vea) - ve;
  return DBL2NUM(vxv);
}

static VALUE func_yv(VALUE self, VALUE vdate){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vdate));
  double ve =
  NUM2DBL(func_eccentricity(self, vdate));
  double vyv =
  sqrt(1.0 - ve * ve) * sin(vea);
  return DBL2NUM(vyv);
}

static VALUE func_true_longitude(VALUE self, VALUE vdate){
  double vta =
  NUM2DBL(func_true_anomaly(self, vdate));
  double vlop =
  NUM2DBL(func_longitude_of_perihelion(self, vdate));
  double vtl =
  fmod(vta + vlop, M2PI);
  return DBL2NUM(vtl);
}

static VALUE func_rv(VALUE self, VALUE vdate){
  double vxv =
  NUM2DBL(func_xv(self, vdate));
  double vyv =
  NUM2DBL(func_yv(self, vdate));
  double vrv =
  sqrt(vxv * vxv + vyv * vyv);
  return DBL2NUM(vrv);
}

static VALUE func_ecliptic_x(VALUE self, VALUE vdate){
  double vrv =
  NUM2DBL(func_rv(self, vdate));
  double vtl =
  NUM2DBL(func_true_longitude(self, vdate));
  double vex = vrv * cos(vtl);
  return DBL2NUM(vex);
}

static VALUE func_ecliptic_y(VALUE self, VALUE vdate){
  double vrv =
  NUM2DBL(func_rv(self, vdate));
  double vtl =
  NUM2DBL(func_true_longitude(self, vdate));
  double vey = vrv * sin(vtl);
  return DBL2NUM(vey);
}

static VALUE func_right_ascension(VALUE self, VALUE vdate){
  double vey =
  NUM2DBL(func_ecliptic_y(self, vdate));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vdate));
  double vex =
  NUM2DBL(func_ecliptic_x(self, vdate));
  double vra =
  fmod(atan2(vey * cos(vooe), vex) + M2PI, M2PI);
  return DBL2NUM(vra * R2D / 15.0);
}

static VALUE func_declination(VALUE self, VALUE vdate){
  double vex =
  NUM2DBL(func_ecliptic_x(self, vdate));
  double vey =
  NUM2DBL(func_ecliptic_y(self, vdate));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vdate));
  double ver = sqrt(vex * vex + vey * vey);
  double vz = vey * sin(vooe);
  double vdec = atan2(vz, ver);
  return DBL2NUM(vdec);
}

static VALUE func_sidetime(VALUE self, VALUE vdate){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double d = jd - DJ00;
  double vst =
  fmod(
    (180 + 357.52911 + 282.9404) +
    (0.985600281725 + 4.70935E-5) * d, 360.0);
  return DBL2NUM(vst / 15.0);
}

static VALUE func_local_sidetime(VALUE self, VALUE vdate, VALUE vlon){
  double vst = NUM2DBL(func_sidetime(self, vdate));
  double vlst = NUM2DBL(vlon) / 15.0 + 12.0 + vst;
  return DBL2NUM(fmod(vlst, 24.0));
}

static VALUE func_dlt(VALUE self, VALUE vdate, VALUE vlat){
  double vsin_alt = sin(-0.8333 * D2R);
  double vlat_r = NUM2DBL(vlat) * D2R;
  double vcos_lat = cos(vlat_r);
  double vsin_lat = sin(vlat_r);
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vdate));
  double vtl =
  NUM2DBL(func_true_longitude(self, vdate));
  double vsin_dec = sin(vooe) * sin(vtl);
  double vcos_dec =
  sqrt( 1.0 - vsin_dec * vsin_dec );
  double vdl =
  acos(
    (vsin_alt - vsin_dec * vsin_lat) /
    (vcos_dec * vcos_lat));
  double vdla = vdl * R2D;
  double vdlt = vdla / 15.0 * 2.0;
  return DBL2NUM(vdlt);
}

static VALUE func_diurnal_arc(VALUE self, VALUE vdate, VALUE vlat){
  double dlt = NUM2DBL(func_dlt(self, vdate, vlat));
  double da = dlt / 2.0;
  return DBL2NUM(da);
}

static VALUE func_t_south(VALUE self, VALUE vdate, VALUE vlon){
  double lst = NUM2DBL(func_local_sidetime(self, vdate, vlon));
  double ra = NUM2DBL(func_right_ascension(self, vdate));
  double vx = lst - ra;
  double vt = vx - 24.0 * floor(vx * INV24 + 0.5);
  return DBL2NUM(12 - vt);
}

static VALUE func_t_rise(VALUE self, VALUE vdate,  VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vdate, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vdate, vlat));
  return DBL2NUM(ts - da);
}

static VALUE func_t_mid_day(VALUE self, VALUE vdate, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vdate, vlon));
  return DBL2NUM(ts);
}

static VALUE func_t_set(VALUE self, VALUE vdate, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vdate, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vdate, vlat));
  return DBL2NUM(ts + da);
}

static VALUE func_rise(VALUE self, VALUE vdate, VALUE vlat, VALUE vlon){
  double rt = NUM2DBL(func_t_rise(self, vdate, vlat, vlon));
  printf("\tSun rises \t\t\t : %2.0f:%02.0f \n",
  floor(rt), floor(fmod(rt, 1) * 60.0));
  return Qnil;
}

static VALUE func_noon(VALUE self, VALUE vdate, VALUE vlat, VALUE vlon){
  double nt = NUM2DBL(func_t_south(self, vdate, vlon));
  printf("\tSun midday \t\t\t : %2.0f:%02.0f \n",
  floor(nt), floor(fmod(nt, 1) * 60.0));
  return Qnil;
}

static VALUE func_set(VALUE self, VALUE vdate, VALUE vlat, VALUE vlon){
  double st = NUM2DBL(func_t_set(self, vdate, vlat, vlon));
  printf("\tSun sets \t\t\t : %2.0f:%02.0f \n",
  floor(st), floor(fmod(st, 1) * 60.0));
  return Qnil;
}

/* Get the days to J2000 h is UT in decimal hours only works between 1901 to 2099 */
static inline double
GetDaysTillJ2000(int year, int month, int day, double hours){
     double days_till_J2000;
     days_till_J2000 = 367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730531.5 + hours / 24;
     return(days_till_J2000);
}

static VALUE func_jd_from_2000(VALUE self, VALUE vdate){
  int year = NUM2INT(rb_funcall(vdate, rb_intern("year"), 0));
  int month = NUM2INT(rb_funcall(vdate, rb_intern("month"), 0));
  int day = NUM2INT(rb_funcall(vdate, rb_intern("day"), 0));
  double days = GetDaysTillJ2000(year, month, day, 12);
  return DBL2NUM(days);
}

static VALUE func_days_from_2000(VALUE self, VALUE vdate, VALUE vlon){
  double jd = NUM2DBL(func_get_jd(self, vdate));
  double lon = NUM2DBL(vlon);
  double days = jd - DJ00 - lon / 360;
  return DBL2NUM(days);
}

static VALUE func_eot(VALUE self, VALUE vdate){
  double ma =
  NUM2DBL(func_mean_anomaly(self, vdate));
  double ta =
  NUM2DBL(func_true_anomaly(self, vdate));
  double tl =
  NUM2DBL(func_true_longitude(self, vdate));
  double ra = 15.0 * D2R *
  NUM2DBL(func_right_ascension(self, vdate));
  return DBL2NUM(fmod((ma - ta + tl - ra), M2PI));
}

static VALUE func_eot_min(VALUE self, VALUE vdate){
  double eot = NUM2DBL(func_eot(self, vdate));
  return DBL2NUM(eot * R2D / 15 * 60);
}

static VALUE func_rev12(VALUE self, VALUE vx){
  double x = NUM2DBL(vx);
  return DBL2NUM(x - 24.0 * floor(x * INV24 + 0.5));
}

void Init_calc_sun(void){
  VALUE cCalcSun = rb_define_class("CalcSun", rb_cObject);
  rb_define_method(cCalcSun, "initialize", t_init, 0);
  rb_define_const(cCalcSun, "DJ00", DBL2NUM(DJ00));
  rb_define_method(cCalcSun, "ajd", func_get_ajd, 1);
  rb_define_method(cCalcSun, "date", func_get_date, 1);
  rb_define_method(cCalcSun, "daylight_time", func_dlt, 2);
  rb_define_method(cCalcSun, "declination", func_declination, 1);
  rb_define_method(cCalcSun, "diurnal_arc", func_diurnal_arc, 2);
  rb_define_method(cCalcSun, "eccentricity", func_eccentricity, 1);
  rb_define_method(cCalcSun, "eccentric_anomaly", func_eccentric_anomaly, 1);
  rb_define_method(cCalcSun, "ecliptic_x", func_ecliptic_x, 1);
  rb_define_method(cCalcSun, "ecliptic_y", func_ecliptic_y, 1);
  rb_define_method(cCalcSun, "eot_min", func_eot_min, 1);
  rb_define_method(cCalcSun, "equation_of_center", func_equation_of_center, 1);
  rb_define_method(cCalcSun, "equation_of_time", func_eot, 1);
  rb_define_method(cCalcSun, "jd", func_get_jd, 1);
  rb_define_method(cCalcSun, "jd2000_dif", func_jd_from_2000, 1);
  rb_define_method(cCalcSun, "jd2000_dif_lon", func_days_from_2000, 2);
  rb_define_method(cCalcSun, "local_sidereal_time", func_local_sidetime, 2);
  rb_define_method(cCalcSun, "longitude_of_perihelion", func_longitude_of_perihelion, 1);
  rb_define_method(cCalcSun, "mean_anomaly", func_mean_anomaly, 1);
  rb_define_method(cCalcSun, "mean_longitude", func_mean_longitude, 1);
  rb_define_method(cCalcSun, "noon", func_noon, 3);
  rb_define_method(cCalcSun, "obliquity_of_ecliptic", func_obliquity_of_ecliptic, 1);
  rb_define_method(cCalcSun, "radius_vector", func_rv, 1);
  rb_define_method(cCalcSun, "reverse_12", func_rev12, 1);
  rb_define_method(cCalcSun, "right_ascension", func_right_ascension, 1);
  rb_define_method(cCalcSun, "rise", func_rise, 3);
  rb_define_method(cCalcSun, "set", func_set, 3);
  rb_define_method(cCalcSun, "sidereal_time", func_sidetime, 1);
  rb_define_method(cCalcSun, "t_mid_day", func_t_mid_day, 3);
  rb_define_method(cCalcSun, "t_rise", func_t_rise, 3);
  rb_define_method(cCalcSun, "t_set", func_t_set, 3);
  rb_define_method(cCalcSun, "t_south", func_t_south, 2);
  rb_define_method(cCalcSun, "true_anomaly", func_true_anomaly, 1);
  rb_define_method(cCalcSun, "true_longitude", func_true_longitude, 1);
  rb_define_method(cCalcSun, "xv", func_xv, 1);
  rb_define_method(cCalcSun, "yv", func_yv, 1);
}