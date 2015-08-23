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

GraphicalLiquidCrystal::GraphicalLiquidCrystal(unsigned char width, unsigned char height)
        : flags(0x00), width(width), height(height) {
}

GraphicalLiquidCrystal::~GraphicalLiquidCrystal() {
}

bool GraphicalLiquidCrystal::isOutOfRange(unsigned char x, unsigned char y) {
    return (x > this->width || y > this->height);
}

unsigned char GraphicalLiquidCrystal::getWidth() {
    return this->width;
}

unsigned char GraphicalLiquidCrystal::getHeight() {
    return this->height;
}

void GraphicalLiquidCrystal::clear() {
    screen(COLOR_BLACK);
}
