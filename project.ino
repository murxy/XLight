#include "src/software.h"

murxy::Software software;

void setup() {
    software.initialize();
}

void loop() {
    software.loop();
}
