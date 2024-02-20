#include <AppWifi.h>

using namespace app;
extern "C" void app_main()
{
  AppWifi app_wifi = AppWifi();
  app_wifi.init();
}