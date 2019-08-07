#ifndef _TB_DEFS_H_
#define _TB_DEFS_H_

typedef signed char tb_int8;
typedef unsigned char tb_uint8;
typedef tb_uint8 tb_byte;
typedef short tb_int16;
typedef unsigned short tb_uint16;
typedef int tb_int32;
typedef unsigned int tb_uint32;

#ifndef __cplusplus
	typedef tb_int8 tb_bool;
	#define tb_true ((tb_bool)1)
	#define tb_false ((tb_bool)0)
	#ifdef _MSC_VER
		#define inline _inline
	#endif
#else
	typedef bool tb_bool;
	#define tb_true true
	#define tb_false false
#endif

#ifdef _WIN32
	#ifdef _MSC_VER
		#define TB_API	__stdcall
		#define TB_MEMALIGNED(n) __declspec(align(n))
		typedef __int64 tb_int64;
		typedef unsigned __int64 tb_uint64;
	#else
		#if __SIZEOF_POINTER__ == 4
			#define TB_API __attribute__((stdcall))
		#elif __SIZEOF_POINTER__ == 8
			#define TB_API
		#endif
		#define TB_MEMALIGNED(n) __attribute__((aligned(n)))
		typedef long long tb_int64;
		typedef unsigned long long tb_uint64;
	#endif
	#ifdef _WIN64
		#define IS_32BIT 0
		#define IS_64BIT 1
		typedef tb_uint64 tb_uaddr;
	#else
		#define IS_32BIT 1
		#define IS_64BIT 0
		typedef tb_uint32 tb_uaddr;
	#endif
	#define atoll _atoi64
	#define strcasecmp stricmp
	#define strncasecmp strnicmp
	#define strtok_r strtok_s
	#define LLFMTSTR "I64"
#else
	typedef long long tb_int64;
	typedef unsigned long long tb_uint64;
	#if __SIZEOF_POINTER__ == 4
		#define IS_32BIT 1
		#define IS_64BIT 0
		typedef tb_uint32 tb_uaddr;
	#elif __SIZEOF_POINTER__ == 8
		#define IS_32BIT 0
		#define IS_64BIT 1
		typedef tb_uint64 tb_uaddr;
	#endif
	#ifdef __i386__
		#define TB_API __attribute__((stdcall))
	#else
		#define TB_API
	#endif
	#define LLFMTSTR "ll"
	#define TB_MEMALIGNED(n) __attribute__((aligned(n)))
#endif

#define TB_ALIGN(size, align) (((size) + ((align) - 1)) & (~((align) - 1)))
#define TB_OFFSETOF(type, member) ((size_t)&((type *)0)->member)
#define TB_CONTAINER_OF(ptr, type, member) ((type *) ((char *) (ptr) - TB_OFFSETOF(type, member)))

typedef tb_uint32 tb_sockid;

typedef struct
{
	tb_int32 year;
	tb_int8  month;		//从1开始，1~12
	tb_int8  day;		//从1开始
	tb_int8  wday;		//星期日0，星期一1，星期二2，...，星期六6。其他值无效忽略。
	tb_int8  hour;		//0~23
	tb_int8  minute;	//0~59
	tb_int8  second;	//0~59
	tb_int16 msecond;	//0~999
} tb_datetime_t;

#endif //_TB_DEFS_H_
