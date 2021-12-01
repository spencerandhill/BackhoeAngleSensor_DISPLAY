void checkSystemStatus() {
    int systemStatus = sensorData.sensorStatus == SENSOR_STATUS_ERROR ? SYSTEM_STATUS_WARNING : SYSTEM_STATUS_OK;
    
    updateSystemStatus(SYSTEM_STATUS_OK);  // System switches to WARNING
}