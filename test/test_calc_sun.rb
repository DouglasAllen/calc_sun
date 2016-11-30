
require 'test/unit'
require 'calc_sun'
require 'date'
#
class TestCalcSun < Test::Unit::TestCase

  def setup
    @t = CalcSun.new
    # @t_ajd = 2_457_719.0 - 2_451_545.0
    @t_ajd = 0.0
    @t_lat = 41.9475360
  end

  def test_mean_anomally
    assert_equal(6.240059966692059, @t.mean_anomally(@t_ajd))
  end

  def test_eccentricity
    assert_equal(0.016709, @t.eccentricity(@t_ajd))
  end

  def test_equation_of_center
    assert_equal(-0.0014713808670014348,
                 @t.equation_of_center(@t_ajd))
  end

  def test_true_anomally
    assert_equal(6.238588585825058, @t.true_anomally(@t_ajd))
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
    assert_equal(4.893644845555237, @t.true_longitude(@t_ajd))
  end

  def test_rv
    assert_equal(0.9972358421993539, @t.rv(@t_ajd))
  end

  def test_ecliptic_x
    assert_equal(0.179766726020472, @t.ecliptic_x(@t_ajd))
  end

  def test_ecliptic_y
    assert_equal(-0.9808992043950975, @t.ecliptic_y(@t_ajd))
  end

  def test_right_ascension
    assert_equal(4.909544390126607, @t.right_ascension(@t_ajd))
  end

  def test_declination
    assert_equal(-0.3729499565416288, @t.declination(@t_ajd))
  end

  def test_sidereal_time
    assert_equal(100.4695099999999, @t.sidereal_time(@t_ajd))
  end

  def test_dlt
    assert_equal(9.179609772086533, @t.dlt(@t_ajd, @t_lat))
  end
end
