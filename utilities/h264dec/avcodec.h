#ifndef AVCODEC_H
#define AVCODEC_H

#include "common.h"


//the following defines might change, so dont expect compatibility if u use them
#define MB_TYPE_INTRA4x4   0x0001
#define MB_TYPE_INTRA16x16 0x0002 //FIXME h264 specific
#define MB_TYPE_INTRA_PCM  0x0004 //FIXME h264 specific
#define MB_TYPE_16x16      0x0008
#define MB_TYPE_16x8       0x0010
#define MB_TYPE_8x16       0x0020
#define MB_TYPE_8x8        0x0040
#define MB_TYPE_INTERLACED 0x0080
#define MB_TYPE_DIRECT2     0x0100 //FIXME
#define MB_TYPE_ACPRED     0x0200
#define MB_TYPE_GMC        0x0400
#define MB_TYPE_SKIP       0x0800
#define MB_TYPE_P0L0       0x1000
#define MB_TYPE_P1L0       0x2000
#define MB_TYPE_P0L1       0x4000
#define MB_TYPE_P1L1       0x8000
#define MB_TYPE_L0         (MB_TYPE_P0L0 | MB_TYPE_P1L0)
#define MB_TYPE_L1         (MB_TYPE_P0L1 | MB_TYPE_P1L1)
#define MB_TYPE_L0L1       (MB_TYPE_L0   | MB_TYPE_L1)
#define MB_TYPE_QUANT      0x00010000
#define MB_TYPE_CBP        0x00020000
//Note bits 24-31 are reserved for codec specific use (h264 ref0, mpeg1 0mv, ...)

#define Ylinesize       192
#define UVlinesize      96


typedef struct AVFrame 
{
    uint8_t *data[4];
    int reference;
    int8_t *qscale_table;
    uint32_t *mb_type;
}AVFrame;

typedef struct AVCodecContext
{
    int width, height;
    void *priv_data;
    int internal_buffer_count;
}AVCodecContext;

typedef struct Publicbuf
{
	uint8_t  Ybuf[2][192*152];//(160+32)*(120+32)
	uint8_t  Cbbuf[2][96*76];//(80+16)*(60+16)
	uint8_t  Crbuf[2][96*76];//(80+16)*(60+16)
	int8_t   TB_QSCALE[88];
	uint32_t MB_TYBASE[100];//100*(u32)
	int16_t  MotionVal[2628];//2628*(u16)
	int8_t   IndexRef[336];
    uint8_t	 RbspBuf[512];//4514
}Publicbuf;


int decode_init(AVCodecContext *avctx);
int decode_frame(AVCodecContext *avctx, void *data, int *data_size,uint8_t *buf, int buf_size);
int decode_end(AVCodecContext *avctx);

int avcodec_open(AVCodecContext *avctx);
int avcodec_close(AVCodecContext *avctx);

/* memory */
void *av_malloc(unsigned int size);
void *av_mallocz(unsigned int size);
void *av_realloc(void *ptr, unsigned int size);
void av_free(void *ptr);
char *av_strdup(const char *s);
void av_freep(void *ptr);
void *av_fast_realloc(void *ptr, int *size, int min_size);

#endif
