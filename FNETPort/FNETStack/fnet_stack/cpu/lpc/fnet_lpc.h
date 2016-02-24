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
* @file fnet_mpc.h
*
* @author Andrey Butok, <EdizonTN@gmail.com>
*
* @date Jun-25-2015
*
* @version 0.0.1.0
*
* @brief LPC Registers definitions.
*
***************************************************************************/

#ifndef _FNET_LPC_H_

#define _FNET_LPC_H_

#include "fnet.h"

#if FNET_LPC

#include "LPC17xx.h"

/*********************************************************************
* The basic data types.
*********************************************************************/
typedef unsigned char 				fnet_uint8_t;      		//  8 bits
typedef unsigned short int 			fnet_uint16_t; 			// 16 bits
typedef unsigned long int 			fnet_uint32_t; 			// 32 bits
typedef signed char 				fnet_int8_t;          	//  8 bits
typedef signed short int 			fnet_int16_t;  			// 16 bits
typedef signed long int 			fnet_int32_t;     		// 32 bits
typedef volatile fnet_uint8_t 		fnet_vuint8_t;			//  8 bits
typedef volatile fnet_uint16_t 		fnet_vuint16_t;			// 16 bits
typedef volatile fnet_uint32_t 		fnet_vuint32_t;			// 32 bits


/* NULL pointer */
#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef __NOP
#define __NOP                             __nop
#endif

#define FNET_LPCETH_DMA_BLOCK_START 0x2007C000 	// AHB Block 1 on the LPC
#define FNET_LPCETH_DMA_BLOCK_LEN 	0x3FFF 		// 16k

/*********************************************************************
*
* Ethernet MAC Controller (EMAC)
*
*********************************************************************/
#define FNET_FEC0_BASE_ADDR                           	((fnet_vuint32*)(void*)(0x20084000))

/************************************************************************
*     FEC registers. In NXP's LPS have name: EMAC - Ethernet MAC, also include Clocking and power control registers
*************************************************************************/
typedef struct
{
		fnet_vuint32_t 	MAC1;				// MAC configuration register 1. Address: 0x2008 4000
		fnet_vuint32_t 	MAC2;				// MAC configuration register 2. Address: 0x2008 4004
		fnet_vuint32_t 	IPGT;				// Back-to-Back Inter-Packet-Gap register. Address: 0x2008 4008
		fnet_vuint32_t 	IPGR;				// Non Back-to-Back Inter-Packet-Gap register. Address: 0x2008 4000C
		fnet_vuint32_t 	CLRT;				// Collision window / Retry register. Address: 0x2008 4010
		fnet_vuint32_t 	MAXF;				// Maximum Frame register. Address: 0x2008 4014
		fnet_vuint32_t 	SUPP;				// PHY Support register. Address: 0x2008 4018
		fnet_vuint32_t 	TEST;				// Test register. Address: 0x2008 401C
		fnet_vuint32_t 	MCFG;				// MII Mgmt Configuration register. Address: 0x2008 4020
		fnet_vuint32_t 	MCMD;				// MII Mgmt Command register. Address: 0x2008 4024
		fnet_vuint32_t 	MADR;				// MII Mgmt Address register. Address: 0x2008 4028
		fnet_vuint32_t 	MWTD;				// MII Mgmt Write Data register. Address: 0x2008 402c
		fnet_vuint32_t 	MRDD;				// MII Mgmt Read Data register. Address: 0x2008 4030
		fnet_vuint32_t 	MIND;				// MII Mgmt Indicators register. Address: 0x2008 4034
		fnet_vuint32_t 	reserved1[2];
		fnet_vuint32_t 	SA0;				// Station Address 0 register. Address: 0x2008 4040
		fnet_vuint32_t 	SA1;				// Station Address 1 register. Address: 0x2008 4044
		fnet_vuint32_t 	SA2;				// Station Address 2 register. Address: 0x2008 4048
		fnet_vuint32_t 	reserved2[45];
		fnet_vuint32_t 	Command;            // Control Register. Address: 0x2008 4100
		fnet_vuint32_t 	Status;				// Status Register. Address: 0x2008 4104
		fnet_vuint32_t 	RxDescriptor;		// Receive descriptor base address register. Address: 0x2008 4108
		fnet_vuint32_t 	RxStatus;			// Receive status base address register. Address: 0x2008 410C
		fnet_vuint32_t 	RxDescriptorNumber;	// Receive number of descriptors register. Address: 0x2008 4110
		fnet_vuint32_t 	RxProduceIndex;		// Receive produce index register. Address: 0x2008 4114
		fnet_vuint32_t 	RxConsumeIndex;		// Receive consume index register. Address: 0x2008 4118
		fnet_vuint32_t 	TxDescriptor;		// Transmit descriptor base address register. Address: 0x2008 411C
		fnet_vuint32_t 	TxStatus;			// Transmit status base address register. Address: 0x2008 4120
		fnet_vuint32_t 	TxDescriptorNumber;	// Transmit number of descriptors register. Address: 0x2008 4124
		fnet_vuint32_t 	TxProduceIndex;		// Transmit produce index register. Address: 0x2008 4128
		fnet_vuint32_t 	TxConsumeIndex;		// Transmit consume index register. Address: 0x2008 412C
		fnet_vuint32_t 	reserved3[10];
		fnet_vuint32_t 	TSV0;				// Transmit status vector 0 register. Address: 0x2008 4158
		fnet_vuint32_t 	TSV1;				// Transmit status vector 1 register. Address: 0x2008 415C
		fnet_vuint32_t 	RSV;				// Receive status vector register. Address: 0x2008 4160
		fnet_vuint32_t 	reserved4[3];
		fnet_vuint32_t 	FlowControlCounter;	// Flow control counter register. Address: 0x2008 4170
		fnet_vuint32_t 	FlowControlStatus;	// Flow control status register. Address: 0x2008 4174
		fnet_vuint32_t 	reserved5[34];
		fnet_vuint32_t 	RxFilterCtrl;		// Receive filter control register. Address: 0x2008 4200
		fnet_vuint32_t 	RxFilterWoLStatus;	// Receive filter WoL status register. Address: 0x2008 4204
		fnet_vuint32_t 	RxFilterWoLClear;	// Receive filter WoL clear register. Address: 0x2008 4208
		fnet_vuint32_t 	reserved6;
		fnet_vuint32_t 	HashFilterL;		// Hash filter table LSBs register. Address: 0x2008 4210
		fnet_vuint32_t 	HashFilterH;		// Hash filter table MSBs register. Address: 0x2008 4214
		fnet_vuint32_t 	reserved7[882];
		fnet_vuint32_t 	IntStatus;			// Interrupt status register. Address: 0x2008 4FE0
		fnet_vuint32_t 	IntEnable;			// Interrupt enable register. Address: 0x2008 4FE4
		fnet_vuint32_t 	IntClear;			// Interrupt clear register. Address: 0x2008 4FE8
		fnet_vuint32_t 	IntSet;				// Interrupt set register. Address: 0x2008 4FEC
		fnet_vuint32_t 	reserved8;
		fnet_vuint32_t 	PowerDown;			// Power-down register. Address: 0x2008 4FF4

} fnet_fec_reg_t;

#define COMMAND_ENABLE_RECEIVE 		0x00000001
#define COMMAND_ENABLE_TRANSMIT 	0x00000002
#define COMMAND_REGISTER_RESET 		0x00000008
#define COMMAND_TX_RESET 			0x00000010
#define COMMAND_RX_RESET 			0x00000020
#define COMMAND_PASS_RUNT_FRAMES 	0x00000040
#define COMMAND_PASS_RX_FILTER 		0x00000080
#define COMMAND_PASS_RMII 			0x00000200
#define COMMAND_FULL_DUPLEX 		0x00000400

/* MAC Configuration Register 1 */
#define EMAC_MAC1_MASK      	 0xcf1f      /*MAC1 register mask*/
/* MAC Configuration Register 2 */
#define EMAC_MAC2_MASK      	 0x73ff      /*MAC2 register mask*/

/* MII Mgmt Command register.	*/
#define EMAC_MCMD_NONE 								0

/************************************************************************
*     MII Register Indexes.
*************************************************************************/
#define FNET_FEC_MII_REG_CR          (0x0000U)   /* Control Register */
#define FNET_FEC_MII_REG_SR          (0x0001U)   /* Status Register */
#define FNET_FEC_MII_REG_IDR1        (0x0002U)   /* Identification Register #1 */
#define FNET_FEC_MII_REG_IDR2        (0x0003U)   /* Identification Register #2 */
#define FNET_FEC_MII_REG_ANAR        (0x0004U)   /* Auto-Negotiation Advertisement Register */
#define FNET_FEC_MII_REG_ANLPAR      (0x0005U)   /* Auto-Negotiation Link Partner Ability Register */
#define FNET_FEC_MII_REG_ANER        (0x0006U)   /* Auto-Negotiation Expansion Register */
#define FNET_FEC_MII_REG_ANNPTR      (0x0007U)   /* Auto-Negotiation Next Page TX Register */

#if FNET_CFG_CPU_MPC5744P
   #define FNET_FEC_MII_REG_PHYSTS      (0x0010U)   /* PHY Status Register */
   #define FNET_FEC_MII_REG_ICR         (0x0011U)   /* Interrupt Control Register */
   #define FNET_FEC_MII_REG_ISR         (0x0012U)   /* Interrupt Status Register */
   #define FNET_FEC_MII_REG_FCSCR       (0x0014U)   /* False Carrier Sense Counter Register */
   #define FNET_FEC_MII_REG_RECR        (0x0015U)   /* Receive Error Counter Register */
   #define FNET_FEC_MII_REG_PCSR        (0x0016U)   /* PCS Sub-Layer Configuration and Status Register */
   #define FNET_FEC_MII_REG_RBR         (0x0017U)   /* RMII and Bypass Register */
   #define FNET_FEC_MII_REG_LEDCR       (0x0018U)   /* LED Direct Control Register */
   #define FNET_FEC_MII_REG_PHYCR       (0x0019U)   /* PHY Status Register */
   #define FNET_FEC_MII_REG_10BTSCR     (0x001AU)   /* 10Base-T Status/Control Register */
   #define FNET_FEC_MII_REG_CDCTRL1     (0x001BU)   /* CD Test Control Register and BIST Extensions Register */
   #define FNET_FEC_MII_REG_EDCR        (0x001DU)   /* Energy Detect Control Register */
#else
   #define FNET_FEC_MII_REG_ICR         (0x0010U)   /* Interrupt Control Register */
   #define FNET_FEC_MII_REG_PSR         (0x0011U)   /* Proprietary Status Register */
   #define FNET_FEC_MII_REG_PCR         (0x0012U)   /* Proprietary Control Register */
#endif

#define FNET_FEC_MII_REG_SR_LINK_STATUS (0x0004U)
#define FNET_FEC_MII_REG_SR_AN_ABILITY  (0x0008U)
#define FNET_FEC_MII_REG_SR_AN_COMPLETE (0x0020U)

#define FNET_FEC_MII_REG_ANAR_NEXT_PAGE (0x8000U)

#define FNET_FEC_MII_REG_CR_RESET       (0x8000U)    /* Resetting a port is accomplished by setting this bit to 1.*/
#define FNET_FEC_MII_REG_CR_LOOPBACK    (0x4000U)    /* Determines Digital Loopback Mode. */
#define FNET_FEC_MII_REG_CR_DATARATE    (0x2000U)    /* Speed Selection bit.*/
#define FNET_FEC_MII_REG_CR_ANE         (0x1000U)    /* Auto-Negotiation Enable bit. */
#define FNET_FEC_MII_REG_CR_PDWN        (0x0800U)    /* Power Down bit. */
#define FNET_FEC_MII_REG_CR_ISOL        (0x0400U)    /* Isolate bit.*/
#define FNET_FEC_MII_REG_CR_ANE_RESTART (0x0200U)    /* Restart Auto-Negotiation bit.*/
#define FNET_FEC_MII_REG_CR_DPLX        (0x0100U)    /* Duplex Mode bit.*/

#define FNET_FEC_MII_TIMEOUT            (0x10000U)   /* Timeout counter for MII communications.*/

#define FNET_FEC_BUF_SIZE           (((FNET_CFG_CPU_ETH0_MTU>FNET_CFG_CPU_ETH1_MTU)?FNET_CFG_CPU_ETH0_MTU:FNET_CFG_CPU_ETH1_MTU)+FNET_ETH_HDR_SIZE+FNET_ETH_CRC_SIZE+16U) /* Ring Buffer sizes in bytes.*/
#define FNET_FEC_TX_BUF_NUM         (FNET_CFG_CPU_ETH_TX_BUFS_MAX)
#define FNET_FEC_RX_BUF_NUM         (FNET_CFG_CPU_ETH_RX_BUFS_MAX)

/* frequency of less than or equal to 2.5 MHz to be compliant with the IEEE 802.3 MII specification. */
#define FNET_FEC_MII_CLOCK_KHZ      (2500U)

#define _ETH_INT_ENABLED			(EMAC_INT_RX_OVERRUN | EMAC_INT_RX_DONE )

#endif /* FNET_LPC */
#endif /* _FNET_LPC_H_ */
