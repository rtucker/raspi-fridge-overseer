#!/usr/bin/env python

PICKLEFILE = '/dev/shm/iface_forecastio.p'
NOWFILE = '/dev/shm/iface_forecastio.now.txt'
LATERFILE = '/dev/shm/iface_forecastio.later.txt'

import codecs
from datetime import datetime, timedelta
import forecastio
import locale
import os
import pickle
import random
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

def get_cache(allow_old=False):
    try:
        forecast = pickle.load(open(PICKLEFILE, 'rb'))
        expires = forecast.http_headers.get('Expires', None)
    except IOError, RuntimeError:
        return None

    if expires is not None:
        checktime = parse_expires(expires)
    else:
        return None

    if allow_old or checktime > time.gmtime():
        # Still valid
        return forecast
    else:
        # Expired
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
    forecast = get_cache(allow_old=True if test else False)

    if forecast is None:
        if not test:
            # Random sleep to avoid stampeding
            time.sleep(random.random()*30)

        forecast = forecastio.load_forecast(*read_config(), units="si")

        if not test:
            set_cache(forecast)

    low, high = get_extreme_temperatures(forecast.hourly().data)

    # \xdf = degree symbol
    nowtxt = u"%s %d\xdf" % (
                    forecast.minutely().summary,
                    round(forecast.currently().temperature),
                )

    if round(forecast.currently().temperature) != round(forecast.currently().apparentTemperature):
        nowtxt += u", feels like %d\xdf" % round(forecast.currently().apparentTemperature)

    latertxt = u""

    if len(forecast.alerts()) > 0:
        latertxt += u"***ALERT*** %s ... " % '; '.join([f.title for f in forecast.alerts()])

    latertxt += u"Next 24h: %s High %d\xdf Low %d\xdf" % (
                    forecast.hourly().summary,
                    round(high),
                    round(low),
                )

    precip_types = []
    integ_precip = 0
    for d in forecast.hourly().data:
        if d.time > datetime.now() + timedelta(days=1):
            continue
        integ_precip += d.precipProbability * d.precipIntensity
        try:
            if d.precipType not in precip_types:
                precip_types.append(d.precipType)
        except forecastio.utils.PropertyUnavailable:
            pass

    if integ_precip > 0.1:
        latertxt += u" H2O(l): %.2g mm as %s." % (integ_precip, ', '.join(precip_types))

    if test:
        print(u"Retrieved: %s" % forecast.http_headers.get('Date', None))
        print(u"Expires:   %s" % forecast.http_headers.get('Expires', None))
        print(nowtxt)
        print(latertxt)

        for a in forecast.alerts():
            print(u"*****************")
            print(u"Alert title      : %s" % (a.title))
            print(u"      expires    : %s" % (a.expires))
            print(u"      description: %s" % (a.description))
            print(u"      uri        : %s" % (a.uri))

    else:
        # emdashes?!
        nowtxt = unicode(nowtxt).replace(u'\u2013', '-')
        latertxt = unicode(latertxt).replace(u'\u2013', '-')
        # force latin_1 instead of ascii, so we can directly use chars
        # above 128 without unicode crap
        with codecs.open(NOWFILE, 'wb', 'latin_1', 'replace') as fp:
            fp.write(unicode(nowtxt))
        with codecs.open(LATERFILE, 'wb', 'latin_1', 'replace') as fp:
            fp.write(unicode(latertxt))

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == 'test':
        main(test=True)
    else:
        main()

