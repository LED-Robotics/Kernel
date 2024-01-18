#include "main.h"
#include "okapi/impl/device/motor/motor.hpp"
#include "pros/misc.h"
using namespace okapi;

using namespace std;

pros::Motor FL(20);
pros::Motor BL(10);
pros::Motor FR(11);
pros::Motor BR(1);

std::shared_ptr<OdomChassisController> chassis =
ChassisControllerBuilder()
.withMotors(
    {-10, -19}, // Left motors are 1 & 2 (reversed)
    {1, 11}    // Right motors are 3 & 4
)
// Green gearset, 4 in wheel diam, 11.5 in wheel track
.withDimensions({AbstractMotor::gearset::blue, 1.0}, {{4.0_in, 17.25_in}, imev5BlueTPR})
.withGains(
    {0.001, 0.0, 0.0000},
    {0.001, 0.0, 0.0000},
    {0.000, 0.0, 0.0000}
)
.withOdometry()
.buildOdometry();
std::shared_ptr<ChassisModel> chassisModel = chassis->getModel();

Motor Climb1(16, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);

Motor Climb2(15, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor Catapault(14);
Motor Catapault2(17);
Motor Intake(12);

// pros::Motor wtf(20);

pros::ADIAnalogIn catapaultTouch{'A'};


bool catapultLoading = false;

const int driveDeadzone = 0.05;

pros::ADIDigitalOut indexer{'C'};
pros::ADIDigitalOut tindexr{'D'};

pros::Imu IMU_(6);

pros::Controller Garfield(pros::E_CONTROLLER_MASTER); //lasagna
pros::Controller John(pros::E_CONTROLLER_PARTNER); //ooga booga set hike

/**
 * Runs initialization code. This occurs as soon as the program is started penis.
 *
 * All other competition modes are blocked by initialize your penis; it is recommended
 * 
 */
void initialize() {
	pros::lcd::initialize();
	// pros::lcd::set_text(1, "Hello PROS User!");


    pros::lcd::register_btn0_cb(lcdAutonSelect);
	pros::lcd::register_btn1_cb(lcdAllianceSelect);
	


    // FL.set_gearing(pros::E_MOTOR_GEARSET_18);
    // BL.set_gearing(pros::E_MOTOR_GEARSET_18);
    // FR.set_gearing(pros::E_MOTOR_GEARSET_18);
    // BR.set_gearing(pros::E_MOTOR_GEARSET_18);

	// FR.set_reversed(true);
    // BR.set_reversed(true);
    Catapault2.setReversed(true);
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

// benis


/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via                                                                                                                                           pineapple broken 20240108_XXXX :(
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// const double inch = 0.106;

void prime() {
    while(catapaultTouch.get_value() > 1800) {
        Catapault.controllerSet(0.4);
        Catapault2.controllerSet(0.4);
    }
    Catapault.moveAbsolute(Catapault.getPosition(), 500);
    Catapault2.moveAbsolute(Catapault2.getPosition(), 500);
}

void shoot (){
  while(catapaultTouch.get_value() < 2200) {
        Catapault.controllerSet(1.0);
        Catapault2.controllerSet(1.0);
    }
    Catapault.controllerSet(0.0);
    Catapault2.controllerSet(0.0);
}


void intake(int seconds) {
 Intake.controllerSet(-1.0);

}
void Outake(int seconds){
 Intake.controllerSet(1.0);

}

void setIntake(bool state) {
    indexer.set_value(state);
    tindexr.set_value(state);
}

void tareAuton() {
    chassis->setState({0_ft, 0_ft, 0_deg});
    chassis->stop();
    chassis->waitUntilSettled();
}

void changeAlliance(int id) {
    int currentSig{RED_ID};
    int alliance{RED_ID};
    alliance = id;
}
int autonomousSelected{0};
bool rollForward{true};
int alliance{RED_ID};


void printData() {
    pros::lcd::clear_line(0);
    switch(autonomousSelected) {
        case 0:
            pros::lcd::print(0, "Auton Selected: Standby");
            break;
        case 1:
            pros::lcd::print(0, "Auton Selected: Left");
            break;
        case 2:
            pros::lcd::print(0, "Auton Selected: Right");
            break;
        case 3:
            pros::lcd::print(0, "Auton Selected: Skills");
            break;
    }


    pros::lcd::clear_line(1);
}


void lcdAutonSelect() {
    autonomousSelected++;
    if(autonomousSelected > 3)
        autonomousSelected = 0;
    printData();
}


void lcdAllianceSelect() {
    if(alliance == RED_ID) {
        alliance = BLUE_ID;
        pros::lcd::set_background_color(0, 0, 255);
    } else if(alliance == BLUE_ID) {
        alliance = RED_ID;
        pros::lcd::set_background_color(255, 0, 0);
    }
    printData();
}
void left(){     
    tareAuton();
     chassis->driveToPoint({2.7_ft, 0.000_ft});  


    // chassis->setState({0_ft, 0_ft, 45_deg});
    // chassis->driveToPoint({1.7_ft, -0.068_ft});  // most consistant score use as a default option if current auton is failing


    // chassis->driveToPoint({0.09_ft, 2.50_ft});
    // chassis->turnToAngle(55_deg);
    // tareAuton();
    // chassis->driveToPoint({1.72_ft, 0.0_ft});
    // tareAuton();

    // tareAuton();
    // chassis->turnToAngle(110_deg);
    // prime();
    // chassis->driveToPoint({2_ft, 0.0_ft});
    // tareAuton();
    // chassis->turnToAngle(45_deg);
    // tareAuton();
    // shoot();
    // setIntake(true);
    // chassis->driveToPoint({3.0_ft, -0.0_ft});
    // tareAuton();
    // chassis->turnToAngle(30_deg);
    // tareAuton();
    // chassis->driveToPoint({1.0_ft, 0.0_ft});
    // tareAuton();
    // setIntake(false);
    // chassis->driveToPoint({-4.7_ft, 1.3_ft});
    // chassis->turnToAngle(0_deg);
    // tareAuton();
}

void right(){
    tareAuton();
    // prime();
    chassis->driveToPoint({2.7_ft, 0.000_ft});  
    // tareAuton();
    // chassis->turnToAngle(-45_deg);
    // tareAuton();
    // shoot();
    // tareAuton();

    // Outake(5);
//  drive(1);
//  turn(180);
//  drive(16);
//  turn(45);
//  drive(5);
//  intake(1);
//  turn(180);
//  drive(10);
//  shoot();
//  turn(-90);
//  drive(18);
//  turn(45);
//  drive(21);
//  intake(1);




}
void skills(){
    while(true) {
        prime();
        pros::delay(500);
        shoot();
        pros::delay(1000);
    }
}

void executeAutonomous() {
    switch(autonomousSelected) {
        case 0:
            break;
        case 1:
            left();
            break;
        case 2:
            right();
            break;
        case 3:
            skills();
            break;
    }
    return;
}

void autonomous() {
    executeAutonomous();
    // left();
    // right();
}

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
void teleopCatapult()
    {
    bool shoot= Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    if(shoot) {
        Catapault.controllerSet(1.0);
        Catapault2.controllerSet(1.0);
    } else {
        
        pros::lcd::print(1, "other: %d", catapaultTouch.get_value());
        if(catapaultTouch.get_value() > 1800) {
            Catapault.controllerSet(0.4);
            Catapault2.controllerSet(0.4);
            
        } else {
            Catapault.moveAbsolute(Catapault.getPosition(), 500);
            Catapault2.moveAbsolute(Catapault2.getPosition(), 500);
        }
    }
    }
void opcontrol() {
    // John.clear();
    tareAuton();
    double thresh = 0.1;
    int shoot = 0;
    bool shot = false;
    int initer = 0;
    int oliniter = 0;
    bool expance = false;
    double x;
    double y;
    double driveCurveExtent = 0.95;

    Climb1.setBrakeMode(AbstractMotor::brakeMode::coast);
    Climb2.setBrakeMode(AbstractMotor::brakeMode::coast);
    Catapault.setBrakeMode(AbstractMotor::brakeMode::coast);
    Intake.setBrakeMode(AbstractMotor::brakeMode::coast);  


    while (true) {

    x = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127.0;  // /127 to push value between -1.0 and 1.0
    y = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0;

    x = driveCurveExtent * pow(x, 3) + (1- driveCurveExtent) * x;
    y = driveCurveExtent * pow(y, 3) + (1- driveCurveExtent) * y;

    chassisModel->arcade(y, x, driveDeadzone);

        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_X))
        {
                // Climb1.move(85);
                // Climb2.move(-85);
                Climb1.moveAbsolute(3350, 250);
                Climb2.moveAbsolute(-3350, 250);
            }

            if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_A))
            {
                Climb1.controllerSet(0.0);
                Climb2.controllerSet(0.0);
            }

            if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)){
                // Climb1.move(-85);
                // Climb2.move(85);
                Climb1.moveAbsolute(0, 500);
                Climb2.moveAbsolute(0, 500);
            }
            
            // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
        // {
        //   Climb1.move(-85);
        //   Climb2.move(85); 
        // }
        //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
        // {
        //   Climb1.move(0);
        //   Climb2.move(0); 
        // }
        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
        {
         indexer.set_value(true);
         tindexr.set_value(true);
        }

        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_B))
        {
         indexer.set_value(false);
         tindexr.set_value(false);
        }

        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
        {
            Intake.moveVelocity(127);
        }

        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        {
            Intake.moveVelocity(-127);
        }
        
        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
        {
            Intake.moveVelocity(0);
        }

       
        teleopCatapult();


        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        // {
        //   Intake1.move(-85);
          
        // }

        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        // {
        //     Intake1.brake();
            
        // }
        
        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_B))
        // {
        //     Intake1.move(85);
           
        // }
		pros::delay(20);

    }

        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        // {
        // Catapault.move(127);
        // }
        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        // {
        // Catapault.brake();
        // }

    //  if (catapaultTouchDown.get_value()) {
    //     Catapault.move(0);
	//     catapultLoading = false;
    //  }



    //  if (catapultLoading) {
	//     Catapault.move(90);
    //  }

    //  if (Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R2) && !catapultLoading) {
	//  Catapault.move(60);
	//  delay(100);
	//  Catapault.move(0);
	//  catapultLoading = true;
    //  }






        //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
        // {
        //     Intake1.brake();
            
        // }

        // if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        // {
        //   Catapault.move(50);
        //  //   if(catapaultTouch.get_value())
        //  //   {
        //  //     Catapault.brake();
        //  //   }
        // //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        // //      {
        // //         Catapault.move(50);
        // //         pros::delay(1000);

        // //      }
        //  int counter = 0;
        //  if(catapaultTouch.get_value())
        //     {

        //       if(counter<=0)
        //       {
        //        Catapault.brake();
        //        pros::delay(100);
        //        Catapault.move(0);
        //        counter++;
        //       }
        //       if(counter==1)
        //       {
        //         if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        //         {
        //         Catapault.move(50);
        //         pros::delay(50);
        //         Catapault.move(0);
        //         counter--;
        //         }
        //       }
        //      //  Catapault.move(10);
        //      //  pros::delay(100);
        //      //  Catapault.brake();
        //      //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        //      //  {
        //      //     Catapault.move(50);
        //      //     pros::delay(500);

        //        }
        //     }
        // }
        //   if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        // {
        //     Catapault.brake();
     
        // }

        



        //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        // {
        //  stupid.move(127);
        //  if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2))
        // {
        //  stupid.brake();
        // }
        // }

 
        //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        //  {
        //     wtf.move(127);
        //  }

        //  if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        //  {
        //     wtf.brake();
        //  }
                 
        //  if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        //  {
        //     wtf.move(127);
        //  }

        //  if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
        //  {
        //     wtf.brake();
        //  }

	

}












