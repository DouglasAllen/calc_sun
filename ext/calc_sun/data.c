/* data.c                                 */
/* by John Abercrombie & Jason Davis      */
/* gives the planetary coordinates in xyz */


#include <stdio.h>
#include <math.h>
#include <time.h>

/* conversion ratio for Astronomical Units */
#ifndef AU_TO_KM
#define AU_TO_KM 1496.13400
#endif

/* setup for objectinfo array */
#define NUM_OF_OBJECTS 10
#define PIECES_OF_INFO 7
#define NUM_XYZ       3

/* if PI's not defined, define it */
#ifndef PI
#define PI 3.14159265358979323846
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

/* Get the days to J2000 h is UT in decimal hours only works between 1901 to 2099 */
double GetDaysTillJ2000(int year, int month, int day, int hour){
     double days_till_J2000;
     days_till_J2000 = 367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day - 730531.5 + hour / 24;
     return(days_till_J2000);
}

/* if x>0 returns 1, if x = 0, returns 0, and if x<0, returns -1 */
/* sgn is a conversion of the BASIC function SGN                 */  
int sgn (double x) {
  if (x > 0)
    return(1);
  else if (x < 0)
    return(-1);
  else
    return(0);
}

/* the function below returns the true integer part */
int int_part(double x) {
	int integer, out_sgn, out_int_part;
        out_sgn      = sgn(x);
	integer      = fabs(x);	
        out_int_part = out_sgn * integer;
	return integer;
}

/* the function below returns an angle in the range 0 to two pi */
double range(double x){
	double b = x / TWOPI;
	double a = TWOPI * (b - int_part(b));
	if (a < 0)
	  a = TWOPI + a;
	return a;
}

double kep(double m, double ecc, int eps) {

/*   
     returns the true anomaly given
     m - the mean anomaly in radians
     ecc - the eccentricity of the orbit
     eps - the convergence paramter (8 or 9 is usually fine 12 or 14 for very accurate work)
*/

  double e, v, delta;

  e = m;
  delta = .05;
  while ((fabs(delta)) >= (pow(10, -eps))) {
      delta = e - ecc * sin(e) - m;
      e     = e - delta / (1 - ecc * cos(e));
  }
  v = 2 * atan(pow(((1 + ecc) / (1 - ecc)), .5) * tan(.5 * e));
  if (v < 0)
    v = v + TWOPI;
  return(v);
}

// Position of planet in its orbit

void planetpos(int objectnum, double d) {

  double ip,op,pp,ap,np,ep,lp,mp,vp,rp,pre_x, pre_y, pre_z;

  ip = objectinfo[objectnum][0];
  op = objectinfo[objectnum][1];
  pp = objectinfo[objectnum][2];
  ap = objectinfo[objectnum][3];
  np = objectinfo[objectnum][4];
  ep = objectinfo[objectnum][5];
  lp = objectinfo[objectnum][6];

  mp = range(np * (d - ELDATE) + lp - pp);
  vp = kep(mp, ep, 12);
  rp = ap * (1 - ep * ep) / (1 + ep * cos(vp));

  /*   heliocentric rectangular coordinates of planet */
  pre_x = (rp * (cos(op) * cos(vp + pp - op) - sin(op) * sin(vp + pp - op) * cos(ip))) * AU_TO_KM;
  pre_y = (rp * (sin(op) * cos(vp + pp - op) + cos(op) * sin(vp + pp - op) * cos(ip))) * AU_TO_KM;
  pre_z = (rp * (sin(vp + pp - op) * sin(ip))) * AU_TO_KM;

  xyz[objectnum][0] = pre_x;
  xyz[objectnum][1] = pre_y;
  xyz[objectnum][2] = pre_z; 
}


