/**************************************************************************$
 * Copyright (C) 2022 Andre Riesberg, andre@riesberg-net.de
 *
 * Redistribution and use in source and binary forms of this file,
 * with or without modification, are permitted. See the Creative
 * Commons Zero (CC0 1.0) License for more details.
 ***************************************************************************/
#ifndef MAIN_H
#define MAIN_H
//---------------------------------------------------------------------------
#include <Arduino.h>
#include <stdint.h>
#include <limits.h>
//---------------------------------------------------------------------------
//#define TF_LOG_LEVEL TF_LOG_LEVEL_INFO
#define TF_LOG_LEVEL TF_LOG_LEVEL_DEBUG
//#include "bindings/config.h"
//---------------------------------------------------------------------------
#define VERSION "V1.0a"
#define APP_NAME "Minimal"
//---------------------------------------------------------------------------
#define HAS_OTA
//---------------------------------------------------------------------------
#undef LOG_LOCAL_LEVEL
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#undef ARDUHAL_LOG_LEVEL
#define ARDUHAL_LOG_LEVEL ARDUHAL_LOG_LEVEL_VERBOSE
#include "esp_log.h"
//---------------------------------------------------------------------------
#endif