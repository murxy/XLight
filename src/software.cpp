#include <Arduino.h>

#include "software.h"

namespace murxy {

const Software::Handler Software::mStateHandlers[] = {
    &Software::onLoading,
    &Software::onReady,
};

void Software::initialize() {
    Serial.begin(9600);
    mLightShow.initialize();
}

void Software::loop() {
    (this->*mStateHandlers[mCurrentState])();
    mLightShow.update();
}

void Software::onLoading() {
    constexpr uint8_t count = 5;
    static LightShow::Columns cur[count] = { LightShow::Columns::Left };
    static uint8_t pos[count] = { 1, 2, 3, 4, 6 };

    if (mTimer == 0) {
        mTimer = millis();
        mLightShow.clear();
    }

    uint8_t erasePos = 0;
    for (uint8_t i = 0; i < count; ++i) {
        uint8_t& position = pos[i];
        LightShow::Columns& column = cur[i];
        if (i == 0)
            mLightShow[column].set(position, false);

        if (column == LightShow::Columns::Left)
            ++position;
        else
            --position;

        if (position > 0 && position < XSAA1064::ledCount()) {
            mLightShow[column].set(position, true);
            continue;
        }

        if (column == LightShow::Columns::Left)
            column = LightShow::Columns::Right;
        else
            column = LightShow::Columns::Left;
    }

    delay(millis() / 100);
    if (millis() - mTimer >= 5 * 1000)
        setState(States::Ready);
}

void Software::onReady() {
    if (mTimer == 0) {
        for (auto &led : mLightShow)
            led.set(XSAA1064::LinkLed);
        mTimer = millis();
    }

    uint8_t byte = 0;
    if (Serial.readBytes(&byte, 1) > 0) {
        if (byte != 'B') return;
    }

    uint16_t command = 0;
    size_t size = Serial.readBytes(reinterpret_cast<uint8_t*>(&command) + 1, 1);
    size += Serial.readBytes(reinterpret_cast<uint8_t*>(&command) + 0, 1);
    if (size == sizeof(command)) {
        mLightShow.execute(command);
    }
}

void Software::setState(States newState) {
    mCurrentState = newState;
    mLightShow.clear();
    mTimer = 0;
}

}  // namespace murxy
