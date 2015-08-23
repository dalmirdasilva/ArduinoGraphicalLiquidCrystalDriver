/**
 * Arduino Graphical Liquid Crystal Driver
 * 
 * GraphicalLiquidCrystal.h
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__
#define __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__ 1

class GraphicalLiquidCrystal {

public:

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

    /**
     * Direction of the scroll.
     */
    enum ScrollDirection {
        SCROLL_UP = 0,
        SCROLL_DOWN = 1
    };

    /**
     * Register select pin modes.
     */
    enum RegisterSelect {
        REGISTER_SELECT_COMMAND = 0,
        REGISTER_SELECT_DATA = 1
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
     * Fill all the buffer with the given pattern
     * 
     * @param   Pattern
     */
    virtual void screen(unsigned char pattern) = 0;

    /**
     * Turns a pixel on or off.
     * 
     * @param x             The x position.
     * @param y             The y position.
     * @param color         The color.
     * @return bool
     */
    virtual bool plot(unsigned char x, unsigned char y, Color color) = 0;

    /**
     * Writes a entire byte at the page and line
     * 
     * @param x
     * @param line
     * @param chunk
     * @return 
     */
    virtual bool streak(unsigned char x, unsigned char line, unsigned char streak) = 0;

    /**
     * Scrolls the glcd to the given line
     *
     * @param   The line
     * @return  bool
     */
    virtual void scrollTo(unsigned char line) = 0;

    /**
     * Scrolls the glcd.
     * 
     * @param chip          The chip selector.
     * @param direction     The scroll direction.
     * @param lines         How many lines will scroll.
     * @return void
     */
    virtual void scroll(ScrollDirection direction, unsigned char lines) = 0;

    /**
     * Checks if the given point is out of range.
     * 
     * @param x			The X position on the screen.
     * @param y			The Y position on the screen.
     * @return 
     */
    bool isOutOfRange(unsigned char x, unsigned char y);

    /**
     * Gets the glcd width.
     * 
     * @return 
     */
    unsigned char getWidth();

    /**
     * Gets the glcd height.
     * 
     * @return 
     */
    unsigned char getHeight();

    /**
     * Clears the display.
     */
    void inline clear();

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

    /**
     * Module width.
     */
    unsigned char width;

    /**
     * Module height.
     */
    unsigned char height;

    /**
     * Protected constructor.
     */
    GraphicalLiquidCrystal(unsigned char width, unsigned char height);

    /**
     * Virtual destructor.
     */
    virtual ~GraphicalLiquidCrystal();
};

#endif /* __ARDUINO_DRIVER_GRAPHICAL_LIQUID_CRYSTAL_H__ */
