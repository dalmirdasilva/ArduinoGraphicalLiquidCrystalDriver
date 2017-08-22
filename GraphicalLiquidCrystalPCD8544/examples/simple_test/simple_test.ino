
#include <Arduino.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalPCD8544.h>

#define DATA_PIN            7
#define CLOCK_PIN           6
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

void setup() {
  unsigned long start;
  Serial.begin(9600);

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
