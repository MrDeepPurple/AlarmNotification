#ifndef CONSTANTS_H
#define CONSTANTS_H

#define EVAL_TIME_MS 500
#define TIME_1_SEC 1000
#define TIME_1_MIN 60 * TIME_1_SEC
#define TASK_CHECK_ALARM 10 * TIME_1_SEC
#define PHASE_CHECK_ALARM 0
#define TASK_CHECK_NEW_CHATS TIME_1_MIN * 10
#define PHASE_CHECK_NEW_CHATS 1
#define TASK_CHECK_WIFI TIME_1_MIN * 5
#define PHASE_CHECK_WIFI 2
#define CHECK_WIFI_TIMEOUT 30 * TIME_1_SEC

#define TASK(timer, interval, task_fnc) \
  if(millis() - timer > (interval)) { \
    task_fnc;\
    timer = millis();\
  }

#ifdef DBG_EN
  #define DEBUG_begin(s) Serial.begin(s)
  #define DEBUG_ln(str) Serial.println(str)
  #define DEBUG(str) Serial.print(str)
#else
  #define DEBUG_begin(s) 
  #define DEBUG_ln(str) 
  #define DEBUG(str) 
#endif

#endif