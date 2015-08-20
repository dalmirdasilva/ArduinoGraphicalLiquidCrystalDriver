/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystalStraight.h
 * 
 * The header file for glcd driver, wicth implements the driver base
 * with direct access, witout buffer.
 * 
 * 01 - GND
 * 02 - VDD
 * 03 - V0
 * 04 - D/I
 * 05 - R/W
 * 06 - E
 * 07 - D0
 * 08 - D1
 * 09 - D2
 * 10 - D3
 * 11 - D4
 * 12 - D5
 * 13 - D6
 * 14 - D7
 * 15 - CS1
 * 16 - CS2
 * 17 - RST
 * 18 - VEE
 * 19 - LED 5v
 * 20 - LED 0v
 *
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_STRAIGHT_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_STRAIGHT_H__ 1

#include <GraphicalLiquidCrystal.h>

#define GRAPHICAL_LIQUID_CRYSTAL_CS1_PIN                                    12
#define GRAPHICAL_LIQUID_CRYSTAL_CS2_PIN                                    13

#define GRAPHICAL_LIQUID_CRYSTAL_RS_PIN                                     3
#define GRAPHICAL_LIQUID_CRYSTAL_RW_PIN                                     2
#define GRAPHICAL_LIQUID_CRYSTAL_EN_PIN                                     A0

#ifdef GRAPHICAL_LIQUID_CRYSTAL_USING_RESET
#define GRAPHICAL_LIQUID_CRYSTAL_RESET_PIN                                  13
#endif

/*
 * Arduino layout
 * 
 * B (digital pin 8 to 13)
 * C (analog input pins)
 * D (digital pins 0 to 7)
 */
#define GRAPHICAL_LIQUID_CRYSTAL_BUS_PIN_NIBBLE_LOW                         PIND
#define GRAPHICAL_LIQUID_CRYSTAL_BUS_PIN_NIBBLE_HIGH                        PINB

#define GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_LOW                         DDRD
#define GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_HIGH                        DDRB

#define GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_LOW                        PORTD
#define GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_HIGH                       PORTB

// Data setup time TDSU ~= 300 ns (at 20Mhz it will be about 15 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_DELAY_TDSU_US	                            0x0a

// Data hold time (write) TDHW ~= 15 ns (at 20Mhz it will be about 1 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_DELAY_TDHW_US	                            0x0a

// Data delay time TDDR ~= 480 ns (at 20Mhz it will be about 25 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_DELAY_TD_US	                            0x0a

// I don't know exactly how many cycles :/
#define GRAPHICAL_LIQUID_CRYSTAL_DELAY_RESET_US								0x0a

#define GRAPHICAL_LIQUID_CRYSTAL_CHECK_FOR_BUSY_ON_WRITE                    0x00
#define GRAPHICAL_LIQUID_CRYSTAL_DEFAULT_ATTEMPTS_ON_BUSY                   0x0a

class GraphicalLiquidCrystalStraight: public GraphicalLiquidCrystal {

public:

    /**
     * Public constructor.
     */
    GraphicalLiquidCrystalStraight();

    /**
     * Initializes the module.
     * 
     * @param mode			On or Off.
     */
    void init(Mode mode);

    /**
     * Issues a reset to the module
     * 
     * @return  void    
     */
    void reset();

protected:

    /**
     * Initializes the IO.
     */
    void initIo();

    /**
     * Writes a byte into the module.
     * 
     * @param chip              The chip selector.
     * @param b                 The byte to be written.
     * @param rs                The register select.
     * @return 
     */
    bool write(Chip chip, unsigned char b, RegisterSelect rs);

    /**
     * Reads a byte from the module.
     * 
     * @param chip              The chip selector.
     * @param rs                The register select.
     * @return 
     */
    unsigned char read(Chip chip, RegisterSelect rs);

    /**
     * Switch the register select pin to the given mode.
     */
    void switchRegisterSelectTo(RegisterSelect rs);

    /**
     * Switch the register select pin to data mode.
     */
    void switchRegisterSelectToData() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_RS_PIN, HIGH);
    }

    /**
     * Switch the register select pin to command mode.
     */
    void switchRegisterSelectToCommand() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_RS_PIN, LOW);
    }

    /**
     * Selects the given chip.
     */
    void switchChipTo(Chip chip);

    /**
     * Disable the chips.
     */
    void disableChips() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_CS1_PIN, LOW);
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_CS2_PIN, LOW);
    }

    /**
     * Switch the rs pin to write.
     */
    void switchRwToWrite() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_RW_PIN, LOW);
    }

    /**
     * Switch the rs pin to read.
     */
    void switchRwToRead() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_RW_PIN, HIGH);
    }

    /**
     * Writes a byte to bus.
     * 
     * @param byte
     */
    void writeToBus(unsigned char b) {
        GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_LOW = (GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_LOW & 0x0f) | (b & 0xf0);
        GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_HIGH = (GRAPHICAL_LIQUID_CRYSTAL_BUS_PORT_NIBBLE_HIGH & 0xf0) | (b & 0x0f);
    }

    /**
     * Reads a byte from bus.
     * 
     * @return 
     */
    unsigned char readFromBus() {
        return (GRAPHICAL_LIQUID_CRYSTAL_BUS_PIN_NIBBLE_LOW & 0xf0) | (GRAPHICAL_LIQUID_CRYSTAL_BUS_PIN_NIBBLE_HIGH & 0x0f);
    }

    /**
     * Sets all bus pin as output.
     */
    void busOutputDirection() {
        GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_LOW |= 0xf0;
        GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_HIGH |= 0x0f;
    }

    /**
     * Sets all bus pin as input.
     */
    void busInputDirection() {
        GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_LOW &= 0x0f;
        GRAPHICAL_LIQUID_CRYSTAL_BUS_DDR_NIBBLE_HIGH &= 0xf0;
    }

    /**
     * Sets the enable pin.
     */
    void setEnablePin() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_EN_PIN, HIGH);
    }

    /**
     * Clears the enable pin.
     */
    void clrEnablePin() {
        digitalWrite(GRAPHICAL_LIQUID_CRYSTAL_EN_PIN, LOW);
    }
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_STRAIGHT_H__ */
