// Version: 0.1
// First working version: -Full hardware support for Display (including LVGL), Touch-Driver and BNO055 Sensor.
//                        -No Sensor Offset calibration, yet.

#include "makerfabs_pin.h"
#include <Wire.h>
#include <lvgl.h>
#include <lv_examples.h>
#include <TFT_eSPI.h>

// LVGL Display Defines
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);

void setup(void)
{
  Serial.begin(115200);
  Serial.println("BackhoeAngleSensor BAS - (c) Author Jan Rathmer"); Serial.println("");

  initEEPROM();
  setFlipXYToEEPROM(true);
  initSensor();
  initDisplay();
}

void loop(void)
{
// Check Buttons
  loopSensor();
  loopDisplay();
  delay(5);
}
