/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * Controller Philp PCD8544 LCD controller
 *
 * GraphicalLiquidCrystal.h
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_PCD8544_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_PCD8544_H__ 1

#define GRAPHICAL_LIQUID_CRYSTAL_PCD8544_WIDTH           0x54
#define GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT          0x30
#define GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES    GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT / 8
#define GRAPHICAL_LIQUID_CRYSTAL_PCD8544_BYTE_SIZE       GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES * 0x54

#include <GraphicalLiquidCrystal.h>

/**
 * The instruction format is divided into two modes: If D/C
 * (mode select) is set LOW, the current byte is interpreted as
 * command byte (see Table 1). Figure 8 shows an example
 * of a serial data stream for initializing the chip. If D/C is set
 * HIGH, the following bytes are stored in the display data
 * RAM. After every data byte, the address counter is
 * incremented automatically.
 *
 * The level of the D/C signal is read during the last bit of data
 * byte.
 *
 * Each instruction can be sent in any order to the PCD8544.
 * The MSB of a byte is transmitted first. Figure 9 shows one
 * possible command stream, used to set up the LCD driver.
 *
 * The serial interface is initialized when SCE is HIGH. In this
 * state, SCLK clock pulses have no effect and no power is
 * consumed by the serial interface. A negative edge on SCE
 * enables the serial interface and indicates the start of a data
 * transmission.
 */
class GraphicalLiquidCrystalPCD8544: public GraphicalLiquidCrystal {

    struct Control {
        unsigned char funtionSet;
        unsigned char displayControl;
    };

    /**
     * Data pin.
     */
    unsigned char dataPin;

    /**
     * Clock pin.
     */
    unsigned char clockPin;

    /**
     * RES pin.
     */
    unsigned char rstPin;

    /**
     * Data/Command pin.
     */
    unsigned char dcPin;

    /**
     * Chip Enable pin.
     */
    unsigned char scePin;

    /**
     * Control bytes
     */
    Control currentControl;

    /**
     * Display buffer
     */
    unsigned char buffer[GRAPHICAL_LIQUID_CRYSTAL_PCD8544_HEIGHT_PAGES][GRAPHICAL_LIQUID_CRYSTAL_PCD8544_WIDTH];

public:

    unsigned char currentScroll;

    /**
     * <pre>
     * Command                   Binary                             Hex
     *                           D7  D6  D5  D4  D3  D2  D1  D0
     * Function set               0   0   1   0   0   PD  V   H     2*
     *
     * (H = 0)
     * Display control            0   0   0   0   1   D   0   E     08 | 0*
     * Set Y address              0   1   0   0   1   Y   Y   Y     48 | 0*
     * Set X address              1   X   X   X   X   X   X   X     80 | **
     *
     * (H = 1)
     * Temperature control        0   0   0   0   0   1   TC  TC    04 | 0*
     * Bias system                0   0   0   1   0   BS  BS  BS    10 | 0*
     * Set V OP                   1   V   V   V   V   V   V   V     80 | 0*
     *
     * PD: 1=chip is in Power-down mode, 0=chip is active
     * V: 1=vertical addressing, 0=horizontal addressing
     * H: 1=use extended instruction set, 0=use basic instruction set
     * Y: Y address bit
     * X: X address bit
     * D and E:
     *      00: display blank
     *      01: normal mode
     *      10: all display segments on
     *      11: inverse video mode
     * TC: Temperature coefficient
     * x = Don't care
     * </pre>
     */
    enum Command {
        COMMAND_FUNCTION_SET = 0x20,
        COMMAND_FUNCTION_SET_POWER_DOWN = 0x04,
        COMMAND_FUNCTION_SET_ACTIVE = 0x00,
        COMMAND_FUNCTION_SET_VERTICAL_ADDRESSING = 0x02,
        COMMAND_FUNCTION_SET_HORIZONTAL_ADDRESSING = 0x00,
        COMMAND_FUNCTION_SET_EXTENDED_INSTRUCTIONS = 0x01,
        COMMAND_FUNCTION_SET_BASIC_INSTRUCTIONS = 0x00,

        COMMAND_DISPLAY_CONTROL = 0x08,
        COMMAND_DISPLAY_CONTROL_BLANK = 0x00,
        COMMAND_DISPLAY_CONTROL_NORMAL = 0x04,
        COMMAND_DISPLAY_CONTROL_ALL_ON = 0x01,
        COMMAND_DISPLAY_CONTROL_INVERSE = 0x05,

        COMMAND_SET_Y_ADDRESS = 0x40,
        COMMAND_SET_X_ADDRESS = 0x80,

        COMMAND_TEMPERATURE_CONTROL = 0x04,
        COMMAND_BIAS_SYSTEM = 0x10,
        COMMAND_SET_VOP = 0x80
    };

    /**
     * Public constructor.
     */
    GraphicalLiquidCrystalPCD8544(unsigned char dataPin, unsigned char clockPin, unsigned char rstPin, unsigned char dcPin, unsigned char scePin);

    /**
     * Initializes the glcd.
     *
     * Immediately following power-on, the contents of all internal
     * registers and of the RAM are undefined. A RES pulse
     * must be applied. Attention should be paid to the
     * possibility that the device may be damaged if not properly
     * reset.
     *
     * All internal registers are reset by applying an external RES
     * pulse (active LOW) at pad 31, within the specified time.
     * However, the RAM contents are still undefined. The state
     * after reset is described in Section 8.2.
     *
     * The RES input must be ≤0.3V DD when V DD reaches V DDmin
     * (or higher) within a maximum time of 100 ms after V DD
     * goes HIGH (see Fig.16).
     *
     * @param mode          On or Off.
     */
    void init(Mode mode);

    /**
     * Issues a resert int the glcd module
     * 
     * @return  void    
     */
    void reset();

    /**
     * Send all buffer
     */
    void sync();

    /**
     * System bias.
     *
     * @param bias
     */
    void setBias(unsigned char bias);

    /**
     * Adjust display contrast.
     *
     * @param contrast
     */
    void setContrast(unsigned char contrast);

    /**
     * Fill all the buffer with the given pattern
     * 
     * @param   Pattern
     */
    void screen(unsigned char pattern);

    /**
     * Page from y
     */
    inline unsigned char getPageFromPoint(unsigned char x, unsigned char y);

    /**
     * Bit from y
     */
    inline unsigned char getBitFromPoint(unsigned char x, unsigned char y);

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
     * @param x
     * @param line
     * @param chunk
     * @return 
     */
    bool streak(unsigned char x, unsigned char line, unsigned char streak);

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
     * @param chip          The chip selector.
     * @param direction     The scroll direction.
     * @param lines         How many lines will scroll.
     * @return void
     */
    void scroll(ScrollDirection direction, unsigned char lines);

    /**
     * Send a command to the module.
     */
    void command(unsigned char command);

protected:

    /**
     * Writes a byte into the module.
     *
     * @param b                 The byte to be written.
     * @return
     */
    inline void send(unsigned char b);

    /**
     * Switch the register select pin to data mode.
     */
    inline void switchRegisterSelectToData();

    /**
     * Switch the register select pin to command mode.
     */
    inline void switchRegisterSelectToCommand();

    /**
     * The enable pin allows data to be clocked in. The signal is active LOW.
     */
    inline void enableChip();

    /**
     * The enable pin allows data to be clocked in. The signal is active LOW.
     */
    inline void disableChip();
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_PCD8544_H__ */
