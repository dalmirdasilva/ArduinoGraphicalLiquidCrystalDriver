/**
 * Arduino Graphical Liquid Crystal Driver
 *
 * GraphicalLiquidCrystal.c
 *
 * The glcd driver functions
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#include "GraphicalLiquidCrystalNokia5110.h"

#include <Arduino.h>

GraphicalLiquidCrystalNokia5110::GraphicalLiquidCrystalNokia5110(unsigned char mosiPin, unsigned char clockPin,
        unsigned char rstPin, unsigned char dcPin, unsigned char scePin) :
        GraphicalLiquidCrystal(0x54, 0x30), mosiPin(mosiPin), clockPin(clockPin), rstPin(rstPin), dcPin(dcPin), scePin(
                scePin), currentScroll(0x00) {

    mosiPort = portOutputRegister(digitalPinToPort(mosiPin));
    mosiPinMask = digitalPinToBitMask(mosiPin);
    clockPort = portOutputRegister(digitalPinToPort(clockPin));
    clockPinMask = digitalPinToBitMask(clockPin);
}

void GraphicalLiquidCrystalNokia5110::init(Mode mode) {

    // Initial control settings
    currentControl.funtionSet = COMMAND_FUNCTION_SET | COMMAND_FUNCTION_SET_ACTIVE
            | COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS | COMMAND_FUNCTION_SET_HORIZONTAL_ADDRESSING;
    currentControl.displayControl = COMMAND_DISPLAY_CONTROL | COMMAND_DISPLAY_CONTROL_NORMAL;

    // Pin direction
    pinMode(mosiPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(rstPin, OUTPUT);
    pinMode(dcPin, OUTPUT);
    pinMode(scePin, OUTPUT);

    // Toggle rstPin to reset
    digitalWrite(rstPin, LOW);
    delay(1); // how much?
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

void GraphicalLiquidCrystalNokia5110::reset() {
}

void GraphicalLiquidCrystalNokia5110::flush() {
    command(COMMAND_SET_X_ADDRESS);
    command(COMMAND_SET_Y_ADDRESS | (currentScroll % GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_HEIGHT_PAGES));
    for (int y = 0; y < GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_HEIGHT_PAGES; y++) {
        for (int x = 0; x < GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_WIDTH; x++) {
            send(buffer[y][x]);
        }
    }
}

void GraphicalLiquidCrystalNokia5110::setBias(unsigned char bias) {
    bias &= 0x07;
    command(COMMAND_BIAS_SYSTEM | bias);
}

void GraphicalLiquidCrystalNokia5110::setContrast(unsigned char contrast) {
    contrast &= 0x7f;
    currentControl.funtionSet |= COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS;
    command(currentControl.funtionSet);
    command(COMMAND_SET_VOP | contrast);
    currentControl.funtionSet &= ~COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS;
    command(currentControl.funtionSet);
}

void GraphicalLiquidCrystalNokia5110::screen(unsigned char pattern) {
    memset(buffer, pattern, GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_BYTE_SIZE);
    flush();
}

inline unsigned char GraphicalLiquidCrystalNokia5110::getPageFromPoint(unsigned char x, unsigned char y) {
    return y / 8;
}

inline unsigned char GraphicalLiquidCrystalNokia5110::getBitFromPoint(unsigned char x, unsigned char y) {
    return y % 8;
}

bool GraphicalLiquidCrystalNokia5110::plot(unsigned char x, unsigned char y, Color color) {
    unsigned char line, bit, b;
    if (isOutOfRange(x, y)) {
        return false;
    }
    line = getPageFromPoint(x, y);
    bit = getBitFromPoint(x, y);
    b = buffer[line][x];
    bitWrite(b, bit, color);
    streak(x, line, b);
    return true;
}

bool GraphicalLiquidCrystalNokia5110::streak(unsigned char x, unsigned char line, unsigned char streak) {
    command(COMMAND_SET_Y_ADDRESS | ((line + currentScroll) % GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_HEIGHT_PAGES));
    command(COMMAND_SET_X_ADDRESS | x);
    buffer[line][x] = streak;
    send(streak);
    return true;
}

void GraphicalLiquidCrystalNokia5110::scrollTo(unsigned char line) {
    currentScroll = line % GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_HEIGHT_PAGES;
    flush();
}

void GraphicalLiquidCrystalNokia5110::scroll(ScrollDirection direction, unsigned char lines) {
    unsigned char line;
    if (direction == SCROLL_UP) {
        line = currentScroll + lines;
    } else {
        if (lines <= currentScroll) {
            line = currentScroll - lines;
        } else {
            line = GRAPHICAL_LIQUID_CRYSTAL_NOKIA_5110_HEIGHT_PAGES - (lines - currentScroll);
        }
    }
    scrollTo(line);
}

void inline GraphicalLiquidCrystalNokia5110::scrollUp(unsigned char lines) {
    scroll(SCROLL_UP, lines);
}

void inline GraphicalLiquidCrystalNokia5110::scrollDown(unsigned char lines) {
    scroll(SCROLL_DOWN, lines);
}

inline void GraphicalLiquidCrystalNokia5110::send(unsigned char b) {
    enableChip();
    transfer(b);
    disableChip();
}

inline void GraphicalLiquidCrystalNokia5110::transfer(unsigned char b) {
    for (unsigned char bit = 0x80; bit > 0; bit >>= 1) {
        *clockPort &= ~clockPinMask;
        if (b & bit) {
            *mosiPort |= mosiPinMask;
        } else {
            *mosiPort &= ~mosiPinMask;
        }
        *clockPort |= clockPinMask;
    }
}

inline void GraphicalLiquidCrystalNokia5110::switchRegisterSelectToData() {
    digitalWrite(dcPin, HIGH);
}

inline void GraphicalLiquidCrystalNokia5110::switchRegisterSelectToCommand() {
    digitalWrite(dcPin, LOW);
}

inline void GraphicalLiquidCrystalNokia5110::enableChip() {
    digitalWrite(scePin, LOW);
}

inline void GraphicalLiquidCrystalNokia5110::disableChip() {
    digitalWrite(scePin, HIGH);
}

void GraphicalLiquidCrystalNokia5110::command(unsigned char command) {
    switchRegisterSelectToCommand();
    send(command);
    switchRegisterSelectToData();
}
