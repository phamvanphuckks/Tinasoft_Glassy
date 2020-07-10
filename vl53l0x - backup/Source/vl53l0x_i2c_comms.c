#include "vl53l0x_def.h"
#include "vl53l0x_i2c_platform.h"

//#define I2C_DEBUG

// done
int VL53L0X_i2c_init() {
	
	GPIO_configuration();
	i2c_configuration();
	
  return VL53L0X_ERROR_NONE;
}

// done
int VL53L0X_write_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count) {
													
	I2C_Write_MultiReg(deviceAddress, index, count, pdata);
  return VL53L0X_ERROR_NONE;
}

//done
int VL53L0X_read_multi(uint8_t deviceAddress, uint8_t index, uint8_t *pdata, uint32_t count) {
	I2C_Read_MultiReg(deviceAddress, index, count, pdata);
												 
  return VL53L0X_ERROR_NONE;
}
// done
int VL53L0X_write_byte(uint8_t deviceAddress, uint8_t index, uint8_t data) {
	
	I2C_Write_Reg(deviceAddress, index, data);
	return VL53L0X_ERROR_NONE;
}
// done
int VL53L0X_write_word(uint8_t deviceAddress, uint8_t index, uint16_t data) {

	I2C_Write_Reg16bit(deviceAddress, index, data);
	return VL53L0X_ERROR_NONE;
}
// done
int VL53L0X_write_dword(uint8_t deviceAddress, uint8_t index, uint32_t data) {
	
	I2C_Write_Reg32bit(deviceAddress, index, data);
	return VL53L0X_ERROR_NONE;
}

//done
int VL53L0X_read_byte(uint8_t deviceAddress, uint8_t index, uint8_t *data) {
	
	*data = I2C_Read_Reg(deviceAddress, index);
	return VL53L0X_ERROR_NONE;
}

//done
int VL53L0X_read_word(uint8_t deviceAddress, uint8_t index, uint16_t *data) {

	*data = I2C_Read_Reg16bit(deviceAddress, index);

	return VL53L0X_ERROR_NONE;
}
//done
int VL53L0X_read_dword(uint8_t deviceAddress, uint8_t index, uint32_t *data) {
	*data = I2C_Read_Reg32bit(deviceAddress, index);

	return VL53L0X_ERROR_NONE;
}
