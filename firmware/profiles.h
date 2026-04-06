#pragma once
#include "config.h"

struct ChannelSpec {
    bool  should_be_connected;
    float expected_min_ohms;
    float expected_max_ohms;
};

extern ChannelSpec g_profile[NUM_CHANNELS];

void profiles_init_default();
bool profiles_set_channel(int ch, bool should, float min_ohms, float max_ohms);
