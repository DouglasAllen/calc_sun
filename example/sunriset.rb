#!/usr/bin/env ruby
require 'benchmark'
lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

cs = CalcSun.new

require 'date'
# DJ00 = DateTime.parse('2000-01-01T12:00:00').jd

day = Date.parse('2016-12-25').jd
lat = 51.4770228
lon = -0.0001147 / 360.0
jd = day - CalcSun::DJ00 - lon
puts
puts Date.jd(day)
cs.rise(jd, lat, lon)
cs.noon(jd, lat, lon)
cs.set(jd, lat, lon)

# printf("\n")
# rise = cs.t_rise(jd, lat, lon)
# printf("\tSun rises \t\t\t : %2.0f:%02.0f\n",
#        rise.floor, (rise % 1 * 60.0).floor)
# midday = cs.t_mid_day(jd, lat, lot)
# midday = cs.t_south(jd, lat, lon)
# printf("\tSun midday \t\t\t : %2.0f:%02.0f UTC\n",
#        midday.floor, ((midday % 1.0) * 60).floor)
# set = cs.t_set(jd, lat, lon)
# printf("\tSun sets \t\t\t : %2.0f:%02.0f UTC\n",
#        set.floor, (set % 1 * 60.0).floor)

n = 1_000_000
Benchmark.bm(7) do |x|
  x.report('rise:') { n.times { ; cs.t_rise(jd, lat, lon); } }
  x.report('midday:') { n.times { ; cs.t_mid_day(jd, lat, lon); } }
  x.report('set:') { n.times { ; cs.t_set(jd, lat, lon); } }
end
