#include <pebble.h>
	
static Window *s_window;
static Layer *s_canvas;

static GBitmap *s_pebble;

static int hour = 0;
static int minute = 0;

static bool updated = false;
	
static void draw_dice(GContext *ctx, GRect rect, int pins) {
	graphics_context_set_fill_color(ctx, GColorWhite);
	graphics_fill_rect(ctx, rect, 10, GCornersAll);
	
	for(int i=0; i<pins; i++) {
		graphics_context_set_fill_color(ctx, GColorBlack);
		int n = i;
		int npair = (pins % 2 == 1);
		switch(n) {
			case 0: // 1
				if(npair) {
					n = 4;
				}
				break;
			case 1: // 2
				n = 8;
				break;
			case 2: // 3
				if(npair) {
					n = 0;
				} else {
					n = 2;
				}
				break;
			case 3: // 4
				n = 6;
				break;
			case 4: // 5
				if(npair) {
					n = 2;
				} else {
					n = 1;
				}
				break;
			case 5: // 6
				n = 7;
				break;
			case 6: // 7
				if(npair) {
					n = 1;
				} else {
					n = 3;
				}
				break;
			case 7: // 8
				n = 5;
				break;
			case 8: // 9
				n = 3;
				break;
			default:
				break;
		};
		graphics_fill_circle(ctx, GPoint(rect.origin.x+15 + 20*(n%3), rect.origin.y+15 + 20*(int)(n/3)), 6);
	}
}

static void layer_update_proc(struct Layer *layer, GContext *ctx) {
	draw_dice(ctx, GRect(0, 	0, 	70, 70), (int)(hour/10));
	draw_dice(ctx, GRect(74, 	0, 	70, 70), hour%10);
	draw_dice(ctx, GRect(0, 	98, 70, 70), (int)(minute/10));
	draw_dice(ctx, GRect(74,	98, 70, 70), minute%10);
	graphics_draw_bitmap_in_rect(ctx, s_pebble, GRect(0, 72, 144, 24));
	
	updated = true;
}

static void update_time() {
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);
	
	static char buffer[] = "00";
	if(clock_is_24h_style()) {
		strftime(buffer, sizeof("00"), "%H", tick_time);
		hour = atoi(buffer);
	} else {
		strftime(buffer, sizeof("00"), "%I", tick_time);
		hour = atoi(buffer);
	}
	strftime(buffer, sizeof("00"), "%M", tick_time);
	minute = atoi(buffer);
	
	if(updated) {
		layer_mark_dirty(s_canvas);
		updated = false;
	}
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	update_time();
}

static void window_load(Window *window) {
	window_set_background_color(window, GColorBlack);
	
	update_time();
	
	s_pebble = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PEBBLE);
	
	s_canvas = layer_create(GRect(0, 0, 144, 168));
	layer_set_update_proc(s_canvas, layer_update_proc);
	layer_add_child(window_get_root_layer(window), s_canvas);
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void window_unload(Window *window) {
	gbitmap_destroy(s_pebble);
	layer_destroy(s_canvas);
}

static void init() {
	s_window = window_create();
	window_set_window_handlers(s_window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});
	
	window_stack_push(s_window, true);
}

static void deinit() {
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}