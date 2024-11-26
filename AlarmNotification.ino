#include <WiFi.h>

#include "Configuration.h"
#include "Constants.h"
#include "AlarmNotification.h"

AlarmNotification alarmnotify;

unsigned long timer_check_wifi = 0;
unsigned long timer_check_alarm_state = 0;
unsigned long timer_check_alarm_msgs = 0;

void setup() {
  // start serial port
  DEBUG_begin(115200);
  delay(100);

  // We start by connecting to a WiFi network
  DEBUG("Connecting to ");
  DEBUG_ln(WIFI_SSID);

  WiFi.setHostname(WIFI_HOSTNAME); //define hostname
  WiFi.begin(WIFI_SSID, WIFI_KEY);
  while (WiFi.status() != WL_CONNECTED)
  {
    DEBUG(".");
    delay(500);
  }
  DEBUG("\nWiFi connected. IP address: ");
  DEBUG_ln(WiFi.localIP());

  DEBUG("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    DEBUG(".");
    delay(100);
    now = time(nullptr);
  }
  DEBUG_ln(now);  

  alarmnotify.setupAlarmNotificationManager(BOT_TOKEN, ENABLED_ALARM_PIN, TRIGGERED_ALARM_PIN);
}

void loop()
{
  TASK(timer_check_alarm_state, TASK_CHECK_ALARM, alarmnotify.checkAlarmState());
  if(WiFi.status() == WL_CONNECTED)
  {
    TASK(timer_check_alarm_msgs, TASK_CHECK_NEW_CHATS, alarmnotify.checkNewRequests())
  }
  TASK(timer_check_wifi, TASK_CHECK_WIFI, alarmnotify.checkAndReconnectWifi(CHECK_WIFI_TIMEOUT))
}
