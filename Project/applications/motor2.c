#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <stdio.h>
#include "drv_common.h"
#include "board.h"

#define LOG_TAG "thread_app"
#define LOG_LVL LOG_LVL_DBG

#define SPEED 350000
#define SPEED1 500000

#define UP 0x47    //'G'
#define DOWN 0x4b  //'K'
#define LEFT 0x48  //'H'
#define RIGHT 0x4a //'J'
#define STOP 0x49  //'I'

#define PWM1_DEV_NAME "pwm3"                                     /* PWM�豸���� */
#define PWM1_DEV_CHANNEL 3                                       /* PWMͨ�� */
#define PWM2_DEV_NAME "pwm3"                                     /* PWM�豸���� */
#define PWM2_DEV_CHANNEL 4                                       /* PWMͨ�� */
#define PWM3_DEV_NAME "pwm4"                                     /* PWM�豸���� */
#define PWM3_DEV_CHANNEL 3                                       /* PWMͨ�� */
#define PWM4_DEV_NAME "pwm4"                                     /* PWM�豸���� */
#define PWM4_DEV_CHANNEL 4                                       /* PWMͨ�� */
struct rt_device_pwm *pwm1_dev, *pwm2_dev, *pwm3_dev, *pwm4_dev; /* PWM�豸��� */

rt_thread_t motor = NULL, uart_tx = NULL;
static rt_device_t uart1;
static rt_sem_t sem;

struct serial_configure uart1_config = {
    BAUD_RATE_115200,   /* 115200 bits/s */
    DATA_BITS_8,        /* 8 databits */
    STOP_BITS_1,        /* 1 stopbit */
    PARITY_NONE,        /* No parity  */
    BIT_ORDER_LSB,      /* LSB first sent */
    NRZ_NORMAL,         /* Normal mode */
    RT_SERIAL_RB_BUFSZ, /* Buffer size */
    0};

// ���ջص�����
rt_err_t rx_callback1(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(sem);
    return RT_EOK;
}

static void motor_ctrl(void *parameter)
{
    rt_uint32_t period, pulse1, pulse2, pulse3, pulse4;
    char direction;
    period = 1000000; /* 1KHz����Ϊ1ms,���ﵥλ������ns��1ms����10��6�η�����ns*/
    pulse1 = pulse2 = pulse3 = pulse4 = 0;

    /* �����豸 */
    pwm1_dev = (struct rt_device_pwm *)rt_device_find(PWM1_DEV_NAME);
    if (pwm1_dev == RT_NULL)
        rt_kprintf("can't find %s device!\n", PWM1_DEV_NAME);
    else
        rt_kprintf("find %s device!\n", PWM1_DEV_NAME);
    pwm2_dev = (struct rt_device_pwm *)rt_device_find(PWM2_DEV_NAME);
    if (pwm2_dev == RT_NULL)
        rt_kprintf("can't find %s device!\n", PWM2_DEV_NAME);
    else
        rt_kprintf("find %s device!\n", PWM2_DEV_NAME);
    pwm3_dev = (struct rt_device_pwm *)rt_device_find(PWM3_DEV_NAME);
    if (pwm3_dev == RT_NULL)
        rt_kprintf("can't find %s device!\n", PWM3_DEV_NAME);
    else
        rt_kprintf("find %s device!\n", PWM3_DEV_NAME);
    pwm4_dev = (struct rt_device_pwm *)rt_device_find(PWM4_DEV_NAME);
    if (pwm4_dev == RT_NULL)
        rt_kprintf("can't find %s device!\n", PWM4_DEV_NAME);
    else
        rt_kprintf("find %s device!\n", PWM4_DEV_NAME);

    while (1)
    {
        while (rt_device_read(uart1, 0, &direction, 1) != 1)
            rt_sem_take(sem, RT_WAITING_FOREVER);
        rt_kprintf("%c", direction);

        if (direction == UP)
        {
            pulse1 = pulse3 = 0;
            pulse2 = pulse4 = 280000;
        }
        if (direction == DOWN)
        {
            pulse1 = pulse3 = 280000;
            pulse2 = pulse4 = 0;
        }
        if (direction == LEFT)
        {
            pulse2 = pulse3 = 0;
            pulse1 = pulse4 = 450000;
        }
        if (direction == RIGHT)
        {
            pulse1 = pulse4 = 0;
            pulse2 = pulse3 = 450000;
        }
        if (direction == STOP)
            pulse1 = pulse2 = pulse3 = pulse4 = 0;

        /* ����PWM���ں������� */
        rt_pwm_set(pwm1_dev, PWM1_DEV_CHANNEL, period, pulse1);
        rt_pwm_set(pwm2_dev, PWM2_DEV_CHANNEL, period, pulse2);
        rt_pwm_set(pwm3_dev, PWM3_DEV_CHANNEL, period, pulse3);
        rt_pwm_set(pwm4_dev, PWM4_DEV_CHANNEL, period, pulse4);
        /* ʹ���豸 */
        rt_pwm_enable(pwm1_dev, PWM1_DEV_CHANNEL);
        rt_pwm_enable(pwm2_dev, PWM2_DEV_CHANNEL);
        rt_pwm_enable(pwm3_dev, -1 * PWM3_DEV_CHANNEL);
        rt_pwm_enable(pwm4_dev, -1 * PWM4_DEV_CHANNEL);
    }
}

int motor_init(void)
{
    /* step1�����Ҵ����豸 */
    uart1 = rt_device_find("uart2");
    /* step2�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
    rt_device_control(uart1, RT_DEVICE_CTRL_CONFIG, (void *)&uart1_config);

    /* step3���򿪴����豸�����жϽ��ռ���ѯ����ģʽ�򿪴����豸           �жϽ������� ==>> ֮��ɸ�Ϊdma*/
    rt_device_open(uart1, RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(uart1, rx_callback1);

    sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (sem == RT_NULL)
    {
        rt_kprintf("rt_sem_create failed...\n");
        return -1;
    }

    motor = rt_thread_create("motor_ctrl", motor_ctrl, RT_NULL, 1024, 9, 5);
    if (motor != RT_NULL)/* �������߳̿��ƿ飬��������߳� */
        rt_thread_startup(motor); // �����߳�1

    return RT_EOK;
}
MSH_CMD_EXPORT(motor_init, motor drive)
