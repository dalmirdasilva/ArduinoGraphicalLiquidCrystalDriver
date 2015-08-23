/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystal.h
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_KS0108_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_KS0108_H__ 1

#include <GraphicalLiquidCrystal.h>

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH                                 64
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT                                64
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_HORIZONTAL_CHIPS                           2
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_VERTICAL_CHIPS                             1
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIPS                                      (GRAPHICAL_LIQUID_CRYSTAL_KS0108_HORIZONTAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_KS0108_VERTICAL_CHIPS)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_WIDTH                                      (GRAPHICAL_LIQUID_CRYSTAL_KS0108_HORIZONTAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_HEIGHT                                     (GRAPHICAL_LIQUID_CRYSTAL_KS0108_VERTICAL_CHIPS * GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_AREA                                  (GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH * GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_AREA                                       (GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_AREA * GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIPS)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_PAGES                                 (GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_HEIGHT / 8)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_PAGE_LINES                                 (GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIP_WIDTH)

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_RESET_BIT							0x10
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_OFF_BIT							    0x20
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STATUS_BUSY_BIT							    0x80

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_TIME_OUT_ON_WRITE_BIT                 0x10
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_PLOT_OUT_OF_RANGE_BIT                 0x20
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_FLAGS_READ_IN_ALL_CHIPS_BIT                 0x40


#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_USING_RESET                        1

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS1_PIN                            12
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CS2_PIN                            13

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RS_PIN                             3
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RW_PIN                             2
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_EN_PIN                             A0

#ifdef GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_USING_RESET
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_RESET_PIN                          13
#endif

/*
 * Arduino layout
 *
 * B (digital pin 8 to 13)
 * C (analog input pins)
 * D (digital pins 0 to 7)
 */
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PIN_NIBBLE_LOW                 PIND
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PIN_NIBBLE_HIGH                PINB

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_LOW                 DDRD
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_DDR_NIBBLE_HIGH                DDRB

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_LOW                PORTD
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_BUS_PORT_NIBBLE_HIGH               PORTB

// Data setup time TDSU ~= 300 ns (at 20Mhz it will be about 15 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TDSU_US                      0x0a

// Data hold time (write) TDHW ~= 15 ns (at 20Mhz it will be about 1 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TDHW_US                      0x0a

// Data delay time TDDR ~= 480 ns (at 20Mhz it will be about 25 cycles)
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_TD_US                        0x0a

// I don't know exactly how many cycles :/
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DELAY_RESET_US                     0x0a

#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_CHECK_FOR_BUSY_ON_WRITE            0x01
#define GRAPHICAL_LIQUID_CRYSTAL_KS0108_STRAIGHT_DEFAULT_ATTEMPTS_ON_BUSY           0x0a

class GraphicalLiquidCrystalKs0108: GraphicalLiquidCrystal {

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
    enum Command {
        COMMAND_DISPLAY_ON_OFF = 0x3e,
        COMMAND_DISPLAY_START_LINE = 0xc0,
        COMMAND_SET_PAGE = 0xb8,
        COMMAND_SET_ADDRESS = 0x40,
        COMMAND_DISPLAY_ON_OFF_ON = 0x01
    };

    enum Chip {
        CHIP_FIST = 0,
        CHIP_SECOND = 1,
        CHIP_ALL = 0xff
    };

    /**
     * Rw pin modes.
     */
    enum ReadWrite {
        WRITE_MODE = 0,
        READ_MODE = 1
    };

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

    /**
     * Checks if the reseting flags in set on the status.
     *
     * @param status
     * @return
     */
    bool inline isReseting(Chip chip);

    /**
     * Checks if the off flags in set on the status.
     * 
     * @param status
     * @return 
     */
    bool inline isOff(Chip chip);

    /**
     * Checks if the busy flags in set on the status.
     * 
     * @param status
     * @return 
     */
    bool inline isBusy(Chip chip);

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
     * @param   The line
     * @return  bool
     */
    void scrollTo(unsigned char line);
    /**
     * Scrolls the glcd.
     * 
     * @param direction     The scroll direction.
     * @param lines         How many lines will scroll.
     * @return void
     */
    void scroll(ScrollDirection direction, unsigned char lines);

    /**
     * Scrolls the glcd to the given line
     *
     * @param   The chip selector
     * @param   The line
     * @return  bool
     */
    void scrollToChip(Chip chip, unsigned char line);
    /**
     * Scrolls the glcd.
     *
     * @param chip          The chip selector.
     * @param direction     The scroll direction.
     * @param lines         How many lines will scroll.
     * @return void
     */
    void scrollChip(Chip chip, ScrollDirection direction, unsigned char lines);

    /**
     * Gets the status of the glcd.
     * 
     * @param chip          The chip selector.
     * @return  			Byte representing the status info.
     */
    unsigned char inline status(Chip chip);

    /**
     * Gets the write timeout flag.
     *
     * @return bool
     */
    virtual bool inline getWriteTimeoutFlag();

    /**
     * Gets the out of range flag.
     *
     * @return bool
     */
    bool inline getOutOfRangeFlag();

    /**
     * Gets the read in all chip flag.
     *
     * @return bool
     */
    bool inline getReadInAllChipsFlag();

protected:

    struct {
        unsigned char scrollTo :6;
    } startLine[GRAPHICAL_LIQUID_CRYSTAL_KS0108_CHIPS];

    /**
     * Protected constructor.
     */
    GraphicalLiquidCrystalKs0108();

    /**
     * Protected virtual destructor.
     */
    virtual ~GraphicalLiquidCrystalKs0108();

    /**
     * Gets a byte from the glcd.
     * 
     * @param chip          The chip selector.
     * @return  
     */
    unsigned char inline readData(Chip chip);

    /**
     * Sends data to the glcd.
     * 
     * @param chip  		The chip selector.
     * @param b 			The data to be sent.
     * @return
     */
    bool inline writeData(Chip chip, unsigned char b);

    /**
     * Sends a command to the glcd.
     * 
     * @param chip  		The chip selector.
     * @param cmd   		The command to be sent.
     * @return
     */
    bool inline command(Chip chip, unsigned char cmd);

    /**
     * Gets the chip from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getChipFromPoint(unsigned char x, unsigned char y);

    /**
     * Gets the page from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getPageFromPoint(unsigned char x, unsigned char y);

    /**
     * Gets the line from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getLineFromPoint(unsigned char x, unsigned char y);

    /**
     * Gets the page bit from a point.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    unsigned char inline getBitFromPoint(unsigned char x, unsigned char y);

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
    void inline setWriteTimeoutFlag();

    /**
     * Clears the write timeout flag.
     */
    void inline clrWriteTimeoutFlag();

    /**
     * Sets the out of range flag.
     */
    void inline setOutOfRangeFlag();

    /**
     * Clears the out of range flag.
     */
    void inline clrOutOfRangeFlag();

    /**
     * Sets the read in all chip flag.
     */
    void inline setReadInAllChipsFlag();

    /**
     * Clears the read in all chip flag.
     */
    void inline clrReadInAllChipsFlag();

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
    void inline switchRegisterSelectToData();

    /**
     * Switch the register select pin to command mode.
     */
    void inline switchRegisterSelectToCommand();

    /**
     * Selects the given chip.
     */
    void inline switchChipTo(Chip chip);

    /**
     * Disable the chips.
     */
    void inline disableChips();

    /**
     * Switch the rs pin to write.
     */
    void inline switchReadWriteToWrite();

    /**
     * Switch the rs pin to read.
     */
    void inline switchReadWriteToRead();

    /**
     * Writes a byte to bus.
     *
     * @param byte
     */
    void inline writeToBus(unsigned char b);

    /**
     * Reads a byte from bus.
     *
     * @return
     */
    unsigned char inline readFromBus();

    /**
     * Sets all bus pin as output.
     */
    void inline busOutputDirection();

    /**
     * Sets all bus pin as input.
     */
    void inline busInputDirection();

    /**
     * Sets the enable pin.
     */
    void inline setEnablePin();

    /**
     * Clears the enable pin.
     */
    void inline clearEnablePin();
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_KS0108_H__ */
