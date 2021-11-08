// MET 418 drivebase configuration - basic setup for end of quarter exit lab testing
// Author:    Willem Scholten
// Date:      11 - Oct - 2021
// Version:   1.0

#ifndef DRIVE_BASE_H_
#define DRIVE_BASE_H_

#include "globals.hpp"

#define WHEEL_DIAM 10     // diameter in cm (larger omni/traction wheels)
#define WHEEL_BASE 38.5   // wheel base distance between center
                          // of two frontwheels in cm

void driveDeviationAutoTest(int speed, int distance); // Function to perform an automatic
                                                      // straight line drive of distance - 5m
                                                      // at a speed of 75% of maximum motor speed

void driveForDistance(float distance, int speed);  // drive a distance in cm at
                                                   // a given speed

void pivotTurn(float angle, int speed);            // pivot turn (turn in place)
                                                   // for angle given in degrees at
                                                   // given speed. -deg counter
                                                   // clockwise turn.

float skewDrive(float maxSpeed, int slice, float prevSpeed);  // Ramp slowely the speed of 
                                                              // the drive wheels up to move 
                                                              // robot none jerking forward
                                                              
#endif
