#ifndef _TB_UTIL_H_
#define _TB_UTIL_H_

#include <time.h>
#include "tb_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

//获取本地时间。
tb_datetime_t* TB_API tb_util_get_localtime(tb_datetime_t* dt);

///获取系统启动后到目前为止经历的毫秒数
tb_uint32 TB_API tb_util_get_ticks(void);

//16进制字符串转成数据。返回值：-1转换失败。>0转换成功。
tb_int32 TB_API tb_util_hex2bin(const char *hex, tb_int32 len, tb_byte *bin);

//数据转换成16进制字符串。add_null_char是否添加终止'\0'字符。
const char* TB_API tb_util_bin2hex(const tb_byte *bin, tb_int32 len, char *hex, tb_bool add_null_char);

#ifdef __cplusplus
}
#endif

#endif //_TB_UTIL_H_