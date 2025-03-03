/***************************************
 ,        .       .           .     ,-.  
 |        |       |           |        ) 
 |    ,-: |-. ,-. |-. ,-. ,-. |-      /  
 |    | | | | `-. | | |-' |-' |      /   
 `--' `-` `-' `-' ' ' `-' `-' `-'   '--' 
****************************************/

// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSORS_H
#define _LINESENSORS_H

// We will use all 5 line sensors (DN1 - 5)
// and so define a constant here, rather than
// type '5' in lots of places.
#define NUM_SENSORS 1

// Pin definitions
// This time, we will use an array to store the
// pin definitions.  This is a bit like a list.
// This way, we can either loop through the
// list automatically, or we can ask for a pin
// by indexing, e.g. sensor_pins[0] is A11,
// sensors_pins[1] is A0.
const int sensor_pins[ NUM_SENSORS ] = { A6 };

// This is the pin used to turn on the infra-
// red LEDs.
#define EMIT_PIN   11


// Class to operate the linesensors.
class LineSensors_c {
  
  public:

    bool calibrating;
    // Store your readings into this array.
    // You can then access these readings elsewhere
    // by using the syntax line_sensors.readings[n];
    // Where n is a value [0:4]
    float readings[ NUM_SENSORS ];

    // Variables to store calibration constants. 
    // Make use of these as a part of the exercises 
    // in labsheet 2.
    float minimum[ NUM_SENSORS ];
    float maximum[ NUM_SENSORS ];
    float scaling[ NUM_SENSORS ];
    float range[ NUM_SENSORS ];
    // Variable to store the calculated calibrated
    // (corrected) readings. Needs to be updated via
    // a function call, which is completed in 
    // labsheet 2.
    float calibrated[ NUM_SENSORS ];

    // Constructor, must exist.
    LineSensor_c() {
      // leave this empty
    }

    // Refer to Labsheet 2: Approach 1
    // Fix areas marked ????
    // Use this function to setup the pins required
    // to perform a read of the line sensors using
    // the ADC.
    void initialiseForADC() {

      // Ensure that the IR LEDs are on
      // for line sensing
      pinMode( EMIT_PIN, INPUT );
      digitalWrite( EMIT_PIN, LOW );

      // Configure the line sensor pins
      // DN1, DN2, DN3, DN4, DN5.
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
              pinMode( sensor_pins[sensor], INPUT_PULLUP );
      }
      
    } // End of initialiseForADC()



    // Refer to Labsheet 2: Approach 1
    // Fix areas marked ????
    // This function is as simple as using a call to 
    // analogRead()
     void readSensorsADC() {

      // First, initialise the pins.
      // You need to complete this function (above).
      initialiseForADC();

      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
               readings[sensor] = analogRead(sensor_pins[sensor]);
      }

    } // End of readSensorsADC()

//     void calibration_routine(){
//      calibrating = true;
//      Serial.print("Calibrating!");
//      for ( int n = 0; n < NUM_SENSORS; n++) {
//             maximum[n] = 0;
//             minimum[n] = 1023;
//      }
//      unsigned long startTime = millis();
//      
//      while( millis() - startTime < 10000 ) {
//          readSensorsADC();
//          
//          for ( int sensor = 0; sensor < NUM_SENSORS; sensor++) {
//              if( readings[sensor] > maximum[sensor]) {
//                 maximum[sensor] = readings[sensor];
//              }
//      
//              if( readings[sensor] < minimum[sensor] ) {
//                  minimum[sensor] = readings[sensor];
//            }
//            
//           }
////           if (millis() - startTime > 2000) {
////              calibrating = false;
////            }
//          break;
//
//        }
//      for( int n = 0; n < NUM_SENSORS; n++) {
//        range[n] = maximum[n] - minimum[n];
//      }
//     }

    void calibration_routine() {
      calibrating = true;
      Serial.print("Calibrating!\n");
    
      for (int n = 0; n < NUM_SENSORS; n++) {
        maximum[n] = 0;
        minimum[n] = 1023;
      }
    
      unsigned long startTime = millis();
//      Serial.print("Start time: ");
//      Serial.println(startTime);

      while (millis() - startTime < 5000) { 
        readSensorsADC();

        Serial.print(readings[0]);
        Serial.print("\n");
        
        for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
          if (readings[sensor] > maximum[sensor]) {
            maximum[sensor] = readings[sensor];
          }
    
          if (readings[sensor] < minimum[sensor]) {
            minimum[sensor] = readings[sensor];
          }
        }
//        unsigned long currentTime = millis();
//        Serial.print("Elapsed time: ");
//        Serial.println(currentTime - startTime);
      }

      calibrating = false;

      for (int n = 0; n < NUM_SENSORS; n++) {
        range[n] = maximum[n] - minimum[n];
      }
    
      Serial.print("Calibration complete!\n");
    }

    // Use this function to apply the calibration values
    // that were captured in your calibration routine.
    // Therefore, you will need to write a calibration
    // routine (see Labsheet 2)
    void calcCalibratedADC() {

      // Get latest readings (raw values)
      readSensorsADC();

      // Apply calibration values, store in calibrated[]
      for( int sensor = 0; sensor < NUM_SENSORS; sensor++ ) {
              calibrated[sensor] = (readings[sensor]- minimum[sensor])/range[sensor];
      }
      
    } // End of calcCalibratedADC()

    bool OnLine(){
      for (int sensor = 0; sensor < NUM_SENSORS; sensor++) {
        if (calibrated[sensor] > 0.9) {
          return true;
        }
      }
      return false;
    }

  
    // Part of the Advanced Exercises for Labsheet 2
    void initialiseForDigital() {
      
      // Ensure that the IR LEDs are on
      // for line sensing
      pinMode( EMIT_PIN, OUTPUT );
      digitalWrite( EMIT_PIN, HIGH );

    } // End of initialiseForDigital()

    // Part of the Advanced Exercises for Labsheet 2
    void readSensorsDigital() {
    //  ???
    } // End of readSensorsDigital()

}; // End of LineSensor_c class defintion



#endif
