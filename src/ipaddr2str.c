
#include <stdio.h>
#include <lwip/ip_addr.h>
#include <lwip/inet.h>

const char *ipaddr2str(struct ip_addr *ipaddr)
{
	static char str[32];

	sprintf(str, "%u.%u.%u.%u",
		ip4_addr1(ipaddr),
		ip4_addr2(ipaddr),
		ip4_addr3(ipaddr),
		ip4_addr4(ipaddr)
	);
	
	return str;
}
