#!/usr/bin/env ruby

require 'rubygems'
gem 'hoe'
require 'hoe'
require 'rake/testtask'
require 'rake/extensiontask'
include Rake

Hoe.plugin :bundler
Hoe.plugin :manifest
# Hoe.plugin :minitest
# Hoe.plugin :test

Hoe.spec 'calc_sun' do
  developer 'kb9agt', 'kb9agt@gmail.com'
  license 'MIT'

  self.version = '1.2.6'

  self.readme_file   = 'README.rdoc'
  self.history_file  = 'HISTORY.rdoc'

  self.extra_dev_deps += [
    ['hoe-bundler',        '~> 1.2.0'],
    ['hoe-debugging',      '~> 1.2.1'],
    ['hoe-gemspec',        '~> 1.0.0'],
    ['hoe-git',            '~> 1.6.0'],
    ['minitest',           '~> 5.8.4'],
    ['rake',               '~> 10.5.0'],
    ['rake-compiler',      '~> 0.9.2'],
    ['rake-compiler-dock', '~> 0.5.1']
  ]

  self.spec_extras = { extensions: ['ext/calc_sun/extconf.rb'] }

  ExtensionTask.new('calc_sun', spec) do |ext|
    ext.lib_dir = File.join('lib', 'calc_sun')
  end
end

Task[:test].prerequisites << :compile
# vim: syntax=ruby

TestTask.new(:test) do |t|
  t.libs << 'test'
  # t.libs << 'lib'
  t.test_files = FileList['test/calc_sun/test_*.rb']
end

task default: :test
