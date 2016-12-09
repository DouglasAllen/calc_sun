require 'rubygems'
gem 'minitest'
require 'minitest/autorun'
# require 'test/unit'
lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'
require 'date'
#
class TestCalcSun < MiniTest::Test
# class TestCalcSun < Test::Unit::TestCase

  def setup
    @t = CalcSun.new
    # @t_ajd = 2_457_719.0 - 2_451_545.0
    @t_ajd = 0.0
    @t_lat = 41.9475360
  end

  def test_mean_anomally
    assert_equal(6.240059966692,
                 @t.mean_anomally(@t_ajd).round(12))
  end

  def test_eccentricity
    assert_equal(0.016709, @t.eccentricity(@t_ajd))
  end

  def test_equation_of_center
    assert_equal(-0.001471380867,
                 @t.equation_of_center(@t_ajd).round(12))
  end

  def test_true_anomally
    assert_equal(6.238588585825,
                 @t.true_anomally(@t_ajd).round(12))
  end

  def test_mean_longitude
    assert_equal(4.895063110817,
                 @t.mean_longitude(@t_ajd).round(12))
  end

  def test_eccentric_anomaly
    assert_equal(4.878582250862,
                 @t.eccentric_anomally(@t_ajd).round(12))
  end

  def test_obliquity_of_ecliptic
    assert_equal(0.409092802283,
                 @t.obliquity_of_ecliptic(@t_ajd).round(12))
  end

  def test_xv
    assert_equal(0.148720277673,
                 @t.xv(@t_ajd).round(12))
  end

  def test_yv
    assert_equal(-0.986083974099,
                 @t.yv(@t_ajd).round(12))
  end

  def test_longitude_of_perihelion
    assert_equal(4.93824156691,
                 @t.longitude_of_perihelion(@t_ajd).round(12))
  end

  def test_true_longitude
    assert_equal(4.893644845555,
                 @t.true_longitude(@t_ajd).round(12))
  end

  def test_rv
    assert_equal(0.997235842199,
                 @t.rv(@t_ajd).round(12))
  end

  def test_ecliptic_x
    assert_equal(0.17976672602,
                 @t.ecliptic_x(@t_ajd).round(12))
  end

  def test_ecliptic_y
    assert_equal(-0.980899204395,
                 @t.ecliptic_y(@t_ajd).round(12))
  end

  def test_right_ascension
    assert_equal(4.909544390127,
                 @t.right_ascension(@t_ajd).round(12))
  end

  def test_declination
    assert_equal(-0.372949956542,
                 @t.declination(@t_ajd).round(12))
  end

  def test_sidereal_time
    assert_equal(100.46951,
                 @t.sidereal_time(@t_ajd).round(12))
  end

  def test_dlt
    assert_equal(9.179609772087,
                 @t.dlt(@t_ajd, @t_lat).round(12))
  end
end
