/**
 * Arduino - Glcd driver
 * 
 * GlcdStraight.cpp
 * 
 * The glcd driver functions for glcd driver, wicth implements the driver base
 * with direct access, witout buffer.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GLCD_STRAIGHT_CPP__
#define __ARDUINO_DRIVER_GLCD_STRAIGHT_CPP__ 1

#include "GlcdStraight.h"

GlcdStraight::GlcdStraight() : Glcd() {
}

void GlcdStraight::init(Mode mode) {
    unsigned char i = 0;
    initIo();
    reset();
	clrEnablePin();
    for (i = 0; i < GLCD_CHIPS; i++) {
        startLine[i].scrollTo = 0;
    }
    scrollTo(CHIP_ALL, 0);
    command(CHIP_ALL, CMD_SET_ADDRESS);
    command(CHIP_ALL, CMD_SET_PAGE);
    if (mode == MODE_ON) {
        command(CHIP_ALL, CMD_DISPLAY_ON_OFF | CMD_DISPLAY_ON_OFF_ON);
    } else {
        command(CHIP_ALL, CMD_DISPLAY_ON_OFF);
    }
}

void GlcdStraight::initIo() {
    pinMode(GLCD_CS1_PIN, OUTPUT);
    pinMode(GLCD_CS2_PIN, OUTPUT);
    pinMode(GLCD_RS_PIN, OUTPUT);
    pinMode(GLCD_RW_PIN, OUTPUT);
    pinMode(GLCD_EN_PIN, OUTPUT);
//    pinMode(GLCD_RESET_PIN, OUTPUT);
}

void GlcdStraight::reset() {
//    digitalWrite(GLCD_RESET_PIN, LOW);
//    delayMicroseconds(GLCD_DELAY_RESET_US);
//    digitalWrite(GLCD_RESET_PIN, HIGH);
//    while (isReseting(Glcd::CHIP_1));
}

void GlcdStraight::switchRegisterSelectTo(RegisterSelect rs) {
    if (rs == RS_COMMAND) {
        switchRegisterSelectToCommand();
    } else {
        switchRegisterSelectToData();
    }
}

void GlcdStraight::switchChipTo(Chip chip) {
    if (chip == CHIP_ALL) {
        digitalWrite(GLCD_CS1_PIN, HIGH);
        digitalWrite(GLCD_CS2_PIN, HIGH);
    } else {
        if (chip == CHIP_1) {
            digitalWrite(GLCD_CS1_PIN, HIGH);
            digitalWrite(GLCD_CS2_PIN, LOW);
        } else {
            digitalWrite(GLCD_CS1_PIN, LOW);
            digitalWrite(GLCD_CS2_PIN, HIGH);
        }
    }
}

bool GlcdStraight::write(Chip chip, unsigned char b, RegisterSelect rs) {

#if GLCD_CHECK_FOR_BUSY_ON_WRITE == 1
    unsigned char attempts = GLCD_DEFAULT_ATTEMPTS_ON_BUSY;
    while (isBusy(chip) && attempts--) {
        if (attempts == 0) {
            setWriteTimeoutFlag();
            return 0;
        }
    }
#else
#warning "Warning: You aren't using busy check on write."
#endif

    switchRegisterSelectTo(rs);
    switchRwToWrite();
    switchChipTo(chip);
    setEnablePin();
    delayMicroseconds(GLCD_DELAY_TDSU_US);
    busOutputDirection();
    writeToBus(b);
    delayMicroseconds(GLCD_DELAY_TDHW_US);
    clrEnablePin();
    disableChips();

    return 1;
}

unsigned char GlcdStraight::read(Chip chip, RegisterSelect rs) {

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
        chip == CHIP_1;
    }

    busInputDirection();
    clrEnablePin();
    switchChipTo(chip);
    switchRegisterSelectTo(rs);
    switchRwToRead();

    // To read the contents of display data RAM, twice access of read 
    // instruction is needed. In first access, data in display data RAM
    // is latched into output register. In second access, MPU can read 
    // data which is latched. That is, to read the data in display data
    // RAM, it needs dummy read. But status read is not needed dummy 
    // read.
    if (rs == RS_DATA) {
        howManyReads = 2;
    }
    
    for (i = 0; i < howManyReads; i++) {
        setEnablePin();
        delayMicroseconds(GLCD_DELAY_TD_US);
        b = readFromBus();
        clrEnablePin();
    }
    
    disableChips();

    return b;
}
#endif /* __ARDUINO_DRIVER_GLCD_STRAIGHT_CPP__ */
