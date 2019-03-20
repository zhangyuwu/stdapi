

#ifndef __GPIO_H__
#define __GPIO_H__

#include <C_m40800.h>

#define gpio_open(id)				(PIO_BASE->PIO_PER = (1 << id))
#define gpio_close(id)				(PIO_BASE->PIO_PDR = (1 << id))
#define gpio_output_enable(id)		(PIO_BASE->PIO_OER = (1 << id))
#define gpio_output_disalbe(id)		(PIO_BASE->PIO_ODR = (1 << id))
#define gpio_set(id, value)			(value ? (PIO_BASE->PIO_SODR = (1 << id)) : (PIO_BASE->PIO_CODR = (1 << id)))
#define gpio_get(id)				(PIO_BASE->PIO_PDSR & (1 << id))

void gpio_toggle(int id);

#endif
