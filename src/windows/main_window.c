#include "main_window.h"

static Window *s_window;
static Layer *s_canvas;
static GBitmap *s_pebble;

static int s_hour, s_minute;

// Number to pin layout mapping
const int DICE_PINS[][9] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0}, /* 0 */
  {0, 0, 0, 0, 1, 0, 0, 0, 0}, /* 1 */
  {1, 0, 0, 0, 0, 0, 0, 0, 1}, /* 2 */
  {1, 0, 0, 0, 1, 0, 0, 0, 1}, /* 3 */
  {1, 0, 1, 0, 0, 0, 1, 0, 1}, /* ... */
  {1, 0, 1, 0, 1, 0, 1, 0, 1},
  {1, 1, 1, 0, 0, 0, 1, 1, 1},
  {1, 1, 1, 0, 1, 0, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1} /* 9 */
};

static void draw_dice(GContext *ctx, GRect rect, int pins) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, rect, 10, GCornersAll);
  graphics_context_set_fill_color(ctx, GColorBlack);

  for (int i=0; i<9; i++) {
    if (DICE_PINS[pins][i]) {
      graphics_fill_circle(ctx, GPoint(rect.origin.x+15 + 20*(i%3), rect.origin.y+15 + 20*(int)(i/3)), 6);
    }
  }
}

static void layer_update_proc(struct Layer *layer, GContext *ctx) {
  draw_dice(ctx, GRect(0,   0,  70, 70), (int)(s_hour/10));
  draw_dice(ctx, GRect(74,  0,  70, 70), s_hour%10);
  draw_dice(ctx, GRect(0,   98, 70, 70), (int)(s_minute/10));
  draw_dice(ctx, GRect(74,	98, 70, 70), s_minute%10);

  graphics_draw_bitmap_in_rect(ctx, s_pebble, GRect(0, 72, 144, 24));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas = layer_create(bounds);
  layer_set_update_proc(s_canvas, layer_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas);

  s_pebble = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PEBBLE);
}

static void window_unload(Window *window) {
  gbitmap_destroy(s_pebble);
  layer_destroy(s_canvas);
  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
  window_stack_push(s_window, true);
}

void main_window_update(int hour, int minute) {
  s_hour = hour;
  s_minute = minute;

  layer_mark_dirty(s_canvas);
}
