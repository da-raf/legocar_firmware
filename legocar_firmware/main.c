#include <stdio.h>
#include <unistd.h>
#include <system.h>
#include <io.h>
#include <alt_types.h>

#define MOTOR_ENCODER

#ifdef ACCELEROMETER

#include "ins.h"

int main(void)
{

	INS *ins = init_ins(GSENSOR_SPI_BASE);

	// auto calibration:
	auto_calibrate_ins(ins, 1000);

	// manual calibration:
	// calibrate_ins(ins, 0.0, 0.46, -0.4);


	double acceleration[DIM];
	double speed[DIM];
	double distance[DIM];

	while(1) {
		usleep(1000 * 10);

		// update the INS with new values from the sensor
		update_ins(ins);
		// load the current state in our arrays
		get_latest_state(ins, acceleration, speed, distance);

		// printf("acceleration: X: %6.2f,\tY: %6.2f,\tZ: %6.2f\n", acceleration[0], acceleration[1], acceleration[2]);
		printf("speed: X: %6.2f,\tY: %6.2f,\tZ: %6.2f\n",        speed[0],        speed[1],        speed[2]);
		// printf("distance: X: %6.2f,\tY: %6.2f,\tZ: %6.2f\n",     distance[0],     distance[1],     distance[2]);
	}


	return 0;
}

// ACCELEROMETER
#endif

#ifdef MOTOR_ENCODER

#include "pwm_motor.h"


int main (void)
{

	unsigned long phase1=0x0,phase2=0x000,duty1=2000,duty2=40000,period=100000,enable=0x2;

	// front engine right/left
	PWM_Motor fr_speed;
	PWM_Motor fl_speed;
	// back  engine right/left
	PWM_Motor br_speed;
	PWM_Motor bl_speed;

	// initialize engines
	init_pwm_motor(&fr_speed, 0x80000000 | A_2_CHANNEL_PWM_0_BASE);
	init_pwm_motor(&fl_speed, 0x80000000 | A_2_CHANNEL_PWM_1_BASE);
	init_pwm_motor(&br_speed, 0x80000000 | A_2_CHANNEL_PWM_2_BASE);
	init_pwm_motor(&bl_speed, 0x80000000 | A_2_CHANNEL_PWM_3_BASE);

	printf ("Start\n");
	while (1)
	{
		usleep(1000 * 10);

		printf ("Duty = %lu\n", duty2);

		if(duty2 < 99000)
			duty2 += 500;

		motor_setting(&fr_speed, phase1, duty1, phase2, duty2, period, enable);
		motor_setting(&fl_speed, phase1, duty1, phase2, duty2, period, enable);
		motor_setting(&br_speed, phase1, duty1, phase2, duty2, period, enable);
		motor_setting(&bl_speed, phase1, duty1, phase2, duty2, period, enable);

	}
}

#endif

