#ifndef MAIN_H_
#define MAIN_H_

enum driver_state_t {
    MANUAL,
    AUTO
};

void flag_monitor(oi_t *sensor_data);

#endif /* UART_H_ */
