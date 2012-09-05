/**
 * Arduino - Glcd driver
 * 
 * Glcd.c
 * 
 * The glcd driver functions
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GLCD_CPP__
#define __ARDUINO_DRIVER_GLCD_CPP__ 1

#include "Glcd.h"

Glcd::Glcd() {
    flags = 0x00;
}

void Glcd::initIo() {
}
    
void Glcd::screen(unsigned char pattern) {

    unsigned char chip, page, line;

    for (page = 0; page < GLCD_CHIP_PAGES; page++) {
        for (line = 0; line < GLCD_PAGE_LINES; line++) {
            writeDataAt(Glcd::CHIP_ALL, page, line, pattern);
        }
    }
}

bool Glcd::plot(unsigned char x, unsigned char y, Color color) {

    unsigned char b;

    unsigned char chip, page, line;

    if (isOutOfRange(x, y)) {

        setOutOfRangeFlag();
        return 0;
    }

    chip = getChipFromPoint(x, y);
    page = getPageFromPoint(x, y);
    line = getLineFromPoint(x, y);

    b = readDataAt((Chip) chip, page, line);

    if (color) {
        bitSet(b, getBitFromPoint(x, y));
    } else {
        bitClear(b, getBitFromPoint(x, y));
    }
    
    return writeDataAt((Chip) chip, page, line, b);
}

bool Glcd::streak(unsigned char x, unsigned char page, unsigned char streak) {
    
    unsigned char chip, line, y;

    y = page * 8;
    
    if (isOutOfRange(x, y)) {

        setOutOfRangeFlag();
        return 0;
    }

    chip = getChipFromPoint(x, y);
    line = getLineFromPoint(x, y);
    
    return writeDataAt((Chip) chip, page, line, streak);
}

bool Glcd::writeDataAt(Chip chip, unsigned char page, unsigned char line, unsigned char b) {
    command(chip, (unsigned char) (CMD_SET_PAGE | page));
    command(chip, (unsigned char) (CMD_SET_ADDRESS | line));
    return writeData(chip, b);
}

unsigned char Glcd::readDataAt(Chip chip, unsigned char page, unsigned char line) {
    command(chip, (unsigned char) (CMD_SET_PAGE | page));
    command(chip, (unsigned char) (CMD_SET_ADDRESS | line));
    return readData(chip);
}

void Glcd::scroll(Chip chip, ScrollDirection direction, unsigned char lines) {
    unsigned char i = 0;
    if (direction == SCROLL_DOWN) {
        lines = -(lines);
    }
    if (chip == CHIP_ALL) {
        for (i = 0; i < GLCD_CHIPS; i++) {
            startLine[i].scrollTo += lines;
            scrollTo((Chip) i, startLine[i].scrollTo);
        }
    } else {
        startLine[chip].scrollTo += lines;
        scrollTo((Chip) chip, startLine[chip].scrollTo);
    }
}

#endif /* __ARDUINO_DRIVER_GLCD_CPP__ */
