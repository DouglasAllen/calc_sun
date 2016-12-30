#!/usr/bin/env ruby

require 'hoe'
require 'rake/testtask'
require 'rake/extensiontask'


Hoe.plugin :manifest

Hoe.spec 'calc_sun' do
  self.version = '0.1.1'
  developer('kb9agt', 'kb9agt@gmail.com')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'HISTORY.rdoc'

  extra_dev_deps << ['rake-compiler', '~> 0.9.3']
  self.spec_extras = { extensions: ['ext/calc_sun/extconf.rb'] }

  Rake::ExtensionTask.new('calc_sun', spec) do |ext|
    ext.lib_dir = File.join('lib', 'calc_sun')
  end
end

Rake::Task[:test].prerequisites << :compile
# vim: syntax=ruby

Rake::TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/*_test.rb']
end

task default: :test
