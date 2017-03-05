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
    @t_ajd = 2_452_930.312847222
    @t_datetime = DateTime.jd(@t_ajd + 0.5)
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_date
    assert_equal(
      '2003-10-17T19:30:29+00:00',
      @t.s_datetime(@t_datetime.to_s).to_s
    )
  end

  def test_ajd
    assert_equal(
      2_452_930.312847222,
      @t.ajd(@t_datetime).round(12)
    )
  end

  def test_jd
    assert_equal(
      2_452_930.0,
      @t.jd(@t_datetime)
    )
  end

  def test_gmst
    assert_equal(
      21.234371884854,
      @t.gmst(@t_datetime).round(12)
    )
  end

  def test_lmst
    assert_equal(
      14.222465218187,
      @t.lmst(@t_datetime, @t_lon).round(12)
    )
  end

  def test_gmst_angle
    assert_equal(
      318.515578272811,
      (@t.gmst(@t_datetime) * 15.0).round(12)
    )
  end

  def test_gmst_angle_180
    assert_equal(
      138.515578272811,
      ((@t.gmst(@t_datetime) * 15.0 + 180.0) % 360.0).round(12)
    )
  end

  def test_comp_gmsts1
    assert_equal(
      21.234371884854,
      @t.gmst(@t_datetime).round(12)
    )
  end



  def test_comp_gmsts1
    assert_equal(
      21.14307169892,
      @v.gmst(@t_ajd + 0.5).round(12)
    )
  end
end
