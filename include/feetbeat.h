#include <mbed.h>
#include <math.h>

enum Stage {
    IDLE,
    SAMPLING,
    CALCULATION,
    FINISHED
};

/* button */
bool blueButtonPressed();

/* stage */
void handleIdleStage(uint8_t &stage);
void handleSamplingStage(uint8_t &stage);
void handleCalculationStage(uint8_t &stage);
void handleFinishedStage(uint8_t &stage);

/* gyroscope */
void initGyro();
void startRecording();
float getLegLengthInMeter();
float getTraveledDistance();
float getAverageSpeed();
float* getSpeedData();
