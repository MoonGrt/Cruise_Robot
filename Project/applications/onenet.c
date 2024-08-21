#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "onenet.h"

extern int tem, humi;
extern double aacx, aacy, aacz;
extern double gyrox, gyroy, gyroz;

void onenet_th(void *parameter)
{
    rt_thread_mdelay(5000);
    while (1)
    {
        if (onenet_mqtt_upload_digit("aacx", aacx) < 0 || onenet_mqtt_upload_digit("aacy", aacy) || onenet_mqtt_upload_digit("aacz", aacz) < 0)
            rt_kprintf("upload has an error\n");
        else
//            rt_kprintf("aacx: %f aacy: %f aacz: %f\n", aacx);
        if (onenet_mqtt_upload_digit("gyrox", gyrox) < 0 || onenet_mqtt_upload_digit("gyroy", gyroy) || onenet_mqtt_upload_digit("gyroz", gyroz) < 0)
            rt_kprintf("upload has an error\n");
        else
//            rt_kprintf("gyrox: %f gyroy: %f gyroz: %f\n", gyrox);
        if (onenet_mqtt_upload_digit("temperature", tem) < 0 || onenet_mqtt_upload_digit("humidity", humi) < 0)
            rt_kprintf("upload has an error\n");
        else
//            rt_kprintf("Tem: %d Hum: %d\n", tem);
        rt_thread_mdelay(500);
    }
}

uint8_t onenet_init(void)
{
    rt_thread_mdelay(1000);

    /* OneNET 连接初始化 */
//    rt_kprintf("start init onenet mqtt \n");
    if (onenet_mqtt_init() < 0)
        rt_kprintf("onenet_mqtt_init failed\n");
    else
        rt_kprintf("onenet_mqtt_init finished\n");

    rt_thread_t onenet = rt_thread_create("onenet", onenet_th, RT_NULL, 2 * 1024, 26, 5);
    /* 创建成功则启动线程 */
    if (onenet != RT_NULL)
        rt_thread_startup(onenet);
    else
        rt_kprintf("onenet create error\n");

    return RT_EOK;
}
MSH_CMD_EXPORT(onenet_init, onenet update);
