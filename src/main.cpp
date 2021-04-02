#include "ThermalHelper.h"

#include <M5Core2.h>

ThermalHelper TH = ThermalHelper();

void setup() {
    M5.begin(true, false, true, true);
    M5.Lcd.begin();
    TH.begin();
}

void loop() {
    M5.update();
    TH.update();
}
