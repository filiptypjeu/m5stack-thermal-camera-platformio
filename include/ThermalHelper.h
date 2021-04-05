#pragma once

#include "env.h"

#define PXS AMG_COLS*AMG_ROWS
#define IPXS INTERPOLATED_ROWS*INTERPOLATED_COLS

// Colors that makes up the whole gradient
#define THERMAL_GRADIENT_N 256
constexpr const uint16_t colorGradient[THERMAL_GRADIENT_N] = {
    0x480F, 0x400F, 0x400F, 0x400F, 0x4010, 0x3810, 0x3810, 0x3810, 0x3810, 0x3010, 
    0x3010, 0x3010, 0x2810, 0x2810, 0x2810, 0x2810, 0x2010, 0x2010, 0x2010, 0x1810, 
    0x1810, 0x1811, 0x1811, 0x1011, 0x1011, 0x1011, 0x0811, 0x0811, 0x0811, 0x0011, 
    0x0011, 0x0011, 0x0011, 0x0011, 0x0031, 0x0031, 0x0051, 0x0072, 0x0072, 0x0092, 
    0x00B2, 0x00B2, 0x00D2, 0x00F2, 0x00F2, 0x0112, 0x0132, 0x0152, 0x0152, 0x0172, 
    0x0192, 0x0192, 0x01B2, 0x01D2, 0x01F3, 0x01F3, 0x0213, 0x0233, 0x0253, 0x0253, 
    0x0273, 0x0293, 0x02B3, 0x02D3, 0x02D3, 0x02F3, 0x0313, 0x0333, 0x0333, 0x0353, 
    0x0373, 0x0394, 0x03B4, 0x03D4, 0x03D4, 0x03F4, 0x0414, 0x0434, 0x0454, 0x0474, 
    0x0474, 0x0494, 0x04B4, 0x04D4, 0x04F4, 0x0514, 0x0534, 0x0534, 0x0554, 0x0554, 
    0x0574, 0x0574, 0x0573, 0x0573, 0x0573, 0x0572, 0x0572, 0x0572, 0x0571, 0x0591, 
    0x0591, 0x0590, 0x0590, 0x058F, 0x058F, 0x058F, 0x058E, 0x05AE, 0x05AE, 0x05AD, 
    0x05AD, 0x05AD, 0x05AC, 0x05AC, 0x05AB, 0x05CB, 0x05CB, 0x05CA, 0x05CA, 0x05CA, 
    0x05C9, 0x05C9, 0x05C8, 0x05E8, 0x05E8, 0x05E7, 0x05E7, 0x05E6, 0x05E6, 0x05E6, 
    0x05E5, 0x05E5, 0x0604, 0x0604, 0x0604, 0x0603, 0x0603, 0x0602, 0x0602, 0x0601, 
    0x0621, 0x0621, 0x0620, 0x0620, 0x0620, 0x0620, 0x0E20, 0x0E20, 0x0E40, 0x1640, 
    0x1640, 0x1E40, 0x1E40, 0x2640, 0x2640, 0x2E40, 0x2E60, 0x3660, 0x3660, 0x3E60, 
    0x3E60, 0x3E60, 0x4660, 0x4660, 0x4E60, 0x4E80, 0x5680, 0x5680, 0x5E80, 0x5E80, 
    0x6680, 0x6680, 0x6E80, 0x6EA0, 0x76A0, 0x76A0, 0x7EA0, 0x7EA0, 0x86A0, 0x86A0, 
    0x8EA0, 0x8EC0, 0x96C0, 0x96C0, 0x9EC0, 0x9EC0, 0xA6C0, 0xAEC0, 0xAEC0, 0xB6E0, 
    0xB6E0, 0xBEE0, 0xBEE0, 0xC6E0, 0xC6E0, 0xCEE0, 0xCEE0, 0xD6E0, 0xD700, 0xDF00, 
    0xDEE0, 0xDEC0, 0xDEA0, 0xDE80, 0xDE80, 0xE660, 0xE640, 0xE620, 0xE600, 0xE5E0, 
    0xE5C0, 0xE5A0, 0xE580, 0xE560, 0xE540, 0xE520, 0xE500, 0xE4E0, 0xE4C0, 0xE4A0, 
    0xE480, 0xE460, 0xEC40, 0xEC20, 0xEC00, 0xEBE0, 0xEBC0, 0xEBA0, 0xEB80, 0xEB60, 
    0xEB40, 0xEB20, 0xEB00, 0xEAE0, 0xEAC0, 0xEAA0, 0xEA80, 0xEA60, 0xEA40, 0xF220, 
    0xF200, 0xF1E0, 0xF1C0, 0xF1A0, 0xF180, 0xF160, 0xF140, 0xF100, 0xF0E0, 0xF0C0, 
    0xF0A0, 0xF080, 0xF060, 0xF040, 0xF020, 0xF800,
};


class ThermalHelper {
    public:
        ThermalHelper();

        /**
         * Start the thermal camera. This will draw the initial frame, and set up all variables. Call update() to update the view.
         */
        void begin();

        /**
         * Draw a new frame.
         */
        void update();


    private:
        /**
         * Handle button presses.
         */
        void handleButtons();

        /**
         * Draw the interpolated pixels of a new frame.
         */
        void drawPixels();

        /**
         * Highlight global minimum and maximum interpolated pixels.
         */
        void drawPixelHighlights();

        /**
         * Draw everything that should never change after the first frame.
         */
        void drawStaticInfo();

        /**
         * Draw the color gradient map.
         */
        void drawGradient();

        /**
         * Draw the temperatures for the color gradient map.
         */
        void drawGradientTemperatures();

        /**
         * Draw a cursor.
         */
        void drawCursor(int16_t x, int16_t y, int16_t size);

        /**
         * Update private temperature values based on read sensor values.
         */
        void updateTemperatures();

        /**
         * Calculated the screen coordinates of a certain interpolated pixel. Top left pixel is given.
         */
        void interpolatedPixelIndexToCoordinates(uint16_t index, int16_t *xy);

        /**
         * Set automatic threshold true/false.
         */
        void setAuto(const bool flag);

        /**
         * Toggle automatic threshold.
         */
        void toggleAuto();

        /**
         * Toggle mode.
         */
        void toggleMode();


        // Array for AMG pixels
        float m_px[PXS];

        // Array for interpolated pixels
        float m_ipx[IPXS];

        // Temperatures
        int16_t m_gradientMin = DEFAULT_MIN_TEMPERATURE;
        int16_t m_gradientMax = DEFAULT_MAX_TEMPERATURE;
        int16_t m_gradientMinBackup{};
        int16_t m_gradientMaxBackup{};
        int16_t m_globalMin{};
        int16_t m_globalMax{};
        int16_t m_globalMinIndex{};
        int16_t m_globalMaxIndex{};
        int16_t m_cursorTemp{};

        // Variables for layout
        int16_t m_pixelSize{}; // Interpolated pixel width and height in screen pixels
        int16_t m_marginHorizontal{}; // Horizontal space between left/right edges and image box
        int16_t m_marginVertical{}; // Vertical space between top/bottom edges and image box
        int16_t m_x1{}; // X-coordinate for the left edge of content in left column
        int16_t m_x2{}; // X-coordinate for the left edge of content in right column
        int16_t m_fontHeight{}; // Height of the font used
        int16_t m_columnWidth{}; // Width of the left and right columns
        int16_t m_cursorCoord[2] = {};

        /**
         * Modes:
         *   0 = Pause
         *   1 = ON/OFF automatic gradient temperatures and ON/OFF draw cursor and highlights
         *   2 = Indrease/decrease gradient max temperature
         *   3 = Indrease/decrease gradient min temperature
         *   4 = Set min and max temperatures with cursor
         */
        uint8_t m_currentMode = 0;
        #define MODE_MAX 4

        // Flags
        bool m_flagAuto = false; // Update min/max gradient temperatures automatically
        bool m_flagCursor = true; // Draw cursor
        bool m_flagHighlights = true; // Draw global minimum and maximum highlights
        bool m_flagUpdate = true; // Update camera view, or pause
};
