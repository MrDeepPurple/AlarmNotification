#ifndef TASK_H
#define TASK_H

#include <UniversalTelegramBot.h>
#include <Arduino.h>
#include <WiFi.h>

#include "Configuration.h"
#include "Constants.h"

void verify_and_connect_wifi(int expiration_ms);

#endif
