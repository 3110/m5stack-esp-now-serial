#pragma once

#if defined(TARGET_M5ATOM)
#include <M5Atom.h>
const bool ENABLE_SERIAL = true;
const bool ENABLE_I2C = true;
const bool ENABLE_DISPLAY = true;
#define M5_BEGIN() M5.begin(ENABLE_SERIAL, ENABLE_I2C, ENABLE_DISPLAY)
#define M5_BTN M5.Btn
#elif defined(TARGET_M5STACK)
#include <M5Stack.h>
const bool ENABLE_SERIAL = true;
const bool ENABLE_LCD = true;
const bool ENABLE_SD = true;
const bool ENABLE_I2C = false;
#define M5_BEGIN() M5.begin(ENABLE_LCD, ENABLE_SD, ENABLE_SERIAL, ENABLE_I2C)
#define M5_BTN M5.BtnA
#endif