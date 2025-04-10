#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_home_icon_black;
extern const lv_img_dsc_t img_settings_icon_white;
extern const lv_img_dsc_t img_arroe_image;
extern const lv_img_dsc_t img_degree_celcius_icon;
extern const lv_img_dsc_t img_humidity_unit_icon;
extern const lv_img_dsc_t img_full_icon;
extern const lv_img_dsc_t img_empty_icon;
extern const lv_img_dsc_t img_elipse;
extern const lv_img_dsc_t img_elipse1;
extern const lv_img_dsc_t img_control_panel;
extern const lv_img_dsc_t img_control;
extern const lv_img_dsc_t img_panel;
extern const lv_img_dsc_t img_set;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[13];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/