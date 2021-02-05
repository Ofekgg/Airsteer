/*
 * NOTE: This sketch file is for use with Arduino Leonardo and Arduino Micro only.
 * 
 * Code build by Ofek Golan
 * Joystick from Matthew Heironimus's Arduino Joystick Library https://github.com/MHeironimus/ArduinoJoystickLibrary
 * Jeff Rowberg's I2Cdev Library https://github.com/jrowberg/i2cdevlib
 *
 */
 
#include <Joystick.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Smoothed.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  8, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  true, true, true);  // Accelerator, Brake, and Steering


MPU6050 accelgyro; 
int16_t ax, ay, az, gx, gy, gz;           

//  A1 - Gas, A2 - Brake.
int Button1 = 4;  // 
int Button2 = 5;  // 
int Button3 = 6;  // 
int Button4 = 7;  // 
int Button5 = 8;  // 
int Button6 = 9;  // 
int Button7 = 10;  // 
int Button8 = 16;  // 

int Steering;
int Gas;
int Brake;

Smoothed <int> Steer; 


const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void Moving()
{
  
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Steering = ay;
  Steering = Steering/100;
  Steering = constrain(Steering, -160, 160);
  Steer.add(Steering);
  Steering = Steer.get();
  Steering = constrain(Steering, -160, 160);
  Steering = map(Steering, -160, 160, 0, 1024);
}

void setup() {
  // Initialize Button Pins
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  pinMode(Button5, INPUT_PULLUP);
  pinMode(Button6, INPUT_PULLUP);
  pinMode(Button7, INPUT_PULLUP);
  pinMode(Button8, INPUT_PULLUP);
  
  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setSteeringRange(0, 1024);
  Joystick.setAcceleratorRange(0, 1024);  // ADC min-Max
  Joystick.setBrakeRange(0, 1024);  // ADC min - Max

  //MPU6050
  Wire.begin();
  accelgyro.initialize();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //Steering smoothing and EXPO
  
  //Steer.begin(SMOOTHED_AVERAGE, 10);  // Initialise the first sensor value store to be the simple average of the last 10 values.
  Steer.begin(SMOOTHED_EXPONENTIAL, 20);  //Higher numbers will result in less filtering/smoothing. Lower number result in more filtering/smoothing
}

void loop() 
{
  // Read Axis
  Moving();
  Gas = analogRead(A1);
  Gas = map(Gas, 866, 563, 0, 1024);
  Gas = constrain(Gas, 0, 1024);
  Brake = analogRead(A2);
  Brake = map(Brake, 865, 565, 0, 1024);
  Brake = constrain(Brake, 0, 1024);

  //Send values
  Joystick.setAccelerator(Gas);
  Joystick.setBrake(Brake);
  Joystick.setSteering(Steering);

  // Buttons
  Joystick.setButton(0, !digitalRead(Button1));
  Joystick.setButton(1, !digitalRead(Button2));
  Joystick.setButton(2, !digitalRead(Button3));
  Joystick.setButton(3, !digitalRead(Button4));
  Joystick.setButton(4, !digitalRead(Button5));
  Joystick.setButton(5, !digitalRead(Button6));
  Joystick.setButton(6, !digitalRead(Button7));
  Joystick.setButton(7, !digitalRead(Button8));
  
  delay(5);
}
