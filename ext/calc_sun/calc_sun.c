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
# define RND12 1000000000000.0

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
 * convert input string to DateTime object.
 *
 */
static VALUE func_set_datetime(VALUE self, VALUE vdatetime){
  VALUE cDateTime = rb_const_get(rb_cObject, rb_intern("DateTime"));
  VALUE vdate = rb_funcall(cDateTime, rb_intern("parse"), 1, vdatetime);
  return vdate;
}
/*
 * call-seq:
 *  ajd2dt(ajd)
 *
 * convert input float to DateTime object.
 *
 */
static VALUE func_ajd_2_datetime(VALUE self, VALUE vajd){
  VALUE cDateTime = rb_const_get(rb_cObject, rb_intern("DateTime"));
  double ajd = NUM2DBL(vajd) + 0.5;
  VALUE vfajd = DBL2NUM(ajd);
  VALUE vdatetime = rb_funcall(cDateTime, rb_intern("jd"), 1, vfajd);
  return vdatetime;
}
/*
 * call-seq:
 *  ajd(date)
 *
 * convert Date or DateTime object to AJD number.
 *
 */
static VALUE func_get_ajd(VALUE self, VALUE vdatetime){
  double ajd = NUM2DBL(rb_funcall(vdatetime, rb_intern("ajd"), 0));
  return DBL2NUM(ajd);
}
/*
 * call-seq:
 *  jd(date)
 *
 * convert Date or DateTime object to JD number.
 *
 */
static VALUE func_get_jd(VALUE self, VALUE vdatetime){
  double jd = NUM2DBL(rb_funcall(vdatetime, rb_intern("jd"), 0));
  return DBL2NUM(jd);
}

/*
 * call-seq:
 *  mean_anomaly(date)
 *
 * convert Date or DateTime object to float
 * representing Solar Mean Anomaly in radians.
 *
 */
static VALUE func_mean_anomaly(VALUE self, VALUE vajd){
  double ajd = NUM2DBL(vajd);
  double t = (ajd - DJ00) / 36525;
  double vma =
  fmod((              357.52910918     +
    t * (           35999.05029113889  +
    t * (     1.0 / -6507.592190889371 +
    t * (  1.0 / 26470588.235294115    +
    t * (1.0 / -313315926.8929504))))) * D2R, M2PI);
  return DBL2NUM(roundf(vma * RND12) / RND12);
}

static VALUE func_eccentricity(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double ve =
  0.016709 -
  1.151e-9 * d;
  return DBL2NUM(roundf(ve * RND12) / RND12);
}

static VALUE func_equation_of_center(VALUE self, VALUE vajd){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double ve2 = ve * 2.0;
  double vesqr = ve * ve;
  double vesqr54 = 5.0 / 4.0 * vesqr;
  double vecube12 = (vesqr * ve) / 12.0;
  double veoc =
  ve2 * sin(vma) +
  vesqr54 * sin(2 * vma) +
  vecube12 * (13.0 * sin(3 * vma) - 3.0 * sin(vma));
  return DBL2NUM(roundf(veoc * RND12) / RND12);
}

static VALUE func_mean_longitude(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double vml =
  fmod(
    (280.4664567 +
     0.9856473601037645 * d
    ) * D2R, M2PI);
  return DBL2NUM(roundf(vml * RND12) / RND12);
}

static VALUE func_eccentric_anomaly(VALUE self, VALUE vajd){
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vml =
  NUM2DBL(func_mean_longitude(self, vajd));
  double vea =
  vml + ve * sin(vml) * (1.0 + ve * cos(vml));
  return DBL2NUM(roundf(vea * RND12) / RND12);
}

static VALUE func_obliquity_of_ecliptic(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double vooe =
  (23.439291 - 3.563E-7 * d) * D2R;
  return DBL2NUM(roundf(vooe * RND12) / RND12);
}

static VALUE func_true_anomaly(VALUE self, VALUE vajd){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double veoc =
  NUM2DBL(func_equation_of_center(self, vajd));
  double vta = vma + veoc;
  return DBL2NUM(roundf(vta * RND12) / RND12);
}

static VALUE func_longitude_of_perihelion(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double vlop =
  fmod(
    (282.9404 +
     4.70935e-05 * d
    ) * D2R, M2PI);
  return DBL2NUM(roundf(vlop * RND12) / RND12);
}

static VALUE func_xv(VALUE self, VALUE vajd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vajd));
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vxv = cos(vea) - ve;
  return DBL2NUM(roundf(vxv * RND12) / RND12);
}

static VALUE func_yv(VALUE self, VALUE vajd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vajd));
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vyv =
  sqrt(1.0 - ve * ve) * sin(vea);
  return DBL2NUM(roundf(vyv * RND12) / RND12);
}

static VALUE func_true_anomaly1(VALUE self, VALUE vajd){
  double xv = NUM2DBL(func_xv(self, vajd));
  double yv = NUM2DBL(func_yv(self, vajd));
  double vta = atan2(yv, xv);
  return DBL2NUM(roundf(vta * RND12) / RND12);
}

static VALUE func_true_longitude(VALUE self, VALUE vajd){
  double vta =
  NUM2DBL(func_true_anomaly(self, vajd));
  double vlop =
  NUM2DBL(func_longitude_of_perihelion(self, vajd));
  double vtl =
  fmod(vta + vlop, M2PI);
  return DBL2NUM(roundf(vtl * RND12) / RND12);
}

static VALUE func_gmst0(VALUE self, VALUE vajd){
  double tl =
  NUM2DBL(func_true_longitude(self, vajd));
  double st =
  fmod(PI + tl, M2PI) * R2D;
  return DBL2NUM(roundf((st / 15.0) * RND12) / RND12);
}

static VALUE func_gmst(VALUE self, VALUE vajd){
  double ajd = NUM2DBL(vajd);
  double vt = fmod(ajd, 1.0) * 24.0;
  double tl =
  NUM2DBL(func_true_longitude(self, vajd));
  double st =
  fmod((PI + tl) * R2D, 360.0) / 15.0;
  double mst = fmod((st + vt), 24.0);
  // roundf(fmod(st + vt, 24) * RND12) / RND12
  return DBL2NUM(roundf(mst * RND12) / RND12);
}

static VALUE func_mean_sidetime(VALUE self, VALUE vajd){
  double ajd = NUM2DBL(vajd);
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
  return DBL2NUM(fmod(roundf((sidereal / 15.0) * RND12) / RND12, 24.0));
}

static VALUE func_rv(VALUE self, VALUE vajd){
  double vxv =
  NUM2DBL(func_xv(self, vajd));
  double vyv =
  NUM2DBL(func_yv(self, vajd));
  double vrv =
  sqrt(vxv * vxv + vyv * vyv);
  return DBL2NUM(roundf(vrv * RND12) / RND12);
}

static VALUE func_ecliptic_x(VALUE self, VALUE vajd){
  double vrv =
  NUM2DBL(func_rv(self, vajd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vajd));
  double vex = vrv * cos(vtl);
  return DBL2NUM(roundf(vex * RND12) / RND12);
}

static VALUE func_ecliptic_y(VALUE self, VALUE vajd){
  double vrv =
  NUM2DBL(func_rv(self, vajd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vajd));
  double vey = vrv * sin(vtl);
  return DBL2NUM(roundf(vey * RND12) / RND12);
}

static VALUE func_right_ascension(VALUE self, VALUE vajd){
  double vey =
  NUM2DBL(func_ecliptic_y(self, vajd));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vajd));
  double vex =
  NUM2DBL(func_ecliptic_x(self, vajd));
  double vra =
  fmod(atan2(vey * cos(vooe), vex) + M2PI, M2PI);
  return DBL2NUM(fmod(roundf((vra * R2D / 15.0) * RND12) / RND12, 24.0));
}

static VALUE func_declination(VALUE self, VALUE vajd){
  double vex =
  NUM2DBL(func_ecliptic_x(self, vajd));
  double vey =
  NUM2DBL(func_ecliptic_y(self, vajd));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vajd));
  double ver = sqrt(vex * vex + vey * vey);
  double vz = vey * sin(vooe);
  double vdec = atan2(vz, ver);
  return DBL2NUM(roundf((vdec * R2D) * RND12) / RND12);
}

static VALUE func_local_sidetime(VALUE self, VALUE vajd, VALUE vlon){
  double vst = NUM2DBL(func_mean_sidetime(self, vajd));
  double vlst = vst + NUM2DBL(vlon) / 15.0 ;
  return DBL2NUM(fmod(roundf(vlst * RND12) / RND12, 24.0));
}

static VALUE func_dlt(VALUE self, VALUE vajd, VALUE vlat){
  double jd = floor(NUM2DBL(vajd));
  double vsin_alt = sin(-0.8333 * D2R);
  double vlat_r = NUM2DBL(vlat) * D2R;
  double vcos_lat = cos(vlat_r);
  double vsin_lat = sin(vlat_r);
  VALUE vjd = DBL2NUM(jd);
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vjd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vjd));
  double vsin_dec = sin(vooe) * sin(vtl);
  double vcos_dec =
  sqrt( 1.0 - vsin_dec * vsin_dec );
  double vdl =
  acos(
    (vsin_alt - vsin_dec * vsin_lat) /
    (vcos_dec * vcos_lat));
  double vdla = vdl * R2D;
  double vdlt = vdla / 15.0 * 2.0;
  return DBL2NUM(roundf(vdlt * RND12) / RND12);
}

static VALUE func_diurnal_arc(VALUE self, VALUE vajd, VALUE vlat){
  double jd = floor(NUM2DBL(vajd));
  double dlt = NUM2DBL(func_dlt(self, DBL2NUM(jd), vlat));
  double da = dlt / 2.0;
  return DBL2NUM(roundf(da * RND12) / RND12);
}

static VALUE func_t_south(VALUE self, VALUE vajd, VALUE vlon){
  double jd = floor(NUM2DBL(vajd));
  double lst = NUM2DBL(func_local_sidetime(self, DBL2NUM(jd), vlon));
  double ra = NUM2DBL(func_right_ascension(self, DBL2NUM(jd)));
  double vx = lst - ra;
  double vt = vx - 24.0 * floor(vx * INV24 + 0.5);
  //printf("%f", jd);
  return DBL2NUM(fmod(roundf((12.0 - vt) * RND12) / RND12, 24.0));
  //return DBL2NUM(fmod(-vx, 24.0));
  //return DBL2NUM(lst - ra);
}

static VALUE func_altitude(VALUE self, VALUE vajd, VALUE vlon){
  return Qnil;
}


static VALUE func_t_rise(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vajd, vlat));
  return DBL2NUM(fmod(roundf((ts - da) * RND12) / RND12, 24.0));
}

static VALUE func_t_mid_day(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  return DBL2NUM(fmod(roundf(ts * RND12) / RND12, 24.0));
}

static VALUE func_t_set(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vajd, vlat));
  return DBL2NUM(roundf(fmod((ts + da), 24.0) * RND12) / RND12);
}

static VALUE func_rise(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double rt = NUM2DBL(func_t_rise(self, vajd, vlat, vlon));
  double ajd = NUM2DBL(vajd);
  double rtajd = floor(ajd) - 0.5 + rt / 24.0;
  VALUE vrt = DBL2NUM(rtajd);
  //VALUE date = rb_sprintf("Sun rises: %2.0f:%02.0f UTC\n",
  //floor(rt), floor(fmod(rt, 1) * 60.0));
  return func_ajd_2_datetime(self, vrt);
}

static VALUE func_noon(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double nt = NUM2DBL(func_t_south(self, vajd, vlon));
  double ajd = NUM2DBL(vajd);
  double ntajd = floor(ajd) - 0.5 + nt / 24.0;
  VALUE vnt = DBL2NUM(ntajd);
  //VALUE date = rb_sprintf("Sun noon: %2.0f:%02.0f UTC\n",
  //floor(nt), floor(fmod(nt, 1.0) * 60.0));
  return func_ajd_2_datetime(self, vnt);
}

static VALUE func_set(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  VALUE vst;
  double stajd;
  double st = NUM2DBL(func_t_set(self, vajd, vlat, vlon));
  double nt = NUM2DBL(func_t_mid_day(self, vajd, vlat, vlon));
  double ajd = NUM2DBL(vajd);
  if (st < nt){
    st += 24.0;
  }
  stajd = floor(ajd) - 0.5 + st / 24.0;
  vst = DBL2NUM(stajd);
  //VALUE date = rb_sprintf("Sun set: %2.0f:%02.0f UTC\n",
  //floor(st), floor(fmod(st, 1.0) * 60.0));
  return func_ajd_2_datetime(self, vst);
}

/* Get the days to J2000 h is UT in decimal hours only works between 1901 to 2099 */
static inline double
GetDaysTillJ2000(int year, int month, int day, double hours){
     double days_till_J2000;
     days_till_J2000 = 367 * year - 7 * (
       year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730531.5 + hours / 24;
     return(days_till_J2000);
}

static VALUE func_jd_from_2000(VALUE self, VALUE vajd){
  int year = NUM2INT(rb_funcall(vajd, rb_intern("year"), 0));
  int month = NUM2INT(rb_funcall(vajd, rb_intern("month"), 0));
  int day = NUM2INT(rb_funcall(vajd, rb_intern("day"), 0));
  double days = GetDaysTillJ2000(year, month, day, 12);
  return DBL2NUM(roundf(days * RND12) / RND12);
}

static VALUE func_days_from_2000(VALUE self, VALUE vajd, VALUE vlon){
  double jd = NUM2DBL(vajd);
  double lon = NUM2DBL(vlon);
  double days = jd - DJ00 - lon / 360;
  return DBL2NUM(roundf(days * RND12) / RND12);
}

static VALUE func_eot(VALUE self, VALUE vajd){
  double ma =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double ta =
  NUM2DBL(func_true_anomaly(self, vajd));
  double tl =
  NUM2DBL(func_true_longitude(self, vajd));
  double ra = 15.0 * D2R *
  NUM2DBL(func_right_ascension(self, vajd));
  return DBL2NUM(roundf(fmod((ma - ta + tl - ra), M2PI) * R2D * RND12) / RND12 );
}

static VALUE func_eot_min(VALUE self, VALUE vajd){
  double eot = NUM2DBL(func_eot(self, vajd));
  return DBL2NUM(roundf((eot / 15 * 60) * RND12) / RND12);
}

static VALUE func_min_to_s(VALUE self, VALUE vmin){
  return Qnil;
}

static VALUE func_rev12(VALUE self, VALUE vx){
  double x = NUM2DBL(vx);
  return DBL2NUM(x - 24.0 * floor(x * INV24 + 0.5));
}

void Init_calc_sun(void){
  VALUE cCalcSun = rb_define_class("CalcSun", rb_cObject);
  rb_require("date");
  // rb_define_const(cCalcSun, "DJ00", DBL2NUM(DJ00));
  //VALUE cDate = rb_const_get(rb_cObject, rb_intern("Date"));
  //VALUE rb_date = rb_funcall(cDate, rb_intern("new"), 0);
  //rb_iv_set(self, "@date", rb_date);
  rb_define_method(cCalcSun, "initialize", t_init, 0);
  rb_define_attr(cCalcSun, "date", 1, 1);
  rb_define_method(cCalcSun, "ajd", func_get_ajd, 1);
  rb_define_method(cCalcSun, "ajd2dt", func_ajd_2_datetime, 1);
  rb_define_method(cCalcSun, "altitude", func_altitude, 2);
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
  rb_define_method(cCalcSun, "gmst0", func_gmst0, 1);
  rb_define_method(cCalcSun, "gmst", func_gmst, 1);
  rb_define_method(cCalcSun, "jd", func_get_jd, 1);
  rb_define_method(cCalcSun, "jd2000_dif", func_jd_from_2000, 1);
  rb_define_method(cCalcSun, "jd2000_dif_lon", func_days_from_2000, 2);
  rb_define_method(cCalcSun, "local_sidereal_time", func_local_sidetime, 2);
  rb_define_method(cCalcSun, "longitude_of_perihelion", func_longitude_of_perihelion, 1);
  rb_define_method(cCalcSun, "mean_anomaly", func_mean_anomaly, 1);
  rb_define_method(cCalcSun, "mean_longitude", func_mean_longitude, 1);
  rb_define_method(cCalcSun, "min_to_s", func_min_to_s, 1);
  rb_define_method(cCalcSun, "noon", func_noon, 3);
  rb_define_method(cCalcSun, "obliquity_of_ecliptic", func_obliquity_of_ecliptic, 1);
  rb_define_method(cCalcSun, "radius_vector", func_rv, 1);
  rb_define_method(cCalcSun, "reverse_12", func_rev12, 1);
  rb_define_method(cCalcSun, "right_ascension", func_right_ascension, 1);
  rb_define_method(cCalcSun, "rise", func_rise, 3);
  rb_define_method(cCalcSun, "set", func_set, 3);
  rb_define_method(cCalcSun, "set_datetime", func_set_datetime, 1);
  rb_define_method(cCalcSun, "t_mid_day", func_t_mid_day, 3);
  rb_define_method(cCalcSun, "t_rise", func_t_rise, 3);
  rb_define_method(cCalcSun, "t_set", func_t_set, 3);
  rb_define_method(cCalcSun, "t_south", func_t_south, 2);
  rb_define_method(cCalcSun, "true_anomaly", func_true_anomaly, 1);
  rb_define_method(cCalcSun, "true_anomaly1", func_true_anomaly1, 1);
  rb_define_method(cCalcSun, "true_longitude", func_true_longitude, 1);
  rb_define_method(cCalcSun, "xv", func_xv, 1);
  rb_define_method(cCalcSun, "yv", func_yv, 1);
}