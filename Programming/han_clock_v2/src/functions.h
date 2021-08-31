//This .h file's name is "functions.h"
#pragma once

// 헤더파일 정의
#ifndef __FUNCTOINS_H_
#define __FUNCTOINS_H_

// 헤더파일 선언
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#ifdef __AVR__
#endif
#include <Wire.h>

// 상수 선언
#define DS3231_I2C_ADDRESS 104
#define LED_PIN 13
#define BUTTON_T 2
#define BUTTON_LED 3
#define LED_COUNT 35
#define debounceDelay 50
#define bu_interval 1200

// 변수 및 객체 선언
extern Adafruit_NeoPixel strip;

extern byte color[12][3];

extern unsigned long time;
extern byte sec, lastSec;
extern byte hourPlus, minPlus;
extern byte minRtc, hourRtc;
extern byte min, hour;
extern bool isResetMillis;
extern byte tMSB, tLSB;
extern float temp3231;
extern byte r;
extern byte g;
extern byte b;
extern byte bright;
extern byte ledmode;
extern long randomBase;
extern unsigned long bu_led_w;
extern bool ledCheck;
extern bool bu_reading[3];
extern bool bu_state[3];
extern bool last_bu_state[3];
extern unsigned long LastDebounceTime[3];
extern unsigned long bu_t_w, last_bu_t_w;
extern unsigned long wait_t, wait_m;
extern bool timeCheck;
extern bool isblinkH, isblinkM;
extern byte tchange;

// 함수 선언
void displayTime(int h, int m);
int updateHour(int h);
int updateMin(int m);
void printled(int n);
void ledclear();
void changeTimeButton();
void changeLedButton();
void longTimeButton();
void longLedButton();
byte decToBcd(byte val);
void set3231Date();
void get3231Date();
float get3231Temp();
void blinkHM();
void showSerialTime();

#endif