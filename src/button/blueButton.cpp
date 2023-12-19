#include <feetbeat.h>

DigitalIn blueButton(PA_0);

bool blueButtonPressed() {
    if (blueButton.read() != 0) { return true;}
    return false;
}