#!/usr/bin/env ruby

require 'rubygems'
gem 'hoe'
require 'hoe'
require 'rake/testtask'
require 'rake/extensiontask'
include Rake


Hoe.spec 'calc_sun' do
  developer 'kb9agt', 'kb9agt@gmail.com'
  license 'MIT'
  self.version = '1.2.6'
  self.extra_dev_deps += [
    ['rake-compiler',      '~> 0.9.2'],
    ['rake-compiler-dock', '~> 0.5.1']
  ]
  self.spec_extras = { extensions: ['ext/calc_sun/extconf.rb',
                                    'ext/side_time/extconf.rb'] }
  ExtensionTask.new('calc_sun', spec) do |ext|
    ext.lib_dir = File.join('lib', 'calc_sun')
  end

  ExtensionTask.new('side_time', spec) do |ext|
    ext.lib_dir = File.join('lib', 'side_time')
  end
end

Task[:test].prerequisites << :compile
# vim: syntax=ruby

TestTask.new(:test) do |t|
  t.libs << 'test'
  # t.libs << 'lib'
  t.test_files = FileList['test/calc_sun/test_*.rb', 'test/side_time/test_*.rb']
end

task default: :test
