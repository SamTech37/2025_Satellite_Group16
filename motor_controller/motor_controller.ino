#include <Servo.h>

Servo servo1;
Servo servo2;

const int ldrPin = A0;  // LDR 接在 A0
int lightValue;
int lv_max = 1023, lv_min = 250;

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
}


void light_to_angle(){
  lightValue = analogRead(ldrPin);  // 讀取光線值（範圍：0～1023）

  // 假設光越強 → 讀到的電阻值越小，轉越大角度
   
  int angle = map(lightValue, lv_min, lv_max, 180, 0);  // 映射成角度
  angle = constrain(angle, 0, 180); // clipping, ensure angle is in [0,180] 
  servo1.write(angle);
  servo2.write(180 - angle);  // 相反方向轉動
  

  // log for debugging
  Serial.print("Light = ");
  Serial.println(lightValue);

  Serial.print("Angle 1 = ");
  Serial.println(angle);

  delay(1000);  // delay 1000 ms for testing
}

void loop() {

  // test_servos();
  light_to_angle();
 
}
