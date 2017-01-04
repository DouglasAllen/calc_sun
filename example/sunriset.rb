#!/usr/bin/env ruby
require 'benchmark'
lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

lat = 51.4770228
lon = -0.0001147

cs = CalcSun.new

require 'date'
day = Date.today
puts day.jd

jd = cs.df2000(day, lon)
puts jd
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

# n = 1_000_000
# Benchmark.bm(7) do |x|
#   x.report('rise:') { n.times { ; cs.t_rise(jd, lat, lon); } }
#   x.report('midday:') { n.times { ; cs.t_mid_day(jd, lat, lon); } }
#   x.report('set:') { n.times { ; cs.t_set(jd, lat, lon); } }
# end
