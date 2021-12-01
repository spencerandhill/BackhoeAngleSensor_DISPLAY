#include "CONSTANTS.h"
#include "makerfabs_pin.h"
#include <lvgl.h>
#include <TFT_eSPI.h>

// LVGL Prototypes
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);

void setup(void) {
  Serial.begin(115200);
  Serial.println("BackhoeAngleSensor BAS - (c) Author Jan Rathmer"); Serial.println("");

  initEspNow();
  initDisplay();
}

void loop(void) {
// Let the LVGL-GUI do its work
  lv_task_handler();
  // delay(5);
  loopEspNow();
}
