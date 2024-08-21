#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <finsh.h>
#include <stdio.h>

// #define MD_DEBUG
#define LOG_TAG "md.example"
#include "md_log.h"

#include "MD_Ported_to_RTT.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mltypes.h"
#include "mpu.h"
#include "log.h"
#include "packet.h"

#define G_CD 9.7843
#define dt 10

double aacx = 0, aacy = 0, aacz = 0;     // ���ٶȴ�����ԭʼ����
double linear_velocity_x = 0, linear_velocity_y = 0, linear_velocity_z = 0;
double gyrox = 0, gyroy = 0, gyroz = 0; // ������ԭʼ����
double angular_velocity_x = 0, angular_velocity_y = 0, angular_velocity_z = 0;
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
double position_x = 0, position_y = 0, position_z = 0;

/* Private typedef -----------------------------------------------------------*/
/* The bus name of the mpu. */
#define RT_MPU_DEVICE_NAME "i2c2"
unsigned char *mpl_key = (unsigned char *)"eMPL 5.1";
extern struct rt_mpu_device *mpu_dev;
extern int mpu_dev_init_flag; /* Flag to show if the mpu device is inited. */

// q30��q16��ʽ,longתfloatʱ�ĳ���.
#define q30 1073741824.0f
#define q16 65536.0f

// �����Ƿ�������
static signed char gyro_orientation[9] = {1, 0, 0,
                                          0, 1, 0,
                                          0, 0, 1};

/**
 * @brief MPU6050�Բ���
 * @return uint8_t 0,���� ����,ʧ��
 */
uint8_t run_self_test(void)
{
    int result;
    // char test_packet[4] = {0};
    long gyro[3], accel[3];
    result = mpu_run_self_test(gyro, accel);
    if (result == 0x7)
    {
        unsigned short accel_sens;
        float gyro_sens;

        mpu_get_gyro_sens(&gyro_sens);
        gyro[0] = (long)(gyro[0] * gyro_sens);
        gyro[1] = (long)(gyro[1] * gyro_sens);
        gyro[2] = (long)(gyro[2] * gyro_sens);
        // inv_set_gyro_bias(gyro, 3);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        // inv_set_accel_bias(accel, 3);
        dmp_set_accel_bias(accel);
        return 0;
    }
    else
        return 1;
}

/**
 * @brief �õ�dmp���������� (ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
 * @return uint8_t 0,���� ����,ʧ��
 */
uint8_t mpu_dmp_get_data()
{
    unsigned long sensor_timestamp;
    short gyro[3], accel[3], sensors;
    unsigned char more;
    long quat[4];
    if (dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more))
        return 1;

    /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
     * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
     **/
    if (sensors & INV_XYZ_GYRO)
    {
        gyrox = 2000 * (double)gyro[0] / 32768;
        gyroy = 2000 * (double)gyro[1] / 32768;
        gyroz = 2000 * (double)gyro[2] / 32768;
    }
    if (sensors & INV_XYZ_ACCEL)
    {
        aacx = 2 * G_CD * (double)accel[0] / 32768;
        aacy = 2 * G_CD * (double)accel[1] / 32768;
        aacz = 2 * G_CD * (double)accel[2] / 32768;
    }

    /* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
     * The orientation is set by the scalar passed to dmp_set_orientation during initialization.
     **/
    if (sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / q30; // q30��ʽת��Ϊ������
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
    }

//    printf("gyro:%.2f %.2f %.2f\n", gyrox, gyroy, gyroz);
//    printf("aac :%.2f %.2f %.2f\n", aacx, aacy, aacz);
//    printf("q~q :%.2f %.2f %.2f %.2f\n", q0, q1, q2, q3);

    return 0;
}

///**
// * @brief �õ�mpl����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
// * @return uint8_t 0,���� ����,ʧ��
// */
//uint8_t mpu_mpl_get_data(void)
//{
//    unsigned long sensor_timestamp, timestamp;
//    short gyro[3], accel_short[3], sensors;
//    unsigned char more;
//    long gyr[3], accel[3], quat[4], temperature;
//    int8_t accuracy;
//
//    if (dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more))
//        return 1;
//
//    if (sensors & INV_XYZ_GYRO)
//    {
//        inv_build_gyro(gyro, sensor_timestamp); // �������ݷ��͸�MPL
//        mpu_get_temperature(&temperature, &sensor_timestamp);
//        inv_build_temp(temperature, sensor_timestamp); // ���¶�ֵ����MPL��ֻ����������Ҫ�¶�ֵ
//    }
//
//    if (sensors & INV_XYZ_ACCEL)
//    {
//        accel[0] = (long)accel_short[0];
//        accel[1] = (long)accel_short[1];
//        accel[2] = (long)accel_short[2];
//        inv_build_accel(accel, 0, sensor_timestamp); // �Ѽ��ٶ�ֵ����MPL
//    }
//
//    inv_execute_on_data();
////    inv_get_sensor_type_euler(data,&accuracy,&timestamp);
//
//    inv_get_sensor_type_quat(quat, &accuracy, &timestamp);
//    for(int i = 0; i < 4; i++)
//    {
//        const float factor = 0.15f; // �˲�����
//        static float tBuff_quat[4];
//        quat[i] = tBuff_quat[i] = tBuff_quat[i] * (1 - factor) + quat[i] * factor;
//    }
//    q0 = quat[0] / q30; // q30��ʽת��Ϊ������
//    q1 = quat[1] / q30;
//    q2 = quat[2] / q30;
//    q3 = quat[3] / q30;
////    printf("quat :%.2f %.2f %.2f %.2f accu:%d time:%d\n", q0, q1, q2, q3, accuracy, timestamp);
//
//    inv_get_sensor_type_accel(accel, &accuracy, &timestamp);
//    for(int i = 0; i < 3; i++)
//    {
//        const float factor = 0.15f; // �˲�����
//        static float tBuff_accel[3];
//        accel[i] = tBuff_accel[i] = tBuff_accel[i] * (1 - factor) + accel[i] * factor;
//    }
//    aacx = accel[0] / q16;
//    aacy = accel[1] / q16;
//    aacz = accel[2] / q16;
////    printf("aac :%.2f %.2f %.2f  accu:%d time:%d\n", aacx, aacy, aacz, accuracy, timestamp);
//
//    inv_get_sensor_type_gyro(gyr, &accuracy, &timestamp);
//    for(int i = 0; i < 3; i++)
//    {
//        const float factor = 0.15f; // �˲�����
//        static float tBuff_gyro[3];
//        gyr[i] = tBuff_gyro[i] = tBuff_gyro[i] * (1 - factor) + gyr[i] * factor;
//    }
//    gyrox = gyr[0] / q16;
//    gyroy = gyr[1] / q16;
//    gyroz = gyr[2] / q16;
////    printf("gyro :%.2f %.2f %.2f  accu:%d time:%d\n", gyrox, gyroy, gyroz, accuracy, timestamp);
//
//
//    return 0;
//}

void mpu6050_entry(void *parameter)
{
    rt_err_t res;

    struct int_param_s int_param;
    unsigned char accel_fsr;
    unsigned short gyro_rate, gyro_fsr;

    /* Initialize mpu6xxx, The parameter is RT_NULL, means auto probing for i2c*/
    mpu_dev = rt_mpu_init(RT_MPU_DEVICE_NAME, RT_NULL);
    if (!mpu_dev)
        return -1;
    else
        mpu_dev_init_flag = 1;

    res = mpu_init(&int_param); // ����mpu6050�����ٶȷ�Χ-2g~2g �����Ǳ��ʣ�2000
    LOG_D("mpu_init end");
    if (res)
        LOG_E("Could not initialize gyro.");
    else
    {
        LOG_D("inv_init_mpl..");
        res = inv_init_mpl(); // ��ʼ��MPL
        if (res)
            return 1;
        inv_enable_quaternion();
        inv_enable_9x_sensor_fusion();
        inv_enable_fast_nomot();
        inv_enable_gyro_tc();
        inv_enable_eMPL_outputs();
        LOG_D("inv_start_mpl..");
        res = inv_start_mpl(); // ����MPL
        if (res)
            return 1;
        LOG_D("mpu_set_sensors..");
        res = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL); // ��������Ҫ�Ĵ�����
        rt_thread_mdelay(3);
        if (res)
            return 2;
        LOG_D("mpu_configure_fifo..");
        res = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL); // ����FIFO
        if (res)
            return 3;
        LOG_D("mpu_set_sample_rate..");
        res = mpu_set_sample_rate(DEFAULT_MPU_HZ); // ���ò�����
        if (res)
            return 4;
        mpu_get_sample_rate(&gyro_rate);
        mpu_get_gyro_fsr(&gyro_fsr);
        mpu_get_accel_fsr(&accel_fsr);
        inv_set_gyro_sample_rate(1000000L / gyro_rate);
        inv_set_accel_sample_rate(1000000L / gyro_rate);
        inv_set_gyro_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_orientation), (long)gyro_fsr << 15);
        inv_set_accel_orientation_and_scale(
            inv_orientation_matrix_to_scalar(gyro_orientation), (long)accel_fsr << 15);
        LOG_D("dmp_load_motion_driver_firmware..");
        res = dmp_load_motion_driver_firmware(); // ����dmp�̼�
        if (res)
            return 6;
        LOG_D("dmp_set_orientation..");
        res = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)); // ���������Ƿ���
        if (res)
            return 7;
        LOG_D("dmp_enable_feature..");
        res = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP | // ����dmp����
                                 DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                                 DMP_FEATURE_GYRO_CAL);
        if (res)
            return 8;
        LOG_D("dmp_set_fifo_rate..");
        res = dmp_set_fifo_rate(DEFAULT_MPU_HZ); // ����DMP�������(��󲻳���200Hz)
        if (res)
            return 9;
        LOG_D("run_self_test..");

        res = run_self_test(); // �Լ�
        if (res)
            rt_kprintf("self test failed\n");
        LOG_D("mpu_set_dmp_state..");
        res = mpu_set_dmp_state(1); // ʹ��DMP
        if (res)
            return 11;
    }
    mpu_reset_fifo();
    /* initial codes end */

    rt_kprintf("mpu6050 init success!\n");
    while (1)
    {
        mpu_dmp_get_data(); // �Ա�����ֱ����dmp����
//        mpu_mpl_get_data();
        rt_thread_mdelay(200);
    }
}

uint8_t mpu6050_init(void)
{
    rt_thread_mdelay(2000);

    rt_thread_t mpu6050_th = rt_thread_create("mpu6050", mpu6050_entry, RT_NULL, 1024, 22, 5);
    if (mpu6050_th == RT_NULL)
        return RT_ERROR;
    else
        rt_thread_startup(mpu6050_th);

    return RT_EOK;
}
// INIT_APP_EXPORT(motion_init);
MSH_CMD_EXPORT(mpu6050_init, mpu6050 driver init);


// �������˲������뺯��
float dT = 0.001;       // ע�⣺dT��ȡֵΪkalman�˲�������ʱ��
float angle, angle_dot; // �ǶȺͽ��ٶ�
float P[2][2] = {{1, 0},
                 {0, 1}};
float Pdot[4] = {0, 0, 0, 0};
float Q_angle = 0.001, Q_gyro = 0.005; // �Ƕ��������Ŷ�,���ٶ��������Ŷ�
float R_angle = 0.5, C_0 = 1;
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;

// �������˲�
float Kalman_Filter(float angle_m, float gyro_m) // angleAx �� gyroGy
{
    angle += (gyro_m - q_bias) * dT;
    angle_err = angle_m - angle;
    Pdot[0] = Q_angle - P[0][1] - P[1][0];
    Pdot[1] = -P[1][1];
    Pdot[2] = -P[1][1];
    Pdot[3] = Q_gyro;
    P[0][0] += Pdot[0] * dT;
    P[0][1] += Pdot[1] * dT;
    P[1][0] += Pdot[2] * dT;
    P[1][1] += Pdot[3] * dT;
    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    E = R_angle + C_0 * PCt_0;
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];
    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
    angle += K_0 * angle_err; // ���ŽǶ�
    q_bias += K_1 * angle_err;
    angle_dot = gyro_m - q_bias; // ���Ž��ٶ�

    return angle;
}
