#include <feetbeat.h>

void handleIdleStage(uint8_t &stage) {
    // TODO: use LCD
    printf("IDLE stage - press button to continue\n");
    while(!blueButtonPressed()) {}
    stage = SAMPLING;
}

void handleSamplingStage(uint8_t &stage) {
    // TODO: use LCD
    initGyro();
    printf("SAMPLING stage - press button to start\n");
    while (!blueButtonPressed()) {}
    readGyro();
    stage = CALCULATION;
}
void handleCalculationStage(uint8_t &stage) {
    // TODO: use LCD
    printf("CALCULATION stage\n");
    calcualteDistance();
    stage = FINISHED;
}

void handleFinishedStage(uint8_t &stage) {
    // TODO: use LCD
    printf("FINISHED stage - press button to restart\n");
    while(!blueButtonPressed()) {}
    stage = IDLE;
}

