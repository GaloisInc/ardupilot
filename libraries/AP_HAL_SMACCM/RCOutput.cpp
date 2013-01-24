/* -*- Mode: C++; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <AP_HAL_Boards.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_SMACCM

#include "RCOutput.h"
#include <hwf4/ardrone.h>

using namespace SMACCM;

extern const AP_HAL::HAL& hal;

static void update_motors(uint32_t now);

void SMACCMRCOutput::init(void* unused)
{
    ardrone_init();
    ardrone_motor_init();
    ardrone_motor_init();
    ardrone_motor_init();
    hal.scheduler->register_timer_process(update_motors);
}

void SMACCMRCOutput::set_freq(uint32_t chmask, uint16_t freq_hz)
{
}

uint16_t SMACCMRCOutput::get_freq(uint8_t ch)
{
    return 50;
}

void SMACCMRCOutput::enable_ch(uint8_t ch)
{
}

void SMACCMRCOutput::enable_mask(uint32_t chmask)
{
}

void SMACCMRCOutput::disable_ch(uint8_t ch)
{
}

void SMACCMRCOutput::disable_mask(uint32_t chmask)
{
}

static uint16_t periods[4];
static uint32_t last_motor_update;

static void update_motors(uint32_t now)
{
    if (now - last_motor_update > 5000) { // 5ms update rate
        last_motor_update = now;
        ardrone_motor_set(periods[2], periods[0], periods[3], periods[1]);
    }
}

#define PERIOD_MIN    1100
#define PERIOD_MAX    1900
#define MOTOR_OFF     0
#define MOTOR_MIN     10
#define MOTOR_MAX     500

// Convert the RC channel period [1100..1900] to an ARDrone motor
// value [0..500].
static uint16_t scale_period(uint16_t us)
{
    if (us == 0 || us <= PERIOD_MIN)
        return MOTOR_OFF;

    if (us > PERIOD_MAX)
        us = PERIOD_MAX;

    float scale = (((float)us - (float)PERIOD_MIN) /
                   (float)(PERIOD_MAX - PERIOD_MIN));
    uint16_t result = (float)(MOTOR_MAX) * scale;

    if (result < MOTOR_MIN)
        result = MOTOR_MIN;
    else if (result > MOTOR_MAX)
        result = MOTOR_MAX;

    return result;
}

void SMACCMRCOutput::write(uint8_t ch, uint16_t period_us)
{
    if (ch < 4) {
        periods[ch] = scale_period(period_us);
    }
}

void SMACCMRCOutput::write(uint8_t ch, uint16_t* period_us, uint8_t len)
{
    for (int i = 0; i < len; ++i)
        write(ch + i, period_us[i]);
}

uint16_t SMACCMRCOutput::read(uint8_t ch)
{
    return 900;
}

void SMACCMRCOutput::read(uint16_t* period_us, uint8_t len)
{
}

#endif // CONFIG_HAL_BOARD == HAL_BOARD_SMACCM
