#include <Wire.h>

#define I2C_ADDR 0x08
#define NUM_ULTRASONIC 2
#define NUM_IR 2
#define NUM_CONTACT 2

void setup() {
  Wire.begin();      // ESP32 as I2C master
  Serial.begin(115200);
}

void loop() {
  byte buffer[32];
  int bytes = Wire.requestFrom(I2C_ADDR, 16); // adjust according to number of sensors
  int idx = 0;
  while (Wire.available()) buffer[idx++] = Wire.read();

  idx = 0;
  String output = "";

  // Ultrasonic
  for (int i = 0; i < NUM_ULTRASONIC; i++) {
    int val = (buffer[idx] << 8) | buffer[idx + 1];
    float dist = val / 10.0;
    output += "U" + String(i) + ":" + String(dist, 1) + ";";
    idx += 2;
  }

  // IR
  for (int i = 0; i < NUM_IR; i++) {
    output += "IR" + String(i) + ":" + String(buffer[idx]) + ";";
    idx++;
  }

  // Contact
  for (int i = 0; i < NUM_CONTACT; i++) {
    output += "C" + String(i) + ":" + String(buffer[idx]) + ";";
    idx++;
  }

  Serial.println(output); // send to PC ROS node
  delay(100); // 10 Hz
}
