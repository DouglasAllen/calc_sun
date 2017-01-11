require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'sidereal_time'
#
class SideTimeVersion < Test::Unit::TestCase #  Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::SideTime::VERSION
  end
end
#
class SideTimeInit < Test::Unit::TestCase #  Minitest::Test
  def setup
    @t = SideTime.new
  end

  def test_that_it_initalizes
    assert_equal(@t.class, SideTime.new.class)
  end
end
#
class SideTimeObject < Test::Unit::TestCase #  Minitest::Test
  def method_private
    assert_equal(@t = SideTime.initialize)
  end
end
#
class TestSideTime < Test::Unit::TestCase
  def setup
    @t = SideTime.new
    @t.date = @t.s_datetime('2017-01-11T00:10:10+00:00')
  end

  def test_jd
    assert_equal(
      2_457_765.0,
      @t.jd(@t.date)
    )
  end

  def test_ajd
    assert_equal(
      2_457_764.507060185,
      @t.ajd(@t.date)
    )
  end

  def test_mean_sidereal_time
    assert_equal(
      7.549536649230867,
      @t.mean_sidereal_time(@t.date)
    )
  end
end
