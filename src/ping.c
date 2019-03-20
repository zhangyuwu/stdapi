
#include <system.h>
#include <lwip/sockets.h>
#include <lwip/tcpip.h>

void ping(struct ip_addr *ipaddr, int count)
{
	int sock, len, fromlen, timeo = 1000, tick, seq_num = 1, nonblock = true;
	struct sockaddr_in to, from;
	static char buffer[256];
	struct icmp_echo_hdr *iecho = (struct icmp_echo_hdr *)buffer;

	if ((sock = lwip_socket(AF_INET, SOCK_RAW, IP_PROTO_ICMP)) < 0) {
		kprintf("Ping Failed: create socket failed!\n");
		return;
	}

	lwip_ioctl(sock, FIONBIO, &nonblock);

	to.sin_len = sizeof(to);
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = ipaddr->addr;

	while (count > 0) {
		len = sizeof(struct icmp_echo_hdr) + 52;
		ICMPH_TYPE_SET(iecho,ICMP_ECHO);
		iecho->chksum = 0;
		iecho->seqno = htons(seq_num++);
		iecho->id = GetCurrentThreadId();
		iecho->chksum = inet_chksum(iecho, len);

		if (lwip_sendto(sock, iecho, len, 0, (struct sockaddr*)&to, sizeof(to)) > 0) {
			tick = GetTickCount();
		
			len = 0;
			while (len <= 0) {
				fromlen = sizeof(struct sockaddr_in);
				len = lwip_recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&from, &fromlen);
				if (len > 0) {
					kprintf("Reply from %s: bytes=%d, seq=%d, time=%dms\n", 
						inet_ntoa(from.sin_addr),
						len,
						ntohs(iecho->seqno),
						GetTickCount() - tick
					);					
					break;
				}
				else {
					sleep(10);
				}
				
				if (GetTickCount() - tick > timeo) {
					kprintf("Request timed out.\n");
					break;
				}
			}
		}

		sleep(1000);
		count--;
	}
}
