//#############SECTION HARDWARE #############

#define DISP_HOR_RES 320
#define DISP_VER_RES 480

#define TOUCH_I2C_PORT              I2C_NUM_1

#define CONFIG_LV_TOUCH_I2C_SDA     I2C_SDA
#define CONFIG_LV_TOUCH_I2C_SCL     I2C_SCL

//#############SECTION ESP NOW #############
#define SENSOR_OFFLINE_TIMEOUT 2000

#define SENSOR_STATUS_ERROR 0   // Sensor OFFLINE or something really bad happened (Peripheral error, or something else)
#define SENSOR_STATUS_WARNING 1 // Sensor ALIVE but NOT OK (Something is weird, but not crucial)
#define SENSOR_STATUS_OK 2      // Sensor ALIVE and OK

//#############SECTION DISPLAY COMMANDS #############
#define DISPLAY_COMMAND_CALIBRATE_SENSOR 0
#define DISPLAY_COMMAND_FLIP_XY 1