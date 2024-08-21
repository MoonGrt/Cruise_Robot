#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define HWTIMER_DEV_NAME "timer13" /* ��ʱ������ */
#define PERIOD_TIME  500  //��������ms t

/* ��ʱ����ʱ�ص����� */
static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size)
{
    rt_kprintf("hello world\n");
    return 0;
}

// ��ʱ���ڳ�ʼ��
static int hwtimer_init()
{
    rt_err_t ret = RT_EOK;
    rt_hwtimerval_t timeout_s;    /* ��ʱ����ʱֵ */
    rt_device_t hw_dev = RT_NULL; /* ��ʱ���豸��� */
    rt_hwtimer_mode_t mode;       /* ��ʱ��ģʽ */

    /* ���Ҷ�ʱ���豸 */
    hw_dev = rt_device_find(HWTIMER_DEV_NAME);
    if (hw_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find %s device!", HWTIMER_DEV_NAME);
        return RT_ERROR;
    }

    /* �Զ�д��ʽ���豸 */
    ret = rt_device_open(hw_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!", HWTIMER_DEV_NAME);
        return ret;
    }

    /* ���ó�ʱ�ص����� */
    rt_device_set_rx_indicate(hw_dev, timeout_cb);

    /* ����ģʽΪ�����Զ�ʱ�� */
    mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d", ret);
        return ret;
    }

    /* ���ö�ʱ����ʱֵΪ5s��������ʱ�� */
    timeout_s.sec = 0;                   /* �� */
    timeout_s.usec = PERIOD_TIME * 1000; /* ΢�� */

    if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("set timeout value failed");
        return RT_ERROR;
    }

    /* ��ʱms */
    rt_thread_mdelay(500);

    /* ��ȡ��ʱ����ǰֵ */
    rt_device_read(hw_dev, 0, &timeout_s, sizeof(timeout_s));
    rt_kprintf("Read: Sec = %d, Usec = %d\n", timeout_s.sec, timeout_s.usec);

    return ret;
}
MSH_CMD_EXPORT(hwtimer_init, hwtimer init);
