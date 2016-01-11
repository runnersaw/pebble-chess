#include <pebble.h>
#include "instructionHandler.h"
#include "settingsHandler.h"
#include "textHandler.h"
#include "chess.h"

#define NUM_INSTRUCTION_SECTIONS 1

#define NUM_INSTRUCTION_ITEMS_SETTINGS 4

#define GAME_INDEX 0
#define INSTRUCTION_INDEX 1
#define SETTINGS_INDEX 2
#define ABOUT_INDEX 3

#define ICON_SIZE 28
#define CELL_HEIGHT 45

static Window *s_instruction_menu_window;
static MenuLayer *s_instruction_menu;

static GBitmap *game_icon;
static GBitmap *instruction_icon;
static GBitmap *settings_icon;

// A simple menu layer can have multiple sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_INSTRUCTION_SECTIONS;
}

// Each section is composed of a number of menu items
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return NUM_INSTRUCTION_ITEMS_SETTINGS;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return 0;
}

static int16_t menu_get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  return CELL_HEIGHT;
}

static void draw_menu(GContext *ctx, const Layer *layer, char *title, GBitmap *bmp) {
  menu_cell_basic_draw(ctx, layer, title, NULL, bmp);
}

static void menu_draw_row_callback(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  if (cell_index->row == GAME_INDEX) {
    draw_menu(ctx, cell_layer, "Play", game_icon);
  } else if (cell_index->row == INSTRUCTION_INDEX) {
    draw_menu(ctx, cell_layer, "Instructions", NULL);
  } else if (cell_index->row == SETTINGS_INDEX) {
    draw_menu(ctx, cell_layer, "Settings", NULL);
  } else if (cell_index->row == ABOUT_INDEX) {
    draw_menu(ctx, cell_layer, "About", NULL);
  }
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  if (cell_index->row == GAME_INDEX) {
    chess_init();
  } else if (cell_index->row == INSTRUCTION_INDEX) {
    text_init(CHESS);
  } else if (cell_index->row == SETTINGS_INDEX) {
    settings_init();
  } else if (cell_index->row == ABOUT_INDEX) {
    text_init(ABOUT);
  }
}

static void load_bitmaps() {
  game_icon = gbitmap_create_with_resource(RESOURCE_ID_CHESS_ICON);
}

static void destroy_bitmaps() {
  gbitmap_destroy(game_icon);
}

static void instruction_menu_window_load(Window *window) {
  load_bitmaps();

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_instruction_menu = menu_layer_create(bounds);

  menu_layer_set_callbacks(s_instruction_menu, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .get_cell_height = menu_get_cell_height_callback,
    .draw_header = NULL,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  menu_layer_set_click_config_onto_window(s_instruction_menu, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_instruction_menu));
}

static void instruction_menu_window_unload(Window *window) {
  destroy_bitmaps();
  menu_layer_destroy(s_instruction_menu);
  window_destroy(window);
  s_instruction_menu_window = NULL;
}

void instruction_init() {
  // Create main Window element and assign to pointer
  s_instruction_menu_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_instruction_menu_window, (WindowHandlers) {
    .load = instruction_menu_window_load,
    .unload = instruction_menu_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_instruction_menu_window, true);
}

void instruction_deinit() {

}

int main() {
  instruction_init();
  app_event_loop();
  instruction_deinit();
}