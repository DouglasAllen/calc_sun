
require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'
#
class CalcSunVersion < Test::Unit::TestCase #  Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::CalcSun::VERSION
  end
end

require 'date'
#
class TestCalcSun100 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_ajd = 2_452_930.312847222
    @t_date = '2003-10-17'
    @t_datetime = DateTime.jd(@t_ajd + 0.5)
    @t_jd = Date.parse('2003-10-17').jd.to_f
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_set_datetime
    assert_equal(
      'Fri, 17 Oct 2003 19:30:29 GMT',
      @t.set_datetime(@t_datetime.to_s).httpdate
    )
  end

  def test_ajd2dt
    assert_equal(
      'Fri, 17 Oct 2003 19:30:29 GMT',
      @t.ajd2dt(@t_ajd).httpdate
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
      @t.jd(@t_datetime).round(12)
    )
  end

  def test_mean_anomaly
    assert_equal(
      4.937428967814,
      @t.mean_anomaly(@t_ajd).round(12)
    )
  end

  def test_eccentricity
    assert_equal(
      0.016707405505,
      @t.eccentricity(@t_ajd).round(12)
    )
  end

  def test_equation_of_center
    assert_equal(
      -0.032718976284,
      @t.equation_of_center(@t_ajd).round(12)
    )
  end

  def test_true_anomaly
    assert_equal(
      4.904709991531,
      @t.true_anomaly(@t_ajd).round(12)
    )
  end

  def test_mean_longitude
    assert_equal(
      3.593570228596,
      @t.mean_longitude(@t_ajd).round(12)
    )
  end

  def test_eccentric_anomaly
    assert_equal(
      3.586383016561,
      @t.eccentric_anomaly(@t_ajd).round(12)
    )
  end

  def test_obliquity_of_ecliptic
    assert_equal(
      0.409084187565,
      @t.obliquity_of_ecliptic(@t_ajd).round(12)
    )
  end

  def test_xv
    assert_equal(
      -0.919408290978,
      @t.xv(@t_ajd).round(12)
    )
  end

  def test_yv
    assert_equal(
      -0.430208593998,
      @t.yv(@t_ajd).round(12)
    )
  end

  def test_longitude_of_perihelion
    assert_equal(
      4.939380206285,
      @t.longitude_of_perihelion(@t_ajd).round(12)
    )
  end

  def test_true_longitude
    assert_equal(
      3.560904890636,
      @t.true_longitude(@t_ajd).round(12)
    )
  end

  def test_radius_vector
    assert_equal(
      1.015081789743,
      @t.radius_vector(@t_ajd).round(12)
    )
  end
end

#
class TestCalcSun200 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_jd = Date.parse('2003-10-17').jd.to_f
    @t_ajd = 2_452_930.312847222
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_ecliptic_x
    assert_equal(
      -0.927144408603,
      @t.ecliptic_x(@t_ajd).round(12)
    )
  end

  def test_ecliptic_y
    assert_equal(
      -0.413272652694,
      @t.ecliptic_y(@t_ajd).round(12)
    )
  end

  def test_declination
    assert_equal(
      -9.198887763129,
      @t.declination(@t_ajd).round(12)
    )
  end

  def test_equation_of_time
    assert_equal(
      3.656528697898,
      @t.equation_of_time(@t_ajd).round(12))
  end

  def test_eot_min
    assert_equal(
      14.626114791593,
      @t.eot_min(@t_ajd).round(12))
  end

  def test_gmst0
    assert_equal(
      1.601654765396,
      @t.gmst0(@t_ajd).round(12)
    )
  end

  def test_gmst
    assert_equal(
      9.109988098232,
      @t.gmst(@t_ajd).round(12)
    )
  end
end

#
class TestCalcSun300 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_jd = Date.parse('2003-10-17').jd.to_f
    @t_ajd = 2_452_930.312847222
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_daylight_time
    assert_equal(
      11.114992936688,
      @t.daylight_time(@t_ajd, @t_lat).round(12)
    )
  end

  def test_diurnal_arc
    assert_equal(
      5.557496468344,
      @t.diurnal_arc(@t_ajd, @t_lat).round(12)
    )
  end

  def test_local_sidereal_time
    assert_equal(
      14.222465218187,
      @t.local_sidereal_time(@t_ajd, @t_lon).round(12)
    )
  end

  def test_right_ascension
    assert_equal(
      13.482863468938,
      @t.right_ascension(@t_ajd).round(12)
    )
  end

  def test_t_south
    assert_equal(
      18.769795904184,
      @t.t_south(@t_ajd, @t_lon).round(12)
    )
  end

  def test_t_rise
    assert_equal(
      13.21229943584,
      @t.t_rise(@t_ajd, @t_lat,  @t_lon).round(12)
    )
  end

  def test_t_mid_day
    assert_equal(
      18.769795904184,
      @t.t_mid_day(@t_ajd, @t_lat, @t_lon).round(12)
    )
  end

  def test_t_set
    assert_equal(
      0.327292372527,
      @t.t_set(@t_ajd, @t_lat, @t_lon).round(12)
    )
  end

  def test_rise_time
    assert_equal(
      'Fri, 17 Oct 2003 13:12:44 GMT',
      @t.rise(@t_ajd, @t_lat, @t_lon).httpdate
    )
  end

  def test_noon_time
    assert_equal(
      'Fri, 17 Oct 2003 18:46:11 GMT',
      @t.noon(@t_ajd, @t_lat, @t_lon).httpdate
    )
  end

  def test_set_time
    assert_equal(
      'Sat, 18 Oct 2003 00:19:38 GMT',
      @t.set(@t_ajd, @t_lat, @t_lon).httpdate
    )
  end
end
