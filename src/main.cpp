#include "main.h"
#include "portdef.hpp"		// portdef.hpp defines all the ports by name we use on the V5 brain
													// using a portdef.hpp file, and using the definitions set in their
													// as the paramenters when we need to specify a port in our code,
													// allows us very quickly to move things around and gives us one
													// place to look for all the ports and their assigned functions

#include "globals.hpp"		// ensure access to all global functions and variables
													// this goes hand in hand with globals.cpp

#include "drivebase.hpp"	// Include the drivebase functions for use, see drivebase.cpp
													// drivebase.hpp for definitions and descriptions

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// While technicaly not needed it is a good practice to intialize the motors
	// and sensors during the intialization face of the program
	// We are using the green cartridge here - if they are changed the MOTOR_GEARSET
	// needs to be subsequently adjusted
	//             red    - 100RPM max  (MOTOR_GEARSET_36)
	//             green  - 200RPM      (MOTOR_GEARSET_18)
	//             blue   - 600RPM      (MOTOR_GEARSET_6)
	// Also notice the right_whell motor has the direction reversed, so in this case
	// if we give a command to move forward, the left motor runs clockwise and the
	// right motor counter clockwise - moving the robot forward.

	// gearset is set to the RED cartridge - you may need to change back to default of green
	// by changing MOTOR_GEARSET_36 to MOTOR_GEARSET_18
	pros::Motor left_wheel (LEFT_MOTOR_PORT, MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_wheel (RIGHT_MOTOR_PORT, MOTOR_GEARSET_36, true, pros::E_MOTOR_ENCODER_DEGREES);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // Setup a Controller object so we cna read the joysticks and buttons
	pros::Controller master(pros::E_CONTROLLER_MASTER);

  // We will create two modes:
	//	1 - autoexecute a deviationDrive test - driving straight for 500cm
	//			while no joystick control of robot
	//  2 - drive usign the joystic control

	// if the A button is pressed we drop into the autodeviation test mode
	// in this case if the X is pressed while executing it an Estop is performed

	// If the Y button is pressed it will drop into Joystick only control and it
	// will ignore all other buttons
  bool eStop = false;				// Emergency stop button

	while(true && !eStop){
		if (master.get_digital(DIGITAL_A)) {
	      // We are running the automatic deviation test code
				// We need to know a few things as defiend in global.hpp /global.cpp
				// int deviationLength -- length of defiation course default is 500cm (5m)
				// int motorMaxSpeed -- defaukt is 75RPM

				// In drivebase.hpp -- wheel diameter default is 10cm (define WHEEL_DIAM 10 )

				// Imagine a wheel of diameter 10cm - this means it's circumfrence is equal to:
			  // circum = 3.14 * 10cm = 31.4cm
			  // This means that for every 360 degree of axel turn the robot travels 31.4cm
			  // This means that for 1degree of rotation this is: (3.14 * 10) / 360 = 0.08722cm of travel
			  // So if we know that then if we want to travel for a given distance in cm, we can calculate
			  // the number of encoder degrees we need to move by taking
			  // Numb of Degrees to travel =  distance to travel / distance for 1 degree

			  // We use drivebase.hpp to set the wheel diameter
			  float degreesTravel = (deviationLength / (3.14 * WHEEL_DIAM)) * 360;
			  // set the "window" for which we need to encoder to reach and stop movement
			  float degreesTravelAbs = fabsf(degreesTravel);        // we are doing absolute values
			  float minTarget = degreesTravelAbs -5;                // we can also go backwards by
			  float maxTarget = degreesTravelAbs +5;                // giving a -distance as input!

				left_wheel.tare_position();       // ensure encoders are reset before
				right_wheel.tare_position();      // movement.
				left_wheel.move_absolute(degreesTravel, motorMaxSpeed);
				right_wheel.move_absolute(degreesTravel, motorMaxSpeed);

				if(DEBUG){
					 std::cout << "\ndriveForDistance -- distance: " << deviationLength << " speed: " << motorMaxSpeed << "\n";
					 std::cout << "Degrees to travel: " << degreesTravel << "\n";
					 std::cout << "minTarget: " << minTarget << " maxTarget: " << maxTarget << "\n";
				}

				while (!((fabs(left_wheel.get_position()) < (maxTarget)) && (fabs(left_wheel.get_position()) > minTarget)) && !eStop) {
					pros::delay(2);
					if (master.get_digital(DIGITAL_X)) {
						eStop = true;
						if(DEBUG) {
							std::cout << "Encoder Left: " << left_wheel.get_position() << " Right: " << right_wheel.get_position() << "\n";
						}
						// End we are stopping
						left_wheel.move_velocity(0);
						right_wheel.move_velocity(0);
					}
				}

				if(DEBUG) {
					std::cout << "Encoder Left: " << left_wheel.get_position() << " Right: " << right_wheel.get_position() << "\n";
				}
				// we will stop the motors
				left_wheel.move_velocity(0);
				right_wheel.move_velocity(0);
	  }

		if (master.get_digital(DIGITAL_Y)) {
				if(DEBUG) {
					std::cout << "Running JoyStick Control program \n";
				}
				// We are running joystick control code
		    while (true && !eStop) {

	    		// Robot is under Joystick Control
					int left = master.get_analog(ANALOG_LEFT_Y);
					int right = master.get_analog(ANALOG_RIGHT_Y);
					left_wheel = left;
					right_wheel = right;
					if (master.get_digital(DIGITAL_X)) {
						eStop = true;
					}
					pros::delay(20);						// Do not exhaust the CPU!
				}
		}		// End Joystick Control code
 	}
	// We have dropped out through eStop
	if(DEBUG) {
		std::cout << "Program ended due to eStop action\n";
	}
}
