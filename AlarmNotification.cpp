#include "AlarmNotification.h"

volatile bool alarm_triggered;

void IRAM_ATTR detect_alarm_triggered() 
{
  alarm_triggered = true;
}


void AlarmNotification::setupAlarmNotificationManager(const String &tg_token, int enable_pin, int alarm_pin)
{
  // Setup pins
  pinMode(enable_pin, INPUT);
  // Setup interrupts
  attachInterrupt(digitalPinToInterrupt(alarm_pin), detect_alarm_triggered, RISING);
  // install telegram certificate root
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

  bot = new UniversalTelegramBot(tg_token, secured_client);
}

void AlarmNotification::sendStartupConfirmationMessage()
{
  // notify to all available chats that the system has restarted
  bot->sendMessage(chatid_str, startup_message);
}

void AlarmNotification::handle_new_requests()
{
  int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
  if(numNewMessages > 0)
  {
    if (bot->messages[numNewMessages - 1].text == "/status")
    {
      if(digitalRead(ENABLED_ALARM_PIN))
        bot->sendMessage(chatid_str, alarm_en);
      else
        bot->sendMessage(chatid_str, alarm_dis);
    }
  }
  
}

void AlarmNotification::handle_alarm_condition()
{
  // verify alarm triggered status and wifi status.
  // in order to avoid fluctuation
  bool alarm_active = digitalRead(ENABLED_ALARM_PIN);

  // invalid trigger, alarm is inactive
  if(alarm_triggered && !alarm_active)
  {
    alarm_triggered = false;
  }
  //
  if(alarm_triggered && alarm_active)
  {
    // wifi is available and alarm is triggere. 
    // Acknowledge alarm
    alarm_triggered = false;
    // send telegram notification
    bot->sendMessage(chatid_str, alarm_wrn);
  }
}

void AlarmNotification::runStep()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    if(millis() - timer_check_alarm > CHECK_ALARM_PERIOD) { 
      handle_alarm_condition();
      timer_check_alarm = millis();
    }

    if(millis() - timer_check_chats > CHECK_CHATS_PERIOD) { 
      handle_new_requests();
      timer_check_alarm = millis();
    }
  }
}