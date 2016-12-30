# -*- ruby -*-
require 'bundler/gem_tasks'
require 'rubygems'
require 'hoe'
require 'rake/testtask'
require 'rake/extensiontask'
require 'rake/testtask'

Hoe.plugin :manifest

Hoe.spec 'calc_sun' do
  self.version = '0.1.0'
  developer('kb9agt', 'kb9agt@gmail.com')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'HISTORY.rdoc'
  self.extra_rdoc_files = FileList['*.rdoc']
  extra_dev_deps << ['rake-compiler', '>= 0']
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
  t.test_files = FileList['test/**/*_test.rb']
end

task default: :test
