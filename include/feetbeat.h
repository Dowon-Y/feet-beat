#include <mbed.h>

enum Status {
    IDLE,
    SAMPLING,
    CALCULATION,
    FINISHED
};

bool blueButtonPressed();
void handleIdleStatus(uint8_t &status);
void handleSamplingStatus(uint8_t &status);
void handleCalculationStatus(uint8_t &status);
void handleFinishedStatus(uint8_t &status);