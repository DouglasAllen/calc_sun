#!/usr/bin/env ruby

require 'hoe'
require 'rake/testtask'
require 'rake/extensiontask'
include Rake

Hoe.plugin :manifest
Hoe.plugin :minitest
Hoe.plugin :test

Hoe.spec 'calc_sun' do
  self.version = '0.1.1'
  developer('kb9agt', 'kb9agt@gmail.com')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'HISTORY.rdoc'

  extra_dev_deps << ['rake-compiler', '~> 0.9.3']
  self.spec_extras = { extensions: ['ext/calc_sun/extconf.rb'] }

  ExtensionTask.new('calc_sun', spec) do |ext|
    ext.lib_dir = File.join('lib', 'calc_sun')
  end
end

Task[:test].prerequisites << :compile
# vim: syntax=ruby

TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/calc_sun/test_*.rb']
end

task default: :test
