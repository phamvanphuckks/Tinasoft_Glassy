#ifndef ADAFRUIT_VL53L0X_H
#define ADAFRUIT_VL53L0X_H

#include "vl53l0x_api.h"
#include <stdbool.h>

#define VL53L0X_I2C_ADDR 0x52 ///< Default sensor I2C address


// done
uint8_t begin(uint8_t i2c_addr , uint8_t debug);

// done
uint8_t setAddress(uint8_t newAddr);

VL53L0X_Error	rangingTest(VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
              uint8_t debug);
VL53L0X_Error getSingleRangingMeasurement( VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
							uint8_t debug);
void	printRangeStatus(VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);


#endif
