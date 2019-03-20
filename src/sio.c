
/* Serial operations for SLIP */

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/netif.h"

static int len;

void
sio_send(char c,void * dev)
{
	len = 1;
	// cyg_io_write(*(cyg_io_handle_t*)dev, &c, &len);
}

char
sio_recv(void * dev)
{
	char c;
	len = 1;
	// cyg_io_read(*(cyg_io_handle_t *)dev, &c, &len);
	return c;			
}

int
sio_write(void *dev, char *b, int size)
{
	int len = size;
	// cyg_io_write(*(cyg_io_handle_t*)dev, b, &len);
	return len;
}
		
int
sio_read(void *dev, char *b, int size)
{
	int len = size;
	// cyg_io_read(*(cyg_io_handle_t*)dev, b, &len);
	
	return len;
}

void * 
sio_open(int devnum)
{
#if LWIP_SLIP
	#define SIODEV SLIP_DEV
#elif PPP_SUPPORT
	#define SIODEV PPP_DEV
#endif
}

void 
sio_read_abort(void * dev)
{
   // printf("Abort called\n");
}
