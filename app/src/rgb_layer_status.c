/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/rgb_layer_status.h>
#include <zmk/rgb_underglow.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/event_manager.h>
#include <init.h>
#include <zmk/keymap.h>


void set_layer_colour()
{
    int active_layer_index = zmk_keymap_highest_layer_active();

    LOG_DBG("Layer changed to %i", active_layer_index);

    struct zmk_led_hsb colour;
    colour.s = 100;
    colour.b = 100;

    switch (active_layer_index)
    {
        case  4 : { colour.h = 150; break; }
        case  5 :
        case  6 :
        case  7 : { colour.h = 300; break; }
        case  8 :
        case  9 : { colour.h = 240; break; }
        case 10 : { colour.h =  40; break; }
        case 11 : { colour.h =   0; break; }
        default : { colour.b =   0; break; }
    }

    if (colour.b == 0)
    {
        zmk_rgb_underglow_off();
    }
    else
    {
        zmk_rgb_underglow_set_hsb(colour);
        zmk_rgb_underglow_on();
    }
}

static int zmk_rgb_layer_status_init(const struct device* _arg)
{
    zmk_rgb_underglow_off();
    return 0;
}

int layer_status_listener(const zmk_event_t* eh)
{
    set_layer_colour();
    return 0;
}

ZMK_LISTENER(rgb_layer_status, layer_status_listener)
ZMK_SUBSCRIPTION(rgb_layer_status, zmk_layer_state_changed);

SYS_INIT(zmk_rgb_layer_status_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
