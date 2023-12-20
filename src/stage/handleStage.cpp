#include <feetbeat.h>

void handleIdleStage(uint8_t &stage) {
    // TODO: use LCD
    // get height info
    printf("IDLE stage - press blue button to continue\n");
    while(!blueButtonPressed()) {}
    stage = SAMPLING;
}

void handleSamplingStage(uint8_t &stage) {
    // TODO: use LCD
    initGyro();
    printf("SAMPLING stage - press blue button to start\n");
    while (!blueButtonPressed()) {}
    startRecording();
    stage = CALCULATION;
}
void handleCalculationStage(uint8_t &stage) {
    // TODO: use LCD
    printf("CALCULATION stage\n");
    getTraveledDistance();
    getAverageSpeed();
    stage = FINISHED;
}

void handleFinishedStage(uint8_t &stage) {
    // TODO: use LCD
    printf("FINISHED stage - press blue button to restart\n");
    while(!blueButtonPressed()) {}
    stage = IDLE;
}

