

// System Status LED (top left corner)
lv_obj_t * systemStatusLED;
lv_obj_t * systemStatusLEDLabel;
int lastSystemStatus = SENSOR_STATUS_ERROR; // Check last System status before updating to prevent unnecessary Display Updates

// Sensor Status LED (top right corner)
lv_obj_t * sensorStatusLED;
lv_obj_t * sensorStatusLEDLabel;
int lastSensorStatus = SENSOR_STATUS_ERROR; // Check last Sensor status before updating to prevent unnecessary Display Updates

// Sensor Temperature
lv_obj_t * sensorTemperatureLabel;

void drawSystemStatus() {
    systemStatusLED  = lv_led_create(lv_scr_act());
    lv_obj_set_size(systemStatusLED, 14, 14);
    lv_obj_set_pos(systemStatusLED, 9, 9);
    lv_led_on(systemStatusLED);
    lv_led_set_brightness(systemStatusLED, 255);
    lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_RED));

    /* Create a label in front of the status LED */
    systemStatusLEDLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(systemStatusLEDLabel, "System");
    lv_obj_align_to(systemStatusLEDLabel, systemStatusLED, LV_ALIGN_OUT_RIGHT_MID, 9, 0);
}

void updateSystemStatus(int systemStatus) {
    if(systemStatus != lastSystemStatus) { // Check, if the status has changed to prevent unnecessary updates
        switch(systemStatus) {
            case SYSTEM_STATUS_ERROR: // Sensor OFFLINE or something really bad happened
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_RED));
                break;
            case SYSTEM_STATUS_WARNING: // Sensor ALIVE but NOT OK
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_ORANGE));
                break;
            case SYSTEM_STATUS_OK: // Sensor ALIVE and OK
                lv_led_set_color(systemStatusLED, lv_palette_main(LV_PALETTE_GREEN));
                break;
        }

        lastSystemStatus = systemStatus;
    }
}

void drawSensorStatus() {
    sensorStatusLED  = lv_led_create(lv_scr_act());
    lv_obj_set_size(sensorStatusLED, 14, 14);
    lv_obj_set_pos(sensorStatusLED, LCD_WIDTH - 19, 9);
    lv_led_on(sensorStatusLED);
    lv_led_set_brightness(sensorStatusLED, 255);
    lv_led_set_color(sensorStatusLED, lv_palette_main(LV_PALETTE_RED));

    /* Create a label in front of the status LED */
    sensorStatusLEDLabel = lv_label_create(lv_scr_act());
    lv_label_set_text(sensorStatusLEDLabel, "Sensor");
    lv_obj_align_to(sensorStatusLEDLabel, sensorStatusLED, LV_ALIGN_OUT_LEFT_MID, -9, 0);
}

void updateSensorStatus(int sensorStatus) {
    if(sensorStatus != lastSensorStatus) { // Check, if the status has changed to prevent unnecessary updates
        switch(sensorStatus) {
            case SENSOR_STATUS_ERROR: // Sensor OFFLINE or something really bad happened
                lv_led_set_color(sensorStatusLED, lv_palette_main(LV_PALETTE_RED));
                break;
            case SENSOR_STATUS_WARNING: // Sensor ALIVE but NOT OK
                lv_led_set_color(sensorStatusLED, lv_palette_main(LV_PALETTE_ORANGE));
                break;
            case SENSOR_STATUS_OK: // Sensor ALIVE and OK
                lv_led_set_color(sensorStatusLED, lv_palette_main(LV_PALETTE_GREEN));
                break;
        }

        lastSensorStatus = sensorStatus;
    }
}

void drawSensorTemperature() {
    sensorTemperatureLabel = lv_label_create(lv_scr_act());
    lv_label_set_recolor(sensorTemperatureLabel, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(sensorTemperatureLabel, "#000000 0°C#");
    lv_obj_set_width(sensorTemperatureLabel, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(sensorTemperatureLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(sensorTemperatureLabel, &lv_font_montserrat_18, 0);
    lv_obj_align(sensorTemperatureLabel, LV_ALIGN_TOP_MID, 0, 5);
}

void updateSensorTemperature(int temperature) {
    String temperatureLabel = "";

    if(temperature >= 30) {
        temperatureLabel = "#ff0000 " + String(temperature) + "°C#"; // RED
        lv_label_set_text(sensorTemperatureLabel, temperatureLabel.c_str());
        return;
    }

    if(temperature >= 25) {
        temperatureLabel = "#ff4000 " + String(temperature) + "°C#"; // Deep Orange
        lv_label_set_text(sensorTemperatureLabel, temperatureLabel.c_str());
        return;
    }

    if(temperature >= 15) {
        temperatureLabel = "#000000 " + String(temperature) + "°C#"; // Black
        lv_label_set_text(sensorTemperatureLabel, temperatureLabel.c_str());
        return;
    }

    if(temperature >= 5) {
        temperatureLabel = "#0080ff " + String(temperature) + "°C#"; // Sky Blue
        lv_label_set_text(sensorTemperatureLabel, temperatureLabel.c_str());
        return;
    }

    if(temperature >= -30 ) {
        temperatureLabel = "#ff0000 " + String(LV_SYMBOL_WARNING) + "#" + " #0000ff " + String(temperature) + "°C#"; // Blue
        lv_label_set_text(sensorTemperatureLabel, temperatureLabel.c_str());
        return;
    }

    lv_label_set_text(sensorTemperatureLabel, "#ff0000 ERROR#");
}