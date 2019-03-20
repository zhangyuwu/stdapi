/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "netif/etharp.h"
#include "lwip/tcpip.h"
#include "lwip/sockets.h"
#include "netif/loopif.h"
#include <rtl8019.h>
#include <debug.h>

/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

struct ethernetif {
	struct eth_addr *ethaddr;
	/* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
static void  ethernetif_input(struct netif *netif);
static err_t ethernetif_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr);

sys_sem_t sem_recv;

#if (ETH_OS_WAIT >0)
sys_sem_t sem_eth;
#endif

void low_level_init(struct netif *netif)
{
	/* set MAC hardware address length */
	netif->hwaddr_len = 6;
	
	/* get MAC address */
	eth_get_mac_addr(netif->hwaddr);
	
	/* maximum transfer unit */
	netif->mtu = 1500;
	
	/* broadcast capability */
	netif->flags = NETIF_FLAG_BROADCAST;
	
	/* Do whatever else is needed to initialize interface. */  
	eth_init();
}

/*
 * low_level_output():
 *
 * Should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 */

err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	struct pbuf *q;
	static unsigned char buffer[1500];
	int len = 0;
  
 
#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE);			/* drop the padding word */
#endif

	memset(buffer, 0, sizeof(buffer));
	for(q = p; q != NULL; q = q->next) {
		/* Send the data from the pbuf to the interface, one pbuf at a
		   time. The size of the data in each pbuf is kept in the ->len
		   variable. */
		memcpy(buffer + len, q->payload, q->len);
		len += q->len;
	}
  
	if (len < 64)
		len = 64;
	
#if (ETH_OS_WAIT >0)
	sys_arch_sem_wait(sem_eth, 0);
#endif
	if (eth_send((unsigned short *)buffer, len) == 0) {
#if (ETH_OS_WAIT >0)
		sys_sem_signal(sem_eth);
#endif
		return ERR_RST;
	}
#if (ETH_OS_WAIT >0)
	sys_sem_signal(sem_eth);
#endif

#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE);			/* reclaim the padding word */
#endif
  
#if LINK_STATS
	lwip_stats.link.xmit++;
#endif /* LINK_STATS */      
	return ERR_OK;
}

/*
 * low_level_input():
 *
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 */

struct pbuf * low_level_input(struct netif *netif)
{
	struct pbuf *p, *q;
	static unsigned char buffer[1500];
	u16_t len;
	int offset = 0;
	
	/* Obtain the size of the packet and put it into the "len"
	 variable. */
	
#if (ETH_OS_WAIT >0)
	sys_arch_sem_wait(sem_eth, 0);
#endif

	len = eth_read((unsigned short *)buffer);
	if (len == 0) {
#if (ETH_OS_WAIT >0)
		sys_sem_signal(sem_eth);
#endif
		return NULL;
	}
	else if (!eth_empty()) {
		// have more data, so post receive task
		sys_sem_signal(sem_recv);
	}
#if (ETH_OS_WAIT >0)
	sys_sem_signal(sem_eth);
#endif

#if ETH_PAD_SIZE
	len += ETH_PAD_SIZE;						/* allow room for Ethernet padding */
#endif

	/* We allocate a pbuf chain of pbufs from the pool. */
	p = pbuf_alloc(PBUF_LINK, len, PBUF_POOL);
	
	if (p != NULL) {

#if ETH_PAD_SIZE
		pbuf_header(p, -ETH_PAD_SIZE);			/* drop the padding word */
#endif

		/* We iterate over the pbuf chain until we have read the entire
		 * packet into the pbuf. */
		for(q = p; q != NULL; q = q->next) {
			/* Read enough bytes to fill this pbuf in the chain. The
			 * available data in the pbuf is given by the q->len
			 * variable. */
			memcpy(q->payload, buffer+offset, q->len);
			offset += q->len;
		}
		// acknowledge that packet has been read();

#if ETH_PAD_SIZE
		pbuf_header(p, ETH_PAD_SIZE);			/* reclaim the padding word */
#endif

#if LINK_STATS
		lwip_stats.link.recv++;
#endif /* LINK_STATS */      
	} else {
    // drop packet();
#if LINK_STATS
		lwip_stats.link.memerr++;
		lwip_stats.link.drop++;
#endif /* LINK_STATS */      
	}

	return p;
}

/*
 * ethernetif_output():
 *
 * This function is called by the TCP/IP stack when an IP packet
 * should be sent. It calls the function called low_level_output() to
 * do the actual transmission of the packet.
 *
 */

err_t ethernetif_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr)
{
	err_t err;
	
	/* resolve hardware address, then send (or queue) packet */
	err = etharp_output(netif, ipaddr, p);
	
	return err;
}

/*
 * ethernetif_input():
 *
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface.
 *
 */

void ethernetif_input(struct netif *netif)
{
	struct ethernetif *ethernetif;
	struct eth_hdr *ethhdr;
	struct pbuf *p;
	
	ethernetif = netif->state;
	
	/* move received packet into a new pbuf */
	p = low_level_input(netif);
	/* no packet could be read, silently ignore this */
	if (p == NULL) {
		return;
	}
	/* points to packet payload, which starts with an Ethernet header */
	ethhdr = p->payload;
	
#if LINK_STATS
	lwip_stats.link.recv++;
#endif /* LINK_STATS */
    
	switch (htons(ethhdr->type)) {
	/* IP packet? */
	case ETHTYPE_IP:		
	#if 1
		// Add by Ken, 2005-6-14 12:12:49
		// is this packet for us? if not ignore it!
		if (memcmp(&(ethhdr->dest), netif->hwaddr, sizeof(struct eth_addr))) {
			LWIP_PLATFORM_DIAG(("IP packet: not for us!\n"));
			pbuf_free(p);
			p = NULL;
			break;
		}
	#endif

		/* update ARP table */
		etharp_ip_input(netif, p);

		/* skip Ethernet header */
		pbuf_header(p, -sizeof(struct eth_hdr));
		
		/* pass to network layer */
		netif->input(p, netif);
		break;
	
	case ETHTYPE_ARP:
		/* pass p to ARP module  */
		etharp_arp_input(netif, ethernetif->ethaddr, p);
		break;
		
	default:
		pbuf_free(p);
		p = NULL;
		break;
	}
}

/*
 * ethernetif_init():
 *
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 */

void eth_receive_task(void *arg)
{
	struct netif *netif = (struct netif *)arg;
	
	while (TRUE) {
		sys_arch_sem_wait(sem_recv, 0);
		if (netif) {
			ethernetif_input(netif);
		}
	}
}

void ethernetif_isr()
{
	sys_sem_signal(sem_recv);
}

err_t ethernetif_init(struct netif *netif)
{
	struct ethernetif *ethernetif;
	
	ethernetif = mem_malloc(sizeof(struct ethernetif));
	
	if (ethernetif == NULL)
	{
		LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
		return ERR_MEM;
	}
	
	netif->state = ethernetif;
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	netif->output = ethernetif_output;
	netif->linkoutput = low_level_output;
	
	ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
	
	low_level_init(netif);
		
	sem_recv = sys_sem_new(0);
#if (ETH_OS_WAIT >0)
	sem_eth = sys_sem_new(1);
#endif

	sys_thread_new(eth_receive_task, netif, DEFAULT_THREAD_PRIO);
		
	return ERR_OK;
}

void tcpip_init_done(void *arg)
{
    sys_sem_t *sem;
    sem = arg;
    sys_sem_signal(*sem);
}

void arp_timer(void *arg)
{
	etharp_tmr();
	sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

void tcp_timeout(void *data)
{
#if TCP_DEBUG
  tcp_debug_print_pcbs();
#endif /* TCP_DEBUG */
  sys_timeout(5000, tcp_timeout, NULL);
}

struct netif * net_startup(struct ip_addr *ipaddr, struct ip_addr *netmask, struct ip_addr *gw)
{
	static struct netif RTL8019_netif;
	static struct netif RTLLOOP_netif;
    sys_sem_t sem;
    struct ip_addr lo_ipaddr, lo_netmask, lo_gw;
	extern err_t ethernetif_init(struct netif *netif);

	// ��ʼ�� tcpip
    sem = sys_sem_new(0);
    tcpip_init(tcpip_init_done, &sem);
    sys_sem_wait(sem);
    sys_sem_free(sem);

	// ���� ip ��ַ
    IP4_ADDR(&lo_gw, 127, 0, 0, 1);
    IP4_ADDR(&lo_ipaddr, 127, 0, 0, 1);
    IP4_ADDR(&lo_netmask, 255, 0, 0, 0);

	// ����������ӿ�
    netif_add(&RTLLOOP_netif, &lo_ipaddr, &lo_netmask, &lo_gw, NULL, loopif_init, tcpip_input);
	netif_set_default(netif_add(&RTL8019_netif, ipaddr, netmask, gw, NULL, ethernetif_init, tcpip_input));
	netif_set_up(&RTL8019_netif);
	
	// ���ó�ʱ
	sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
	sys_timeout(5000, tcp_timeout, NULL);

	// �����ж�
	eth_irq_enable();
	
	return &RTL8019_netif;
}
