/***************************************************************************
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License Version 3
* or later (the "LGPL").
*
* As a special exception, the copyright holders of the FNET project give you
* permission to link the FNET sources with independent modules to produce an
* executable, regardless of the license terms of these independent modules,
* and to copy and distribute the resulting executable under terms of your
* choice, provided that you also meet, for each linked independent module,
* the terms and conditions of the license of that module.
* An independent module is a module which is not derived from or based
* on this library.
* If you modify the FNET sources, you may extend this exception
* to your version of the FNET sources, but you are not obligated
* to do so. If you do not wish to do so, delete this
* exception statement from your version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License
* and the GNU Lesser General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*
**********************************************************************/ /*!
*
* @file fnet_lpc.c
*
* @author Marcelo Martins Maia do Couto, <marcelo.m.maia@gmail.com>
*
* @date Feb-20-2016
*
* @version 0.0.0.1
*
* @brief LPC port of the FNET stack: CPU-specific API implementation.
*
***************************************************************************/
#include "fnet.h"

#if FNET_LPC

#include "LPC17xx.h"
#include "lpc17xx_uart.h"	/* For Serial. */

/************************************************************************
* NAME: fnet_cpu_reset
*
* DESCRIPTION: Initiate software reset.
*************************************************************************/
void fnet_cpu_reset (void)
{
	NVIC_SystemReset();
	while(1);				// Todo: Do this better....
}

/************************************************************************
 * NAME: fnet_cpu_disable_irq
 *
 * DESCRIPTION: Disable IRQs
 *************************************************************************/
fnet_cpu_irq_desc_t fnet_cpu_irq_disable(void)
{
	return (0);
}

/************************************************************************
* NAME: fnet_cpu_disable_irq
*
* DESCRIPTION: Enables IRQs at interrupt level mask value.
*************************************************************************/
void fnet_cpu_irq_enable(fnet_cpu_irq_desc_t irq_desc)
{
	char x;
	x++; // stub
}

// stub function, we use CMSIS for now
fnet_return_t fnet_cpu_isr_install(fnet_uint32_t vector_number, fnet_uint32_t priority)
{
	return FNET_OK;
}

#endif /*FNET_LPC*/
