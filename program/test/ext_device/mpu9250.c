#include "stm32f4xx_conf.h"
#include "mpu9250.h"
#include "spi.h"
#include "imu.h"



#define MPU9250_SPI SPI4
#define MPU9250_SELECT() 	GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define MPU9250_DESELECT() 	GPIO_SetBits(GPIOE,GPIO_Pin_4)

void mpu9250_delay(uint32_t count){

	while(count--){

	}
}

uint8_t mpu9250_read_byte(uint8_t addr)
{
	uint8_t rxData;

	MPU9250_SELECT();

					 SPI_xfer(MPU9250_SPI,addr | 0x80);
			rxData = SPI_xfer(MPU9250_SPI,0xff);
	MPU9250_DESELECT();

return rxData;
}
uint8_t mpu9250_read_who_am_i()
{
	uint8_t rcv_id;
	rcv_id = mpu9250_read_byte(MPU9250_WHO_AM_I);

return rcv_id;
}

void mpu9250_write_byte(uint8_t addr,uint8_t data){

	MPU9250_SELECT();

					 SPI_xfer(MPU9250_SPI,addr);
					 SPI_xfer(MPU9250_SPI,data);
	MPU9250_DESELECT();

}

void mpu9250_reset(){

	mpu9250_write_byte(MPU9250_PWR_MGMT_1,0x80); //Reset command = 0x80

	mpu9250_delay(1000000);
}

void mpu9250_initialize_config(){
	mpu9250_reset(); // reset chip and wait
	mpu9250_write_byte(MPU9250_GYRO_CONFIG,0x10); // 0x10 => Full scale 1000Hz
	mpu9250_write_byte(MPU9250_ACCEL_CONFIG,0x10); // 0x10 => Full scale 8g


}

void mpu9250_read_accel_temp_gyro(imu_unscaled_data_t* imu_unscaledData){

	uint8_t buffer[14];

	MPU9250_SELECT();

					 SPI_xfer(MPU9250_SPI,MPU9250_ACCEL_XOUT_H|0x80);
		buffer[0] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[1] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[2] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[3] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[4] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[5] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[6] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[7] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[8] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[9] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[10] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[11] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[12] =  SPI_xfer(MPU9250_SPI,0xff);
		buffer[13] =  SPI_xfer(MPU9250_SPI,0xff);

		imu_unscaledData->acc[1] = ((uint16_t)buffer[0]<<8)+(uint16_t)buffer[1];
		imu_unscaledData->acc[2] = ((uint16_t)buffer[2]<<8)+(uint16_t)buffer[3];
		imu_unscaledData->acc[3] = ((uint16_t)buffer[4]<<8)+(uint16_t)buffer[5];

		imu_unscaledData->temp = ((uint16_t)buffer[6]<<8)+(uint16_t)buffer[7];

		imu_unscaledData->gyro[1] = ((uint16_t)buffer[8]<<8)+(uint16_t)buffer[9];
		imu_unscaledData->gyro[2] = ((uint16_t)buffer[10]<<8)+(uint16_t)buffer[11];
		imu_unscaledData->gyro[3] = ((uint16_t)buffer[12]<<8)+(uint16_t)buffer[13];


	MPU9250_DESELECT();

	
}

void mpu9250_convert_to_scale(imu_unscaled_data_t* imu_unscaledData, imu_raw_data_t* imu_scaledData){

	imu_scaledData->acc[1]	= (float)(imu_unscaledData->acc[1])*MPU9250A_8g;
	imu_scaledData->acc[2]	= (float)(imu_unscaledData->acc[2])*MPU9250A_8g;
	imu_scaledData->acc[3]	= (float)(imu_unscaledData->acc[3])*MPU9250A_8g;

	imu_scaledData->gyro[1]	= (float)(imu_unscaledData->gyro[1])*MPU9250G_1000dps;
	imu_scaledData->gyro[2]	= (float)(imu_unscaledData->gyro[2])*MPU9250G_1000dps;
	imu_scaledData->gyro[3]	= (float)(imu_unscaledData->gyro[3])*MPU9250G_1000dps;




}