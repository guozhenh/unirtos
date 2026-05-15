/*****************************************************************/ /**
* @file qosa_dev_silkDecCommon_eigen.h
* @brief VPU operation interface
* @author xavier.bao@quectel.com
* @date 2026-01-21
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-08-15 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_SILKDECCOMMOM_EIGEN_H__
#define __QOSA_SILKDECCOMMOM_EIGEN_H__

#include "qosa_system_config.h"
#ifdef CONFIG_QOSA_EIGEN718_PLATFORM_FUNC
#include "silkDecCommon.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Enum
 ===========================================================================*/

/*===========================================================================
 * callback
===========================================================================*/

/*===========================================================================
 *  Struct
===========================================================================*/

/*===========================================================================
 * Functions declaration
 ===========================================================================*/

extern void SilkDecVpuInit(void);
typedef void (*_api_SilkDecVpuInit_t)(void);

extern void SilkDecVpuClock_On(void);
typedef void (*_api_SilkDecVpuClock_On_t)(void);

extern void SilkDecVpuClock_Off(void);
typedef void (*_api_SilkDecVpuClock_Off_t)(void);

extern void SKP_Silk_biquad_vpu(const Vpu_int16 *in, const Vpu_int16 *B, const Vpu_int16 *A,Vpu_int32 *S, Vpu_int16 *out, const Vpu_int32 len);
typedef void (*_api_SKP_Silk_biquad_vpu_t)(const Vpu_int16 *in, const Vpu_int16 *B, const Vpu_int16 *A, Vpu_int32 *S, Vpu_int16 *out, const Vpu_int32 len);

extern void SKP_Silk_LTPQ16Cal_SMULWB_vpu(Vpu_int32* sLTP_Q16, Vpu_int16 *sLTP, Vpu_int32 inv_gain_Q32, Vpu_int32 len);
typedef void (*_api_SKP_Silk_LTPQ16Cal_SMULWB_vpu_t)(Vpu_int32* sLTP_Q16, Vpu_int16 *sLTP, Vpu_int32 inv_gain_Q32, Vpu_int32 len);

extern void SKP_Silk_LTPQ16Cal_SMULWW_vpu(Vpu_int32* sLTP_Q16, Vpu_int32 gain_adj_Q16, Vpu_int32 cal_len);
typedef void (*_api_SKP_Silk_LTPQ16Cal_SMULWW_vpu_t)(Vpu_int32* sLTP_Q16, Vpu_int32 gain_adj_Q16, Vpu_int32 cal_len);

extern void SKP_Silk_LongTermPredict_vpu(Vpu_int32 *sLTP_Q16, Vpu_int32 *pres_Q10,Vpu_int32 *pexc_Q10,Vpu_int32 *pred_lag_ptr,Vpu_int16 *CoeffA,Vpu_int32 cal_len,Vpu_int32 seg_Len);
typedef void (*_api_SKP_Silk_LongTermPredict_vpu_t)(Vpu_int32 *sLTP_Q16,Vpu_int32 *pres_Q10,Vpu_int32 *pexc_Q10,Vpu_int32 *pred_lag_ptr,Vpu_int16 *CoeffA,Vpu_int32 cal_len,Vpu_int32 seg_Len);	

extern void SKP_Silk_ScaleWithGain_vpu(Vpu_int16* pxq, Vpu_int32* vec_Q10, Vpu_int32 Gain_Q16, Vpu_int32 cal_len);
typedef void (*_api_SKP_Silk_ScaleWithGain_vpu_t)(Vpu_int16* pxq, Vpu_int32* vec_Q10, Vpu_int32 Gain_Q16, Vpu_int32 cal_len);

extern void SKP_Silk_decode_short_term_prediction_vpu(Vpu_int32	*vec_Q10, Vpu_int32 *pres_Q10, Vpu_int32 *sLPC_Q14, Vpu_int16 *A_Q12_tmp, Vpu_int32 LPC_order, Vpu_int32 subfr_length);
typedef void (*_api_SKP_Silk_decode_short_term_prediction_vpu_t)(Vpu_int32 *vec_Q10, Vpu_int32 *pres_Q10, Vpu_int32 *sLPC_Q14, Vpu_int16 *A_Q12_tmp, Vpu_int32 LPC_order, Vpu_int32 subfr_length);	

extern void SKP_Silk_NLSF_MSVQ_decode_half_vpu(Vpu_int32 *pNLSF_Q15, Vpu_int32 volatile *vldAddr, Vpu_int16	nStages, const Vpu_int32 LPC_order);
typedef void (*_api_SKP_Silk_NLSF_MSVQ_decode_half_vpu_t)(Vpu_int32 *pNLSF_Q15, Vpu_int32 volatile *vldAddr, Vpu_int16	nStages, const Vpu_int32 LPC_order);

#endif /* CONFIG_QOSA_EIGEN718_PLATFORM_FUNC */
#endif /* __QOSA_SILKDECCOMMOM_EIGEN_H__ */
