/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
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
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define NO_SYS 					0

#ifdef LWIP_DEBUG
#define TCP_DEBUG				DBG_ON
#define TCP_INPUT_DEBUG			DBG_ON
#define TCP_FR_DEBUG			DBG_ON
#define TCP_RTO_DEBUG			DBG_ON
#define TCP_REXMIT_DEBUG		DBG_ON
#define TCP_CWND_DEBUG			DBG_ON
#define TCP_WND_DEBUG			DBG_ON
#define TCP_OUTPUT_DEBUG		DBG_ON
#define TCP_RST_DEBUG			DBG_ON
#define TCP_QLEN_DEBUG			DBG_ON
#define TCPIP_DEBUG				DBG_ON
#endif

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
/* 16-bit controller requires 2-byte alignment, 32-bit controller requires 4-byte alignment */
#define MEM_ALIGNMENT           4

/* MEM_SIZE: the size of the heap memory. If the application will send
   a lot of data that needs to be copied, this should be set high. */
/* TCP/IP Heap size */
/* The size of the memory pool for copying buffers into temporary locations,
   which is not the total memory size. */
#define MEM_SIZE                (1024 * 64)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
   sends a lot of data out of ROM (or other static memory), this
   should be set high. */
/* Means [N(value)] concurrent pbuf ROM buffers are available. This value should be 
   larger if most data is sent from ROM (i.g. static web pages). */
#define MEMP_NUM_PBUF           32

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        6

/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB        10

/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
   connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 8

/* The following four are used only with the sequential API and can be
   set to 0 if the application only will use the raw API. */
/* MEMP_NUM_NETBUF: the number of struct netbufs. */
/* Maximum number of packet buffers passed between the application and stack threads */
/* The maximum number of buffers that can be passed between the
   application thread and the protocol stack thread (in either direction) at any one time. */
#define MEMP_NUM_NETBUF         16

/* MEMP_NUM_NETCONN: the number of struct netconns. */
#define MEMP_NUM_NETCONN        16

/* MEMP_NUM_APIMSG: the number of struct api_msg, used for
   communication between the TCP/IP stack and the sequential
   programs. */
/* Maximum number of pending API calls from the application to the stack thread */
/* The size of the message box that sends API calls from the application
   thread to the protocol thread. This option maps onto the lwIP */
#define MEMP_NUM_API_MSG        32

/* MEMP_NUM_TCPIPMSG: the number of struct tcpip_msg, which is used
   for sequential API communication and incoming packets. Used in
   src/api/tcpip.c. */
/* Maximum number of messages passed from the protocol stack thread to the application */
/* The combination of API calls passed from the application thread to the stack
   thread, and packets being passed the other way. */
#define MEMP_NUM_TCPIP_MSG      128

/* MEMP_NUM_SYS_TIMEOUT: the number of simultaneously active
   timeouts. */
#define MEMP_NUM_SYS_TIMEOUT    6

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: Maximum packet buffer size, the number of buffers in the pbuf pool. */
/* Means [N(value)] concurrent pbuf RAM pool packet buffers can be held in memory. 
  It is desirable to have a lot of these buffers to be able to queue more incoming packets 
  if they arrive at a short time interval. But since each pbuf requires approximately 
  PBUF_POOL_BUFSIZE bytes of RAM, such a pbuf type is very costly. */
#define PBUF_POOL_SIZE          256

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
/* This defines the size of each pbuf pool payload. Some bytes are required for 
  the pbuf structure and the Ethernet frame header. In this case, the effective 
  IPSEC MTU will be around 420 bytes per packet. All pbuf pool buffers must fit 
  within MEM_SIZE, so there is room to scope left to favor more, but smaller or less, 
  but larger network packets. */
#define PBUF_POOL_BUFSIZE       128

/* PBUF_LINK_HLEN: the number of bytes that should be allocated for a
   link level header. */
#define PBUF_LINK_HLEN          16

/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

/* Controls if TCP should queue segments that arrive out of
   order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ         0

/* TCP Maximum segment size. */
#define TCP_MSS                 1024

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             (1024 * 2)

/* TCP sender buffer space (pbufs). This must be at least = 2 *
   TCP_SND_BUF/TCP_MSS for things to work. */
/* The TCP send queue can rapidly grow if lwIP does not receive the ACK packets. 
   If the queue is full, the user application may fails. In this case, 
   [N(value)] TCP packets can be simultaneously ¡°on the way¡±. */
#define TCP_SND_QUEUELEN        (4 * TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                 (1024 * 8)

/* Maximum number of retransmissions of data segments. */
#define TCP_MAXRTX              12

/* Maximum number of retransmissions of SYN segments. */
#define TCP_SYNMAXRTX           4

/* ---------- ARP options ---------- */
#define ARP_TABLE_SIZE			10
#define ARP_QUEUEING			0

/* ---------- IP options ---------- */
/* Define IP_FORWARD to 1 if you wish to have the ability to forward
   IP packets across network interfaces. If you are going to run lwIP
   on a device with only one network interface, define this to 0. */
#define IP_FORWARD              0

/* If defined to 1, IP options are allowed (but ignored). If
   defined to 0, all packets with IP options are dropped. */
#define IP_OPTIONS              1

/* IP reassembly and segmentation.These are orthogonal even
 * if they both deal with IP fragments */
#define IP_REASSEMBLY			1
#define IP_FRAG					1

/* ---------- ICMP options ---------- */
#define ICMP_TTL                255


/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
   interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
   turning this on does currently not work. */
#define LWIP_DHCP               0

#define DHCP_USE_API			0

/* 1 if you want to do an ARP check on the offered address
   (recommended). */
#define DHCP_DOES_ARP_CHECK     0

/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define UDP_TTL                 255

/*
 * Enable LightWeight Protection.
 * Refer \altera\kits\nios2\components\altera_lwip\UCOSII\src\downloads\lwip-1.1.0\src\include\lwip\sys.h
 * for changes to the SYS_ARCH_DECL_PROTECT(), SYS_ARCH_PROTECT() and
 * SYS_ARCH_UNPROTECT() macros.
 */
#define SYS_LIGHTWEIGHT_PROT	1

/* ---------- Statistics options ---------- */
/*#define STATS*/

#ifdef STATS
#define LINK_STATS
#define IP_STATS
#define ICMP_STATS
#define UDP_STATS
#define TCP_STATS
#define MEM_STATS
#define MEMP_STATS
#define PBUF_STATS
#define SYS_STATS
#endif /* STATS */

#endif /* __LWIPOPTS_H__ */
