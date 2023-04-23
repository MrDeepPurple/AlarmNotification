#ifndef ALARM_NOTIFICATION_H
#define ALARM_NOTIFICATION_H

#include "Configuration.h"
#include "Constants.h"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

void IRAM_ATTR detect_alarm_triggered();

class AlarmNotification
{
  private:
    WiFiClientSecure secured_client;
    UniversalTelegramBot *bot;

    String chatid_str = String(CHAT_ID);
    String alarm_en = String(STATUS_ON);
    String alarm_dis = String(STATUS_OFF);
    String alarm_wrn = String(ALERT_MESSAGE);
    String startup_message = String(BOOT_MESSAGE);

    unsigned long timer_check_alarm = 0, timer_check_chats = 0;
    const unsigned long CHECK_ALARM_PERIOD = TIME_1_SEC, CHECK_CHATS_PERIOD = TIME_1_SEC;

    void handle_alarm_condition();
    void handle_new_requests();

  public:
    void setupAlarmNotificationManager(const String &tg_token, int enable_pin, int alarm_pin);
    void sendStartupConfirmationMessage();
    void runStep();
};

#endif

