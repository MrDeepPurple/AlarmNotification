#ifndef NOTIFICATION_LIB_H
#define NOTIFICATION_LIB_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include "Constants.h"
#include "Configuration.h"

namespace notifications
{
  class NotificationLib
  {
    private:
      WiFiClientSecure secured_client;
      UniversalTelegramBot *bot;
      String chatid_str = String(CHAT_ID);
      String tg_token = String(BOT_TOKEN);

    public:
      NotificationLib();
      bool getCommand(String &msg);
      void send(String message);
  };
}

#endif