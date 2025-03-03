# include "Encoders.h"   // provide automatic counting of encoders
# include "Kinematics.h" // maths for odometry/dead-reckoning
# include "Motors.h"
#include "LineSensors.h"
// Create a global instance of the kinematics class
Kinematics_c pose;
Motors_c motors;
LineSensors_c line_sensors; 


float left;
float right;
float angle_difference;
float current_angle;
float turn_threshold;
float target;
float distance_threshold;
float to_travel;
float travelled;
float travel_from_y;
float travel_from_x;

bool turning = false;

bool calibrated = false;


void setup() {

  Serial.begin(9600);
//  delay(2000);  
  setupEncoder0();
  setupEncoder1();

  motors.initialise();
//
//  // Initialise the pose of the robot to x=0, y=0,
//  // theta=0.  
  pose.initialise( 0, 0, 0 );
  // put your setup code here, to run once:
  line_sensors.initialiseForADC();
  setForwards(-300);
}

void loop() {
  // put your main code here, to run repeatedly:
  pose.update();
//  line_sensors.readSensorsADC();
//  int sensorValue = analogRead(line_sensors.readings[0]); // Read IR reflection
//  Serial.print("Sensor Value: ");
//  Serial.println(sensorValue);  // Print the sensor reading
//  delay(50);
  if( checkForwards()){
    motors.setPWM(0,0);
  }
  
}

void setForwards(float forwards_distance) {
  if( forwards_distance > 0 ) {
    motors.setPWM(30,31.87);
    pose.update();
    travel_from_x = pose.x;
    travel_from_y = pose.y;
    to_travel = abs(forwards_distance);    
  }
  else if ( forwards_distance < 0 ){
    motors.setPWM(-30,-31.87);
    pose.update();
    travel_from_x = pose.x;
    travel_from_y = pose.y;
    to_travel = abs(forwards_distance);
    Serial.print("Distance to travel: "); Serial.print(to_travel);
  }
}

bool checkForwards() {
  
  if( sqrt(sq(pose.x - travel_from_x) + sq(pose.y - travel_from_y)) - to_travel < distance_threshold ) {
    return false;
  } else{
    return true;
  }
}

void setTurn( float left_pwm, float right_pwm, float target_angle) {
  motors.setPWM(left_pwm,right_pwm);
  target = target_angle;
}

bool checkTurn() {
  pose.update();  // Ensure pose is being updated

  angle_difference = target - pose.theta;
  
  Serial.print("Angle Difference: "); Serial.println(angle_difference);
  Serial.print("Target Angle: "); Serial.println(target);
  Serial.print("Current Pose Angle: "); Serial.println(pose.theta);

  if (abs(angle_difference) < turn_threshold) {
    motors.setPWM(0,0);
    Serial.println("Turn Completed!");
    return true;
  }
  return false;
}
