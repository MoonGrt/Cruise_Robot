//#include <rtthread.h>
//#include <rtdevice.h>
//#include <micro_ros_rtt.h>
//#include <stdio.h>
//#include <rcl/rcl.h>
//#include <rcl/error_handling.h>
//#include <rclc/rclc.h>
//#include <rclc/executor.h>
//#include <lcd_spi_port.h>
//
//#include <sensor_msgs/msg/image.h>
//
//// ������Ϣ�ļ�
//sensor_msgs__msg__Image image_msg;
//
//// �������ⷢ����
//rcl_publisher_t publisher;
//
//rclc_executor_t executor;
//rclc_support_t support;
//rcl_allocator_t allocator;
//rcl_node_t node;
//rcl_timer_t timer;
//
//extern struct drv_lcd_device _lcd;
//struct rt_event gc0328c_event;
//
//static void pub_entry(void *parameter)
//{
//    while (1)
//    {
//        rt_event_recv(&gc0328c_event, 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
//        for (uint32_t i = 0; i < 240; i++)
//            memcpy(&image_msg.data.data[i * LCD_BYTES_PER_PIXEL * LCD_WIDTH], &_lcd.lcd_info.framebuffer[i * LCD_BYTES_PER_PIXEL * LCD_WIDTH], LCD_WIDTH * LCD_BYTES_PER_PIXEL);
//        rcl_publish(&publisher, &image_msg, NULL);
//    }
//}
//
//static void ros_image(int argc, char *argv[]) // ����������ȷ���ڷ����ߺͶ�����֮��ʹ����ͬ���������ͣ�std_msgs__msg__Float32��
//{
//#if defined MICRO_ROS_USE_SERIAL // Serial setup
//    set_microros_transports();
//#endif
//#if defined MICRO_ROS_USE_UDP // UDP setup
//    if (argc == 2)
//        set_microros_udp_transports(argv[1], 9999);
//    else
//        set_microros_udp_transports("192.168.1.135", 9999);
//#endif
//
//    // ��ʼ���ڴ������
//    allocator = rcl_get_default_allocator();
//    // ������ʼ��ѡ��
//    rclc_support_init(&support, 0, NULL, &allocator);
//    // �����ڵ�
//    rclc_node_init_default(&node, "pub", "", &support);
//
//    // �����߳�ʼ��
//    rclc_publisher_init_default(
//        &publisher,
//        &node,
//        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Image),
//        "pub_image");
//
//    // �������ݳ�ʼ��
//    image_msg.header.frame_id.data = "image";
//    image_msg.header.frame_id.size = 5;
//    image_msg.height = 240;
//    image_msg.width = LCD_WIDTH;
//    image_msg.encoding.data = "rgb8"; // RGB 8-bit encoding
//    image_msg.is_bigendian = false; // ��˻���С��
//    image_msg.step = LCD_WIDTH * LCD_BYTES_PER_PIXEL; // ȫ�г���(�ֽ�)
//    image_msg.data.data = (uint8_t *)rt_malloc(sizeof(uint8_t) * LCD_WIDTH * 240 * LCD_BYTES_PER_PIXEL);
//    image_msg.data.size = sizeof(uint8_t) * LCD_WIDTH * 240 * LCD_BYTES_PER_PIXEL;
//
//    /* init spi data notify event */
//    rt_event_init(&gc0328c_event, "gc0328c", RT_IPC_FLAG_FIFO);
//
//    rt_thread_t thread = rt_thread_create("pubimage", pub_entry, RT_NULL, 2048, 20, 10);
//    if (thread != RT_NULL)
//        rt_thread_startup(thread);
//}
//MSH_CMD_EXPORT(ros_image, micro ros pub)
