#include <feetbeat.h>

SPI spi(PF_9, PF_8, PF_7,PC_1,use_gpio_ssel); // mosi, miso, sclk, cs

//address of first register with gyro data
#define OUT_X_L 0x28

//register fields(bits): data_rate(2),Bandwidth(2),Power_down(1),Zen(1),Yen(1),Xen(1)
#define CTRL_REG1 0x20

//configuration: 200Hz ODR,50Hz cutoff, Power on, Z on, Y on, X on
#define CTRL_REG1_CONFIG 0b01'10'1'1'1'1

//register fields(bits): reserved(1), endian-ness(1),Full scale sel(2), reserved(1),self-test(2), SPI mode(1)
#define CTRL_REG4 0x23

//configuration: reserved,little endian,500 dps,reserved,disabled,4-wire mode
#define CTRL_REG4_CONFIG 0b0'0'01'0'00'0

#define SPI_FLAG 1

#define PI_OVER_180 0.017453292519943295769236907684886f
#define DPS_500_SENSITIVITY 17.5f
#define GYRO_SCALE_FACTOR PI_OVER_180 * DPS_500_SENSITIVITY / 1000.0f

// TODO: change this back to 20s
#define SAMPLING_DURATION 10 // for testing
// #define SAMPLING_DURATION 20 // in second
#define INTERVAL 0.5                                                                            
#define REQUIRED_IDX (int)(SAMPLING_DURATION/INTERVAL)

uint8_t write_buf[32]; 
uint8_t read_buf[32];

EventFlags flags;

float data[REQUIRED_IDX];

float traveledDistance = 0.0f;
float averageSpeed = 0.0f;

// move these to LCD screen control later
float userHeightInCm = 178.0f;
bool userIsMale = true;


// callback for spi.transfer()
void spi_cb(int event){
    flags.set(SPI_FLAG);
}

void initGyro() {
    spi.format(8, 3);
    spi.frequency(1'000'000);

    write_buf[0] = CTRL_REG1;
    write_buf[1] = CTRL_REG1_CONFIG;
    spi.transfer(write_buf, 2, read_buf, 2, spi_cb,SPI_EVENT_COMPLETE);
    flags.wait_all(SPI_FLAG);

    write_buf[0] = CTRL_REG4;
    write_buf[1] = CTRL_REG4_CONFIG;
    spi.transfer(write_buf, 2, read_buf, 2, spi_cb, SPI_EVENT_COMPLETE);
    flags.wait_all(SPI_FLAG); 
}

float getLinearVelocity() {
    int16_t raw_gx,raw_gy,raw_gz;
    float gx, gy, gz;

    //prepare the write buffer to trigger a sequential read
    write_buf[0]=OUT_X_L|0x80|0x40;
    
    // each iteration takes about 0.5 to sample the angular velocities
    // start sequential sample reading
    spi.transfer(write_buf,7,read_buf,7,spi_cb,SPI_EVENT_COMPLETE );
    flags.wait_all(SPI_FLAG);
    raw_gx = (((uint16_t)read_buf[2]) <<8) | ((uint16_t)read_buf[1]);
    raw_gy = (((uint16_t)read_buf[4]) <<8) | ((uint16_t)read_buf[3]);
    raw_gz = (((uint16_t)read_buf[6]) <<8) | ((uint16_t)read_buf[5]);

    gx = ((float)raw_gx)*GYRO_SCALE_FACTOR;
    gy = ((float)raw_gy)*GYRO_SCALE_FACTOR;
    gz = ((float)raw_gz)*GYRO_SCALE_FACTOR;

    // logic: we are going to put this device vertically in our pocket.
    // so the angular velocity should be vector addition of x and z axis of the device
    return sqrt(gx * gx + gz * gz);
}

void startRecording() {
    float spotSpeed = 0.0f;
    for (int i = 0; i < REQUIRED_IDX; i++) {
        ThisThread::sleep_for(500ms);
        spotSpeed = getLinearVelocity();
        data[i] = spotSpeed;
        printf("[%d/%d] spot speed %4.5fm/s recorded. keep moving...\n", i+1, REQUIRED_IDX, spotSpeed);
    }
}

float getTraveledDistance() {
    traveledDistance = 0.0f;
    float legLength = getLegLengthInMeter();
    for (int i = 0; i < REQUIRED_IDX; i++) {

        traveledDistance += data[i] * legLength * 0.5;
    }
    printf("Traveled distance: %4.5fm\n", traveledDistance);
    return traveledDistance;
}

float getAverageSpeed() {
    averageSpeed = traveledDistance / SAMPLING_DURATION;
    printf("Average speed: %4.5fm/s\n", averageSpeed);
    return averageSpeed;
}

float getLegLengthInMeter() {
    // average leg to body ratio is about 1.123 (male) 1.124 (female)
    // source: https://psycnet.apa.org/fulltext/2010-01934-004.html
    if (userIsMale) {
        return userHeightInCm * (1.123f / (1.123f + 1.0f)) / 100.0f;
    }
    return userHeightInCm * (1.124f / (1.124f + 1.0f)) / 100.0f;
}

float* getSpeedData() { return data; }