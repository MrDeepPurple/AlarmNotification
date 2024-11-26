#ifndef ALARM_NOTIFICATION_H
#define ALARM_NOTIFICATION_H

#include "Configuration.h"
#include "Constants.h"
#include "EventBuffer.h"
#include "NotificationLib.h"

class AlarmNotification
{
  private:

    int enabled_pin;
    int alarm_pin;

    bool alarm_msg_latch;
    bool alarm_en_latch;
    bool evt_disconnect_latch;

    String alarm_en = String(STATUS_ON);
    String alarm_dis = String(STATUS_OFF);
    String alarm_wrn = String(ALERT_MESSAGE);
    String startup_message = String(BOOT_MESSAGE);
    
    eventbuffer::EventBuffer *evtbuff;
    notifications::NotificationLib *notify;

    const unsigned long CHECK_ALARM_PERIOD = TIME_1_SEC, CHECK_CHATS_PERIOD = TIME_1_SEC;

    void send_triggered_state();
    void send_alarm_log();

  public:
    void setupAlarmNotificationManager(const String &tg_token, int enable_pin, int alarm_pin);
    void checkNewRequests();
    void checkAlarmState();
    void checkAndReconnectWifi(int expiration_ms);
};

#endif

