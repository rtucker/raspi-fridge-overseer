#ifndef __INCLUDE_OVERSEER_H_
#define __INCLUDE_OVERSEER_H_

#ifndef TRUE
#  define       TRUE    (1==1)
#  define       FALSE   (1==2)
#endif

#define SENSOR_ARRAY_LENGTH 16

struct sensor_s
{
    char *filename;
    float reading;
};

typedef struct sensor_s sensor_t;

#endif
