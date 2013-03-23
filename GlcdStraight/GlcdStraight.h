/**
 * Arduino - Glcd driver
 * 
 * GlcdStraight.h
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

#ifndef __ARDUINO_DRIVER_GLCD_STRAIGHT_H__
#define __ARDUINO_DRIVER_GLCD_STRAIGHT_H__ 1

#include <Glcd.h>

#define GLCD_CS1_PIN                                    12
#define GLCD_CS2_PIN                                    13

#define GLCD_RS_PIN                                     3
#define GLCD_RW_PIN                                     2
#define GLCD_EN_PIN                                     A0

#ifdef GLCD_USING_RESET
#define GLCD_RESET_PIN                                  13
#endif

/*
 * Arduino layout
 * 
 * B (digital pin 8 to 13)
 * C (analog input pins)
 * D (digital pins 0 to 7)
 */
#define GLCD_BUS_PIN_NIBBLE_LOW                         PIND
#define GLCD_BUS_PIN_NIBBLE_HIGH                        PINB

#define GLCD_BUS_DDR_NIBBLE_LOW                         DDRD
#define GLCD_BUS_DDR_NIBBLE_HIGH                        DDRB

#define GLCD_BUS_PORT_NIBBLE_LOW                        PORTD
#define GLCD_BUS_PORT_NIBBLE_HIGH                       PORTB

// Data setup time TDSU ~= 300 ns (at 20Mhz it will be about 15 cycles)
#define GLCD_DELAY_TDSU_US	                            0x0a

// Data hold time (write) TDHW ~= 15 ns (at 20Mhz it will be about 1 cycles)
#define GLCD_DELAY_TDHW_US	                            0x0a

// Data delay time TDDR ~= 480 ns (at 20Mhz it will be about 25 cycles)
#define GLCD_DELAY_TD_US	                            0x0a

// I don't know exactly how many cycles :/
#define GLCD_DELAY_RESET_US								0x0a

#define GLCD_CHECK_FOR_BUSY_ON_WRITE                    0x00
#define GLCD_DEFAULT_ATTEMPTS_ON_BUSY                   0x0a


class GlcdStraight : public Glcd {
public:

    GlcdStraight();
    
    /**
     * Initializes the glcd.
     * 
     * @param mode			On or Off.
     */
    void init(Mode mode);
    
    /**
     * Issues a resert int the glcd module
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
     * Writes a byte into the glcd.
     * 
     * @param chip              The chip selector.
     * @param b                 The byte to be written.
     * @param rs                The register select.
     * @return 
     */
    bool write(Chip chip, unsigned char b, RegisterSelect rs);

    /**
     * Reads a byte from the glcd.
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
        digitalWrite(GLCD_RS_PIN, HIGH);
    }

    /**
     * Switch the register select pin to command mode.
     */
    void switchRegisterSelectToCommand() {
        digitalWrite(GLCD_RS_PIN, LOW);
    }

    /**
     * Selects the given chip.
     */
    void switchChipTo(Chip chip);

    /**
     * Disable the chips.
     */
    void disableChips() {
        digitalWrite(GLCD_CS1_PIN, LOW);
        digitalWrite(GLCD_CS2_PIN, LOW);
    }

    /**
     * Switch the rs pin to write.
     */
    void switchRwToWrite() {
        digitalWrite(GLCD_RW_PIN, LOW);
    }

    /**
     * Switch the rs pin to read.
     */
    void switchRwToRead() {
        digitalWrite(GLCD_RW_PIN, HIGH);
    }

    /**
     * Writes a byte to bus.
     * 
     * @param byte
     */
    void writeToBus(unsigned char b) {
        GLCD_BUS_PORT_NIBBLE_LOW = (GLCD_BUS_PORT_NIBBLE_LOW & 0x0f) | (b & 0xf0);
        GLCD_BUS_PORT_NIBBLE_HIGH = (GLCD_BUS_PORT_NIBBLE_HIGH & 0xf0) | (b & 0x0f);
    }

    /**
     * Reads a byte from bus.
     * 
     * @return 
     */
    unsigned char readFromBus() {
        return (GLCD_BUS_PIN_NIBBLE_LOW & 0xf0) | (GLCD_BUS_PIN_NIBBLE_HIGH & 0x0f);
    }

    /**
     * Sets all bus pin as output.
     */
    void busOutputDirection() {
        GLCD_BUS_DDR_NIBBLE_LOW |= 0xf0;
        GLCD_BUS_DDR_NIBBLE_HIGH |= 0x0f;
    }

    /**
     * Sets all bus pin as input.
     */
    void busInputDirection() {
        GLCD_BUS_DDR_NIBBLE_LOW &= 0x0f;
        GLCD_BUS_DDR_NIBBLE_HIGH &= 0xf0;
    }

    /**
     * Sets the enable pin.
     */
    void setEnablePin() {
        digitalWrite(GLCD_EN_PIN, HIGH);
    }

    /**
     * Clears the enable pin.
     */
    void clrEnablePin() {
        digitalWrite(GLCD_EN_PIN, LOW);
    }
};

#endif /* __ARDUINO_DRIVER_GLCD_STRAIGHT_H__ */
