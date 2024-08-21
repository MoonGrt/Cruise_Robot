//#include <rtthread.h>
//#include <rtdevice.h>
//#include <micro_ros_rtt.h>
//#include <stdio.h>
//#include <rcl/rcl.h>
//#include <rcl/error_handling.h>
//#include <rclc/rclc.h>
//#include <rclc/executor.h>
//#include <std_msgs/msg/float32.h>
//
//// ������Ϣ�ļ�
//std_msgs__msg__Float32 pub_msg;
//
//// �������ⷢ����
//rcl_publisher_t publisher;
//
//rclc_executor_t executor;
//rclc_support_t support;
//rcl_allocator_t allocator;
//rcl_node_t node;
//
//static void pub_entry(void *parameter)
//{
//    while(1)
//    {
//        rt_thread_mdelay(500);
//        pub_msg.data = 15.1;
//        rcl_publish(&publisher, &pub_msg, NULL);
//    }
//}
//
//static void microros_pub(int argc, char* argv[]) // ����������ȷ���ڷ����ߺͶ�����֮��ʹ����ͬ���������ͣ�std_msgs__msg__Float32��
//{
//#if defined MICRO_ROS_USE_SERIAL // Serial setup
//     set_microros_transports();
//#endif
//#if defined MICRO_ROS_USE_UDP // UDP setup
//     if(argc==2)
//         set_microros_udp_transports(argv[1], 9999);
//     else
//         set_microros_udp_transports("192.168.1.135", 9999);
//#endif
//
//     // ��ʼ���ڴ������
//     allocator = rcl_get_default_allocator();
//     // ������ʼ��ѡ��
//     rclc_support_init(&support, 0, NULL, &allocator);
//     // �����ڵ� topic_sub_test
//     rclc_node_init_default(&node, "pub_float", "", &support);
//
//     // �����߳�ʼ��
//     rclc_publisher_init_default(
//         &publisher,
//         &node,
//         ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
//         "pub_float");
//
//     rt_thread_t thread = rt_thread_create("pubfloat", pub_entry, RT_NULL, 2048, 30, 10);
//     if(thread != RT_NULL)
//     {
//         rt_thread_startup(thread);
//         rt_kprintf("[micro_ros] New thread pubfloat\n");
//     }
//}
//MSH_CMD_EXPORT(microros_pub, micro ros pub)
