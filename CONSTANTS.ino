//#############SECTION EEPROM STORAGE ADDRESSES#############
#define EEPROM_ADDRESS_OFFSET_H 1
#define EEPROM_ADDRESS_OFFSET_V 2

#define EEPROM_ADDRESS_OFFSET_H_negativ 3
#define EEPROM_ADDRESS_OFFSET_V_negativ 4

#define EEPROM_ADDRESS_FLIP_XY 5

//#############SECTION BUTTONS#############
#define BUTTON_PIN 4 // Switch connected with Pin D4
#define BUTTON_TIME_SET_OFFSET 500
#define BUTTON_TIME_SWITCH_PAGE 30


//#############SECTION ESP NOW #############
uint8_t broadcastAddressEsp8266Sensor[] = {0xA4, 0xCF, 0x12, 0xBF, 0x76, 0x2D};

//#############SECTION DISPLAY COMMANDS #############
#define DISPLAY_COMMAND_SET_OFFSET 0
#define DISPLAY_COMMAND_FLIP_XY 1