require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'sidereal_time'
require 'calc_sun'
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
    @v = CalcSun.new
    @t = SideTime.new
    @t.date = @t.s_datetime('2017-01-11T00:10:10+00:00')
    @lon = -90.0
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

  def test_gmst
    assert_equal(
      7.549536649231,
      @t.gmst(@t.date).round(12)
    )
  end

  def test_lmst
    assert_equal(
      1.549536649231,
      @t.lmst(@t.date, @lon).round(12)
    )
  end

  def test_gmst_angle
    assert_equal(
      113.243049738463,
      (@t.gmst(@t.date) * 15.0).round(12)
    )
  end

  def test_gmst_angle_180
    assert_equal(
      293.243049738463,
      (@t.gmst(@t.date) * 15.0 + 180.0).round(12)
    )
  end

  def test_comp_gmsts
    assert_equal(
      @v.gmst(@t.date).round(12),
      @t.gmst(@t.date).round(12)
    )
  end
end
