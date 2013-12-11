/*
 * INS.cpp
 *
 *  Created on: 03.12.2013
 *      Author: raphael
 */

#include "ins.h"

#include <unistd.h>

#include "terasic_lib/terasic_includes.h"
#include "terasic_lib/accelerometer_adxl345_spi.h"

// acceleration in m/s² per one discrete step in the output values of the sensor
const double ms2_per_digi = 0.04;

//! number of steps to wait until we try to get the next XYZ-data from the g-sensor
const int data_wait  = 1000;
//! number of steps to wait until retry if data was not available immediately
const int data_retry = 100;


INS *init_ins(alt_u32 sensor_spi_base_addr) {
	INS *ins = (INS *) malloc( sizeof(INS) );

	ins->sensor_spi_base_addr = sensor_spi_base_addr;

	ADXL345_SPI_Init( ins->sensor_spi_base_addr );

	// initialize the arrays
	int j;
	for(j=0; j<DIM; j++) {
		ins->sensor_calibration[j] = 0;
		ins->acceleration[j]       = 0;
		ins->speed[j]              = 0;
		ins->distance[j]           = 0;
	}

	return ins;
}

void calibrate_ins(INS *ins, double x, double y, double z) {
	ins->sensor_calibration[0] = x / ms2_per_digi;
	ins->sensor_calibration[1] = y / ms2_per_digi;
	ins->sensor_calibration[2] = z / ms2_per_digi;
}

int auto_calibrate_ins(INS *ins, int values) {
	// this value will be returned
	// set to 0 when we cannot read from the sensor
	int success = 1;

	// initialize the array that is used below for calculating the average output
	// of the acceleration sensor
	int sum[DIM];
	int j;
	for(j=0; j<DIM; j++)
		sum[j] = 0;

	// read #values values from the sensor
	int i;
	for(i=0; i<values; i++) {
		// wait for next value from sensor
		wait_for_data(ins);

		// read acceleration data from the sensor
		success = ADXL345_SPI_XYZ_Read( ins->sensor_spi_base_addr, (alt_u16 *) ins->acceleration );

		if(!success) {
			printf("ERROR: cannot read from sensor!\n");
			return success;
		}

		// update sums with the latest output
		for(j=0; j<DIM; j++)
			sum[j] += ins->acceleration[j];

		// make a small pause until we try to get the next data from the sensor
		usleep(data_wait);

	}

	// calculate the average values for all the dimensions
	for(j=0; j<DIM; j++)
		ins->sensor_calibration[j] = sum[j] / values;

	printf( "calibration successful: %6.2f, %6.2f, %6.2f", ins->sensor_calibration[0] * ms2_per_digi,
												  	  	   ins->sensor_calibration[1] * ms2_per_digi,
												  	  	   ins->sensor_calibration[2] * ms2_per_digi );

	// reset the speed to 0 as the system has to be standing still now
	for(j=0; j<DIM; j++)
		ins->speed[j] = 0;

	return success;
}

void wait_for_data(INS *ins) {
	while(!ADXL345_SPI_IsDataReady( ins->sensor_spi_base_addr ))
		usleep(data_retry);

	return;
}

int update_ins(INS *ins) {

	// check the sensor calibration for Z-axis: if this is 0, the ins has probably not been calibrated
	// => g-force
	if(ins->sensor_calibration[2] == 0)
		printf("WARNING: g-sensor has probably not been calibrated!\n");

	wait_for_data(ins);

	bool success;
	alt_16 *acc = ins->acceleration;

	success = ADXL345_SPI_XYZ_Read(ins->sensor_spi_base_addr, (alt_u16 *) acc);
	if(!success) {
		printf("ERROR: reading from sensor failed!\n");
		return success;
	}

	// update speed using the acceleration and the calibration
	int j;
	for(j=0; j<DIM; j++)
		ins->speed[j] += ins->acceleration[j] - ins->sensor_calibration[j];

	// update distance using the speed
	for(j=0; j<DIM; j++)
		ins->distance[j] += ins->speed[j];

	return success;
}

int get_latest_state(INS *ins, double acceleration[DIM], double speed[DIM], double distance[DIM]) {

	// do we need to multiply mg_per_digi^2 to speed and mg_per_digi^3 to distance?
	// const int mg_per_digi2 = mg_per_digi * mg_per_digi;
	// const int mg_per_digi3 = mg_per_digi2 * mg_per_digi;

	int j;
	for(j=0; j<DIM; j++) {
		acceleration[j] = (ins->acceleration[j] - ins->sensor_calibration[j]) * ms2_per_digi;
		speed[j]        = (double) ins->speed[j];
		distance[j]     = (double) ins->distance[j];
	}

	return 1;
}
