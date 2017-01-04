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
# define DJ00 2451545.0
/* conversion ratio for Astronomical Units */
#ifndef AU_TO_KM
#define AU_TO_KM 1496.13400
#endif
/* define two-pi and how to get Radians */
#ifndef TWOPI
#define TWOPI 2*PI
#endif

#ifndef RADS
#define RADS PI/180
#endif

/* define constants for getting planet position */
#define DATE_OF_ELEMENTS 2450680.5
#define DATE_OF_MEAN_ELEMENTS 2451545

#define ELDATE (DATE_OF_ELEMENTS - 2451545)

/* setup for objectinfo array */
#define NUM_OF_OBJECTS 10
#define PIECES_OF_INFO 7
#define NUM_XYZ       3

double xyz[NUM_OF_OBJECTS][NUM_XYZ] = {
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 },
  { 0.0, 0.0, 0.0 }
};

/*   below are the osculating elements for JD = 2450680.5
referred to mean ecliptic and equinox of J2000
i - inclination
o - longitude of ascending node at date of elements
p - longitude of perihelion at date of elements
a - mean distance (au)
n - daily motion
e - eccentricity of orbit
l - mean longitude at date of elements
   objectinfo is a 2-dimentional array with all planetary info
   implemented as [object][info]
   The info is in the order of i o p a n e l */

double objectinfo[NUM_OF_OBJECTS][PIECES_OF_INFO] = {
{/* Mercury */
1.22615358, 0.84358570, 1.35182732, 0.3870978, 0.07142503, 0.2056324, 5.48772864},
{/* Venus */
0.05924904, 1.33847380, 2.29966328, 0.7233238, 0.02796293, 0.0067933, 4.13539098},
{/* Earth */
0.00000715, 6.09468974, 1.79510081, 1.00002, 0.01720161, 0.0166967, 5.73172287},
{/* Mars */
0.03228719, 0.86509687, 5.86584567, 1.5236365, .0091466, 0.0934231, 4.58022986},
{/* Jupiter */
0.02277009, 1.75355499, 0.2739783, 5.202597, 0.0014503, 0.0484646, 5.62973107},
{/* Saturn */
0.04337562, 1.98331886, 1.55095193, 9.57189999, 0.00058096, 0.0531651, 0.36577895},
{/* Uranus */
0.0134989, 1.29320869, 3.06620665, 19.30181, 0.00202859, 0.0428959, 5.291658},
{/* Neptune */
0.03085917, 2.30021305, 0.12576843, 30.26664, .00010331, 0.0102981, 5.23361585},
{/* Pluto */
0.29882429, 1.92655202, 3.92354379, 39.5804, .00006908, 0.2501272, 4.11488598},
{/* Earth's Moon */
0.02373719, 2.18380483, 1.45187308, 26176.3035239/* km */, 0.22807144, 0.054900, 5.73172287}
};

#define FLOOR(a) lrintf(floorf(a))
#define FLOAT(a) (float)a
static VALUE JULIAN;        /* Date::JULIAN */
static VALUE GREGORIAN;     /* Date::GREGORIAN */
static VALUE ITALY;         /* Date::ITALY */

static inline int
civil_to_jd(int y, int m, int d, VALUE sg)
{
  int a, b, jd;
  if ( m <= 2 ) {
    y-= 1;
    m+= 12;
  }
  a = y / 100;
  b = 2 - a + (a / 4);
  jd = FLOOR(365.25 * (y + 4716)) +
    FLOOR(30.6001 * (m + 1)) +
    d + b - 1524;
  if ( sg == JULIAN || (sg != GREGORIAN && jd < FIX2INT(sg)) )
    jd -= b;
  return jd;
}

static VALUE t_init(VALUE self){
  return self;
}

static VALUE func_rev12(VALUE self, VALUE vx){
  double x = NUM2DBL(vx);
  return DBL2NUM(x - 24.0 * floor(x * INV24 + 0.5));
}

static VALUE func_mean_anomaly(VALUE self, VALUE vd){
  double d = NUM2DBL(vd);
  double vma =
  fmod(
    (357.5291  +
     0.98560028 * d
    ) * D2R, M2PI);
  return DBL2NUM(vma);
}

static VALUE func_eccentricity(VALUE self, VALUE vd){
  double d = NUM2DBL(vd);
  double ve =
  0.016709 -
  1.151e-9 * d;
  return DBL2NUM(ve);
}

static VALUE func_equation_of_center(VALUE self, VALUE vd){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vd));
  double ve =
  NUM2DBL(func_eccentricity(self, vd));
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

static VALUE func_true_anomaly(VALUE self, VALUE vd){
  double vma =
  NUM2DBL(func_mean_anomaly(self, vd));
  double veoc =
  NUM2DBL(func_equation_of_center(self, vd));
  double vta = vma + veoc;
  return DBL2NUM(vta);
}

static VALUE func_mean_longitude(VALUE self, VALUE vd){
  double d = NUM2DBL(vd);
  double vml =
  fmod(
    (280.4664567 +
     0.9856473601037645 * d
    ) * D2R, M2PI);
  return DBL2NUM(vml);
}

static VALUE func_eccentric_anomaly(VALUE self, VALUE vd){
  double ve =
  NUM2DBL(func_eccentricity(self, vd));
  double vml =
  NUM2DBL(func_mean_longitude(self, vd));
  double vea =
  vml + ve * sin(vml) * (1.0 + ve * cos(vml));
  return DBL2NUM(vea);
}

static VALUE func_obliquity_of_ecliptic(VALUE self, VALUE vd){
  double d = NUM2DBL(vd);
  double vooe =
  (23.439291 - 3.563E-7 * d) * D2R;
  return DBL2NUM(vooe);
}

static VALUE func_longitude_of_perihelion(VALUE self, VALUE vd){
  double d = NUM2DBL(vd);
  double vlop =
  fmod(
    (282.9404 +
     4.70935e-05 * d
    ) * D2R, M2PI);
  return DBL2NUM(vlop);
}

static VALUE func_xv(VALUE self, VALUE vd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vd));
  double ve =
  NUM2DBL(func_eccentricity(self, vd));
  double vxv = cos(vea) - ve;
  return DBL2NUM(vxv);
}

static VALUE func_yv(VALUE self, VALUE vd){
  double vea =
  NUM2DBL(func_eccentric_anomaly(self, vd));
  double ve =
  NUM2DBL(func_eccentricity(self, vd));
  double vyv =
  sqrt(1.0 - ve * ve) * sin(vea);
  return DBL2NUM(vyv);
}

static VALUE func_true_longitude(VALUE self, VALUE vd){
  double vta =
  NUM2DBL(func_true_anomaly(self, vd));
  double vlop =
  NUM2DBL(func_longitude_of_perihelion(self, vd));
  double vtl =
  fmod(vta + vlop, M2PI);
  return DBL2NUM(vtl);
}

static VALUE func_rv(VALUE self, VALUE vd){
  double vxv =
  NUM2DBL(func_xv(self, vd));
  double vyv =
  NUM2DBL(func_yv(self, vd));
  double vrv =
  sqrt(vxv * vxv + vyv * vyv);
  return DBL2NUM(vrv);
}

static VALUE func_ecliptic_x(VALUE self, VALUE vd){
  double vrv =
  NUM2DBL(func_rv(self, vd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vd));
  double vex = vrv * cos(vtl);
  return DBL2NUM(vex);
}

static VALUE func_ecliptic_y(VALUE self, VALUE vd){
  double vrv =
  NUM2DBL(func_rv(self, vd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vd));
  double vey = vrv * sin(vtl);
  return DBL2NUM(vey);
}

static VALUE func_right_ascension(VALUE self, VALUE vd){
  double vey =
  NUM2DBL(func_ecliptic_y(self, vd));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vd));
  double vex =
  NUM2DBL(func_ecliptic_x(self, vd));
  double vra =
  fmod(atan2(vey * cos(vooe), vex) + M2PI, M2PI);
  return DBL2NUM(vra * R2D / 15.0);
}

static VALUE func_declination(VALUE self, VALUE vd){
  double vex =
  NUM2DBL(func_ecliptic_x(self, vd));
  double vey =
  NUM2DBL(func_ecliptic_y(self, vd));
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vd));
  double ver = sqrt(vex * vex + vey * vey);
  double vz = vey * sin(vooe);
  double vdec = atan2(vz, ver);
  return DBL2NUM(vdec);
}

static VALUE func_sidetime(VALUE self, VALUE vjd){
  double vd = NUM2DBL(vjd);
  double vst =
  fmod(
    (180 + 357.52911 + 282.9404) +
    (0.985600281725 + 4.70935E-5) * vd, 360.0);
  return DBL2NUM(vst / 15.0);
}

static VALUE func_local_sidetime(VALUE self, VALUE vjd, VALUE vlon){
  double vst = NUM2DBL(func_sidetime(self, vjd));
  double vlst = NUM2DBL(vlon) / 15.0 + 12.0 + vst;
  return DBL2NUM(fmod(vlst, 24.0));
}

static VALUE func_dlt(VALUE self, VALUE vd, VALUE vlat){
  double vsin_alt = sin(-0.8333 * D2R);
  double vlat_r = NUM2DBL(vlat) * D2R;
  double vcos_lat = cos(vlat_r);
  double vsin_lat = sin(vlat_r);
  double vooe =
  NUM2DBL(func_obliquity_of_ecliptic(self, vd));
  double vtl =
  NUM2DBL(func_true_longitude(self, vd));
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

static VALUE func_diurnal_arc(VALUE self, VALUE vjd, VALUE vlat){
  double dlt = NUM2DBL(func_dlt(self, vjd, vlat));
  double da = dlt / 2.0;
  return DBL2NUM(da);
}

static VALUE func_t_south(VALUE self, VALUE vjd, VALUE vlon){
  double lst = NUM2DBL(func_local_sidetime(self, vjd, vlon));
  double ra = NUM2DBL(func_right_ascension(self, vjd));
  double vx = lst - ra;
  double vt = vx - 24.0 * floor(vx * INV24 + 0.5);
  return DBL2NUM(12 - vt);
}

static VALUE func_t_rise(VALUE self, VALUE vjd,  VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vjd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vjd, vlat));
  return DBL2NUM(ts - da);
}

static VALUE func_t_mid_day(VALUE self, VALUE vjd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vjd, vlon));
  return DBL2NUM(ts);
}

static VALUE func_t_set(VALUE self, VALUE vjd, VALUE vlat, VALUE vlon){
  double ts = NUM2DBL(func_t_south(self, vjd, vlon));
  double da = NUM2DBL(func_diurnal_arc(self, vjd, vlat));
  return DBL2NUM(ts + da);
}

static VALUE func_rise(VALUE self, VALUE vjd, VALUE vlat, VALUE vlon){
  double rt = NUM2DBL(func_t_rise(self, vjd, vlat, vlon));
  printf("\tSun rises \t\t\t : %2.0f:%02.0f \n",
  floor(rt), floor(fmod(rt, 1) * 60.0));
  return Qnil;
}

static VALUE func_noon(VALUE self, VALUE vjd, VALUE vlat, VALUE vlon){
  double nt = NUM2DBL(func_t_south(self, vjd, vlon));
  printf("\tSun midday \t\t\t : %2.0f:%02.0f \n",
  floor(nt), floor(fmod(nt, 1) * 60.0));
  return Qnil;
}

static VALUE func_set(VALUE self, VALUE vjd, VALUE vlat, VALUE vlon){
  double st = NUM2DBL(func_t_set(self, vjd, vlat, vlon));
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

static VALUE func_get_jd(VALUE self, VALUE vdate_time){
  int year = NUM2INT(rb_funcall(vdate_time, rb_intern("year"), 0));
  int month = NUM2INT(rb_funcall(vdate_time, rb_intern("month"), 0));
  int day = NUM2INT(rb_funcall(vdate_time, rb_intern("day"), 0));
  double jd = civil_to_jd(year, month, day, ITALY) - 13;
  return DBL2NUM(jd);
}

static VALUE func_jd_from_2000(VALUE self, VALUE vdate_time){
  int year = NUM2INT(rb_funcall(vdate_time, rb_intern("year"), 0));
  int month = NUM2INT(rb_funcall(vdate_time, rb_intern("month"), 0));
  int day = NUM2INT(rb_funcall(vdate_time, rb_intern("day"), 0));
  double days = GetDaysTillJ2000(year, month, day, 12);
  return DBL2NUM(days);
}

static VALUE func_days_from_2000(VALUE self, VALUE vdate_time, VALUE vlon){
  double jd = NUM2DBL(func_get_jd(self, vdate_time));
  double lon = NUM2DBL(vlon);
  double days = jd - DJ00 - lon / 360;
  return DBL2NUM(days);
}

void Init_calc_sun(void){
  VALUE cCalcSun = rb_define_class("CalcSun", rb_cObject);
  rb_define_method(cCalcSun, "initialize", t_init, 0);
  rb_define_const(cCalcSun, "DJ00", DBL2NUM(DJ00));
  rb_define_method(cCalcSun, "jd", func_get_jd, 1);
  rb_define_method(cCalcSun, "a2000", func_jd_from_2000, 1);
  rb_define_method(cCalcSun, "df2000", func_days_from_2000, 2);
  rb_define_method(cCalcSun, "reverse_12", func_rev12, 1);
  rb_define_method(cCalcSun, "mean_anomaly", func_mean_anomaly, 1);
  rb_define_method(cCalcSun, "eccentricity", func_eccentricity, 1);
  rb_define_method(cCalcSun, "equation_of_center", func_equation_of_center, 1);
  rb_define_method(cCalcSun, "true_anomaly", func_true_anomaly, 1);
  rb_define_method(cCalcSun, "mean_longitude", func_mean_longitude, 1);
  rb_define_method(cCalcSun, "eccentric_anomaly", func_eccentric_anomaly, 1);
  rb_define_method(cCalcSun, "obliquity_of_ecliptic", func_obliquity_of_ecliptic, 1);
  rb_define_method(cCalcSun, "longitude_of_perihelion", func_longitude_of_perihelion, 1);
  rb_define_method(cCalcSun, "xv", func_xv, 1);
  rb_define_method(cCalcSun, "yv", func_yv, 1);
  rb_define_method(cCalcSun, "true_longitude", func_true_longitude, 1);
  rb_define_method(cCalcSun, "rv", func_rv, 1);
  rb_define_method(cCalcSun, "ecliptic_x", func_ecliptic_x, 1);
  rb_define_method(cCalcSun, "ecliptic_y", func_ecliptic_y, 1);
  rb_define_method(cCalcSun, "right_ascension", func_right_ascension, 1);
  rb_define_method(cCalcSun, "declination", func_declination, 1);
  rb_define_method(cCalcSun, "sidereal_time", func_sidetime, 1);
  rb_define_method(cCalcSun, "local_sidereal_time", func_local_sidetime, 2);
  rb_define_method(cCalcSun, "dlt", func_dlt, 2);
  rb_define_method(cCalcSun, "diurnal_arc", func_diurnal_arc, 2);
  rb_define_method(cCalcSun, "t_south", func_t_south, 2);
  rb_define_method(cCalcSun, "t_rise", func_t_rise, 3);
  rb_define_method(cCalcSun, "t_mid_day", func_t_mid_day, 3);
  rb_define_method(cCalcSun, "t_set", func_t_set, 3);
  rb_define_method(cCalcSun, "rise", func_rise, 3);
  rb_define_method(cCalcSun, "noon", func_noon, 3);
  rb_define_method(cCalcSun, "set", func_set, 3);
}