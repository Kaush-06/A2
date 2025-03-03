/******************************************
 ______       _                _           
(_____ \     | |           _  (_)          
 _____) )___ | |__   ___ _| |_ _  ____     
|  __  // _ \|  _ \ / _ (_   _) |/ ___)    
| |  \ \ |_| | |_) ) |_| || |_| ( (___     
|_|   |_\___/|____/ \___/  \__)_|\____)    
                                           
  ______                                   
 / _____)             _                    
( (____  _   _  ___ _| |_ _____ ____   ___ 
 \____ \| | | |/___|_   _) ___ |    \ /___)
 _____) ) |_| |___ | | |_| ____| | | |___ |
(______/ \__  (___/   \__)_____)_|_|_(___/ 
        (____/                             
 EMATM0054/53: University of Bristol.
 https://github.com/paulodowd/EMATM0054_53
*******************************************/


// These #include commands essentially "copy and paste" 
// the above .h files (tabs above) into your code here.
#include "Motors.h"         // Labsheet 1
#include "PID.h"            // Labsheet 1 - Advanced
#include "LineSensors.h"    // Labsheet 2
//#include "Magnetometer.h" // Labsheet 3
#include "Kinematics.h"     // Labsheet 4

// Encoders.h does not need modifying.
#include "Encoders.h"     // For encoder counts


// Used for Labsheet 0. Defines which pin the buzzer
// is attached to. #define works like a 
// find-and-replace through your code.
// See Labsheet 0.
#define BUZZER_PIN 6

// Instance of a class to operate motors.
// A class is like a template, and we name
// an "instance" to use.
// You can recognise a class type in the
// code by the convention of "_c"
// You will need to complete the class.
// See Labsheet 1.
   



// Instance of a class to operate the magnetometer.
// Completing the class is a later exercise in 
// Labsheet 3, so you can leave this commented
// out.
// See Labsheet 3.
//Magnetometer_c magnetometer;

// Instance of a class to estimate the pose
// of the robot.  You will need to calibrate
// this, and potentially improve it.
// See Labsheet 4.
Kinematics_c pose; 

// The setup() function runs only once when the
// robot is powered up (either by plugging in
// the USB cable, or activating the motor power.
// Use this function to do "once only" setup 
// and configuration of your robot.

// Instance of a class to operate the line
// sensors to measure the surface reflectance.
// You will need to complete the class.
// See Labsheet 2.
LineSensors_c line_sensors;
Motors_c motors; 

void setup() {
  Serial.begin(9600);
  delay(1500);
  Serial.println("***RESET***");

  line_sensors.initialiseForDigital();
  line_sensors.calibrateSensors();  // Start calibration at setup
}

void loop() {
      // line_sensors.applyCalibration();  // Apply calibration to readings

      // Serial.print("Calibrated Readings: ");
      // for (int i = 0; i < NUM_SENSORS; i++) {
      //   Serial.print(line_sensors.calibrated[i]);
      //   Serial.print(" ");
      // }
      // Serial.println();
      
      // delay(500);  // Delay for readability

    //newcode for obj detection task here
 


    line_sensors.digitalReadAllSensors(); //  Use digital read function

    // Print sensor readings for debugging
    Serial.print("Calibrated Readings: ");
    bool black_detected = false;
    motors.setPWM(30, 30);

    for (int i = 0; i < NUM_SENSORS; i++) {
        Serial.print(line_sensors.readings[i]); //  Print raw digital readings
        Serial.print(" ");

        // Detect black line (threshold can be adjusted)
        if (line_sensors.readings[i] > 1000) { // Adjust threshold based on testing
            black_detected = true;
        }
    }
    Serial.println(); // New line for readability

    if (black_detected) {
        Serial.println("Black line detected! Stopping and turning...");
        motors.setPWM(0, 0); // Stop motors
        delay(500); // Short pause

        // Perform a turn
        motors.setTurn(30, -30, 800); // Turn right for 800ms (adjust as needed)

        delay(500); // Pause after turn
    } else {
        // Move forward at moderate speed if no black line is detected
        motors.setPWM(30, 30);
    }

    delay(100); // Small delay to avoid excessive CPU usage

}
  //void setup() {

  // Setup up the buzzer as an output for
  // This is used in Labsheet 0
  //pinMode( BUZZER_PIN, OUTPUT );

  // Setup motors.  This is calling a function
  // from within the Motors_c class. You can 
  // review this inside Motors.h
  // Complete by working through Labsheet 1.
  

  // Setup the line sensors.  This is calling a 
  // function from within the LineSensors_c 
  // class. You can review this inside 
  // LineSensors.h.  
  // Complete by working through Labsheet 2.
  //                line_sensors.initialiseForADC();

  // These two functions are in encoders.h and
  // they activate the encoder sensors to 
  // measure the wheel rotation.  You do not 
  // need to change or update these.
  // These are used in Labsheet 4.
  // Encoder counts are counted automatically.
  //        setupEncoder0();
  //        setupEncoder1();

  // Setup the pose (kinematics). This is calling 
  // a function within the Kinematics_c class.
  // You can review this within Kinematics.h
  // This is used in Labsheet 4.
  //            pose.initialise(0, 0, 0);

  // Activates the Serial port, and the delay 
  // is used to wait for the connection to be
  // established.
                    // Serial.begin(9600);
                    // delay(2000);
                    // Serial.println(" *** READY *** ");

      //}


// put your main code here, to run repeatedly:
  //void loop() {

  // Used for Labsheet 0
      // analogWrite( BUZZER_PIN, 120 ); // on
      // delay(5);
      // analogWrite( BUZZER_PIN, 0 ); // off
      // delay(5);

  // The below is used for Labsheet 1.
  // using .setPWM() on `motors` means that we
  // are calling a function defined within 
  // "motors".  "motors" is just the name we 
  // have given to an instance (a copy) of the
  // class (i.e the class is like a template).  
  //
  // Here we are sending to the "setPWM()" function
  // inside the Motors_c class instance "motors"
  // We pass setPWM the arguments (values) +25 and 
  // 0.  This should cause your robot to rotate 
  // around 1 wheel. You will need to review 
  // Motors.h and fix the code to for other motor 
  // control to work properly.
  // See Labsheet 1
  
  //}


