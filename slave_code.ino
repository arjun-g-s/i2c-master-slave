#include <Wire.h>

// ---------- CONFIG ----------
#define I2C_ADDR 0x08  // Arduino I2C slave address

// Define your sensors
const int numUltrasonic = 2;
const int trigPins[numUltrasonic] = {2, 4};
const int echoPins[numUltrasonic] = {3, 5};

const int numIR = 2;
const int irPins[numIR] = {6, 7};

const int numContact = 2;
const int contactPins[numContact] = {8, 9};

byte sensorData[32]; // Adjust buffer size

// ---------- SETUP ----------
void setup() {
  Wire.begin(I2C_ADDR);
  Wire.onRequest(sendData);
  Serial.begin(115200);

  for (int i = 0; i < numUltrasonic; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  for (int i = 0; i < numIR; i++) pinMode(irPins[i], INPUT);
  for (int i = 0; i < numContact; i++) pinMode(contactPins[i], INPUT);
}

// ---------- ULTRASONIC FUNCTION ----------
float readUltrasonic(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2.0; // cm
}

// ---------- SEND DATA ----------
void sendData() {
  int idx = 0;

  // Ultrasonic
  for (int i = 0; i < numUltrasonic; i++) {
    int val = (int)(readUltrasonic(trigPins[i], echoPins[i]) * 10); // one decimal
    sensorData[idx++] = (val >> 8) & 0xFF;
    sensorData[idx++] = val & 0xFF;
  }

  // IR
  for (int i = 0; i < numIR; i++) sensorData[idx++] = digitalRead(irPins[i]);

  // Contact
  for (int i = 0; i < numContact; i++) sensorData[idx++] = digitalRead(contactPins[i]);

  Wire.write(sensorData, idx);
}

void loop() {
  delay(100); // loop runs continuously, data is sent on request
}
