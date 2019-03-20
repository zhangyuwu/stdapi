
#include "lwip/opt.h"

#if defined(LWIP_DHCP) && defined(DHCP_USE_API)
#include "lwip/api.h"
#include "lwip/api_msg.h"
#include "lwip/api_dhcp.h"
#include "lwip/memp.h"

err_t
api_dhcp_start(struct netif *netif)
{
  struct netconn *conn;
  struct api_msg *msg;
  err_t err = ERR_OK;

  if (netif == NULL) {
    return ERR_ARG;
  }

  conn = memp_malloc(MEMP_NETCONN);
  if (conn == NULL) {
    return ERR_MEM;
  }

  conn->err = ERR_OK;
  if ((conn->mbox = sys_mbox_new()) == SYS_MBOX_NULL) {
    memp_free(MEMP_NETCONN, conn);
    return ERR_MEM;
  }

  if ((msg = memp_malloc(MEMP_API_MSG)) == NULL) {
    memp_free(MEMP_NETCONN, conn);
    return ERR_MEM;
  }
  msg->type = API_MSG_START_DHCP; /* Start DHCP */
  msg->msg.conn = conn;
  msg->msg.msg.w.dataptr = netif;

  api_msg_post(msg);  
  sys_mbox_fetch(conn->mbox, NULL);
  memp_free(MEMP_API_MSG, msg);

  err = conn->err;
  memp_free(MEMP_NETCONN, conn);
  return err;
}
#endif
