#pragma once

#include <esp_wifi.h>
#include <esp_netif.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include "esp_log.h"

#define SSID "WIFI GONZALEZ 2.4"
#define PASSWORD "GONZALEZ12"
const char *TAG = "main";

namespace app
{
  class AppWifi
  {
  private:
    static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
    {
      switch (event_id)
      {
      case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG, "WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
      case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
      case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGE(TAG, "WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
        break;
      case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "WiFi got IP ... \n");
        break;
      default:
        break;
      }
    }

  public:
    esp_err_t init()
    {
      nvs_flash_init();
      esp_netif_init();
      esp_event_loop_create_default();
      esp_netif_create_default_wifi_sta();
      wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
      esp_wifi_init(&wifi_init_config);
      esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
      esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
      wifi_config_t wifi_config = {
          .sta{
              .ssid = SSID,
              .password = PASSWORD,
          }};
      esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
      esp_wifi_set_mode(WIFI_MODE_STA);
      esp_wifi_start();
      esp_wifi_connect();
      return ESP_OK;
    }
  };
}
