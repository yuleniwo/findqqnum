#ifndef _TB_MD5_H_
#define _TB_MD5_H_

#include "tb_defs.h"

#define MD5_BLOCK_SIZE 16               // MD5 outputs a 16 tb_byte digest

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	tb_uint32 state[4];
	tb_uint32 count[2];
	tb_uint8 buffer[64];
} tb_md5_t;

void TB_API tb_md5_init(tb_md5_t *md5);
//tb_md5_update��MD5����������̣�buf��Ҫ�任���ֽڴ���len�ǳ���,����֮ǰ��Ҫ����MD5_Init
void TB_API tb_md5_update(tb_md5_t *md5, const void *buf, tb_uint32 buf_len);
/*--------------------------------------------------------------------------
 * ��������tb_md5_final
 * ���ܣ�MD5���������ȡ���ս��
 * ����1��digest��MD5������
 * ����2��md5��MD5�����м����
 * ���أ���
 *--------------------------------------------------------------------------*/
void TB_API tb_md5_final(tb_md5_t *md5, tb_uint8 digest[16]);

//buf������ buf_len�����ĵĳ���
void TB_API tb_md5(const void *buf, tb_uint32 buf_len, tb_uint8 out[16]);

#ifdef __cplusplus
}
#endif

#endif //_TB_MD5_H_
