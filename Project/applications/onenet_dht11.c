//#include <rtthread.h>
//#include <rtdevice.h>
//#include "drv_common.h"
//#include "onenet.h"
//
//rt_thread_t onenet_update_thread = RT_NULL;
//extern int tem, humi;
//
//static void onenet_mqtt_update_data_entry(void *parameter)
//{
//    /* OneNET 连接初始化 */
//    rt_kprintf("start init onenet mqtt \n");
//    if (onenet_mqtt_init() < 0)
//        rt_kprintf("onenet_mqtt_init failed\n");
//    else
//        rt_kprintf("onenet_mqtt_init finished\n");
//
//    while (1)
//    {
//        if (onenet_mqtt_upload_digit("temperature", tem) < 0 || onenet_mqtt_upload_digit("Humidity", humi) < 0)
//            rt_kprintf("upload has an error\n");
//        else
//        {
//            rt_kprintf("Temperature: %d\n", tem);
//            rt_kprintf("Humidity: %d\n", humi);
//        }
//        rt_thread_mdelay(1000);
//    }
//}
//
//static onenet_open_dht11()
//{
//    rt_thread_mdelay(1000);
//
//    onenet_update_thread = rt_thread_create("onenet", onenet_mqtt_update_data_entry, RT_NULL, 2 * 1024, 26, 5);
//    /* 创建成功则启动线程 */
//    if (onenet_update_thread != RT_NULL)
//        rt_thread_startup(onenet_update_thread);
//    else
//        rt_kprintf("onenet_update_thread create error\n");
//
//    return RT_EOK;
//}
//MSH_CMD_EXPORT(onenet_open_dht11, onenet open);
