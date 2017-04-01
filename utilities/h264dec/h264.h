#include "common.h"
#include "dsputil.h"
#include "avcodec.h"
#include "mpegvideo.h"
#include "h264data.h"
#include "golomb.h"
#include "cabac.h"

//#include <assert.h>

#define NAL_SLICE		1
#define NAL_DPA			2
#define NAL_DPB			3
#define NAL_DPC			4
#define NAL_IDR_SLICE	5
#define NAL_SEI			6
#define NAL_SPS			7
#define NAL_PPS			8
#define NAL_PICTURE_DELIMITER	9
#define NAL_FILTER_DATA		10

#define LUMA_DC_BLOCK_INDEX   25
#define CHROMA_DC_BLOCK_INDEX 26

#define CHROMA_DC_COEFF_TOKEN_VLC_BITS 8
#define COEFF_TOKEN_VLC_BITS           8
#define TOTAL_ZEROS_VLC_BITS           9
#define CHROMA_DC_TOTAL_ZEROS_VLC_BITS 3
#define RUN_VLC_BITS                   3
#define RUN7_VLC_BITS                  6

#define MAX_MMCO_COUNT  2
#define REF_COUNT0      1


#define XCHG(a,b,t,xchg)\
t= a;\
if(xchg)\
    a= b;\
b= t;

typedef struct SPS
{
    int log2_max_frame_num;            ///< log2_max_frame_num_minus4 + 4
    int poc_type;                      ///< pic_order_cnt_type
    int log2_max_poc_lsb;              ///< log2_max_pic_order_cnt_lsb_minus4
    int ref_frame_count;               ///< num_ref_frames
    int mb_width;                      ///< frame_width_in_mbs_minus1 + 1
    int mb_height;                     ///< frame_height_in_mbs_minus1 + 1
    int frame_mbs_only_flag;
    int mb_aff;                        ///<mb_adaptive_frame_field_flag
    int crop_left;              ///< frame_cropping_rect_left_offset
    int crop_right;             ///< frame_cropping_rect_right_offset
    int crop_top;               ///< frame_cropping_rect_top_offset
    int crop_bottom;            ///< frame_cropping_rect_bottom_offset
}SPS;
/**
 * Picture parameter set
 */
typedef struct PPS
{
    int sps_id;
    int cabac;                  ///< entropy_coding_mode_flag
    int pic_order_present;      ///< pic_order_present_flag
    int slice_group_count;      ///< num_slice_groups_minus1 + 1
    int ref_count[2];           ///< num_ref_idx_l0/1_active_minus1 + 1
    int init_qp;                ///< pic_init_qp_minus26 + 26
    int chroma_qp_index_offset;
    int deblocking_filter_parameters_present; ///< deblocking_filter_parameters_present_flag
    int constrained_intra_pred; ///< constrained_intra_pred_flag
    int redundant_pic_cnt_present; ///< redundant_pic_cnt_present_flag
}PPS;

/**
 * Memory management control operation opcode.
 */
typedef enum MMCOOpcode
{
    MMCO_END=0,
    MMCO_SHORT2UNUSED,
    MMCO_LONG2UNUSED,
    MMCO_SHORT2LONG,
    MMCO_SET_MAX_LONG,
    MMCO_RESET,
    MMCO_LONG,
}MMCOOpcode;

/**
 * Memory management control operation.
 */
typedef struct MMCO
{
    MMCOOpcode opcode;
    int short_frame_num;
    int long_index;
}MMCO;

/**
 * H264Context
 */
typedef struct H264Context
{
    MpegEncContext s;
    int nal_unit_type;
    int chroma_qp; //QPc
    int chroma_pred_mode;
    int intra16x16_pred_mode;

    int8_t intra4x4_pred_mode_cache[5*8];
    int8_t intra4x4_pred_mode[99][8];
    void (*pred4x4  [9+3])(uint8_t *src, uint8_t *topright, int stride);//FIXME move to dsp?
    void (*pred8x8  [4+3])(uint8_t *src, int stride);
    void (*pred16x16[4+3])(uint8_t *src, int stride);
    unsigned int topleft_samples_available;
    unsigned int top_samples_available;
    unsigned int topright_samples_available;
    unsigned int left_samples_available;
    uint8_t top_border[10][32];
    uint8_t left_border[18+2*9];
    uint8_t non_zero_count_cache[6*8];
    uint8_t non_zero_count[99][16];
    int16_t mv_cache[1][5*8][2];
    int8_t ref_cache[1][5*8];

#define LIST_NOT_USED -1 //FIXME rename?
#define PART_NOT_AVAILABLE -2

    int block_offset[16+8];
	uint16_t mb2b_xy[99];
	uint16_t mb2b8_xy[99];

    int b_stride;
    int b8_stride;

    SPS sps; ///< current sps
    PPS pps; //FIXME move tp Picture perhaps? (->no) do we need that?

    int slice_num;
    uint8_t slice_table_base[99];
    uint8_t *slice_table;      ///< slice_table_base + mb_stride + 1
    int slice_type;
    int mb_field_decoding_flag;
    int sub_mb_type[4];
    int frame_num;
    int curr_pic_num;
    int max_pic_num;
    int deblocking_filter;         ///< disable_deblocking_filter_idc with 1<->0
    int slice_alpha_c0_offset;
    int slice_beta_offset;
    int redundant_pic_count;
    int ref_count[2];// FIXME split for AFF
    Picture *short_ref[16];
    Picture *long_ref[16];
    Picture default_ref_list[8];
    Picture ref_list[8]; //FIXME size?

    MMCO mmco[MAX_MMCO_COUNT];
    int mmco_index;
    int long_ref_count;  ///< number of actual long term references
    int short_ref_count; ///< number of actual short term references
    //data partitioning
    GetBitContext intra_gb;
    GetBitContext inter_gb;
    GetBitContext *intra_gb_ptr;
    GetBitContext *inter_gb_ptr;

    DCTELEM mb[16*24];
    CABACContext cabac;
    uint8_t      cabac_state[399];
    int          cabac_init_idc;
    uint16_t     *cbp_table;
    uint8_t     *chroma_pred_mode_table;
    int         last_qscale_diff;
    int16_t     mvd_cache[5*8][2];
    
    int16_t (*coeff_token_vlc[4])[2];
    int16_t (*chroma_dc_coeff_token_vlc)[2];
    int16_t (*total_zeros_vlc[15])[2];
    int16_t (*chroma_dc_total_zeros_vlc[3])[2];
    int16_t (*run_vlc[6])[2];
    int16_t (*run7_vlc)[2];    
}H264Context;


