# encoding: utf-8

require 'rubygems'
require 'bundler'
begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  $stderr.puts e.message
  $stderr.puts 'Run `bundle install` to install missing gems'
  exit e.status_code
end
require 'bundler/gem_tasks'

require 'hoe'

require 'rake'
require 'rake/clean'
require 'rake/extensiontask'
require 'rake/testtask'
require 'rake/win32'
require 'rdoc/task'
require 'rspec/core/rake_task'
require 'yard'

require 'rbconfig'
include RbConfig

# Hoe.plugins.delete :newb
# Hoe.plugins.delete :test
# Hoe.plugins.delete :signing
# Hoe.plugins.delete :publish
# Hoe.plugins.delete :clean
# Hoe.plugins.delete :package
# Hoe.plugins.delete :compiler
# Hoe.plugins.delete :debug
# Hoe.plugins.delete :rcov
# Hoe.plugins.delete :gemcutter
# Hoe.plugins.delete :racc
# Hoe.plugins.delete :inline
# Hoe.plugins.delete :gem_prelude_sucks
# Hoe.plugins.delete :flog
# Hoe.plugins.delete :flay
# Hoe.plugins.delete :deps
# Hoe.plugins.delete :minitest
# Hoe.plugins.delete :rdoc
# Hoe.plugins.delete :travis

# Hoe.plugin :newb
# Hoe.plugin :test
# Hoe.plugin :signing
# Hoe.plugin :publish
# Hoe.plugin :clean
# Hoe.plugin :package
# Hoe.plugin :compiler
# Hoe.plugin :debug
# Hoe.plugin :rcov
# Hoe.plugin :gemcutter
# Hoe.plugin :racc
# Hoe.plugin :inline
# Hoe.plugin :gem_prelude_sucks
# Hoe.plugin :flog
# Hoe.plugin :flay
# Hoe.plugin :deps
Hoe.plugin :minitest
# Hoe.plugin :rdoc
Hoe.plugin :travis

Hoe.spec 'calc_sun' do
  developer('Douglas Allen', 'kb9agt@gmail.com')
  license('MIT')
  self.readme_file   = 'README.rdoc'
  self.history_file  = 'History.rdoc'
  self.version = '1.2.6'
  self.extra_dev_deps += [
    ['rake-compiler',      '~> 0.9.2'],
    ['rake-compiler-dock', '~> 0.5.1']
  ]
  self.spec_extras = { extensions: ['ext/calc_sun/extconf.rb',
                                    'ext/side_time/extconf.rb'] }
  Rake::ExtensionTask.new('calc_sun', spec) do |ext|
    ext.lib_dir = File.join('lib', 'calc_sun')
  end

  Rake::ExtensionTask.new('side_time', spec) do |ext|
    ext.lib_dir = File.join('lib', 'side_time')
  end
end

Rake::Task[:test].prerequisites << :compile
# vim: syntax=ruby

Rake::TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/calc_sun/test_*.rb', 'test/side_time/test_*.rb']
end

task default: :test
