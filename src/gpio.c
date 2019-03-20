
#include <system.h>
#include <gpio.h>

void gpio_toggle(int id)
{
	static bool on[32] = { false };
	
	if (id >= 0 && id < 32) {
		gpio_set(id, on[id]);
		on[id] = !on[id];
	}
}
