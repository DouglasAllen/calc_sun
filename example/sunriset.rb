#!/usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'calc_sun'
# require 'date' # for now we still need the Date class.

date_str = Date.today.to_s
lat = 51.4770228
lon = -0.0001147

cs = CalcSun.new
cs.date = date_str
p cs.date
day = cs.sdate(cs.date)

cs.rise(day, lat, lon)
cs.noon(day, lat, lon)
cs.set(day, lat, lon)

