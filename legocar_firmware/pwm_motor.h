/*
 * pwm_motor.h
 *
 *  Created on: 10.12.2013
 *      Author: raphael
 */

#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

#include <alt_types.h>

typedef struct PWM_Motor {
	// bitwise encoded direction: 0x2 => forwards, 0x1 => backwards
	volatile unsigned int *enable;
	volatile unsigned int *period;
	volatile unsigned int *phase1;
	volatile unsigned int *phase2;
	volatile unsigned int *duty1;
	volatile unsigned int *duty2;
} PWM_Motor;

// initialize the given structure pwm_motor
void init_pwm_motor(PWM_Motor *pwm_motor, alt_u32 pwm_base_address);
void motor_setting(const PWM_Motor *pwm_motor, unsigned long phase1, unsigned long duty1,
		unsigned long phase2, unsigned long duty2, unsigned long period, unsigned long enable);

#endif /* PWM_MOTOR_H_ */
