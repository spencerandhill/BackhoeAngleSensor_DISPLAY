#include <WiFi.h>
#include <esp_now.h>

// Prototypes
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

bool newDataReceived = false;

// Structure to send data
// Must match the receiver structure
typedef struct struct_message {
  float horizonAngle;
  float verticalAngle;
  bool flipXYAxis;
  int temperature;
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
}

void initEspNow() {
  Serial.println("Init ESP-NOW");
  Serial.print("ESP32 Display-Board MAC Address:  ");

  // Set device as a Wi-Fi Station and print MacAddress
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register Receiver Callback Method and get status of received packet
  esp_now_register_recv_cb(OnDataRecv);
}

void loopEspNow()
{
    if(newDataReceived == true){
        Serial.print("HorizonAngle: ");
        Serial.println(sensorData.horizonAngle);
        Serial.print("VerticalAngle: ");
        Serial.println(sensorData.verticalAngle);

        Serial.print("FlipXYAxis: ");
        Serial.println(sensorData.flipXYAxis == 1 ? "True" : "False");
        Serial.println();

        Serial.print("Temperature: ");
        Serial.println(sensorData.temperature);

        // Now Update the graphics
    //   updateSliderXY(sensorData.verticalangle, sensordata.horizonangle);
    //   updateshovelx(sensordata.verticalangle);

        // Reset for the next loop-run
        newDataReceived = false;
    }
}