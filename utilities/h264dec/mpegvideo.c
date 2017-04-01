#include "avcodec.h"
#include "dsputil.h"
#include "mpegvideo.h"
#include "malloc.h"	


int MPV_common_init(MpegEncContext *s)
{
    dsputil_init(&s->dsp, s->avctx);
    s->mb_width  = (s->width  + 15) / 16;
    s->mb_height = (s->height + 15) / 16;
    s->mb_stride = s->mb_width + 1;
    /* set default edge pos, will be overriden in decode_header if needed */
    s->h_edge_pos= s->mb_width*16;
    s->v_edge_pos= s->mb_height*16;
    s->context_initialized = 1;
	//初始化两个图像缓冲
	s->picture[0].data[0]=s->mybuf->Ybuf[0]+3088;
	s->picture[0].data[1]=s->mybuf->Cbbuf[0]+776;
	s->picture[0].data[2]=s->mybuf->Crbuf[0]+776;
	s->picture[0].qscale_table=s->mybuf->TB_QSCALE;
	s->picture[0].mb_type=s->mybuf->MB_TYBASE+s->mb_stride+1;
	s->picture[0].motion_val[0]=(int16_t(*)[2])s->mybuf->MotionVal+2;
	s->picture[0].ref_index=s->mybuf->IndexRef;

	s->picture[1].data[0]=s->mybuf->Ybuf[1]+3088;
	s->picture[1].data[1]=s->mybuf->Cbbuf[1]+776;
	s->picture[1].data[2]=s->mybuf->Crbuf[1]+776;
	s->picture[1].qscale_table=s->mybuf->TB_QSCALE;
	s->picture[1].mb_type=s->mybuf->MB_TYBASE+s->mb_stride+1;
	s->picture[1].motion_val[0]=(int16_t(*)[2])s->mybuf->MotionVal+2;
	s->picture[1].ref_index=s->mybuf->IndexRef;

    return 0;
}

void MPV_common_end(MpegEncContext *s)
{
	s->context_initialized = 0;
	s->current_picture_ptr= NULL;
}

static void draw_edges_c(uint8_t *buf, int wrap, int width, int height, int w)
{
    uint8_t *ptr, *last_line;
    int i;

    last_line = buf + (height - 1) * wrap;
    for(i=0;i<w;i++)   /* top and bottom */
	{
        memcpy(buf - (i + 1) * wrap, buf, width);
        memcpy(last_line + (i + 1) * wrap, last_line, width);
    }

    ptr = buf;
    for(i=0;i<height;i++)    /* left and right */
	{
        memset(ptr - w, ptr[0], w);
        memset(ptr + width, ptr[width-1], w);
        ptr += wrap;
    }

    for(i=0;i<w;i++)    /* corners */
	{
        memset(buf - (i + 1) * wrap - w, buf[0], w); /* top left */
        memset(buf - (i + 1) * wrap + width, buf[width-1], w); /* top right */
        memset(last_line + (i + 1) * wrap - w, last_line[0], w); /* top left */
        memset(last_line + (i + 1) * wrap + width, last_line[width-1], w); /* top right */
    }
}

int MPV_frame_start(MpegEncContext *s, AVCodecContext *avctx)
{
	s->current_picture_ptr=&s->picture[s->avctx->internal_buffer_count];
	if(s->avctx->internal_buffer_count)s->avctx->internal_buffer_count=0;
	else s->avctx->internal_buffer_count=1;
	s->current_picture_ptr->reference=s->pict_type!=B_TYPE?3:0;
    return 0;
}

void MPV_frame_end(MpegEncContext *s)
{
    if(s->pict_type != B_TYPE)
	{
        draw_edges_c(s->current_picture_ptr->data[0], Ylinesize , s->h_edge_pos   , s->v_edge_pos   , EDGE_WIDTH  );
        draw_edges_c(s->current_picture_ptr->data[1], UVlinesize, s->h_edge_pos>>1, s->v_edge_pos>>1, EDGE_WIDTH/2);
        draw_edges_c(s->current_picture_ptr->data[2], UVlinesize, s->h_edge_pos>>1, s->v_edge_pos>>1, EDGE_WIDTH/2);
    } 
}

