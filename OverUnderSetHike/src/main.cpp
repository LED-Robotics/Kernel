#include "main.h"
using namespace okapi;

using namespace std;

// pros::Motor FL(20);
// pros::Motor BL(10);
// pros::Motor FR(11);
// pros::Motor BR(1);

std::shared_ptr<OdomChassisController> chassis =
ChassisControllerBuilder()
.withMotors(
    {10, 20}, // Left motors are 1 & 2 (reversed)
    {-1, -11}    // Right motors are 3 & 4
)
// Green gearset, 4 in wheel diam, 11.5 in wheel track
.withDimensions({AbstractMotor::gearset::green, (5.0 / 3.0)}, {{3.25_in, 19.0_in}, imev5GreenTPR})
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
Motor Intake1(3);
// pros::Motor wtf(20);

pros::ADIDigitalIn catapaultTouch{'B'};
pros::ADIDigitalIn catapaultTouchDown{'A'};

bool catapultLoading = false;

const int driveDeadzone = 0.05;

pros::ADIDigitalOut indexer{'C'};
pros::ADIDigitalOut tindexr{'D'};

pros::Imu IMU_(6);

pros::Controller Garfield(pros::E_CONTROLLER_MASTER); //lasagna
pros::Controller John(pros::E_CONTROLLER_PARTNER); //ooga booga set hike

pros::Vision vision (9);
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
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

// const double inch = 0.106;

void shoot (){
  Catapault.controllerSet(1.0);
  if(catapaultTouch.get_value()){
    Catapault.controllerSet(0.0);
  }
}


void intake(int seconds) {
 Intake1.controllerSet(-1.0);

}
void Outake(int seconds){
 Intake1.controllerSet(1.0);

}

// void drive(int distance){
//     const double INCH_CONSTANT = 0.09794150344116636047315923899847;
//     distance *= INCH_CONSTANT*360;

//     FL.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
//     BL.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
//     FR.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
//     BR.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

//     FL.tare_position();
//     BL.tare_position();
//     FR.tare_position();
//     BR.tare_position();

//     double X = BL.get_position();

//     while(X>distance or X<distance-5){
//         X = BL.get_position();
//         FL.move((X-distance)*5);
//         BL.move((X-distance)*5);
//         FR.move((X-distance)*5);
//         BR.move((X-distance)*5);
//     }

//     FL.move(0);
//     FR.move(0);
//     BL.move(0);
//     BR.move(0);
// }


// void turn(double angle){
//     const double INCH_CONSTANT = 0.09794150344116636047315923899847;

//     IMU_.tare();
//     int speed = 90;
//     double modifier = 1.0;
//     if(angle < 0) { speed *= -1; }
// 	double current = 0.0;
//     angle = abs(angle);
//     while(current < angle) {
//         current = abs(IMU_.get_rotation());
//         modifier = 1.8 - (current / angle) * 1.3;
//         if(modifier > 1.0) modifier = 1.0;

//         BL.move(speed * modifier);
//         FL.move(speed * modifier);
//         BR.move(-speed * modifier);
//         FR.move(-speed * modifier);
//         pros::delay(20);
//     }

//     BL.move(0);
//     FL.move(0);
//     BR.move(0);
//     FR.move(0);

//     // angle /= 2;

//     // IMU_.tare_heading();
//     // delay(250);
//     // int Bound = 0;

//     // double X = IMU_.get_heading();

//     // while(X>angle+5 or X<angle-5){
//     //  X = IMU_.get_heading();
   
//     //  FL.move((angle-X)*-0.7);
//     //  BL.move((angle-X)*-0.7);
//     //  FR.move((angle-X)*0.7);
//     //  BR.move((angle-X)*0.7);
//     //  delay(20);
//     // }

//     // FL.move(0);
//     // FR.move(0);
//     // BL.move(0);
//     // BR.move(0);
// }

void changeAlliance(int id) {
    int currentSig{RED_ID};
    int alliance{RED_ID};
    alliance = id;
}
int autonomousSelected{2};
bool rollForward{true};
int alliance{RED_ID};


void printData() {
    pros::lcd::clear_line(0);
    switch(autonomousSelected) {
        case 0:
            pros::lcd::print(0, "Auton Selected: Standby");
            break;
        case 1:
            pros::lcd::print(0, "Auton Selected: Longer");
            break;
        case 2:
            pros::lcd::print(0, "Auton Selected: ouralliagnceside");
            break;
        case 3:
            pros::lcd::print(0, "Auton Selected: oppositealliganceside");
            break;
    }


    pros::lcd::clear_line(1);
}


void lcdAutonSelect() {
    autonomousSelected++;
    if(autonomousSelected > 4)
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
void ourAlliganceSide(){     
 Outake(5);
//  drive(1);
//  turn(180);
//  drive(16);
//  turn(45);
//  drive(5);
//  intake(1);
//  turn(135);
//  drive(15);
//  Outake(1);
//  drive(5);
//  turn(135);
//  drive(42);
//  turn(-45);
//  drive(20);
//  intake(1);

}

void oppositeAlliganceSide(){
 Outake(5);
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
     //1
     changeAlliance(BLUE_ID);
 




}
void longer(){
    // drive(2);




}
void executeAutonomous() {
    switch(autonomousSelected) {
        case 0:
            break;
        case 1:
            longer();
            break;
        case 2:
            ourAlliganceSide();
            break;
        case 3:
            skills();
            break;
        case 4:
            oppositeAlliganceSide();
            break;
    }
    return;
}

void autonomous() {
    Catapault.setBrakeMode(AbstractMotor::brakeMode::coast);
    Climb1.setBrakeMode(AbstractMotor::brakeMode::coast);
    Climb2.setBrakeMode(AbstractMotor::brakeMode::coast);
    Intake1.setBrakeMode(AbstractMotor::brakeMode::coast);  
    
    


    longer();



    ourAlliganceSide();



    oppositeAlliganceSide();


	skills();
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
     //Have Button to prime, STOP at limit switch, and fire at a button
     if(shoot==1 && !catapaultTouchDown.get_new_press() || !catapaultTouch.get_new_press()){
         Catapault=80;
     }
     if(catapaultTouchDown.get_value() || catapaultTouch.get_new_press()){
         Catapault=20;
     }
     if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)){
        Catapault=127;
        pros::delay(250);
        Catapault=0;
     }
     if(Garfield.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
        Catapault=20;
     }
    }
void opcontrol() {
    John.clear();
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
    Intake1.setBrakeMode(AbstractMotor::brakeMode::coast);  
  
//  float driveDeadzone;
//  float driveCurveExtent;
    
//   controller_analog_e_t DRIVE_X;
//   controller_analog_e_t DRIVE_Y;


    while (true) {
//      double driveX;
//      double driveY;

    x = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) / 127.0;  // /127 to push value between -1.0 and 1.0
    y = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) / 127.0;

    x = driveCurveExtent * pow(x, 3) + (1- driveCurveExtent) * x;
    y = driveCurveExtent * pow(y, 3) + (1- driveCurveExtent) * y;

    chassisModel->arcade(y, x, driveDeadzone);
 


        // x = Garfield.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // y = Garfield.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // FL.move(y*80+x*80);
        // BL.move(y*80+x*80);
        // FR.move(y*80-x*80);
        // BR.move(y*80-x*80);

        // if (abs(x)<thresh){
        //     x = 0;
        // }

        // if(abs(y)<thresh){
        //     y = 0;
        // }
        
        // while(x == 0 && y == 0)
        // {
        // FL.brake();
        // BL.brake();
        // FR.brake();
        // BR.brake();
    
        // }



//  double turnVar;
//  double straightVar;


//     turnVar = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);//
//     straightVar = (double)Garfield.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);//
    
//     if (fabs(turnVar) < driveDeadzone) {
//         turnVar = 0.0;
//     }
//     if (fabs(straightVar) < driveDeadzone) {
//         straightVar = 0.0;
//     }
//     double completeSpeedLeft = straightVar;
//     double completeSpeedRight = straightVar;//full speed for going straight/backwards
    
//     if (turnVar!=0.0){//If You Want to Turn
//         if(turnVar>0.0){//Right************************************//
//             if(straightVar>0.0){//Going Forward
//                 completeSpeedRight=straightVar-fabs(turnVar);
//                 if(completeSpeedRight<0.0){
//                     completeSpeedRight=0.0;
//                 }
//             }
//             else if(straightVar<0.0){//Going Backward
//                 completeSpeedRight=straightVar+fabs(turnVar);
//                 if (completeSpeedRight>0.0){
//                     completeSpeedRight=0.0;
//                 }
//             }
//             else{
//                 completeSpeedLeft=fabs(turnVar);
//                 completeSpeedRight=-fabs(turnVar);
//             }
//         }
//         if(turnVar<0.0){//Left**************************************//
//             if(straightVar>0.0){//Going Forward
//                 completeSpeedLeft=straightVar-fabs(turnVar);
//                 if(completeSpeedLeft<0.0){
//                     completeSpeedLeft=0.0;
//                 }
//             }
//             else if(straightVar<0){//Going Backwards
//                 completeSpeedLeft=straightVar+fabs(turnVar);
//                 if(completeSpeedLeft>0.0){
//                     completeSpeedLeft=0.0;
//                 }
//             }
//             else{
//                 completeSpeedLeft=-fabs(turnVar);
//                 completeSpeedRight=fabs(turnVar);
//             }
//         }
        
              //Pos. values for right turn relative, Neg. for left relative
    // }

 
     
            

        if(Garfield.get_digital(pros::E_CONTROLLER_DIGITAL_X))
        {
                // Climb1.move(85);
                // Climb2.move(-85);
                Climb1.moveAbsolute(550, 500);
                Climb2.moveAbsolute(-550, 500);
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

		pros::delay(20);
	

}












