
require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

R2D = 180.0 / Math::PI
# doc
class TestMyTime < Test::Unit::TestCase # MiniTest::Test
  LON = -88.743
  def setup
    @t = CalcSun.new
    @time = Time.new(2017, 3, 17, 0, 0, 0, '+00:00').getgm
    # p @time.zone
    # p @time.gmt_offset
    # p l = @time.getlocal
    # p l.gmt_offset / 3600.0
    @ajd0 = @time.to_datetime.ajd.to_f
    @ajd = @ajd0 + @t.t_south(@ajd0 + 0.5, LON) / 24.0
  end

  def test_t_south_time
    assert_equal(
      '2017-03-17T18:03:17+00:00',
      DateTime.jd(@ajd + 0.5).to_s
    )
  end

  def test1_gmsa0
    assert_equal(
      174.761497853952,
      @t.gmsa0(@ajd0)
    )
  end

  def test2_gmsa0
    assert_equal(
      174.761497853952,
      @t.gmsa0(@ajd)
    )
  end

  def test1_gmst0
    assert_equal(
      11.650766733312,
      @t.gmst0(@ajd0)
    )
  end

  def test2_gmst0
    assert_equal(
      11.650766733312,
      @t.gmst0(@ajd)
    )
  end

  def test1_gmsa
    assert_equal(
      86.326166683648,
      @t.gmsa(@ajd)
    )
  end

  def test1_gmst
    assert_equal(
      5.75507791872,
      @t.gmst(@ajd)
    )
  end

=begin
  def test_t_south
    assert_equal(
      18.054878593024,
      @t.t_south(@ajd0, @lon)
    )
  end

  def test_t_south_ajd
    assert_equal(
      2_457_830.252286608,
      @ajd0
    )
  end

  def test_right_ascension
    assert_equal(
      23.868102148096,
      @t.right_ascension(@ajd0)
    )
  end

  def test_mean_sidereal_time
    assert_equal(
      11.871732105216,
      @t.mean_sidereal_time(@ajd0)
    )
  end

  def test_true_longitude
    assert_equal(
      357.84379831549927,
      @t.true_longitude(@ajd0) * R2D
    )
  end



  def test_gha
    assert_equal(
      177.89620630697482,
      @t.gha(@ajd0)
    )
  end
=end
end

# doc
class TestMyTime100 < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @date = '2017-03-17' # lucky day?
    @ajd0 = Time.new(2017, 3, 17, 0, 0, 0, '+00:00').getgm.to_datetime.ajd.to_f
    @jd0 = @ajd0.floor + 0.5
    @lat = 41.9475
    @lon = -88.743
    @ajd = @ajd0 + @t.t_south(@jd0, @lon) / 24.0
    @jd = @ajd + 0.5
  end
end
