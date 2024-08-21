//#include <rtthread.h>
//#include <rtdevice.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include "drv_common.h"
//#include "board.h"
//
//static rt_device_t uart1;
//static rt_sem_t sem_uart1;
//rt_thread_t YDLIDAR_th = NULL;
//uint8_t send_buf[18] = {'\0'};
//uint8_t Rx_buffer_temp[18];
//uint8_t Rx_buffer_ok[18];
//uint8_t Sensor_Data[18];
//
//uint16_t dis = 0;
//uint8_t engy = 0, noise = 0;
//
//struct serial_configure uart1_config = {
//    BAUD_RATE_115200,   /* 115200 bits/s */
//    DATA_BITS_8,        /* 8 databits */
//    STOP_BITS_1,        /* 1 stopbit */
//    PARITY_NONE,        /* No parity  */
//    BIT_ORDER_LSB,      /* LSB first sent */
//    NRZ_NORMAL,         /* Normal mode */
//    RT_SERIAL_RB_BUFSZ, /* Buffer size */
//    0};
//
//// ��������
//uint8_t countsum(uint8_t *buf)
//{
//    uint8_t len = 0;
//    uint8_t checksum = 0;
//    len = sizeof(buf) + 1;
//    while (len--)
//    {
//        checksum += *buf;
//        buf++;
//    }
//
//    // ���������λ
//    checksum &= 0xFF;
//
//    return checksum;
//}
//
//// ���ò�����
//// 230400��460800��512000��921600��1500000 �ֱ��Ӧ���� 0-4��Ĭ��Ϊ 460800��
//void SMD15_setbaudrate(uint32_t baudrate)
//{
//    uint8_t baud;
//    // �����״��ʼ�� 230400��460800��512000��921600��1500000
//    if (baudrate == 230400)
//        baud = 0;
//    else if (baudrate == 460800)
//        baud = 1;
//    else if (baudrate == 512000)
//        baud = 2;
//    else if (baudrate == 921600)
//        baud = 3; // ����stm32��ߵĲ����ʣ�������4.5M,���ɴﵽ���9.2MHz
//    else if (baudrate == 1500000)
//        baud = 4;
//
//    send_buf[0] = 0xAA;
//    send_buf[1] = 0x55;
//    send_buf[2] = 0x66;
//    send_buf[3] = 0x01;
//    switch (baud)
//    {
//    case 0:
//        send_buf[4] = 0x00;
//        break;
//    case 1:
//        send_buf[4] = 0x01;
//        break;
//    case 2:
//        send_buf[4] = 0x02;
//        break;
//    case 3:
//        send_buf[4] = 0x03;
//        break;
//    case 4:
//        send_buf[4] = 0x04;
//        break;
//    default:
//        break;
//    }
//    send_buf[5] = countsum(send_buf);
//
//    rt_device_write(uart1, 0, send_buf, 6);
//    memset(send_buf, 0, sizeof(send_buf));
//}
//
//// �������Ƶ��
//// 10hz��100hz��200hz��500hz��1000hz��1800hz ���Ƶ�ʣ��ֱ��Ӧ���� 0-5��Ĭ��Ϊ100hz��
//void SMD15_setScanfHZ(uint8_t hz)
//{
//    send_buf[0] = 0xAA;
//    send_buf[1] = 0x55;
//    send_buf[2] = 0x64;
//    send_buf[3] = 0x01;
//
//    switch (hz)
//    {
//    case 0:
//        send_buf[4] = 0x00;
//        break;
//    case 1:
//        send_buf[4] = 0x01;
//        break;
//    case 2:
//        send_buf[4] = 0x02;
//        break;
//    case 3:
//        send_buf[4] = 0x03;
//        break;
//    case 4:
//        send_buf[4] = 0x04;
//        break;
//    case 5:
//        send_buf[4] = 0x05;
//        break;
//    default:
//        break;
//    }
//    send_buf[5] = countsum(send_buf);
//
//    rt_device_write(uart1, 0, send_buf, 6);
//    memset(send_buf, 0, sizeof(send_buf));
//}
//
//// ��ʼɨ��
//void start_scan(void)
//{
//    send_buf[0] = 0xAA;
//    send_buf[1] = 0x55;
//    send_buf[2] = 0x60;
//    send_buf[3] = 0x00;
//    send_buf[4] = 0x5F;
//
//    rt_device_write(uart1, 0, send_buf, 5);
//    memset(send_buf, 0, sizeof(send_buf));
//}
//
//// ֹͣɨ��
//void stop_scan(void)
//{
//    send_buf[0] = 0xAA;
//    send_buf[1] = 0x55;
//    send_buf[2] = 0x61;
//    send_buf[3] = 0x00;
//    send_buf[4] = 0x60;
//
//    rt_device_write(uart1, 0, send_buf, 5);
//    memset(send_buf, 0, sizeof(send_buf));
//}
//
//// ���ջص�����
//rt_err_t rx_callback(rt_device_t dev, rt_size_t size)
//{
//    rt_sem_release(sem_uart1);
//    return RT_EOK;
//}
//
//// �жϽ��յ������ݰ��Ƿ�����
//void SDM15_Decode(uint8_t RxData)
//{
//    static uint8_t RecCmd_Step = 0, Checksum = 0, RecCmd_Data_len = 0, Data_cnt = 0;
//    switch (RecCmd_Step)
//    {
//    case 0:
//        if (RxData == 0xAA)
//        {
//            Rx_buffer_temp[0] = RxData;
//            RecCmd_Step++;
//        }
//        Checksum = 0xAA;
//        break;
//
//    case 1:
//        if (RxData == 0x55)
//        {
//            Rx_buffer_temp[1] = RxData;
//            RecCmd_Step++;
//            Data_cnt = 0;
//            Checksum += RxData;
//            RecCmd_Data_len = 0;
//        }
//        else
//            RecCmd_Step = 0;
//        break;
//
//    case 2:
//        Rx_buffer_temp[2] = RxData;
//        Checksum += RxData;
//        RecCmd_Step++;
//        break;
//
//    case 3:
//        Rx_buffer_temp[3] = RxData;
//        Checksum += RxData;
//        RecCmd_Data_len = Rx_buffer_temp[3];
//        RecCmd_Step = RecCmd_Data_len == 0 ? 5 : RecCmd_Step + 1;
//        break;
//
//    case 4:
//        if (Data_cnt < RecCmd_Data_len)
//        {
//            Rx_buffer_temp[4 + Data_cnt++] = RxData;
//            Checksum += RxData;
//            if (Data_cnt >= RecCmd_Data_len)
//                RecCmd_Step++;
//        }
//        break;
//
//    case 5:
//        if (Checksum == RxData)
//        {
//            memcpy(Sensor_Data, &Rx_buffer_temp[0], 18);
//            memset(Rx_buffer_temp, 0, 18); // ��һ������
//            RecCmd_Step = 0;
//            Data_cnt = 0;
//            Checksum = 0;
//        }
//        else
//        {
//            RecCmd_Step = 0;
//            Data_cnt = 0;
//            Checksum = 0;
//            memset(Rx_buffer_temp, 0, 18); // ��һ������
//        }
//        break;
//
//    default:
//        break;
//    }
//}
//
//// ������롢ǿ�ȡ�����
//uint8_t print_message(void)
//{
//    if (Sensor_Data[0] == '\0')
//        return 0; // û��Ϣ����
//
//    if (Sensor_Data[3] == 0x00) // û���ݳ���Ҳ����
//    {
//        memset(Sensor_Data, 0, 18); // ��һ������
//        return 0;
//    }
//    dis = Sensor_Data[5] << 8 | Sensor_Data[4];
//    engy = Sensor_Data[6];
//    noise = Sensor_Data[7];
//    rt_kprintf("dis:%d, engy:%d, noise:%d\r\n", dis, engy, noise);
//    return 1;
//}
//
//static void YDLIDAR_entry()
//{
//    uint8_t buffer;
//
//    /* step1�����Ҵ����豸 */
//    uart1 = rt_device_find("uart1");
//    if (uart1 == RT_NULL)
//        rt_kprintf("find uart1 failed\n");
//    /* step2�����ƴ����豸��ͨ�����ƽӿڴ�����������֣�����Ʋ��� */
//    rt_device_control(uart1, RT_DEVICE_CTRL_CONFIG, (void *)&uart1_config);
//    /* step3���򿪴����豸�����жϽ��ռ���ѯ����ģʽ�򿪴����豸           �жϽ������� ==>> ֮��ɸ�Ϊdma*/
//    rt_device_open(uart1, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
//    rt_device_set_rx_indicate(uart1, rx_callback);
//
//    rt_thread_mdelay(200);
//    //    stop_scan();//ֹͣɨ��
//    SMD15_setbaudrate(460800); // ���ò����ʣ�Ĭ��Ϊ460800���ı䲨�����������״�
//    //    SMD15_setScanfHZ(1); //����ɨ��Ƶ��
//    start_scan();
//    while (1)
//    {
//        while (rt_device_read(uart1, 0, &buffer, 1) != 1) /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
//            rt_sem_take(sem_uart1, RT_WAITING_FOREVER);         /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
//        SDM15_Decode(buffer);
//    }
//}
//
//int YDLIDAR(void)
//{
//    sem_uart1 = rt_sem_create("sem_uart1", 0, RT_IPC_FLAG_FIFO);
//    if (sem_uart1 == RT_NULL)
//    {
//        rt_kprintf("sem_YDLIDAR failed...\n");
//        return -1;
//    }
//
//    YDLIDAR_th = rt_thread_create("YDLIDAR", YDLIDAR_entry, NULL, 1024*8, 20, 5);
//    if (YDLIDAR_th != RT_NULL)
//        rt_thread_startup(YDLIDAR_th);
//
//    while (1)
//    {
//        rt_thread_mdelay(200);
////        print_message(); // ��ӡ��Ϣ
//        dis = Sensor_Data[5] << 8 | Sensor_Data[4];
//    }
//
//    return RT_EOK;
//}
//MSH_CMD_EXPORT(YDLIDAR, YDLIDAR SDM15)
