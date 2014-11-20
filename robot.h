/*
 * robot.h
 * Author: Taylor Bodin
 * Date: 18 Nov 2014
 * Description: Prototypes robot control functions
 */
#include <msp430.h>

#define PWM_FREQ 0x07D0
#define PWM_DUTY_50 0x0500

typedef	unsigned short int16;

void sleep(int16 millis);

void shutDown();

void moveForward(int16 duration);

void moveBackward(int16 duration);

void turnLeft(int16 duration);

void turnRight(int16 duration);
