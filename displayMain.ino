#include "ft6x36.h"

// Objects, that should be seen on EVERY Screen at the LV_INDEV_TYPE_POINTER
lv_obj_t * systemStatusLED;
lv_obj_t * systemStatusLEDLabel;
int lastSensorStatus = SENSOR_STATUS_ERROR; // Check last Sensor status before updating to prevent unnecessary Display Updates

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

// LVGL Setup Stuff
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
static bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc) {
    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    bool res = ft6x36_read(drv, data);
    return res;
}

void initDisplay() {
    Serial.println("Display Begin");

    // LVGL Init
    lv_init();

    // Touch Init
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);

#if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin(); /* TFT init */
    tft.setRotation(2); /* Portrait orientation */

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 480;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the touch input device driver*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_driver_read;
    lv_indev_drv_register(&indev_drv);

    createDisplayContent();
}

void drawSystemSensorStatus() {

    systemStatusLED  = lv_led_create(lv_scr_act(), NULL);
    lv_obj_set_size(systemStatusLED, 10, 10);
    lv_obj_set_pos(systemStatusLED, LCD_WIDTH - 15, 5);
    lv_led_on(systemStatusLED);
    lv_led_set_bright(systemStatusLED, 10);

    /* Create a label in front of the status LED */
    systemStatusLEDLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(systemStatusLEDLabel, "Sensor");
    lv_obj_set_auto_realign(systemStatusLEDLabel, true);
    lv_obj_align(systemStatusLEDLabel, systemStatusLED, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}

void updateSystemSensorStatus(int sensorStatus) {
    if(sensorStatus != lastSensorStatus) { // Check, if the status has changed to prevent unnecessary updates
        switch(sensorStatus) {
            case SENSOR_STATUS_ERROR: // Sensor OFFLINE or something really bad happened
                lv_led_off(systemStatusLED);
                break;
            case SENSOR_STATUS_WARNING: // Sensor ALIVE but NOT OK
                lv_led_set_bright(systemStatusLED, 128);
                break;
            case SENSOR_STATUS_OK: // Sensor ALIVE and OK
                lv_led_set_bright(systemStatusLED, 255);
                break;
        }

        lastSensorStatus = sensorStatus;
    }
}

void createDisplayContent() {
    drawSystemSensorStatus();
    drawSensorSliders();
    drawShovel();
    drawCalibrationButton();
    drawFlipXYSlider();
}