
#include <Arduino.h>
#include <GraphicalLiquidCrystalGraphicState.h>
#include <GraphicalLiquidCrystalDrawer.h>
#include <GraphicalLiquidCrystal.h>
#include <GraphicalLiquidCrystalNokia5110.h>
#include <ByteArrayInputStream.h>
#include <GraphicalLiquidCrystalBitmapFont.h>
#include <GraphicalLiquidCrystalText.h>

#define DATA_PIN            7
#define CLOCK_PIN           6
#define RST_PIN             3
#define DC_PIN              5
#define SCE_PIN             4

// https://github.com/dalmirdasilva/GlcdBitmapFontCreator (MSB)
const unsigned char f[] = { 0x0, 0x5, 0x8, 0x1, 0x20, 0x7e, 0x0, 0x8,

        // body
        0x0, 0x0, 0x0, 0x0, 0x0, 0x54, 0x0, 0x54, 0x54, 0x54, 0x3c, 0x42, 0x4a, 0x4c, 0x2e, 0x60, 0x0, 0x78, 0x0, 0x7e,
        0x60, 0x0, 0x78, 0x0, 0x0, 0x60, 0x0, 0x0, 0x0, 0x0, 0x4, 0x12, 0x4a, 0x12, 0x4, 0x0, 0x10, 0x48, 0x10, 0x0,
        0x0, 0x0, 0x40, 0x0, 0x0, 0x38, 0x44, 0x4e, 0x44, 0x38, 0x8, 0x7c, 0x1e, 0x7c, 0x8, 0x18, 0x18, 0x7e, 0x3c,
        0x18, 0x8, 0xc, 0x7e, 0xc, 0x8, 0x10, 0x30, 0x7e, 0x30, 0x10, 0x18, 0x3c, 0x7e, 0x18, 0x18, 0x0, 0x7e, 0x0,
        0x7e, 0x0, 0x0, 0x7e, 0x3c, 0x18, 0x0, 0x7e, 0x18, 0x7e, 0x3c, 0x18, 0x0, 0x3c, 0x3c, 0x3c, 0x0, 0x18, 0x3c,
        0x7e, 0x18, 0x7e, 0x38, 0x38, 0x4c, 0x46, 0x38, 0x1c, 0x1c, 0x32, 0x62, 0x1c, 0x3c, 0x4a, 0x4e, 0x42, 0x3c,
        0x7e, 0x42, 0x4e, 0x4c, 0x78, 0x2e, 0x4c, 0x4a, 0x42, 0x3c, 0x2a, 0x54, 0x54, 0x54, 0x2a, 0x5e, 0x0, 0x5e, 0x0,
        0x5e, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x7e, 0x42, 0x72, 0x32, 0x1e, 0x7e, 0x42,
        0x62, 0x42, 0x7e, 0x3e, 0x77, 0x63, 0x77, 0x3e, 0x3e, 0x77, 0x77, 0x77, 0x3e, 0x3e, 0x6b, 0x77, 0x6b, 0x3e,
        0x3e, 0x77, 0x6f, 0x73, 0x3e, 0x3e, 0x7f, 0x45, 0x7f, 0x3e, 0x1c, 0x3e, 0x3e, 0x3e, 0x1c, 0x54, 0x54, 0x44,
        0x44, 0x40, 0x44, 0x54, 0x54, 0x54, 0x44, 0x40, 0x44, 0x44, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x40,
        0x2c, 0x12, 0x12, 0xc, 0x58, 0x5c, 0x5e, 0x5c, 0x58, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0x40, 0x40, 0x40, 0x40,
        0x7f, 0x2, 0x4, 0x2, 0x7f, 0x7f, 0x2, 0x4, 0x8, 0x7f, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x7f, 0x9, 0x9, 0x9, 0x6,
        0x3e, 0x41, 0x51, 0x21, 0x5e, 0x7f, 0x9, 0x9, 0x19, 0x66, 0x26, 0x49, 0x49, 0x49, 0x32, 0x1, 0x1, 0x7f, 0x1,
        0x1, 0x3f, 0x40, 0x40, 0x40, 0x3f, 0x1f, 0x20, 0x40, 0x20, 0x1f, 0x3f, 0x40, 0x3c, 0x40, 0x3f, 0x63, 0x14, 0x8,
        0x14, 0x63, 0x7, 0x8, 0x70, 0x8, 0x7, 0x71, 0x49, 0x45, 0x43, 0x0, 0x0, 0x7f, 0x41, 0x41, 0x0, 0x2, 0x4, 0x8,
        0x10, 0x20, 0x0, 0x41, 0x41, 0x7f, 0x0, 0x4, 0x2, 0x1, 0x2, 0x4, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0, 0x3, 0x7,
        0x0, 0x0, 0x20, 0x54, 0x54, 0x54, 0x78, 0x7f, 0x44, 0x44, 0x44, 0x38, 0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44,
        0x44, 0x44, 0x7f, 0x38, 0x54, 0x54, 0x54, 0x18, 0x8, 0x7e, 0x9, 0x9, 0x0, 0x18, 0xa4, 0xa4, 0xa4, 0x7c, 0x7f,
        0x4, 0x4, 0x78, 0x0, 0x0, 0x0, 0x7d, 0x0, 0x0, 0x40, 0x80, 0x84, 0x7d, 0x0, 0x7f, 0x10, 0x28, 0x44, 0x0, 0x0,
        0x0, 0x7f, 0x40, 0x0, 0x7c, 0x4, 0x18, 0x4, 0x78, 0x7c, 0x4, 0x4, 0x78, 0x0, 0x38, 0x44, 0x44, 0x44, 0x38, 0xfc,
        0x44, 0x44, 0x44, 0x38, 0x38, 0x44, 0x44, 0x44, 0xfc, 0x44, 0x78, 0x44, 0x4, 0x8, 0x8, 0x54, 0x54, 0x54, 0x20,
        0x4, 0x3e, 0x44, 0x24, 0x0, 0x3c, 0x40, 0x20, 0x7c, 0x0, 0x1c, 0x20, 0x40, 0x20, 0x1c, 0x3c, 0x60, 0x30, 0x60,
        0x3c, 0x6c, 0x10, 0x10, 0x6c, 0x0, 0x9c, 0xa0, 0x60, 0x3c, 0x0, 0x64, 0x54, 0x54, 0x4c, 0x0, 0x8, 0x3e, 0x41,
        0x41, 0x0, 0x0, 0x0, 0x7f, 0x0, 0x0, 0x0, 0x41, 0x41, 0x3e, 0x8, 0x2, 0x1, 0x2, 0x1, 0x0 };

#define GLYPHICONS_BURGER_MENU              '!'
#define GLYPHICONS_SPIN_CLOCKWISE           '"'
#define GLYPHICONS_SIGNAL_LEVEL_HIGH        '#'
#define GLYPHICONS_SIGNAL_LEVEL_MEDIUM      '$'
#define GLYPHICONS_SIGNAL_LEVEL_LOW         '%'
#define GLYPHICONS_ANTENA_LEVEL_HIGH        '&'
#define GLYPHICONS_ANTENA_LEVEL_MEDIUM      '\''
#define GLYPHICONS_ANTENA_LEVEL_LOW         '('
#define GLYPHICONS_POWER                    ')'
#define GLYPHICONS_HOME                     '*'
#define GLYPHICONS_ARROW_RIGHT              '+'
#define GLYPHICONS_ARROW_UP                 ','
#define GLYPHICONS_ARROW_DOWN               '-'
#define GLYPHICONS_ARROW_LEFT               '.'
#define GLYPHICONS_PAUSE                    '/'
#define GLYPHICONS_PLAY                     '0'
#define GLYPHICONS_FORWARD                  '1'
#define GLYPHICONS_STOP                     '2'
#define GLYPHICONS_REWIND                   '3'
#define GLYPHICONS_THUMBS_UP                '4'
#define GLYPHICONS_THUMBS_DOWN              '5'
#define GLYPHICONS_CLOCK                    '6'
#define GLYPHICONS_FILE                     '7'
#define GLYPHICONS_SPIN_COUNTERCLOCKWISE    '8'
#define GLYPHICONS_DATABASE                 '9'
#define GLYPHICONS_BARCODE                  ':'
#define GLYPHICONS_PLAID_ODD                ';'
#define GLYPHICONS_PLAID_EVEN               '<'
#define GLYPHICONS_NOTE                     '='
#define GLYPHICONS_DEVICE                   '>'
#define GLYPHICONS_PLUS_SIGN                '?'
#define GLYPHICONS_MINUS_SIGN               '@'
#define GLYPHICONS_REMOVE_SIGN              'A'
#define GLYPHICONS_OK_SIGN                  'B'
#define GLYPHICONS_INFO_SIGN                'C'
#define GLYPHICONS_DOT                      'D'
#define GLYPHICONS_ALIGN_LEFT               'E'
#define GLYPHICONS_ALIGN_CENTER             'F'
#define GLYPHICONS_ALIGN_RIGHT              'G'
#define GLYPHICONS_JUSTIFY                  'H'
#define GLYPHICONS_SEARCH                   'I'
#define GLYPHICONS_EJECT                    'J'

void setup() {
    Serial.begin(9600);

    ByteArrayInputStream is((unsigned char *) f, sizeof(f));

    GraphicalLiquidCrystalNokia5110 glcdDriver(DATA_PIN, CLOCK_PIN, RST_PIN, DC_PIN, SCE_PIN);

    GraphicalLiquidCrystalGraphicState graphicState;
    GraphicalLiquidCrystalDrawer glcdDrawer(&glcdDriver, &graphicState);

    GraphicalLiquidCrystalRectangle rec(0, 0, glcdDriver.getWidth() - 1, glcdDriver.getHeight() - 1);
    GraphicalLiquidCrystalBitmapFont font(&is);
    GraphicalLiquidCrystalText glcdText(&glcdDriver, &font, &graphicState);

    glcdDriver.init(GraphicalLiquidCrystal::MODE_ON);
    glcdDriver.screen(0x00);
    glcdDriver.setContrast(0x3a);
    graphicState.setColor(GraphicalLiquidCrystal::COLOR_WHITE);

    unsigned char str[] = { "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~" };
    unsigned long started = millis();
    glcdText.printString(&rec, str, sizeof(str), 1);
    unsigned long finished = millis();
    Serial.print("It took: ");
    Serial.println(finished - started);

    delay(3000);
    for (unsigned char i = 0; i < sizeof(str); i++) {
        glcdDriver.screen(0x00);
        glcdText.printString(&rec, &str[i], 1, 1);
        delay(500);
    }
}

void loop() {
}
