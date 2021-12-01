#include <WiFi.h>
#include <esp_now.h>

// Prototypes
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

//###################### GENERAL SENSOR INFO ################################
uint8_t broadcastAddressEsp8266Sensor[] = {0xA4, 0xCF, 0x12, 0xBF, 0x76, 0x2D};

unsigned long sensorLastTimeSeen = 0;

//###################### RECEIVE DATA ################################
bool newDataReceived = false;

// Structure to send data
// Must match the receiver structure
typedef struct struct_sensor_data {
  float horizonAngle;
  float verticalAngle;
  bool flipXYAxis;
  int temperature;
  int sensorStatus;
} struct_sensor_data;

// Create a struct_sensor_data container
struct_sensor_data sensorData;

// callback function that will be executed when data is received
// Keep this function to a minimum, as it's called in the WiFi-task
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&sensorData, incomingData, sizeof(sensorData));
    Serial.print("Bytes received: ");
    Serial.println(len);
    newDataReceived = true;
    sensorLastTimeSeen = millis();
}

//###################### SEND DATA ################################
// Structure to receive data
// Must match the sender structure
typedef struct struct_sensor_command {
  int commandType;
  bool commandValue;
} struct_sensor_command;

// Create a container for  Sensor Commands from the Display
struct_sensor_command sensorCommand;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");

  if(status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Fail");
  }
}

void initEspNow() {
  Serial.println("Init ESP-NOW");
  Serial.print("ESP32 Display-Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddressEsp8266Sensor, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register Receiver Callback Method and get status of received packet
  esp_now_register_recv_cb(OnDataRecv);
}

void sendCalibrateSensor() {
  
  sensorCommand.commandType = DISPLAY_COMMAND_CALIBRATE_SENSOR;
  sensorCommand.commandValue = true;

  // Send message via ESP-NOW
  int result = esp_now_send(broadcastAddressEsp8266Sensor, (uint8_t *) &sensorCommand, sizeof(sensorCommand));
  if (result != 0) {
    Serial.println("Error sending data - DISPLAY_COMMAND_CALIBRATE_SENSOR");
  }

}

void sendFlipXYCommand(bool flipXY) {

  sensorCommand.commandType = DISPLAY_COMMAND_FLIP_XY;
  sensorCommand.commandValue = flipXY;

  // Send message via ESP-NOW
  int result = esp_now_send(broadcastAddressEsp8266Sensor, (uint8_t *) &sensorCommand, sizeof(sensorCommand));
  if (result != 0) {
    Serial.println("Error sending data - DISPLAY_COMMAND_FLIP_XY");
  }

}

void loopEspNow() {
    // Check, if sensorData has been received for some time 
    if(millis() - sensorLastTimeSeen > SENSOR_OFFLINE_TIMEOUT) {
      updateSystemStatus(SYSTEM_STATUS_WARNING);  // System switches to WARNING
      updateSensorStatus(SENSOR_STATUS_ERROR);      // Sensor switches to ERROR
    }

    if(newDataReceived == true){
        Serial.print("HorizonAngle: ");
        Serial.println(sensorData.horizonAngle);
        Serial.print("VerticalAngle: ");
        Serial.println(sensorData.verticalAngle);

        Serial.print("FlipXYAxis: ");
        Serial.println(sensorData.flipXYAxis == 1 ? "True" : "False");

        Serial.print("Temperature: ");
        Serial.println(sensorData.temperature);

        Serial.print("SensorStatus: ");
        Serial.println(sensorData.sensorStatus);
        Serial.println();

        // Now Update the graphics
        updateFlipXYSwitch(sensorData.flipXYAxis);
        updateSensorSliders(sensorData.verticalAngle, sensorData.horizonAngle);
        updateShovelX(sensorData.verticalAngle);
        updateSensorStatus(sensorData.sensorStatus);
        // Reset for the next loop-run
        newDataReceived = false;
    }
}