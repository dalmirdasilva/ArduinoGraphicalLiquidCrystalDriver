/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystalWire.h
 * 
 * The header file for glcd driver, wicth implements the driver base
 * using i2c with a PIC microcontroller.
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_WIRE_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_WIRE_H__ 1

#include <GraphicalLiquidCrystal.h>

class GraphicalLiquidCrystalWire: public GraphicalLiquidCrystal {

protected:

    unsigned char device;

    struct {
        unsigned char page;
        unsigned char line;
    } chipInfo[GRAPHICAL_LIQUID_CRYSTAL_CHIPS];

public:

    /**
     * Public constructor.
     * 
     * @param address       The interface address.
     */
    GraphicalLiquidCrystalWire(unsigned char device);

    /**
     * Initializes the module.
     * 
     * @param mode			On or Off.
     */
    void init(Mode mode);

    /**
     * Issues a reset to the module.
     * 
     * @return  void
     */
    void reset();

    /**
     * Writes a byte into the glcd. BLOCKING!
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

private:

    /**
     * Makes the header of the i2c communication.
     * 
     * <pre>
     * header: 0b00000000
     *           ||||||||_ Chip b0 \: the chip, 11 means all chips
     *           |||||||__ Chip b1 /
     *           ||||||___ Register Select: 1 means data, 0 means command
     *           |||||____ Read/Write: 1 means read, 0 means write
     *           ||||_____ Unused
     *           |||______ Unused
     *           ||_______ Unused
     *           |________ Unused
     * </pre>
     * 
     * @param chip          The chip.
     * @param rs            The register select.
     * @param rw            Read/Write.
     * @return              unsigned char
     */
    unsigned char makeHeader(Chip chip, RegisterSelect rs, Rw rw);
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_WIRE_H__ */

