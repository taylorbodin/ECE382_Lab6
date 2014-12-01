ECE382_Lab6
===========

PWM Robot Motion

## Objectives and Purpose

To use the PWM features of the MSP430 to drive a pair of motors on a robot. 

## Functions 

### Sleep (new and improved)

```
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
```

In this new version of sleep I'm actually using the clock to count out more accurate ms. I performed the dimensional analysis
given the clock settings above to allow for a one ms rollover of TACCR0. Then I use a for loop that waits until TAIFG is set
decrement and reset the flag. After millis iterations TACCR0 is returned to normal and the timer a interupt is disabled. 
I'm pretty sure that I did the analysis correctly, but these settings produce delays much longer than 1 milli and I left it as
a todo in intrests of getting A functionality. 

### shutDown

```
void shutDown(){
	P2OUT &= ~(BIT0 | BIT5);
	P2OUT &= ~(BIT3 | BIT1);
	TA1CCTL1 = OUTMOD_5;
	TA1CCTL2 = OUTMOD_5;
	sleep(1);
}
```

Shutdown is a result of weird buggy behavior with trying to disable the motors. I noticed after I "did the dance" my left 
motor would stay on indefintely. I created this shutDown function to force the motor enables low and the outmod to stop. This
seemed to fix the bug and provided a nice little function to stop all motion.

### initRobot

```
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
```

initRobot initializes the MSP430 for use with the robot. First it sets up all the pins to work with the motors. 
The uses for each pin are documented with comments. Next the clock is set up and TA1CCR1 and 2 are set to the desired
PWM duty cycle. 

### moveForward, moveBackward, turnLeft, turnRight

```
void moveForward(int16 duration){
	P2OUT &= ~(BIT3 | BIT1); 		// Motor terminal low for forward operation
	P2OUT |= BIT0 | BIT5;

	TA1CCTL1 = OUTMOD_7;
	TA1CCTL2 = OUTMOD_7;

	sleep(duration);

	shutDown();

	return;
}
```

I chose moveForward as an exmplar for the movement functions. The first step in a movement function is to set the motor
terminals either high or low as appropriate. For move forward this is low. I empirically determined the others. Next I set
the motor enables high. Finally I set TA1CCTL1 and 2 to either OUTMOD_5 or 7 to get the correct duty cycle, either high or low,
as appropriate. The motors continue to run while sleep is running for duration milliseconds. Finally, shutdown runs to kill
any movement and we return. 


### Main

```
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    initMSP430();

    while (1) {
    	if (packetIndex > 40) {
    				_disable_interrupt();

    				while(packetData[i] != 2 && i < 80){
    					i++;
    				}

    				for(j = 0; j < 31; j++){
    					i++;
    					irPacket += packetData[i];
    					irPacket <<= 1;
    				}

    				if(irPacket == CH_UP){
    					initRobot();

    					moveForward(50);

    					newPacket = TRUE;
    				}

    				if(irPacket == CH_DW){
    					initRobot();

    					moveBackward(50);

    					newPacket = TRUE;
    				}

    				if(irPacket == VOL_UP){
    					initRobot();

    					turnRight(50);

    					newPacket = TRUE;
    				}

    				if(irPacket == VOL_DW){
    					initRobot();

    					turnLeft(50);

    					newPacket = TRUE;
    				}

    				if(irPacket == ONE){
    					newPacket = TRUE;
    				}

    				if(irPacket == TWO){
    					newPacket = TRUE;
    				}

    				if(irPacket == THR){
    					newPacket = TRUE;
    				}

    				if(irPacket == PWR){
    					newPacket = TRUE;
    					shutDown();
    				}

    				if(newPacket == TRUE){
    					initMSP430();
    					newPacket = FALSE;
    				}

    				i = 0;
    				packetIndex = 0;
    				_enable_interrupt();
    			} // end if new IR packet arrived
    		}
}
```

Main for Lab 6 is very similiar to main for the remote control lab. The first step is to set up the MSP430 to listen to the
IR decoder for IR packets from the remote. I already had that functionality from the remote lab so I included that function
under main. Next I wait until I have enough bit to be sure I've captured an entire packet with the while less than 40 loop. 
After I have enough bits, I disabled the interupt and look through the array holding the ir packet bits for a start bit denoted
by a two. Once that's found I start passing bits into IRpacket which holds the complete IR packet. I check that packet against
known values in my main logic. If for example it's an up ch + packet, which is up on the remote, I initialize the msp to 
communicate with the robot, move forward for 50 "millis" and set the newPacket flag to true. At the end of the main logic,
I clear the packetIndex, reenable interupts, and if there was a new packet, reinitialize the msp for ir detection.

## Debugging and Testing

My plan to debug this lab was simple. First I would get required functionality which meant that I was going to be able to
move my robot. Then I would check to make sure packets were being decoded correctly. Finally, I would attempt a functionality.
The main obstacle to the first goal was powering the motor driver. I was trying to power CC1, the logic one, with 3.3 V. It 
didn't work until I tried 5 V. I troubleshot with a headphone jack with leads attached to it so I could listen to the PWM signal from
MSP430 so I knew that that wasn't the problem. After that the motors worked fine and it was just trial and error to work out
what set ups produced forward and backward motion for each motor. 

## Conclusion
During this lab my robot produced motion that was slow but very controlled. While I it won't be breaking any speed records in
the maze, I won't hit any walls, and it might be faster because it's smooth. As the saying goes, slow is smooth, smooth is 
fast. During this lab I learned the importance of reading techincal documentation. I was getting frustrated with the motor 
driver so I took a break to read the documentation. That was where I got the idea to use 5 V for VCC1. 

## Documentation
NONE 
