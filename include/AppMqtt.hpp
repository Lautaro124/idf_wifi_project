#pragma once

#include <mqtt_client.h>

namespace app
{
  class AppMqttClient
  {
  private:
    const char host[15] = "198.199.86.120";
    const char ssid[8] = "gonraul";
    const char password[9] = "m1ch1fu5";
    const int port = 1883;

  public:
    esp_err_t init(void)
    {
      const esp_mqtt_client_config_t config{
          .broker = AppMqttClient::host,
          .credentials{
              .username = AppMqttClient::ssid,
          }};

      esp_mqtt_client_init(&config);
      return ESP_OK;
    }
  };
}
