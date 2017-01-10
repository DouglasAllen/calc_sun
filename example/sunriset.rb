#!/usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require 'calc_sun'
require 'date' # for now we still need the Date class.

lat = 51.4770228
lon = -0.0001147

cs = CalcSun.new
date_str = '2017-01-09'
day = cs.date(date_str)

cs.rise(day, lat, lon)
cs.noon(day, lat, lon)
cs.set(day, lat, lon)

