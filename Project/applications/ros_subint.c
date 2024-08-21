//#include <rtthread.h>
//#include <rtdevice.h>
//#include <micro_ros_rtt.h>
//#include "drv_common.h"
//#include <stdio.h>
//#include <rcl/rcl.h>
//#include <rcl/error_handling.h>
//#include <rclc/rclc.h>
//#include <rclc/executor.h>
//
//#include <std_msgs/msg/int32.h>
//
//rcl_subscription_t subscriber;
//std_msgs__msg__Int32 msg;
//rclc_executor_t executor;
//rclc_support_t support;
//rcl_allocator_t allocator;
//rcl_node_t node;
//
//static void sub_callback(const void *msgin)
//{
//    const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
//    rt_kprintf("[micro_ros] received data %d\n", msg->data);
//}
//
//static void subint_th(void *parameter)
//{
//    while (1)
//    {
//        rt_thread_mdelay(10);
//        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
//    }
//}
//
//static void microros_subint(int argc, char *argv[])
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
//    // 初始化内存分配器
//    allocator = rcl_get_default_allocator();
//
//    // 创建初始化选项
//    if (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK)
//    {
//        rt_kprintf("[micro_ros] failed to initialize\n");
//        return;
//    };
//
//    // 创建节点
//    if (rclc_node_init_default(&node, "subint", "", &support) != RCL_RET_OK)
//    {
//        rt_kprintf("[micro_ros] failed to create node\n");
//        return;
//    }
//    rt_kprintf("[micro_ros] node created\n");
//
//    // create subscriber
//    rclc_subscription_init_default(
//        &subscriber,
//        &node,
//        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//        "subint32");
//
//    // create executor
//    rclc_executor_init(&executor, &support.context, 1, &allocator);
//    rclc_executor_add_subscription(&executor, &subscriber, &msg, &sub_callback, ON_NEW_DATA);
//    rt_kprintf("[micro_ros] executor created\n");
//
//    rt_thread_t thread = rt_thread_create("subint", subint_th, RT_NULL, 2048, 30, 10);
//    if (thread != RT_NULL)
//    {
//        rt_thread_startup(thread);
//        rt_kprintf("[micro_ros] New thread subint\n");
//    }
//    else
//        rt_kprintf("[micro_ros] Failed to create thread subint\n");
//
//    // ros2 topic pub --once /subint32 std_msgs/msg/Int32 data:\ 0
//}
//MSH_CMD_EXPORT(microros_subint, micro ros subscribe int32)
