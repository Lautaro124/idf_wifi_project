#pragma once

#include <driver/gpio.h>

namespace app
{
  class AppLed
  {
  public:
    void init(void)
    {
      gpio_config_t config{
          config.pin_bit_mask = (1ULL << GPIO_NUM_2),
          config.mode = GPIO_MODE_OUTPUT,
          config.pull_up_en = GPIO_PULLUP_DISABLE,
          config.pull_down_en = GPIO_PULLDOWN_DISABLE,
          config.intr_type = GPIO_INTR_DISABLE};
      gpio_config(&config);
    }
    void set(bool value)
    {
      gpio_set_level(GPIO_NUM_2, static_cast<uint32_t>(value));
    }
  };
}