/*
 * robot.h
 * Author: Taylor Bodin
 * Date: 18 Nov 2014
 * Description: Prototypes robot control functions
 */
#include <msp430g2553.h>

typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define PWM_FREQ 0x07D0
#define PWM_DUTY 0x03E8

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6


#define		averageLogic0Pulse	0x0212
#define		averageLogic1Pulse	0x0666
#define		averageStartPulse	0x1140
#define		minLogic0Pulse		averageLogic0Pulse - 0x100
#define		maxLogic0Pulse		averageLogic0Pulse + 0x100
#define		minLogic1Pulse		averageLogic1Pulse - 0x100
#define		maxLogic1Pulse		averageLogic1Pulse + 0x100
#define		minStartPulse		averageStartPulse - 0x100
#define		maxStartPulse		averageStartPulse + 0x100

#define		PWR		0x61A0F00E
#define		ONE		0x61A000FE
#define		TWO		0x61A0807E
#define		THR		0x61A040BE

#define		VOL_UP	0x61A030CE
#define		VOL_DW	0x61A0B04E
#define		CH_UP	0x61A050AE
#define		CH_DW	0x61A0D02E

#define		TRUE				1
#define		FALSE				0

void sleep(int16 millis);

void shutDown();

void initRobot();

void moveForward(int16 duration);

void moveBackward(int16 duration);

void turnLeft(int16 duration);

void turnRight(int16 duration);
