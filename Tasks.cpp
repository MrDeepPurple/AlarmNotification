#include "Tasks.h"
#include "Constants.h"


void verify_and_connect_wifi(int expiration_ms)
{
  int elapsed = 0;
  // verify that wifi is not connected
  if(WiFi.status() != WL_CONNECTED)
  {
    // connect
    WiFi.begin(WIFI_SSID, WIFI_KEY);
  }

  // wait until connected or expired
  while (WiFi.status() != WL_CONNECTED) {
      delay(EVAL_TIME_MS);
      DEBUG(".");

      elapsed += EVAL_TIME_MS;
      if(expiration_ms > 0 && elapsed > expiration_ms)
      {
        break;
      }
  }
}
