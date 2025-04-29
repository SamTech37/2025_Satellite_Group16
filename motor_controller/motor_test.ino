

void test_servos() {
  servo1.write(0);   // 轉到 0 度
  servo2.write(0);
  delay(1000);

  servo1.write(90);  // 轉到 90 度
  servo2.write(90);
  delay(1000);

  servo1.write(180); // 轉到 180 度
  servo2.write(180);
  delay(1000);
}
