#include <AppWifi.hpp>
#include <AppMqtt.hpp>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

using namespace app;

extern "C" void app_main()
{
  AppWifi app_wifi = AppWifi();
  AppMqttClient app_mqtt = AppMqttClient();
  ESP_ERROR_CHECK(app_wifi.init());
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  ESP_ERROR_CHECK(app_mqtt.init());
}