/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystal.h
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__ 1

#include <Arduino.h>

#define GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH                                 64
#define GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT                                64
#define GRAPHICAL_LIQUID_CRYSTAL_HORIZONTAL_CHIPS                           2
#define GRAPHICAL_LIQUID_CRYSTAL_VERTICAL_CHIPS                             1
#define GRAPHICAL_LIQUID_CRYSTAL_CHIPS                                      (GRAPHICAL_LIQUID_CRYSTAL_HORIZONTAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_VERTICAL_CHIPS)
#define GRAPHICAL_LIQUID_CRYSTAL_WIDTH                                      (GRAPHICAL_LIQUID_CRYSTAL_HORIZONTAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH)
#define GRAPHICAL_LIQUID_CRYSTAL_HEIGHT                                     (GRAPHICAL_LIQUID_CRYSTAL_VERTICAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT)
#define GRAPHICAL_LIQUID_CRYSTAL_CHIP_AREA                                  (GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH * GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT)
#define GRAPHICAL_LIQUID_CRYSTAL_AREA                                       (GRAPHICAL_LIQUID_CRYSTAL_CHIP_AREA * GRAPHICAL_LIQUID_CRYSTAL_CHIPS)
#define GRAPHICAL_LIQUID_CRYSTAL_CHIP_PAGES                                 (GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT / 8)
#define GRAPHICAL_LIQUID_CRYSTAL_PAGE_LINES                                 (GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH)

#define GRAPHICAL_LIQUID_CRYSTAL_STATUS_RESET_BIT							0x10
#define GRAPHICAL_LIQUID_CRYSTAL_STATUS_OFF_BIT							    0x20
#define GRAPHICAL_LIQUID_CRYSTAL_STATUS_BUSY_BIT							0x80

#define GRAPHICAL_LIQUID_CRYSTAL_FLAGS_TIME_OUT_ON_WRITE_BIT				0x10
#define GRAPHICAL_LIQUID_CRYSTAL_FLAGS_PLOT_OUT_OF_RANGE_BIT				0x20
#define GRAPHICAL_LIQUID_CRYSTAL_FLAGS_READ_IN_ALL_CHIPS_BIT				0x40

class GraphicalLiquidCrystal {

public:

    /**
     * <pre>
     * Command                   Binary                             Hex
     *                           D7  D6  D5  D4  D3  D2  D1  D0 
     * Display on/off             0   0   1   1   1   1   1  1/0    3e or 3f
     * Display start line         1   1   A   A   A   A   A   A     c0 or ff
     * Set page                   1   0   1   1   1   A   A   A     b8 to bf
     * Set address                0   1   A   A   A   A   A   A     40 to 7f
     * Status read                B   0   S   R   0   0   0   0     
     * 
     * B: 1=Busy, 0=Not busy 
     * S: 1=On, 0=Off 
     * R: 1=Reset
     * A: Address 
     * 
     * x = Don't care
     * </pre>
     */
    enum Cmd {
        CMD_DISPLAY_ON_OFF = 0x3e,
        CMD_DISPLAY_START_LINE = 0xc0,
        CMD_SET_PAGE = 0xb8,
        CMD_SET_ADDRESS = 0x40,
        CMD_DISPLAY_ON_OFF_ON = 0x01
    };

    /**
     * initialization mode.
     */
    enum Mode {
        MODE_OFF = 0,
        MODE_ON = 1
    };

    /**
     * GraphicalLiquidCrystal color.
     */
    enum Color {
        COLOR_BLACK = 0x00,
        COLOR_WHITE = 0xff
    };

    enum Chip {
        CHIP_1 = 0,
        CHIP_2 = 1,
        CHIP_ALL = 0xff
    };

    /**
     * Rw pin modes.
     */
    enum Rw {
        RW_WRITE = 0,
        RW_READ = 1
    };

    /**
     * Direction of the scroll.
     */
    enum ScrollDirection {
        SCROLL_UP = 0,
        SCROLL_DOWN = 1
    };

    /**
     * Rs pin modes.
     */
    enum RegisterSelect {
        RS_COMMAND = 0,
        RS_DATA = 1
    };

    /**
     * Initializes the glcd.
     * 
     * @param mode			On or Off.
     */
    virtual void init(Mode mode) = 0;

    /**
     * Issues a resert int the glcd module
     * 
     * @return  void    
     */
    virtual void reset() = 0;

    /**
     * Checks if the reseting flags in set on the status.
     * 
     * @param status
     * @return 
     */
    bool isReseting(Chip chip) {
        return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_STATUS_RESET_BIT) != 0);
    }

    /**
     * Checks if the off flags in set on the status.
     * 
     * @param status
     * @return 
     */
    bool isOff(Chip chip) {
        return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_STATUS_OFF_BIT) != 1);
    }

    /**
     * Checks if the busy flags in set on the status.
     * 
     * @param status
     * @return 
     */
    bool isBusy(Chip chip) {
        return ((status(chip) & GRAPHICAL_LIQUID_CRYSTAL_STATUS_BUSY_BIT) != 0);
    }

    /**
     * Fill all the buffer with the given pattern
     * 
     * @param   Pattern
     */
    void screen(unsigned char pattern);

    /**
     * Turns a pixel on or off.
     * 
     * @param x             The x position.
     * @param y             The y position.
     * @param color         The color.
     * @return bool
     */
    bool plot(unsigned char x, unsigned char y, Color color);

    /**
     * Writes a entire byte at the page and line
     * 
     * @param line
     * @param page
     * @param chunk
     * @return 
     */
    bool streak(unsigned char x, unsigned char page, unsigned char streak);

    /**
     * Scrolls the glcd to the given line
     * 
     * @param   The chip selector
     * @param   The line
     * @return  bool
     */
    void scrollTo(Chip chip, unsigned char line) {
        command(chip, GraphicalLiquidCrystal::CMD_DISPLAY_START_LINE | (line & 0x3f));
    }

    /**
     * Scrolls the glcd.
     * 
     * @param chip          The chip selector.
     * @param direction     The scroll direction.
     * @param lines         How many lines will scroll.
     * @return void
     */
    void scroll(Chip chip, ScrollDirection direction, unsigned char lines);

    /**
     * Gets the status of the glcd.
     * 
     * @param chip          The chip selector.
     * @return  			Byte representing the status info.
     */
    unsigned char inline status(Chip chip) {
        return read(chip, GraphicalLiquidCrystal::RS_COMMAND);
    }

    /**
     * Gets the write timeout flag.
     * 
     * @return bool
     */
    bool inline getWriteTimeoutFlag() {
        return ((flags & GRAPHICAL_LIQUID_CRYSTAL_FLAGS_TIME_OUT_ON_WRITE_BIT) != 0);
    }

    /**
     * Gets the out of range flag.
     * 
     * @return bool
     */
    bool inline getOutOfRangeFlag() {
        return ((flags & GRAPHICAL_LIQUID_CRYSTAL_FLAGS_TIME_OUT_ON_WRITE_BIT) != 0);
    }

    /**
     * Gets the read in all chip flag.
     * 
     * @return bool
     */
    bool inline getReadInAllChipsFlag() {
        return ((flags & GRAPHICAL_LIQUID_CRYSTAL_FLAGS_READ_IN_ALL_CHIPS_BIT) != 0);
    }

    /**
     * Checks if the given point is out of range.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    bool inline isOutOfRange(unsigned char x, unsigned char y) {
        return (x > GRAPHICAL_LIQUID_CRYSTAL_WIDTH || y > GRAPHICAL_LIQUID_CRYSTAL_HEIGHT);
    }

    /**
     * Gets the glcd width.
     * 
     * @return 
     */
    unsigned char inline getWidth() {
        return GRAPHICAL_LIQUID_CRYSTAL_WIDTH;
    }

    /**
     * Gets the glcd height.
     * 
     * @return 
     */
    unsigned char inline getHeight() {
        return GRAPHICAL_LIQUID_CRYSTAL_HEIGHT;
    }

    /**
     * Clears the display.
     */
    void inline clear() {
        screen(0x00);
    }

protected:

    /**
     * <pre>
     * 0b00000000
     *   ||||||||_ Timeout on write operation
     *   |||||||__ Plot out of range
     *   ||||||___ Read all chip at same time
     *   |||||____ Unused
     *   ||||_____ Unused
     *   |||______ Unused
     *   ||_______ Unused
     *   |________ Unused 
     * </pre>
     */
    unsigned char flags;

    struct {
        unsigned char scrollTo :6;
    } startLine[GRAPHICAL_LIQUID_CRYSTAL_CHIPS];

    /**
     * Protected constructor.
     */
    GraphicalLiquidCrystal();

    /**
     * Protected virtual destructor.
     */
    virtual ~GraphicalLiquidCrystal();

    /**
     * Initializes the IO.
     */
    virtual void initIo();

    /**
     * Writes a byte into the glcd.
     * 
     * @param chip              The chip selector.
     * @param b                 The byte to be written.
     * @param rs                The register select.
     * @return 
     */
    virtual bool write(Chip chip, unsigned char b, RegisterSelect rs) = 0;

    /**
     * Reads a byte from the glcd.
     * 
     * @param chip              The chip selector.
     * @param rs                The register select.
     * @return 
     */
    virtual unsigned char read(Chip chip, RegisterSelect rs) = 0;

    /**
     * Gets a byte from the glcd.
     * 
     * @param chip          The chip selector.
     * @return  
     */
    unsigned char inline readData(Chip chip) {
        return read(chip, GraphicalLiquidCrystal::RS_DATA);
    }

    /**
     * Sends data to the glcd.
     * 
     * @param chip  		The chip selector.
     * @param b 			The data to be sent.
     * @return
     */
    bool inline writeData(Chip chip, unsigned char b) {
        return write(chip, b, GraphicalLiquidCrystal::RS_DATA);
    }

    /**
     * Sends a command to the glcd.
     * 
     * @param chip  		The chip selector.
     * @param cmd   		The command to be sent.
     * @return
     */
    bool inline command(Chip chip, unsigned char cmd) {
        return write(chip, cmd, GraphicalLiquidCrystal::RS_COMMAND);
    }

    /**
     * Gets the chip from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getChipFromPoint(unsigned char x, unsigned char y) {
        return ((y / GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT) * GRAPHICAL_LIQUID_CRYSTAL_HORIZONTAL_CHIPS) + (x / GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH);
    }

    /**
     * Gets the page from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getPageFromPoint(unsigned char x, unsigned char y) {
        return (y % GRAPHICAL_LIQUID_CRYSTAL_CHIP_HEIGHT) / 8;
    }

    /**
     * Gets the line from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getLineFromPoint(unsigned char x, unsigned char y) {
        return x % GRAPHICAL_LIQUID_CRYSTAL_CHIP_WIDTH;
    }

    /**
     * Gets the page bit from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getBitFromPoint(unsigned char x, unsigned char y) {
        return y % 8;
    }

    /**
     * Sends data to the glcd by the given chip, page and line.
     * 
     * @param chip              The chip selector.
     * @param page 				The page selector.
     * @param page 				The line selector.
     * @param data 				The data to be sent.
     * @return
     */
    bool writeDataAt(Chip chip, unsigned char page, unsigned char line, unsigned char byte);

    /**
     * Gets a byte from the glcd.
     * 
     * @param chip              The chip selector.
     * @param page 				The page selector.
     * @param page 				The line selector.
     * @return
     */
    unsigned char readDataAt(Chip chip, unsigned char page, unsigned char line);

    /**
     * Sets the write timeout flag.
     */
    void inline setWriteTimeoutFlag() {
        flags |= GRAPHICAL_LIQUID_CRYSTAL_FLAGS_TIME_OUT_ON_WRITE_BIT;
    }

    /**
     * Clears the write timeout flag.
     */
    void inline clrWriteTimeoutFlag() {
        flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_FLAGS_TIME_OUT_ON_WRITE_BIT);
    }

    /**
     * Sets the out of range flag.
     */
    void inline setOutOfRangeFlag() {
        flags |= GRAPHICAL_LIQUID_CRYSTAL_FLAGS_PLOT_OUT_OF_RANGE_BIT;
    }

    /**
     * Clears the out of range flag.
     */
    void inline clrOutOfRangeFlag() {
        flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_FLAGS_PLOT_OUT_OF_RANGE_BIT);
    }

    /**
     * Sets the read in all chip flag.
     */
    void inline setReadInAllChipsFlag() {
        flags |= GRAPHICAL_LIQUID_CRYSTAL_FLAGS_READ_IN_ALL_CHIPS_BIT;
    }

    /**
     * Clears the read in all chip flag.
     */
    void inline clrReadInAllChipsFlag() {
        flags &= ~(GRAPHICAL_LIQUID_CRYSTAL_FLAGS_READ_IN_ALL_CHIPS_BIT);
    }
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__ */
