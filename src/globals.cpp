// ------- globals.cpp ---------------------------------------------------------
//
// Use globals.cpp together with globals.hpp to define all motor and other objects
// which should be available/accessible in autonomous and opcontrol code.
//
// forexample setup a motor definition and then add a extern pros::motor to
// globals.h to ensure all other modules have access -- of course by including
// #include "globals.h" in the relevant source (.cpp) files

#include "globals.hpp"
#include "portdef.hpp"
#include "main.h"

// declare the motor objects we are using - see initialize section for more
// details see the initialize() function in main.cpp
pros::Motor left_wheel (LEFT_MOTOR_PORT, MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_wheel (RIGHT_MOTOR_PORT, MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);

int motorMaxSpeed = 75;            // Maximum movement speed
const int motorDefaultSpeed = 75;  // our default movement speed
const int maxAllowedSpeed = 100;   // a constant maximum allowed speed
const int deviationLength = 500;   // defualt deviation test course length is 5m or 500cm
