/*
 * ds3231.c
 *
 *  Created on: 7 maj 2018
 *      Author: pszymans
 */


#include "ds3231.h"
#include "../i2c/i2c.h"

static unsigned char bcd (unsigned char data)
{
	unsigned char bc;

	bc = ((((data & (1 << 6)) | (data & (1 << 5)) | (data & (1 << 4)))*0x0A) >> 4)
	+ ((data & (1 << 3))|(data & (1 << 2))|(data & (1 << 1))|(data & 0x01));

  return bc;
}

static unsigned char bin(unsigned char dec)
{
	char bcd;
	char n, dig, num, count;

	num = dec;
	count = 0;
	bcd = 0;

	for (n = 0; n < 4; n++) {
		dig = num % 10;
		num = num / 10;
		bcd = (dig << count) | bcd;
		count += 4;
	}
	return bcd;
}


void rtc3231_init(void)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x0E);
	TWI_write(0x20);
	TWI_write(0x08);
	TWI_stop();
}

void rtc3231_read_time(struct rtc_time *time)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x00);

	TWI_start();
	TWI_write(RTC_RADDR);
	time->sec = bcd(TWI_read(1));
	time->min = bcd(TWI_read(1));
	time->hour = bcd(TWI_read(0));
	TWI_stop();
}

void rtc3231_read_date(struct rtc_date *date)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x03);

	TWI_start();
	TWI_write(RTC_RADDR);
	date->wday = bcd(TWI_read(1));
	date->day = bcd(TWI_read(1));
	date->month = bcd(TWI_read(1));
	date->year = bcd(TWI_read(0));
	TWI_stop();
}

void rtc3231_read_datetime(struct rtc_time *time, struct rtc_date *date)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x00);

	TWI_start();
	TWI_write(RTC_RADDR);
	time->sec = bcd(TWI_read(1));
	time->min = bcd(TWI_read(1));
	time->hour = bcd(TWI_read(0));

	date->wday = bcd(TWI_read(1));
	date->day = bcd(TWI_read(1));
	date->month = bcd(TWI_read(1));
	date->year = bcd(TWI_read(0));
	TWI_stop();
}

void rtc3231_write_time(struct rtc_time *time)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x00);
	TWI_write(bin(time->sec));
	TWI_write(bin(time->min));
	TWI_write(bin(time->hour));
	TWI_stop();
}

void rtc3231_write_date(struct rtc_date *date)
{
	TWI_start();
	TWI_write(RTC_WADDR);
	TWI_write(0x03);
	TWI_write(bin(date->wday));
	TWI_write(bin(date->day));
	TWI_write(bin(date->month));
	TWI_write(bin(date->year));
	TWI_stop();
}
