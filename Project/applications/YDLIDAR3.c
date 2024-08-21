#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "drv_common.h"
#include "board.h"

#include <sensor_msgs/msg/laser_scan.h>
#include <rmw_microros/time_sync.h>

#define PI 3.1415926535
#define BUFF_LEN 320

rt_device_t uart5;
rt_sem_t sem_uart5,sem_decode;
uint8_t Rx_buffer_temp[90];
uint8_t Sensor_Data[90];
int LSA, FSA, data_len = 0;
extern sensor_msgs__msg__LaserScan scan_msg;
float data[40];
int64_t pub_time;

struct serial_configure uart5_config = {
    BAUD_RATE_115200,   /* 115200 bits/s */
    DATA_BITS_8,        /* 8 databits */
    STOP_BITS_1,        /* 1 stopbit */
    PARITY_NONE,        /* No parity  */
    BIT_ORDER_LSB,      /* LSB first sent */
    NRZ_NORMAL,         /* Normal mode */
    RT_SERIAL_RB_BUFSZ, /* Buffer size */
    0};

void YDLIDAR_Decode(void *parameter)
{
    scan_msg.ranges.data = (float *)rt_malloc(sizeof(float) * BUFF_LEN);
    scan_msg.ranges.capacity = BUFF_LEN;
    while(1)
    {
        rt_sem_take(sem_decode, RT_WAITING_FOREVER);
        uint8_t index = 0, len = Sensor_Data[3];

        if(Sensor_Data[2] & 0x01)
        {
//            rt_kprintf("%d\n", data_len);
            scan_msg.ranges.size = data_len;
            scan_msg.angle_increment = (float)360 / data_len / 180 * PI;
            data_len = 0;
            continue;
        }

        while (index < 40)
        {
            if (((Sensor_Data[index * 2 + 10] & 0x03) == 2) || ((Sensor_Data[index * 2 + 10] & 0x03) == 3))
                data[index] = 0.0;
            else
                data[index] = (float)((Sensor_Data[index * 2 + 10] >> 2) + (Sensor_Data[index * 2 + 11] << 6)) / 1000.0;
            index++;
        }

        if(data_len < BUFF_LEN)
            memcpy(&scan_msg.ranges.data[data_len], data, sizeof(float) * len);
        data_len += len;
    }
}

void YDLIDAR_Storage(uint8_t RxData)
{
    static uint8_t RecCmd_Step = 0, index = 0, data_len = 0;
    switch (RecCmd_Step)
    {
    case 0:
        if (RxData == 0xAA)
        {
            Rx_buffer_temp[index] = RxData;
            index++;
            RecCmd_Step = 1;
        }
        break;
    case 1:
        if (RxData == 0x55)
        {
            Rx_buffer_temp[index] = RxData;
            index++;
            RecCmd_Step = 2;
        }
        else
            RecCmd_Step = 0;
        break;
    case 2:
        Rx_buffer_temp[index] = RxData;
        index++;
        RecCmd_Step = 3;
        break;
    case 3:
        Rx_buffer_temp[index] = RxData;
        index++;
        data_len = RxData;
        RecCmd_Step = 4;
        break;
    case 4:
        Rx_buffer_temp[index] = RxData;
        index++;
        if (index == 10 + data_len * 2)
        {
            memcpy(Sensor_Data, Rx_buffer_temp, index);
            rt_sem_release(sem_decode);
            RecCmd_Step = index = 0;
        }
        break;
    default:
        break;
    }
}

rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(sem_uart5);
    return RT_EOK;
}

static void YDLIDAR_entry()
{
    uint8_t buffer;
    uart5 = rt_device_find("uart5");
    if (uart5 == RT_NULL)
        rt_kprintf("find uart5 failed\n");
    rt_device_control(uart5, RT_DEVICE_CTRL_CONFIG, (void *)&uart5_config);
    rt_device_open(uart5, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    rt_device_set_rx_indicate(uart5, rx_callback);

    rt_kprintf("YDLIDAR init success\n");
    while (1)
    {
        while (rt_device_read(uart5, 0, &buffer, 1) != 1)
            rt_sem_take(sem_uart5, RT_WAITING_FOREVER);
        YDLIDAR_Storage(buffer);
    }
}

int YDLIDAR_init(void)
{
    sem_uart5 = rt_sem_create("sem_uart5", 0, RT_IPC_FLAG_FIFO);
    if (sem_uart5 == RT_NULL)
        rt_kprintf("sem_uart5 failed...\n");

    sem_decode = rt_sem_create("sem_decode", 0, RT_IPC_FLAG_FIFO);
    if (sem_decode == RT_NULL)
        rt_kprintf("sem_decode failed...\n");

    rt_thread_t YDLIDAR_th = rt_thread_create("YDLIDAR", YDLIDAR_entry, NULL, 1024, 16, 5);
    if (YDLIDAR_th != RT_NULL)
        rt_thread_startup(YDLIDAR_th);

    rt_thread_t decoder_th = rt_thread_create("Decode", YDLIDAR_Decode, NULL, 1024, 16, 5);
    if (decoder_th != RT_NULL)
        rt_thread_startup(decoder_th);

    return RT_EOK;
}
MSH_CMD_EXPORT(YDLIDAR_init, YDLIDAR X3)