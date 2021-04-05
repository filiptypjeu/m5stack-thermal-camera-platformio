#include "ThermalHelper.h"
#include "interpolation.h"

#include <M5Core2.h>
#include <Adafruit_AMG88XX.h>

Adafruit_AMG88xx amg;

ThermalHelper::ThermalHelper() {}

void ThermalHelper::begin() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(2);

    if (!amg.begin()) {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.print("Error on AMG88xx setup");
        while (1) {
            delay(1);
        }
    }

    M5.Lcd.setTextColor(TFT_WHITE);

    // Width of the left and right columns is basically set to the width of 3 letters
    this->m_columnWidth = M5.Lcd.textWidth("32C");
    this->m_fontHeight = M5.Lcd.fontHeight(); 

    auto w = M5.Lcd.width();
    auto h = M5.Lcd.height();

    // Every interpolated pixel will be drawn as a sqaure.
    // Need to calculate the the size of an interpolated pixel in both directions, while also leaving room for the left and right columns.
    auto px_w = (w - 2*this->m_columnWidth - 2*SPACE_BETWEEN_COLUMNS)/INTERPOLATED_COLS;
    auto px_h = h/INTERPOLATED_ROWS;
    this->m_pixelSize = min(px_w, px_h);

    this->m_marginHorizontal = (w - this->m_pixelSize*INTERPOLATED_COLS) / 2;
    this->m_marginVertical = (h - this->m_pixelSize*INTERPOLATED_ROWS) / 2;

    this->m_x1 = this->m_marginHorizontal - SPACE_BETWEEN_COLUMNS - this->m_columnWidth;
    this->m_x2 = w - this->m_marginHorizontal + SPACE_BETWEEN_COLUMNS;

    this->drawGradient();
    this->drawGradientTemperatures();
    this->drawStaticInfo();
    this->setAuto(false);
}

void ThermalHelper::update() {
    this->handleButtons();

    // Read pixel values
    amg.readPixels(this->m_px);

    // Update min and max temperatures
    this->updateTemperatures();

    // Interpolate pixel values
    Interpolation::interpolate_image(this->m_px, AMG_ROWS, AMG_COLS, this->m_ipx, INTERPOLATED_ROWS, INTERPOLATED_COLS);

    // Draw all pixels
    this->drawPixels();

    // Draw cursor in the middle
    if (this->m_flagCursor) {
        drawCursor(this->m_marginHorizontal + this->m_pixelSize*INTERPOLATED_COLS/2 + this->m_pixelSize/2, this->m_marginVertical + this->m_pixelSize*INTERPOLATED_ROWS/2 + this->m_pixelSize/2, this->m_fontHeight);
    }
}

// PRIVATE

void ThermalHelper::handleButtons() {
    if (M5.BtnA.wasPressed()) {
        this->m_gradientMin = this->m_cursorTemp;
        this->drawGradientTemperatures();
    }

    if (M5.BtnB.wasPressed()) {
        this->toggleAuto();
        this->drawGradientTemperatures();
    }

    if (M5.BtnC.wasPressed()) {
        this->m_gradientMax = this->m_cursorTemp;
        this->drawGradientTemperatures();
    }
}

void ThermalHelper::drawPixels() {
    uint16_t r = INTERPOLATED_ROWS;
    uint16_t c = INTERPOLATED_COLS;
    int16_t size = this->m_pixelSize;
    int16_t t{};
    float *p = this->m_ipx;

    for (auto y = 0; y < r; y++) {
        for (auto x = 0; x < c; x++) {
            // Get the pixel temperature
            t = static_cast<int16_t>(p[y * c + x]);
            // Constrain the temperature to the current min-max-interval
            t = constrain(t, this->m_gradientMin, this->m_gradientMax);

            // Map the temperature to a color index
            uint8_t i = map(t, this->m_gradientMin, this->m_gradientMax, 0, THERMAL_GRADIENT_N - 1);

            // Draw the pixel
            M5.Lcd.fillRect(this->m_marginHorizontal + size * x, this->m_marginVertical + size * y, size, size, colorGradient[i]);
        }
    }
}

void ThermalHelper::drawStaticInfo() {
    M5.Lcd.setCursor(this->m_x2, 0);
    M5.Lcd.print("Max");

    M5.Lcd.setCursor(this->m_x2, 3*this->m_fontHeight);
    M5.Lcd.print("Min");

    // Draw cursor
    drawCursor(this->m_x2 + this->m_columnWidth/2, 6.5*this->m_fontHeight - SPACE_BETWEEN_COLUMNS, this->m_fontHeight);
}

void ThermalHelper::drawGradient() {
    // Top y-position of gradient
    auto a = this->m_fontHeight;
    // Bottom y-position of gradient
    auto b = M5.Lcd.height() - this->m_fontHeight;
    // Left x-position of gradient
    auto x = this->m_marginHorizontal - SPACE_BETWEEN_COLUMNS - this->m_columnWidth;

    // The gradient is drawn as individual lines with differnet colors
    for (auto y = a; y < b; y++) {
        // Map the current y-position to a color index, inverted
        uint8_t i = THERMAL_GRADIENT_N - map(y, a, b - 1, 1, THERMAL_GRADIENT_N);
        M5.Lcd.drawLine(x, y, x + this->m_columnWidth, y, colorGradient[i]);
        Serial.printf("Y = %d, i = %d\n", y, i);
    }
}

void ThermalHelper::drawGradientTemperatures() {
    auto h = M5.Lcd.height();

    M5.Lcd.fillRect(this->m_x1, 0, this->m_columnWidth, this->m_fontHeight, TFT_BLACK);
    M5.Lcd.setCursor(this->m_x1, 0);
    M5.Lcd.print(this->m_gradientMax);
    M5.Lcd.print("C");

    M5.Lcd.fillRect(this->m_x1, h - this->m_fontHeight, this->m_columnWidth, this->m_fontHeight, TFT_BLACK);
    M5.Lcd.setCursor(this->m_x1, h - this->m_fontHeight);
    M5.Lcd.print(this->m_gradientMin);
    M5.Lcd.print("C");
}

void ThermalHelper::drawCursor(int16_t x, int16_t y, int16_t size) {
    M5.Lcd.drawCircle(x, y, size*0.3, TFT_WHITE);
    M5.Lcd.drawLine(x, y - size/2, x, y + size/2, TFT_WHITE);
    M5.Lcd.drawLine(x - size/2, y, x + size/2, y, TFT_WHITE);
}

void ThermalHelper::updateTemperatures() {
    float MIN = INT_MAX;
    float MAX = INT_MIN;
    float t{};
    uint16_t r = AMG_ROWS;
    uint16_t c = AMG_COLS;
    float *p = this->m_px;

    for (auto y = 0; y < r; y++) {
        for (auto x = 0; x < c; x++) {
            // Get the pixel temperature
            t = p[y * c + x];

            if (t > MAX) {
                MAX = t;
            } else if (t < MIN) {
                MIN = t;
            }
        }
    }

    this->m_globalMax = static_cast<int16_t>(MAX);
    this->m_globalMin = static_cast<int16_t>(MIN);
    this->m_cursorTemp = static_cast<int16_t>(this->m_ipx[INTERPOLATED_ROWS*INTERPOLATED_COLS/2 + INTERPOLATED_COLS/2]);

    auto w = M5.Lcd.width();

    M5.Lcd.fillRect(this->m_x2, this->m_fontHeight, w, this->m_fontHeight, TFT_BLACK);
    M5.Lcd.setCursor(this->m_x2, this->m_fontHeight);
    M5.Lcd.print(this->m_globalMax);
    M5.Lcd.print("C");

    M5.Lcd.fillRect(this->m_x2, 4*this->m_fontHeight, w, this->m_fontHeight, TFT_BLACK);
    M5.Lcd.setCursor(this->m_x2, 4*this->m_fontHeight);
    M5.Lcd.print(this->m_globalMin);
    M5.Lcd.print("C");

    M5.Lcd.fillRect(this->m_x2, 7*this->m_fontHeight, w, this->m_fontHeight, TFT_BLACK);
    if (this->m_flagCursor) {
        M5.Lcd.setCursor(this->m_x2, 7*this->m_fontHeight);
        M5.Lcd.print(this->m_cursorTemp);
        M5.Lcd.print("C");
    }

    // If gradient temperatures are static
    if (!this->m_flagAuto) {
        return;
    }

    this->m_gradientMax = max(this->m_globalMax, static_cast<int16_t>(this->m_globalMin + SHORTEST_AUTO_TEMPERATURE_INTERVAL));
    this->m_gradientMin = this->m_globalMin;
    this->drawGradientTemperatures();
}

void ThermalHelper::setAuto(const bool flag) {
    this->m_flagAuto = flag;

    M5.Lcd.setCursor(M5.Lcd.width() - this->m_marginHorizontal + SPACE_BETWEEN_COLUMNS + 0.4*this->m_columnWidth, M5.Lcd.height() - this->m_fontHeight);

    if (!flag) {
        this->m_gradientMin = 20;
        this->m_gradientMax = 32;
        M5.Lcd.setTextColor(TFT_DARKGREY);
    }

    M5.Lcd.print("A");
    M5.Lcd.setTextColor(TFT_WHITE);
    if (flag == this->m_flagAuto) {
        return;
    }
}

void ThermalHelper::toggleAuto() {
    this->setAuto(!this->m_flagAuto);
}
