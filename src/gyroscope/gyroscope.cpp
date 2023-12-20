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

#define GYRO_SCALE_FACTOR (17.5f * 0.017453292519943295769236907684886f / 1000.0f)

#define SAMPLING_DURATION 20
#define INTERVAL 0.5
#define REQUIRED_IDX (int)(SAMPLING_DURATION/INTERVAL)

uint8_t write_buf[32]; 
uint8_t read_buf[32];

EventFlags flags;

float x_samples[REQUIRED_IDX];
float y_samples[REQUIRED_IDX];
float z_samples[REQUIRED_IDX];

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

void readGyro() {
    int16_t raw_gx,raw_gy,raw_gz;
    //prepare the write buffer to trigger a sequential read
    write_buf[0]=OUT_X_L|0x80|0x40;

    for (int i = 0; i < REQUIRED_IDX; i++) {
        // each iteration takes about 0.5 to sample the angular velocities
        // start sequential sample reading
        spi.transfer(write_buf,7,read_buf,7,spi_cb,SPI_EVENT_COMPLETE );
        flags.wait_all(SPI_FLAG);
        //read_buf after transfer: garbage byte, gx_low,gx_high,gy_low,gy_high,gz_low,gz_high
        //Put the high and low bytes in the correct order lowB,HighB -> HighB,LowB
        raw_gx = (((uint16_t)read_buf[2]) <<8) | ((uint16_t)read_buf[1]);
        raw_gy = (((uint16_t)read_buf[4]) <<8) | ((uint16_t)read_buf[3]);
        raw_gz = (((uint16_t)read_buf[6]) <<8) | ((uint16_t)read_buf[5]);

        x_samples[i] = ((float)raw_gx)*GYRO_SCALE_FACTOR;
        y_samples[i] = ((float)raw_gy)*GYRO_SCALE_FACTOR;
        z_samples[i] = ((float)raw_gz)*GYRO_SCALE_FACTOR;

        printf("%d/%d recorded... keep running...\n", i+1, REQUIRED_IDX);
        ThisThread::sleep_for(500ms);
    }    
}

void calcualteDistance() {
    // TODO: implement this
    for (int i = 0; i < (int)REQUIRED_IDX; i++) {
        printf("%d: \tgx: %d \t gy: %d \t gz: %d\n", i, x_samples[i], y_samples[i], z_samples[i]);
    }
}


