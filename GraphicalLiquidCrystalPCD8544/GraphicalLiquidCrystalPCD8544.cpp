/**
 * Arduino Graphical Liquid Crystal Driver
 *
 * GraphicalLiquidCrystal.c
 *
 * The glcd driver functions
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#include <Arduino.h>
#include "GraphicalLiquidCrystalPCD8544.h"

GraphicalLiquidCrystalPCD8544::GraphicalLiquidCrystalPCD8544(unsigned char dataPin, unsigned char clockPin, unsigned char rstPin, unsigned char dcPin, unsigned char scePin)
        : GraphicalLiquidCrystal(0x54, 0x30), dataPin(dataPin), clockPin(clockPin), rstPin(rstPin), dcPin(dcPin), scePin(scePin), currentScroll(0x00) {
}

void GraphicalLiquidCrystalPCD8544::init(Mode mode) {

    // Initial control settings
    currentControl.funtionSet = COMMAND_FUNCTION_SET | COMMAND_FUNCTION_SET_ACTIVE | COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS | COMMAND_FUNCTION_SET_HORIZONTAL_ADDRESSING;
    currentControl.displayControl = COMMAND_DISPLAY_CONTROL | COMMAND_DISPLAY_CONTROL_NORMAL;

    // Pin direction
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(rstPin, OUTPUT);
    pinMode(dcPin, OUTPUT);
    pinMode(scePin, OUTPUT);

    // Toggle rstPin to reset
    digitalWrite(rstPin, LOW);
    delay(1000); // how much?
    digitalWrite(rstPin, HIGH);

    // Issues function set command
    command(currentControl.funtionSet);

    // Issues contrast command
    command(COMMAND_SET_VOP | 0x3f);

    // Issues temperature control
    command(COMMAND_TEMPERATURE_CONTROL | 0x02);

    // Issues system bias command
    command(COMMAND_BIAS_SYSTEM | 0x04);

    // Back normal instruction set
    currentControl.funtionSet &= ~COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS;
    command(currentControl.funtionSet);

    // Issues a display control
    command(currentControl.displayControl);

    // Sends the buffer to the module
    screen(0x00);
}

void GraphicalLiquidCrystalPCD8544::reset() {
}

void GraphicalLiquidCrystalPCD8544::sync() {
    command(COMMAND_SET_X_ADDRESS);
    Serial.print("sync with: ");
    Serial.println(currentScroll);
    command(COMMAND_SET_Y_ADDRESS | currentScroll);
    for (int y = 0; y < GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES; y++) {
        for (int x = 0; x < GRAPHICAL_LIQUID_CRYSTAL_PCD8544_WIDTH; x++) {
            send(buffer[y][x]);
        }
    }
}

void GraphicalLiquidCrystalPCD8544::setBias(unsigned char bias) {
    bias &= 0x07;
    command(COMMAND_BIAS_SYSTEM | bias);
}

void GraphicalLiquidCrystalPCD8544::setContrast(unsigned char contrast) {
    contrast &= 0x7f;
    currentControl.funtionSet |= COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS;
    command(currentControl.funtionSet);
    command(COMMAND_SET_VOP | contrast);
}

void GraphicalLiquidCrystalPCD8544::screen(unsigned char pattern) {
    memset(buffer, pattern, GRAPHICAL_LIQUID_CRYSTAL_PCD8544_BYTE_SIZE);
    sync();
}

inline unsigned char GraphicalLiquidCrystalPCD8544::getPageFromPoint(unsigned char x, unsigned char y) {
    return y / 8;
}

inline unsigned char GraphicalLiquidCrystalPCD8544::getBitFromPoint(unsigned char x, unsigned char y) {
    return y % 8;
}

bool GraphicalLiquidCrystalPCD8544::plot(unsigned char x, unsigned char y, Color color) {
    unsigned char line, bit, b;
    if (isOutOfRange(x, y)) {
        return false;
    }
    line = getPageFromPoint(x, y);
    bit = getBitFromPoint(x, y);
    b = buffer[line][x];
    bitWrite(b, bit, color);
    buffer[line][x] = b;
    streak(x, line, b);
    return true;
}

bool GraphicalLiquidCrystalPCD8544::streak(unsigned char x, unsigned char line, unsigned char streak) {
    command(COMMAND_SET_Y_ADDRESS | ((line + currentScroll) % GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES));
    command(COMMAND_SET_X_ADDRESS | x);
    // Should save into internal buffer?
    send(streak);
    return true;
}

void GraphicalLiquidCrystalPCD8544::scrollTo(unsigned char line) {
    currentScroll = line % GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES;
    sync();
}

void GraphicalLiquidCrystalPCD8544::scroll(ScrollDirection direction, unsigned char lines) {
    /*unsigned char line;
    if (direction == SCROLL_DOWN) {
        line = currentScroll - lines;
    } else {
        line = currentScroll + lines;
    }*/
    scrollTo(0);
}

inline void GraphicalLiquidCrystalPCD8544::send(unsigned char b) {
    enableChip();
    shiftOut(dataPin, clockPin, MSBFIRST, b);
    disableChip();
}

inline void GraphicalLiquidCrystalPCD8544::switchRegisterSelectToData() {
    digitalWrite(dcPin, HIGH);
}

inline void GraphicalLiquidCrystalPCD8544::switchRegisterSelectToCommand() {
    digitalWrite(dcPin, LOW);
}

inline void GraphicalLiquidCrystalPCD8544::enableChip() {
    digitalWrite(scePin, LOW);
}

inline void GraphicalLiquidCrystalPCD8544::disableChip() {
    digitalWrite(scePin, HIGH);
}

void GraphicalLiquidCrystalPCD8544::command(unsigned char command) {
    switchRegisterSelectToCommand();
    send(command);
    switchRegisterSelectToData();
}
