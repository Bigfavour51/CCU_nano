#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *home;
    lv_obj_t *settings;
    lv_obj_t *main_btn_continue;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *home_btn_home;
    lv_obj_t *home_btn_settings;
    lv_obj_t *obj2;
    lv_obj_t *home_arc_voltage;
    lv_obj_t *home_label_voltage;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *home_arc_current;
    lv_obj_t *home_label_current;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *home_arc_power;
    lv_obj_t *home_label_power;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *home_label_temperature;
    lv_obj_t *home_label_humidity;
    lv_obj_t *obj9;
    lv_obj_t *home_label_level;
    lv_obj_t *home_img_empty;
    lv_obj_t *home_img_full;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *home_label_temperature_1;
    lv_obj_t *obj12;
    lv_obj_t *settings_btn_home;
    lv_obj_t *settings_btn_settings;
    lv_obj_t *settings_slider_temp;
    lv_obj_t *obj13;
    lv_obj_t *settings_label_temp;
    lv_obj_t *settings_slider_distance;
    lv_obj_t *obj14;
    lv_obj_t *settings_label_distance;
    lv_obj_t *settings_slider_powercutoff;
    lv_obj_t *obj15;
    lv_obj_t *settings_label_powercutoff;
    lv_obj_t *settings_btn_save;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_HOME = 2,
    SCREEN_ID_SETTINGS = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_home();
void tick_screen_home();

void create_screen_settings();
void tick_screen_settings();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/