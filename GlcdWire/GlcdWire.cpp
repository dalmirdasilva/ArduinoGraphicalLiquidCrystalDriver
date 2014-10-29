/**
 * Arduino - Glcd driver
 * 
 * GlcdWire.h
 * 
 * The header file for glcd driver, wicth implements the driver base
 * using i2c with a PIC microcontroller.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GLCD_WIRE_CPP__
#define __ARDUINO_DRIVER_GLCD_WIRE_CPP__ 1

#include <GlcdWire.h>
#include <Wire.h>
#include <Arduino.h>

GlcdWire::GlcdWire(unsigned char device) : Glcd() {
    this->device = device;
}

void GlcdWire::init(Mode mode) {
    Wire.begin();
    for (unsigned char i = 0; i < GLCD_CHIPS; i++) {
        chipInfo[i].page = 0x00;
        chipInfo[i].line = 0x00;
    }
}

void GlcdWire::reset() {
}

bool GlcdWire::write(Chip chip, unsigned char b, RegisterSelect rs) {
    unsigned char cmd, page, line;
    bool success = false;
    cmd = b & 0xc0;
    if (rs == Glcd::RS_COMMAND) {
        if (cmd == (Glcd::CMD_SET_PAGE & 0xc0)) {
            if (chip == Glcd::CHIP_1 || chip == Glcd::CHIP_ALL) {
                chipInfo[0].page = b & 0x07;
            }
            if (chip == Glcd::CHIP_2 || chip == Glcd::CHIP_ALL) {
                chipInfo[1].page = b & 0x07;
            }
        } else if(cmd == (Glcd::CMD_SET_ADDRESS & 0xc0)) {
            if (chip == Glcd::CHIP_1 || chip == Glcd::CHIP_ALL) {
                chipInfo[0].line = b & 0x3f;
            }
            if (chip == Glcd::CHIP_2 || chip == Glcd::CHIP_ALL) {
                chipInfo[1].line = b & 0x3f;
            }
        }
    } else {
        if (chip == Glcd::CHIP_1 || chip == Glcd::CHIP_ALL) {
            page = (chipInfo[0].page << 2) | (chip & 0x03);
            line = chipInfo[0].line;
        } else {
            page = (chipInfo[1].page << 2) | (chip & 0x03);
            line = chipInfo[1].line;
        }
        Wire.beginTransmission((int)device);
        Wire.write(page);
        Wire.write(line);
        Wire.write(b);
        if (Wire.endTransmission() == 0) {
            success = true;
        }
    }
    return success;
}

unsigned char GlcdWire::read(Chip chip, RegisterSelect rs) {
    unsigned char page, line;
    if (rs == Glcd::RS_COMMAND) {
        return 0x20;
    }
    if (chip == Glcd::CHIP_1 || chip == Glcd::CHIP_ALL) {
        page = (chipInfo[0].page << 2) | (chip & 0x03);
        line = chipInfo[0].line;
    } else {
        page = (chipInfo[1].page << 2) | (chip & 0x03);
        line = chipInfo[1].line;
    }
    Wire.beginTransmission((int)device);
    Wire.write(page);
    Wire.write(line);
    Wire.endTransmission();
    Wire.requestFrom((int)device, (int)1);
    while (!Wire.available());
    return Wire.read();
}

#endif /* __ARDUINO_DRIVER_GLCD_WIRE_CPP__ */
