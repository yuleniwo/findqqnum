#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tb_md5.h"
#include "tb_thread.h"
#include "tb_atomic.h"
#include "tb_util.h"
#include "tb_global.h"

#define MAX_THREADS	64
#define DEF_BEG_QQ	100000000
#define DEF_END_QQ	2999999999
#define __STR__(num) #num
#define _QQ2STR_(num) __STR__(num)

static tb_uint8 hash[16];
static tb_int32 hash_len = 0;
static atomic_u32_t beg = ATOMIC_INIT(DEF_BEG_QQ);
static volatile tb_int32 found = 0;
static tb_uint32 end = DEF_END_QQ;

static void usage(const char* prog_name)
{
	printf("Usage: %s [-b <begin qq num, default " _QQ2STR_(DEF_BEG_QQ) ">] "
			"[-e <end qq num, default " _QQ2STR_(DEF_END_QQ) ">] "
			"[-q <qq num, output qq num hash>] [-t <thread num, default 8>] [-h <hash>]\n",
			prog_name);
	printf("Ex: %s -h DEA8D90E3170BCBEACE4786B4CB21AF7\n", prog_name);
	printf("    %s -q 529698939\n", prog_name);
	printf("    %s -h DEA8D90E3170BCBEACE4786B4CB21AF7 -b 529600000 -e 529699999\n", prog_name);
}

static tb_int32 uint2str(tb_uint32 num, char* str)
{
	static const tb_uint32 tab[] = {1000000000, 100000000, 10000000, 
		1000000, 100000, 10000, 1000, 100, 10, 1};
	tb_uint32 n;
	tb_int32 i, j = 0;
	if(num != 0)
	{
		for(i=0; i<10; i++)
		{
			n = num / tab[i];
			if(j != 0 || n != 0)
			{
				str[j++] = '0' + n;
				num -= tab[i] * n;
			}
		}
	}
	else
		str[j++] = '0';
	str[j] = '\0';
	return j;
}

static tb_int32 calc_hash(tb_uint32 qq)
{
	char str_qq[32+16];
	tb_uint8 h[16];
	int len;
	//len = sprintf(str_qq + 32, "%u", qq);
	len = uint2str(qq, str_qq + 32);
	tb_md5(str_qq + 32, len, h);
	tb_util_bin2hex(h, 16, str_qq, tb_false);
	tb_md5(str_qq, 32 + len, h);
	tb_util_bin2hex(h, 16, str_qq, tb_false);
	tb_md5(str_qq, 32 + len, h);
	return memcmp(h, hash, hash_len) == 0 ? 0 : 1;
}

static void test_qq_hash(tb_uint32 qq)
{
	char str_qq[32+16];
	tb_uint8 h[16];
	int len;
	//len = sprintf(str_qq + 32, "%u", qq);
	len = uint2str(qq, str_qq + 32);
	tb_md5(str_qq + 32, len, h);
	tb_util_bin2hex(h, 16, str_qq, tb_false);
	tb_md5(str_qq, 32 + len, h);
	tb_util_bin2hex(h, 16, str_qq, tb_false);
	tb_md5(str_qq, 32 + len, h);
	tb_util_bin2hex(h, 16, str_qq, tb_true);
	printf("QQ:[%u] --> hash:[%s]\n", qq, str_qq);
}

void* TB_API thread_proc(void* param)
{
	tb_uint32 qq = 0;
	while(!found)
	{
		qq = atomic_fetch_add_u32(&beg, 1, ATOMIC_ACQ_REL);
		if(qq <= end && qq != 0)
		{
			if(calc_hash(qq) == 0)
			{
				tb_datetime_t dt;
				found = 1;
				tb_util_get_localtime(&dt);
				printf("[%04d-%02d-%02d %02d:%02d:%02d.%03d] Found QQ: %u\n", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.msecond, qq);
				break;
			}
		}
		else break;
	}
	return NULL;
}

int main(int argc, char* argv[])
{
	
	tb_uint32 test_qq = 529698939;
	tb_int32 i, len, thread_num = 8;
	tb_thread_t threads[MAX_THREADS];
	tb_datetime_t dt;
	usage(argv[0]);
	for(i=1; i<argc; )
	{
		if('\0' == argv[i][0])
		{
			i++;
			continue;
		}
		switch(argv[i][1])
		{
		case 'h':
			len = (tb_int32)strlen(argv[i+1]);
			if(len >= 2 && len <= 32 && (len & 0x01) == 0)
			{
				tb_util_hex2bin(argv[i+1], len, hash);
				hash_len = len >> 1;
			}
			i += 2;
			break;
		
		case 'b':
			//beg = atoi(argv[i+1]);
			atomic_store_u32(&beg, atoi(argv[i+1]), ATOMIC_RELEASE);
			i += 2;
			break;
		
		case 'e':
			end = atoi(argv[i+1]);
			i += 2;
			break;
		
		case 'q':
			test_qq = atoi(argv[i+1]);
			i += 2;
			break;
		
		case 't':
			thread_num = atoi(argv[i+1]);
			if(thread_num <= 0)
				thread_num = 1;
			else if(thread_num > MAX_THREADS)
				thread_num = MAX_THREADS;
			i += 2;
			break;
		}
	}
	if(hash_len <= 0)
	{
		test_qq_hash(test_qq);
		return 0;
	}
	if(atomic_load_u32(&beg, ATOMIC_ACQUIRE) > end)
	{
		printf("End QQ number must be bigger than begin QQ number!!!\n");
		return 1;
	}
	tb_util_get_localtime(&dt);
	printf("[%04d-%02d-%02d %02d:%02d:%02d.%03d] Finding QQ ...\n", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.msecond);
	
	for(i=0; i<thread_num; i++)
	{
		tb_thread_init(&threads[i]);
		tb_thread_run1(&threads[i], thread_proc, NULL);
	}
	thread_proc(NULL);
	for(i=0; i<thread_num; i++)
	{
		tb_thread_join(&threads[i]);
		tb_thread_uninit(&threads[i]);
	}
	if(!found)
	{
		tb_util_get_localtime(&dt);
		printf("[%04d-%02d-%02d %02d:%02d:%02d.%03d] Not found.\n", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, dt.msecond);
	}
	return 0;
}