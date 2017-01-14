#!/usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

lat = 51.4770228 # 051:28:37.28
lon = -0.0001147 # -000:00:00.41

require 'calc_sun'
cs = CalcSun.new

# require 'date' included in CalcSun class
date_time_utc = DateTime.now.to_time.utc
cs.date = date_time_utc.to_s
puts "UTC: #{cs.date}"

# set the date with sdate as date is just an ivar
day = cs.sdate(cs.date)

cs.rise(day, lat, lon)
cs.noon(day, lat, lon)
cs.set(day, lat, lon)

