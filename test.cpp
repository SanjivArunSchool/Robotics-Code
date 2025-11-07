#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
motor leftMotorA = motor(PORT12, ratio6_1, false);
motor leftMotorB = motor(PORT18, ratio6_1, false);
motor_group LeftDriveSmart = motor_group(leftMotorA, leftMotorB);
motor rightMotorA = motor(PORT13, ratio6_1, true);
motor rightMotorB = motor(PORT17, ratio6_1, true);
motor_group RightDriveSmart = motor_group(rightMotorA, rightMotorB);
inertial DrivetrainInertial = inertial(PORT9);
smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, DrivetrainInertial, 299.24, 320, 40, mm, 0.5);

controller Controller1 = controller(primary);


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}

bool vexcode_initial_drivetrain_calibration_completed = false;
void calibrateDrivetrain() {
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  DrivetrainInertial.calibrate();
  while (DrivetrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  vexcode_initial_drivetrain_calibration_completed = true;
  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}

void vexcodeInit() {

  // Calibrate the Drivetrain
  calibrateDrivetrain();

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Students                                                  */
/*    Team:         W                                                         */
/*    Created:      7/8/2025, 10:16:11 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

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

void pre_auton(void) {
  Intake.setVelocity (50000,rpm); // Was 100,000 RPM but changed to 50K for testing.
  Drivetrain.setDriveVelocity (10000, rpm);
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

// Basic movement functions for autonomous:
// === Movement Functions ===
void move_forward(double inches1) {
  Drivetrain.setDriveVelocity(30, percent);
  Drivetrain.driveFor(reverse, inches1, inches);
  Drivetrain.setStopping(coast);
}

void move_backward(double inches1) {
  Drivetrain.setDriveVelocity(50, percent);
  Drivetrain.driveFor(forward, inches1, distanceUnits::in);
}

// === Fixed Turn Function ===
void turnToRotate(double targetAngle, double speed = 25) {
  // Reset inertial rotation so 0 is your current facing direction
  DrivetrainInertial.setRotation(0, degrees);

  double current = 0;
  double tolerance = 2.0; // acceptable error in degrees
  double startTime = Brain.timer(msec);
  double timeout = 4000; // 4 seconds safety limit

  if (targetAngle > 0) {
    // Turn right
    LeftDriveSmart.spin(reverse, speed, percent);
    RightDriveSmart.spin(forward, speed, percent);
    while ((current < targetAngle - tolerance) && (Brain.timer(msec) - startTime < timeout)) {
      current = DrivetrainInertial.rotation(degrees);
      wait(10, msec);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();
  } else {
    // Turn left
    LeftDriveSmart.spin(forward, speed, percent);
    RightDriveSmart.spin(reverse, speed, percent);
    while ((current > targetAngle + tolerance) && (Brain.timer(msec) - startTime < timeout)) {
      current = DrivetrainInertial.rotation(degrees);
      wait(10, msec);
    }
    LeftDriveSmart.stop();
    RightDriveSmart.stop();
  }

  // Stop all drive motors with brake hold
  LeftDriveSmart.stop(coast);
  RightDriveSmart.stop(coast);

  Brain.Screen.clearScreen();
  Brain.Screen.print("Turn done at %.2f°", current);
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
  Drivetrain.setDriveVelocity(50, percent);
  Drivetrain.driveFor(forward, inches, distanceUnits::in);
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

void autonomous(void) {
  // Set a safe speed for testing
  Left.setVelocity(60, percent);
  Right.setVelocity(60, percent);
  Intake.setVelocity(60, percent);

  move_forward(12); // 12 63-18
  //turnToRotate(75); // 90
  // move_forward(8.5);
  // turnToRotate(0);
  // intake_and_move_forward(22);
  // move_backward(8);
  // turnToRotate(-45);  // 315° == -45°
  // move_forward(23);
  // intake_out(5);
  // move_backward(23);
  // turnToRotate(0);
  // move_backward(47);
  // turnToRotate(-90);
  // move_backward(33);
  // Controller1.Screen.clearScreen();
  // Controller1.Screen.setCursor(1,1);
  // Controller1.Screen.print("Done");
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
// Works like a charm
void usercontrol(void) {
  // Remove autonomous(); from here! Only run user control code in this loop.
  //autonomous();
  // ..........................................................................
  while(true) {
    // Intake control
    int intakePercent = 60;
    Intake.setVelocity(intakePercent, percent); // Set intake speed to 40%
    if (Controller1.ButtonL1.pressing()) {
      Intake.spin(forward);
    } else if (Controller1.ButtonL2.pressing()) {
      Intake.spin(reverse);
    } else {
      Intake.stop();
    }
    // Change Speed of Intake
    if(Controller1.ButtonR1.pressing()) {
      intakePercent = intakePercent - 10;
    } else if(Controller1.ButtonR2.pressing()) {
      intakePercent = intakePercent + 10;
    }

    // Arcade drive with left stick
    int fwd = -Controller1.Axis3.position(percent); // Invert up/down
    int turn = -Controller1.Axis4.position(percent); // Invert left/right

    double boostFactor = 1.0;
    double initialBoostFactor = boostFactor;
    bool isBoostActivated = false;
    double leftSpeed = (fwd + turn) * boostFactor;
    double rightSpeed = (fwd - turn) * boostFactor;

    // Deadzone for small joystick movements
    if(abs(leftSpeed) > 15 || abs(rightSpeed) > 15){
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
    if(Controller1.ButtonX.pressing()) {
      boostFactor += 0.1;
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Overall speed increased");
      leftSpeed = (fwd + turn) * boostFactor;
      rightSpeed = (fwd - turn) * boostFactor;
    } else if (Controller1.ButtonB.pressing()) {
      boostFactor -= 0.1;
      Controller1.Screen.clearScreen();
      Controller1.Screen.print("Overall speed decreased");
      leftSpeed = (fwd + turn) * boostFactor;
      rightSpeed = (fwd - turn) * boostFactor;
    }
    if(Controller1.ButtonY.pressing()) {
      initialBoostFactor = boostFactor;
      if(isBoostActivated) {
        isBoostActivated = false;
      } else {
        isBoostActivated = true;
      }
      boostFactor = 2.0;
      Controller1.Screen.clearScreen();
      Controller1.Screen.setCursor(1,1);
      Controller1.Screen.print("Boost Activated");
      leftSpeed = (fwd + turn) * boostFactor;
      rightSpeed = (fwd - turn) * boostFactor;
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
  // autonomous();
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(0,0);
  Brain.Screen.print("In Main Function Awaiting command, Status: ");
  Drivetrain.isDone();
  // Also, stop all motors when main ends.
  Left.stop();
  Right.stop();
  Intake.stop();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
