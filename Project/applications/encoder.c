#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdio.h>
#include <math.h>

#define HWTIMER_DEV_NAME   "timer13"         // 定时器名称
#define PULSE1_ENCODER_DEV_NAME    "pulse2"  // 脉冲编码器名称
#define PULSE2_ENCODER_DEV_NAME    "pulse3"  // 脉冲编码器名称

#define PERIOD_TIME  10      // 采样周期ms
#define WHEEL_DISTANCE 124                                              //124mm
#define PULSE_PER_REVOLUTION 1040   // 每转的脉冲数
#define GEAR_RATIO 20               // 减速比
#define WHEEL_DIAMETER_MM 48        // 轮胎直径（mm）
#define WHEEL_CIRCUMFERENCE_MM (WHEEL_DIAMETER_MM * 3.1415926535) // 轮胎周长(mm)
#define PULSE_COUNT_THRESHOLD 1040/2
#define DISTANCE_PER_PULSE_MM (WHEEL_CIRCUMFERENCE_MM / PULSE_PER_REVOLUTION) // 计算每脉冲对应的位移（mm）

rt_device_t pulse_encoder_r = RT_NULL, pulse_encoder_l = RT_NULL;   // 脉冲编码器设备句柄
double disp_x = 0.0, disp_y = 0.0, linear_sp = 0.0, angular_sp = 0.0;
double quat_x = 0.0, quat_y = 0.0, quat_z = 0.0, quat_w = 0.0, yaw = 0.0;

// 转换欧拉角为四元数
void quaternion_from_euler(double yaw, double pitch, double roll)
{
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    quat_w = cr * cp * cy + sr * sp * sy;
    quat_x = sr * cp * cy - cr * sp * sy;
    quat_y = cr * sp * cy + sr * cp * sy;
    quat_z = cr * cp * sy - sr * sp * cy;
}

/* 定时器超时回调函数 */
static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size)
{
    double disp = 0.0, disp_r = 0.0, disp_l = 0.0, sp_r = 0.0, sp_l = 0.0;
    rt_int32_t count_r = 0, count_l = 0;

    // 读取脉冲编码器计数值
    rt_device_read(pulse_encoder_l, 0, &count_l, 1);
    rt_device_read(pulse_encoder_r, 0, &count_r, 1);

    // 清空脉冲编码器计数值
    rt_device_control(pulse_encoder_l, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);
    rt_device_control(pulse_encoder_r, PULSE_ENCODER_CMD_CLEAR_COUNT, RT_NULL);

    // 如果脉冲计数值异常-则丢弃数据
    if (count_l <= -PULSE_COUNT_THRESHOLD || count_l >= PULSE_COUNT_THRESHOLD)
        return 0;
    if (count_r <= -PULSE_COUNT_THRESHOLD || count_r >= PULSE_COUNT_THRESHOLD)
        return 0;

    // 计算速度
    disp_l = count_l * DISTANCE_PER_PULSE_MM;
    disp_r = count_r * DISTANCE_PER_PULSE_MM;
    disp = (disp_l + disp_r) / 2.0;
    sp_l = disp_l / PERIOD_TIME;
    sp_r = disp_r / PERIOD_TIME;
    linear_sp = (sp_r + sp_l) / 2.0;
    angular_sp = (sp_r - sp_l) / WHEEL_DISTANCE * 1000;

    yaw += angular_sp * PERIOD_TIME / 1000;
    disp_x += disp * cos(yaw);
    disp_y += disp * sin(yaw);

    quaternion_from_euler(yaw, 0, 0);

    return 0;
}

// 定时周期初始化
static int hwtimer_init()
{
    rt_err_t ret = RT_EOK;
    rt_hwtimerval_t timeout_s;      // 定时器超时值
    rt_device_t hw_dev = RT_NULL;   // 定时器设备句柄
    rt_hwtimer_mode_t mode;         // 定时器模式

    // 查找定时器设备
    hw_dev = rt_device_find(HWTIMER_DEV_NAME);
    if (hw_dev == RT_NULL)
    {
        rt_kprintf("hwtimer sample run failed! can't find %s device!", HWTIMER_DEV_NAME);
        return RT_ERROR;
    }

    // 以读写方式打开设备
    ret = rt_device_open(hw_dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!", HWTIMER_DEV_NAME);
        return ret;
    }

    // 设置超时回调函数
    rt_device_set_rx_indicate(hw_dev, timeout_cb);

    // 设置模式为周期性定时器
    mode = HWTIMER_MODE_PERIOD;
    ret = rt_device_control(hw_dev, HWTIMER_CTRL_MODE_SET, &mode);
    if (ret != RT_EOK)
    {
        rt_kprintf("set mode failed! ret is :%d", ret);
        return ret;
    }

    // 设置定时器超时值为指定周期
    timeout_s.sec = 0;      // 秒
    timeout_s.usec = PERIOD_TIME * 1000;     // 微秒

    if (rt_device_write(hw_dev, 0, &timeout_s, sizeof(timeout_s)) != sizeof(timeout_s))
    {
        rt_kprintf("set timeout value failed");
        return RT_ERROR;
    }

//    // 延时一段时间
//    rt_thread_mdelay(500);
//
//    // 读取定时器当前值
//    rt_device_read(hw_dev, 0, &timeout_s, sizeof(timeout_s));
//    LOG_D("Read: Sec = %d, Usec = %d\n", timeout_s.sec, timeout_s.usec);

    return ret;
}

void displacement_en(void *parameter)
{
    rt_thread_mdelay(500);
    while(1)
    {
        printf("disp:%.3fm %.3fm\n", disp_x / 1000, disp_y / 1000);
        printf("sp:%.2fm %.2fm\n", linear_sp, angular_sp);
        printf("quat:%.2f %.2f %.2f %.2f %.2f\n", quat_w, quat_x, quat_y, quat_z, yaw);
        rt_kprintf("\n");
        rt_thread_mdelay(1000);
    }
}

uint8_t encoder_init(void)
{
    rt_err_t ret = RT_EOK;

    // 查找脉冲编码器设备
    pulse_encoder_l = rt_device_find(PULSE2_ENCODER_DEV_NAME);
    if (pulse_encoder_l == RT_NULL)
    {
        rt_kprintf("pulse encoder sample run failed! can't find %s device!", PULSE2_ENCODER_DEV_NAME);
        return RT_ERROR;
    }

    // 以只读方式打开设备
    ret = rt_device_open(pulse_encoder_l, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", PULSE2_ENCODER_DEV_NAME);
        return ret;
    }

    // 查找脉冲编码器设备
    pulse_encoder_r = rt_device_find(PULSE1_ENCODER_DEV_NAME);
    if (pulse_encoder_r == RT_NULL)
    {
        rt_kprintf("pulse encoder sample run failed! can't find %s device!", PULSE1_ENCODER_DEV_NAME);
        return RT_ERROR;
    }

    // 以只读方式打开设备
    ret = rt_device_open(pulse_encoder_r, RT_DEVICE_OFLAG_RDONLY);
    if (ret != RT_EOK)
    {
        rt_kprintf("open %s device failed!\n", PULSE1_ENCODER_DEV_NAME);
        return ret;
    }

//    rt_thread_t displacement = rt_thread_create("displacement", displacement_en, RT_NULL, 1024, 15, 10);
//    if (displacement == RT_NULL)
//        return RT_ERROR;
//    else
//        rt_thread_startup(displacement);

    hwtimer_init();
    rt_kprintf("encoder init success\n");
    return RT_EOK;
}
MSH_CMD_EXPORT(encoder_init, encoder driver init);
