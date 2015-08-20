/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystalWire.h
 * 
 * The header file for glcd driver, wicth implements the driver base
 * using i2c communication.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#include <Wire.h>
#include <Arduino.h>
#include "GraphicalLiquidCrystalWire.h"

GraphicalLiquidCrystalWire::GraphicalLiquidCrystalWire(unsigned char device)
        : GraphicalLiquidCrystal() {
    this->device = device;
}

void GraphicalLiquidCrystalWire::init(Mode mode) {
    Wire.begin();
    for (unsigned char i = 0; i < GRAPHICAL_LIQUID_CRYSTAL_CHIPS; i++) {
        chipInfo[i].page = 0x00;
        chipInfo[i].line = 0x00;
    }
}

void GraphicalLiquidCrystalWire::reset() {
}

bool GraphicalLiquidCrystalWire::write(Chip chip, unsigned char b, RegisterSelect rs) {
    unsigned char cmd, page, line;
    bool success = false;
    cmd = b & 0xc0;
    if (rs == GraphicalLiquidCrystal::RS_COMMAND) {
        if (cmd == (GraphicalLiquidCrystal::CMD_SET_PAGE & 0xc0)) {
            if (chip == GraphicalLiquidCrystal::CHIP_1 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
                chipInfo[0].page = b & 0x07;
            }
            if (chip == GraphicalLiquidCrystal::CHIP_2 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
                chipInfo[1].page = b & 0x07;
            }
        } else if (cmd == (GraphicalLiquidCrystal::CMD_SET_ADDRESS & 0xc0)) {
            if (chip == GraphicalLiquidCrystal::CHIP_1 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
                chipInfo[0].line = b & 0x3f;
            }
            if (chip == GraphicalLiquidCrystal::CHIP_2 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
                chipInfo[1].line = b & 0x3f;
            }
        }
    } else {
        if (chip == GraphicalLiquidCrystal::CHIP_1 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
            page = (chipInfo[0].page << 2) | (chip & 0x03);
            line = chipInfo[0].line;
        } else {
            page = (chipInfo[1].page << 2) | (chip & 0x03);
            line = chipInfo[1].line;
        }
        Wire.beginTransmission((int) device);
        Wire.write(page);
        Wire.write(line);
        Wire.write(b);
        if (Wire.endTransmission() == 0) {
            success = true;
        }
    }
    return success;
}

unsigned char GraphicalLiquidCrystalWire::read(Chip chip, RegisterSelect rs) {
    unsigned char page, line;
    if (rs == GraphicalLiquidCrystal::RS_COMMAND) {
        return 0x20;
    }
    if (chip == GraphicalLiquidCrystal::CHIP_1 || chip == GraphicalLiquidCrystal::CHIP_ALL) {
        page = (chipInfo[0].page << 2) | (chip & 0x03);
        line = chipInfo[0].line;
    } else {
        page = (chipInfo[1].page << 2) | (chip & 0x03);
        line = chipInfo[1].line;
    }
    Wire.beginTransmission((int) device);
    Wire.write(page);
    Wire.write(line);
    Wire.endTransmission();
    Wire.requestFrom((int) device, (int) 1);
    // TODO: use maximum retries here.
    while (!Wire.available())
        ;
    return Wire.read();
}
