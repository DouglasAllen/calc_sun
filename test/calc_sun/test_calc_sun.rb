
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
# doc
class TestDateTime < Test::Unit::TestCase # MiniTest::Test
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
end

# doc
class TestAnles < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_ajd = 2_452_930.312847222
    @t_date = '2003-10-17'
    @t_datetime = DateTime.jd(@t_ajd + 0.5)
    @t_jd = Date.parse('2003-10-17').jd.to_f
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_mean_anomaly
    assert_equal(
      4.937428762624,
      @t.mean_anomaly(@t_ajd)
    )
  end

  def test_eccentricity
    assert_equal(
      0.016707405824,
      @t.eccentricity(@t_ajd)
    )
  end

  def test_equation_of_center
    assert_equal(
      -0.032718979072,
      @t.equation_of_center(@t_ajd)
    )
  end

  def test_true_anomaly
    assert_equal(
      4.904710045696,
      @t.true_anomaly(@t_ajd)
    )
  end

  def test_mean_longitude
    assert_equal(
      3.593570353152,
      @t.mean_longitude(@t_ajd)
    )
  end

  def test_eccentric_anomaly
    assert_equal(
      3.586383151104,
      @t.eccentric_anomaly(@t_ajd)
    )
  end

  def test_obliquity_of_ecliptic
    assert_equal(
      0.409084198912,
      @t.obliquity_of_ecliptic(@t_ajd)
    )
  end

  def test_xv
    assert_equal(
      -0.919408214016,
      @t.xv(@t_ajd)
    )
  end

  def test_yv
    assert_equal(
      -0.430208712704,
      @t.yv(@t_ajd)
    )
  end

  def test_longitude_of_perihelion
    assert_equal(
      4.93938016256,
      @t.longitude_of_perihelion(@t_ajd)
    )
  end

  def test_true_longitude
    assert_equal(
      3.560904851456,
      @t.true_longitude(@t_ajd)
    )
  end

  def test_radius_vector
    assert_equal(
      1.015081795584,
      @t.radius_vector(@t_ajd)
    )
  end
end

# doc
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
      -0.927144411136,
      @t.ecliptic_x(@t_ajd)
    )
  end

  def test_ecliptic_y
    assert_equal(
      -0.413272604672,
      @t.ecliptic_y(@t_ajd)
    )
  end

  def test_declination
    assert_equal(
      -9.198886715392,
      @t.declination(@t_ajd)
    )
  end

  def test_equation_of_time
    assert_equal(
      3.656521089024,
      @t.equation_of_time(@t_ajd)
    )
  end

  def test_eot_min
    assert_equal(
      14.626084356096,
      @t.eot_min(@t_ajd)
    )
  end

  def test_gmsa0
    assert_equal(
      24.024820219904,
      @t.gmsa0(@t_ajd)
    )
  end

  def test_gmst0
    assert_equal(
      1.60165462016,
      @t.gmst0(@t_ajd)
    )
  end

  def test_gmsa
    assert_equal(
      136.649820340224,
      @t.gmsa(@t_ajd)
    )
  end

  def test_gmst
    assert_equal(
      9.109988442112,
      @t.gmst(@t_ajd)
    )
  end
end

#
class TestSunTimes < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_jd = Date.parse('2003-10-17').jd.to_f
    @t_ajd = 2_452_930.312847222
    @t_lat = 39.742476
    @t_lon = -105.1786
  end

  def test_daylight_time
    assert_equal(
      11.114992631808,
      @t.daylight_time(@t_ajd, @t_lat)
    )
  end

  def test_diurnal_arc
    assert_equal(
      5.557496315904,
      @t.diurnal_arc(@t_ajd, @t_lat)
    )
  end

  def test_local_sidereal_time
    assert_equal(
      14.222466482176,
      @t.local_sidereal_time(@t_ajd, @t_lon)
    )
  end

  def test_right_ascension
    assert_equal(
      13.48286283776,
      @t.right_ascension(@t_ajd)
    )
  end

  def test_t_south
    assert_equal(
      18.769795612672,
      @t.t_south(@t_ajd, @t_lon)
    )
  end

  def test_t_rise
    assert_equal(
      13.21229877248,
      @t.t_rise(@t_ajd, @t_lat, @t_lon)
    )
  end

  def test_t_mid_day
    assert_equal(
      18.769795612672,
      @t.t_mid_day(@t_ajd, @t_lat, @t_lon)
    )
  end

  def test_t_set
    assert_equal(
      0.327291928576,
      @t.t_set(@t_ajd, @t_lat, @t_lon)
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
