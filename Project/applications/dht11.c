#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "sensor.h"
#include "sensor_dallas_dht11.h"
#include "drv_gpio.h"

#define DHT11_DATA_PIN GET_PIN(G, 7)
int tem = 0;
int humi = 0;

static void dht11_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;
    rt_uint8_t get_data_freq = 1; /* 1Hz */

    dev = rt_device_find("temp_dht11");
    if (dev == RT_NULL)
        return;

    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }

    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)(&get_data_freq));
    rt_kprintf("dht11 init success!\n");

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed! result is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
            if (sensor_data.data.temp >= 0)
            {
                tem = (sensor_data.data.temp & 0xffff) >> 0;        // get temp
                humi = (sensor_data.data.temp & 0xffff0000) >> 16; // get humi
//                rt_kprintf("temp:%d, humi:%d\n", tem, humi);
            }
        rt_thread_mdelay(1000);
    }
}

uint8_t dht11_initial(void)
{
    rt_thread_mdelay(1000);
    struct rt_sensor_config cfg;
    cfg.intf.user_data = (void *)DHT11_DATA_PIN;
    rt_hw_dht11_init("dht11", &cfg);

    rt_thread_t dht11_thread = rt_thread_create("dht11", dht11_entry, RT_NULL, 1024, 20, 5);
    /* �����ɹ��������߳� */
    if (dht11_thread != RT_NULL)
        rt_thread_startup(dht11_thread);
    else
        rt_kprintf("dht11_thread create error\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(dht11_initial, dht11 driver init);
