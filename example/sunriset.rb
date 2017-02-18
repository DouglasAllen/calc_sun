#!/usr/bin/env ruby

lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

lat = 39.742476
lon = -105.1786

require 'calc_sun'
cs = CalcSun.new

# require 'date' is included in CalcSun class
ajd = DateTime.new(2003, 10, 17, 12, 30, 30).ajd.to_f
puts cs.ajd2dt(ajd)

# set the date/time with #set_datetime as date and time string.
# this just creates a DateTime object and you could create your own.
day = cs.set_datetime('2003-10-17 12:30:30')
puts day

puts "Sun rise: #{cs.rise(day.ajd, lat, lon).httpdate}"
puts "Sun noon: #{cs.noon(day.ajd, lat, lon).httpdate}"
puts "Sun set: #{cs.set(day.ajd, lat, lon).httpdate}"
