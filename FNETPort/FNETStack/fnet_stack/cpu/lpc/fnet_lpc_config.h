/**************************************************************************
* 
* Copyright 2012-2015 by Andrey Butok. FNET Community.
*
***************************************************************************
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
* @file fnet_lpc_config.h
*
* @author Andrey Butok, <EdizonTN@gmail.com>
*
* @date Jun-25-2015
*
* @version 0.0.1.0
*
* @brief NXP's LPC-specific default configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_LPC_CONFIG_H_
#define _FNET_LPC_CONFIG_H_

#include "fnet.h"

#if FNET_LPC
// ******************************************************************************
// LPC is an LITTLE ENDIAN
// ******************************************************************************
/*! @cond */
 #define FNET_CFG_CPU_LITTLE_ENDIAN      				(1)
/*! @endcond */

// ******************************************************************************
//  Vector number of the Ethernet Receive Frame interrupt.
//  NOTE: User application should not change this parameter.
// ******************************************************************************
#ifndef FNET_CFG_CPU_ETH0_VECTOR_NUMBER
	#define FNET_CFG_CPU_ETH0_VECTOR_NUMBER      		(ENET_IRQn)
#endif

// ******************************************************************************
// Vector number of the timer interrupt.
//  NOTE: User application should not change this parameter.
// ******************************************************************************
#ifndef FNET_CFG_CPU_TIMER_VECTOR_NUMBER
	#define FNET_CFG_CPU_TIMER_VECTOR_NUMBER    		(TIMER2_IRQn)
#endif

// ******************************************************************************
//  On-chip SRAM memory start address.
// ******************************************************************************
/*! @cond */
#ifndef FNET_CFG_CPU_SRAM_ADDRESS 
    #define FNET_CFG_CPU_SRAM_ADDRESS   				(0x10000000)
#endif
/*! @endcond */

// ******************************************************************************
// LPC Start FLASH Address
// ******************************************************************************
/*! @cond */
#ifndef FNET_CFG_CPU_FLASH_ADDRESS
    #define FNET_CFG_CPU_FLASH_ADDRESS      			(0x00000000)
#endif
/*! @endcond */


// ******************************************************************************
// LPC haven't CACHE
// ******************************************************************************
/*! @cond */
#define FNET_CFG_CPU_CACHE              				(0)
/*! @endcond */

// ******************************************************************************
// Minimal block of data for save to Flash
// ******************************************************************************
/*! @cond */
#define FNET_CFG_CPU_FLASH_PROGRAM_SIZE 				(4U)
/*! @endcond */

#endif /* FNET_LPC */

#endif /* _FNET_LPC_CONFIG_H_ */
