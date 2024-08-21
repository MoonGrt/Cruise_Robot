#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <netdev_ipaddr.h>
#include <netdev.h>
#include <dfs_fs.h>

#include "web.h"
#include "basic.h"
#include "monitor.h"
#include "bt_module.h"
#include "wifi.h"

#define DBG_COLOR
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

extern uint8_t ros_car();
extern uint8_t motor_init(void);
extern uint8_t dht11_initial(void);
extern uint8_t mpu6050_init(void);
extern uint8_t camera_init(void);
extern uint8_t onenet_init(void);
extern uint8_t YDLIDAR_init(void);
extern uint8_t bianma_init(void);
extern uint8_t ai_init(void);
extern void wlan_autoconnect_init(void);

int main(void)
{
    /* init Wi-Fi auto connect feature */
    wlan_autoconnect_init();
    /* enable auto reconnect on WLAN device */
    rt_wlan_config_autoreconnect(RT_TRUE);
//
////    wifi_init();
////    basic_init();
//    sys_monitor_init();
//    web_init();
////    bluetooth_init();
//
    while (!rt_wlan_is_ready())
        rt_thread_mdelay(2000);
////    dht11_initial();
////    mpu6050_init();
////    onenet_init();
    YDLIDAR_init();
    encoder_init();
    ros_car();
    motor_init();
    ai_init();

    return RT_EOK;
}

#include "stm32h7xx.h"
static int vtor_config(void)
{
    /* Vector Table Relocation in Internal QSPI_FLASH */
    SCB->VTOR = QSPI_BASE;
    return 0;
}
INIT_BOARD_EXPORT(vtor_config);
