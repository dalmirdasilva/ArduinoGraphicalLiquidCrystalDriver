#include <Wire.h>
#include <Closeable.h>
#include <Seekable.h>
#include <InputStream.h>
#include <SeekableInputStream.h>
#include <ExternalEepromInputStream.h> 
#include <ExternalEepromSeekableInputStream.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <GraphicLiquidCrystal.h>
#include <GraphicLiquidCrystalPoint.h>
#include <GraphicLiquidCrystalRectangle.h>
#include <GraphicLiquidCrystalStraight.h>
#include <GraphicLiquidCrystalGraphicState.h>
#include <GraphicLiquidCrystalDrawer.h>
#include <GraphicLiquidCrystalShapes.h>
#include <GraphicLiquidCrystalText.h>
#include <GraphicLiquidCrystalBitmapFont.h>

void setup() {
  Serial.begin(9600);
  uint16_t i, j;

  Serial.println("Are you sure to continue? (y/n)");
  while (!Serial.available());
  while (Serial.read() != 'y');

  External24cl256Eeprom ee(0x50);
  ExternalEepromSeekableInputStream eeis(&ee);
    
  GraphicLiquidCrystalGraphicState graphicState;
  GraphicLiquidCrystalStraight glcdDriver;
  GraphicLiquidCrystalDrawer glcdDrawer(&glcdDriver, &graphicState);
    
  glcdDriver.init(GraphicLiquidCrystal::MODE_ON);
  glcdDriver.screen(0x00);
  graphicState.setColor(GraphicLiquidCrystal::COLOR_WHITE);

  GraphicLiquidCrystalBitmapFont font(&eeis);
  GraphicLiquidCrystalText glcdText(&glcdDriver, &font, &graphicState);
    
  glcdDrawer.line(10, 10, 20, 10);
  glcdDrawer.line(20, 10, 30, 20);
  glcdDrawer.line(30, 20, 20, 20);
  glcdDrawer.line(20, 20, 10, 10);

  GraphicLiquidCrystalRectangle rec(30, 30, 100, 60);
  glcdText.printString(&rec, (unsigned char*)"#{Testing...} @OK!", 100, 1);
  
  eeis.close();
  Serial.print("Finished at: ");
  Serial.println(millis());
}

void loop() {
}
