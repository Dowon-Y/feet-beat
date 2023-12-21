#include <feetbeat.h>

void handleIdleStage(uint8_t &stage) {
    initLcd();	
    userHeightSetupDisplay();
    userGenderSetupDisplay();
    stage = SAMPLING;
}

void handleSamplingStage(uint8_t &stage) {
    initGyro();
    samplingStageDisplay();
    stage = CALCULATION;
}

void handleCalculationStage(uint8_t &stage) {
    calculationDisplay();
    stage = FINISHED;
}

void handleFinishedStage(uint8_t &stage) {
    finishedDisplay();
    stage = IDLE;
}

