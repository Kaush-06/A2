#define EMIT_PIN    11    // Shared emitter control pin
#define NUM_SENSORS 5
#include "Motors.h"  

// Pin definitions for line sensors
const int sensor_pins[NUM_SENSORS] = { 12, A0, A2, A3, A4 }; // DN1 - DN5

class LineSensors_c {
  public:
    float readings[NUM_SENSORS];
    float minimum[NUM_SENSORS];
    float maximum[NUM_SENSORS];
    float range[NUM_SENSORS];
    float calibrated[NUM_SENSORS];
    bool calibrating;

    LineSensors_c() {
      calibrating = true;
      for (int i = 0; i < NUM_SENSORS; i++) {
        minimum[i] = 1023; // Set min high initially
        maximum[i] = 0;    // Set max low initially
        range[i] = 1;      // Avoid division by zero
      }
    }

    void initialiseForDigital() {
      pinMode(EMIT_PIN, OUTPUT);
      digitalWrite(EMIT_PIN, HIGH);
    }

    unsigned long digitalReadSensor(int which) {
      if (which < 0 || which >= NUM_SENSORS) return 0;

      pinMode(sensor_pins[which], OUTPUT);
      digitalWrite(sensor_pins[which], HIGH);
      delayMicroseconds(10);
      pinMode(sensor_pins[which], INPUT);

      unsigned long start_time = micros();
      unsigned long timeout = 5000;

      while (digitalRead(sensor_pins[which]) == HIGH) {
        if (micros() - start_time > timeout) {
          Serial.println("Timeout occurred!");
          return timeout;
        }
      }
      unsigned long end_time = micros();
      return end_time - start_time;
    }

    void digitalReadAllSensors() {
      for (int i = 0; i < NUM_SENSORS; i++) {
        readings[i] = digitalReadSensor(i);
      }
    }

    Motors_c motors; 

    void calibrateSensors() {
      Serial.println("Starting Calibration...");
      unsigned long start_time = millis();
      unsigned long duration = 5000; // Calibrate for 5 seconds
      //newcode here
      motors.setTurn(30, -30, 3000); // Rotate smoothly for 3 seconds

      while (millis() - start_time < duration) {
        digitalReadAllSensors();
        for (int i = 0; i < NUM_SENSORS; i++) {
          if (readings[i] > maximum[i]) maximum[i] = readings[i];
          if (readings[i] < minimum[i]) minimum[i] = readings[i];
        }
        delay(10);
      }
      motors.setPWM(0, 0); // Stop the robot after calibration
      for (int i = 0; i < NUM_SENSORS; i++) {
        range[i] = maximum[i] - minimum[i];
        if (range[i] == 0) range[i] = 1; // Prevent division by zero
      }
      calibrating = false;
      Serial.println("Calibration Complete!");
    }

    void applyCalibration() {  // Renamed from calcCalibratedADC
      digitalReadAllSensors();
      for (int i = 0; i < NUM_SENSORS; i++) {
        calibrated[i] = (readings[i] - minimum[i]) / range[i];
        if (calibrated[i] < 0) calibrated[i] = 0;
        if (calibrated[i] > 1) calibrated[i] = 1;
      }
    }
};
