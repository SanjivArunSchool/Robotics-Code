
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

// brain Brain; // <- What's this for?

//Left Side Motors | Also instances
motor FrontLeft = motor (PORT18, false);
motor BackLeft = motor (PORT12, false);
motor_group Left = motor_group (FrontLeft, BackLeft);

//Right Side Motors
motor FrontRight = motor (PORT13, true);
motor BackRight = motor (PORT17, true);
motor_group Right = motor_group (FrontRight, BackRight); 

// Example values, adjust to match your robot's actual measurements!
double WHEEL_TRAVEL = 4.0;    // in inches (typical VEX wheel)
double TRACK_WIDTH = 12.0;    // in inches (distance between left and right wheels)
double WHEEL_BASE = 10.0;     // in inches (distance between front and back wheels)

//Drive Definition
drivetrain Drive = drivetrain(
  Left, Right, WHEEL_TRAVEL, TRACK_WIDTH, WHEEL_BASE, distanceUnits::in
);
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
void move_forward(double inches) { // Move backward for a distance in inches
  Drive.setDriveVelocity(50, percent);
  Drive.driveFor(reverse, inches, distanceUnits::in);
}
void move_backward(double inches) { // Move forward for a distance in inches
  Drive.setDriveVelocity(50, percent);
  Drive.driveFor(forward, inches, distanceUnits::in);
}
void turn_right(double degrees) { // Turn left for a certain angle
  Drive.setTurnVelocity(50, percent);
  Drive.turnFor(left, degrees, rotationUnits::deg);
}
void turn_left(double degrees) { // Turn right for a certain angle
  Drive.setTurnVelocity(50, percent);
  Drive.turnFor(right, degrees, rotationUnits::deg);
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
// Runs intake and moves forward at the same time for a given distance (inches) and intake speed (percent)
void intake_and_move_forward(double inches, int intake_speed = 40) {
  // Set intake speed and start spinning
  Intake.setVelocity(intake_speed, percent);
  Intake.spin(reverse);
  // Move forward while intake is running
  Drive.setDriveVelocity(50, percent);
  Drive.driveFor(forward, inches, distanceUnits::in);
  // Stop intake after moving
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

// void autonomous(void) {
//   // Autonomous routine using distance (inches) and angle (degrees)
//   move_forward(24); // Move forward 24 inches
//   turn_right(90); // Turn right 90 degrees
//   move_forward(5); // Move forward 5 inches
//   turn_left(90); // Turn left 90 degrees
//   intake_and_move_forward(20, 40); // Move forward 24 inches while intaking at 40% speed
//   turn_left(45);   // Turn left 45 degrees
//   move_forward(10); // Move forward 10 inches (chugging robot to score)
//   intake_out(1);   // Outtake for 1 second to score
//   move_backward(5); // Move backward 5 inches to original position
//   turn_right(45);   // Turn right 90 degrees (Continue moving in initial direction)
//   move_forward(10); // Move forward 12 inches to collect more octaballs // CHANGE
//   intake_and_move_forward(5, 40); // Move forward 12 inches while intaking at 40% speed // CHANGE
//   turn_left(135); // Turn left 135 degrees
//   move_forward(10); // Move forward 10 inches to reach scoring zone
//   intake_out(3);   // Outtake for 1 second to score
//   move_backward(5); // Move backward 5 inches to original position
//   turn_right(135);   // Turn right 135 degrees 
//   move_forward(5);
//   turn_left(90);
//   move_forward(12);
//   intake_and_move_forward(12, 40); // Move forward 12 inches while intaking at 40% speed
//   turn_left(90);
//   move_forward(2);
//   turn_left(45);
//   move_forward(10); // Move forward 10 inches to reach scoring zone
//   intake_out(3);   // Outtake for 1 second to score
//   move_backward(5); // Move backward 5 inches to original position
//   turn_right(45);   // Turn right 90 degrees 
//   move_forward(12); // Move forward 12 inches to collect more octaballs
//   intake_and_move_forward(12, 40); // Move forward 12 inches while intaking at 40% speed
//   turn_left(90);
//   move_forward(2);
//   turn_left(45);
//   move_forward(10); // Move forward 10 inches to reach scoring zone
//   intake_out(3);   // Outtake for 1 second to score
//   move_backward(5); // Move backward 5 inches to original position
//   turn_right(45);   // Turn right 45 degrees to correct orientation
//   turn_right(180);

//   move_backward(24); // Move forward 24 inches to get ready to park
//   move_forward(1); // For safety
//   move_backward(12); // Move to the center of the platform
//   // END //
// }

void autonomous(void) {
  // turn_right(90);
  move_backward(8);
  // shold be able to park in.
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
  //autonomous();
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
