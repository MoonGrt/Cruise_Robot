//#include <rtthread.h>
//#include <rtdevice.h>
//#include "drv_common.h"
//#include "board.h"
//
//#define SPEED 350000
//#define SPEED1 450000
//
//#define PWM1_DEV_NAME "pwm3"                                     /* PWM设备名称 */
//#define PWM1_DEV_CHANNEL 3                                       /* PWM通道 */
//#define PWM2_DEV_NAME "pwm3"                                     /* PWM设备名称 */
//#define PWM2_DEV_CHANNEL 4                                       /* PWM通道 */
//#define PWM3_DEV_NAME "pwm4"                                     /* PWM设备名称 */
//#define PWM3_DEV_CHANNEL 3                                       /* PWM通道 */
//#define PWM4_DEV_NAME "pwm4"                                     /* PWM设备名称 */
//#define PWM4_DEV_CHANNEL 4                                       /* PWM通道 */
//struct rt_device_pwm *pwm1_dev, *pwm2_dev, *pwm3_dev, *pwm4_dev; /* PWM设备句柄 */
//
//static rt_thread_t motor_th;
//extern double linear_x,angular_z;
//extern rt_sem_t sem_ros;
//
//static void motor_entry()
//{
//    rt_uint32_t period, pulse1, pulse2, pulse3, pulse4;
//    period = 1000000; /* 1KHz周期为1ms,这里单位是纳秒ns，1ms等于10的6次方纳秒ns*/
//    pulse1 = pulse2 = pulse3 = pulse4 = 0;        /* PWM脉冲宽度值，单位为纳秒ns */ //500000
//
//    /* 查找设备 */
//    pwm1_dev = (struct rt_device_pwm *)rt_device_find(PWM1_DEV_NAME);
//    if (pwm1_dev == RT_NULL)
//        rt_kprintf("can't find %s device!\n", PWM1_DEV_NAME);
//    pwm2_dev = (struct rt_device_pwm *)rt_device_find(PWM2_DEV_NAME);
//    if (pwm2_dev == RT_NULL)
//        rt_kprintf("can't find %s device!\n", PWM2_DEV_NAME);
//    pwm3_dev = (struct rt_device_pwm *)rt_device_find(PWM3_DEV_NAME);
//    if (pwm3_dev == RT_NULL)
//        rt_kprintf("can't find %s device!\n", PWM3_DEV_NAME);
//    pwm4_dev = (struct rt_device_pwm *)rt_device_find(PWM4_DEV_NAME);
//    if (pwm4_dev == RT_NULL)
//        rt_kprintf("can't find %s device!\n", PWM4_DEV_NAME);
//
//    /* 设置PWM周期和脉冲宽度 */
//    rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL, period, pulse1);
//    rt_pwm_set(pwm2_dev, PWM2_DEV_CHANNEL, period, pulse2);
//    rt_pwm_set(pwm3_dev, PWM3_DEV_CHANNEL, period, pulse3);
//    rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL, period, pulse4);
//    /* 使能设备 */
//    rt_pwm_enable(pwm1_dev, PWM1_DEV_CHANNEL);
//    rt_pwm_enable(pwm2_dev, PWM2_DEV_CHANNEL);
//    rt_pwm_enable(pwm3_dev, PWM3_DEV_CHANNEL);
//    rt_pwm_enable(pwm4_dev, PWM4_DEV_CHANNEL);
//
//    rt_kprintf("motor init success!\n");
//
//    while (1)
//    {
//        rt_sem_take(sem_ros, RT_WAITING_FOREVER);
////        printf("linear_x:%f angular_z: %f\n\n", linear_x, angular_z);
//
//        // 如果速度为零，则停止两个电机
//        if (linear_x == 0 && angular_z == 0)
//            pulse1 = pulse2 = pulse3 = pulse4 = 0;
//        // 根据线速度和角速度控制两个电机的转速
//        if (linear_x > 0)
//        {
//            pulse1 = pulse3 = 0;
//            pulse2 = pulse4 = SPEED;
//        }
//        if (linear_x < 0)
//        {
//            pulse1 = pulse3 = SPEED;
//            pulse2 = pulse4 = 0;
//        }
//        if (angular_z > 0)
//        {
//            pulse2 = pulse3 = SPEED1;
//            pulse1 = pulse4 = 0;
//        }
//        if (angular_z < 0)
//        {
//            pulse1 = pulse4 = SPEED1;
//            pulse2 = pulse3 = 0;
//        }
//
//        /* 设置PWM周期和脉冲宽度 */
//        rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL, period, pulse1);
//        rt_pwm_set(pwm2_dev, PWM2_DEV_CHANNEL, period, pulse2);
//        rt_pwm_set(pwm3_dev, PWM3_DEV_CHANNEL, period, pulse3);
//        rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL, period, pulse4);
//        /* 使能设备 */
//        rt_pwm_enable(pwm1_dev, PWM1_DEV_CHANNEL);
//        rt_pwm_enable(pwm2_dev, PWM2_DEV_CHANNEL);
//        rt_pwm_enable(pwm3_dev, PWM3_DEV_CHANNEL);
//        rt_pwm_enable(pwm4_dev, PWM4_DEV_CHANNEL);
//    }
//}
//
//int motor_init(void)
//{
//    motor_th = rt_thread_create("motor", motor_entry, RT_NULL, 1024, 20, 5);
//    /* 如果获得线程控制块，启动这个线程 */
//    if (motor_th != RT_NULL)
//        rt_thread_startup(motor_th); // 启动线程1
//    else
//        rt_kprintf("motor_th created failed\n");
//
//    return RT_EOK;
//}
//MSH_CMD_EXPORT(motor_init, motor drive)
