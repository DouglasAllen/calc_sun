
require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

R2D = 180.0 / Math::PI
# doc
class TestMyTime < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @date = '2017-03-17' # lucky day?
    @jd0 = Time.new(2017, 3, 17, 12, 0, 0).to_datetime.jd.to_f
    @ajd0 = @jd0 - 0.5
    @lat = 41.9475
    @lon = -88.743
    @ajd = @ajd0 + @t.t_south(@jd0, @lon) / 24.0
    @jd = @ajd + 0.5
  end

  def test_t_south
    assert_equal(
      18.054878593024,
      @t.t_south(@jd0, @lon)
    )
  end

  def test_t_south_ajd
    assert_equal(
      2_457_830.252286608,
      @ajd
    )
  end

  def test_t_south_time
    assert_equal(
      '2017-03-17T18:03:17+00:00',
      DateTime.jd(@jd).to_s
    )
  end

  def test_right_ascension
    assert_equal(
      23.868102148096,
      @t.right_ascension(@jd)
    )
  end

  def test_local_sidereal_time
    assert_equal(
      11.871732105216,
      @t.local_sidereal_time(@jd, @lon)
    )
  end

  def test_true_longitude
    assert_equal(
      357.84379831549927,
      @t.true_longitude(@jd) * R2D
    )
  end

  def test_gmst
    assert_equal(
      9.109988442112,
      @t.gmst(@jd)
    )
  end
end
