#include "i2c.h"

GPIO_InitTypeDef GPIO_InitStructure;
I2C_InitTypeDef  I2C_InitStructure;

// define led
void GPIO_configuration(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = 	GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin 	= 	GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_PuPd  = 	GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = 	GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



void i2c_configuration(void)
{
	
	/* Peripheral Clock Enable -------------------------------------------------*/

	RCC_APB1PeriphClockCmd(I2Cx_CLK, ENABLE);
	RCC_AHBPeriphClockCmd(I2Cx_SDA_GPIO_CLK|I2Cx_SCL_GPIO_CLK, ENABLE);
	
	/* I2C GPIO configuration -----------------------------------------------*/ 

	// SDA pin
  GPIO_InitStructure.GPIO_Pin 		  = I2Cx_SDA_PIN ;
  GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType     = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed		  = GPIO_Speed_50MHz;
  GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
	// SCL pin
	GPIO_InitStructure.GPIO_Pin 		  =  I2Cx_SCL_PIN;
	GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
	// AF
	GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);
	GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);	
  
 /* I2Cx configuration ----------------------------------------------------*/
	
	I2C_InitStructure.I2C_Mode 					   		= I2C_Mode_I2C;
	I2C_InitStructure.I2C_AnalogFilter			  = I2C_AnalogFilter_Disable;
	I2C_InitStructure.I2C_DigitalFilter 			= 0x00;
	I2C_InitStructure.I2C_Timing							= 0x20000A0D;
	I2C_InitStructure.I2C_OwnAddress1 		 		= 0x00;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_Ack 						 		= I2C_Ack_Disable;

	I2C_Init(I2Cx, &I2C_InitStructure);
	I2C_Cmd(I2Cx, ENABLE);	
}


void I2C_Write_Reg(uint8_t Addr, uint8_t Reg, uint8_t Data)
{
	//Wait until I2C isn't busy
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);	
	
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	//Ensure the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	
	
	//Send the address of the register we wish to write to
	I2C_SendData(I2Cx, Reg);
	
	//Ensure that the transfer complete reload flag is
	//set, essentially a standard TC flag
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
	
	//and ensure the I2C peripheral doesn't add
	//any start or stop conditions
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	//Send the value you wish you write to the register
	I2C_SendData(I2Cx, Data);

	//Wait for the stop flag to be set indicating
	//a stop condition has been sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);
}

void I2C_Write_MultiReg(uint8_t Addr,uint8_t Reg, uint8_t NumOfRegister, uint8_t  *Data)
{
	int8_t Cnt = 0;
	
	//Wait until I2C isn't busy
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);	
	
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	//Ensure the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	
	
	//Send the address of the register we wish to write to
	I2C_SendData(I2Cx, Reg);
	
	//Ensure that the transfer complete reload flag is
	//set, essentially a standard TC flag
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
	
	
	//and ensure the I2C peripheral doesn't add
	//any start or stop conditions
	I2C_TransferHandling(I2Cx, Addr, NumOfRegister+1, I2C_Reload_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	for(Cnt = 0 ; Cnt < NumOfRegister; Cnt++)
	{
		//Send the value you wish you write to the register
		I2C_SendData(I2Cx, Data[Cnt]);
		
		// ensure one of the bytes transmitted
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	

	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	//Wait for the stop flag to be set indicating
	//a stop condition has been sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);	
}

void I2C_Write_Reg16bit(uint8_t Addr, uint8_t Reg, uint16_t Data)
{
	uint8_t i;
	//Wait until I2C isn't busy
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);	
	
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	//Ensure the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	
	
	//Send the address of the register we wish to write to
	I2C_SendData(I2Cx, Reg);
	
	//Ensure that the transfer complete reload flag is
	//set, essentially a standard TC flag
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
	
	//and ensure the I2C peripheral doesn't add
	//any start or stop conditions
	I2C_TransferHandling(I2Cx, Addr, 3, I2C_Reload_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
	
	uint8_t buff[2];
  buff[1] = Data & 0xFF;
  buff[0] = Data >> 8;
	for ( i = 0; i < 2; i++) {
		
		//Send the value you wish you write to the register
		I2C_SendData(I2Cx, buff[i]);
		
		// ensure one of the bytes transmitted
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	

	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	

	//Wait for the stop flag to be set indicating
	//a stop condition has been sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);	
}

void I2C_Write_Reg32bit(uint8_t Addr, uint8_t Reg, uint32_t Data)
{

	uint8_t i;
	//Wait until I2C isn't busy
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);	
	
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);

	//Ensure the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	
	
	//Send the address of the register we wish to write to
	I2C_SendData(I2Cx, Reg);
	
	//Ensure that the transfer complete reload flag is
	//set, essentially a standard TC flag
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET);
	
	//and ensure the I2C peripheral doesn't add
	//any start or stop conditions
	I2C_TransferHandling(I2Cx, Addr, 5, I2C_Reload_Mode, I2C_No_StartStop);

	//Again, wait until the transmit interrupted flag is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);
	
  uint8_t buff[4];

  buff[3] = Data & 0xFF;
  buff[2] = Data >> 8;
  buff[1] = Data >> 16;
  buff[0] = Data >> 24;
	for ( i = 0; i < 4; i++) {
		
		//Send the value you wish you write to the register
		I2C_SendData(I2Cx, buff[i]);
		
		// ensure one of the bytes transmitted
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);	

	}
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	//Wait for the stop flag to be set indicating
	//a stop condition has been sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for the next potential transfer
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);	
	
}



uint8_t I2C_Read_Reg(uint8_t Addr, uint8_t Reg)
{
	int8_t SingleData = 0;

	//ensure the I2C peripheral isn't busy!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

	// Send S + Addr[7:1] + [0] = Write
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	//Wait until the transmit interrupt status is set

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	//Send the address of the register you wish to read
	I2C_SendData(I2Cx, (uint8_t)Reg);

	//Wait until transfer is complete!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);

	I2C_TransferHandling(I2Cx, Addr, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	//Wait until the RX register is full of luscious data!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);

	SingleData = I2C_ReceiveData(I2Cx);

	//Wait for the stop condition to be sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for next transfers
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

	return SingleData;	
}


uint16_t I2C_Read_Reg16bit(uint8_t Addr, uint8_t Reg)
{
	//ensure the I2C peripheral isn't busy!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

	// Send S + Addr[7:1] + [0] = Write
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	//Wait until the transmit interrupt status is set

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	//Send the address of the register you wish to read
	I2C_SendData(I2Cx, (uint8_t)Reg);

	//Wait until transfer is complete!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);


	I2C_TransferHandling(I2Cx, Addr, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);


	//Wait until the RX register is full of luscious data!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);


	uint16_t SingleData = 0;
	uint8_t i, buff[2];

	for(i = 0; i < 2; i++){
		
		/* Wait until all data are received */
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
		buff[i] = I2C_ReceiveData(I2Cx);
	}	
	

  SingleData = buff[0];
  SingleData <<= 8;
  SingleData |= buff[1];
	

	//Wait for the stop condition to be sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for next transfers
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

	return SingleData;	
}


uint32_t I2C_Read_Reg32bit(uint8_t Addr, uint8_t Reg)
{
	//ensure the I2C peripheral isn't busy!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

	// Send S + Addr[7:1] + [0] = Write
	I2C_TransferHandling(I2Cx, Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	//Wait until the transmit interrupt status is set

	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	//Send the address of the register you wish to read
	I2C_SendData(I2Cx, (uint8_t)Reg);

	//Wait until transfer is complete!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);


	I2C_TransferHandling(I2Cx, Addr, 4, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);


	//Wait until the RX register is full of luscious data!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);

	uint32_t SingleData = 0;
	uint8_t i, buff[4];

	for(i = 0; i < 4; i++){
		
		/* Wait until all data are received */
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
		buff[i] = I2C_ReceiveData(I2Cx);
	}	
	
  SingleData = buff[0];
  SingleData <<= 8;
  SingleData |= buff[1];
  SingleData <<= 8;
  SingleData |= buff[2];
  SingleData <<= 8;
  SingleData |= buff[3];

	//Wait for the stop condition to be sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for next transfers
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

	return SingleData;		
}

void I2C_Read_MultiReg(uint8_t Addr,uint8_t Reg, uint8_t NumOfRegister, uint8_t *Data)
{
	int8_t Cnt;

	// ensure the I2C peripheral isn't busy!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) == SET);

	I2C_TransferHandling(I2Cx, Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

	//Wait until the transmit interrupt status is set
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET);

	//Send the address of the register you wish to read
	I2C_SendData(I2Cx, (uint8_t)Reg);

	//Wait until transfer is complete!
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET);

	I2C_TransferHandling(I2Cx, Addr, NumOfRegister, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);


	for(Cnt = 0; Cnt < NumOfRegister; Cnt++){
		
		/* Wait until all data are received */
		while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
		Data[Cnt] = I2C_ReceiveData(I2Cx);
	}
	
	//Wait for the stop condition to be sent
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET);

	//Clear the stop flag for next transfers
	I2C_ClearFlag(I2Cx, I2C_FLAG_STOPF);

}

