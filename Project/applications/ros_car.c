#include <micro_ros_rtt.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <inttypes.h>

#include <rmw_microros/time_sync.h>
#include <sensor_msgs/msg/laser_scan.h>
#include <geometry_msgs/msg/twist.h> //twist类型对应的头文件,twist
#include <nav_msgs/msg/odometry.h>
#include <sensor_msgs/msg/image.h>
#include <sensor_msgs/msg/imu.h>
#include <std_msgs/msg/u_int64.h>

#define PI 3.1415926535
#define TIME_OFFSET 1690553343+60
// 时间戳     https://tool.lu/timestamp/

rt_sem_t sem_ros;

rclc_executor_t executor,executor1;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

geometry_msgs__msg__Twist sub_msg_twist; // sub_msg_twist：twist类型
sensor_msgs__msg__LaserScan scan_msg;
sensor_msgs__msg__Image image_msg;
nav_msgs__msg__Odometry odom_msg;
sensor_msgs__msg__Imu imu_msg;
std_msgs__msg__UInt64 time_msg;

// 声明话题订阅者
rcl_subscription_t sub_twist,sub_time;
// 声明话题发布者
rcl_publisher_t pub_odom, pub_laser, pub_imu;

extern float q0, q1, q2, q3;
extern float gyrox, gyroy, gyroz; // 陀螺仪原始数据
extern float aacx, aacy, aacz;     // 加速度传感器原始数据
extern double disp_x, disp_y, linear_sp, angular_sp;
extern double quat_x, quat_y, quat_z, quat_w;
double linear_x, angular_z;
int64_t pub_time;
int LSA, FSA;

static void ros_subth(void *parameter)
{
    while (1)
    {
        rt_thread_mdelay(100);
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
        rclc_executor_spin_some(&executor1, RCL_MS_TO_NS(100));
    }
}

// twist message
void sub_callback(const void *msgin)
{
    // Please remember to enable -u_printf_float in compiler settings
    const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;
    printf("linear  x:%f y: %f z: %f\n", msg->linear.x, msg->linear.y, msg->linear.z);
    printf("angular x:%f y: %f z: %f\n", msg->angular.x, msg->angular.y, msg->angular.z);
    linear_x = msg->linear.x;
    angular_z = msg->angular.z;
    rt_sem_release(sem_ros);
}

// twist message
void sub_callback1(const void *msgin)
{
    // Please remember to enable -u_printf_float in compiler settings
    const std_msgs__msg__UInt64 *msg = (const std_msgs__msg__UInt64 *)msgin;
//    printf("timestamp:%" PRIu64 "\n", msg->data); // 使用printf函数打印uint64_t类型的整数值     头文件     #include <inttypes.h>
}

void MPU6050_Decode(void)
{
    imu_msg.orientation.w = (double)q0;
    imu_msg.orientation.x = (double)q1;
    imu_msg.orientation.y = (double)q2;
    imu_msg.orientation.z = (double)q3;
    imu_msg.angular_velocity.x = gyrox;
    imu_msg.angular_velocity.y = gyroy;
    imu_msg.angular_velocity.z = gyroz;
    imu_msg.linear_acceleration.x = aacx;
    imu_msg.linear_acceleration.y = aacy;
    imu_msg.linear_acceleration.z = aacz;
    imu_msg.header.stamp.sec = time_msg.data / 1000;
    imu_msg.header.stamp.nanosec = pub_time * 1000 * 1000;
}

void Odometry_Decode(void)
{
    odom_msg.twist.twist.linear.x = linear_sp;
    odom_msg.twist.twist.linear.y = 0;
    odom_msg.twist.twist.linear.z = 0;
    odom_msg.twist.twist.angular.x = 0;
    odom_msg.twist.twist.angular.y = 0;
    odom_msg.twist.twist.angular.z = angular_sp;
    odom_msg.pose.pose.position.x = disp_x / 1000;
    odom_msg.pose.pose.position.y = disp_y / 1000;
    odom_msg.pose.pose.position.z = 0;
    odom_msg.pose.pose.orientation.x = quat_x;
    odom_msg.pose.pose.orientation.y = quat_y;
    odom_msg.pose.pose.orientation.z = quat_z;
    odom_msg.pose.pose.orientation.w = quat_w;
    odom_msg.header.stamp.sec = time_msg.data / 1000;
    odom_msg.header.stamp.nanosec = pub_time * 1000 * 1000;
}

void Laser_Decode(void)
{
    scan_msg.header.stamp.sec = time_msg.data / 1000;
    scan_msg.header.stamp.nanosec = pub_time * 1000 * 1000;
}

static void ros_pubth(void *parameter)
{
    // 发布数据初始化
    odom_msg.header.frame_id.data = "odom";
    odom_msg.header.frame_id.size = 4;
    odom_msg.child_frame_id.data = "base_footprint";
    odom_msg.child_frame_id.size = 14;

    scan_msg.header.frame_id.data = "laser_link";
    scan_msg.header.frame_id.size = 10;
    scan_msg.time_increment = 0.0;
    scan_msg.scan_time = 0.0;
    scan_msg.range_min = 0.12;
    scan_msg.range_max = 3.5;
    scan_msg.angle_min = 0;
    scan_msg.angle_max = 360 / 180 * PI;
    scan_msg.angle_increment = (float)360 / 320 / 180 * PI;

//    imu_msg.header.frame_id.data = "imu";
//    imu_msg.header.frame_id.size = 3;

    while (1)
    {
//        if (!rmw_uros_epoch_synchronized()) // 判断时间是否同步
//        {
//            rmw_uros_sync_session(1000); //  同步时间
//            continue;
//        }

        pub_time = rt_tick_get() % 1000;
        Odometry_Decode();
        Laser_Decode();
        rcl_publish(&pub_odom, &odom_msg, NULL);
        rcl_publish(&pub_laser, &scan_msg, NULL);
        rt_thread_mdelay(100);
    }
}

uint8_t ros_car(int argc, char *argv[])
{
#if defined MICRO_ROS_USE_SERIAL // Serial setup
    set_microros_transports();
#endif
#if defined MICRO_ROS_USE_UDP // UDP setup
    if (argc == 2)
        set_microros_udp_transports(argv[1], 9999);
    else
        set_microros_udp_transports("192.168.1.135", 9999);
#endif

    allocator = rcl_get_default_allocator();

    // create init_options
    if (rclc_support_init(&support, 0, NULL, &allocator) != RCL_RET_OK)
    {
        rt_kprintf("[micro_ros] failed to initialize\n");
        return -1;
    };
    // create node
    if (rclc_node_init_default(&node, "ros_car", "", &support) != RCL_RET_OK)
    {
        rt_kprintf("[micro_ros] failed to create node\n");
        return -1;
    }

    // create subscriber
    rclc_subscription_init_default(
        &sub_twist,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
        "cmd_vel");
    // create executor
    rclc_executor_init(&executor, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor, &sub_twist, &sub_msg_twist, &sub_callback, ON_NEW_DATA);

    // create subscriber
    rclc_subscription_init_default(
        &sub_time,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, UInt64),
        "timestamp");
    // create executor
    rclc_executor_init(&executor1, &support.context, 1, &allocator);
    rclc_executor_add_subscription(&executor1, &sub_time, &time_msg, &sub_callback1, ON_NEW_DATA);

    // 发布者初始化
    rclc_publisher_init_default(
        &pub_odom,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(nav_msgs, msg, Odometry),
        "odom");
    rclc_publisher_init_default(
        &pub_laser,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, LaserScan),
        "scan");
    rclc_publisher_init_default(
        &pub_imu,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Imu),
        "imu");

    sem_ros = rt_sem_create("sem_ros", 0, RT_IPC_FLAG_FIFO);
    if (sem_ros == RT_NULL)
        rt_kprintf("sem_ros failed...\n");

    rt_thread_t ros_sub = rt_thread_create("ros_sub", ros_subth, RT_NULL, 1024 * 2, 16, 10);
    if (ros_sub != RT_NULL)
        rt_thread_startup(ros_sub);
    else
        rt_kprintf("[micro_ros] Failed to create thread ros_car_sub\n");

    rt_thread_t ros_pub = rt_thread_create("ros_pub", ros_pubth, RT_NULL, 1024 * 2, 20, 10);
    if (ros_pub != RT_NULL)
        rt_thread_startup(ros_pub);
    else
        rt_kprintf("[micro_ros] Failed to create thread ros_pub\n");

    rt_kprintf("[micro_ros] ros car init success\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(ros_car, micro ros car)
