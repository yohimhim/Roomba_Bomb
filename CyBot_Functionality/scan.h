#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "scanning.h"
#include <stdbool.h>

#ifndef SCAN_H_
#define SCAN_H_
typedef struct obj {
    int objNum;
    int width;
    int angle;
    double distance;
    double TRUEWIDTH;
} obj;

 struct Objects{
    int linear_distance;
    int deg_count;
    float linear_width;
    float distance;
    float width;
    float angle;
};

int get_largest_dist();
float get_largest_width();
float run_autonomous_scan(oi_t *sensor_data);



#endif
