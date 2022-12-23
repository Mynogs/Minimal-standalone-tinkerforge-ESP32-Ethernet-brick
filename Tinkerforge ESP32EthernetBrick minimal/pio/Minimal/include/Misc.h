/**************************************************************************$
 * Copyright (C) 2013-2022 Ing. Buero Riesberg - All Rights Reserved
 * Unauthorized copy, print, modify or use of this file, via any medium is
 * strictly prohibited. Proprietary and confidential.
 * Written by Andre Riesberg <andre@riesberg-net.de>
 ***************************************************************************/
#ifndef MISC_H
#define MISC_H
//---------------------------------------------------------------------------
#include "main.h"
#include <stdint.h>
//---------------------------------------------------------------------------
#define TIME_MS(TMS) ((TMS + portTICK_RATE_MS - 1) / portTICK_RATE_MS)
#define TIME_S(TS) (((TS * 1000) + portTICK_RATE_MS - 1) / portTICK_RATE_MS)
//---------------------------------------------------------------------------
#define TRACE printf("%u\n", __LINE__);
//---------------------------------------------------------------------------
extern int32_t max(int32_t a, int32_t b);
extern float32_t normalize(float32_t value, float32_t range);
extern float32_t interpolate(float32_t value0, float32_t value1, float32_t slide);
//---------------------------------------------------------------------------
#endif