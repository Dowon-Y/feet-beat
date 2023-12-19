#include <mbed.h>
#include <feetbeat.h>

uint8_t systemSatus = 0;

int main() {	
	while(true) {
		switch(systemSatus) {
			case IDLE:
				handleIdleStatus(systemSatus);
				break;
			case SAMPLING:
				handleSamplingStatus(systemSatus);
				break;
			case CALCULATION:
				handleCalculationStatus(systemSatus);
				break;
			case FINISHED:
				handleFinishedStatus(systemSatus);
				break;
			default:
				handleIdleStatus(systemSatus);
				break;
		}
		ThisThread::sleep_for(500ms);
	}
}