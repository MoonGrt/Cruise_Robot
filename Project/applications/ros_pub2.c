//#include <rtthread.h>
//#include <rtdevice.h>
//#include <micro_ros_rtt.h>
//#include <stdio.h>
//#include <rcl/rcl.h>
//#include <rcl/error_handling.h>
//#include <rclc/rclc.h>
//#include <rclc/executor.h>
//
//#include <sensor_msgs/msg/laser_scan.h>
//#include <rmw_microros/time_sync.h>
//
//// 声明消息文件
//sensor_msgs__msg__LaserScan scan_msg;
//
//// 声明话题发布者
//rcl_publisher_t publisher;
//
//rclc_executor_t executor;
//rclc_support_t support;
//rcl_allocator_t allocator;
//rcl_node_t node;
//
//static void ros_pubth(void *parameter)
//{
//  // 发布数据初始化
//  scan_msg.header.frame_id.data = "laser";
//  scan_msg.header.frame_id.size = 5;
//
//  while (1)
//  {
//      if (!rmw_uros_epoch_synchronized()) // 判断时间是否同步
//      {
//        rmw_uros_sync_session(1000); //  同步时间
//        continue;
//      }
//    rcl_publish(&publisher, &scan_msg, NULL);
//    rt_thread_mdelay(100);
//  }
//}
//
//uint8_t ros_pub(int argc, char* argv[])
//{
//#if defined MICRO_ROS_USE_SERIAL // Serial setup
//  set_microros_transports();
//#endif
//#if defined MICRO_ROS_USE_UDP // UDP setup
//  if (argc == 2)
//    set_microros_udp_transports(argv[1], 9999);
//  else
//    set_microros_udp_transports("192.168.1.135", 9999);
//#endif
//
//  allocator = rcl_get_default_allocator();
//
//  // create init_options
//  if (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK)
//  {
//    rt_kprintf("[micro_ros] failed to initialize\n");
//    return;
//  };
//  // create node
//  if (rclc_node_init_default(&node, "ros_car", "", &support) != RCL_RET_OK)
//  {
//    rt_kprintf("[micro_ros] failed to create node\n");
//    return;
//  }
//
//  rclc_publisher_init_default(
//      &publisher,
//      &node,
//      ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, LaserScan),
//      "scan");
//
//
//  rt_thread_t ros_pub = rt_thread_create("ros_pub", ros_pubth, RT_NULL, 1024 * 2, 25, 10);
//  if (ros_pub != RT_NULL)
//    rt_thread_startup(ros_pub);
//  else
//    rt_kprintf("[micro_ros] Failed to create thread ros_pub\n");
//
//  rt_kprintf("[micro_ros] ros car init success\n");
//  return RT_EOK;
//}
//MSH_CMD_EXPORT(ros_pub, micro ros pub)
