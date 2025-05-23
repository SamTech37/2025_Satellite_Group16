#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

#define SERVOPINH 5
#define SERVOPINV 6

Servo horizontal, vertical;
int servoh = 90, servohLimitHigh = 180, servohLimitLow = 0;
int servov = 120, servovLimitHigh = 180, servovLimitLow = 90;

const int ldrPins[4] = {A0, A1, A2, A3};

LiquidCrystal_PCF8574 lcd(0x27);   // 改成你實際的 I2C 位址

int tol = 100;     // 靈敏度
int dtime = 100;   // 反應間隔

void setup() {
  Serial.begin(9600);

  horizontal.attach(SERVOPINH);
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);

  lcd.begin(16, 2);
  lcd.setBacklight(255);           // ★ 開背光
  lcd.print("LDR Value:");
}

void loop() {
  // 讀光敏電阻
  int ldr[4];
  for (int i = 0; i < 4; i++) ldr[i] = analogRead(ldrPins[i]);

  // 平均值
  int avt = (ldr[0] + ldr[1]) >> 1;
  int avd = (ldr[2] + ldr[3]) >> 1;
  int avl = (ldr[0] + ldr[2]) >> 1;
  int avr = (ldr[1] + ldr[3]) >> 1;
  int veg = (avt + avd + avl + avr) >> 2;

  // 顯示 veg
  lcd.clear();  // 清除顯示內容
  lcd.setCursor(0, 1);
  lcd.print("veg:");
  lcd.print(veg);
  Serial.print("veg= ");Serial.println(veg);
  Serial.print("tol= ");Serial.println(tol);
  Serial.print("dtime= ");Serial.println(dtime);

  // 依亮度自動調靈敏度
  if (veg > 0 && veg < 300) {
    tol   = map(veg, 10, 300, 5, 100);
    dtime = map(veg, 10, 300, 100, 50);
  } else {
    tol = 50; dtime = 50;
  }

  // 伺服控制
  int dvert  = avt - avd;
  int dhoriz = avl - avr;

  if (abs(dvert) > tol) {
    servov += (dvert > 0 ? 1 : -1);
    servov = constrain(servov, servovLimitLow, servovLimitHigh);
    vertical.write(servov);
  }

  if (abs(dhoriz) > tol) {
    servoh += (dhoriz < 0 ? -1 : 1);
    servoh = constrain(servoh, servohLimitLow, servohLimitHigh);
    horizontal.write(servoh);
  }

  delay(dtime);
}
