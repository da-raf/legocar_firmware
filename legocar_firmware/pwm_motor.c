/*
 * pwm_motor.c
 *
 *  Created on: 10.12.2013
 *      Author: raphael
 */

#include "pwm_motor.h"


void init_pwm_motor(PWM_Motor *pwm_motor, alt_u32 pwm_base_address) {

	pwm_motor->enable = (volatile unsigned int *) pwm_base_address;
	pwm_motor->period = (volatile unsigned int *) pwm_base_address + 1;
	pwm_motor->duty1  = (volatile unsigned int *) pwm_base_address + 2;
	pwm_motor->duty2  = (volatile unsigned int *) pwm_base_address + 3;
	pwm_motor->phase1 = (volatile unsigned int *) pwm_base_address + 4;
	pwm_motor->phase2 = (volatile unsigned int *) pwm_base_address + 5;
}

void motor_setting(const PWM_Motor *pwm_motor, unsigned long phase1, unsigned long duty1,
											   unsigned long phase2, unsigned long duty2,
											   unsigned long period, unsigned long enable) {

	*pwm_motor->enable = enable;
	*pwm_motor->period = period;
	*pwm_motor->phase1 = phase1;
	*pwm_motor->phase2 = phase2;
	*pwm_motor->duty1  = duty1;
	*pwm_motor->duty2  = duty2;
}
