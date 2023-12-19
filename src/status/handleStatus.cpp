#include <feetbeat.h>

void handleIdleStatus(uint8_t &status) {
    while(!blueButtonPressed()) {
        printf("IDLE status\n");
    }
    status = SAMPLING;
}

void handleSamplingStatus(uint8_t &status) {
    while(!blueButtonPressed()) {
        printf("SAMPLING status\n");
    }   
    status = CALCULATION;
}
void handleCalculationStatus(uint8_t &status) {
    while(!blueButtonPressed()) {
        printf("CALCULATION status\n");
    }
    status = FINISHED;
}

void handleFinishedStatus(uint8_t &status) {
    while(!blueButtonPressed()) {
        printf("FINISHED status\n");
    }
    status = IDLE;
}

