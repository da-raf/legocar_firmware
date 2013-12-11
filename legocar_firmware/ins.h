/*
 * ins.h
 *
 *  Created on: 03.12.2013
 *      Author: raphael
 */

#ifndef INS_H_
#define INS_H_

#include <alt_types.h>

//! number of dimensions, the accelerometer can measure
#define DIM 3


typedef struct INS {
	alt_u32 sensor_spi_base_addr;
	alt_16  sensor_calibration[DIM];
	alt_16  acceleration[DIM];
	alt_16  speed[DIM];
	alt_16  distance[DIM];
} INS;


// create a new INS based on the sensor at spi base address 'sensor_spi_base_addr'
INS *init_ins(alt_u32 sensor_spi_base_addr);

// calibrate the ins manually
void calibrate_ins(INS *ins, double x, double y, double z);

// calculate the calibration of the INS using 'values' inputs from the sensor
int  auto_calibrate_ins(INS *ins, int values);

// wait until the next data item from the sensor is available
void wait_for_data(INS *ins);

// load the next data item from the sensor
int  update_ins(INS *ins);

// write the latest state (acceleration, speed, distance) to the arrays
int  get_latest_state(INS *ins, double acceleration[DIM], double speed[DIM], double distance[DIM]);

#endif /* INS_H_ */
