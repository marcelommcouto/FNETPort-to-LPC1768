/** 
 *   Modulo: fnet_lpc1768_ethernet
 *   @file fnet_lpc1768_ethernet.c
 *   Veja fnet_lpc1768_ethernet.h para mais informações.
 ******************************************************************************/

/*******************************************************************************
 *                             MODULOS UTILIZADOS							   *
 ******************************************************************************/

/*
 * Inclusão de arquivos de cabeçalho da ferramenta de desenvolvimento.
 * Por exemplo: '#include <stdlib.h>'.
 */
#include <stdint.h>   /* Para as definições de uint8_t/uint16_t */
#include <stdbool.h>  /* Para as definições de true/false */
#include "stdio.h"

/*
 * Inclusão de arquivos de cabeçalho sem um arquivo ".c" correspondente.
 * Por exemplo: 
 * #include "stddefs.h" 
 * #include "template_header.h"
 */
#include "fnet.h"

#if FNET_LPC && FNET_CFG_CPU_ETH0

#ifdef __USE_CMSIS
	#include "LPC17xx.h"
#endif
#include "stack/fnet_eth_prv.h"
#include "stack/fnet_netif_prv.h"

#include "fnet_lpc1768_ethernet.h"

#include "lpc17xx_emac.h"

/*
 * Inclusão de arquivos de cabeçalho de outros módulos utilizados por este.
 * Por exemplo: '#include "serial.h"'.
 */

/*
 * Inclusão dos arquivos ".tab.h" deste módulo.
 * Por exemplo: 
 * #include "barcode.tab.h"
 * #include "template.tab.h"
 */

/*
 * Inclusão do arquivo de cabeçalho deste módulo.
 */

/*******************************************************************************
 *                     CONSTANTES E DEFINICOES DE MACRO						   *
 ******************************************************************************/
/* MII Mgmt Configuration register - Clock divider setting */
const uint8_t ethclkdiv[] = {4, 6, 8, 10, 14, 20, 28, 36, 40, 44, 48, 52, 56, 60, 64};

const fnet_netif_api_t fnet_lpceth_api =
{
    FNET_NETIF_TYPE_ETHERNET,       	/* Data-link type. */
    sizeof(fnet_mac_addr_t),
    fnet_lpceth_init,                  	/* Initialization function.*/
	fnet_lpceth_release,               	/* Shutdown function.*/
#if FNET_CFG_IP4
	fnet_eth_output_ip4,            	/* IPv4 Transmit function.*/
#endif
	fnet_eth_change_addr_notify,   		/* Address change notification function.*/
	fnet_eth_drain,                 	/* Drain function.*/
	fnet_lpceth_get_hw_addr,
	fnet_lpceth_set_hw_addr,
	fnet_lpceth_is_connected,
	fnet_lpceth_get_statistics
#if FNET_CFG_MULTICAST
    #if FNET_CFG_IP4
        ,fnet_eth_multicast_join_ip4
	    ,fnet_eth_multicast_leave_ip4
    #endif
    #if FNET_CFG_IP6
        ,fnet_eth_multicast_join_ip6
	    ,fnet_eth_multicast_leave_ip6
    #endif
#endif
#if FNET_CFG_IP6
    ,fnet_eth_output_ip6            /* IPv6 Transmit function.*/
#endif
};

/** @brief Receive group interrupts */
#define RXINTGROUP (EMAC_INT_RX_OVERRUN | EMAC_INT_RX_ERR | EMAC_INT_RX_DONE)

/** @brief Transmit group interrupts */
#define TXINTGROUP (EMAC_INT_TX_UNDERRUN | EMAC_INT_TX_ERR | EMAC_INT_TX_DONE)

/* The PHY address connected the to MII/RMII */
#define PHYADDR 1
#define DP83848_PHYSTS_REG  0x10	/*!< PHY special control/status Reg */

/* DP83848_PHYSTS_REG status definitions */
#define LAN8_SPEEDMASK      (3 << 1)	/*!< Speed and duplex mask */
#define LAN8_SPEED100F      (2 << 1)	/*!< 100BT full duplex */
#define LAN8_SPEED10F       (3 << 1)	/*!< 10BT full duplex */
#define LAN8_SPEED100H      (0 << 1)	/*!< 100BT half duplex */
#define LAN8_SPEED10H       (1 << 1)	/*!< 10BT half duplex */

#define PHY_LINK_ERROR     (1 << 0)	/*!< PHY status bit for link error */
#define PHY_LINK_BUSY      (1 << 1)	/*!< PHY status bit for MII link busy */
#define PHY_LINK_CHANGED   (1 << 2)	/*!< PHY status bit for changed state (not persistent) */
#define PHY_LINK_CONNECTED (1 << 3)	/*!< PHY status bit for connected state */
#define PHY_LINK_SPEED100  (1 << 4)	/*!< PHY status bit for 100Mbps mode */
#define PHY_LINK_FULLDUPLX (1 << 5)	/*!< PHY status bit for full duplex mode */

/*******************************************************************************
 *                      ESTRUTURAS E DEFINIÇÕES DE TIPOS					   *
 ******************************************************************************/
typedef struct { } fnet_lpceth_if_t;

/*******************************************************************************
 *                        VARIÁVEIS PUBLICAS (Globais)						   *
 ******************************************************************************/
fnet_lpceth_if_t fnet_lpceth0_if;

fnet_eth_if_t fnet_lpceth_eth0_if =
{
    &fnet_lpceth0_if,              /* Points to CPU-specific control data structure of the interface. */
    0,
    fnet_lpceth_output
#if FNET_CFG_MULTICAST
    ,
    fnet_lpceth_multicast_join,
    fnet_lpceth_multicast_leave,
#endif /* FNET_CFG_MULTICAST */
};

fnet_netif_t fnet_eth0_if =
{
	0,                          /* Pointer to the next net_if structure.*/
	0,                          /* Pointer to the previous net_if structure.*/
	"eth0",                     /* Network interface name.*/
	FNET_CFG_CPU_ETH0_MTU,      /* Maximum transmission unit.*/
	&fnet_lpceth_eth0_if,       /* Points to interface specific data structure.*/
	&fnet_lpceth_api            /* Interface API */
};

/*******************************************************************************
 *                  DECLARACOES DE VARIAVEIS PRIVADAS (static)				   *
 ******************************************************************************/
static fnet_uint8_t recvPacketsWaiting = 0;
static fnet_uint8_t txDescriptorStatus[EMAC_NUM_TX_FRAG];
static fnet_uint32_t lastInterruptStatus = 0;
static fnet_uint8_t usedTxDescr = 0;
static fnet_uint8_t isTxDone = 0;
static fnet_uint32_t recvPackets = 0;
static fnet_uint32_t sentPackets = 0;
static fnet_uint8_t *rxFragmentPtr;
static fnet_uint8_t *txFragmentPtr;

/*******************************************************************************
 *                   PROTOTIPOS DAS FUNCOES PRIVADAS (static)				   *
 ******************************************************************************/
static void fnet_lpceth_interrupt_handler_top(void *cookie);
static void fnet_lpceth_interrupt_handler_bottom(void *cookie);
static fnet_uint32_t fnet_lpceth_crc_hash(fnet_mac_addr_t multicast_addr);

/*******************************************************************************
 *                      IMPLEMENTACAO DAS FUNCOES PUBLICAS					   *
 ******************************************************************************/
/*
 * A documentação destas funções é realizada no arquivo ".h" deste módulo.
 */

void fnet_lpceth_write_to_phy(fnet_uint8_t reg, fnet_uint16_t value)
{
	/* Write a data 'Value' to PHY register 'PhyReg'. */
	fnet_uint32_t tout;

	/* Write value at PHY address and register */
	LPC_EMAC->MCMD = 0;
	LPC_EMAC->MADR = (((PHYADDR & 0x1f) << 8) | (reg & 0x1f));
	LPC_EMAC->MWTD = value;

	/* Wait until operation completed */
	tout = 0;
	for (tout = 0; tout < EMAC_MII_WR_TOUT; tout++)
	{
		if ((LPC_EMAC->MIND & EMAC_MIND_BUSY) == 0)
		{
//			return (0);
		}
	}
	// Time out!
//	return (-1);
}

fnet_uint16_t fnet_lpceth_read_from_phy(fnet_uint8_t reg)
{
	/* Read a PHY register 'PhyReg'. */
	fnet_uint32_t tout;

	LPC_EMAC->MADR = (((PHYADDR & 0x1f) << 8) | (reg & 0x1f));
	LPC_EMAC->MCMD = EMAC_MCMD_READ;

	/* Wait until operation compllastInterruptStatuseted */
	tout = 0;
	for (tout = 0; tout < EMAC_MII_RD_TOUT; tout++)
	{
		if ((LPC_EMAC->MIND & EMAC_MIND_BUSY) == 0)
		{
			LPC_EMAC->MCMD = 0;
			return (LPC_EMAC->MRDD);
		}
	}
	// Time out!
	return (-1);
}

int fnet_lpceth_get_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
	hw_addr[0] = (LPC_EMAC->SA0 & 0xFF00) >> 8;
	hw_addr[1] = (LPC_EMAC->SA0 & 0xFF);
	hw_addr[2] = (LPC_EMAC->SA1 & 0xFF00) >> 8;
	hw_addr[3] = (LPC_EMAC->SA1 & 0xFF);
	hw_addr[4] = (LPC_EMAC->SA2 & 0xFF00) >> 8;
	hw_addr[5] = (LPC_EMAC->SA2 & 0xFF);

	return (FNET_OK);
}
int fnet_lpceth_set_hw_addr(fnet_netif_t *netif, fnet_uint8_t *hw_addr)
{
	LPC_EMAC->SA0 = (hw_addr[0] << 8) | hw_addr[1];
	LPC_EMAC->SA1 = (hw_addr[2] << 8) | hw_addr[3];
	LPC_EMAC->SA2 = (hw_addr[4] << 8) | hw_addr[5];

	return (FNET_OK);
}

int fnet_lpceth_get_statistics(struct fnet_netif *netif, struct fnet_netif_statistics * statistics)
{
	statistics->tx_packet = sentPackets;
	statistics->rx_packet = recvPackets;
	return FNET_OK;
}

int fnet_lpceth_is_connected(fnet_netif_t *netif)
{
	return FNET_ERR;
}

int fnet_lpceth_init(fnet_netif_t *netif)
{
	volatile fnet_uint32_t loop;
	fnet_uint32_t tmp;
	fnet_uint16_t tmp16;

	fnet_println("Ethernet PWR On");

	LPC_SC->PCONP |= (1 << 30);

    LPC_PINCON->PINSEL2 = 0x50150105;
    LPC_PINCON->PINSEL3 = (LPC_PINCON->PINSEL3 & ~0x0000000F) | 0x00000005;

    LPC_EMAC->MAC1 = EMAC_MAC1_RES_TX | EMAC_MAC1_RES_MCS_TX | EMAC_MAC1_RES_RX | EMAC_MAC1_RES_MCS_RX | EMAC_MAC1_SIM_RES | EMAC_MAC1_SOFT_RES;
    LPC_EMAC->Command = EMAC_CR_REG_RES | EMAC_CR_TX_RES | EMAC_CR_RX_RES | EMAC_CR_PASS_RUNT_FRM;

	for (loop = 100; loop; loop--);

	fnet_println("Done reset MAC Datapaths.");

	/* Initial MAC configuration for  full duplex, 100Mbps, inter-frame gap use default values */
	LPC_EMAC->MAC1 = EMAC_MAC1_PASS_ALL;
	LPC_EMAC->MAC2 = EMAC_MAC2_FULL_DUP | EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_EN;

	LPC_EMAC->Command = EMAC_CR_FULL_DUP | EMAC_CR_PASS_RUNT_FRM | EMAC_CR_RMII;

//	LPC_EMAC->IPGT = EMAC_IPGT_FULL_DUP;
	LPC_EMAC->IPGR = EMAC_IPGR_P2_DEF;
//	LPC_EMAC->SUPP = EMAC_SUPP_SPEED;
	LPC_EMAC->MAXF = EMAC_ETH_MAX_FLEN;
	LPC_EMAC->CLRT = EMAC_CLRT_DEF;

	LPC_EMAC->MAC2 &= ~(EMAC_MAC2_HUGE_FRM_EN);		/* Disable Huge MAC Frames. */

	LPC_EMAC->Command |= EMAC_CR_PASS_RX_FILT;		/* Setup default filter */
	LPC_EMAC->IntClear = 0xffff;					/* Clear all MAC interrupts */
	LPC_EMAC->IntEnable = 0;						/* Disable MAC interrupts */

	/* Initialize the PHY */
	tmp = SystemCoreClock / EMAC_MCFG_MII_MAXCLK;
	for (loop = 0; loop < sizeof(ethclkdiv); loop++)
	{
		if (ethclkdiv[loop] >= tmp) break;
	}

	LPC_EMAC->MCFG = ((loop & 0x0f) << 2) | EMAC_MCFG_RES_MII;	/*  Save clock divider and Write to MII configuration register and reset */

	for (loop = 100; loop; loop--);								/* Wait a bit. */

	LPC_EMAC->MCFG &= ~(EMAC_MCFG_RES_MII);						/* release reset */

	fnet_println("About to Write to PHY");

	/* Only first read and write are checked for failure */
	/* Put the DP83848C in reset mode and wait for completion */
	fnet_lpceth_write_to_phy(EMAC_PHY_REG_BMCR, EMAC_PHY_BMCR_RESET);

	// Loop until hardware reset completes
	for (loop = 0; loop < EMAC_PHY_RESP_TOUT; loop++)
	{
		tmp16 = fnet_lpceth_read_from_phy(EMAC_PHY_REG_BMCR);
		if (!(tmp16 & (EMAC_PHY_BMCR_RESET | EMAC_PHY_BMCR_POWERDOWN)))
		{
			break;	// Reset has completed
		}
	}

	/* Setup Auto Negotiation link */
	fnet_lpceth_write_to_phy(EMAC_PHY_REG_BMCR, EMAC_PHY_BMCR_AN);

	for(loop = 0; loop < EMAC_PHY_RESP_TOUT; loop++)
	{
		tmp16 = fnet_lpceth_read_from_phy(EMAC_PHY_REG_BMSR);
		if (tmp16 & EMAC_PHY_BMSR_LINK_ESTABLISHED)
		{
			break;
		}
		else
		{
			fnet_timer_delay(10); // wait a bit
		}
		// TODO: Figure out what to do in event of timeout..
	}

	// Read the autonegotiation partner register to find out if the other end is 100BASE-TX FULL
	tmp16 = fnet_lpceth_read_from_phy(DP83848_PHYSTS_REG);
	if (tmp16 & LAN8_SPEED100F)
	{
		LPC_EMAC->MAC2 |= EMAC_MAC2_FULL_DUP;
		LPC_EMAC->Command |= EMAC_CR_FULL_DUP;
		LPC_EMAC->IPGT = EMAC_IPGT_FULL_DUP;
		LPC_EMAC->SUPP = EMAC_SUPP_SPEED;
		fnet_printf("100BASE-TX Full Duplex");
	}

	fnet_lpceth_init_dma();

#if FNET_CFG_MULTICAST
	LPC_EMAC->Command &= ~EMAC_CR_PASS_RX_FILT;
	LPC_EMAC->RxFilterCtrl |= (EMAC_RFC_PERFECT_EN | EMAC_RFC_BCAST_EN | EMAC_RFC_MCAST_HASH_EN);
#else
	LPC_EMAC->RxFilterCtrl |= (EMAC_RFC_PERFECT_EN | EMAC_RFC_BCAST_EN);
#endif

	/* Clear and enable rx/tx interrupts */
	LPC_EMAC->IntEnable |= (RXINTGROUP | TXINTGROUP);

	/* Enable RX and TX */
	LPC_EMAC->Command |= EMAC_CR_TX_EN;
	LPC_EMAC->Command |= EMAC_CR_RX_EN;
	LPC_EMAC->MAC1 |= EMAC_MAC1_REC_EN;

	NVIC_EnableIRQ(ENET_IRQn);

	fnet_isr_vector_init(ENET_IRQn, fnet_lpceth_interrupt_handler_top, fnet_lpceth_interrupt_handler_bottom, FNET_CFG_CPU_ETH_VECTOR_PRIORITY,0);

	return FNET_OK;
}

/* This acts as the receive handler, trigged by an interrupt on reception of an Ethernet frame */
void ENET_IRQHandler(void)
{
	fnet_isr_handler(ENET_IRQn);
}

/******************************************************************************
 *                    IMPLEMENTACAO DAS FUNCOES PRIVADAS					  *
 *****************************************************************************/
void fnet_lpceth_init_dma(void)
{
	fnet_memset((void *)FNET_LPCETH_DMA_BLOCK_START,0,FNET_LPCETH_DMA_BLOCK_LEN);
	fnet_uint8_t loop;

	LPC_EMAC->RxDescriptor = FNET_LPCETH_DMA_BLOCK_START;
	LPC_EMAC->RxStatus = LPC_EMAC->RxDescriptor + (sizeof(RX_Desc) * EMAC_NUM_RX_FRAG);
	rxFragmentPtr = (uint8_t *)(LPC_EMAC->RxStatus + (sizeof(RX_Stat) * EMAC_NUM_RX_FRAG));
	LPC_EMAC->TxDescriptor = (uint32_t)(rxFragmentPtr + (EMAC_ETH_MAX_FLEN * EMAC_NUM_RX_FRAG));
	LPC_EMAC->TxStatus = LPC_EMAC->TxDescriptor + (sizeof(TX_Desc) * EMAC_NUM_TX_FRAG);
    txFragmentPtr = (uint8_t *)(LPC_EMAC->TxStatus + (sizeof(TX_Stat) * EMAC_NUM_TX_FRAG));
    LPC_EMAC->RxDescriptorNumber = EMAC_NUM_RX_FRAG - 1;
    LPC_EMAC->TxDescriptorNumber = EMAC_NUM_TX_FRAG - 1;

    LPC_EMAC->RxConsumeIndex = 0;
    LPC_EMAC->TxProduceIndex = 0;

    for(loop = 0; loop < EMAC_NUM_RX_FRAG; loop++)
    {
    	RX_Desc *rxDesc = (RX_Desc *) (LPC_EMAC->RxDescriptor + loop * sizeof(RX_Desc));
    	rxDesc->Ctrl = EMAC_RCTRL_INT | (EMAC_ETH_MAX_FLEN-1);
    	rxDesc->Packet = (uint32_t)(rxFragmentPtr + (loop * EMAC_ETH_MAX_FLEN));
    	RX_Stat *rxStatus = (RX_Stat *) (LPC_EMAC->RxStatus + loop * sizeof(RX_Stat));
    	rxStatus->Info = 0;
    	rxStatus->HashCRC = 0;
    }

    for(loop = 0; loop < EMAC_NUM_TX_FRAG; loop++)
    {
    	TX_Desc *txDesc = (TX_Desc * )(LPC_EMAC->TxDescriptor + (loop * sizeof(TX_Desc)));
    	txDesc->Packet = (uint32_t)(txFragmentPtr + (loop * EMAC_ETH_MAX_FLEN));
    	txDescriptorStatus[loop] = 0;
    }
}

static void fnet_lpceth_interrupt_handler_top(void *cookie)
{
    lastInterruptStatus = LPC_EMAC->IntStatus;

    if ((LPC_EMAC->IntStatus & EMAC_INT_RX_FIN) == EMAC_INT_RX_FIN)
    {
        LPC_EMAC->Command |= (1 << 5); // Do an RX reset
        LPC_EMAC->IntClear = EMAC_INT_RX_FIN;
    }
    if ((LPC_EMAC->IntStatus & EMAC_INT_RX_DONE) == EMAC_INT_RX_DONE)
    {
        recvPacketsWaiting++;
    }
    if ((LPC_EMAC->IntStatus & EMAC_INT_RX_OVERRUN) == EMAC_INT_RX_OVERRUN)
    {
        LPC_EMAC->Command |= (1 << 5); // Do an RX reset
        fnet_printf("RxOverrun!\n");
    }
    if ((LPC_EMAC->IntStatus & EMAC_INT_TX_UNDERRUN) == EMAC_INT_TX_UNDERRUN)
    {
        fnet_printf("TxUnderrun!\n");
    }
    if ((LPC_EMAC->IntStatus & EMAC_INT_TX_DONE) == EMAC_INT_TX_DONE)
    {
        usedTxDescr--;
        isTxDone = 1;
    }

    LPC_EMAC->IntClear = 0xff;
}

static void fnet_lpceth_interrupt_handler_bottom(void *cookie)
{
	fnet_printf("Last IntStatus: %ld\n", lastInterruptStatus);

    fnet_isr_lock();

    /* Ignore duplicate interrupts from transmit done*/
    if (!isTxDone)
    {
        fnet_lpceth_input(0x0);
    }
    else
    {
        isTxDone = 0;
    }

    fnet_isr_unlock();
}

void fnet_lpceth_input(fnet_netif_t *netif)
{
    while (recvPacketsWaiting > 0)
    {
#if LPC_DEBUG_LEDS
        led1_on();
#endif
        fnet_netbuf_t *nb;
        fnet_eth_header_t *ethheader;

        RX_Desc *rxDescriptor = (RX_Desc *) (LPC_EMAC->RxDescriptor + (LPC_EMAC->RxConsumeIndex * sizeof(RX_Desc)));
        RX_Stat *rxStatus = (RX_Stat *) (LPC_EMAC->RxStatus + (LPC_EMAC->RxConsumeIndex * sizeof(RX_Stat)));
        if (rxStatus->Info != 0)
        {
            fnet_uint16_t pktSize = (rxStatus->Info & EMAC_RINFO_SIZE) - 1;
            fnet_uint32_t isOverrun = (rxStatus->Info & (1 << 28));

            if (isOverrun != 0)
            {
                fnet_printf("Overrun\n");
            }
            /* Note - the ethernet header type will be in big endian, but this has already been accounted for elsewhere in FNET.
             * (do not do a endian conversion here)
             */
            ethheader = (fnet_eth_header_t *) (rxDescriptor->Packet);
            fnet_eth_trace("\nRX", ethheader);

            void *layer3Ptr = rxDescriptor->Packet + sizeof (fnet_eth_header_t);
            fnet_uint16_t sizeOfLayer3 = pktSize - sizeof (fnet_eth_header_t);

            nb = fnet_netbuf_from_buf(layer3Ptr, sizeOfLayer3, FNET_TRUE);
            if (nb != 0)
            {
                fnet_eth_prot_input(&fnet_eth0_if, nb, ethheader->type);
                recvPackets++;
            }
            else
            {
                fnet_printf("Could not allocate netbuf for RX packet\n");
            }

            // Reset the status and descriptor
            fnet_memset_zero(rxDescriptor->Packet, pktSize);

            rxStatus->Info = 0;
            rxStatus->HashCRC = 0;
        }

        fnet_uint8_t nextDescriptor = LPC_EMAC->RxConsumeIndex + 1;
        if (nextDescriptor > LPC_EMAC->RxDescriptorNumber)
        {
            LPC_EMAC->RxConsumeIndex = 0;
        }
        else
        {
            LPC_EMAC->RxConsumeIndex = nextDescriptor;
        }
        recvPacketsWaiting--;
#if LPC_DEBUG_LEDS
        led1_off();
#endif
    }
}

void fnet_lpceth_output(fnet_netif_t *netif, unsigned short type, const fnet_mac_addr_t dest_addr, fnet_netbuf_t* nb)
{
#if LPC_DEBUG_LEDS
	led2_on();
#endif
	fnet_uint8_t currentIndex = LPC_EMAC->TxProduceIndex;
	fnet_uint8_t nextIndex = currentIndex+1;

	if (nb->total_length == 0)
	{
		return;
	}
	/* Unlike the other FNET targets, we don't have enough space
	 * to hold frames until TX fragments are available, so discard them.
	 * Perhaps we can do a more intelligent method, i.e let certain
	 * types (ARP) through always?
	 */
	if (usedTxDescr == EMAC_NUM_TX_FRAG)
	{
		fnet_printf("Out of TX descriptors!");
		fnet_netbuf_free_chain(nb);
		return; // memory full
	}
	//led2_on();
	if (nextIndex > LPC_EMAC->TxDescriptorNumber)
	{
		nextIndex = 0; // loop around once we reach the maximum number of descriptors
	}

	if (txDescriptorStatus[currentIndex])
	{
		fnet_println("TX Descriptor %d busy",currentIndex);
        fnet_netbuf_free_chain(nb);
        return;
	}
	txDescriptorStatus[currentIndex] = 1;

	fnet_printf("tx: %d %d\n", LPC_EMAC->TxProduceIndex, LPC_EMAC->TxConsumeIndex);
	TX_Desc *descriptor = (TX_Desc *)(LPC_EMAC->TxDescriptor + (currentIndex * sizeof(TX_Desc)));
//	TX_Stat *status = (TX_Stat *)(LPC_EMAC->TxStatus + (currentIndex * sizeof(TX_Stat)));

	fnet_memset(descriptor->Packet, 0, EMAC_ETH_MAX_FLEN);

	/* Construct the ethernet header directly onto the TX packet buffer */
	fnet_eth_header_t *ethHeader = (fnet_eth_header_t *)descriptor->Packet;
	fnet_memcpy (ethHeader->destination_addr, dest_addr, sizeof(fnet_mac_addr_t));
	fnet_lpceth_get_hw_addr(netif, ethHeader->source_addr);

	ethHeader->type=fnet_htons(type);

	void *packetData = descriptor->Packet + FNET_ETH_HDR_SIZE;

	fnet_netbuf_to_buf(nb, 0, FNET_NETBUF_COPYALL, packetData);

	fnet_uint16_t frameSize = nb->total_length + FNET_ETH_HDR_SIZE;

	descriptor->Ctrl = (frameSize-1) | (1<<30) | (1<<31);

	LPC_EMAC->TxProduceIndex = nextIndex;

	usedTxDescr++;
	sentPackets++;

	fnet_netbuf_free_chain(nb);
	txDescriptorStatus[currentIndex] = 0;
#if LPC_DEBUG_LEDS
	led2_off();
#endif
}

void fnet_lpceth_release(fnet_netif_t *netif) { }

#if FNET_CFG_MULTICAST

/** Calculate a CRC32 hash of a MAC addressed - used for multicast and the imperfect hash filter
 * Copied from fnet_fec_crc_hash
 * */
static fnet_uint32_t fnet_lpceth_crc_hash(fnet_mac_addr_t multicast_addr)
{
	fnet_uint32_t crc = 0xFFFFFFFFL;
	int i;
	int j;

	for (i=0; i<6; i++)
	{
		uint8_t c = multicast_addr[i];
		for (j=0; j<8; j++)
		{
			if ((c ^ crc) & 1)
			{
				crc >>= 1;
				c >>= 1;
				crc ^= 0xEDB88320L;
			}
			else
			{
				crc >>= 1;
				c >>= 1;
			}
		}
	}
   return(crc);
}

/************************************************************************
* NAME: fnet_lpceth_multicast_join
*
* DESCRIPTION: Joins a multicast group on FEC interface.
*************************************************************************/
void fnet_lpceth_multicast_join(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr)
{
	fnet_uint32_t reg_value;
	fnet_uint32_t crc;

	/* Set the appropriate bit in the hash table */
	crc = fnet_lpceth_crc_hash(multicast_addr);
	crc = (crc >> 23);
	crc &= 0x3F;

	reg_value = 0x1 << (crc);

	if (crc < 32)
	{
		LPC_EMAC->HashFilterL |= reg_value;
	}
	else
	{
		LPC_EMAC-> HashFilterH |= reg_value;
	}
}

/************************************************************************
* NAME: fnet_fec_multicast_leave
*
* DESCRIPTION: Leavess a multicast group on FEC interface.
*************************************************************************/
void fnet_lpceth_multicast_leave(fnet_netif_t *netif, fnet_mac_addr_t multicast_addr)
{
	fnet_uint32_t reg_value;
	fnet_uint32_t crc;

    /* Set the appropriate bit in the hash table */
    crc = fnet_lpceth_crc_hash(multicast_addr );
    crc = crc >> 23;
    crc &= 0x3F;

    reg_value = 0x1 << (crc);

    if (crc < 32)
    {
        LPC_EMAC->HashFilterL &= ~reg_value;
    }
    else
    {
        LPC_EMAC->HashFilterH &= ~reg_value;
    }
}
#endif

/******************************************************************************	
 *                                    EOF                                     *
 *****************************************************************************/
#endif
