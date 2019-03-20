
#include <system.h>
#include <hardware.h>

void backlight(bool enable)
{
	outp16(BLIGHT_ADDR, enable ? 1 : 0);
}
