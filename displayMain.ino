#include "ft6x36.h"

// Objects, that should be seen on EVERY Screen at the LV_INDEV_TYPE_POINTER
lv_obj_t * systemStatusLED;
lv_obj_t * systemStatusLEDLabel;
int lastSensorStatus = SENSOR_STATUS_ERROR; // Check last Sensor status before updating to prevent unnecessary Display Updates

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */

// LVGL Stuff
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[DISP_VER_RES * DISP_HOR_RES / 10];

// LVGL Prototypes
static void touch_driver_read(lv_indev_drv_t * indev, lv_indev_data_t * data);


// #define USE_LV_LOG LV_USE_LOG
#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(const char * buf) {
    Serial.printf("%s\r\n", buf);
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

void touch_driver_read(lv_indev_drv_t * indev, lv_indev_data_t * data) {
    ft6x36_read(indev, data);
}

void initDisplay() {
    Serial.println("Display Begin");

    // Touch Init for LVGL using I2C-Manager
    lv_init();

    // Touch Init
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);

#if USE_LV_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

    tft.begin(); /* TFT init */
    tft.setRotation(2); /* Portrait orientation */

    lv_disp_draw_buf_init(&draw_buf, buf, NULL, DISP_VER_RES * 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;    /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;          /*Assign the buffer to the display*/
    disp_drv.hor_res = DISP_HOR_RES;               /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = DISP_VER_RES;               /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

    /*Initialize the touch input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_driver_read;
    lv_indev_drv_register(&indev_drv);

    createDisplayContent();
}

void drawSystemSensorStatus() {

    systemStatusLED  = lv_led_create(lv_scr_act());
    lv_obj_set_size(systemStatusLED, 14, 14);
    lv_obj_set_pos(systemStatusLED, LCD_WIDTH - 19, 9);
    lv_led_on(systemStatusLED);
    lv_led_set_brightness(systemStatusLED, 255);
    lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_RED));

    /* Create a label in front of the status LED */
    systemStatusLEDLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(systemStatusLEDLabel, "Sensor");
    lv_obj_align_to(systemStatusLEDLabel, systemStatusLED, LV_ALIGN_OUT_LEFT_MID, -9, 0);
}

void updateSystemSensorStatus(int sensorStatus) {
    if(sensorStatus != lastSensorStatus) { // Check, if the status has changed to prevent unnecessary updates
        switch(sensorStatus) {
            case SENSOR_STATUS_ERROR: // Sensor OFFLINE or something really bad happened
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_RED));
                break;
            case SENSOR_STATUS_WARNING: // Sensor ALIVE but NOT OK
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_ORANGE));
                break;
            case SENSOR_STATUS_OK: // Sensor ALIVE and OK
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_GREEN));
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
    drawFlipXYSwitch();
}