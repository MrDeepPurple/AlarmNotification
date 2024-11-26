#include "NotificationLib.h"

namespace notifications
{
  
NotificationLib::NotificationLib()
{
  // install telegram certificate root
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  bot = new UniversalTelegramBot(tg_token, secured_client);
}

bool NotificationLib::getCommand(String &msg)
{
  bool newmsg = false;
  int numNewMessages = bot->getUpdates(bot->last_message_received + 1);
  if(numNewMessages > 0)
  {
    msg = String(bot->messages[numNewMessages - 1].text);
    newmsg = true;
  }
  return newmsg;
}

void NotificationLib::send(String message)
{
  // notify to all available chats that the system has restarted
  bot->sendMessage(chatid_str, message);
}

}