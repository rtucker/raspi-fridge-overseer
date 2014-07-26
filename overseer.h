#ifndef __INCLUDE_OVERSEER_H_
#define __INCLUDE_OVERSEER_H_

#include <dirent.h>

#ifndef TRUE
#  define       TRUE    (1==1)
#  define       FALSE   (1==2)
#endif

#define SENSOR_ARRAY_LENGTH 16
#define DISPLAY_WIDTH 16

struct sensor_s
{
    char  filename[NAME_MAX+1];
    float reading;
};

typedef struct sensor_s sensor_t;

#define MAX_FORECAST_WIDTH 512
#define FORECAST_NOW "/dev/shm/iface_forecastio.now.txt"
#define FORECAST_LATER "/dev/shm/iface_forecastio.later.txt"

#define MAX_OUTBUF 1024

#define MUNIN_FILE "/dev/shm/overseer_reading"

#endif // __INCLUDE_OVERSEER_H_
