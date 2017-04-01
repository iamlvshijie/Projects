
#ifndef AVCODEC_MPEGVIDEO_H
#define AVCODEC_MPEGVIDEO_H

#include "dsputil.h"

#define EDGE_WIDTH 16

#define MAX_PICTURE_COUNT 2//15

#define I_TYPE 1
#define P_TYPE 2
#define B_TYPE 3
#define S_TYPE 4
#define SI_TYPE 5
#define SP_TYPE 6

#define MB_TYPE_INTRA MB_TYPE_INTRA4x4 //default mb_type if theres just one type
#define IS_INTRA4x4(a)   ((a)&MB_TYPE_INTRA4x4)
#define IS_INTRA16x16(a) ((a)&MB_TYPE_INTRA16x16)
#define IS_PCM(a)        ((a)&MB_TYPE_INTRA_PCM)
#define IS_INTRA(a)      ((a)&7)
#define IS_INTER(a)      ((a)&(MB_TYPE_16x16|MB_TYPE_16x8|MB_TYPE_8x16|MB_TYPE_8x8))
#define IS_SKIP(a)       ((a)&MB_TYPE_SKIP)
#define IS_INTRA_PCM(a)  ((a)&MB_TYPE_INTRA_PCM)
#define IS_INTERLACED(a) ((a)&MB_TYPE_INTERLACED)
#define IS_DIRECT(a)     ((a)&MB_TYPE_DIRECT2)
#define IS_GMC(a)        ((a)&MB_TYPE_GMC)
#define IS_16X16(a)      ((a)&MB_TYPE_16x16)
#define IS_16X8(a)       ((a)&MB_TYPE_16x8)
#define IS_8X16(a)       ((a)&MB_TYPE_8x16)
#define IS_8X8(a)        ((a)&MB_TYPE_8x8)
#define IS_SUB_8X8(a)    ((a)&MB_TYPE_16x16) //note reused
#define IS_SUB_8X4(a)    ((a)&MB_TYPE_16x8)  //note reused
#define IS_SUB_4X8(a)    ((a)&MB_TYPE_8x16)  //note reused
#define IS_SUB_4X4(a)    ((a)&MB_TYPE_8x8)   //note reused
#define IS_ACPRED(a)     ((a)&MB_TYPE_ACPRED)
#define IS_QUANT(a)      ((a)&MB_TYPE_QUANT)
#define IS_DIR(a, part, list) ((a) & (MB_TYPE_P0L0<<((part)+2*(list))))
#define USES_LIST(a, list) ((a) & ((MB_TYPE_P0L0|MB_TYPE_P1L0)<<(2*(list)))) ///< does this mb use listX, note doesnt work if subMBs
#define HAS_CBP(a)        ((a)&MB_TYPE_CBP)

#define VP_START            1          ///< current MB is the first after a resync marker
#define AC_ERROR            2
#define DC_ERROR            4
#define MV_ERROR            8
#define AC_END              16
#define DC_END              32
#define MV_END              64

#define PICT_TOP_FIELD     1
#define PICT_BOTTOM_FIELD  2
#define PICT_FRAME         3


typedef struct Picture
{
    uint8_t *data[4];
    int reference;
	int8_t *qscale_table;
	uint32_t *mb_type;
    int16_t (*motion_val[1])[2]; 
    int8_t *ref_index;
    int poc;                    ///< h264 frame POC
    int frame_num;              ///< h264 frame_num
    int pic_id;                 ///< h264 pic_num or long_term_pic_idx
    int long_ref;               ///< 1->long term reference 0->short term reference
} Picture;

typedef struct MpegEncContext
{
    AVCodecContext *avctx;
    int width, height;///< picture size. must be a multiple of 16
    /* sequence parameters */
    int context_initialized;
	int mb_width, mb_height;   ///< number of MBs horizontally & vertically
    int mb_stride;             ///< mb_width+1 used for some arrays to allow simple addressng of left & top MBs withoutt sig11
    int h_edge_pos, v_edge_pos;///< horizontal / vertical position of the right/bottom edge (pixel replicateion)
	Picture picture[2];          ///< main picture buffer
	Picture *current_picture_ptr;  ///< pointer to the current picture
#define PREV_PICT_TYPES_BUFFER_SIZE 256
    int qscale;                 ///< QP
    int pict_type;              ///< I_TYPE, P_TYPE, B_TYPE, ...
    DSPContext dsp;             ///< pointers for accelerated dsp fucntions
    /* macroblock layer */
    int mb_x, mb_y;
    int mb_skip_run;
    /* mpeg4 specific */
    int data_partitioning;           ///< data partitioning flag from header
    /* decompression specific */
    GetBitContext gb;
    /* MPEG2 specific - I wish I had not to support this mess. */
    int picture_structure;
    Publicbuf *mybuf;
} MpegEncContext;

int MPV_common_init(MpegEncContext *s);
void MPV_common_end(MpegEncContext *s);
int MPV_frame_start(MpegEncContext *s, AVCodecContext *avctx);
void MPV_frame_end(MpegEncContext *s);
int ff_find_unused_picture(MpegEncContext *s, int shared);

#endif
