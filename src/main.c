#include <pebble.h>
#include "windows/main_window.h"

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  main_window_update(tick_time->tm_hour, tick_time->tm_min);
}

static void init() {
  main_window_push();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
