#include <mbed.h>
#include <math.h>
#include "../src/drivers/LCD_DISCO_F429ZI.h"

#define SAMPLING_DURATION 20 // in second
#define INTERVAL 0.5                                                                            
#define REQUIRED_IDX (int)(SAMPLING_DURATION/INTERVAL)

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

/* LCD */
extern float data[REQUIRED_IDX];
extern char buf[60];
extern LCD_DISCO_F429ZI lcd;
extern uint8_t userHeightInCm;
extern bool userIsMale;
void initLcd();
void clearScreen();
uint8_t getUserHeightInCm();
bool getUserIsMale();
void userHeightSetupDisplay();
void userGenderSetupDisplay();
void samplingStageDisplay();
void calculationDisplay();
void finishedDisplay();
