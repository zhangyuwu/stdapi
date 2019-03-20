#ifndef __LWIP_API_DHCP_H__
#define __LWIP_API_DHCP_H__

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/netif.h"
#include "lwip/err.h"

#if defined(LWIP_DHCP) && defined(DHCP_USE_API)
err_t api_dhcp_start(struct netif *netif);
#endif

#endif /* __LWIP_API_DHCP_H__ */
