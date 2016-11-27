#include <ruby.h>
// #include <math.h>

#ifndef DBL2NUM
# define DBL2NUM(dbl) rb_float_new(dbl)
#endif

#define R2D 57.295779513082320876798154814105
#define D2R 0.017453292519943295769236907684886
#define M2PI M_PI * 2.0

// static ID id_status;

static VALUE 
t_init(VALUE self)
{
	return self;
}

static VALUE
func_mean_anomally(VALUE self, VALUE vd)
{
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(fmod((357.5291  + 0.98560028 * NUM2DBL(vd)) * D2R, M2PI));
}

static VALUE
func_eccentricity(VALUE self, VALUE vd)
{
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(0.016709 - 1.151e-9 * NUM2DBL(vd));
}

static VALUE
func_equation_of_center(VALUE self, VALUE vd)
{
  VALUE vma = func_mean_anomally(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(1.9148 * D2R * sin(NUM2DBL(vma)) +
                 0.0200 * D2R * sin(2 * NUM2DBL(vma)) +
                 0.0003 * D2R * sin(3 * NUM2DBL(vma)));
}

static VALUE
func_true_anomally(VALUE self, VALUE vd)
{
  VALUE vma = func_mean_anomally(self, vd);
  VALUE veoc = func_equation_of_center(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));  
  return DBL2NUM(NUM2DBL(vma) + NUM2DBL(veoc));
}

static VALUE
func_mean_longitude(VALUE self, VALUE vd)
{
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(fmod(280.4664567 * D2R +
                        0.9856473601037645 * D2R * NUM2DBL(vd), M2PI));
}

static VALUE
func_eccentric_anomally(VALUE self, VALUE vd)
{
  VALUE ml = func_mean_longitude(self, vd);
  VALUE e = func_eccentricity(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));  
  return DBL2NUM(NUM2DBL(ml) + NUM2DBL(e) * sin(NUM2DBL(ml)) * (1.0 + NUM2DBL(e) * cos(NUM2DBL(ml))));
}

static VALUE
func_obliquity_of_ecliptic(VALUE self, VALUE vd)
{
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM((23.439291 - 3.563E-7 * NUM2DBL(vd)) * D2R);
}

static VALUE
func_longitude_of_perihelion(VALUE self, VALUE vd)
{
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(fmod(282.9404 * D2R + 4.70935e-05 * D2R * NUM2DBL(vd), M2PI));
}

static VALUE
func_xv(VALUE self, VALUE vd)
{
  VALUE vea = func_eccentric_anomally(self, vd);
  VALUE ve = func_eccentricity(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(cos(NUM2DBL(vea)) - NUM2DBL(ve));
}

static VALUE
func_yv(VALUE self, VALUE vd)
{
  VALUE vea = func_eccentric_anomally(self, vd);
  VALUE ve = func_eccentricity(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));
  return DBL2NUM(sqrt(1.0 - NUM2DBL(ve) * NUM2DBL(ve)) * sin(NUM2DBL(vea)));
}

static VALUE
func_true_longitude(VALUE self, VALUE vd)
{
  VALUE vta = func_true_anomally(self, vd);
  VALUE vap = func_longitude_of_perihelion(self, vd);
  // rb_ivar_set(self, id_status, INT2FIX(0));  
  return DBL2NUM(fmod(NUM2DBL(vta) + NUM2DBL(vap), M2PI));
}

static VALUE
func_rv(VALUE self, VALUE vd)
{
  VALUE vxv = func_xv(self, vd);
  VALUE vyv = func_yv(self, vd);
  return DBL2NUM(sqrt(NUM2DBL(vxv) * NUM2DBL(vxv) + NUM2DBL(vyv) * NUM2DBL(vyv)));
}

static VALUE
func_ecliptic_x(VALUE self, VALUE vd)
{
  VALUE vrv = func_rv(self, vd);
  VALUE vtl = func_true_longitude(self, vd);
  return DBL2NUM(NUM2DBL(vrv) * cos(NUM2DBL(vtl)));
}

static VALUE
func_ecliptic_y(VALUE self, VALUE vd)
{
  VALUE vrv = func_rv(self, vd);
  VALUE vtl = func_true_longitude(self, vd);
  return DBL2NUM(NUM2DBL(vrv) * sin(NUM2DBL(vtl)));
}

static VALUE
func_right_ascension(VALUE self, VALUE vd)
{
  VALUE vey = func_ecliptic_y(self, vd);
  VALUE vooe = func_obliquity_of_ecliptic(self, vd);
  VALUE vex = func_ecliptic_x(self, vd);
  return DBL2NUM(fmod(atan2(NUM2DBL(vey) * cos(NUM2DBL(vooe)), NUM2DBL(vex)) + M2PI, M2PI));
}

static VALUE
func_declination(VALUE self, VALUE vd)
{
  VALUE vex = func_ecliptic_x(self, vd);
  VALUE vey = func_ecliptic_y(self, vd);
  VALUE vooe = func_obliquity_of_ecliptic(self, vd);
  return DBL2NUM(atan2(NUM2DBL(vey) * sin(NUM2DBL(vooe)), sqrt(NUM2DBL(vex) * NUM2DBL(vex) + NUM2DBL(vey) * NUM2DBL(vey))));
}

static VALUE
func_sidetime(VALUE self, VALUE vd)
{
  return DBL2NUM(fmod( ( 180 + 357.52911 + 282.9404 ) + 
			( 0.985600281725 + 4.70935E-5 ) * NUM2DBL(vd), 360.0) * D2R);
} 

void Init_calc_sun(void)
{
  VALUE cCalcSun = rb_define_class("CalcSun", rb_cObject);
  rb_define_method(cCalcSun, "initialize", t_init, 0); 
  rb_define_method(cCalcSun, "mean_anomally", func_mean_anomally, 1);
  rb_define_method(cCalcSun, "eccentricity", func_eccentricity, 1);
  rb_define_method(cCalcSun, "equation_of_center", func_equation_of_center, 1);
  rb_define_method(cCalcSun, "true_anomally", func_true_anomally, 1);
  rb_define_method(cCalcSun, "mean_longitude", func_mean_longitude, 1);
  rb_define_method(cCalcSun, "eccentric_anomally", func_eccentric_anomally, 1);
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
}