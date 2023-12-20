#include <mbed.h>
#include <feetbeat.h>

uint8_t systemSatus = 0;

int main() {	
	while(true) {
		switch(systemSatus) {
			case IDLE:
				handleIdleStage(systemSatus);
				break;
			case SAMPLING:
				handleSamplingStage(systemSatus);
				break;
			case CALCULATION:
				handleCalculationStage(systemSatus);
				break;
			case FINISHED:
				handleFinishedStage(systemSatus);
				break;
			default:
				handleIdleStage(systemSatus);
				break;
		}
		ThisThread::sleep_for(500ms);
	}
}