#include "profiles.h"

ChannelSpec g_profile[NUM_CHANNELS];

void profiles_init_default() {
    for (int i = 0; i < NUM_CHANNELS; ++i) {
        g_profile[i].should_be_connected = true;
        g_profile[i].expected_min_ohms   = 0.0f;
        g_profile[i].expected_max_ohms   = 5.0f;  // tweak per harness
    }
}

bool profiles_set_channel(int ch, bool should, float min_ohms, float max_ohms) {
    if (ch < 0 || ch >= NUM_CHANNELS) return false;
    g_profile[ch].should_be_connected = should;
    g_profile[ch].expected_min_ohms   = min_ohms;
    g_profile[ch].expected_max_ohms   = max_ohms;
    return true;
}
