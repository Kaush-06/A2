#include "Motors.h"
#include "LineSensors.h"
#include "Kinematics.h"
#include "encoders.h"

Kinematics_c pose;
Motors_c motors;
LineSensors_c line_sensors;
bool calibrated = false;

void setup() {
    Serial.begin(9600);
    delay(2000);  
    setupEncoder0();
    setupEncoder1();
    motors.initialise();
    pose.initialise(0, 0, 0);
    // Initialize IR emitter and sensors for Digital Approach
//    line_sensors.initialiseForDigital();
//
//    // Start calibration
//    line_sensors.calibration_routine();
//    calibrated = true;
}

void loop() {
    pose.update();

    if( !calibrated ) {
      motors.setPWM(20,-20);
      line_sensors.calibration_routine();
      calibrated = true;
    }
  
    if( calibrated == true ) {
      motors.setPWM(0,0);
    }
//    line_sensors.digitalReadAllSensors();
//    unsigned long discharge_time = line_sensors.measureDischargeTime();
//    Serial.print(discharge_time);
//    Serial.print("\n");

    // **Ensure calibration has completed before proceeding**
    if (!calibrated) return;

    // **Get calibrated IR intensity**
    float ir_intensity = line_sensors.calcCalibratedDischargeTime();

    Serial.print("IR Intensity: ");
    Serial.println(ir_intensity);

    
     // **Stop if the leader has stopped**
//    if (ir_intensity <0.01) {  
//        motors.setPWM(0, 0);  // Stop movement
//
//        // Move backward slightly if too close
//        if (ir_intensity < 0.02) {
//            motors.setPWM(-20, -20);
//        }
//
//        // **Wait until the IR signal changes (leader moves again)**
//        while (true) { 
//            unsigned long new_sensor_time = line_sensors.measureDischargeTime();
//            if (abs(new_sensor_time - sensor_time) > 20) {  // Leader moved again
//                Serial.println("Leader moving again! Resuming follower...");
//                break;  // Exit loop and resume movement
//            }
//            delay(100);
//        }
//    } else {
//        motors.setPWM(30, 30);  // Move forward normally
//    }

//    delay(500);  // 
}
