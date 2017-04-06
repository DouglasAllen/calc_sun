
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
    @tz = -7
    @jd = Date.parse('2003-10-17').jd.to_f
    @time = Time.new(2003, 10, 17, 12, 30, 30.01, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @datetime = DateTime.jd(@ajd + 0.5)
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_set_datetime
    assert_equal(
      'Fri, 17 Oct 2003 19:30:30 GMT',
      @t.set_datetime(@datetime.to_s).httpdate
    )
  end

  def test_parse
    # just a reverse check. DateTime.parse is the set_datetime
    assert_equal(
      DateTime.parse(@datetime.httpdate).to_s,
      @t.set_datetime(@datetime.httpdate).to_s
    )
  end

  def test_ajd2dt
    assert_equal(
      @time.httpdate,
      @t.ajd2dt(@ajd).httpdate
    )
  end

  def test_ajd
    assert_equal(
      @ajd,
      @t.ajd(@datetime)
    )
  end

  def test_jd
    assert_equal(
      @jd,
      @t.jd(@datetime)
    )
  end
end

# doc
class TestAngles1 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @jd = Date.parse('2003-10-17').jd.to_f
    @time = Time.new(2003, 10, 17, 12, 30, 30, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @datetime = DateTime.jd(@ajd + 0.5)
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_mean_anomaly
    assert_equal(
      4.937428762624,
      @t.mean_anomaly(@ajd)
    )
  end

  def test_eccentricity
    assert_equal(
      0.016707405824,
      @t.eccentricity(@ajd)
    )
  end

  def test_equation_of_center
    assert_equal(
      -0.0327188992,
      @t.equation_of_center(@ajd)
    )
  end

  def test_true_anomaly
    assert_equal(
      4.904710045696,
      @t.true_anomaly(@ajd)
    )
  end

  def test_mean_longitude
    assert_equal(
      3.593570353152,
      @t.mean_longitude(@ajd)
    )
  end

  def test_eccentric_anomaly
    assert_equal(
      3.586383151104,
      @t.eccentric_anomaly(@ajd)
    )
  end

  def test_obliquity_of_ecliptic
    assert_equal(
      0.409084198912,
      @t.obliquity_of_ecliptic(@ajd)
    )
  end

  def test_longitude_of_perihelion
    assert_equal(
      4.93938016256,
      @t.longitude_of_perihelion(@ajd)
    )
  end

  def test_xv
    assert_equal(
      -0.919408214016,
      @t.xv(@ajd)
    )
  end

  def test_yv
    assert_equal(
      -0.430208712704,
      @t.yv(@ajd)
    )
  end

  def test_true_anomaly1
    assert_equal(
      3.579247853568,
      @t.true_anomaly1(@ajd)
    )
  end

  def test_true_longitude
    assert_equal(
      3.560904851456,
      @t.true_longitude(@ajd)
    )
  end
end

# doc
class TestAngles2 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @jd = Date.parse('2003-10-17').jd.to_f
    @time = Time.new(2003, 10, 17, 12, 30, 30, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @datetime = DateTime.jd(@ajd + 0.5)
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_mean_sidereal_time
    assert_equal(
      21.234372837376,
      @t.mean_sidereal_time(@ajd)
    )
  end

  def test_gmsa0
    assert_equal(
      25.089399586816,
      @t.gmsa0(@ajd)
    )
  end

  def test_gmsa
    assert_equal(
      318.515579977728,
      @t.gmsa(@ajd)
    )
  end

  def test_gmst0
    assert_equal(
      1.672626700288,
      @t.gmst0(@ajd)
    )
  end

  def test_gmst
    assert_equal(
      21.234372837376,
      @t.gmst(@ajd)
    )
  end
end

# doc
class TestAngles3 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @time = Time.new(2003, 10, 17, 12, 30, 30, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @jd = Date.parse('2003-10-17').jd.to_f
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_radius_vector
    assert_equal(
      1.015081795584,
      @t.radius_vector(@ajd)
    )
  end

  def test_ecliptic_x
    assert_equal(
      -0.927144411136,
      @t.ecliptic_x(@ajd)
    )
  end

  def test_ecliptic_y
    assert_equal(
      -0.413272604672,
      @t.ecliptic_y(@ajd)
    )
  end

  def test_right_ascension
    assert_equal(
      13.48286283776,
      @t.right_ascension(@ajd)
    )
  end

  def test_gha
    assert_equal(
      116.27264999424,
      @t.gha(@ajd)
    )
  end

  def test_declination
    assert_equal(
      -9.198886715392,
      @t.declination(@ajd)
    )
  end

  def test_local_sidereal_time
    assert_equal(
      14.222466482176,
      @t.local_sidereal_time(@ajd, @lon)
    )
  end
end

#
class TestSunTimes < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @time = Time.new(2003, 10, 17, 12, 30, 30, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @jd = Date.parse('2003-10-17').jd.to_f
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_daylight_time
    assert_equal(
      11.114991583232,
      @t.daylight_time(@ajd, @lat)
    )
  end

  def test_diurnal_arc
    assert_equal(
      5.557495791616,
      @t.diurnal_arc(@ajd, @lat)
    )
  end

  def test_t_south
    assert_equal(
      18.769797709824,
      @t.t_south(@ajd, @lon)
    )
  end

  def test_t_rise
    assert_equal(
      13.212301918208,
      @t.t_rise(@ajd, @lat, @lon)
    )
  end

  def test_t_mid_day
    assert_equal(
      18.769797709824,
      @t.t_mid_day(@ajd, @lat, @lon)
    )
  end

  def test_t_set
    assert_equal(
      0.32729350144,
      @t.t_set(@ajd, @lat, @lon)
    )
  end

  def test_rise
    assert_equal(
      'Fri, 17 Oct 2003 13:12:44 GMT',
      @t.rise(@ajd, @lat, @lon).httpdate
    )
  end

  def test_rise_jd
    assert_equal(
      2_452_930.0505125797,
      @t.rise_jd(@ajd, @lat, @lon)
    )
  end

  def test_noon
    assert_equal(
      'Fri, 17 Oct 2003 18:46:11 GMT',
      @t.noon(@ajd, @lat, @lon).httpdate
    )
  end

  def test_noon_jd
    assert_equal(
      2_452_930.2820749045,
      @t.noon_jd(@ajd, @lat, @lon)
    )
  end

  def test_set
    assert_equal(
      'Sat, 18 Oct 2003 00:19:38 GMT',
      @t.set(@ajd, @lat, @lon).httpdate
    )
  end

  def test_set_jd
    assert_equal(
      2_452_930.5136372293,
      @t.set_jd(@ajd, @lat, @lon)
    )
  end
end

#
class TestSunTimes < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @time = Time.new(2003, 10, 17, 12, 30, 30, '-07:00').getgm.to_datetime
    @ajd = @time.ajd # 2_452_930.312847222
    @jd = Date.parse('2003-10-17').jd.to_f
    @lat = 39.742476
    @lon = -105.1786
  end

  def test_jd2000_dif
    assert_equal(
      1385,
      @t.jd2000_dif(@ajd)
    )
  end

  def test_jd2000_dif_lon
    assert_equal(
      1385.605036507136,
      @t.jd2000_dif_lon(@ajd, @lon)
    )
  end

  def test_equation_of_time
    assert_equal(
      3.656521089024,
      @t.equation_of_time(@ajd)
    )
  end

  def test_eot_min
    assert_equal(
      14.626084356096,
      @t.eot_min(@ajd)
    )
  end

  def test_lha
    assert_equal(
      11.094050471936,
      @t.lha(@ajd, @lon)
    )
  end

  def test_altitude
    assert_equal(
      39.98945902592,
      @t.altitude(@ajd, @lat, @lon)
    )
  end

  def test_azimuth
    assert_equal(
      194.354333351936,
      @t.azimuth(@ajd, @lat, @lon)
    )
  end

  def test_rise_az
    assert_equal(
      101.228025479168,
      @t.rise_az(@ajd, @lat, @lon)
    )
  end

  def test_noon_az
    assert_equal(
      180.018873368576,
      @t.noon_az(@ajd, @lat, @lon)
    )
  end

  def test_set_az
    assert_equal(
      258.665965682688,
      @t.set_az(@ajd, @lat, @lon)
    )
  end
end
