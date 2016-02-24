#include "fnet.h"

#if FNET_LPC

#ifdef __USE_CMSIS
	#include "LPC17xx.h"
#endif

#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

/********************************************************************/
void fnet_cpu_serial_putchar(fnet_index_t port_number, fnet_char_t character )
{
	switch(port_number)
	{
		case 0:
//			UART_SendByte(LPC_UART0, character);
			UART_Send(LPC_UART0, &character, 1, BLOCKING);
		break;
		case 1:
			UART_SendByte(LPC_UART1, character);
		break;
		case 2:
			UART_SendByte(LPC_UART2, character);
		break;
		case 3:
			UART_SendByte(LPC_UART3, character);
		break;

	}
 }
/********************************************************************/
fnet_int32_t fnet_cpu_serial_getchar( fnet_index_t port_number )
{
	fnet_uint8_t digit = '\0';

	switch(port_number)
	{
		case 0:
//			digit = UART_ReceiveByte(LPC_UART0);
			UART_Receive(LPC_UART0, &digit, 1, BLOCKING);
		break;
		case 1:
			digit = UART_ReceiveByte(LPC_UART1);
		break;
		case 2:
			digit = UART_ReceiveByte(LPC_UART2);
		break;
		case 3:
			digit = UART_ReceiveByte(LPC_UART3);
		break;
	}

	if(digit != '\0')
	{
		return (digit);
	}
	else
	{
		return FNET_ERR;
	}
}

/********************************************************************/
void fnet_cpu_serial_init(fnet_index_t port_number, fnet_uint32_t baud_rate)
{
	UART_CFG_Type serialconfig;
	PINSEL_CFG_Type PinCfg;

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);

	UART_ConfigStructInit(&serialconfig);

	serialconfig.Baud_rate = baud_rate;

	switch(port_number)
	{
		case 0:
			UART_Init(LPC_UART0, &serialconfig);
			UART_TxCmd(LPC_UART0, ENABLE);
		break;
		case 1:
			UART_Init(LPC_UART1, &serialconfig);
			UART_TxCmd(LPC_UART1, ENABLE);
		break;
		case 2:
			UART_Init(LPC_UART2, &serialconfig);
			UART_TxCmd(LPC_UART2, ENABLE);
		break;
		case 3:
			UART_Init(LPC_UART3, &serialconfig);
			UART_TxCmd(LPC_UART3, ENABLE);
		break;
	}
}

#endif
