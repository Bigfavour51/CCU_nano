//For EEZ studio to convert images, python3 and above must be installed and the following packages 
//python3 -m pip install pypng
//python3 -m pip install lz4

#include <Arduino.h>
#include <lvgl.h>

#include "LovyanGfXSetup.h"

#include "ui/ui.h"
// #include "i2c_variables.h"
#include <UART_Slave.h>
// #include "i2c_slave.h"
#include "ThingspeakDataPush.h"

bool useSerialMonitor = false; //to enable/disable serial monitor
bool defaultThresholdsSet =false; //to check if default thresholds are set


// #define LED_OUTPUT_PIN 4 //hb -- 17 is blue , 4 is red
#define BUZZER_OUTPUT_PIN 4 //hb 
uint32_t beepDuration = 75; //default 75ms

// #define ANALOG_INPUT_PIN 35

#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT

/* LVGL draws into this buffer, 1/10 screen size usually works well. The size is in bytes. */
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
// #define DRAW_BUF_SIZE (SCREEN_WIDTH * 10)

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

LGFX tft;

extern lv_event_t g_eez_event; //eez    
extern bool g_eez_event_is_available; //eez

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display. */

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  // uint32_t w = ( area->x2 - area->x1 + 1 );
  //  uint32_t h = ( area->y2 - area->y1 + 1 );
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)px_map, w * h);
  tft.endWrite();

  // /* Call it to tell LVGL you are ready. 
  lv_disp_flush_ready(disp);
}



/* Read the touchpad. */
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  else
  {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = touchX;
    data->point.y = touchY;
#if 0
    Serial.printf("x: %03d, y: %03d\n", data->point.x, data->point.y);
#endif
  }
}

void setup()
{
  String LVGL_Arduino = "Hello Arduino ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  Serial.begin(115200);
  delay(1000); //wait for serial monitor to open
  // setup_12C_slave(); //handle i2c communication
  uart_slave_setup();


  if (useSerialMonitor) {
    Serial.println(LVGL_Arduino);
  }

  tft.begin();
  tft.setRotation(0); //HB CHECK ROTATION
  tft.setBrightness(255);

  lv_init();

  /* Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb((lv_tick_get_cb_t)millis);

  /* Register print function for debugging. */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  /* Create a display. */
  lv_display_t *disp = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  /* Initialize the (dummy) input device driver. */
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
  lv_indev_set_read_cb(indev, my_touchpad_read);

  Serial.println("Setup done");

#if 0 //hb
  /* Create a simple label. */
  lv_obj_t *label = lv_label_create( lv_scr_act() );
  lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
  lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
#endif

  ui_init(); //hb --> eez studio ui initialization

  pinMode(BUZZER_OUTPUT_PIN, OUTPUT); // hb
  digitalWrite(BUZZER_OUTPUT_PIN, LOW); // hb
  Thingspeak_setup();
 
}

void loop()
{
  static boolean setBuzzerFlag = false;
  static boolean longBeep = false; //flag to check if long beep is required (particularly for settings save button)
  lv_task_handler(); /* Let LVGL do its work. */
  ui_tick(); //hb
  //---Handle Default Thresholds value on settings screen---//
  if(defaultThresholdsSet==false){
    setDefaultMeterData(); //set default values for all the parameters --> REMOVE THIS LATER
    setDefaultThresholds(); //set default thresholds for all the parameters
    lv_slider_set_value(objects.settings_slider_distance, DistanceThreshold, LV_ANIM_OFF); //set default distance threshold value on settings screen
    lv_slider_set_value(objects.settings_slider_temp, TempThreshold, LV_ANIM_OFF); //set default temperature threshold value on settings screen
    lv_slider_set_value(objects.settings_slider_powercutoff, PowerThreshold, LV_ANIM_OFF); //set default power threshold value on settings screen
    defaultThresholdsSet=true; //set the flag to true to avoid setting default values again and again
    //update the slider labels on settings screen
    char labelDistanceLimit[12];
    sprintf(labelDistanceLimit, "%dcm", DistanceThreshold);
    lv_label_set_text(objects.settings_label_distance, labelDistanceLimit); //set the label text to distance threshold value
    char labelTempLimit[12];  
    sprintf(labelTempLimit, "%d°C", TempThreshold);
    lv_label_set_text(objects.settings_label_temp, labelTempLimit); //set the label text to temperature threshold value
    char labelPowerLimit[12];
    sprintf(labelPowerLimit, "%dW", PowerThreshold);  
    lv_label_set_text(objects.settings_label_powercutoff, labelPowerLimit); //set the label text to power threshold value

    if(useSerialMonitor) {
      Serial.printf("Default thresholds set: Power: %dW, Distance: %dcm, Temperature: %dC\n", PowerThreshold, DistanceThreshold, TempThreshold);
    }
  }

  //---Handle Touch events --------//

if (g_eez_event_is_available == true) {
  lv_obj_t *obj = lv_event_get_target_obj(&g_eez_event);
  if(useSerialMonitor){
  Serial.printf("Received event from obj: %u\n", obj);
  }
  g_eez_event_is_available = false;

  //---Handle Main Screen Continue Button (Switch to Home)---//
  if (obj == objects.main_btn_continue) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_RELEASED) {
          if (useSerialMonitor) {
              Serial.printf("continue button clicked.\n");
          }
          lv_scr_load(objects.home);
          lv_obj_add_flag(objects.main, LV_OBJ_FLAG_HIDDEN); // Hide main screen
          lv_obj_clear_flag(objects.home, LV_OBJ_FLAG_HIDDEN); // Show home screen
          // delete main screen
          lv_obj_del(objects.main);
          // clean up main screen components
          lv_obj_clean(objects.main);
          setBuzzerFlag = true;
      }
  }

  //--handle Home screen settings button (switch to settings page)---//
  else if (obj == objects.home_btn_settings) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_RELEASED) {
          if(useSerialMonitor) {
              Serial.printf("settings button clicked.\n");
          }
          lv_scr_load(objects.settings);
          lv_obj_add_flag(objects.home, LV_OBJ_FLAG_HIDDEN); // Hide home screen
          lv_obj_clear_flag(objects.settings, LV_OBJ_FLAG_HIDDEN); // Show settings screen
          setBuzzerFlag = true;
      }
  }
  
  //handle Settings screen home button (switch to home page)---//
  else if (obj == objects.settings_btn_home) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_RELEASED) {
          if(useSerialMonitor) {
              Serial.printf("home button clicked.\n");
          }
          lv_scr_load(objects.home);
          lv_obj_add_flag(objects.settings, LV_OBJ_FLAG_HIDDEN); // Hide settings screen
          lv_obj_clear_flag(objects.home, LV_OBJ_FLAG_HIDDEN); // Show home screen
          setBuzzerFlag = true;
      }
  }

  //--Handle settings slider temoperature threshold and update the label---//
  else if (obj == objects.settings_slider_temp) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_VALUE_CHANGED) {
          TempThreshold = lv_slider_get_value(objects.settings_slider_temp); //get the value of the slider
          char labelTempLimit[12];
          sprintf(labelTempLimit, "%d°C", TempThreshold); //set the label text to temperature threshold value
          lv_label_set_text(objects.settings_label_temp, labelTempLimit); //set the label text to temperature threshold value
          setBuzzerFlag = true;
      }
  }

  //--Handle settings slider power threshold and update the label---//
  else if (obj == objects.settings_slider_powercutoff) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_VALUE_CHANGED) {
          PowerThreshold = lv_slider_get_value(objects.settings_slider_powercutoff); //get the value of the slider
          char labelPowerLimit[12];
          sprintf(labelPowerLimit, "%dW", PowerThreshold); //set the label text to power threshold value
          lv_label_set_text(objects.settings_label_powercutoff, labelPowerLimit); //set the label text to power threshold value
          setBuzzerFlag = true;
      }
  }

  //-- Handle settings slider distance threshold and update the label---//
  else if (obj == objects.settings_slider_distance) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_VALUE_CHANGED) {
          DistanceThreshold = lv_slider_get_value(objects.settings_slider_distance); //get the value of the slider
          char labelDistanceLimit[12];
          sprintf(labelDistanceLimit, "%dcm", DistanceThreshold); //set the label text to distance threshold value
          lv_label_set_text(objects.settings_label_distance, labelDistanceLimit); //set the label text to distance threshold value
          setBuzzerFlag = true;
      }
  }

  //--Handle settings save btn--> if serial monitor, print all slider values --//
   else if(obj == objects.settings_btn_save) {
      lv_event_code_t code = lv_event_get_code(&g_eez_event);
      if (code == LV_EVENT_RELEASED) {
          if(useSerialMonitor) {
              Serial.printf("save button clicked.\n");
              Serial.printf("Temperature Limit: %d°C\n", TempThreshold);
              Serial.printf("Distance Limit: %dcm\n", DistanceThreshold);
              Serial.printf("Power Limit: %dW\n", PowerThreshold);
              
          }

          longBeep = true; //long beep for save button
          setBuzzerFlag = true;

          // switch to home screen after saving settings
          lv_scr_load(objects.home);
          lv_obj_add_flag(objects.settings, LV_OBJ_FLAG_HIDDEN); // Hide settings screen
          lv_obj_clear_flag(objects.home, LV_OBJ_FLAG_HIDDEN); // Show home screen
      
      }
    }
  }

      // ========== Buzzer beeper START ==========
      static uint32_t buzzerTimePrev = 0L;
      static boolean buzzerIsBusy = false;
      if (longBeep)
      {
        beepDuration = 500; // 500 ms
      //   longBeep = false;
      }
      else {
        beepDuration = 75; // 75 ms
        }
      
      if (setBuzzerFlag && !buzzerIsBusy)
      {
        buzzerTimePrev = millis();
        setBuzzerFlag = false;
        buzzerIsBusy = true;
        digitalWrite(BUZZER_OUTPUT_PIN, HIGH);
      }
      
      if (buzzerIsBusy && millis() - buzzerTimePrev > beepDuration) // Beep duration: 75 ms only.
      {
        buzzerIsBusy = false;
        longBeep = false;
        digitalWrite(BUZZER_OUTPUT_PIN, LOW);
      }
      // ========== Buzzer beeper END   ==========

      //=====HOME SCREEN DATA DISPLAY =================//
      if(lv_screen_active() == objects.home) {
      //Set arc values for current, power and voltage on home screen
      lv_arc_set_value(objects.home_arc_voltage,(int)Voltage); //set the arc value to voltage value
      lv_arc_set_value(objects.home_arc_current,(int)(Current*10)); //set the arc value to current value
      lv_arc_set_value(objects.home_arc_power,(int)Power); //set the arc value to power value
      
      //---Update the arc label values on home screen---//
      char labelVoltage[12];
      sprintf(labelVoltage, "%.1fV", Voltage); //set the label text to voltage value
      lv_label_set_text(objects.home_label_voltage, labelVoltage); //set the label text to voltage value

      char labelCurrent[12];
      sprintf(labelCurrent, "%.2fA", Current); //set the label text to current value
      lv_label_set_text(objects.home_label_current, labelCurrent); //set the label text to current value
      
      char labelPower[12];
      sprintf(labelPower, "%.2fW", Power); //set the label text to power value
      lv_label_set_text(objects.home_label_power, labelPower); //set the label text to power value
      
      //Set label values for temperature and humidity on home screen
      char labelTemperature[12];
      sprintf(labelTemperature, "%.2f°C", temperature); //set the label text to temperature value
      lv_label_set_text(objects.home_label_temperature, labelTemperature); //set the label text to temperature value

      char labelHumidity[12];
      sprintf(labelHumidity, "%.2f%%", humidity); //set the label text to humidity value
      lv_label_set_text(objects.home_label_humidity, labelHumidity); //set the label text to humidity value 

      //---Update the water level image on home screen---//
      if(waterLevel == true) { //if water level is high/true, show full image
          lv_obj_clear_flag(objects.home_img_full, LV_OBJ_FLAG_HIDDEN); //show full image
          lv_obj_add_flag(objects.home_img_empty, LV_OBJ_FLAG_HIDDEN); //hide empty image
      }
      else { //if water level is low/false, show empty image
          lv_obj_clear_flag(objects.home_img_empty, LV_OBJ_FLAG_HIDDEN); //show full image
          lv_obj_add_flag(objects.home_img_full, LV_OBJ_FLAG_HIDDEN); //hide empty image
      }
    
    }

    uart_slave_read(); // Read from UART
    Thingspeak_push_data(); //push data to thingspeak

}