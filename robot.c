#include "robot.h"

//TODO: This code does not give a millis delay. I'm not understanding something with the clock. It's much too long.
void sleep(int16 millis){

	TACTL |= ID_0 | TASSEL_2 | MC_0;		//Stop the clock while we set everything up
	TAR = 0;
	TACTL &= ~TAIFG;
	TACCR0 = 0x1F40; 	 //1 ms rollover
	TACTL |= TAIE;
	TACTL |= MC_1;
	int16 i;

	for(i = millis; i > 0; i--){
		while((TACTL & TAIFG) == 0);
		TACTL &= ~TAIFG;
		TAR = 0;
	}

	TACCR0 = PWM_FREQ; // Get everything back to normal for PWM
	TACTL &= ~ TAIE;
	return;
}

void shutDown(){
	P2OUT &= ~(BIT0 | BIT5);
	P2OUT &= ~(BIT3 | BIT1);
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;
	sleep(1);
}

void initRobot(){
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

	    P1DIR |= BIT6 | BIT0;
	    P1OUT |= BIT0;
	    P1OUT &= ~BIT6;

	    // Initialize the clock
		TA1CTL = ID_2 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK
	    TA1CCR0 = PWM_FREQ;						// set signal period to produce 100hz signal

	    TA1CCR1 = PWM_DUTY;
	    TA1CCTL1 = OUTMOD_5;					// set TACCTL1 to Reset mode (Grounded/Stopped)

	    TA1CCR2 = PWM_DUTY;
	    TA1CCTL2 = OUTMOD_5;
}

void moveForward(int16 duration){
	P2OUT &= ~(BIT3 | BIT1); 		// Motor terminal low for forward operation
	P2OUT |= BIT0 | BIT5;

	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_7;

	sleep(duration);

	shutDown();

	return;
}

void moveBackward(int16 duration){
	P2OUT |= BIT3 | BIT1; 		// Motor terminal high for backward operation
	P2OUT |= BIT0 | BIT5;

	TA1CCTL1 = OUTMOD_3;
	TA1CCTL2 = OUTMOD_3;

	sleep(duration);

	shutDown();

	return;
}

void turnRight(int16 duration){
	P2OUT |= BIT1;				//Right high -> Forward
	P2OUT &= ~ BIT3; 			//Left low -> Backward
	P2OUT |= BIT0 | BIT5;

	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_3;

	sleep(duration);

	shutDown();

	return;
}

void turnLeft(int16 duration){
	P2OUT |= BIT3;				//Left high -> Forward
	P2OUT &= ~ BIT1; 			//Right low -> Backward
	P2OUT |= BIT0 | BIT5;

	TA1CCTL1 = OUTMOD_3;
	TA1CCTL2 = OUTMOD_7;

	sleep(duration);

	shutDown();

	return;
}
