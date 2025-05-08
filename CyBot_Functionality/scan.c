#include "Timer.h"
#include "lcd.h"
#include <stdint.h>
#include "movement.h"
#include "open_interface.h"
#include "uart-interrupt.h"
#include "scan.h"
#include <stdbool.h>
#include "servo.h"
#include "scanning.h"


#define AVERAGE_COUNT 4

int largest_Distance;
int largest_Width;


float run_autonomous_scan(oi_t *sensor_data)
{

    double ping_distance = 0.0;
    uint16_t ir_distance;



    // 1) Base case: if we've reached the recursion limit, stop

    // 2) Clear any leftover objects
    obj storedObj[10] = {0};

    // Variables for scanning
    double distanceReadings[AVERAGE_COUNT] = {0};
    int index = 0;
    int currentObj = 0;
    int objCount = 0;
    int firstDegree = 0, lastDegree = 0;
    double firstDistance = 0;

    // 3) Perform the scan loop, same as your 'm' command
    timer_waitMillis(500); // let servo settle
    int angle;
    for (angle = 0; angle <= 180; angle += 2) {

      //  cyBOT_Scan(angle, scan);
        timer_waitMillis(200);
        servoo_move(angle);
        ping_distance = get_pingDistance();
        ir_distance = adc_read();


        // Rolling average distance
        distanceReadings[index] = ping_distance;
        index = (index + 1) % AVERAGE_COUNT;

        double avgDistance = 0;
        int j;
        for (j = 0; j < AVERAGE_COUNT; j++) {
            avgDistance += distanceReadings[j];
        }
        avgDistance /= AVERAGE_COUNT;

        // Print data
        char data_line[64];
        sprintf(data_line, "%d\t%.2f\t%d\n",
                angle, ping_distance, ir_distance, avgDistance);
        uart_sendStr(data_line);

        // Simple IR-based detection: IR >= 900 & distance < 60
        if (ir_distance >= 900 && currentObj == 0 && avgDistance < 60.0) {
            objCount++;
            currentObj = 1;
            firstDegree = angle;
            firstDistance = avgDistance;
        }
        else if (ir_distance < 900 && currentObj == 1) {
            lastDegree = angle;
            currentObj = 0;

            // Record object
            int degObj = (firstDegree + lastDegree) / 2;
            int radialWidth = lastDegree - firstDegree;

            char msg[80];
           // sprintf(msg, "OBJECT #%d: angle=%d dist=%.2f width=%d\r\n",
            //        objCount, degObj, firstDistance, radialWidth);
            //uart_sendStr(msg);

            storedObj[objCount - 1].objNum  = objCount;
            storedObj[objCount - 1].width  = radialWidth;
            storedObj[objCount - 1].angle  = degObj;
            storedObj[objCount - 1].distance = firstDistance;

            double size = ((double) radialWidth / 360.0) * (2.0 * M_PI) * firstDistance;
            storedObj[objCount - 1].TRUEWIDTH = size;

            //sprintf(msg, "SIZE: %.2f\r\n", size);
            //uart_sendStr(msg);
        }
    } // end for

    // 4) If no objects, done
    if (objCount == 0) {
        //uart_sendStr("No objects detected.\r\n");
        uart_sendStr("END\n");
        return 0;
    }

    // 5) Pick smallest object
    obj largestObj = storedObj[0];
    double largestWidth = storedObj[0].TRUEWIDTH;

    int i;
    for(i = 1; i < objCount; i++){
        if(storedObj[i].TRUEWIDTH > largestWidth){
            largestWidth = storedObj[i].TRUEWIDTH;
            largestObj = storedObj[i];
        }
    }

   // cyBOT_Scan(largestObj.angle, scan); // point to the largest width
    servoo_move(largestObj.angle);
    largest_Distance = largestObj.distance;
    largest_Width = largestObj.width;
    lcd_printf("Width; %d",largestObj.width );
    uart_sendStr("END\n");

    return largestObj.angle;



    }



int get_largest_dist() {return largest_Distance;}
float get_largest_width() {return largest_Width;}
