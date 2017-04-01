#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"
#include "h264.h"
#include "malloc.h"	
#include <stdarg.h>


void av_free(void *ptr)
{
    if (ptr)
	{
		myfree(SRAMIN2,ptr);
	}
}

void av_freep(void *arg)
{
    void **ptr= (void**)arg;
    av_free(*ptr);
    *ptr = NULL;
}

int avcodec_open(AVCodecContext *avctx)
{
    int ret;

    avctx->priv_data=mymalloc(SRAMIN2,sizeof(H264Context));
    if (!avctx->priv_data)return -1;
    ret = decode_init(avctx);
    if (ret < 0)
	{
        av_freep(&avctx->priv_data);
        return ret;
    }
    return 0;
}

int avcodec_close(AVCodecContext *avctx)
{
    decode_end(avctx);
    av_freep(&avctx->priv_data);
    return 0;
}

