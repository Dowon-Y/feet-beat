#include <feetbeat.h>
#include "../drivers/LCD_DISCO_F429ZI.h"

LCD_DISCO_F429ZI lcd;

char buf[60];
uint8_t userHeightInCm;
bool userIsMale = true;

uint8_t getUserHeightInCm() { return userHeightInCm; }
bool getUserIsMale() { return userIsMale; }

void initLcd() {
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_GREEN);
    lcd.SetBackColor(LCD_COLOR_BLACK);

    const char* lines[] = {
        "  __           _     ",
        " / _| ___  ___| |_ ",
        "| |_ / _ \\/ _ \\ __|",
        "|  _|  __/  __/ |_ ",
        "|_|  \\___|\\___|\\__|",
        " _                _   ",
        "| |__   ___  __ _| |_ ",
        "| '_ \\ / _ \\/ _` | __|",
        "| |_) |  __/ (_| | |_ ",
        "|_.__/ \\___|\\__,_|\\__|",
        "  ",
        "  ",
        "  ",
        "  ",
        "  Press Blue Button",
        "     To Continue...",
        "  ",
        "written by Dowon Yang"
    };

    for (int i = 0; i < sizeof(lines) / sizeof(lines[0]); ++i) {
        lcd.DisplayStringAtLine(i, (uint8_t *)lines[i]);
    }
    while(!blueButtonPressed()) {}
}
                                         
void userHeightSetupDisplay() {
    bool needSetup = true;
    while (needSetup) {
        lcd.Clear(LCD_COLOR_BLACK);
        lcd.DisplayStringAtLine(3, (uint8_t *)"Hold the blue button");
        lcd.DisplayStringAtLine(4, (uint8_t *)"to set your height");
        ThisThread::sleep_for(1000ms);
        while(!blueButtonPressed()) {
            lcd.DisplayStringAtLine(8, (uint8_t *)"Hold to begin...");
        }
        userHeightInCm = 150;
        while(blueButtonPressed()) {
            userHeightInCm++;
            sprintf(buf, "- Your height: %d cm -", userHeightInCm);
            lcd.DisplayStringAtLine(8, (uint8_t *)buf);
            ThisThread::sleep_for(500ms);
        }
        ThisThread::sleep_for(1000ms);
        uint8_t timeLeft = 7;
        while(!blueButtonPressed() && timeLeft > 0){
            lcd.DisplayStringAtLine(12, (uint8_t *)"To reset, hold the");
            sprintf(buf, "button in %d seconds", timeLeft);
            lcd.DisplayStringAtLine(13, (uint8_t *)buf);
            ThisThread::sleep_for(1000ms);
            timeLeft--;
        }
        if (timeLeft == 0 ) { needSetup = false; }
        ThisThread::sleep_for(500ms);
    }
}

void userGenderSetupDisplay() {
    lcd.Clear(LCD_COLOR_BLACK);
    uint8_t timeLeft = 7;
    lcd.DisplayStringAtLine(3, (uint8_t *)"If you are a woman,");
    lcd.DisplayStringAtLine(4, (uint8_t *)"hold the blue button in ");
    lcd.DisplayStringAtLine(5, (uint8_t *)"(diff stat will be used)");
    while(!blueButtonPressed() && timeLeft > 0){
        sprintf(buf, "    %d seconds", timeLeft);
        lcd.DisplayStringAtLine(6, (uint8_t *)buf);
        ThisThread::sleep_for(1000ms);
        timeLeft--;
    }
    if (timeLeft != 0 ) { 
        userIsMale = false;
    }
    lcd.Clear(LCD_COLOR_BLACK);
    ThisThread::sleep_for(500ms);
}


void samplingStageDisplay() {
    if (userIsMale) {
        lcd.DisplayStringAtLine(1, (uint8_t *)"[Your Gender]: Male");
    } else {
        lcd.DisplayStringAtLine(1, (uint8_t *)"[Your Gender]: Female");
    }
    sprintf(buf, "[Your Height]: %d cm", userHeightInCm);
    lcd.DisplayStringAtLine(2, (uint8_t *)buf);
    
    lcd.DisplayStringAtLine(4, (uint8_t *)"Press blue button and");
    lcd.DisplayStringAtLine(5, (uint8_t *)"run!");
    while (!blueButtonPressed()) {}
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAtLine(1, (uint8_t *)"Recording speed...");
    lcd.DisplayStringAtLine(5, (uint8_t *)"   Keep Moving!");
    startRecording();
    ThisThread::sleep_for(1000ms);
}

void calculationDisplay() {
    float travelDist = getTraveledDistance();
    float avgSpeed = getAverageSpeed();
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAtLine(1, (uint8_t *)"[Result]");
    sprintf(buf, "Distance: %.2f m", travelDist);
    lcd.DisplayStringAtLine(2, (uint8_t *)buf);
    sprintf(buf, "Avg Speed: %.2f m/s", avgSpeed);
    lcd.DisplayStringAtLine(3, (uint8_t *)buf);

    lcd.DisplayStringAtLine(6, (uint8_t *)"Hold to see details");
    while (!blueButtonPressed()) {}
    lcd.Clear(LCD_COLOR_BLACK);
    for (int i = 0; i < REQUIRED_IDX; i++) {
        sprintf(buf, "[%d/%d]: %.2f m/s", i+1, REQUIRED_IDX, data[i]);
        lcd.DisplayStringAtLine((i+1) % 19, (uint8_t *)buf);
        if ((i + 1) % 19 == 0) { lcd.Clear(LCD_COLOR_BLACK); }
        ThisThread::sleep_for(500ms);
    }
}

void finishedDisplay() {
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAtLine(1, (uint8_t *)"Program terminated");
    lcd.DisplayStringAtLine(3, (uint8_t *)"Press button to ");
    lcd.DisplayStringAtLine(4, (uint8_t *)"restart...");
    while(!blueButtonPressed()) {}
}







