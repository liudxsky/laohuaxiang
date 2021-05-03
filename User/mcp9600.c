/*
    Seeed_MCP9600.cpp
    Driver for MCP9600

    Copyright (c) 2018 Seeed Technology Co., Ltd.
    Website    : www.seeed.cc
    Author     : downey
    Create Time: May 2018
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "mcp9600.h"
#include "./flash/deviceinfo.h"
#include "i2c_sw.h"
extern dev_info_t dev_info;


#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
uint32_t  I2CTimeout=0;
u8 _IIC_ADDR;
uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
	printf("I2c error%d",errorCode);
	return errorCode;
}
void I2C_PortInit()
{
	/*
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_I2C2);
GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_I2C2);
GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//Config the I2C
	//Set the clock speed of I2C. It has to be equal with the external device
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_ClockSpeed = 10000;
	//I2C mode
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C device adress
	I2C_InitStructure.I2C_OwnAddress1 = 0x33; 
	//I2C Acknowladge configuration
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//Enable the I2C with the prepared configuration
	I2C_Init(I2C2, &I2C_InitStructure);

	//And start the I2C 
	I2C_Cmd(I2C2, ENABLE);
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	*/
	SW_I2C_initial();
}
err_t Start_I2C_Transmitting(){

//For transmiting to the slave, start the data transmitting
I2C_GenerateSTART(I2C2, ENABLE);
 //Waiting for flag
	I2CTimeout=I2CT_FLAG_TIMEOUT;
 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
 {
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(-1);
  }  
 //Send address to slave
 I2C_Send7bitAddress(I2C2, _IIC_ADDR, I2C_Direction_Transmitter);
//And check the transmitting
	I2CTimeout=I2CT_FLAG_TIMEOUT;
while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(-2);
  }

}
err_t Start_I2C_Receiving(){

//For transmiting to the slave, start the data transmitting
I2C_GenerateSTART(I2C2, ENABLE);
 //Waiting for flag
	I2CTimeout=I2CT_FLAG_TIMEOUT;
 while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
 {
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(0);
  }  
 //Send address to slave
 I2C_Send7bitAddress(I2C2, _IIC_ADDR, I2C_Direction_Receiver);
//And check the transmitting
	I2CTimeout=I2CT_FLAG_TIMEOUT;
while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(1);
  }  

}


err_t IIC_write_byte(u8 reg, u8 byte) {

	Start_I2C_Transmitting();

	I2C_SendData(I2C2, byte);
	// wait for the data trasnmitted flag
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(2);
  }  
	I2C_GenerateSTOP(I2C2, ENABLE);
}

err_t IIC_write_16bit(u8 reg, u16 value) {
	u8 temp=value>>8;
	
		Start_I2C_Transmitting();
		I2C_SendData(I2C2,temp);
		I2C_SendData(I2C2,value&0xff);
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(3);
  }  
	I2C_GenerateSTOP(I2C2, ENABLE);
}
err_t IIC_read_byte(u8 reg, u8* byte) {
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	uint8_t temp;
	Start_I2C_Transmitting();
	I2C_SendData(I2C2, reg);
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(4);
  }  
	//I2C_GenerateSTOP(I2C2, ENABLE);

	Start_I2C_Receiving();
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
		{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(5);
  }  

	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
		{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(6);
  }  
	temp = I2C_ReceiveData(I2C2);
	*byte=temp;
	return 0;
}
err_t IIC_read_16bit(u8 start_reg, u16* value) {
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	uint8_t received_i2c_data[2];
	Start_I2C_Transmitting();
	I2C_SendData(I2C2, start_reg);
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(7);
  }  
	//I2C_GenerateSTOP(I2C2, ENABLE);
	
	Start_I2C_Receiving();
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(8);
  }  
	received_i2c_data[0] = I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
	I2CTimeout=I2CT_FLAG_TIMEOUT;
	while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
	{
    if((I2CTimeout--) == 0) 
			return I2C_TIMEOUT_UserCallback(9);
  }  
	received_i2c_data[1] = I2C_ReceiveData(I2C2);
	u16 temp=received_i2c_data[0];
	temp=temp<<8;
	temp+=received_i2c_data[1];
	
	*value=temp;
	return 0;
}

err_t IIC_read_bytes(u8 start_reg, u8* data, u32 data_len) {

}

err_t MCP9600init() {
	I2C_PortInit();
	_IIC_ADDR=MCP9600_IIC_ADDR<<1;
    err_t ret = NO_ERROR;
    u16 ver;
    ret = MCP9600read_version(&ver);
    if (!ret) {
       printf("MCP9600 ver:%d",ver);
    }
		/*
    if (!MCP9600set_therm_type(THER_TYPE_K)) {
			return -1;
		}
    
		if(!MCP9600set_filt_coefficients(FILT_MID))
		{
			return -2;
		}
		if(!MCP9600set_cold_junc_resolution(COLD_JUNC_RESOLUTION_0_25))
		{
			return -3;
		}
		if(!MCP9600set_ADC_meas_resolution(ADC_14BIT_RESOLUTION))
		{
			return -4;
		}
		
		if(!MCP9600set_burst_mode_samp(BURST_32_SAMPLE))
		{
			return -5;
		}
		if(!		MCP9600set_sensor_mode(NORMAL_OPERATION))
		{
			return -6;
		}
		*/
    return ret;
}

/** @brief read version.
    @param ver.
    @return 0 if successed.
 * */
err_t MCP9600read_version(u16* ver) {
	u16 data16=0;
    if (IIC_read_16bit(VERSION_ID_REG_ADDR, &data16)) {
        return ERROR_COMM;
    }
		*ver=data16;
    return NO_ERROR;
}

/** @brief read hot-junction,the temperature result.
    @param value: result.
    @return 0 if successed.
 * */
err_t MCP9600read_hot_junc(float* value) {
    *value = 0;
    u16 read_value = 0;
    if (IIC_read_16bit(HOT_JUNCTION_REG_ADDR, &read_value)) {
        return ERROR_COMM;
    }
    // Serial.print("read hot junc value=");
    // Serial.println(read_value,HEX);
    if (read_value & 0x8000) {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0 - 4096.0;
    } else {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0;
    }
    return NO_ERROR;
}


/** @brief read junction delta.
    @param value: result.
    @return 0 if successed.
 * */
err_t MCP9600read_junc_temp_delta(float* value) {
    *value = 0;
    u16 read_value = 0;
    if (IIC_read_16bit(JUNCTION_TEMP_DELTA_REG_ADDR, &read_value)) {
        return ERROR_COMM;
    }
    if (read_value & 0x8000) {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0 - 4096.0;
    } else {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0;
    }
    return NO_ERROR;
}

/** @brief read cold-junction.
    @param value: result.
    @return 0 if successed.
 * */
err_t MCP9600read_cold_junc(float* value) {
    *value = 0;
    u16 read_value = 0;
    if (IIC_read_16bit(COLD_JUNCTION_TEMP_REG_ADDR, &read_value)) {
        return ERROR_COMM;
    }


    if (read_value & 0xf000) {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0 - 4096;
    } else {
        *value = (read_value >> 8) * 16.0 + (read_value & 0x00ff) / 16.0;
    }
    return NO_ERROR;
}

/** @brief read raw ADC value.
    @param data
    @param data_len
    @return 0 if successed.
 * */
err_t MCP9600read_ADC_data(u8* data, u32 data_len) {
    if (IIC_read_bytes(RAW_ADC_DATA_REG_ADDR, data, data_len)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief read sensor status.1
    @param byte: result.
    @return 0 if successed.
 * */
err_t MCP9600read_status(u8* byte) {
    *byte = 0;
    if (IIC_read_byte(STAT_REG_ADDR, byte)) {
        return ERROR_COMM;
    }
	
    return NO_ERROR;
}

/** @brief set_therm_cfg
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_therm_cfg(u8 set_byte) {
    if (IIC_write_byte(THERM_SENS_CFG_REG_ADDR, set_byte)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}


/** @brief read thermocouple status.
    @param byte: result.
    @return 0 if successed.
 * */
err_t MCP9600read_therm_cfg(u8* byte) {
    if (IIC_read_byte(THERM_SENS_CFG_REG_ADDR, byte)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief set_therm_type
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_therm_type(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(THERM_SENS_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }

    byte_to_set = (therm_cfg_data & 0x8f) | set_byte;

    return IIC_write_byte(THERM_SENS_CFG_REG_ADDR, byte_to_set);
}

/** @brief set_filt_coefficients
    The  content of filt-coefficients register are:code-junction-resolution/ADC measurement resolution/burst mode temp samples/sensor mode
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_filt_coefficients(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(THERM_SENS_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }

    byte_to_set = (therm_cfg_data & 0xf8) | set_byte;

    return IIC_write_byte(THERM_SENS_CFG_REG_ADDR, byte_to_set);
}


/** @brief set_dev_cfg
    The content of device configuration register are:interrupt clear/monitor TH or TC/inerrupt pin rise or FALL/
                                                    Active-high or low/compare mode or int mode /alert enable or not
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_dev_cfg(u8 set_byte) {
    if (IIC_write_byte(DEVICE_CFG_REG_ADDR, set_byte)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief read_dev_cfg
    @param byte: the byte to be read in.
    @return 0 if successed.
 * */
err_t MCP9600read_dev_cfg(u8* byte) {
    if (IIC_read_byte(DEVICE_CFG_REG_ADDR, byte)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief set_sensor_mode
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_sensor_mode(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(DEVICE_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xfc) | set_byte;
    return IIC_write_byte(DEVICE_CFG_REG_ADDR, byte_to_set);
}

/** @brief set_burst_mode_samp
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_burst_mode_samp(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(DEVICE_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xe3) | set_byte;
    return IIC_write_byte(DEVICE_CFG_REG_ADDR, byte_to_set);
}


/** @brief set_ADC_meas_resolution
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_ADC_meas_resolution(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(DEVICE_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0x9f) | set_byte;
    return IIC_write_byte(DEVICE_CFG_REG_ADDR, byte_to_set);
}

/** @brief set_cold_junc_resolution
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_cold_junc_resolution(u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(DEVICE_CFG_REG_ADDR, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0x7f) | set_byte;
    return IIC_write_byte(DEVICE_CFG_REG_ADDR, byte_to_set);
}

/** @brief set_alert_limitation
    @param alert num the channel of alert
    @param value: the 16bit value to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_limit(u8 alert_num, u16 value) {
    if (IIC_write_16bit(TEMP_ALERT1_LIMIT_REG_ADDR + alert_num, value)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief set_alert_hysteresis
    @param alert num the channel of alert
    @param set_byte: the byte to be set..
    @return 0 if successed.
 * */
err_t MCP9600set_alert_hys(u8 alert_num, u16 value) {
    if (IIC_write_byte(ALERT1_HYS_REG_ADDR + alert_num, value)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief set_alert_cfg
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_cfg(u8 alert_num, u8 set_byte) {
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, set_byte);
}

/** @brief read_alert_cfg
    @param alert num the channel of alert
    @param byte: the byte to be read in.
    @return 0 if successed.
 * */
err_t MCP9600read_alert_cfg(u8 alert_num, u8* byte) {
    *byte = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, byte)) {
        return ERROR_COMM;
    }
    return NO_ERROR;
}

/** @brief clear_int_flag
    @param alert num the channel of alert
    @return 0 if successed.
 * */
err_t MCP9600clear_int_flag(u8 alert_num) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data | 0x80);
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}


/** @brief set_alert_for_TH_or_TC
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_for_TH_or_TC(u8 alert_num, u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xef) | set_byte;
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}

/** @brief set_alert_limit_direction
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_limit_direction(u8 alert_num, u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xf7) | set_byte;
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}

/** @brief set_alert_bit
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_bit(u8 alert_num, u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }

    byte_to_set = (therm_cfg_data & 0xfb) | set_byte;

    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}

/** @brief set_alert_mode_bit
    set alert mode:comparator mode or INT mode.
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_mode_bit(u8 alert_num, u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xfd) | set_byte;
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}

/** @brief set_alert_enable
    Eable alert pin or not.
    @param alert num the channel of alert
    @param set_byte: the byte to be set.
    @return 0 if successed.
 * */
err_t MCP9600set_alert_enable(u8 alert_num, u8 set_byte) {
    u8 therm_cfg_data = 0;
    u8 byte_to_set = 0;
    if (IIC_read_byte(ALERT1_CFG_REG_ADDR + alert_num, &therm_cfg_data)) {
        return ERROR_COMM;
    }
    byte_to_set = (therm_cfg_data & 0xfe) | set_byte;
    return IIC_write_byte(ALERT1_CFG_REG_ADDR + alert_num, byte_to_set);
}


/** @brief check_data_update
    check if data ready.
    @param stat :indicate if data ready
    @return 0 if successed.
 * */
err_t MCP9600check_data_update(char* stat) {
    *stat = 0;
    err_t ret = NO_ERROR;
    u8 byte = 0;
    CHECK_RESULT(ret, MCP9600read_status(&byte));
    if (byte & 0x40) {
        *stat = true;
    } else {
        *stat = false;
    }
    return NO_ERROR;
}

/** @brief read_INT_stat
    check if any interruption is generated.
    @param stat :indicate if any interruption is generated
    @return 0 if successed.
 * */
err_t MCP9600read_INT_stat(u8* stat) {
    *stat = 0;
    err_t ret = NO_ERROR;
    u8 byte = 0;
    CHECK_RESULT(ret, MCP9600read_status(&byte));
    for (int i = 0; i < 4; i++) {
        if (byte & 1 << i) {
		printf("channel:%d interrupt",i);
        }
    }
    *stat = byte;
    return NO_ERROR;
}




/******************************************************************************************************/
u16 MCP9600covert_temp_to_reg_form(float temp) {
    u8 negetive = 0;
    if (temp < 0) {
        negetive = 1;
    }
    temp = abs(temp);
    u16 dest_temp = 0;
    u8 temp_H = 0, temp_L = 0;
    u16 interger = (u16)temp;
    float decimal = temp - interger;
    temp_H = interger / 16;
    temp_L |= (interger % 16) << 4;
    temp_L |= (u8)(decimal / 0.25) << 2;
    if (negetive) {
        temp_H |= 0x80;
    }
    dest_temp = (u16)temp_H << 8 | temp_L;
    return dest_temp;
}


