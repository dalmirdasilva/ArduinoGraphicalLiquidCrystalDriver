/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystal.c
 * 
 * The glcd driver functions
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#include "GraphicalLiquidCrystal.h"

GraphicalLiquidCrystal::GraphicalLiquidCrystal()
        : flags(0x00) {
}

void GraphicalLiquidCrystal::initIo() {
}

void GraphicalLiquidCrystal::screen(unsigned char pattern) {
    unsigned char page, line;
    for (page = 0; page < GRAPHICAL_LIQUID_CRYSTAL_CHIP_PAGES; page++) {
        for (line = 0; line < GRAPHICAL_LIQUID_CRYSTAL_PAGE_LINES; line++) {
            writeDataAt(GraphicalLiquidCrystal::CHIP_ALL, page, line, pattern);
        }
    }
}

bool GraphicalLiquidCrystal::plot(unsigned char x, unsigned char y, Color color) {

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

bool GraphicalLiquidCrystal::streak(unsigned char x, unsigned char page, unsigned char streak) {

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

bool GraphicalLiquidCrystal::writeDataAt(Chip chip, unsigned char page, unsigned char line, unsigned char b) {
    command(chip, (unsigned char) (CMD_SET_PAGE | page));
    command(chip, (unsigned char) (CMD_SET_ADDRESS | line));
    return writeData(chip, b);
}

unsigned char GraphicalLiquidCrystal::readDataAt(Chip chip, unsigned char page, unsigned char line) {
    command(chip, (unsigned char) (CMD_SET_PAGE | page));
    command(chip, (unsigned char) (CMD_SET_ADDRESS | line));
    return readData(chip);
}

void GraphicalLiquidCrystal::scroll(Chip chip, ScrollDirection direction, unsigned char lines) {
    unsigned char i = 0;
    if (direction == SCROLL_DOWN) {
        lines = -(lines);
    }
    if (chip == CHIP_ALL) {
        for (i = 0; i < GRAPHICAL_LIQUID_CRYSTAL_CHIPS; i++) {
            startLine[i].scrollTo += lines;
            scrollTo((Chip) i, startLine[i].scrollTo);
        }
    } else {
        startLine[chip].scrollTo += lines;
        scrollTo((Chip) chip, startLine[chip].scrollTo);
    }
}
