/* 
 * Code build by Ofek Golan
 * Joystick from Matthew Heironimus's Arduino Joystick Library https://github.com/MHeironimus/ArduinoJoystickLibrary
 * Jeff Rowberg's I2Cdev Library https://github.com/jrowberg/i2cdevlib
 *
 */
 
#include <Joystick.h>
#include <I2Cdev.h>
#include <MPU6050.h>

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  8, 0,                  // Button Count, Hat Switch Count
  false, false, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  true, true, true);  // Accelerator, Brake, and Steering

float elapsedTime, currentTime, previousTime;

MPU6050 accelgyro; 
int16_t ax, ay, az, gx, gy, gz;
int16_t gyroAngleY,accAngleY;

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

void Comp()
{
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;

  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  accAngleY = (atan(ay / sqrt(pow(ax, 2) + pow(az, 2))) * 180 / PI) + 0; //accError "0"

  gyroAngleY = ay + gz * elapsedTime; 
  Steering = 0.98 * gyroAngleY + 0.2 * accAngleY;

  Steering = Steering / 10;
  Steering = constrain(Steering, -1600, 1600);
  if(ax<1000 && ay >15800)
  {
    Steering = 1600;
  }
  if(ax<1000 && ay <-15800)
  {
    Steering = -1600;
  }
  Steering = map(Steering, -1600, 1600, 0, 1024);

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
  Joystick.setAcceleratorRange(0, 1024);
  Joystick.setBrakeRange(0, 1024);

  //MPU6050
  Wire.begin();
  accelgyro.initialize();
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
}

void loop() 
{
  // Read Axis

  Comp(); //steering filter
  
  Gas = analogRead(A1);
  Gas = map(Gas, 866, 563, 0, 1024); // ADC min-Max
  Gas = constrain(Gas, 0, 1024);
  Brake = analogRead(A2);
  Brake = map(Brake, 865, 565, 0, 1024); // ADC min - Max
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
