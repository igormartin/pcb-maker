#pragma once

#include "main.hpp"
#include "Thread.hpp"
#include <msg.h>

#define ENABLE_DEBUG (1)
#include "debug.h"

template <uint16_t STACK_SIZE>
class SensorsThread : public Thread<STACK_SIZE>
{
public:
    using Thread<STACK_SIZE>::Thread;

    struct SensorMessage {
        bool hasActualValue;
        float temperature;
    };

protected:
    const uint32_t SLEEP_TIME = 100 * US_PER_MS;
    const uint16_t SENSOR_STARTUP_TIME = 2;
    const kernel_pid_t RECEIVER_THREAD_PID = 2;

    bool hasActualTemperature = false;
    int16_t rawTemperature = 0;
    float temperature = 0;

    ds18_t sensor = {};
    ds18_params_t sensorParams = {};

    msg_t msg = {};
    SensorMessage sensorMessage = {};

    void run() override
    {
        sensor = {};
        sensorParams = {
            .pin      = GPIO_PIN(PORT_B, 11),
            .out_mode = GPIO_OD_PU,
            .in_mode  = GPIO_IN_PU,
        };

        ds18_init(&sensor, &sensorParams);
        xtimer_sleep(SENSOR_STARTUP_TIME);

        loop {
            if (ds18_get_temperature(&sensor, &rawTemperature) != DS18_OK) {
                DEBUG("[ERROR] Could not read temperature\n");
                this->hasActualTemperature = false;
                this->sendMessage();

                xtimer_usleep(SLEEP_TIME);
                continue;
            }

            this->temperature = (float)rawTemperature / 100;
            this->hasActualTemperature = true;

            DEBUG("[INFO] Temperature: %d *C\n", (uint8_t)temperature);

            this->sendMessage();
            xtimer_usleep(SLEEP_TIME);
        }
    }

private:
    void sendMessage()
    {
        sensorMessage.hasActualValue = this->hasActualTemperature;
        sensorMessage.temperature = this->temperature;

        msg.content.ptr = &sensorMessage;
        msg_send(&msg, this->RECEIVER_THREAD_PID);
    }
};
