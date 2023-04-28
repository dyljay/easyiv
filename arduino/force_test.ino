#include <AccelStepper.h>

// don't change this! it's an initialization setting. 1 means that the arduino is interfacing with a driver (the black box)
#define motorInterfaceType 1


// connect the DIR+ pin on the driver to the pin on the arduino specified here. Feel free to change it
const int dirPin = 2;


// Connect PUL+ (pulse -- PWM) on the driver to this pin
const int stepPin = 3;

/*
======================================================================================================================================
--------------------------------------------------------------- WIRING ---------------------------------------------------------------
======================================================================================================================================

Let's start from the top:
You should see ENA+/-, DIR+/-, and PUL+/- which correspond to "enable," "direction," and "pulse". Connect the DIR+ and PUL+ pins to the pins that
correspond with the variables you defined above. All other pins (should all be green wires) connect to ground (arduino ground).

Below this, you'll see A+/- and B+/-. These correspond to where the motor on the slide should connect to.

On the stepper motor you'll see 4 wires, a black, green, red, and blue wire. They should be connected to the pins on the driver specified below:

                A+ > BLACK

                A- > GREEN

                B+ > RED

                B- > BLUE

Finally, at the very bottom, you'll see ground and VCC. 

Get the 12V 5A power adapter from the locker and connect the M-F 12V power connector (looks like: https://media.digikey.com/Photos/Sparkfun%20Elec%20%20Photos/MFG_PRT-10288.jpg)
to the end of it. The positive side (indicated on the connector) should go into VCC and the negative goes into the GROUND pin. If you did this correctly, you should see a blue light  
turn on in the driver.

I've already set the settings on the driver to what I felt was best, so no more tampering needed! You should be able to run the code now.

////////////////// OTHER NOTES //////////////////

The motor is triggered due to rising-edge signals. On the driver, I have set 400 PWM signals to correspond to 1 revolution. Because the screw has a lead of 
8, 400 PWM signals correspond to 8 mm of movement

CONVERSION RATIO FROM STEPS TO MM

8 mm / 400 sign

*/

/*
======================================================================================================================================
------------------------------------------------------------- ACTUAL CODE ------------------------------------------------------------
======================================================================================================================================
*/


// creating stepper object to control motor
AccelStepper stepper(motorInterfaceType, stepPin, dirPin);


// i pulled this out of my ass the value has no purpose i just dont want to rewrite 1250 so many times
int arbitrary_signals = 5000;


void setup() {
    // set the max speed the motor is allowed to spin
    stepper.setMaxSpeed(400);

    // sets the acceleration of the stepper motor. 
    // ***SIDE NOTE***: if the acceleration is too slow, there will be a lot of counter-torque produced by the rod, so it will make a really loud concerning sound.
    // It's fine, just up this a lil but don't push it too high. Stay below 1600 because that's the fastest that I tested.
    stepper.setAcceleration(800);

    // move() and moveTo() basically tell the internal motor how many signals the motor gotta send, but it doesn't actually send the signals. It's just configuring the logic.
    // the 1250 here is the number of PWM signals the motor will need to receive before it can call itself finished. You have to call stepper.run() for it to actually move
    //You can do a conversion of steps to distance based on the parameters that I've defined above
    stepper.move(arbitrary_signals);
}

void loop() {
  // checks if it has moved the required distance, if it has, it will flip directions and move the same distance the other way

  if (stepper.distanceToGo() == 0) {
    arbitrary_signals *= -1;
    stepper.move(arbitrary_signals);
  }

  // you MUST call this super often, basically every time you run through this loop. this is the one that actually sends the PWM signal to the motor, so if this is not accessable and
  // called every loop, the motor won't run.
  //stepper.run();
}
