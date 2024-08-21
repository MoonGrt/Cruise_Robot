//#include <rtthread.h>
//#include <rtdevice.h>
//#include "drv_common.h"
//#include "drv_spi_ili9488.h"
//#include <lcd_spi_port.h>
//
//struct rt_event ov2640_event;
//extern int camera_sample();
//
//static void camera_then(void *parameter)
//{
//    /* init spi data notify event */
//    rt_event_init(&ov2640_event, "ov2640", RT_IPC_FLAG_FIFO);
//    struct drv_lcd_device *lcd;
//    lcd = (struct drv_lcd_device *)rt_device_find("lcd");
//    struct rt_device_rect_info rect_info = {0, 0, LCD_WIDTH, 240};
//
//    camera_sample();
//
//    while(1)
//    {
//        rt_event_recv(&ov2640_event,
//                            1,
//                            RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,
//                            RT_WAITING_FOREVER,
//                            RT_NULL);
//        lcd->parent.control(&lcd->parent, RTGRAPHIC_CTRL_RECT_UPDATE, &rect_info);
//    }
//}
//
//uint8_t camera_init(void)
//{
//    rt_thread_t camera_th = rt_thread_create("ros_scanpub", camera_then, RT_NULL, 1024 * 2, 24, 10);
//    if (camera_th != RT_NULL)
//      rt_thread_startup(camera_th);
//    else
//      rt_kprintf("[micro_ros] Failed to create thread camera_th\n");
//
//    return RT_EOK;
//}
//MSH_CMD_EXPORT(camera_init, camera driver init);
