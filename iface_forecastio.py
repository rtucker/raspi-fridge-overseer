#!/usr/bin/env python

PICKLEFILE = '/dev/shm/iface_forecastio.p'

import forecastio
import pickle
import time

def read_config(filename=".secrets"):
    # Reads the configuration file, by default .secrets
    # This contains three lines:
    #   api_key
    #   latitude
    #   longitude
    # Returns these three values as a tuple...

    with open(filename, 'r') as fp:
        api_key = fp.readline().strip()
        lat = fp.readline().strip()
        lng = fp.readline().strip()

    return (api_key, lat, lng)

def parse_expires(value):
    # Parses an Expires header (Sun, 13 Jul 2014 16:34:00 +0000)
    # into a time.

    return time.strptime(value, "%a, %d %b %Y %H:%M:%S +0000")

def get_cache():
    try:
        forecast = pickle.load(open(PICKLEFILE, 'rb'))
        expires = forecast.http_headers.get('Expires', None)
    except IOError:
        return None

    if expires is not None:
        checktime = parse_expires(expires)
    else:
        return None

    if checktime > time.gmtime():
        # Still valid
        return forecast
    else:
        # Still valid
        return None

def set_cache(forecast):
    pickle.dump(forecast, open(PICKLEFILE, 'wb'))

def get_extreme_temperatures(data):
    t_min = 999
    t_max = -999
    for d in data:
        if d.temperature < t_min:
            t_min = d.temperature
        if d.temperature > t_max:
            t_max = d.temperature

    return (t_min, t_max)

def main():
    forecast = get_cache()

    if forecast is None:
        forecast = forecastio.load_forecast(*read_config(), units="si")
        set_cache(forecast)

    print("Cur: %s, %d degC" % (forecast.currently().summary, round(forecast.currently().temperature)))

    low, high = get_extreme_temperatures(forecast.hourly().data)

    print("Today: %s (High %d, Low %d)" % (forecast.hourly().summary, round(high), round(low)))

if __name__ == '__main__': main()

