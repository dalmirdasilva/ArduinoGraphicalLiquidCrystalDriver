//////#include <Arduino.h>
//////#include <Wire.h>
//////#include <WiredDevice.h>
//////#include <RegisterBasedWiredDevice.h>
//////#include <Accelerometer.h>
//////#include <AccelerometerMMA8451.h>
//////
//////AccelerometerMMA8451 acc(0);
//////
//////void setup() {
//////    Serial.begin(9600);
//////    acc.standby();
//////    acc.setDynamicRange(AccelerometerMMA8451::DR_4G);
//////    acc.activate();
//////}
//////
//////void loop() {
//////    Serial.print("x: ");
//////    Serial.println(acc.readXg());
//////    Serial.print("y: ");
//////    Serial.println(acc.readYg());
//////    Serial.print("z: ");
//////    Serial.println(acc.readZg());
//////    Serial.println("-----------");
//////    delay(1000);
//////}
//////#include <Wire.h>
//////#include <WiredDevice.h>
//////#include <RegisterBasedWiredDevice.h>
//////#include <Accelerometer.h>
//////#include <AccelerometerMPU9250.h>
//////
//////AccelerometerMPU9250 acc(0);
//////
//////void setup() {
//////    Serial.begin(9600);
//////    acc.setFullScaleRange(AccelerometerMPU9250::FS_SEL_8G);
//////}
//////
//////void loop() {
//////    Serial.print("x: ");
//////    Serial.println(acc.readXg());
//////    Serial.print("y: ");
//////    Serial.println(acc.readYg());
//////    Serial.print("z: ");
//////    Serial.println(acc.readZg());
//////    Serial.println("-----------");
//////    delay(1000);
//////}
//////
//////#include <MAX7219Driver.h>
//////#include <LedMatrixDriver.h>
//////#include <LedMatrixMAX7219Driver.h>
//////
//////void setup() {
//////
//////    Serial.begin(9600);
//////    Serial.println("Ready.");
//////    randomSeed(analogRead(0));
//////
//////    MAX7219Driver driver(12, 11, 10);
//////    LedMatrixMAX7219Driver matrix(&driver, 8, 8);
//////
//////    char row, col;
//////    while (true) {
//////        row = random(0, 7);
//////        col = random(0, 7);
//////        matrix.setLed(row, col, 1);
//////        delay(100);
//////        matrix.setLed(row, col, 0);
//////        delay(100);
//////    }
//////}
//////
//////void loop() {
//////}
////
////#include <Arduino.h>
////#include <RegisterBasedSPIDevice.h>
////#include <ReaderMFRC522.h>
////
////#define SS_PIN      10
////#define RST_PIN     3
////#define LED_PIN     A1
////
////RegisterBasedSPIDevice device(SS_PIN);
////ReaderMFRC522 reader(&device, RST_PIN);
////
////void setup() {
////    Serial.begin(9600);
////    pinMode(LED_PIN, OUTPUT);
////    digitalWrite(LED_PIN, LOW);
////
////    Serial.println("Initializing...");
////
////    reader.initialize();
////
////    Serial.print("Version: ");
////    switch (reader.getVersion()) {
////    case ReaderMFRC522::CLONE:
////        Serial.println("clone");
////        break;
////    case ReaderMFRC522::V0_0:
////        Serial.println("v0_0");
////        break;
////    case ReaderMFRC522::V1_0:
////        Serial.println("v1_0");
////        break;
////    case ReaderMFRC522::V2_0:
////        Serial.println("v2_0");
////        break;
////    default:
////        Serial.println("unknown");
////        return;
////    }
////
////    Serial.print("Self test: ");
////    if (reader.performSelfTest()) {
////        digitalWrite(LED_PIN, HIGH);
////        Serial.println("Passed!");
////    } else {
////        Serial.println("**failed**");
////    }
////
////    Serial.println("Done");
////}
////
////void loop() {
////}
//
//#include <Arduino.h>
//#include <GraphicalLiquidCrystal.h>
//#include <GraphicalLiquidCrystalPCD8544.h>
//
//#define DATA_PIN            7
//#define CLOCK_PIN           6
//#define RST_PIN             3
//#define DC_PIN              5
//#define SCE_PIN             4
//
//void setup() {
//  unsigned long start;
//  Serial.begin(9600);
//
//  GraphicalLiquidCrystalPCD8544 glcd(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);
//
//  glcd.init(GraphicalLiquidCrystalPCD8544::MODE_ON);
//  glcd.screen(0x00);
//
//  glcd.plot(10, 10, GraphicalLiquidCrystalPCD8544::COLOR_WHITE);
//  delay(1000);
//  glcd.scrollTo(2);
//  delay(1000);
//
//  start = millis();
//  for (int i = 0; i < 256; i++) {
//      glcd.screen(i);
//  }
//
//  Serial.println(millis() - start);
//}
//
//void loop() {
//}
//
//#include <Arduino.h>
//#include <Wire.h>
//#include <Closeable.h>
//#include <Seekable.h>
//#include <InputStream.h>
//#include <SeekableInputStream.h>
//#include <GraphicalLiquidCrystal.h>
//#include <GraphicalLiquidCrystalPoint.h>
//#include <GraphicalLiquidCrystalRectangle.h>
//#include <GraphicalLiquidCrystalGraphicState.h>
//#include <GraphicalLiquidCrystalDrawer.h>
//#include <GraphicalLiquidCrystalText.h>
//#include <GraphicalLiquidCrystalBitmapFont.h>
//#include <GraphicalLiquidCrystalPoint.h>
//
//#include <avr/pgmspace.h>
//#include <Closeable.h>
//#include <Seekable.h>
//#include <InputStream.h>
//#include <SeekableInputStream.h>
//#include <ByteArrayInputStream.h>
//#include <GraphicalLiquidCrystalBitmapFont.h>
//
//#include <GraphicalLiquidCrystal.h>
//#include <GraphicalLiquidCrystalPCD8544.h>
//
//#define DATA_PIN            7
//#define CLOCK_PIN           6
//#define RST_PIN             3
//#define DC_PIN              5
//#define SCE_PIN             4
//
//const unsigned char f[] = { 0x0, 0x5, 0x8, 0x9, 0x20, 0x21, 0x0, 0x28, 0x44, 0x44,
//        0x0, 0x32, 0x53, 0x53, 0x0, 0x37, 0x61, 0x61, 0x0, 0x3c, 0x64, 0x64,
//        0x0, 0x41, 0x69, 0x69, 0x0, 0x46, 0x6c, 0x6d, 0x0, 0x4b, 0x72, 0x72,
//        0x0, 0x55, 0x76, 0x76, 0x0, 0x5a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x60,
//        0xfa, 0x60, 0x0, 0xfe, 0x82, 0x82, 0x82, 0x7c, 0x64, 0x92, 0x92, 0x92,
//        0x4c, 0x4, 0x2a, 0x2a, 0x2a, 0x1e, 0x1c, 0x22, 0x22, 0x22, 0xfe, 0x0,
//        0x0, 0xbe, 0x0, 0x0, 0x0, 0x0, 0xfe, 0x2, 0x0, 0x3e, 0x20, 0x18, 0x20,
//        0x1e, 0x22, 0x1e, 0x22, 0x20, 0x10, 0x38, 0x4, 0x2, 0x4, 0x38 };
//
//void setup() {
//    Serial.begin(9600);
//
//    GraphicalLiquidCrystalPoint pointA(0, 0);
//    GraphicalLiquidCrystalPoint pointB(50, 50);
//
//    ByteArrayInputStream is((unsigned char *)f, sizeof(f));
//
//    GraphicalLiquidCrystalPCD8544 glcdDriver(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);
//
//    GraphicalLiquidCrystalGraphicState graphicState;
//    GraphicalLiquidCrystalDrawer glcdDrawer(&glcdDriver, &graphicState);
//
//    GraphicalLiquidCrystalBitmapFont font(&is);
//    GraphicalLiquidCrystalText glcdText(&glcdDriver, &font, &graphicState);
//
//    glcdDriver.init(GraphicalLiquidCrystal::MODE_ON);
//    glcdDriver.screen(0x00);
//    graphicState.setColor(GraphicalLiquidCrystal::COLOR_WHITE);
//
//////
////    glcdDriver.plot(10, 10, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(11, 11, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(12, 12, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(13, 13, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(14, 14, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(15, 15, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(16, 16, GraphicalLiquidCrystal::COLOR_WHITE);
////    glcdDriver.plot(17, 17, GraphicalLiquidCrystal::COLOR_WHITE);
//
//    for (int i = 0; i < glcdDriver.getWidth(); i += 1) {
//        glcdDrawer.drawLine(0, 0, i, glcdDriver.getHeight() - 1);
//    }
//    for (int i = glcdDriver.getHeight() - 1; i >= 0; i -= 1) {
//        glcdDrawer.drawLine(0, 0, glcdDriver.getWidth() - 1, i);
//    }
////    delay(4000);
////    glcdDriver.screen(0x00);
////
////    GraphicalLiquidCrystalRectangle rec(30, 30, 100, 60);
////    glcdText.printString(&rec, (unsigned char*) "#{Testing...} @OK!", 100, 1);
//
//    is.close();
//    Serial.print("Finished at: ");
//    Serial.println(millis());
//}
//
//void loop() {
//}



#include <Arduino.h>
#include <GraphicalLiquidCrystalGraphicState.h>
#include <GraphicalLiquidCrystalDrawer.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalPCD8544.h>

#define DATA_PIN            7
#define CLOCK_PIN           6
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

void setup() {
    Serial.begin(9600);

    GraphicalLiquidCrystalPCD8544 glcdDriver(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);

    GraphicalLiquidCrystalGraphicState graphicState;
    GraphicalLiquidCrystalDrawer glcdDrawer(&glcdDriver, &graphicState);

    glcdDriver.init(GraphicalLiquidCrystal::MODE_ON);
    glcdDriver.screen(0x00);
    graphicState.setColor(GraphicalLiquidCrystal::COLOR_WHITE);

    unsigned long started = millis();

    for (int i = 0; i < glcdDriver.getWidth(); i += 1) {
        glcdDrawer.drawLine(0, 0, i, glcdDriver.getHeight() - 1);
    }

    for (int i = glcdDriver.getHeight() - 1; i >= 0; i -= 1) {
        glcdDrawer.drawLine(0, 0, glcdDriver.getWidth() - 1, i);
    }

    unsigned long finished = millis();
    Serial.print("It took: ");
    Serial.println(finished - started);
}

void loop() {
}
