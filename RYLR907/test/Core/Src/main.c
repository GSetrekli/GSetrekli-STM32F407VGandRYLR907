/*
 * main.c
 *
 *  Created on: Apr 26, 2021
 *      Author: goknur
 *  ST_LORA2
 *	STM32F407VG and RYLR900.
 *	STM32F407VG is RX (IT MODE RECEIVER).
 *	STM32F407VG is in semi-hosting mode.
 *	Other LoRa (RYLR900) is connected to other computer via TTL converter,
 *	Arduino port is used for the computer that is connected to TTL
 *
 */


/*
 *  In this application, STM32F407VG can get only 3 different messages from other LoRa,
 *  ST knows what received message is from other LoRa.
 *
 */
#include <string.h>
#include <stdio.h>
#include<stdint.h>
#include<stdarg.h>
#include <stddef.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"
//#include "stm32f4xx_hal.h" /*It is declared in main.h and main.h is in msp.c and it.c*/
#include "main.h"


#define TRUE 		1
#define FALSE 		0

#define NoOfMsg 	3


void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
void delay(uint32_t number);
static void Messages(char *p[]);

UART_HandleTypeDef huart2;


char data_buffer[100];
uint8_t recvd_data;
uint32_t count=0;
uint8_t reception_complete = FALSE;
uint8_t OrderOfAllMsg=0;
int cmpValue,cmpValue1,cmpValue2;

char *AllMsg[NoOfMsg];

char tempMsg[100];
char OneMsg[100];
char *ret;
uint32_t control = 0;

extern void initialise_monitor_handles();

void printmsg(char *format,...)
 {

	char str[80];

	/*Extract the the argument list using VA apis */
	va_list args;
	va_start(args, format);
	vsprintf(str, format,args);
	HAL_UART_Transmit(&huart2,(uint8_t *)str, strlen(str),HAL_MAX_DELAY);
	va_end(args);

 }
//extern void initialise_monitor_handles();

static void Messages(char *p[])
{
	p[0]=",Hello!,";
	p[1]=",enes,";
	p[2]=",goku,";
}

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	//initialise_monitor_handles();

	//printmsg("AT+SEND=0,5,HELLO\r\n");
	printf("Starting...\n");
	delay(500000);
	delay(500000);
	delay(500000);

	Messages(AllMsg);

	//strcpy(OneMsg,AllMsg[0]);
	//printf("OneMsg: %c\n",OneMsg[5]);

    while(1)
    {
    	HAL_UART_Receive_IT(&huart2,&recvd_data,1);
    }

    return 0;
}


void SystemClockConfig(void)
{


}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if ( HAL_UART_Init(&huart2) != HAL_OK )
	{
	//There is a problem
		Error_handler();
	}

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(recvd_data == '\r')
	{
		reception_complete = TRUE;
		data_buffer[count++]='\r';
		//HAL_UART_Transmit(huart,data_buffer,count,HAL_MAX_DELAY);
		printf("data_buffer: %s\n",data_buffer);
		//strcpy(OneMsg,AllMsg[1]);

		for(int OrderOfAllMsg=0;OrderOfAllMsg<NoOfMsg;OrderOfAllMsg++)
		{
			strcpy(OneMsg,AllMsg[OrderOfAllMsg]);

			ret = strstr(data_buffer, OneMsg);

			if (ret != NULL)
			{
				printmsg("AT+SEND=0,%d,%s is found\r\n",(strlen(OneMsg)+9),OneMsg);
				if (OrderOfAllMsg==0)
				{
					printf("Code implementation for Hello!\n");
				}
				else if (OrderOfAllMsg==1)
				{
					printf("Code implementation for enes\n");
				}
				else if (OrderOfAllMsg==2)
				{
					printf("Code implementation for goku\n");
				}

			}

			else
			{
				printf("NULL\n");
			}

		}

		delay(500000);
		delay(500000);
		delay(500000);

		count=0;
		control=0;
		//recvd_data=0;

		memset(&data_buffer,0,sizeof(data_buffer));

	}

	else
	{
		data_buffer[count++] = recvd_data;
	}

}


void Error_handler(void)

{
	while(1);
}

void delay(uint32_t number)

{
	for (uint32_t i=0;i<number;i++ );
}
