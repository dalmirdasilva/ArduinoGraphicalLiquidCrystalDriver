/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystalKs0108.c
 * 
 * The glcd driver functions
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#include "GraphicalLiquidCrystalKs0108.h"
#include <Arduino.h>

GraphicalLiquidCrystalKs0108::GraphicalLiquidCrystalKs0108()
        : GraphicalLiquidCrystal(0x80, 0x40) {
}

void GraphicalLiquidCrystalKs0108::screen(unsigned char pattern) {
    unsigned char page, line;
    for (page = 0; page < GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_PAGES; page++) {
        for (line = 0; line < GRAPHICAL_LIQUID_CRYSTAL_KS0108_PAGE_LINES; line++) {
            writeDataAt(CHIP_ALL, page, line, pattern);
        }
    }
}

bool GraphicalLiquidCrystalKs0108::plot(unsigned char x, unsigned char y, Color color) {
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

bool GraphicalLiquidCrystalKs0108::streak(unsigned char x, unsigned char page, unsigned char streak) {
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

bool GraphicalLiquidCrystalKs0108::writeDataAt(Chip chip, unsigned char page, unsigned char line, unsigned char b) {
    command(chip, (unsigned char) (COMMAND_SET_PAGE | page));
    command(chip, (unsigned char) (COMMAND_SET_ADDRESS | line));
    return writeData(chip, b);
}

unsigned char GraphicalLiquidCrystalKs0108::readDataAt(Chip chip, unsigned char page, unsigned char line) {
    command(chip, (unsigned char) (COMMAND_SET_PAGE | page));
    command(chip, (unsigned char) (COMMAND_SET_ADDRESS | line));
    return readData(chip);
}

void GraphicalLiquidCrystalKs0108::init(Mode mode) {
    unsigned char i = 0;
    initIo();
    reset();
    clearEnablePin();
    for (i = 0; i < GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIPS; i++) {
        startLine[i].scrollTo = 0;
    }
    scrollToChip(CHIP_ALL, 0);
    command(CHIP_ALL, COMMAND_SET_ADDRESS);
    command(CHIP_ALL, COMMAND_SET_PAGE);
    if (mode == MODE_ON) {
        command(CHIP_ALL, COMMAND_DISPLAY_ON_OFF | COMMAND_DISPLAY_ON_OFF_ON);
    } else {
        command(CHIP_ALL, COMMAND_DISPLAY_ON_OFF);
    }
}

void GraphicalLiquidCrystalKs0108::initIo() {
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN, OUTPUT);
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN, OUTPUT);
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RS_PIN, OUTPUT);
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RW_PIN, OUTPUT);
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_EN_PIN, OUTPUT);

#ifdef GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_USING_RESET
    pinMode(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RESET_PIN, OUTPUT);
#endif
}

void GraphicalLiquidCrystalKs0108::reset() {
#ifdef GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_USING_RESET
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RESET_PIN, LOW);
    delayMicroseconds(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_RESET_US);
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RESET_PIN, HIGH);
    while (isReseting(CHIP_FIST))
        ;
#endif
}

void GraphicalLiquidCrystalKs0108::switchRegisterSelectTo(RegisterSelect rs) {
    if (rs == REGISTER_SELECT_COMMAND) {
        switchRegisterSelectToCommand();
    } else {
        switchRegisterSelectToData();
    }
}

void GraphicalLiquidCrystalKs0108::switchChipTo(Chip chip) {
    if (chip == CHIP_ALL) {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN, HIGH);
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN, HIGH);
    } else {
        if (chip == CHIP_FIST) {
            digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN, HIGH);
            digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN, LOW);
        } else {
            digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN, LOW);
            digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN, HIGH);
        }
    }
}

bool GraphicalLiquidCrystalKs0108::write(Chip chip, unsigned char b, RegisterSelect rs) {
#if GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CHECK_FOR_BUSY_ON_WRITE == 1
    unsigned char attempts = GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DEFAULT_ATTEMPTS_ON_BUSY;
    while (isBusy(chip) && attempts--) {
        if (attempts == 0) {
            setWriteTimeoutFlag();
            return false;
        }
    }
#endif
    switchRegisterSelectTo(rs);
    switchReadWriteToWrite();
    switchChipTo(chip);
    setEnablePin();
    delayMicroseconds(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TDSU_US);
    busOutputDirection();
    writeToBus(b);
    delayMicroseconds(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TDHW_US);
    clearEnablePin();
    disableChips();
    return true;
}

unsigned char GraphicalLiquidCrystalKs0108::read(Chip chip, RegisterSelect rs) {
    unsigned char b = 0;
    unsigned char i, howManyReads = 1;

    // In some cases is necessary to write data in all chips,
    // But, to know if the module is not busy is necessary a read
    // operation to get the glcd status. But read in all chips will
    // cause conflicts in the bus.
    // I decided to choose the first chip in this case and go on, but
    // you can make another decision.
    if (chip == CHIP_ALL) {
        setReadInAllChipsFlag();
        // BUG? was ==
        chip = CHIP_FIST;
    }
    busInputDirection();
    clearEnablePin();
    switchChipTo(chip);
    switchRegisterSelectTo(rs);
    switchReadWriteToRead();

    // To read the contents of display data RAM, twice access of read
    // instruction is needed. In first access, data in display data RAM
    // is latched into output register. In second access, MPU can read
    // data which is latched. That is, to read the data in display data
    // RAM, it needs dummy read. But status read is not needed dummy
    // read.
    if (rs == REGISTER_SELECT_DATA) {
        howManyReads = 2;
    }
    for (i = 0; i < howManyReads; i++) {
        setEnablePin();
        delayMicroseconds(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TD_US);
        b = readFromBus();
        clearEnablePin();
    }
    disableChips();
    return b;
}

void GraphicalLiquidCrystalKs0108::switchRegisterSelectToData() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RS_PIN, HIGH);
}

void GraphicalLiquidCrystalKs0108::switchRegisterSelectToCommand() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RS_PIN, LOW);
}

void GraphicalLiquidCrystalKs0108::switchReadWriteToWrite() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RW_PIN, LOW);
}

void GraphicalLiquidCrystalKs0108::switchReadWriteToRead() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RW_PIN, HIGH);
}

void GraphicalLiquidCrystalKs0108::writeToBus(unsigned char b) {
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_LOW = (GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_LOW & 0x0f) | (b & 0xf0);
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_HIGH = (GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_HIGH & 0xf0) | (b & 0x0f);
}

unsigned char GraphicalLiquidCrystalKs0108::readFromBus() {
    return (GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PIN_NIBBLE_LOW & 0xf0) | (GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PIN_NIBBLE_HIGH & 0x0f);
}

void GraphicalLiquidCrystalKs0108::busOutputDirection() {
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_LOW |= 0xf0;
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_HIGH |= 0x0f;
}

void GraphicalLiquidCrystalKs0108::busInputDirection() {
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_LOW &= 0x0f;
    GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_HIGH &= 0xf0;
}

void GraphicalLiquidCrystalKs0108::setEnablePin() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_EN_PIN, HIGH);
}

void GraphicalLiquidCrystalKs0108::clearEnablePin() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_EN_PIN, LOW);
}

void GraphicalLiquidCrystalKs0108::disableChips() {
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN, LOW);
    digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN, LOW);
}

bool GraphicalLiquidCrystalKs0108::isReseting(Chip chip) {
    return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_RESET_BIT) != 0);
}

bool GraphicalLiquidCrystalKs0108::isOff(Chip chip) {
    return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_OFF_BIT) != 1);
}

bool GraphicalLiquidCrystalKs0108::isBusy(Chip chip) {
    return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_BUSY_BIT) != 0);
}

unsigned char GraphicalLiquidCrystalKs0108::status(Chip chip) {
    return read(chip, REGISTER_SELECT_COMMAND);
}

bool GraphicalLiquidCrystalKs0108::getWriteTimeoutFlag() {
    return ((flags & GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_TIME_OUT_ON_WRITE_BIT) != 0);
}

bool GraphicalLiquidCrystalKs0108::getOutOfRangeFlag() {
    return ((flags & GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_TIME_OUT_ON_WRITE_BIT) != 0);
}

bool GraphicalLiquidCrystalKs0108::getReadInAllChipsFlag() {
    return ((flags & GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_READ_IN_ALL_CHIPS_BIT) != 0);
}

unsigned char GraphicalLiquidCrystalKs0108::readData(Chip chip) {
    return read(chip, REGISTER_SELECT_DATA);
}

bool GraphicalLiquidCrystalKs0108::writeData(Chip chip, unsigned char b) {
    return write(chip, b, REGISTER_SELECT_DATA);
}

bool GraphicalLiquidCrystalKs0108::command(Chip chip, unsigned char cmd) {
    return write(chip, cmd, REGISTER_SELECT_COMMAND);
}

unsigned char GraphicalLiquidCrystalKs0108::getChipFromPoint(unsigned char x, unsigned char y) {
    return ((y / GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT) * GRAPHICAL_LIQUID_CRYSTAL_KS0108_HORIZONTAL_CHIPS) + (x / GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH);
}

unsigned char GraphicalLiquidCrystalKs0108::getPageFromPoint(unsigned char x, unsigned char y) {
    return (y % GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT) / 8;
}

unsigned char GraphicalLiquidCrystalKs0108::getLineFromPoint(unsigned char x, unsigned char y) {
    return x % GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH;
}

unsigned char GraphicalLiquidCrystalKs0108::getBitFromPoint(unsigned char x, unsigned char y) {
    return y % 8;
}

void GraphicalLiquidCrystalKs0108::setWriteTimeoutFlag() {
    flags |= GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_TIME_OUT_ON_WRITE_BIT;
}

void GraphicalLiquidCrystalKs0108::clrWriteTimeoutFlag() {
    flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_TIME_OUT_ON_WRITE_BIT);
}

void GraphicalLiquidCrystalKs0108::setOutOfRangeFlag() {
    flags |= GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_PLOT_OUT_OF_RANGE_BIT;
}

void GraphicalLiquidCrystalKs0108::clrOutOfRangeFlag() {
    flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_PLOT_OUT_OF_RANGE_BIT);
}

void GraphicalLiquidCrystalKs0108::setReadInAllChipsFlag() {
    flags |= GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_READ_IN_ALL_CHIPS_BIT;
}

void GraphicalLiquidCrystalKs0108::clrReadInAllChipsFlag() {
    flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_READ_IN_ALL_CHIPS_BIT);
}

void GraphicalLiquidCrystalKs0108::scrollToChip(Chip chip, unsigned char line) {
    command(chip, COMMAND_DISPLAY_START_LINE | (line & 0x3f));
}

void GraphicalLiquidCrystalKs0108::scrollTo(unsigned char line) {
    scrollToChip(CHIP_ALL, line);
}

void GraphicalLiquidCrystalKs0108::scroll(ScrollDirection direction, unsigned char lines) {
    scrollChip(CHIP_ALL, direction, lines);
}

void GraphicalLiquidCrystalKs0108::scrollChip(Chip chip, ScrollDirection direction, unsigned char lines) {
    unsigned char i = 0;
    if (direction == SCROLL_DOWN) {
        lines = -(lines);
    }
    if (chip == CHIP_ALL) {
        for (i = 0; i < GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIPS; i++) {
            startLine[i].scrollTo += lines;
            scrollToChip((Chip) i, startLine[i].scrollTo);
        }
    } else {
        startLine[chip].scrollTo += lines;
        scrollToChip((Chip) chip, startLine[chip].scrollTo);
    }
}

