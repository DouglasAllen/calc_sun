
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
    @t_date = Date.parse('2000-01-01')
    @t_ajd = 0.0
    @t_lat = 0.0
    @t_lon = 0.0
  end

  def test_jd
    assert_equal(
      2_451_545.0,
      @t.jd(@t_date).round(12)
    )
  end

  def test_mean_anomaly
    assert_equal(
      6.240059966692,
      @t.mean_anomaly(@t_date).round(12)
    )
  end

  def test_eccentricity
    assert_equal(
      0.016709,
      @t.eccentricity(@t_date)
    )
  end

  def test_equation_of_center
    assert_equal(
      -0.001471380867,
      @t.equation_of_center(@t_date).round(12)
    )
  end

  def test_true_anomaly
    assert_equal(
      6.238588585825,
      @t.true_anomaly(@t_date).round(12)
    )
  end

  def test_mean_longitude
    assert_equal(
      4.895063110817,
      @t.mean_longitude(@t_date).round(12)
    )
  end

  def test_eccentric_anomaly
    assert_equal(
      4.878582250862,
      @t.eccentric_anomaly(@t_date).round(12)
    )
  end

  def test_obliquity_of_ecliptic
    assert_equal(
      0.409092802283,
      @t.obliquity_of_ecliptic(@t_date).round(12)
    )
  end

  def test_xv
    assert_equal(
      0.148720277673,
      @t.xv(@t_date).round(12)
    )
  end

  def test_yv
    assert_equal(
      -0.986083974099,
      @t.yv(@t_date).round(12)
    )
  end

  def test_longitude_of_perihelion
    assert_equal(
      4.93824156691,
      @t.longitude_of_perihelion(@t_date).round(12)
    )
  end

  def test_true_longitude
    assert_equal(
      4.893644845555,
      @t.true_longitude(@t_date).round(12)
    )
  end

  def test_radius_vector
    assert_equal(
      0.997235842199,
      @t.radius_vector(@t_date).round(12)
    )
  end
end

#
class TestCalcSun200 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_date = Date.parse('2017-01-03')
    @t_lat = 0.0
    @t_lon = 0.0
  end

  def test_ecliptic_x
    assert_equal(
      0.229457387793,
      @t.ecliptic_x(@t_date).round(12)
    )
  end

  def test_ecliptic_y
    assert_equal(
      -0.96964143837,
      @t.ecliptic_y(@t_date).round(12)
    )
  end

  def test_right_ascension
    assert_equal(
      18.964170843151,
      @t.right_ascension(@t_date).round(12)
    )
  end

  def test_declination
    assert_equal(
      -0.369294740577,
      @t.declination(@t_date).round(12)
    )
  end

  def test_sidereal_time
    assert_equal(
      6.88757643322,
      @t.sidereal_time(@t_date).round(12)
    )
  end

  def test_local_sidereal_time
    assert_equal(
      18.88757643322,
      @t.local_sidereal_time(@t_date, @t_lon).round(12)
    )
  end

  def test_daylight_time
    assert_equal(
      12.120499330542,
      @t.daylight_time(@t_date, @t_lat).round(12)
    )
  end

  def test_diurnal_arc
    assert_equal(
      6.060249665271,
      @t.diurnal_arc(@t_date, @t_lat).round(12)
    )
  end

  def test_t_south
    assert_equal(
      12.076594409931,
      @t.t_south(@t_date, @t_lon).round(12)
    )
  end

  def test_t_rise
    assert_equal(
      6.01634474466,
      @t.t_rise(@t_date, @t_lon, @t_lat).round(12)
    )
  end

  def test_t_mid_day
    assert_equal(
      12.076594409931,
      @t.t_mid_day(@t_date, @t_lat, @t_lon).round(12)
    )
  end

  def test_t_set
    assert_equal(
      18.136844075202,
      @t.t_set(@t_date, @t_lon, @t_lat).round(12)
    )
  end

  def test_rise_time
    rise = @t.t_rise(@t_date, @t_lon, @t_lat).round(12)
    assert_equal(
      "Sun rises \t\t\t : 6:0 UTC",
      "Sun rises \t\t\t : #{rise.floor}:#{(rise % 1 * 60.0).floor} UTC"
    )
  end

  def test_midday_time
    midday = @t.t_mid_day(@t_date, @t_lat, @t_lon).round(12)
    assert_equal(
      "Sun mid day \t\t\t : 12:4 UTC",
      "Sun mid day \t\t\t : #{midday.floor}:#{((midday % 1.0) * 60).floor} UTC"
    )
  end

  def test_set_time
    set = @t.t_set(@t_date, @t_lon, @t_lat).round(12)
    assert_equal(
      "Sun sets \t\t\t : 18:8 UTC",
      "Sun sets \t\t\t : #{set.floor}:#{(set % 1 * 60.0).floor} UTC"
    )
  end
end

#
class TestCalcSun300 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @t_date_str = '2000-01-01'
    @t_date = Date.parse('2000-01-01')
    @t_lat = 0.0
    @t_lon = 0.0
  end

  def test_jd1
    dt_obj = DateTime.new(2017, 1, 3)
    assert_equal(dt_obj.jd, @t.jd(dt_obj))
  end

  def test_jd2
    dt_obj = DateTime.new(2017, 1, 3, 4, 5, 6)
    assert_equal(dt_obj.jd, @t.jd(dt_obj))
  end

  def test_ajd1
    dt_obj = DateTime.new(2017, 1, 3)
    assert_equal(dt_obj.ajd, @t.ajd(dt_obj))
  end

  def test_ajd2
    dt_obj = DateTime.new(2017, 1, 3, 4, 5, 6)
    assert_equal(dt_obj.ajd, @t.ajd(dt_obj))
  end

  def test_jd2000_dif
    dt_obj = DateTime.new(2017, 1, 3, 4, 5, 6)
    dt_2000 = DateTime.new(2000, 1, 1, 12)
    assert_equal(dt_obj.jd - dt_2000.jd, @t.jd2000_dif(dt_obj))
  end

  def test_jd2000_dif_lon
    dt_obj = DateTime.new(2017, 1, 3, 4, 5, 6)
    dt_2000 = DateTime.new(2000, 1, 1, 12)
    assert_equal(dt_obj.jd - dt_2000.jd, @t.jd2000_dif_lon(dt_obj, @t_lon))
  end

  def test_equation_of_time
    assert_equal(-0.014428163704,
                 @t.equation_of_time(@t_date).round(12))
  end

  def test_eot_min
    assert_equal(-3.306691545536,
                 @t.eot_min(@t_date).round(12))
  end

  def test_sdate
    assert_equal(@t_date, @t.sdate(@t_date_str))
  end
end


