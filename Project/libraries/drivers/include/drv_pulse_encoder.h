#ifndef __PULSE_ENCODER_CONFIG_H__
#define __PULSE_ENCODER_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_PULSE_ENCODER1
#ifndef PULSE_ENCODER1_CONFIG
#define PULSE_ENCODER1_CONFIG                     \
    {                                             \
       .tmr_handler     = TMR1,          \
       .encoder_irqn             = TMR1_OVF_TMR10_IRQn,  \
       .name                     = "pulse1"       \
    }
#endif /* PULSE_ENCODER1_CONFIG */
#endif /* BSP_USING_PULSE_ENCODER1 */

#ifdef BSP_USING_PULSE_ENCODER2
#ifndef PULSE_ENCODER2_CONFIG
#define PULSE_ENCODER2_CONFIG                  \
    {                                          \
       .tmr_handler     = TMR2,       \
       .encoder_irqn             = TMR2_GLOBAL_IRQn,  \
       .name                     = "pulse2"    \
    }
#endif /* PULSE_ENCODER2_CONFIG */
#endif /* BSP_USING_PULSE_ENCODER2 */

#ifdef BSP_USING_PULSE_ENCODER3
#ifndef PULSE_ENCODER3_CONFIG
#define PULSE_ENCODER3_CONFIG                  \
    {                                          \
       .tmr_handler     = TMR3,       \
       .encoder_irqn             = TMR3_GLOBAL_IRQn,  \
       .name                     = "pulse3"    \
    }
#endif /* PULSE_ENCODER3_CONFIG */
#endif /* BSP_USING_PULSE_ENCODER3 */

#ifdef BSP_USING_PULSE_ENCODER4
#ifndef PULSE_ENCODER4_CONFIG
#define PULSE_ENCODER4_CONFIG                  \
    {                                          \
       .tmr_handler     = TMR4,       \
       .encoder_irqn             = TMR4_GLOBAL_IRQn,  \
       .name                     = "pulse4"    \
    }
#endif /* PULSE_ENCODER4_CONFIG */
#endif /* BSP_USING_PULSE_ENCODER4 */

#ifdef __cplusplus
}
#endif

#endif /* __PULSE_ENCODER_CONFIG_H__ */
