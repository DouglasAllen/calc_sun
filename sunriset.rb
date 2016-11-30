
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

require 'date'
J2000 = DateTime.parse('2000-01-01T12:00:00').jd
INV360 = 1.0 / 360.0
def rev180(x)
  x - 360.0 * (x * INV360 + 0.5).floor
end

include Math
lat = 41.95
lon = -88.75
day = Date.parse('2016-11-29')
jd = day.jd
d = jd - lon / 360.0 - J2000
cs = CalcSun.new
st = cs.sidereal_time(d)
lst = (st + 180 + lon) % 360.0

ra = cs.right_ascension(d) * 180 / PI
t_south = 12.0 - rev180(lst - ra) / 15.0

diurnal_arc = cs.dlt(d, lat)
rise = t_south - diurnal_arc / 2.0
set = t_south + diurnal_arc / 2.0

printf("\n")

printf("\tSun rises \t\t\t : %2.0f:%02.0f UTC\n",
       rise.floor, (rise % 1 * 60.0).floor)

printf("\tSun at south \t\t : %2.0f:%02.0f UTC\n",
       ((rise + set) / 2.0).floor,
       (((rise + set) / 2.0 % 1.0) * 60).floor)

printf("\tSun sets \t\t\t : %2.0f:%02.0f UTC\n",
       set.floor, (set % 1 * 60.0).floor)
