#include "tb_util.h"

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif

tb_datetime_t* TB_API tb_util_get_localtime(tb_datetime_t* dt)
{
#ifdef _WIN32
	SYSTEMTIME st;

	GetLocalTime(&st);
	dt->year = st.wYear;
	dt->month = (tb_int8)st.wMonth;
	dt->day = (tb_int8)st.wDay;
	dt->wday = (tb_int8)st.wDayOfWeek;
	dt->hour = (tb_int8)st.wHour;
	dt->minute = (tb_int8)st.wMinute;
	dt->second = (tb_int8)st.wSecond;
	dt->msecond = (tb_int16)st.wMilliseconds;
#else
	struct tm now;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	localtime_r((time_t *)&tv.tv_sec, &now);
	dt->year = (tb_int32)(now.tm_year + 1900);
	dt->month = (tb_int8)(now.tm_mon + 1);
	dt->day = (tb_int8)now.tm_mday;
	dt->wday = (tb_int8)now.tm_wday;
	dt->hour = (tb_int8)now.tm_hour;
	dt->minute = (tb_int8)now.tm_min;
	dt->second = (tb_int8)now.tm_sec;
	dt->msecond = (tb_int16)(tv.tv_usec / 1000);
#endif
	return dt;
}

tb_uint32 TB_API tb_util_get_ticks(void)
{
#ifdef _WIN32
	return GetTickCount();
#else
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

tb_int32 TB_API tb_util_hex2bin(const char *hex, tb_int32 len, tb_byte *bin)
{
	tb_byte bt;
	tb_int32 i;
	tb_uint32 dw = 0;
	if(len <= 0 || len%2)
		return -1;
	for(i=0; i<len; i++)
	{
		if(hex[i]>='0' && hex[i]<='9')
			bt = hex[i]-'0';
		else if(hex[i]>='A' && hex[i]<='F')
			bt = hex[i]-'A' + 10;
		else if(hex[i]>='a' && hex[i]<='f')
			bt = hex[i]-'a' + 10;
		else return -1;
		bt <<= 4;
		i++;
		if(hex[i]>='0' && hex[i]<='9')
			bt |= hex[i]-'0';
		else if(hex[i]>='A' && hex[i]<='F')
			bt |= hex[i]-'A' + 10;
		else if(hex[i]>='a' && hex[i]<='f')
			bt |= hex[i]-'a' + 10;
		else return -1;
		bin[i/2] = bt;
	}
	return len / 2;
}

const char* TB_API tb_util_bin2hex(const tb_byte *bin, tb_int32 len, char *hex, tb_bool add_null_char)
{
	tb_int32 i;
	const char *tab = "0123456789ABCDEF";
	hex[0] = '\0';
	for(i=0; i<len; i++)
	{
		hex[i*2  ] = tab[bin[i] >> 4];
		hex[i*2+1] = tab[bin[i] & 0x0F];
	}
	if(add_null_char && len > 0)
		hex[i*2] = '\0';
	return hex;
}
