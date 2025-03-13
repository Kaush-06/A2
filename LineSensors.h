#ifndef _LINESENSORS_H
#define _LINESENSORS_H
#define NUM_SENSORS 1

#define EMIT_PIN    11  // IR emitter pin (Leader robot)
#define BL_PIN      A6   // Left bump sensor (Digital approach)
//#define BR_PIN      5   // Right bump sensor (Digital approach)
const int sensor_pins[NUM_SENSORS] = { A6 };

#include "Motors.h"

class LineSensors_c {
  public:
    float readings[NUM_SENSORS];
    bool calibrating;
    unsigned long minimum, maximum, range;
    Motors_c motors;

    LineSensors_c() {
        calibrating = true;
        minimum = 100000;  // Set high for proper min detection
        maximum = 0;
        range = 1;  // Default to 1 to avoid division errors
    }

//     **Initialize Digital Approach for IR detection**
    void initialiseForDigital() {
        pinMode(EMIT_PIN, OUTPUT);
        digitalWrite(EMIT_PIN, HIGH); // Keep IR LED ON
        pinMode(BL_PIN, INPUT);
//        pinMode(BR_PIN, INPUT);
    }

    // **Measure Discharge Time for IR detection**
    unsigned long measureDischargeTime() {
        unsigned long start_time = micros();

        // Charge capacitor
        pinMode(BL_PIN, OUTPUT);
        digitalWrite(BL_PIN, HIGH);
        delayMicroseconds(10);

        // Switch to INPUT and measure discharge time
        pinMode(BL_PIN, INPUT);
        while (digitalRead(BL_PIN) == HIGH) {
            if (micros() - start_time > 30000) break;  // Prevent infinite loop
        }

        return micros() - start_time;
    }

    // **Calibration Routine**
    void calibration_routine() {
        calibrating = true;
        Serial.println("Calibrating...");
  
        unsigned long startTime = millis();
        maximum = 0;
        minimum = 100000;  // Set to a large initial value

        while (millis() - startTime < 5000) {  // Calibrate for 5 seconds
            unsigned long discharge_time = measureDischargeTime();
            Serial.println(discharge_time);  // Debugging

            // Update max and min values
            if (discharge_time > maximum) maximum = discharge_time;
            if (discharge_time < minimum) minimum = discharge_time;
        }

        // Store calibration values
        range = maximum - minimum;
        if (range == 0) range = 1;  // Prevent divide-by-zero

        calibrating = false;
        Serial.println("Calibration complete!");
        Serial.print("Min: "); Serial.println(minimum);
        Serial.print("Max: "); Serial.println(maximum);
    }

    // **Normalize Discharge Time to IR Intensity (0 to 1)**
    float calcCalibratedDischargeTime() {
        unsigned long discharge_time = measureDischargeTime();

        // Prevent extreme values
        if (discharge_time < minimum) discharge_time = minimum;
        if (discharge_time > maximum) discharge_time = maximum;

        // Normalize between 0 and 1
        float ir_intensity = (float)(discharge_time - minimum) / range;

        // Ensure values are within [0,1]
        if (ir_intensity < 0) ir_intensity = 0;
        if (ir_intensity > 1) ir_intensity = 1;

        return ir_intensity;
    }
};

#endif
