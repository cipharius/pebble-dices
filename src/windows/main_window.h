#pragma once

#include <pebble.h>

// Creates and pushes the main_window to the windows stack
void main_window_push();

// Updates main_window
void main_window_update(struct tm *tick_time);
