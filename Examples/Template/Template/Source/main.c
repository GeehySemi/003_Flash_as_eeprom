/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.1
 *
 * @date        2022-04-11
 *
 * @attention
 *
 *  Copyright (C) 2018-2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be usefull and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

#include "main.h"
#include "stdio.h"


#define BUF_SIZE               10
uint16_t buf_write[BUF_SIZE] = {0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009};
uint16_t buf_read[BUF_SIZE];
uint16_t i, address;

/* USART Init */
void USARTInit(void);

int fputc(int ch, FILE *f)
{
   while (USART_ReadStatusFlag(USART1, USART_FLAG_TXBE) == RESET);
   USART_TxData8(USART1, (uint8_t) ch);    
   return ch;
}

/**
  * @brief  compare whether the valus of buffer 1 and buffer 2 are equal.
  * @param  buffer1: buffer 1 address.
            buffer2: buffer 2 address.
  * @retval 0: equal.
  *         1: unequal.
  */
uint32_t buffer_compare(uint16_t* buffer1, uint16_t* buffer2, uint32_t len)
{
  uint32_t i;
  
  for(i = 0; i < len; i++)
  {
    if(buffer1[i] != buffer2[i])
    {
      return 1;
    }
  }

  return 0;
}
/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
int main(void)
{
	
    GPIO_Config_T gpioConfig;

    /** LED2 GPIO configuration */
    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.pin = BOARD_LED2_GPIO_PIN;
    gpioConfig.speed = GPIO_SPEED_10MHz;
    GPIO_Config(BOARD_LED2_GPIO_PORT, &gpioConfig);
	
    /* TX-->PD5  RX-->PD6 */	
		USARTInit();  
	
    /* flash eeprom init */
    flash_ee_init();

	    /* write data to eeprom */  
    for(i = 0; i < BUF_SIZE; i++)
    {
        address = i;
			
        flash_ee_data_write(address, buf_write[i]);
				printf("buf_write[%d] == %x \r\n",i,buf_write[i]);
    }
  
    /* read data from eeprom */  
    for(i = 0; i < BUF_SIZE; i++)
    {
       address = i;
    
       flash_ee_data_read(address, &buf_read[i]);
			 printf("buf_read[%d] == %x \r\n",i,buf_read[i]);
    }
  
    /* compare data */
    if(buffer_compare(buf_write, buf_read, BUF_SIZE) == 0) 
    {
				GPIO_SetBit(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
				printf("buf_write == buf_read \r\n");
    }
    else
    { 
				GPIO_ClearBit(BOARD_LED2_GPIO_PORT, BOARD_LED2_GPIO_PIN);
				printf("Error \r\n");
    }  
		
    while (1)
    {

    }
}

/*!
 * @brief       USART Init   
 *
 * @param       None
 *
 * @retval      None
 *
 * @note
 */
void USARTInit(void)
{
    USART_Config_T usartConfig;
    USART_SyncClockConfig_T usartSyncClockConfig;
    
    /**  Enable USART Clock  */
    usartSyncClockConfig.enable = USART_CLKEN_ENABLE;
    /**  Enable last bit clock  */
    usartSyncClockConfig.lastBitClock = USART_LBCP_ENABLE;
    /**  Clock phase is 2 edge */
    usartSyncClockConfig.phase = USART_CLKPHA_2EDGE;
    /**  Clock polarity is high */
    usartSyncClockConfig.polarity = USART_CLKPOL_HIGH;
    /**  Synchronous communication clock configuration */
    USART_ConfigSyncClock(USART1, &usartSyncClockConfig);
    
    /**  BaudRate is 115200 */
    usartConfig.baudRate = 115200;
    /**  No interrupt */
    usartConfig.interrupt = USART_INT_NONE;
    /**  Enable RX and TX */
    usartConfig.mode = USART_MODE_TX_RX;
    /**  Parity disable */
    usartConfig.parity = USART_PARITY_NONE;
    /**  One stop bit */
    usartConfig.stopBits = USART_STOP_BIT_1;
    /**  Word length is 8bit */
    usartConfig.wordLength = USART_WORD_LEN_8B;
    /**  USART1 configuration */
    USART_Config(USART1, &usartConfig);
    
    /**  Enable USART1 */
    USART_Enable(USART1);
}
