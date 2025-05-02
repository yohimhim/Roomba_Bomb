#include "Timer.h"
#include "lcd.h"
#include <stdint.h>
#include "movement.h"
#include "open_interface.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "scan.h"
#include "adc.h"
#include <stdbool.h>
#define AVERAGE_COUNT 4

int ir_vals[] = {3758, 3308, 2305, 1852, 1621, 1463, 1312, 1207, 1125, 1069, 1044, 1016, 931, 925, 900, 832, 822, 799, 775, 757};
int dist_cm[] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
int current_ir_vals[16];
float largest_Distance;
float largest_Width;


int ir_scan(){
    char data[50]; //for the data
    char buffer[10]; //for lcd data print

    int x = 0;
    int i = 0;
    double avg_ir;
    int distance_cm;

    while (x < 16){
        uint32_t IR_val;
        //ADC scan data using built adc
        timer_waitMillis(1000); //helps improve ADC values due to decrease in noise
        IR_val = adc_conversion();
        current_ir_vals[x] = IR_val;

        if (cancel_scan_flag == 1) {
            lcd_clear();
            lcd_puts("CANCELLED SCAN.");
            return 0;
        }

        sprintf(data, "%d\r\n", IR_val);   //print out data
        char* dataPosition = data;
        while(*dataPosition !='\0'){



            cyBot_sendByte(*dataPosition++);
        }

        x++;

    }

    for (; i < 19; i++){
         avg_ir += current_ir_vals[i];
    }
    avg_ir = avg_ir / 16;
    distance_cm = obj_distance(avg_ir);



    return distance_cm;
}

int obj_distance(double avg_ir) {
    int k = 0;
    for (; k < 18; k++){
        if (avg_ir > ir_vals[k]){
            return dist_cm[k] - 2; //if the avg ir value is less than
            //the standard plot then we have found a match
            //and we can use the matching index of the dist_cm to get the distance in cm
            //and subtract two to adjust for it being a bit shorter than the plot point
        }
    }
    return 105;
}

//float ping_scan(oi_t *sensor_data) {
//
//
//    cyBOT_Scan_t scan;
//
//    cyBOT_Scan(0, &scan);
//
//
//
//    //Declaring object stuff
//    struct Objects object[5];
//
//    int objectCount = 0;
//
//    int i,j;
//
//
//    int batchSize =2;
//
//
//    char header[] ="Angle\tPING Distance\tIR value\r\n";
//    int headerLength = 30;
//
//    char data[50]; //for the data
//    char data2[50]; //for the average
//    float avg[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
//    float angle[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
//    int index = 0;
//    uint32_t IR_val;
//
//
//    int isObject;
//    int largestIndex = 0;
//    float total = 0.0;
//    float batchAvg = 0.0;
//
//
//
//    while(1){
//        objectCount = 0;
//        isObject = 0;
//
//
//            for (i=0; i<headerLength;i++){
//                cyBot_sendByte(header[i]); //Prints the header out 1 character at a time.
//            }
//
//
//            for(j=0; j <= 180; j+=2){
//
//                //ADC scan data using built adc
//               IR_val = adc_conversion();
//
//                cyBOT_Scan(j, &scan);
//                float scanDistance;
//                scanDistance = scan.sound_dist;
//                sprintf(data, "%d\t%.2f\t\t%d\r\n", j, scanDistance, IR_val);   //print out data
//
//
//                if (cancel_scan_flag == 1) {
//                    lcd_clear();
//                    lcd_puts("CANCELLED SCAN.");
//                    return -1;
//                }
//
//                char* dataPosition = data;
//                while(*dataPosition !='\0'){
//                    cyBot_sendByte(*dataPosition++);
//                }
//
//
//                if (index == batchSize-1){
//                     avg[index] = scanDistance;
//                     angle[index] = j;
//                     index = 0;
//                     total = 0.0;
//                     batchAvg = 0.0;
//                     int k;
//                     for (k = 0; k <= 4; k++){
//                         total += avg[k];
//                     }
//                     batchAvg = total / batchSize;
//
//                     int count = 0;
//                     if (batchAvg <= 50){
//                         if (isObject == 0){ //new object
//                             count += 1;
//                             object[objectCount].linear_distance += obj_distance(IR_val);
//                             object[objectCount].angle = angle[batchSize/2];
//                             object[objectCount].distance = avg[batchSize/2];
//                             object[objectCount].width = batchSize*2;
//                             objectCount++;
//                             isObject = 1;
//                             sprintf(data2, "\nNew Object\r\n");  //String for data2
//                              char* arrayPosition = data2;
//                              while(*arrayPosition !='\0'){                   //printing out string
//                                  cyBot_sendByte(*arrayPosition++);
//                              }
//
//
//                         }
//                         else{
//                             object[objectCount-1].linear_distance += obj_distance(IR_val);
//                             object[objectCount-1].width += batchSize*2;
//                             object[objectCount-1].angle =  (angle[batchSize/2] + object[objectCount-1].angle)/2 ;
//                             object[objectCount-1].distance = (avg[batchSize/2] + object[objectCount-1].distance)/2;
//                             sprintf(data2, "\nSame Object Angle: %f\r\n", object[objectCount-1].angle);  //String for data2
//                             char* arrayPosition = data2;
//                             while(*arrayPosition !='\0'){                   //printing out string
//                             cyBot_sendByte(*arrayPosition++);
//                             object[objectCount-1].deg_count = count;
//                          }
//
//                         }
//                         }
//                         else{
//                             isObject = 0;
//                             count = 0;
//                         }
//                     count += 1;
//
//             sprintf(data2, "\nAvg: %f width %f\r\n------------------------------------------\r\n", batchAvg, object[objectCount-1].angle);  //String for data2
//             char* arrayPosition = data2;
//             while(*arrayPosition !='\0'){                   //printing out string
//                 cyBot_sendByte(*arrayPosition++);
//             }
//
//         }
//         else {
//             avg[index] = scanDistance;
//             angle[index] = j;
//             index+=1;
//         }
//
//
//
//
//
//
//    }
//            int k;
//            for (k = 0; k < objectCount; k++){
//               object[k].linear_distance = object[k].linear_distance / object[k].deg_count;
//               float r = object[k].linear_distance;
//               object[k].linear_width = (2*3.14159) * (r) * ((2*object[k].deg_count) / 360);
//            }
//
//
//    int largestWidth = object[0].linear_width;
//     int t;
//     for (t = 1; t < objectCount; t++) {
//         if (object[t].linear_width > 0 && object[t].linear_width > largestWidth) {
//             largestWidth = object[t].linear_width;
//             largestIndex = t;
//         }
//     }
//     largest_Distance = object[largestIndex].distance;
//     largest_Width = object[largestIndex].width;
//     cyBOT_Scan(object[largestIndex].angle, &scan);
//     break;
//     }
//     return object[largestIndex].angle;
//
//
//}

float run_autonomous_scan(oi_t *sensor_data, cyBOT_Scan_t *scan)
{
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
    for (angle = 5; angle <= 180; angle += 2) {

        cyBOT_Scan(angle, scan);

        // Rolling average distance
        distanceReadings[index] = scan->sound_dist;
        index = (index + 1) % AVERAGE_COUNT;

        double avgDistance = 0;
        int j;
        for (j = 0; j < AVERAGE_COUNT; j++) {
            avgDistance += distanceReadings[j];
        }
        avgDistance /= AVERAGE_COUNT;

        // Print data
        char data_line[64];
        sprintf(data_line, "Angle: %d PING: %.2f IR: %d AvgDist: %.2f\r\n",
                angle, scan->sound_dist, scan->IR_raw_val, avgDistance);
        uart_sendStr(data_line);

        // Simple IR-based detection: IR >= 900 & distance < 60
        if (scan->IR_raw_val >= 900 && currentObj == 0 && avgDistance < 60.0) {
            objCount++;
            currentObj = 1;
            firstDegree = angle;
            firstDistance = avgDistance;
        }
        else if (scan->IR_raw_val < 900 && currentObj == 1) {
            lastDegree = angle;
            currentObj = 0;

            // Record object
            int degObj = (firstDegree + lastDegree) / 2;
            int radialWidth = lastDegree - firstDegree;

            char msg[80];
            sprintf(msg, "OBJECT #%d: angle=%d dist=%.2f width=%d\r\n",
                    objCount, degObj, firstDistance, radialWidth);
            uart_sendStr(msg);

            storedObj[objCount - 1].objNum  = objCount;
            storedObj[objCount - 1].width  = radialWidth;
            storedObj[objCount - 1].angle  = degObj;
            storedObj[objCount - 1].distance = firstDistance;

            double size = ((double) radialWidth / 360.0) * (2.0 * M_PI) * firstDistance;
            storedObj[objCount - 1].TRUEWIDTH = size;

            sprintf(msg, "SIZE: %.2f\r\n", size);
            uart_sendStr(msg);
        }
    } // end for

    // 4) If no objects, done
    if (objCount == 0) {
        uart_sendStr("No objects detected.\r\n");
        return;
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

    cyBOT_Scan(largestObj.angle, scan); // point to the largest width
    largest_Distance = largestObj.distance;
    largest_Width = largestObj.width;
lcd_printf("Width; %d",largestObj.width );
    return largestObj.angle;



    }



float get_largest_dist() {return largest_Distance;}
float get_largest_width() {return largest_Width;}
