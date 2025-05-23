#include <Servo.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"

#define SERVOPINH 5
#define SERVOPINV 6

Servo horizontal, vertical;
int servoh = 90, servohLimitHigh = 180, servohLimitLow = 0;
int servov = 120, servovLimitHigh = 180, servovLimitLow = 90;

const int ldrPins[4] = {A0, A1, A2, A3};
int ldr[4]; // LDR readings

LiquidCrystal_PCF8574 lcd(0x27);   // Change to your actual I2C address

int tol = 100;     // Sensitivity threshold
int dtime = 100;   // Response delay time
int veg; // Average LDR value

void initializeServos() {
  horizontal.attach(SERVOPINH);
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
}

void initializeLCD() {
  lcd.begin(16, 2);
  lcd.setBacklight(255);           // Turn on backlight
  lcd.print("LDR Value:");
}

void readLDRs() {
  for (int i = 0; i < 4; i++) {
    ldr[i] = analogRead(ldrPins[i]);
  }
}

void calculateAverages() {
  int avt = (ldr[0] + ldr[1]) >> 1; // Average of top LDRs
  int avd = (ldr[2] + ldr[3]) >> 1; // Average of bottom LDRs
  int avl = (ldr[0] + ldr[2]) >> 1; // Average of left LDRs
  int avr = (ldr[1] + ldr[3]) >> 1; // Average of right LDRs
  veg = (avt + avd + avl + avr) >> 2; // Overall average
}

void displayLCD() {
  lcd.clear();  // Clear display content
  lcd.setCursor(0, 1);
  lcd.print("veg:");
  lcd.print(veg);
}

void printSerial() {
  Serial.print("veg= "); Serial.println(veg);
  Serial.print("tol= "); Serial.println(tol);
  Serial.print("dtime= "); Serial.println(dtime);
}

void adjustSensitivity() {
  // Adjust sensitivity automatically based on brightness
  if (veg > 0 && veg < 300) {
    tol   = map(veg, 10, 300, 5, 100);
    dtime = map(veg, 10, 300, 100, 50);
  } else {
    tol = 50; 
    dtime = 50;
  }
}

void controlServos() {
  // Recalculate averages or pass them as parameters if preferred
  int avt = (ldr[0] + ldr[1]) >> 1; 
  int avd = (ldr[2] + ldr[3]) >> 1; 
  int avl = (ldr[0] + ldr[2]) >> 1; 
  int avr = (ldr[1] + ldr[3]) >> 1; 

  int dvert  = avt - avd; // Difference between top and bottom LDRs
  int dhoriz = avl - avr; // Difference between left and right LDRs

  // Vertical servo control
  if (abs(dvert) > tol) {
    servov += (dvert > 0 ? 1 : -1);
    servov = constrain(servov, servovLimitLow, servovLimitHigh);
    vertical.write(servov);
  }

  // Horizontal servo control
  if (abs(dhoriz) > tol) {
    servoh += (dhoriz < 0 ? -1 : 1); // Note: dhoriz logic might need inversion depending on LDR placement
    servoh = constrain(servoh, servohLimitLow, servohLimitHigh);
    horizontal.write(servoh);
  }
}

void setup() {
  Serial.begin(9600);
  initializeServos();
  initializeLCD();
}

void loop() {
  readLDRs();
  calculateAverages();
  displayLCD();
  printSerial();
  adjustSensitivity();
  controlServos();
  delay(dtime);
}