#!/usr/bin/env python

PICKLEFILE = '/dev/shm/iface_forecastio.p'
NOWFILE = '/dev/shm/iface_forecastio.now.txt'
LATERFILE = '/dev/shm/iface_forecastio.later.txt'

import forecastio
import os
import pickle
import sys
import time

def read_config(filename=".secrets"):
    # Reads the configuration file, by default .secrets
    # This contains three lines:
    #   api_key
    #   latitude
    #   longitude
    # Returns these three values as a tuple...

    if os.path.dirname(filename) == '':
        base = os.path.dirname(sys.argv[0])
        filename = os.path.join(base, filename)

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

def main(test=False):
    forecast = get_cache()

    if forecast is None:
        forecast = forecastio.load_forecast(*read_config(), units="si")
        if test:
            print("*** Cache miss")
        else:
            set_cache(forecast)

    low, high = get_extreme_temperatures(forecast.hourly().data)

    nowtxt = "Now: %d, dwpt %d. %s" % (
                    round(forecast.currently().temperature),
                    round(forecast.currently().dewPoint),
                    forecast.minutely().summary
                )
    if len(forecast.alerts()) > 0:
        latertxt = "***ALERT*** %s ***ALERT***" % forecast.alerts().title
    else:
        latertxt = "Later: %s High %d, Low %d." % (
                        forecast.hourly().summary,
                        round(high),
                        round(low)
                    )

    if test:
        print("Expires: %s" % forecast.http_headers.get('Expires', None))
        print(nowtxt)
        print(latertxt)

        for a in forecast.alerts():
            print("*****************")
            print("Alert title      : %s" % (a.title))
            print("      expires    : %s" % (a.expires))
            print("      description: %s" % (a.description))
            print("      uri        : %s" % (a.uri))

    else:
        with open(NOWFILE, 'w') as fp:
            fp.write(nowtxt)
        with open(LATERFILE, 'w') as fp:
            fp.write(latertxt)

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        main(test=True)
    else:
        main()

