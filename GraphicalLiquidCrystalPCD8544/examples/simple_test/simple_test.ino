////#include <Arduino.h>
////#include <Wire.h>
////#include <WiredDevice.h>
////#include <RegisterBasedWiredDevice.h>
////#include <Accelerometer.h>
////#include <AccelerometerMMA8451.h>
////
////AccelerometerMMA8451 acc(0);
////
////void setup() {
////    Serial.begin(9600);
////    acc.standby();
////    acc.setDynamicRange(AccelerometerMMA8451::DR_4G);
////    acc.activate();
////}
////
////void loop() {
////    Serial.print("x: ");
////    Serial.println(acc.readXg());
////    Serial.print("y: ");
////    Serial.println(acc.readYg());
////    Serial.print("z: ");
////    Serial.println(acc.readZg());
////    Serial.println("-----------");
////    delay(1000);
////}
////#include <Wire.h>
////#include <WiredDevice.h>
////#include <RegisterBasedWiredDevice.h>
////#include <Accelerometer.h>
////#include <AccelerometerMPU9250.h>
////
////AccelerometerMPU9250 acc(0);
////
////void setup() {
////    Serial.begin(9600);
////    acc.setFullScaleRange(AccelerometerMPU9250::FS_SEL_8G);
////}
////
////void loop() {
////    Serial.print("x: ");
////    Serial.println(acc.readXg());
////    Serial.print("y: ");
////    Serial.println(acc.readYg());
////    Serial.print("z: ");
////    Serial.println(acc.readZg());
////    Serial.println("-----------");
////    delay(1000);
////}
////
////#include <MAX7219Driver.h>
////#include <LedMatrixDriver.h>
////#include <LedMatrixMAX7219Driver.h>
////
////void setup() {
////
////    Serial.begin(9600);
////    Serial.println("Ready.");
////    randomSeed(analogRead(0));
////
////    MAX7219Driver driver(12, 11, 10);
////    LedMatrixMAX7219Driver matrix(&driver, 8, 8);
////
////    char row, col;
////    while (true) {
////        row = random(0, 7);
////        col = random(0, 7);
////        matrix.setLed(row, col, 1);
////        delay(100);
////        matrix.setLed(row, col, 0);
////        delay(100);
////    }
////}
////
////void loop() {
////}
//
//#include <Arduino.h>
//#include <RegisterBasedSPIDevice.h>
//#include <ReaderMFRC522.h>
//
//#define SS_PIN      10
//#define RST_PIN     3
//#define LED_PIN     A1
//
//RegisterBasedSPIDevice device(SS_PIN);
//ReaderMFRC522 reader(&device, RST_PIN);
//
//void setup() {
//    Serial.begin(9600);
//    pinMode(LED_PIN, OUTPUT);
//    digitalWrite(LED_PIN, LOW);
//
//    Serial.println("Initializing...");
//
//    reader.initialize();
//
//    Serial.print("Version: ");
//    switch (reader.getVersion()) {
//    case ReaderMFRC522::CLONE:
//        Serial.println("clone");
//        break;
//    case ReaderMFRC522::V0_0:
//        Serial.println("v0_0");
//        break;
//    case ReaderMFRC522::V1_0:
//        Serial.println("v1_0");
//        break;
//    case ReaderMFRC522::V2_0:
//        Serial.println("v2_0");
//        break;
//    default:
//        Serial.println("unknown");
//        return;
//    }
//
//    Serial.print("Self test: ");
//    if (reader.performSelfTest()) {
//        digitalWrite(LED_PIN, HIGH);
//        Serial.println("Passed!");
//    } else {
//        Serial.println("**failed**");
//    }
//
//    Serial.println("Done");
//}
//
//void loop() {
//}

#include <Arduino.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalPCD8544.h>

#define DATA_PIN            6
#define CLOCK_PIN           7
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

void setup() {
  unsigned long start;
  Serial.begin(9600);

  Serial.println("Are you sure to continue? (y/n)");
  while (!Serial.available());
  while (Serial.read() != 'y');

  GraphicalLiquidCrystalPCD8544 glcd(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);

  glcd.init(GraphicalLiquidCrystalPCD8544::MODE_ON);
  glcd.screen(0x00);

  start = millis();
  for (int i = 0; i < 256; i++) {
      glcd.screen(i);
  }

  Serial.println(millis() - start);
}

void loop() {
}
