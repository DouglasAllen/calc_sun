
require 'test/unit'
require 'calc_sun'
require 'date'
#
class TestCalcSun < Test::Unit::TestCase

  def setup
    @t = CalcSun.new
    # @t_ajd = 2_457_719.0 - 2_451_545.0
    @t_ajd = 0.0
  end

  def test_mean_anomally
    assert_equal(6.240059966692059, @t.mean_anomally(@t_ajd))
  end

  def test_eccentricity
    assert_equal(0.016709, @t.eccentricity(@t_ajd))
  end

  def test_equation_of_center
    assert_equal(-0.0014715287836893668,
                 @t.equation_of_center(@t_ajd))
  end

  def test_true_anomally
    assert_equal(6.23858843790837, @t.true_anomally(@t_ajd))
  end

  def test_mean_longitude
    assert_equal(4.89506311081711, @t.mean_longitude(@t_ajd))
  end

  def test_eccentric_anomaly
    assert_equal(4.8785822508615935, @t.eccentric_anomally(@t_ajd))
  end

  def test_obliquity_of_ecliptic
    assert_equal(0.4090928022830742, @t.obliquity_of_ecliptic(@t_ajd))
  end

  def test_xv
    assert_equal(0.14872027767348, @t.xv(@t_ajd))
  end

  def test_yv
    assert_equal(-0.9860839740994566, @t.yv(@t_ajd))
  end

  def test_longitude_of_perihelion
    assert_equal(4.938241566909764, @t.longitude_of_perihelion(@t_ajd))
  end

  def test_true_longitude
    assert_equal(4.893644697638548, @t.true_longitude(@t_ajd))
  end

  def test_rv
    assert_equal(0.9972358421993539, @t.rv(@t_ajd))
  end

  def test_ecliptic_x
    assert_equal(0.1797665809291078, @t.ecliptic_x(@t_ajd))
  end

  def test_ecliptic_y
    assert_equal(-0.9808992309855856, @t.ecliptic_y(@t_ajd))
  end

  def test_right_ascension
    assert_equal(4.909544229885129, @t.right_ascension(@t_ajd))
  end

  def test_declination
    assert_equal(-0.37294996573991546, @t.declination(@t_ajd))
  end
end
