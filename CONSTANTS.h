//#############SECTION ESP NOW #############
uint8_t broadcastAddressEsp8266Sensor[] = {0xA4, 0xCF, 0x12, 0xBF, 0x76, 0x2D};

#define SENSOR_OFFLINE_TIMEOUT 2000

#define SENSOR_STATUS_ERROR 0   // Sensor OFFLINE or something really bad happened (Peripheral error, or something else)
#define SENSOR_STATUS_WARNING 1 // Sensor ALIVE but NOT OK (Something is weird, but not crucial)
#define SENSOR_STATUS_OK 2      // Sensor ALIVE and OK

//#############SECTION DISPLAY COMMANDS #############
#define DISPLAY_COMMAND_CALIBRATE_SENSOR 0
#define DISPLAY_COMMAND_FLIP_XY 1