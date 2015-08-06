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

#ifndef __ARDUINO_DRIVER_GLCD_WIRE_H__
#define __ARDUINO_DRIVER_GLCD_WIRE_H__ 1

#include <Glcd.h>

class GlcdWire : public Glcd {
protected:

	unsigned char device;
    
    struct {
        unsigned char page;
        unsigned char line;
    } chipInfo[GLCD_CHIPS];
	
public:
	
    /**
     * Public constructor.
     * 
     * @param address       The interface address.
     */
	GlcdWire(unsigned char device);
    
    /**
     * Initializes the glcd.
     * 
     * @param mode			On or Off.
     */
    void init(Mode mode);
    
    /**
     * Issues a reset int the glcd module
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
     * Makes the header of the pic i2c communication.
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

#endif /* __ARDUINO_DRIVER_GLCD_WIRE_H__ */

