/*
===============================================================================
 Name        : FNETPort.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>

// TODO: insert other include files here
#include "fnet.h"
#include "cpu/fnet_cpu.h"
#include "services/dhcp/fnet_dhcp.h"
#include "fnet_user_config.h"
#include "stack/fnet_timer_prv.h"
#include "stack/fnet_arp.h"
#include "stack/fnet_mempool.h"

int main(void)
{
	SystemInit();

	SystemCoreClockUpdate();

    /* Init UART */
    fnet_cpu_serial_init(0, 115200);

    /* Init UART */
	fnet_println("\nTesting Serial... 1. 2. 3.");

	if (fnet_init_static() != FNET_ERR)
	{
		fnet_println("FNET INIT Done.");
	}
	else
	{
		fnet_println("FNET INIT Failed.");
	}
#if FNET_CFG_DHCP
	fnet_netif_desc_t netif;
	struct fnet_dhcp_params dhcp_params;

	// Get current net interface.
	if ((netif = fnet_netif_get_default()) == 0)
	{
		fnet_println("ERROR: Network Interface isn't configured!");
	}
	else
	{
		if (fnet_arp_init(netif) != FNET_ERR)
		{
			fnet_println("ARP Init Success.");
		}
		else
		{
			fnet_println("ARP Init Fail\n");
		}

		fnet_memset_zero(&dhcp_params, sizeof(struct fnet_dhcp_params));

		fnet_printf("DHCP Init:");
		// Enable DHCP client.
		if (fnet_dhcp_init(netif, &dhcp_params) != FNET_ERR)
		{
			fnet_println(" success");
		}
		else
		{
			fnet_println(" fail!");
		}
	}
#endif

	volatile static int x;
	while(1)
	{
		fnet_poll_services();
		x++;
	}

    return 0;
}
