/*-------------------------------------------------------------------------------
	Taylor Bodin
	Fall 2014
	MSP430G2553
	This program implements a control system for a small, circular robot driven with
	PWM signals to two DC motors.
-------------------------------------------------------------------------------*/

#include <msp430.h> 
#include "robot.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // Initialize output pins
    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1 and Right Motor
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2 and Left Motor
    P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2

    P2DIR |= BIT1;							// P2.1 and P2.3 are associated with the motor terminals
    P2OUT &= ~BIT1;

    P2DIR |= BIT3;
    P2OUT &= ~BIT3;

    P2DIR |= BIT0;							// P2.0 and P2.5 are associated with the motor enable and disable
    P2OUT &= ~BIT0;

    P2DIR |= BIT5;
    P2OUT &= ~BIT5;

    P1DIR &= ~BIT3;							// P1.3 is a button that will initiate THE DANCE
    P1REN |= BIT3;

    P1DIR |= BIT6 | BIT0;
    P1OUT |= BIT0;
    P1OUT &= ~BIT6;

    // Initialize the clock
	TA1CTL = ID_2 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK
    TA1CCR0 = PWM_FREQ;						// set signal period to produce 100hz signal

    TA1CCR1 = PWM_DUTY_50;						// 50% duty cycle
    TA1CCTL1 = OUTMOD_5;					// set TACCTL1 to Reset mode (Grounded/Stopped)

    TA1CCR2 = PWM_DUTY_50;
    TA1CCTL2 = OUTMOD_5;

    // Do the Dance!
    while (1) {

    	while((P1IN & BIT3) != 0);			// Wait for a button press
    	while((P1IN & BIT3) == 0);			// and release

    	P1OUT |= BIT6;

    	moveForward(500);

    	sleep(10);

    	moveBackward(500);

    	sleep(10);

    	turnLeft(250);

    	sleep(10);

    	turnRight(250);

    	sleep(10);

    	turnLeft(500);

    	sleep(10);

    	turnRight(500);

    	shutDown();

    	P1OUT &= ~BIT6;
    }
}
