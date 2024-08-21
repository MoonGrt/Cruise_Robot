#ifndef __DHT11_H__
#define __DHT11_H__

#include <rtthread.h>
#include "sensor.h"
#include "stdint.h"

#define CONNECT_SUCCESS  0
#define CONNECT_FAILED   1

struct dht11_device
{
    rt_base_t pin;
    rt_mutex_t lock;
};
typedef struct dht11_device *dht11_device_t;

uint8_t dht11_init(rt_base_t pin);
int32_t dht11_get_temperature(rt_base_t pin);
int rt_hw_dht11_init(const char *name, struct rt_sensor_config *cfg);

#endif /* __DS18B20_H_ */
