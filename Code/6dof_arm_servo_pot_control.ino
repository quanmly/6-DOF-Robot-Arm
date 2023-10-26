// Code to control 6 DOF robot arm with 6 different pots

// things to include
#include <Servo.h>
#include <Smoothed.h>

// create time constants
const unsigned long timeinterval = 15;
unsigned long timepast = 0;
unsigned long smoothconst = 40;

// create servo objects with names
Servo servo_waist;
Servo servo_shoulder;
Servo servo_elbow;
Servo servo_wristpitch;
Servo servo_wristroll;
Servo servo_claw;

// initialize potentiometers
int potpin_waist = 1;
int potpin_shoulder = 2;
int potpin_elbow = 3;
int potpin_wristpitch = 4;
int potpin_wristroll = 5;
int potpin_claw = 6;

int potval_waist_current = 0;

Smoothed <long int> potval_waist;
Smoothed <long int> potval_shoulder;
Smoothed <long int> potval_elbow;
Smoothed <long int> potval_wristpitch;
Smoothed <long int> potval_wristroll;
Smoothed <long int> potval_claw;


void setup() {
  // attach servos to pins
  servo_waist.attach(8);
  servo_shoulder.attach(7);
  servo_elbow.attach(5);
  servo_wristpitch.attach(4);
  servo_wristroll.attach(3);
  servo_claw.attach(2);

  // serial monitor
  Serial.begin(9600);

  // smoothing via moving average
  potval_waist.begin(SMOOTHED_AVERAGE,smoothconst);
  potval_elbow.begin(SMOOTHED_AVERAGE,smoothconst);
  potval_shoulder.begin(SMOOTHED_AVERAGE,smoothconst);
  potval_wristpitch.begin(SMOOTHED_AVERAGE,smoothconst);
  potval_wristroll.begin(SMOOTHED_AVERAGE,smoothconst);
  potval_claw.begin(SMOOTHED_AVERAGE,smoothconst);

}

void loop() {
  // read pot values
  long int potval_waist_current = analogRead(potpin_waist);
  long int potval_shoulder_current = analogRead(potpin_shoulder);
  long int potval_elbow_current = analogRead(potpin_elbow);
  long int potval_wristpitch_current = analogRead(potpin_wristpitch);
  long int potval_wristroll_current = analogRead(potpin_wristroll);
  long int potval_claw_current = analogRead(potpin_claw);

  // add new values
  potval_waist.add(potval_waist_current);
  potval_shoulder.add(potval_shoulder_current);
  potval_elbow.add(potval_elbow_current);
  potval_wristpitch.add(potval_wristpitch_current);
  potval_wristroll.add(potval_wristroll_current);
  potval_claw.add(potval_claw_current);

  // get smoothed values
  long int potval_waist_smoothed = potval_waist.get();
  long int potval_shoulder_smoothed = potval_shoulder.get();
  long int potval_elbow_smoothed = potval_elbow.get();
  long int potval_wristpitch_smoothed = potval_wristpitch.get();
  long int potval_wristroll_smoothed = potval_wristroll.get();
  long int potval_claw_smoothed = potval_claw.get();
  
  // convert values to long int
  long int potval_waist = potval_waist_smoothed;
  long int potval_shoulder = potval_shoulder_smoothed;
  long int potval_elbow = potval_elbow_smoothed;
  long int potval_wristpitch = potval_wristpitch_smoothed;
  long int potval_wristroll = potval_wristroll_smoothed;
  long int potval_claw = potval_claw_smoothed;

  // publish smoothed values
  /*
  Serial.println("waist pot:"); Serial.print("\t"); Serial.print(potval_waist);
  Serial.println("shoulder pot:"); Serial.print("\t"); Serial.print(potval_shoulder);
  Serial.println("elbow pot:"); Serial.print("\t"); Serial.print(potval_elbow);
  Serial.println("wrist pitch pot:"); Serial.print("\t"); Serial.print(potval_wristpitch);
  Serial.println("wrist roll pot:"); Serial.print("\t"); Serial.print(potval_wristroll);
  Serial.println("claw pot:"); Serial.print("\t"); Serial.print(potval_claw);
  */

  // map pot values
  potval_waist = map(potval_waist,0,1023,0,180);
  potval_shoulder = map(potval_shoulder,0,1023,0,180);
  potval_elbow = map(potval_elbow,0,1023,0,180);
  potval_wristpitch = map(potval_wristpitch,0,1023,0,180);
  potval_wristroll = map(potval_wristroll,0,1023,0,180);
  potval_claw = map(potval_claw,0,1023,0,180);

  // timing
  unsigned long timenow = millis();

  // write servo values to servo, use millis to delay
  if(timenow - timepast >= timeinterval){
    servo_waist.write(potval_waist);
    servo_shoulder.write(potval_shoulder);
    servo_elbow.write(potval_elbow);
    servo_wristpitch.write(potval_wristpitch);
    servo_wristroll.write(potval_wristroll);
    servo_claw.write(potval_claw);

    // update timepast
    unsigned long timepast = millis();

  }

}
