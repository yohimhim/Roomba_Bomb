

#include "open_interface.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include <stdio.h>

#include "Timer.h"
#include "lcd.h"

float calculatelinearW(float distance, int angleWidth)
{

    return 2 * distance * tan(angleWidth * M_PI / 360.0);

}

void cyBOT_sendString(char *input) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        cyBot_sendByte(input[i]);
    }
}



float avgDistance(int i, cyBOT_Scan_t *scan_data) {
    float sum = 0;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;
   cyBOT_Scan(i, scan_data);
   sum += scan_data -> sound_dist;

   return sum / 6;
}



int getSmallestObject(cyBOT_Scan_t *scan_data)
{
    int currAngle     = 0;
    int objCounter    = 0;
    float angleWidth  = 0.0f;  // difference in servo degrees for the current object
    float smallestLW  = -1.0f; // smallest linear width found so far
    int   smallestAng = -1;    // angle (servo degrees) of the smallest object center

    float lastDist = avgDistance(0, scan_data);  // distance at angle = 0
    int objectDetected = 0;

    // We'll scan from 0..180 in increments of 2 degrees:
    while (currAngle < 180)
    {
        float currDist = avgDistance(currAngle, scan_data);

        if (objectDetected == 1)
        {
            // Condition for object END:
            if (currDist > lastDist + 10.0f)
            {
                objectDetected = 0;

                // If angleWidth is zero, that means it was a false start,
                // so decrement the object counter
                if (angleWidth == 0.0f)
                {
                    objCounter--;
                }
                else
                {
                    // We have an object with angleWidth degrees
                    // We'll approximate its distance as the "lastDist" at the end
                    // or you might want to store the MIN distance for better accuracy
                    float linearW = calculatelinearW(lastDist, angleWidth);

                    // The center angle is (current angle) minus half of the angleWidth
                    float centerAngle = (float)currAngle - (angleWidth / 2.0f);

                    // Print the object info
                    char *buffer;
                    // Num, center angle, distance (cm), linear width (cm)
                    // asprintf is a GNU extension. If not available, use sprintf into a fixed buffer.
                    asprintf(&buffer, "|%d \t |%.2f \t |%.2f \t    |%.2f\n\r",
                             objCounter,
                             centerAngle,
                             lastDist,
                             linearW);
                    cyBOT_sendString(buffer);

                    // Check if this is the smallest so far
                    if (smallestLW < 0.0f || linearW < smallestLW)
                    {
                        smallestLW  = linearW;
                        smallestAng = (int)(centerAngle + 0.5f); // round to nearest int if you want
                    }
                }
                // Reset angle width for the next object
                angleWidth = 0.0f;
            }
            else
            {
                // We're still in the same object, so add the incremental angle
                // that we've just moved (2 degrees each loop).
                // You had 2.5 previously. Adjust if needed.
                angleWidth += 2.0f;
            }
        }
        else
        {
            // Condition for object START:
            if (currDist < lastDist - 3.4f)
            {
                objectDetected = 1;
                objCounter++;
                // Start new object with angleWidth = 0
                angleWidth = 0.0f;
            }
        }

        // Move servo angle by 2 degrees each loop
        currAngle += 3;
        // Update lastDist for next iteration
        lastDist = currDist;
    }

    // Return the servo angle of the smallest object’s center
    return smallestAng;
}

int main(void)
{
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data);           // do this only once at start of main()
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0011);

    // If needed:
    // cyBOT_SERVO_cal();
    right_calibration_value = 117250;
    left_calibration_value  = 1345750;

    cyBOT_Scan_t *scan_data = calloc(1, sizeof(cyBOT_Scan_t));

    char *formatted_data = (char*) malloc(sizeof(char) * 100);
    int i;
    float sum;
    int angle;

    while (1) {
        char received = (char) cyBot_getByte();
        switch (received)
        {
            case 'w':
                oi_setWheels(200, 200);
                break;
            case 's':
                oi_setWheels(-50, -50);
                break;
            case 'a':
                oi_setWheels(50, -50);
                break;
            case 'd':
                oi_setWheels(-50, 50);
                break;
            case ' ':
                oi_setWheels(0, 0);
                break;

            case 'm':
                cyBOT_sendString("Degrees PING Distance (cm)\n\r");
                for (i = 45; i <= 135; i += 2)
                {
                    sum = 0.0f;
                    cyBOT_Scan(i, scan_data); sum += scan_data->sound_dist;
                    cyBOT_Scan(i, scan_data); sum += scan_data->sound_dist;
                    cyBOT_Scan(i, scan_data); sum += scan_data->sound_dist;
                    sprintf(formatted_data, "%d \t %f \n\r", i, (sum / 3.0f));
                    cyBOT_sendString(formatted_data);
                }
                break;

            case 'q':
                cyBOT_sendString("Num \t CenterAngle \t Dist: \t Width:\n\r");
                angle = getSmallestObject(scan_data);
                // After we find the smallest object, scan at that angle
                cyBOT_Scan(angle, scan_data);
                // Just send the angle as a single byte
                cyBot_sendByte((char)angle);
                break;
        }

        // Echo the received character (optional)
        cyBot_sendByte(received);
    }

    // oi_free(sensor_data); // do this once at end of main()
    return 0;
}
