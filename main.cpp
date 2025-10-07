/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Students
/*    Team:         W                                                         */
/*    Created:      7/8/2025, 10:16:11 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// libraries
#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

brain Brain; // <- What's this for?

//Left Side Motors | Also instances
motor FrontLeft = motor (PORT18, false);
motor BackLeft = motor (PORT12, false);
motor_group Left = motor_group (FrontLeft, BackLeft);

//Right Side Motors
motor FrontRight = motor (PORT13, true);
motor BackRight = motor (PORT17, true);
motor_group Right = motor_group (FrontRight, BackRight); 

//Drive Definition
drivetrain Drive = drivetrain (Left,Right);
controller Controller1; 

//Intake
motor ILeft = motor (PORT10,true);
motor IRight = motor (PORT15,false);
motor_group Intake = motor_group (ILeft, IRight);


// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
// Custom wait function
// TimeUnites is: msec, sec
void wait(int time, timeUnits units) { // might change this to floats for better adjustments
  switch (units) {
    case timeUnits::msec:
      break;
    case timeUnits::sec: // 1 second,instead of 1 milisecond
      time = time / 1000;
      break;
    default:
      // If the unit is not recognized, default to milliseconds
      break;
  }
  // Perform the wait
  task::sleep(time);
}
void pre_auton(void) {
  Intake.setVelocity (50000,rpm); // Was 100,000 RPM but changed to 50K for testing.
  Drive.setDriveVelocity (10000, rpm);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

// Basic movement functions for autonomous:
void move_forward(int numSec) { // numSec for time running
  Right.setVelocity(50,percent); // 50% velocity
  Right.spin(forward);
  Brain.Screen.print("Right Motor moving forward");
  Brain.Screen.print("Motor velocity: %d", FrontLeft.velocity(percent));
  Left.setVelocity(50,percent); // 50% velocity
  Left.spin(forward);
  Brain.Screen.print("Left Motor moving forward");
  Brain.Screen.print("Pausing brain for %d seconds", numSec);
  wait(numSec, sec); // Waits for 1 second

  Right.stop();
  Left.stop();
}
void move_backward(int numSec) { // numSec for time running
  Right.setVelocity(50,percent); // 50% velocity
  Right.spin(reverse);
  Brain.Screen.print("Right Motor moving backward");
  Brain.Screen.print("Motor velocity: %d", FrontLeft.velocity(percent));
  Left.setVelocity(50,percent); // 50% velocity
  Left.spin(reverse);
  Brain.Screen.print("Left Motor moving backward");
  Brain.Screen.print("Pausing brain for %d seconds", numSec);
  wait(numSec, sec); // Waits for 1 second

  Right.stop();
  Left.stop();
}
void turn_right(int numSec) { // numSec for time running
  Right.setVelocity(50,percent); // 50% velocity
  Right.spin(reverse);
  Brain.Screen.print("Right Motor moving backward");
  Left.setVelocity(50,percent); // 50% velocity
  Left.spin(forward);
  Brain.Screen.print("Left Motor moving forward");
  Brain.Screen.print("Pausing brain for %d seconds", numSec);
  wait(numSec, sec); // Waits for 1 second

  Right.stop();
  Left.stop();
}
void turn_left(int numSec) { // numSec for time running
  Right.setVelocity(50,percent); // 50% velocity
  Right.spin(forward);
  Brain.Screen.print("Right Motor moving forward");
  Left.setVelocity(50,percent); // 50% velocity
  Left.spin(reverse);
  Brain.Screen.print("Left Motor moving backward");
  Brain.Screen.print("Pausing brain for %d seconds", numSec);
  wait(numSec, sec); // Waits for 1 second

  Right.stop();
  Left.stop();
}
// Intake functions:
void intake_in(int numSec) {
  Brain.Screen.print("Intake motor spinning forward");
  Intake.setVelocity(40, percent); // Set intake speed to 40%
  Intake.spin(forward);
  Brain.Screen.print("Pausing brain for %d seconds", numSec);
  wait(numSec, sec); // Waits for 1 second

  Intake.stop();
}
void intake_out(int numSec) {
  Brain.Screen.print("Intake motor spinning reverse");
  Intake.spin(reverse);
  wait(numSec, sec); // Waits for 1 second
  
  Intake.stop();
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here. Needs to be hard-coded
  /**
   * Basic outline:
   * Once the robot spawns in, make sure the robot collects the 3 octaballs then turns Left/Right and then collects the other 3 on the opposite side. 
   * Then, the robot comes to the center and drops them all off. Finally, the robot should make its way to 1 of the droppers and end the program.
   */
  // ..........................................................................
  move_forward(2); // Move forward for 2 seconds 
  turn_right(1); // Turn right for 1 second
  move_forward(3); // Move forward for 2 seconds
  turn_right(2); // Turn right for 2 second
  intake_in(3); // Intake in for 3 seconds
  move_backward(4); // Move backward for 4 seconds
  move_forward(1); // Just to get out of the way.
  turn_left(2); // Turn left for 2 seconds
  move_forward(2); // Move forward for 2 seconds
  turn_right(2); // Turn right for 2 second
  intake_in(3); // Intake in for 3 seconds
  move_forward(2); // Getting the top octaball
  move_backward(2); // Move backward for 5 seconds
  turn_left(1); // Turn left for 1 second

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // Remove autonomous(); from here! Only run user control code in this loop.
  autonomous();
  // ..........................................................................
  while(true) {
    // Intake control
  if (Controller1.ButtonL1.pressing()) {
    Intake.setVelocity(40, percent); // Set intake speed to 40%
    Intake.spin(forward);
  } else if (Controller1.ButtonL2.pressing()) {
    Intake.setVelocity(40, percent); // Set intake speed to 40%
    Intake.spin(reverse);
  } else {
    Intake.stop();
  }

    // Arcade drive with left stick
    int fwd = -Controller1.Axis3.position(percent); // Invert up/down
    int turn = -Controller1.Axis4.position(percent); // Invert left/right

    int leftSpeed = fwd + turn;
    int rightSpeed = fwd - turn;

    // Deadzone for small joystick movements
    if(abs(leftSpeed) > 10 || abs(rightSpeed) > 10){
      Left.setVelocity(leftSpeed, percent);
      Left.spin(forward);
      Right.setVelocity(rightSpeed, percent);
      Right.spin(forward);
    } else{
      Left.setVelocity(0, percent);
      Right.setVelocity(0, percent);
      Left.setStopping(coast);
      Right.setStopping(coast);
    }

    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol); // This is an infinite loop

  // Run the pre-autonomous function.
  pre_auton();
  // Prevent main from exiting with an infinite loop.
  // Also, stop all motors when main ends.
  Left.stop();
  Right.stop();
  Intake.stop();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
