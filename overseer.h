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

#endif
