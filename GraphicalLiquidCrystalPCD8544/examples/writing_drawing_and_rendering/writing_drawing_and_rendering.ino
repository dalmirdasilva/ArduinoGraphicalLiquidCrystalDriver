#include <Wire.h>
#include <Closeable.h>
#include <Seekable.h>
#include <InputStream.h>
#include <SeekableInputStream.h>
#include <ExternalEepromInputStream.h> 
#include <ExternalEepromSeekableInputStream.h>
#include <ByteArrayInputStream.h>
#include <ByteArraySeekableInputStream.h>
#include <ExternalEeprom.h>
#include <External24cl256Eeprom.h>
#include <Glcd.h>
#include <GlcdStraight.h>
#include <GlcdGraphicState.h>
#include <GlcdDrawer.h>
#include <GlcdShapes.h>
#include <GlcdText.h>
#include <GlcdBitmapFont.h>
#include <GlcdBitmapImage.h>
#include <GlcdBitmapRender.h>

unsigned char img[] = {0x00, 0x08, 0x10,
0x00, 0x30, 0x30, 0x06, 0x06, 0x30, 0x30, 0x00,
0xc0, 0x20, 0x10, 0x08, 0x08, 0x20, 0x20, 0xc0};

void setup() {
  Serial.begin(9600);
  uint16_t i, j;

  Serial.println("Sure? (y/n)");
  while (!Serial.available());
  while (Serial.read() != 'y');

  External24cl256Eeprom ee(0x50);
  ExternalEepromSeekableInputStream eeis(&ee);
  ByteArraySeekableInputStream basis(img, sizeof(img));	
    
  GlcdGraphicState graphicState;
  GlcdStraight glcdDriver;
  GlcdDrawer glcdDrawer(&glcdDriver, &graphicState);
    
  glcdDriver.init(Glcd::MODE_ON);
  glcdDriver.screen(0x00);
  graphicState.setColor(Glcd::COLOR_WHITE);

  GlcdBitmapRender bitmapRender(&glcdDriver);
  GlcdBitmapImage image(&basis);
  bitmapRender.drawImageAtRow(&image, 100, 4);
  bitmapRender.drawImage(&image, 50, 32);

  GlcdBitmapFont font(&eeis);
  GlcdText glcdText(&glcdDriver, &font, &graphicState);
    
  glcdDrawer.line(10, 10, 20, 10);
  glcdDrawer.line(20, 10, 30, 20);
  glcdDrawer.line(30, 20, 20, 20);
  glcdDrawer.line(20, 20, 10, 10);
  glcdDrawer.circle(40, 20, 15);

  Rectangle rec(0, 0, 100, 40);
  glcdText.printString(&rec, (unsigned char*)"All in!", 100, 1);
  
  eeis.close();
  basis.close();
  Serial.print("Finished at: ");
  Serial.println(millis());
}

void loop() {
}
