
#include <Arduino.h>
#include <ByteArrayInputStream.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalNokia5110.h>
#include <GraphicalLiquidCrystalDrawer.h>
#include <GraphicalLiquidCrystalBitmapRender.h>
#include <GraphicalLiquidCrystalBitmapImage.h>

#define DATA_PIN            7
#define CLOCK_PIN           6
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

unsigned char img[] = { 0x00, 0x08, 0x10, 0x00, 0x30, 0x30, 0x06, 0x06, 0x30, 0x30, 0x00, 0xc0, 0x20, 0x10, 0x08, 0x08,
        0x20, 0x20, 0xc0 };

void setup() {
    Serial.begin(9600);

    ByteArrayInputStream is(img, sizeof(img));

    GraphicalLiquidCrystalNokia5110 glcdDriver(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);

    glcdDriver.init(GraphicalLiquidCrystal::MODE_ON);
    glcdDriver.setContrast(0x3a);
    glcdDriver.screen(0x00);

    GraphicalLiquidCrystalBitmapRender bitmapRender(&glcdDriver);
    GraphicalLiquidCrystalBitmapImage image(&is);
    bitmapRender.drawImageAtRow(&image, 50, 4);
    bitmapRender.drawImage(&image, 0, 0);
}

void loop() {
}
