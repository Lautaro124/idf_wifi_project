#pragma once

#include <mqtt_client.h>
#include <esp_log.h>

const char *TAGMQTT = "mqtt_log";
namespace app
{
  class AppMqttClient
  {
  private:
    const char host[21] = "mqtt://192.168.0.209";

    static void log_error_if_nonzero(const char *message, int error_code)
    {
      if (error_code != 0)
      {
        ESP_LOGE(TAGMQTT, "Last error %s: 0x%x", message, error_code);
      }
    }

    static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
    {
      ESP_LOGD(TAGMQTT, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
      esp_mqtt_event_handle_t event = static_cast<esp_mqtt_event_handle_t>(event_data);
      esp_mqtt_client_handle_t client = event->client;
      int msg_id;
      switch ((esp_mqtt_event_id_t)event_id)
      {
      case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, "/conection", "OK", 0, 1, 2);
        ESP_LOGI(TAGMQTT, "sent publish successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "/light", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;

      case MQTT_EVENT_DISCONNECTED:
        ESP_LOGE(TAGMQTT, "MQTT_EVENT_DISCONNECTED");
        break;

      case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/conection", "Succses", 0, 0, 0);
        ESP_LOGI(TAGMQTT, "sent publish successful, msg_id=%d", msg_id);
        break;

      case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;

      case MQTT_EVENT_DATA:
        ESP_LOGI(TAGMQTT, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;

      case MQTT_EVENT_ERROR:
        ESP_LOGE(TAGMQTT, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
          log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
          log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
          log_error_if_nonzero("captured as transport's socket errno", event->error_handle->esp_transport_sock_errno);
          ESP_LOGI(TAGMQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
      default:
        ESP_LOGI(TAGMQTT, "Other event id:%d", event->event_id);
        break;
      }
    }

  public:
    esp_err_t init(void)
    {
      esp_mqtt_client_config_t config = {
          .broker = {
              .address = {
                  .uri = AppMqttClient::host,
                  .port = 1883,
              },
          },
      };
      esp_mqtt_client_handle_t client = esp_mqtt_client_init(&config);
      esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, client);
      esp_mqtt_client_start(client);
      return ESP_OK;
    }
  };
}
