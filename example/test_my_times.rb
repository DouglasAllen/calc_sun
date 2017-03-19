
require 'rubygems'
# gem 'minitest'
# require 'minitest/autorun'

require 'test/unit'
lib = File.expand_path('../../../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'calc_sun'

# doc
class TestMyTime < Test::Unit::TestCase # MiniTest::Test
  def setup
    @t = CalcSun.new
    @date = '2017-03-17'
    @ajd = DateTime.new(2017, 3, 17, 12, 0, 0).jd.to_f
    @lat = 41.9475
    @lon = -88.743
  end

  def test_t_south
    assert_equal(
      18.054878593024,
      @t.t_south(@ajd, @lon) / 24.0
    )
  end

  def test_t_south_ajd
    assert_equal(
      2_457_830.2522278614,
      @ajd + @t.t_south(@ajd, @lon) / 24.0 - 0.5
    )
  end

  def test_t_south_time
    assert_equal(
      52286608,
      DateTime.jd(@ajd + @t.t_south(@ajd, @lon) / 24.0 - 0.5).to_s
    )
  end
end
