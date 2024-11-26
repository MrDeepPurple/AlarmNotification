#include "AlarmNotification.h"

 /*
  * @brief Initialization function. Telegram bot and pin setup. At the end of the function
  * sends a confirmation message
  * @param
  * @return
  */
void AlarmNotification::setupAlarmNotificationManager(const String &tg_token, int en_pin, int al_pin)
{
  evtbuff = new eventbuffer::EventBuffer();
  notify = new notifications::NotificationLib();

  enabled_pin = en_pin;
  alarm_pin = al_pin;
  alarm_msg_latch = false;
  alarm_en_latch = true;

  // Setup pins
  pinMode(enabled_pin, INPUT);
  pinMode(alarm_pin, INPUT);

  // notify to all available chats that the system has restarted
  notify->send(startup_message);
  evtbuff->add(eventbuffer::event_t::STARTED);
}

 /*
  * @brief Cyclic function. Check if a request has been sent through Telegram and It calls
  * the appropriate processing function
  * @param
  * @return
  */
void AlarmNotification::checkNewRequests()
{
  String msg;
  bool newmsgreceived = notify->getCommand(msg);
  if (newmsgreceived)
  {
    if (msg == "/status")
    {
      send_triggered_state();
    }
    else if (msg == "/log")
    {
      send_alarm_log();
    }
  }
}

 /*
  * @brief send a message through telegram reporting the state of the alarm
  * (enabled/disabled)
  * @param
  * @return
  */
void AlarmNotification::send_triggered_state()
{
  if(digitalRead(enabled_pin))
      notify->send(alarm_en);
  else
      notify->send(alarm_dis);
}

 /*
  * @brief send the event log as Telegram message
  * @param
  * @return
  */
void AlarmNotification::send_alarm_log()
{
  // get a iterator to the event buffer
  eventbuffer::EventBuffer::iterator* itr = evtbuff->getIterator();

  // create a string containing the final message starting from
  // the LOG_HEADER string
  String msg = String(LOG_HEADER);
  
  // log should never be empty
  if(!itr->empty())
  {
    // for each entry in the log, print a message based
    // on the type of event
    do 
    {
      msg.concat("\n[");
      msg.concat(itr->getDate());
      msg.concat("]");

      switch(itr->getEvent())
      {
        case eventbuffer::event_t::CONN_REST:
          msg.concat(LOG_CONN_REST);
          break;
        case eventbuffer::event_t::CONN_LOST:
          msg.concat(LOG_CONN_LOST);
          break;
        case eventbuffer::event_t::STARTED:
          msg.concat(LOG_STARTED);
          break;
        case eventbuffer::event_t::ALARM_ENABLED:
          msg.concat(LOG_ALARM_ENABLED);
          break;
        case eventbuffer::event_t::ALARM_DISABLED:
          msg.concat(LOG_ALARM_DISABLED);
          break;
        case eventbuffer::event_t::ALARM_TRIGGERED:
          msg.concat(LOG_ALARM_TRIGGERED);
          break;
        case eventbuffer::event_t::ALARM_BACKTONORMAL:
          msg.concat(LOG_ALARM_BACKTONORMAL);
          break;
      }
    }
    while(itr->next());
  }
  else
  {
    // otherwise print empty log message
    msg.concat(LOG_EMPTY);
  }

  // clear the iterator
  delete itr;

  // send the message through Telegram
  notify->send(msg);
}

 /*
  * @brief cyclic function. Check the state of the alarm (enabled/disabled) and its condition
  * (triggered/normal). Send a notification through Telegram if the alarm is triggered
  * @param
  * @return
  */
void AlarmNotification::checkAlarmState()
{
  // verify alarm enabled status
  bool alarm_enabled = digitalRead(enabled_pin);
  // verify alarm triggering condition
  bool alarm_triggered = digitalRead(alarm_pin);

  if(alarm_enabled)
  {
    // if alarm enabled latch is not set, then this is
    // the first read of an alarm enabled. A new event
    // shall be added to the log
    if(!alarm_en_latch)
    {
      // Set the latch to avoid log repetitions
      alarm_en_latch = true;
      // log alarm enabled
      evtbuff->add(eventbuffer::event_t::ALARM_ENABLED);
    }

    // if alarm is triggered check latch state
    // and store log and send message
    if(alarm_triggered && !alarm_msg_latch)
    {
      // Set the latch to avoid message repetitions
      alarm_msg_latch = true;
      // send telegram notification only if wifi is
      // connected (otherwise system will be blocked
      // on message send
      if(WiFi.status() == WL_CONNECTED)
      {
        notify->send(alarm_wrn);
      }

      // add alarm enabled to the event buffer
      evtbuff->add(eventbuffer::event_t::ALARM_TRIGGERED);
    }

  }

  // reset alarm message latch. Messages are
  // allowed again
  if(alarm_msg_latch && (!alarm_enabled || !alarm_triggered))
  {
    alarm_msg_latch = false;
    evtbuff->add(eventbuffer::event_t::ALARM_BACKTONORMAL);
  }

  // reset alarm log latch. Store alarm log
  if(alarm_en_latch && !alarm_enabled)
  {
    alarm_en_latch = false;
    evtbuff->add(eventbuffer::event_t::ALARM_DISABLED);
  }
}

 /*
  * @brief Cyclic function. Check the status of the WiFi network and try to
  * reconnect in case of connection lost
  * @param
  * @return
  */
void AlarmNotification::checkAndReconnectWifi(int expiration_ms)
{
  int elapsed = 0;
  // verify that wifi is not connected
  if(WiFi.status() != WL_CONNECTED)
  {
    // connect
    WiFi.begin(WIFI_SSID, WIFI_KEY);

    if(!evt_disconnect_latch)
    {
      evt_disconnect_latch = true;
      evtbuff->add(eventbuffer::event_t::CONN_LOST);
    }

  }

  // wait until connected or expired
  while (WiFi.status() != WL_CONNECTED) {
      delay(EVAL_TIME_MS);
      elapsed += EVAL_TIME_MS;
      if(expiration_ms > 0 && elapsed > expiration_ms)
      {
        break;
      }
  }

  if(evt_disconnect_latch && WiFi.status() == WL_CONNECTED)
  {
    evtbuff->add(eventbuffer::event_t::CONN_REST);
    evt_disconnect_latch = false;
  }
}
