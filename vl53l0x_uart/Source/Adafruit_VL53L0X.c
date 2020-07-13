#include "Adafruit_VL53L0X.h"
#include "delay.h"

#define VERSION_REQUIRED_MAJOR 1 			///< Required sensor major version
#define VERSION_REQUIRED_MINOR 0 			///< Required sensor minor version
#define VERSION_REQUIRED_BUILD 1 			///< Required sensor build

#define STR_HELPER(x) #x     				///< a string helper
#define STR(x) STR_HELPER(x) 				///< string helper wrapper

VL53L0X_Error Status =	VL53L0X_ERROR_NONE; 

VL53L0X_Dev_t MyDevice;
VL53L0X_Dev_t *pMyDevice = &MyDevice;
VL53L0X_DeviceInfo_t DeviceInfo;

// done
uint8_t begin(uint8_t i2c_addr, uint8_t debug) {
  uint32_t refSpadCount;
  uint8_t isApertureSpads;
  uint8_t VhvSettings;
  uint8_t PhaseCal;

  // Initialize Comms
  pMyDevice->I2cDevAddr 		  = VL53L0X_I2C_ADDR; // default
  pMyDevice->comms_type 			= 1;
  pMyDevice->comms_speed_khz 	= 100;


  VL53L0X_i2c_init();

  // unclear if this is even needed:
  if ((VL53L0X_IMPLEMENTATION_VER_MAJOR != VERSION_REQUIRED_MAJOR )||
      (VL53L0X_IMPLEMENTATION_VER_MINOR != VERSION_REQUIRED_MINOR )||
      (VL53L0X_IMPLEMENTATION_VER_SUB   != VERSION_REQUIRED_BUILD )) {
  
    Status = VL53L0X_ERROR_NOT_SUPPORTED;

    return false;
  }

	
  Status = VL53L0X_DataInit(&MyDevice); // Data initialization
	
//  if (!setAddress(i2c_addr)) {
//    return false;
//  }

  Status = VL53L0X_GetDeviceInfo(&MyDevice, &DeviceInfo);

  if (Status == VL53L0X_ERROR_NONE) {


    if ((DeviceInfo.ProductRevisionMajor != 1) ||
        (DeviceInfo.ProductRevisionMinor != 1)) {
      Status = VL53L0X_ERROR_NOT_SUPPORTED;
    }
  }

  if (Status == VL53L0X_ERROR_NONE) {
  

    Status = VL53L0X_StaticInit(pMyDevice); // Device Initialization
  }

  if (Status == VL53L0X_ERROR_NONE) {
   
    Status = VL53L0X_PerformRefSpadManagement(
        pMyDevice, &refSpadCount, &isApertureSpads); // Device Initialization

  }

  if (Status == VL53L0X_ERROR_NONE) {
   
    Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings,
                                           &PhaseCal); // Device Initialization
  }

  if (Status == VL53L0X_ERROR_NONE) {
    // no need to do this when we use VL53L0X_PerformSingleRangingMeasurement
 
    Status = VL53L0X_SetDeviceMode(
        pMyDevice,
        VL53L0X_DEVICEMODE_SINGLE_RANGING); // Setup in single ranging mode
  }

  // Enable/Disable Sigma and Signal check
  if (Status == VL53L0X_ERROR_NONE) {
    Status = VL53L0X_SetLimitCheckEnable(
        pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
  }

  if (Status == VL53L0X_ERROR_NONE) {
    Status = VL53L0X_SetLimitCheckEnable(
        pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);
  }

  if (Status == VL53L0X_ERROR_NONE) {
    Status = VL53L0X_SetLimitCheckEnable(
        pMyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1);
  }

  if (Status == VL53L0X_ERROR_NONE) {
    Status = VL53L0X_SetLimitCheckValue(
        pMyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD,
        (FixPoint1616_t)(1.5 * 0.023 * 65536));
  }

  if (Status == VL53L0X_ERROR_NONE) {
    return true;
  } else {
    return false;
  }
}

/**************************************************************************/
/*!
    @brief  Change the I2C address of the sensor
    @param  newAddr the new address to set the sensor to
    @returns True if address was set successfully, False otherwise
*/
/**************************************************************************/
//done
uint8_t setAddress(uint8_t newAddr) {
  newAddr &= 0x7F;

  Status = VL53L0X_SetDeviceAddress(pMyDevice, newAddr * 2); // 7->8 bit

  delay_ms(10);

  if (Status == VL53L0X_ERROR_NONE) {
    pMyDevice->I2cDevAddr = newAddr; // 7 bit addr
    return true;
  }
  return false;
}

/**************************************************************************/
/*!
    @brief  get a ranging measurement from the device
    @param  RangingMeasurementData the pointer to the struct the data will be
   stored in
    @param debug Optional debug flag. If true debug information will print via
   Serial.print during execution. Defaults to false.
    @returns True if address was set successfully, False otherwise
*/

/**************************************************************************/
VL53L0X_Error getSingleRangingMeasurement(
  VL53L0X_RangingMeasurementData_t *RangingMeasurementData, uint8_t debug) {
  VL53L0X_Error Status = VL53L0X_ERROR_NONE;
  FixPoint1616_t LimitCheckCurrent;

  /*
   *  Step  4 : Test ranging mode
   */

  if (Status == VL53L0X_ERROR_NONE) {
  
    Status = VL53L0X_PerformSingleRangingMeasurement(pMyDevice,
                                                     RangingMeasurementData);

    if (debug) {
      printRangeStatus(RangingMeasurementData);
    }

    if (debug) {
      VL53L0X_GetLimitCheckCurrent(pMyDevice,
                                   VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD,
                                   &LimitCheckCurrent);

//      Serial.print(F("RANGE IGNORE THRESHOLD: "));
//      Serial.println((float)LimitCheckCurrent / 65536.0);

//      Serial.print(F("Measured distance: "));
//      Serial.println(RangingMeasurementData->RangeMilliMeter);
    }
  }

  return Status;
}

/**************************************************************************/
/*!
    @brief  print a ranging measurement out via Serial.print in a human-readable
   format
    @param pRangingMeasurementData a pointer to the ranging measurement data
*/
/**************************************************************************/
void printRangeStatus(
    VL53L0X_RangingMeasurementData_t *pRangingMeasurementData) {
  char buf[VL53L0X_MAX_STRING_LENGTH];
  uint8_t RangeStatus;

  /*
   * New Range Status: data is valid when pRangingMeasurementData->RangeStatus =
   * 0
   */

  RangeStatus = pRangingMeasurementData->RangeStatus;

  VL53L0X_GetRangeStatusString(RangeStatus, buf);

//  Serial.print(F("Range Status: "));
//  Serial.print(RangeStatus);
//  Serial.print(F(" : "));
//  Serial.println(buf);
}
		
VL53L0X_Error	rangingTest(VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
              uint8_t debug) {
    return getSingleRangingMeasurement(pRangingMeasurementData, debug);
};
