
#include <C_m40800.h>
#include <system.h>
#include <ds1302.h>
#include <hardware.h>

// ----------------------------------------------------------------------------
typedef struct {
	TDate	dateR;
	TTime	timeR;
	TDate	dateW;
	TTime	timeW;
	bool	date_need_set;
	bool	time_need_set;
} TDs1302TimerData;

TDs1302TimerData ds1302_timer_data;

// ----------------------------------------------------------------------------
byte ds1302_read(byte cmd)
{
	byte data = 0;
	int i;
	
	outp16(DS1302_ADDR, DS1302_CSL_CLKL_DATAL);
	outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
	udelay(4);
	
	for (i = 0; i < 8; i++) {
		if ((cmd & (1 << i)) == 0) {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAL);
			udelay(1);			
		}
		else {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAH);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAH);
			udelay(1);			
		}
		
		if (i == 7) {
			outp16(DS1302_ADDR, 0x01);
			udelay(1);
		}
		else {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
		}
	}

	// Read DATA
	for (i = 0; i < 8; i++) {
		if ((inp16(DS1302_ADDR) & 0x01) == 0) {
			data &= ~(1<<i);
		}
		else {
			data |= (1<<i);
		}
		
		udelay(1);
		outp16(DS1302_ADDR, 0x03);
		udelay(1);
		outp16(DS1302_ADDR, 0x01);
		udelay(1);
	}
	
	outp16(DS1302_ADDR, DS1302_CSL_CLKL_DATAL);
	return data;
}

// ----------------------------------------------------------------------------
void ds1302_write(byte cmd, byte data)
{
	int i;
	
	outp16(DS1302_ADDR, DS1302_CSL_CLKL_DATAL);
	outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
	udelay(4);
	
	for (i = 0; i < 8; i++) {
		if ((cmd & (1 << i)) == 0) {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAL);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
		}
		else {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAH);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAH);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAH);
			udelay(1);
		}
	}
	
	for (i = 0; i < 8; i++) {									// write 1 byte to DS1302
		if ((data & (1 << i)) == 0) {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAL);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAL);
			udelay(1);
		}
		else {
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAH);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKH_DATAH);
			udelay(1);
			outp16(DS1302_ADDR, DS1302_CSH_CLKL_DATAH);
			udelay(1);
		}
	}

	outp16(DS1302_ADDR, DS1302_CSL_CLKL_DATAL);					// CS = 0
	udelay(4);
}

// ----------------------------------------------------------------------------
byte ds1302_bcd2int(byte bcd)
{
	byte ret;
	ret = ((bcd & 0xf0) >> 4) * 10 + (bcd & 0x0f);
	return ret;
}

// ----------------------------------------------------------------------------
byte ds1302_int2bcd(byte i)
{
	byte ret;
	i %= 100;
	ret = (((i / 10) << 4) | (i % 10));
	return ret;
}

// ----------------------------------------------------------------------------
TDate * ds1302_get_date(TDate *date)
{
	date->day = ds1302_bcd2int(ds1302_read(0x87));
	date->month = ds1302_bcd2int(ds1302_read(0x89));
	date->week = ds1302_bcd2int(ds1302_read(0x8B));
	date->year = ds1302_bcd2int(ds1302_read(0x8D)) + 2000;
	
	return date;
}

// ----------------------------------------------------------------------------
TTime * ds1302_get_time(TTime *time)
{
	time->second = ds1302_bcd2int(ds1302_read(0x81));
	if (time->second == 0) udelay(10);
	time->minute = ds1302_bcd2int(ds1302_read(0x83));
	if (time->minute == 0) udelay(10);
	time->hour = ds1302_bcd2int(ds1302_read(0x85));
	
	return time;
}

// ----------------------------------------------------------------------------
bool ds1302_set_date(const TDate *date)
{	
	if (date->month >= 1 && date->month <= 12 && date->day >= 1 && date->day <= 31) {
		ds1302_write(0x8E, 0x00);			// Disable write protect
		ds1302_write(0x90, 0x00);			// Disable the trickle charger
		ds1302_write(0x8C, ds1302_int2bcd(date->year % 100));
		ds1302_write(0x88, ds1302_int2bcd(date->month));
		ds1302_write(0x86, ds1302_int2bcd(date->day));
		ds1302_write(0x8A, ds1302_int2bcd(date->week));
		ds1302_write(0x8E, 0x80);			// Enable write protect
		
		return true;
	}

	return false;
}

// ----------------------------------------------------------------------------
bool ds1302_set_time(const TTime *time)
{
	if (time->hour <= 23 && time->minute <= 59 && time->second <= 59) {
		ds1302_write(0x8E, 0x00);			// Disable write protect
		ds1302_write(0x90, 0x00);			// Disable the trickle charger
		ds1302_write(0x84, ds1302_int2bcd(time->hour));
		ds1302_write(0x82, ds1302_int2bcd(time->minute));
		ds1302_write(0x80, ds1302_int2bcd(time->second));
		ds1302_write(0x8E, 0x80);			// Enable write protect
		
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void ds1302_init()
{
	//outp16(DS1302_ADDR, 0x0);
	
	ds1302_timer_data.date_need_set = false;
	ds1302_timer_data.time_need_set = false;
	
	ds1302_get_date(&(ds1302_timer_data.dateR));
	ds1302_get_time(&(ds1302_timer_data.timeR));
}

// ----------------------------------------------------------------------------
void isr_ds1302()
{
	ds1302_get_date(&(ds1302_timer_data.dateR));
	ds1302_get_time(&(ds1302_timer_data.timeR));
	
	if (ds1302_timer_data.date_need_set) {
		if (ds1302_set_date(&(ds1302_timer_data.dateW))) {
			ds1302_timer_data.dateR = ds1302_timer_data.dateW;
		}
		ds1302_timer_data.date_need_set = false;
	}
	
	if (ds1302_timer_data.time_need_set) {
		if (ds1302_set_time(&(ds1302_timer_data.timeW))) {
			ds1302_timer_data.timeR = ds1302_timer_data.timeW;
		}
		ds1302_timer_data.time_need_set = false;
	}
}

/* ------------------------------------------------------------------------
 *  ��������
 *      TDate * GetCurrentDate(TDate *date)
 *  ���ܣ�
 *      ��ȡ��ǰ������
 *  ������
 *      date            ----    ������ڵ�ָ��
 *  ����ֵ��
 *      ���������ָ��(data)
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/
TDate * GetCurrentDate(TDate *date)
{
	*date = ds1302_timer_data.dateR;
	return date;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      TTime * GetCurrentTime(TTime *time)
 *  ���ܣ�
 *      ��ȡ��ǰ��ʱ��
 *  ������
 *      time            ----    ���ʱ���ָ��
 *  ����ֵ��
 *      �����ʱ��ָ��(time)
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/
TTime * GetCurrentTime(TTime *time)
{
	*time = ds1302_timer_data.timeR;
	return time;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      bool SetCurrentDate(const TDate *date)
 *  ���ܣ�
 *      ���õ�ǰ����
 *  ������
 *      date            ----    �����õ�����ָ��
 *  ����ֵ��
 *      ���óɹ�true��ʧ��false
 *  ˵����
 *      �����������ڵ�ֵ�����Ϲ淶����1��12����1��31�����򷵻ش���
 * -----------------------------------------------------------------------*/
bool SetCurrentDate(const TDate *date)
{
	if (date->month >= 1 && date->month <= 12 && date->day >= 1 && date->day <= 31) {
		ds1302_timer_data.dateW = *date;
		ds1302_timer_data.dateR = *date;
		ds1302_timer_data.date_need_set = true;
		return true;
	}
	return false;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      bool SetCurrentTime(const TTime *time)
 *  ���ܣ�
 *      ���õ�ǰʱ��
 *  ������
 *      time            ----    �����õ�ʱ��ָ��
 *  ����ֵ��
 *      ���óɹ�true��ʧ��false
 *  ˵����
 *      ��������ʱ���ֵ�����Ϲ淶��ʱ0��23����0��59����0��59�����򷵻ش���
 * -----------------------------------------------------------------------*/
bool SetCurrentTime(const TTime *time)
{
	if (time->hour <= 23 && time->minute <= 59 && time->second <= 59) {
		ds1302_timer_data.timeW = *time;
		ds1302_timer_data.timeR = *time;
		ds1302_timer_data.time_need_set = true;
		return true;
	}
	
	return false;
}
