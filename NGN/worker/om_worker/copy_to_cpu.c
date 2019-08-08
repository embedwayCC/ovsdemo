//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <signal.h>
//#include "nngn.h"
#include "mpack.h"
//#include "mpack-config.h"
//#include "srv_mpack_common.h"

//#include "auth.h"
//#include "appdefs.h"
//#include "app.h"
//#include "string_buf.h"
//#include "global.h"
//#include "tna_exact_match.h"
//#include <getopt.h>
#include "copy_to_cpu.h"
#include <stdint.h>
#include <pkt_mgr/pkt_mgr_intf.h>
#include <syslog.h>

int modify_mac_on_cpu(int pkt_len, char *pkt_data)
{
    #if 0
    printf("pkt_len : %d\n",pkt_len);
    int i;
    printf("pkt_data : \n");
    for(i = 0; i < pkt_len; i++)
    {  
        printf(" %02x", (unsigned char)pkt_data[i]);
        if( (i + 1) % 16 == 0 )   
            printf("\n");  
    }
    printf("\n\n");
    #endif
    ETHHEADER *eth_header=(ETHHEADER*)pkt_data;
	#if 0
    printf("dst mac : \n");
    for(i = 0; i < 6; i++)
    {  
        printf(" %02x", eth_header->dst_mac[i]);
    }
    printf("\n\n");
	#endif
    eth_header->dst_mac[3] = 0x07;
    eth_header->dst_mac[4] = 0x08;
    eth_header->dst_mac[5] = 0x09;
    #if 0
    printf("dst mac after modify: \n");
    for(i = 0; i < 6; i++)
    {  
        printf(" %02x", eth_header->dst_mac[i]);
    }
    printf("\n\n");
    #endif

    return 0;
}

static bf_status_t cpu_tx_complete(bf_dev_id_t device, bf_pkt_tx_ring_t tx_ring, uint64_t tx_cookie, uint32_t status)
{
    /* free the packet */
    //printf("cpu_tx_complete\n");
    bf_pkt *pkt = (bf_pkt *)(uintptr_t)tx_cookie;
    bf_pkt_free(device, pkt);
    return 0;
}
int cpu_tx(bf_dev_id_t device, char *out_packet, int pkt_size)
{
    bf_pkt *pkt = NULL;
    bf_pkt_tx_ring_t tx_ring = BF_PKT_TX_RING_0;

    if (bf_pkt_alloc(device, &pkt, pkt_size, BF_DMA_CPU_PKT_TRANSMIT_0) != 0)
    {
        //printf("bf_pkt_alloc fail\n");
        syslog(LOG_DEBUG,"%s L#%d  bf_pkt_alloc fail\n", __func__, __LINE__);
        return -1;
    }

    /* copy the packet buffer and send it */
    if (bf_pkt_data_copy(pkt, (uint8_t *)out_packet, pkt_size) != 0)
    {
        //printf("bf_pkt_data_copy failed: pkt_size=%d\n", pkt_size);
        syslog(LOG_DEBUG,"%s L#%d  bf_pkt_data_copy failed: pkt_size=%d\n", __func__, __LINE__, pkt_size);
        bf_pkt_free(device, pkt);
        return -1;
    }

    if (bf_pkt_tx(device, pkt, tx_ring, (void *)pkt) != BF_SUCCESS)
    {
        bf_pkt_free(device, pkt);
    }

    return 0;
}

static bf_status_t cpu_pcie_rx(bf_dev_id_t device, bf_pkt *pkt, void *cookie, bf_pkt_rx_ring_t rx_ring)
{
    //printf("cpu_pcie_rx\n");
    bf_pkt *orig_pkt = NULL;
    static char in_packet[10000];
    char *pkt_buf = NULL;
    char *bufp = NULL;
    unsigned int packet_size = 0;
    int pkt_len = 0;
	int status = 0;
    int i;

    /* save a copy of the original packet */
    orig_pkt = pkt;

    /* assemble the received packet */
    bufp = &in_packet[0];
    do
    {
        pkt_buf = (char *)bf_pkt_get_pkt_data(pkt);
        pkt_len = bf_pkt_get_pkt_size(pkt);
        modify_mac_on_cpu(pkt_len, pkt_buf);
        syslog(LOG_DEBUG,"%s L#%d  modify_mac_on_cpu success\n", __func__, __LINE__);
        #if 0
        printf("pkt_data after modify mac: \n");
		for(i = 0; i < pkt_len; i++)
        {  
            printf(" %02x", (unsigned char)pkt_buf[i]);
            if( (i + 1) % 16 == 0 )   
                printf("\n");  
        }
        printf("\n\n");
        #endif
		cpu_tx(device, pkt_buf, pkt_len);
        syslog(LOG_DEBUG,"%s L#%d  cpu_tx success\n", __func__, __LINE__);
        /*if ((packet_size + pkt_len) > 10000)
        {
            printf("Packet too large to Transmit - SKipping\n");
            break;
        }
        memcpy(bufp, pkt_buf, pkt_len);
        bufp += pkt_len;
        packet_size += pkt_len;*/
        pkt = bf_pkt_get_nextseg(pkt);
    }while (pkt);

    /* free the packet */
    bf_pkt_free(device, orig_pkt);

    return 0;
}

int copy_to_cpu_callback_register(int device)
{
    int status = 0;

    bf_pkt_tx_ring_t tx_ring;
    bf_pkt_rx_ring_t rx_ring;

    /* register callback for TX complete */
    for (tx_ring = BF_PKT_TX_RING_0; tx_ring < BF_PKT_TX_RING_MAX; tx_ring++) {
        bf_pkt_tx_done_notif_register(device, cpu_tx_complete, tx_ring);
    }

    /* register callback for RX */
    for (rx_ring = BF_PKT_RX_RING_0; rx_ring < BF_PKT_RX_RING_MAX; rx_ring++) {
        bf_pkt_rx_register(device, cpu_pcie_rx, rx_ring, 0);
    }

    return status;
}

