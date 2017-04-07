#include <ruby.h>
#include <math.h>
#include <time.h>
/* if PI's not defined, define it */
#ifndef PI
#define PI 3.1415926535897932384626433832795028841971L
#endif
#ifndef DBL2NUM
# define DBL2NUM(dbl) rb_float_new(dbl)
#endif
# define PI2 PI * 2.0
# define R2D 57.295779513082320876798154814105L
# define D2R 0.017453292519943295769236907684886L
# define M2PI M_PI * 2.0
# define INV24 1.0 / 24.0
# define INV360 1.0 / 360.0
# define DJ00 2451545.0L
# define RND12 1000000000000.0L
/* macro for normalizing
* angles into 0-2pie range
*/
static inline double
anp(double angle){
  double w = fmod(angle, M2PI);
  if (w < 0) w += M2PI;
  return w;
}

/*
 * call-seq:
 *  initialize()
 *
 * Create CalcSun class instance Ruby object.
 *
 */
static VALUE t_init(VALUE self){
  return self;
}
/*
 * call-seq:
 *  date('yyyy-mm-ddT00:00:00+/-zoneoffset')
 * or
 *  date('20010203T040506+0700')
 * or
 *  date('3rd Feb 2001 04:05:06 PM')
 *
 * given a string representing date time
 * convert to DateTime object.
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
 * give Astronomical Julian Day Number
 * convert to DateTime object.
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
 * given Date or DateTime object
 * convert  to Astronomical Julian Day Number.
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
 * given Date or DateTime object
 * convert to JD number.
 *
 */
static VALUE func_get_jd(VALUE self, VALUE vdatetime){
  double jd = NUM2DBL(rb_funcall(vdatetime, rb_intern("jd"), 0));
  return DBL2NUM(jd);
}

/*
 * call-seq:
 *  mean_anomaly(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Mean Anomaly of Sun in radians
 * rounded to 12 decimal places.
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
/*
 * call-seq:
 *  eccentricity(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Eccentriciy of Earth Orbit around Sun.
 * rounded to 12 decimal places.
 *
 */
static VALUE func_eccentricity(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double ve =
  0.016709 -
  1.151e-9 * d;
  return DBL2NUM(roundf(ve * RND12) / RND12);
}

/*
 * call-seq:
 *  equation_of_center(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Equation of Center in radians.
 * rounded to 12 decimal places.
 *
*/

static VALUE func_equation_of_center(VALUE self, VALUE vajd){
  double mas =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double eoe =
  NUM2DBL(func_eccentricity(self, vajd));
  double sin1a = sin(1.0 * mas) * 1.0 / 4.0;
  double sin1b = sin(1.0 * mas) * 5.0 / 96.0;
  double sin2a = sin(2.0 * mas) * 11.0 / 24.0;
  double sin2b = sin(2.0 * mas) * 5.0 / 4.0;
  double sin3a = sin(3.0 * mas) * 13.0 / 12.0;
  double sin3b = sin(3.0 * mas) * 43.0 / 64.0;
  double sin4 = sin(4.0 * mas) * 103.0 / 96.0;
  double sin5 = sin(5.0 * mas) * 1097.0 / 960.0;
  double ad3 = sin3a - sin1a;
  double ad4 = sin4 - sin2a;
  double ad5 = sin5 + sin1b - sin3b;
  double veoc = eoe * (sin1a * 8.0 + eoe * (sin2b + eoe * (ad3 + eoe * (ad4 + eoe * ad5))));
  return DBL2NUM(roundf(veoc * RND12) / RND12);
}
/*
 * call-seq:
 *  true_anomaly(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns True Anomaly of Sun in radians.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_true_anomaly(VALUE self, VALUE vajd){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double veoc =
  NUM2DBL(func_equation_of_center(self, vajd));
  double vta = vma + veoc;
  return DBL2NUM(roundf(vta * RND12) / RND12);
}
/*
 * call-seq:
 *  mean_longitude(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Mean Longitude of Sun in radians.
 * rounded to 12 decimal places.
 *
*/
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
/*
 * call-seq:
 *  eccentric_anomaly(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Eccentric Anomaly of Sun in radians.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_eccentric_anomaly(VALUE self, VALUE vajd){
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vml =
  NUM2DBL(func_mean_longitude(self, vajd));
  double vea =
  vml + ve * sin(vml) * (1.0 + ve * cos(vml));
  return DBL2NUM(roundf(vea * RND12) / RND12);
}
/*
 * call-seq:
 *  obliquity_of_ecliptic(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Earth Tilt (Obliquity) in radians.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_obliquity_of_ecliptic(VALUE self, VALUE vajd){
  double jd = NUM2DBL(vajd);
  double d = jd - DJ00;
  double vooe =
  (23.439291 - 3.563E-7 * d) * D2R;
  return DBL2NUM(roundf(vooe * RND12) / RND12);
}
/*
 * call-seq:
 *  longitude_of_perihelion(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Longitude of Sun at Perihelion in radians.
 * rounded to 12 decimal places.
 *
 *
*/
static VALUE func_longitude_of_perihelion(VALUE self, VALUE vajd){
  double vml = NUM2DBL(func_mean_longitude(self, vajd));
  double vma = NUM2DBL(func_mean_anomaly(self, vajd));
  double vlop = anp(vml - vma);
  return DBL2NUM(roundf(vlop * RND12) / RND12);
}
/*
 * call-seq:
 *  xv(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns X component of Radius Vector in radians.
 * rounded to 12 decimal places.
 *
 *
*/
static VALUE func_xv(VALUE self, VALUE vajd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vajd));
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vxv = cos(vea) - ve;
  return DBL2NUM(roundf(vxv * RND12) / RND12);
}
/*
 * call-seq:
 *  yv(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Y component of Radius Vector in radians.
 * rounded to 12 decimal places.
 *
 *
*/
static VALUE func_yv(VALUE self, VALUE vajd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vajd));
  double ve =
  NUM2DBL(func_eccentricity(self, vajd));
  double vyv =
  sqrt(1.0 - ve * ve) * sin(vea);
  return DBL2NUM(roundf(vyv * RND12) / RND12);
}
/*
 * call-seq:
 *  true_anomaly1(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns True Anomaly of Sun in radians.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_true_anomaly1(VALUE self, VALUE vajd){
  double xv = NUM2DBL(func_xv(self, vajd));
  double yv = NUM2DBL(func_yv(self, vajd));
  double vta = anp(atan2(yv, xv));
  return DBL2NUM(roundf(vta * RND12) / RND12);
}
/*
 * call-seq:
 *  true_longitude1(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns True Longitude of Sun in radians.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_true_longitude(VALUE self, VALUE vajd){
  double vml =
  NUM2DBL(func_mean_longitude(self, vajd));
  double veoc =
  NUM2DBL(func_equation_of_center(self, vajd));
  double vtl = anp(vml + veoc);
  return DBL2NUM(roundf(vtl * RND12) / RND12);
}
/*
 * call-seq:
 *  mean_sidereal_time(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Mean Sidereal Time in hours.
 * rounded to 12 decimal places.
 *
*/
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
  sidereal = anp(sidereal * D2R) * R2D;
  /* change to hours */
  return DBL2NUM(fmod(roundf((sidereal / 15.0) * RND12) / RND12, 24.0));
}
/*
 * call-seq:
 *  gmsa0(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Mean Sidereal Time at midnight as angle.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_gmsa0(VALUE self, VALUE vajd){
  double msa0;
  double ajd0 = NUM2DBL(vajd);
  double ajdt = fmod(ajd0, 1.0);
  if (ajdt <= 0.5){
    ajd0 -= 0.5;
    ajd0 = floor(ajd0) + 0.5;
  }
  else{
    ajd0 = floor(ajd0) + 0.5;
  }
  msa0 =
  NUM2DBL(func_mean_sidetime(self, DBL2NUM(ajd0))) * 15;
  return DBL2NUM(roundf(msa0 * RND12) / RND12);
}
/*
 * call-seq:
 *  gmsa(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Mean Sidereal Time as angle.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_gmsa(VALUE self, VALUE vajd){
  double ajd = NUM2DBL(vajd) - 0.5;
  double ajdt = fmod(ajd, 1.0);
  double vtr = ajdt * 24.0 * 1.00273790935 * 15 * D2R;
  double msar0 = NUM2DBL(func_gmsa0(self, vajd)) * D2R;
  double msa = anp(msar0 + vtr) * R2D;
  return DBL2NUM(roundf(msa * RND12) / RND12);
}
/*
 * call-seq:
 *  gmst0(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Mean Sidereal Time at midnight in hours.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_gmst0(VALUE self, VALUE vajd){
  double era0 =
  NUM2DBL(func_gmsa0(self, vajd)) / 15.0;
  return DBL2NUM(roundf(era0 * RND12) / RND12);
}
/*
 * call-seq:
 *  gmst(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Mean Sidereal Time in hours.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_gmst(VALUE self, VALUE vajd){
  double vmst = NUM2DBL(func_gmsa(self, vajd)) / 15.0;
  return DBL2NUM(roundf(vmst * RND12) / RND12);
}
/*
 * call-seq:
 *  radius_vector(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns radius vector.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_rv(VALUE self, VALUE vajd){
  double vxv =
  NUM2DBL(func_xv(self, vajd));
  double vyv =
  NUM2DBL(func_yv(self, vajd));
  double vrv =
  sqrt(vxv * vxv + vyv * vyv);
  return DBL2NUM(roundf(vrv * RND12) / RND12);
}
/*
 * call-seq:
 *  ecliptic_x(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns ecliptic x component.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_ecliptic_x(VALUE self, VALUE vajd){
  double vrv =
  NUM2DBL(func_rv(self, vajd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vajd));
  double vex = vrv * cos(vtl);
  return DBL2NUM(roundf(vex * RND12) / RND12);
}
/*
 * call-seq:
 *  ecliptic_y(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns ecliptic y component.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_ecliptic_y(VALUE self, VALUE vajd){
  double vrv =
  NUM2DBL(func_rv(self, vajd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vajd));
  double vey = vrv * sin(vtl);
  return DBL2NUM(roundf(vey * RND12) / RND12);
}
/*
 * call-seq:
 *  right_ascension(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Right Ascension of Sun in hours.
 * rounded to 12 decimal places.
 *
*/
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
/*
 * call-seq:
 *  gha(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Greenwich Hour Angle in degrees.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_gha(VALUE self, VALUE vajd){
  double gmsa =
  NUM2DBL(func_mean_sidetime(self, vajd)) * 15 * D2R;
  double ra =
  NUM2DBL(func_right_ascension(self, vajd)) * 15 * D2R;
  double gha = anp(gmsa - ra);
  return DBL2NUM(roundf(gha * R2D * RND12) / RND12);
}
/*
 * call-seq:
 *  declination(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns Declination of Sun in degrees.
 * rounded to 12 decimal places.
 *
*/
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
/*
 * call-seq:
 *  local_sidereal_time(ajd, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Longitude,
 * returns Local Mean Sidereal Time in hours.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_local_sidetime(VALUE self, VALUE vajd, VALUE vlon){
  double vst = NUM2DBL(func_mean_sidetime(self, vajd));
  double vlst = vst + NUM2DBL(vlon) / 15.0 ;
  return DBL2NUM(fmod(roundf(vlst * RND12) / RND12, 24.0));
}
/*
 * call-seq:
 *  daylight_time(ajd, lat)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude,
 * returns Hours of Day Light.
 * rounded to 12 decimal places.
 *
*/
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
/*
 * call-seq:
 *  diurnal_arc(ajd, lat)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude,
 * returns Hours from Noon or half Day Light Time.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_diurnal_arc(VALUE self, VALUE vajd, VALUE vlat){
  double jd = floor(NUM2DBL(vajd));
  double dlt = NUM2DBL(func_dlt(self, DBL2NUM(jd), vlat));
  double da = dlt / 2.0;
  return DBL2NUM(roundf(da * RND12) / RND12);
}
/*
 * call-seq:
 *  t_south(ajd, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Longitude,
 * returns Time in Hours when Sun transits local meridian.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_t_south(VALUE self, VALUE vajd, VALUE vlon){
  double jd = floor(NUM2DBL(vajd));
  double lst = NUM2DBL(func_local_sidetime(self, DBL2NUM(jd), vlon));
  double ra = NUM2DBL(func_right_ascension(self, DBL2NUM(jd)));
  double vx = lst - ra;
  double vt = vx - 24.0 * floor(vx * INV24 + 0.5);
  return DBL2NUM(fmod(roundf((12.0 - vt) * RND12) / RND12, 24.0));
}
/*
 * call-seq:
 *  t_rise(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns Time in Hours when Sun rises.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_t_rise(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vajd, vlat));
  return DBL2NUM(fmod(roundf((ts - da) * RND12) / RND12, 24.0));
}
/*
 * call-seq:
 *  t_mid_day(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns Time in Hours when Sun transits local meridian.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_t_mid_day(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  return DBL2NUM(fmod(roundf(ts * RND12) / RND12, 24.0));
}
/*
 * call-seq:
 *  t_set(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns Time in Hours when Sun sets.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_t_set(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vajd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vajd, vlat));
  return DBL2NUM(roundf(fmod((ts + da), 24.0) * RND12) / RND12);
}
/*
 * call-seq:
 *  rise(ajd. lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Ruby DateTime object when Sun rises.
 *
*/
static VALUE func_rise(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double rt = NUM2DBL(func_t_rise(self, vajd, vlat, vlon));
  double ajd = NUM2DBL(vajd);
  double rtajd = floor(ajd) - 0.5 + rt / 24.0;
  VALUE vrt = DBL2NUM(rtajd);
  return func_ajd_2_datetime(self, vrt);
}
/*
 * call-seq:
 *  rise_jd(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Astronomical Julian Day Number when Sun rises.
 *
*/
static VALUE func_rise_jd(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double rt = NUM2DBL(func_t_rise(self, vajd, vlat, vlon));
  double ajd = NUM2DBL(vajd);
  double rtajd = floor(ajd) - 0.5 + rt / 24.0;
  return DBL2NUM(rtajd);
}
/*
 * call-seq:
 *  noon(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Ruby DateTime object when Sun transits
 * local meridian.
 *
*/
static VALUE func_noon(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double nt = NUM2DBL(func_t_south(self, vajd, vlon));
  double ajd = NUM2DBL(vajd);
  double ntajd = floor(ajd) - 0.5 + nt / 24.0;
  VALUE vnt = DBL2NUM(ntajd);
  return func_ajd_2_datetime(self, vnt);
}
/*
 * call-seq:
 *  noon_jd(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Astronomical Julian Day Number when Sun
 * transits local meridian.
 *
*/
static VALUE func_noon_jd(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double nt = NUM2DBL(func_t_south(self, vajd, vlon));
  double ajd = NUM2DBL(vajd);
  double ntajd = floor(ajd) - 0.5 + nt / 24.0;
  return DBL2NUM(ntajd);
}
/*
 * call-seq:
 *  set(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Ruby DateTime object when Sun sets.
 *
*/
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
  return func_ajd_2_datetime(self, vst);
}
/*
 * call-seq:
 *  set_jd(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns a Astronomical Julian Day Number when Sun sets.
 *
*/
static VALUE func_set_jd(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double stajd;
  double st = NUM2DBL(func_t_set(self, vajd, vlat, vlon));
  double nt = NUM2DBL(func_t_mid_day(self, vajd, vlat, vlon));
  double ajd = NUM2DBL(vajd);
  if (st < nt){
    st += 24.0;
  }
  stajd = floor(ajd) - 0.5 + st / 24.0;
  return DBL2NUM(stajd);
}
/*
* macro for days since JD 2000
*/
#define days_since_2000_Jan_0(y,m,d) \
    (367L * (y) - ((7 * ((y) + (((m) + 9) / 12))) / 4) + ((275 * (m))  /9) + (d) - 730531.5L)
/*
 * call-seq:
 *  jd2000_dif(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns the number of days and decimal time
 * since JD 2000.
 *
*/
static VALUE func_jd_from_2000(VALUE self, VALUE vajd){
  double ajd = NUM2DBL(vajd);
  double days = ajd - 2451545.0;
  return INT2NUM(days);
}
/*
 * call-seq:
 *  jd2000_dif_lon(ajd, lon)
 *
 * given an Astronomical Julian Day Number and
 * local  Longitude,
 * returns the number of days and decimal time
 * since JD 2000.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_days_from_2000(VALUE self, VALUE vajd, VALUE vlon){
  double jd = NUM2DBL(vajd);
  double lon = NUM2DBL(vlon);
  double days = jd - DJ00 - lon / 360;
  return DBL2NUM(roundf(days * RND12) / RND12);
}
/*
 * call-seq:
 *  eot(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns equation of time in degrees
 * rounded to 12 decimal places.
 *
*/
static VALUE func_eot(VALUE self, VALUE vajd){
  double ma =
  NUM2DBL(func_mean_anomaly(self, vajd));
  double ta =
  NUM2DBL(func_true_anomaly(self, vajd));
  double tl =
  NUM2DBL(func_true_longitude(self, vajd));
  double ra = 15.0 * D2R *
  NUM2DBL(func_right_ascension(self, vajd));
  return DBL2NUM(roundf(anp(ma - ta + tl - ra) * R2D * RND12) / RND12 );
}
/*
 * call-seq:
 *  eot_jd(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns equation of time as time of a fractional
 * day for easy Julian Number work
 * rounded to 12 decimal places.
 *
*/
static VALUE func_eot_jd(VALUE self, VALUE vajd){
  double veot =
  NUM2DBL(func_eot(self, vajd));
  double jdeot = veot / 360.0;
  return DBL2NUM(roundf(jdeot * RND12) / RND12);
}
/*
 * call-seq:
 *  eot_min(ajd)
 *
 * given an Astronomical Julian Day Number
 * returns equation of time in minutes
 * rounded to 12 decimal places.
 *
*/
static VALUE func_eot_min(VALUE self, VALUE vajd){
  double eot = NUM2DBL(func_eot(self, vajd));
  return DBL2NUM(roundf((eot / 15 * 60) * RND12) / RND12);
}
/*
 * call-seq:
 *  lha(ajd, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Longitude,
 * returns Local Hour Angle in degrees.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_lha(VALUE self, VALUE vajd, VALUE vlon){
  double lon = NUM2DBL(vlon) * D2R;
  double gha = NUM2DBL(func_gha(self, vajd)) * D2R;
  double lha = anp(gha + lon) * R2D;
  return DBL2NUM(roundf(lha * RND12) / RND12);
}
/*
 * call-seq:
 *  altitude(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns the Altitude of the Sun in degrees.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_altitude(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double lat = NUM2DBL(vlat) * D2R;
  double delta = NUM2DBL(func_declination(self, vajd)) * D2R;
  double lha = NUM2DBL(func_lha(self, vajd, vlon)) * D2R;
  double alt =
  asin(sin(lat) * sin(delta) +
    cos(lat) * cos(delta) * cos(lha)) * R2D;
  return DBL2NUM(roundf(alt * RND12) / RND12);
}
/*
 * call-seq:
 *  azimuth(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns the Azimuth of the Sun in degrees.
 * rounded to 12 decimal places.
 *
*/
static VALUE func_azimuth(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double lat = NUM2DBL(vlat) * D2R;
  double delta = NUM2DBL(func_declination(self, vajd)) * D2R;
  double lha = NUM2DBL(func_lha(self, vajd, vlon)) * D2R;
  double az;
  az =
  atan2(sin(lha), cos(lha) * sin(lat) -
            tan(delta) * cos(lat)) * R2D + 180.0;
  return DBL2NUM(roundf(az * RND12) / RND12);
}
/*
 * call-seq:
 *  rise_az(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns the Azimuth of the Sun rise in degrees.
 *
*/
static VALUE func_rise_az(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double rjd = NUM2DBL(func_rise_jd(self, vajd, vlat, vlon));
  double raz = NUM2DBL(func_azimuth(self, DBL2NUM(rjd), vlat, vlon));
  return DBL2NUM(raz);
}
/*
 * call-seq:
 *  noon_az(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns the Azimuth of the Sun at transit in degrees.
 * Note : should be very close to 180 degrees.
 *
*/
static VALUE func_noon_az(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double njd = NUM2DBL(func_noon_jd(self, vajd, vlat, vlon));
  double naz = NUM2DBL(func_azimuth(self, DBL2NUM(njd), vlat, vlon));
  return DBL2NUM(naz);
}
/*
 * call-seq:
 *  set_az(ajd, lat, lon)
 *
 * given an Astronomical Julian Day Number and
 * local Latitude and Longitude,
 * returns the Azimuth of the Sun set in degrees.
 *
*/
static VALUE func_set_az(VALUE self, VALUE vajd, VALUE vlat, VALUE vlon){
  double sjd = NUM2DBL(func_set_jd(self, vajd, vlat, vlon));
  double saz = NUM2DBL(func_azimuth(self, DBL2NUM(sjd), vlat, vlon));
  return DBL2NUM(saz);
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
  rb_define_method(cCalcSun, "altitude", func_altitude, 3);
  rb_define_method(cCalcSun, "azimuth", func_azimuth, 3);
  rb_define_method(cCalcSun, "daylight_time", func_dlt, 2);
  rb_define_method(cCalcSun, "declination", func_declination, 1);
  rb_define_method(cCalcSun, "diurnal_arc", func_diurnal_arc, 2);
  rb_define_method(cCalcSun, "eccentricity", func_eccentricity, 1);
  rb_define_method(cCalcSun, "eccentric_anomaly", func_eccentric_anomaly, 1);
  rb_define_method(cCalcSun, "ecliptic_x", func_ecliptic_x, 1);
  rb_define_method(cCalcSun, "ecliptic_y", func_ecliptic_y, 1);
  rb_define_method(cCalcSun, "eot", func_eot, 1);
  rb_define_method(cCalcSun, "eot_jd", func_eot_jd, 1);
  rb_define_method(cCalcSun, "eot_min", func_eot_min, 1);
  rb_define_method(cCalcSun, "equation_of_center", func_equation_of_center, 1);
  rb_define_method(cCalcSun, "gha", func_gha, 1);
  rb_define_method(cCalcSun, "gmsa0", func_gmsa0, 1);
  rb_define_method(cCalcSun, "gmsa", func_gmsa, 1);
  rb_define_method(cCalcSun, "gmst0", func_gmst0, 1);
  rb_define_method(cCalcSun, "gmst", func_gmst, 1);
  rb_define_method(cCalcSun, "jd", func_get_jd, 1);
  rb_define_method(cCalcSun, "jd2000_dif", func_jd_from_2000, 1);
  rb_define_method(cCalcSun, "jd2000_dif_lon", func_days_from_2000, 2);
  rb_define_method(cCalcSun, "lha", func_lha, 2);
  rb_define_method(cCalcSun, "local_sidereal_time", func_local_sidetime, 2);
  rb_define_method(cCalcSun, "longitude_of_perihelion", func_longitude_of_perihelion, 1);
  rb_define_method(cCalcSun, "mean_anomaly", func_mean_anomaly, 1);
  rb_define_method(cCalcSun, "mean_longitude", func_mean_longitude, 1);
  rb_define_method(cCalcSun, "mean_sidereal_time", func_mean_sidetime, 1);
  rb_define_method(cCalcSun, "noon", func_noon, 3);
  rb_define_method(cCalcSun, "noon_jd", func_noon_jd, 3);
  rb_define_method(cCalcSun, "noon_az", func_noon_az, 3);
  rb_define_method(cCalcSun, "obliquity_of_ecliptic", func_obliquity_of_ecliptic, 1);
  rb_define_method(cCalcSun, "radius_vector", func_rv, 1);
  rb_define_method(cCalcSun, "right_ascension", func_right_ascension, 1);
  rb_define_method(cCalcSun, "rise", func_rise, 3);
  rb_define_method(cCalcSun, "rise_jd", func_rise_jd, 3);
  rb_define_method(cCalcSun, "rise_az", func_rise_az, 3);
  rb_define_method(cCalcSun, "set", func_set, 3);
  rb_define_method(cCalcSun, "set_jd", func_set_jd, 3);
  rb_define_method(cCalcSun, "set_az", func_set_az, 3);
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