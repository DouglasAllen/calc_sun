
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'
cs = CalcSun.new

require 'date'
J2000 = DateTime.parse('2000-01-01T12:00:00').jd

lat = 41.95
lon = -88.75
day = Date.parse('2016-12-25')
jd = day.jd - J2000 - lon / 360.0
rise = cs.t_rise(jd, lon, lat)
set = cs.t_set(jd, lon, lat)

printf("\n")

printf("\tSun rises \t\t\t : %2.0f:%02.0f UTC\n",
       rise.floor, (rise % 1 * 60.0).floor)

printf("\tSun at south \t\t : %2.0f:%02.0f UTC\n",
       ((rise + set) / 2.0).floor,
       (((rise + set) / 2.0 % 1.0) * 60).floor)

printf("\tSun sets \t\t\t : %2.0f:%02.0f UTC\n",
       set.floor, (set % 1 * 60.0).floor)
