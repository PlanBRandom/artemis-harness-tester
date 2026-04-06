#pragma once
#include "config.h"
#include "profiles.h"

void output_print_channel_human(int ch, const ChannelSpec& spec, float r);
void output_print_channel_json(int ch, const ChannelSpec& spec, float r, bool last);
void output_print_profile_entry_json(int ch, const ChannelSpec& spec, bool last);

void print_summary();
void print_csv_matrix();
void print_json_matrix();
