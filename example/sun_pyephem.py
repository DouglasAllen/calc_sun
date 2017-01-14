

import ephem as e
import datetime as dt
from math import radians as rad

date = dt.datetime.utcnow()
d = e.Date(str(date))

G = e.Observer()
G.lat = rad(51.4770228)  # lat/long in decimal degrees
G.long = rad(-0.0001147)

print "UTC: ", d

print "sunrise: ", G.previous_rising(e.Sun())
print "transit: ", G.previous_transit(e.Sun())
print "sunset: ", G.previous_setting(e.Sun())
