#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//--------------------- PIN config ------------------------//
/************************************************************
 * This section provides the relevant configuration for the  
 * pins connected to the alarm main board. Currently two
 * pins shall be connected
 *
 * ENABLED_ALARM_PIN is used to verify if the alarm is 
 *      enabled or disabled. It shall be connected to to a
 *      line in the main board which provides high voltage
 *      when the alarm is enabled
 *
 * TRIGGERED_ALARM_PIN shall be connected to the alarm 
 *      sounder in the main board 
 *
 ***********************************************************/
#define ENABLED_ALARM_PIN 26
#define TRIGGERED_ALARM_PIN 27 

//--------------------- Wifi config ------------------------//
/************************************************************
 * This section provides the relevant configuration for the  
 * wifi connection. In particular three strings shall be
 * provided: SSID, KEY and HOSTNAME
 *
 ***********************************************************/
// wifi network name (SSID)
#define WIFI_SSID       "your-wifi-ssid"
// wifi network password
#define WIFI_KEY        "your-wifi-password"
// hostname of the device in current wifi networs
#define WIFI_HOSTNAME   "ESP32 Alarm Panel"

//------------------- Telegram config ----------------------//
/************************************************************
 * This section provides the relevant configuration for the  
 * telegram bot. It requires the token of the bot and a list
 * of chat IDs to whom the notifications will be sent
 *
 ***********************************************************/
// telegram bot token as provided by botfather  
#define BOT_TOKEN "your-telegram-bot-token"
// telegram chat id list (Currently only one chat is supported)
#define CHAT_ID 00000000 // replace with your chat id

//---------------- Notification messages -------------------//
/************************************************************
 * This section provides the strings related to the
 * notification messages. Change them or translate them as
 * you like.
 *
 ***********************************************************/
#define STATUS_ON "Centrale abilitata"
#define STATUS_OFF "Centrale disabilitata"
#define ALERT_MESSAGE "Allarme in corso!"
#define BOOT_MESSAGE "ESP32 riavviato e operativo"
#define LOG_CONN_REST "connection restored"
#define LOG_CONN_LOST "Connection lost"
#define LOG_STARTED "System started"
#define LOG_ALARM_ENABLED "Alarm enabled"
#define LOG_ALARM_DISABLED "Alarm disabled"
#define LOG_ALARM_TRIGGERED "Alarmed"
#define LOG_ALARM_BACKTONORMAL "Alarm stopped"
#define LOG_HEADER "Last events:"
#define LOG_EMPTY "No event registered"
#define EVENT_LOG_SIZE 20

//--------------------- Misc config ------------------------//
/************************************************************
 * This section provides generic functionalities
 ***********************************************************/
// uncomment to enable serial debug
//#define DBG_EN

#endif
