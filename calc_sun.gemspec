# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun/version'

Gem::Specification.new do |spec|
  spec.name          = 'calc_sun'
  spec.version       = CalcSun::VERSION
  spec.author        = 'Douglas Allen'
  spec.email         = ['kb9agt@gmail.com']

  spec.summary       = 'Calculates Sun Times ea. rise, noon, set'
  spec.description   = 'supply the jd date, latitude, and longitude in the calls
                        to each method t_rise(jd, lat, lon),
                        t_mid_day(jd, lat, lon), and t_set(jd, lat, lon)'
  spec.homepage      = 'https://github.com/DouglasAllen/calc_sun'
  spec.license       = 'MIT'
  require 'rake'
  spec.files = FileList['lib     .rb',
                        '[A-Z]*',
                        'test/   *'].to_a
  spec.extensions << 'ext/calc_sun/extconf.rb'
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ['lib']
  spec.platform = Gem::Platform::RUBY
  spec.add_development_dependency 'bundler'#, '~> 1.13'
  spec.add_development_dependency 'hoe-bundler'#, '~> 1.2', '>= 1.2.0'
  spec.add_development_dependency 'hoe-debugging'#, '~> 1.2', '>= 1.2.1'
  spec.add_development_dependency 'hoe-gemspec'#, '~> 1.0', '>= 1.0.0'
  spec.add_development_dependency 'hoe-git'#, '~> 1.6', '>= 1.6.0'
  spec.add_development_dependency 'minitest'#, '~> 5.8', '>= 5.8.4'
  spec.add_development_dependency 'rake'#, '~> 10.5', '>= 10.5.0'
end
