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
* @file fnet_lpc1768_config.h
*
* @author Marcelo Martins Maia do Couto, <marcelo.m.maia@gmail.com>
*
* @date Feb-20-2016
*
* @version 0.0.1.0
*
* @brief NXP's LPC1768 specific configuration file.
*
***************************************************************************/

/************************************************************************
 * !!!DO NOT MODIFY THIS FILE!!!
 ************************************************************************/

#ifndef _FNET_LPC1768_CONFIG_H_
#define _FNET_LPC1768_CONFIG_H_

#define FNET_LPC                        				(1)

#include "lpc17xx_emac.h"

// ******************************************************************************
// Maximum Timer number that is avaiable on the used platform.
// ******************************************************************************
#define FNET_CFG_CPU_TIMER_NUMBER_MAX  					(4)				// Timer 0 - Timer 3

// Timer number used for FNET
#ifndef FNET_CFG_CPU_TIMER_NUMBER
	#define FNET_CFG_CPU_TIMER_NUMBER           		(2)				// LPC_TIM2
#endif

// Size of the internal static heap buffer.
#ifndef FNET_CFG_HEAP_SIZE
	#define FNET_CFG_HEAP_SIZE          				(10 * 1024)		// if you running from internal RAM only !!
#endif

// Default system bus frequency in Hz
#ifndef FNET_CFG_CPU_CLOCK_HZ
	#define FNET_CFG_CPU_CLOCK_HZ						SystemCoreClock
#endif

// LPC Flash Module is not tested yet.
#define FNET_CFG_CPU_FLASH              				(0)
#ifdef FNET_CFG_CPU_FLASH
	#define FNET_CFG_CPU_FLASH_PAGE_SIZE    			(2*1024)
#endif

// The platform does not have second Ethernet Module.
#define FNET_CFG_CPU_ETH1        						(0)

// Ethernet Management Information Base (MIB) Block Counters it's not present on LPC
#define	FNET_CFG_CPU_ETH_MIB							(0)

// Defines the maximum number of incoming frames that may be buffered by the Ethernet module.
#ifndef FNET_CFG_CPU_ETH_RX_BUFS_MAX
	#define FNET_CFG_CPU_ETH_RX_BUFS_MAX    			(EMAC_NUM_RX_FRAG)
#endif

#ifndef FNET_CFG_CPU_ETH_TX_BUFS_MAX
	#define FNET_CFG_CPU_ETH_TX_BUFS_MAX				(EMAC_NUM_TX_FRAG)
#endif

// On-chip Flash size.
#define FNET_CFG_CPU_FLASH_SIZE         				(1024 * 512)    // 512 KB

// SRAM size.
#define FNET_CFG_CPU_SRAM_SIZE          				(1024 * 32)     // 32 KB

// maximum for LPC1768 (Size of DMA memory is 0x3fff): 576
#ifndef FNET_CFG_CPU_ETH0_MTU
	#define FNET_CFG_CPU_ETH0_MTU            			(576)
#endif

#define FNET_CFG_CPU_ETH_HW_TX_PROTOCOL_CHECKSUM    	(0)
#define FNET_CFG_CPU_ETH_HW_RX_PROTOCOL_CHECKSUM    	(0)

//NXP LPC ETHERNET MODULE SETTINGS -------------------------------------------------------------------------
// When enabled (set to "1"), the MAC will pass all frames regardless of type (normal vs. Control).
// When disabled, the MAC does not pass valid Control frames.
#define FNET_CFG_PASS_ALL_RECEIVE_FRAMES				(1)

// Set this bit to append a CRC to every frame whether padding was required or not.
// Must be set if PAD/CRC ENABLE is set. Clear this bit if frames presented to the MAC contain a CRC.
#define FNET_CFG_CRC_ENABLE 							(1)

// When enabled (set to "1"), both transmit and receive frame lengths are compared to
// the Length/Type field. If the Length/Type field represents a length then the check is
//performed. Mismatches are reported in the StatusInfo word for each received frame.
#define FNET_CFG_FRAME_LENGTH_CHECKING					(1)

// When enabled (set to "1"), frames of any length are transmitted and received.
#define FNET_CFG_HUGE_FRAME_ENABLE						(0)

// This bit determines the number of bytes, if any, of proprietary header information
// that exist on the front of IEEE 802.3 frames. When 1, four bytes of header (ignored
// by the CRC function) are added. When 0, there is no proprietary header.
#define FNET_CFG_DELAYED_CRC							(0)

// Set this bit to have the MAC pad all short frames. Clear this bit if frames presented
// to the MAC have a valid length. This bit is used in conjunction with AUTO PAD ENABLE and VLAN PAD ENABLE
#define FNET_CFG_PAD_CRC_ENABLE							(1)

// Set this bit to cause the MAC to pad all short frames to 64 bytes and append a valid CRC.
// Note: This bit is ignored if FNET_CFG_PAD_CRC_ENABLE is cleared.
#define FNET_CFG_VLAN_PAD_ENABLE						(0)

// Set this bit to cause the MAC to automatically detect the type of frame, either tagged or un-tagged,
// by comparing the two octets following the source address with 0x8100 (VLAN Protocol ID) and pad accordingly.
// Note: This bit is ignored if FNET_CFG_PAD_CRC_ENABLE is cleared.
#define FNET_CFG_AUTO_PAD_ENABLE						(0)

// When enabled (set to "1"), the MAC will verify the content of the preamble to ensure it contains 0x55 and is error-free.
// A packet with an incorrect preamble is discarded.
// When disabled, no preamble checking is performed.
#define FNET_CFG_PPREAM_ENF 							(0)

// When enabled (set to "1"), the MAC only allows receive packets which contain preamble fields less
// than 12 bytes in length. When disabled, the MAC allows any length preamble as per the Standard.
#define FNET_CFG_LPREAM_ENF								(0)

// When enabled (set to "1"), the MAC will immediately retransmit following a collision rather than using the
// Binary Exponential Backoff algorithm as specified in the Standard.
#define FNET_CFG_NO_BACKOFF								(0)

// When enabled (set to "1"), after the MAC incidentally causes a collision during back pressure,
// it will immediately retransmit without backoff, reducing the chance of further collisions and ensuring transmit packets get sent.
#define FNET_CFG_BACK_PRESSURE							(0)

// When enabled (set to "1") the MAC will defer to carrier indefinitely as per the Standard.
// When disabled, the MAC will abort when the excessive deferral limit is reached.
#define FNET_CFG_EXCESS_DEF								(0)

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
//#define E_XTAL        									(12000000UL)        	// External Main Crystal frequency
//#define	E_RTC_X											(32768UL)				// External RTC Crystal frequency


#endif /* _FNET_LPC1788_CONFIG_H_ */
