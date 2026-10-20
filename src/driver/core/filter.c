/**
 * @file    filter.c
 * @brief   实现滤波器相关功能
 * @author  liaofei
 * @version v2.1.0
 * @data    2021年4月27日
 */

#include "filter.h"

#define DEBUG_TAG "mean_filter,%d", __LINE__
#define DEBUG_LEVEL ERROR_OUTPUT
#include "debug.h"

/**
 * @function nane: filter_net_notch_Q
 * @author: liaofei
 * @data: 2021年4月27日
 * @input param:
 *              center_freq 陷波中心频率
 *              cutoff 陷波带宽
 * @output param:
 *              return 返回二阶陷波器的Q参数
 * @description: 
 * 计算二阶陷波器的Q参数
 * @modeify log: 
 * 2021年4月27日 liaofei 初次编写
 */
static float filter_net_notch_Q(float center_freq, float cutoff)
{
    float octaves;

    octaves = log2f(center_freq  / cutoff) * 2;
    return sqrtf(powf(2, octaves)) / (powf(2, octaves) - 1);
}

/**
 * @function nane: biquad_filter_init
 * @author: liaofei
 * @data: 2021年4月27日
 * @input param:
 *              filter 滤波器句柄
 *              sampling_freq 采样频率
 *              filter_freq 滤波器工作频率
 *              Q 滤波器参数
 *              filter_type 滤波器类型
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description: 
 * 初始化二阶滤波器
 * @modeify log: 
 * 2021年4月27日 liaofei 初次编写
 */
int32_t biquad_filter_init(biquad_filter_t *filter, float sampling_freq, float filter_freq, float Q, uint32_t filter_type)
{
    float sn;
    float cs;
    float omega;
    float alpha;
    float sample_rate;
    float a0, a1, a2;
    float b0, b1, b2;

    if(!filter) {
        return ERR_INVAL;
    }

    // Check for Nyquist frequency and if it's not possible to initialize filter as requested - set to no filtering at all
    if (filter_freq < (sampling_freq / 2)) {
        // setup variables
        sample_rate = sampling_freq;
        omega = 2.0f * M_PI * filter_freq / sample_rate;
        sn = sinf(omega);
        cs = cosf(omega);
        alpha = sn / (2 * Q);

        switch (filter_type) {
        case FILTER_LPF:
            b0 = (1 - cs) / 2;
            b1 = 1 - cs;
            b2 = (1 - cs) / 2;
            break;
        case FILTER_NOTCH:
            b0 =  1;
            b1 = -2 * cs;
            b2 =  1;
            break;
        }
        a0 =  1 + alpha;
        a1 = -2 * cs;
        a2 =  1 - alpha;

        // precompute the coefficients
        filter->b0 = b0 / a0;
        filter->b1 = b1 / a0;
        filter->b2 = b2 / a0;
        filter->a1 = a1 / a0;
        filter->a2 = a2 / a0;
    }
    else {
        // Not possible to filter frequencies above Nyquist frequency - passthrough
        filter->b0 = 1.0f;
        filter->b1 = 0.0f;
        filter->b2 = 0.0f;
        filter->a1 = 0.0f;
        filter->a2 = 0.0f;
    }

    // zero initial samples
    filter->d1 = filter->d2 = 0;

    return ERR_OK;
}

/**
 * @function nane: biquad_filter_init_notch
 * @author: liaofei
 * @data: 2021年4月27日
 * @input param:
 *              filter 滤波器句柄
 *              sampling_freq 采样频率
 *              filter_freq 陷波中心频率
 *              cutoff_Hz 陷波带宽
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description: 
 * 初始化二阶陷波器
 * @modeify log: 
 * 2021年4月27日 liaofei 初次编写
 */
int32_t biquad_filter_init_notch(biquad_filter_t *filter, float sampling_freq, float filter_freq, float cutoff_Hz)
{
    float Q;

    Q = filter_net_notch_Q(filter_freq, cutoff_Hz);
    return biquad_filter_init(filter, sampling_freq, filter_freq, Q, FILTER_NOTCH);
}

/**
 * @function nane: biquad_filter_init_LPF
 * @author: liaofei
 * @data: 2021年4月27日
 * @input param:
 *              filter 滤波器句柄
 *              sampling_freq 采样频率
 *              filter_freq 截止频率
 * @output param:
 *              return 成功返回ERR_OK，失败返回错误码
 * @description: 
 * 初始化二阶低通滤波器
 * @modeify log: 
 * 2021年4月27日 liaofei 初次编写
 */
int32_t biquad_filter_init_LPF(biquad_filter_t *filter, float sampling_freq, float filter_freq)
{
    return biquad_filter_init(filter, sampling_freq, filter_freq, BIQUAD_Q, FILTER_LPF);
}

/**
 * @function nane: biquad_filter_apply
 * @author: liaofei
 * @data: 2021年4月27日
 * @input param:
 *              filter 滤波器句柄
 *              input 原始值
 * @output param:
 *              return 滤波后的值
 * @description: 
 * 滤波器滤波
 * @modeify log: 
 * 2021年4月27日 liaofei 初次编写
 */
float biquad_filter_apply(biquad_filter_t *filter, float input)
{
    float result;

    if(filter) {
        result = filter->b0 * input + filter->d1;
        filter->d1 = filter->b1 * input - filter->a1 * result + filter->d2;
        filter->d2 = filter->b2 * input - filter->a2 * result;
        return result;
    }
    else {
        return input;
    }
}
