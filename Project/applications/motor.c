//#include <rtthread.h>
//#include <rtdevice.h>
//#include "drv_common.h"
//#include "board.h"
//
//#define SPEED 350000
//#define SPEED1 450000
//
//#define PWM1_DEV_NAME "pwm3"                                     /* PWM�豸���� */
//#define PWM1_DEV_CHANNEL 3                                       /* PWMͨ�� */
//#define PWM2_DEV_NAME "pwm3"                                     /* PWM�豸���� */
//#define PWM2_DEV_CHANNEL 4                                       /* PWMͨ�� */
//#define PWM3_DEV_NAME "pwm4"                                     /* PWM�豸���� */
//#define PWM3_DEV_CHANNEL 3                                       /* PWMͨ�� */
//#define PWM4_DEV_NAME "pwm4"                                     /* PWM�豸���� */
//#define PWM4_DEV_CHANNEL 4                                       /* PWMͨ�� */
//struct rt_device_pwm *pwm1_dev, *pwm2_dev, *pwm3_dev, *pwm4_dev; /* PWM�豸��� */
//
//static rt_thread_t motor_th;
//extern double linear_x,angular_z;
//extern rt_sem_t sem_ros;
//
//static void motor_entry()
//{
//    rt_uint32_t period, pulse1, pulse2, pulse3, pulse4;
//    period = 1000000; /* 1KHz����Ϊ1ms,���ﵥλ������ns��1ms����10��6�η�����ns*/
//    pulse1 = pulse2 = pulse3 = pulse4 = 0;        /* PWM������ֵ����λΪ����ns */ //500000
//
//    /* �����豸 */
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
//    /* ����PWM���ں������� */
//    rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL, period, pulse1);
//    rt_pwm_set(pwm2_dev, PWM2_DEV_CHANNEL, period, pulse2);
//    rt_pwm_set(pwm3_dev, PWM3_DEV_CHANNEL, period, pulse3);
//    rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL, period, pulse4);
//    /* ʹ���豸 */
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
//        // ����ٶ�Ϊ�㣬��ֹͣ�������
//        if (linear_x == 0 && angular_z == 0)
//            pulse1 = pulse2 = pulse3 = pulse4 = 0;
//        // �������ٶȺͽ��ٶȿ������������ת��
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
//        /* ����PWM���ں������� */
//        rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL, period, pulse1);
//        rt_pwm_set(pwm2_dev, PWM2_DEV_CHANNEL, period, pulse2);
//        rt_pwm_set(pwm3_dev, PWM3_DEV_CHANNEL, period, pulse3);
//        rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL, period, pulse4);
//        /* ʹ���豸 */
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
//    /* �������߳̿��ƿ飬��������߳� */
//    if (motor_th != RT_NULL)
//        rt_thread_startup(motor_th); // �����߳�1
//    else
//        rt_kprintf("motor_th created failed\n");
//
//    return RT_EOK;
//}
//MSH_CMD_EXPORT(motor_init, motor drive)
