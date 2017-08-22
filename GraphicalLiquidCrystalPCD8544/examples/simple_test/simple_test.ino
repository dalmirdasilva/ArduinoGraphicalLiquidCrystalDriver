#include <Arduino.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalPCD8544.h>

#define MOSI_PIN            7
#define CLOCK_PIN           6
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

void setup() {
  Serial.begin(9600);

  GraphicalLiquidCrystalPCD8544 glcd(MOSI_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);

  glcd.init(GraphicalLiquidCrystalPCD8544::MODE_ON);
  glcd.screen(0x00);

  unsigned long started = millis();
  for (int i = 0; i < 256; i++) {
      glcd.screen(i);
  }
  unsigned long finished = millis();

  Serial.print("It took (ms): ");
  Serial.println(finished - started);
}

void loop() {
}
