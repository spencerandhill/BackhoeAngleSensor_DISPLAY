// Calibration Button
static void event_handlerCalibrationButton(lv_event_t * e);
static lv_obj_t * buttonCalibrateSensorLabel;
static lv_obj_t * buttonCalibrateSensor;

#define CALIBRATE_BUTTON_SET_WIDTH 100


// FlipXY Switch
static void event_handlerFlipXYSwitch(lv_event_t * e);
static lv_obj_t * switchFlipXYLabel;
static lv_obj_t * switchFlipXY;

#define FLIPXY_SWITCH_SET_WIDTH 70

static void event_handlerCalibrationButton(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_PRESSED) {
        printf("Pressed\n");
    }
    else if(code == LV_EVENT_RELEASED) {
        printf("Released\n");
        sendCalibrateSensor();
    }
}

void drawCalibrationButton() {
    buttonCalibrateSensor = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(buttonCalibrateSensor, event_handlerCalibrationButton, LV_EVENT_ALL, NULL);
    lv_obj_set_size(buttonCalibrateSensor, CALIBRATE_BUTTON_SET_WIDTH, STANDARD_BUTTON_HEIGHT);
    lv_obj_set_pos(buttonCalibrateSensor, (LCD_WIDTH - CALIBRATE_BUTTON_SET_WIDTH) - 10, 330);

    buttonCalibrateSensorLabel = lv_label_create(buttonCalibrateSensor);
    lv_label_set_text(buttonCalibrateSensorLabel, " Calibrate\n   Sensor");
}

static void event_handlerFlipXYSwitch(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        bool flipped = lv_obj_has_state(obj, LV_STATE_CHECKED);
        printf("State: %s\n", flipped ? "On" : "Off");

        sendFlipXYCommand(flipped);
    }
}

void drawFlipXYSwitch() {
    /*Create a switch and apply the styles*/
    switchFlipXY = lv_switch_create(lv_scr_act());
    lv_obj_add_event_cb(switchFlipXY, event_handlerFlipXYSwitch, LV_EVENT_ALL, NULL);
    lv_obj_set_size(switchFlipXY, FLIPXY_SWITCH_SET_WIDTH, STANDARD_BUTTON_HEIGHT);
    lv_obj_set_pos(switchFlipXY, 30, 280 + STANDARD_BUTTON_HEIGHT);

    switchFlipXYLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(switchFlipXYLabel, "Flip XY Axis?");
    lv_obj_set_size(switchFlipXYLabel, 100, 20);
    lv_obj_set_pos(switchFlipXYLabel, 25, 340 + STANDARD_BUTTON_HEIGHT);
}

void updateFlipXYSwitch(bool flipped) {
    if(flipped) {
        lv_obj_add_state(switchFlipXY, LV_STATE_CHECKED);
    } else {
        lv_obj_clear_state(switchFlipXY, LV_STATE_CHECKED);
    }
}