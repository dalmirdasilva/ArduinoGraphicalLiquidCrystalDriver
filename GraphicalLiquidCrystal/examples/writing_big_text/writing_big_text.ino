#include <Wire.h>
#include <Closeable.h>
#include <Seekable.h>
#include <InputStream.h>
#include <SeekableInputStream.h>
#include <ExternalEepromInputStream.h> 
#include <ExternalEepromSeekableInputStream.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <Glcd.h>
#include <GlcdStraight.h>
#include <GlcdGraphicState.h>
#include <GlcdDrawer.h>
#include <GlcdShapes.h>
#include <GlcdText.h>
#include <GlcdSimpleText.h>
#include <GlcdBitmapFont.h>

void setup() {
  Serial.begin(9600);

  Serial.println("Sure? (y/n)");
  while (!Serial.available());
  while (Serial.read() != 'y');

  Serial.print("Started at: ");
  Serial.println(millis());

  External24cl256Eeprom ee(0x50);
  ExternalEepromSeekableInputStream eeis(&ee);
    
  GlcdGraphicState graphicState;
  GlcdStraight glcdDriver;
    
  glcdDriver.init(Glcd::MODE_ON);
  glcdDriver.screen(0x00);
  graphicState.setColor(Glcd::COLOR_WHITE);
  graphicState.setLeading(1);
  graphicState.setSpace(1);

  GlcdBitmapFont font(&eeis);
  GlcdText glcdText(&glcdDriver, &font, &graphicState);

  Rectangle rec(0, 10, 40, 63);
  glcdText.printString(&rec, (unsigned char*)"Lorem ipsum dolor sit amet, consectetur adipiscing elit.", 100, 1);

  graphicState.setLeading(3);
  graphicState.setSpace(3);
  Rectangle rec0(41, 5, 70, 63);
  glcdText.printString(&rec0, (unsigned char*)"Integer et felis sit amet felis varius auctor. In et metus est.", 100, 1);

  graphicState.setLeading(1);
  graphicState.setSpace(1);
  Rectangle rec2(71, 0, 127, 63);
  glcdText.printString(&rec2, (unsigned char*)"Morbi pulvinar", 100, 2);

  Serial.print("Finished at: ");
  Serial.println(millis());
}

void loop() {
}
