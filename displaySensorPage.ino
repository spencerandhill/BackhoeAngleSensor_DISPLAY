// Sliders
static lv_obj_t * sliderX;
static lv_obj_t * sliderY;

static lv_obj_t * sliderX_label;
static lv_obj_t * sliderY_label;

// Set Offset Button
static void event_handlerOffsetButton(lv_obj_t * obj, lv_event_t event);
static lv_obj_t * buttonSetOffsetLabel;
static lv_obj_t * buttonSetOffset;

#define OFFSET_BUTTON_SET_WIDTH 100


// FlipXY Switch
static void event_handlerFlipXYSwitch(lv_obj_t * obj, lv_event_t event);
static lv_obj_t * switchFlipXYLabel;
static lv_obj_t * switchFlipXY;

#define FLIPXY_SWITCH_SET_WIDTH 70

// Offset-Animation Arc
#define OFFSET_ANIMATION_ARC_WIDTH 150
#define OFFSET_ANIMATION_ARC_HEIGHT 150
static lv_obj_t * arc;

unsigned long arcCreatedTime = 0;

// Shovel
#define LV_COLOR_DEPTH 1
LV_IMG_DECLARE(shovel);

lv_obj_t * shovelImg;

void createSensorSliders() {
// X-Slider    
    /* Create a slider at the bottom of the display */
    sliderX = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(sliderX, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(sliderX, -20, 20);
    lv_obj_set_size(sliderX, 200, 50);
    lv_obj_set_pos(sliderX, 50, 230);
    lv_slider_set_value(sliderX, 0, LV_ANIM_OFF);
    
    /* Create a label below the slider */
    sliderX_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sliderX_label, "0");
    lv_obj_set_auto_realign(sliderX_label, true);
    lv_obj_align(sliderX_label, sliderX, LV_ALIGN_OUT_BOTTOM_LEFT, -5, 10);

// Y-Slider    
    /* Create a slider at the right of the display */
    sliderY = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(sliderY, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_slider_set_range(sliderY, -128, 128);
    lv_obj_set_size(sliderY, 50, 200);
    lv_obj_set_pos(sliderY, 250, 50);
    lv_slider_set_value(sliderY, 0, LV_ANIM_OFF);

    /* Create a label below the slider */
    sliderY_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(sliderY_label, "0");
    lv_obj_set_auto_realign(sliderY_label, true);
    lv_obj_align(sliderY_label, sliderY, LV_ALIGN_OUT_TOP_MID, 0, 0);
}

void updateSliderXY(float xValue, float yValue)  {
    lv_slider_set_value(sliderX, (int16_t) xValue, LV_ANIM_OFF);
    lv_label_set_text(sliderX_label, String(xValue, 1).c_str());

    lv_slider_set_value(sliderY, (int16_t) yValue, LV_ANIM_OFF);
    lv_label_set_text(sliderY_label, String(yValue, 1).c_str());
}

void drawSetOffsetArc() {
    /*Create an Arc*/
    arc = lv_arc_create(lv_scr_act(), NULL);
    
    lv_arc_set_end_angle(arc, 200);
    lv_arc_set_range(arc, 0, 5);
    lv_arc_set_bg_angles(arc, 0, 360);
    lv_arc_set_angles(arc, 0, 360);
    lv_arc_set_rotation(arc, 270);
    lv_arc_set_value(arc, 0);
    lv_obj_set_size(arc, LCD_WIDTH, LCD_WIDTH); // Should cover as much screen as possible without overlapping the offset Button
    lv_obj_set_pos(buttonSetOffset, 0, 0);

    arcCreatedTime = millis();
}

void updateOffsetArc() {
    int16_t currentArcValue = lv_arc_get_value(arc);
    lv_arc_set_value(arc, currentArcValue + 1);
}

static void event_handlerOffsetButton(lv_obj_t * obj, lv_event_t event) {
    if(event == LV_EVENT_PRESSED) {
        printf("Pressed\n");
        drawSetOffsetArc();
    } else if(event == LV_EVENT_PRESSING && millis() - arcCreatedTime > 1000) {
        printf("Pressing\n");
        updateOffsetArc();
    }
    else if(event == LV_EVENT_RELEASED) {
        printf("Released\n");
        lv_obj_del(arc);
    }
}

void drawOffsetButton() {
    buttonSetOffset = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(buttonSetOffset, event_handlerOffsetButton);
    lv_obj_set_size(buttonSetOffset, OFFSET_BUTTON_SET_WIDTH, 50);
    lv_obj_set_pos(buttonSetOffset, (LCD_WIDTH - OFFSET_BUTTON_SET_WIDTH) - 10, 330);

    buttonSetOffsetLabel = lv_label_create(buttonSetOffset, NULL);
    lv_label_set_text(buttonSetOffsetLabel, "Set Offset");
}

static void event_handlerFlipXYSwitch(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        bool flipped = lv_switch_get_state(obj);
        printf("State: %s\n", flipped ? "On" : "Off");

        sendFlipXYCommand(flipped);
    }
}

void drawFlipXYSlider() {
    /*Create a switch and apply the styles*/
    switchFlipXY = lv_switch_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(switchFlipXY, event_handlerFlipXYSwitch);
    lv_obj_set_size(switchFlipXY, FLIPXY_SWITCH_SET_WIDTH, 50);
    lv_obj_set_pos(switchFlipXY, 30, 330);

    switchFlipXYLabel = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(switchFlipXYLabel, "Flip XY Axis?");
    lv_obj_set_size(switchFlipXYLabel, 100, 20);
    lv_obj_set_pos(switchFlipXYLabel, 25, 380);
}

void updateFlipXYSlider(bool flipped) {
    if(flipped) {
        lv_switch_on(switchFlipXY, LV_ANIM_ON);
    } else {
        lv_switch_off(switchFlipXY, LV_ANIM_ON);
    }
}

void drawShovel() {
    shovelImg = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(shovelImg, &shovel);
    lv_obj_set_pos(shovelImg, 70, 50);

    lv_img_set_pivot(shovelImg, 100, 66); // somewhere in the middle on the right SIDE
    lv_img_set_angle(shovelImg, 20);
}

void updateShovelX(float xValue) {
    lv_img_set_angle(shovelImg, (int16_t) xValue * 10);
}