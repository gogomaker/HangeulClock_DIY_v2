//This .h file's name is "functions.h"
#pragma once

// 헤더파일 정의
#ifndef __FUNCTOINS_H__
#define __FUNCTOINS_H__

// 헤더파일 선언
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include <avr/power.h>
#ifdef __AVR__
#endif
#include <Wire.h>

/* 상수 선언 */
// 핀번호 지정

#define BT_RX 4     //조정 필요
#define BT_TX 5     //조정 필요

#define FLICKER 3
#define NEOPIN 6
#define BUZZER 7
#define DHTPIN 8
#define MODE_SW 2
#define LED_SW 9
#define TIME_SW 10
#define ALARM_SW 11
#define EXT_BRIGHT A1

//기타 상수
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define LED_CNT 35
#define DS3231_I2C_ADDRESS 104
#define DEB_DLY 50
#define BU_INTERVAL 1200

#define TME_INDEX 0
#define LED_INDEX 1
#define ALM_INDEX 2
#define MOD_INDEX 3

// 변수 및 객체 선언
extern Adafruit_NeoPixel strip;

extern byte color[13][4];

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
extern bool bu_stat[4];
extern bool l_bu_stat[4];
extern unsigned long l_deb_tme[4];
extern unsigned long bu_t_w;
extern unsigned long wait_t, wait_m;
extern bool timeCheck;
extern bool isblinkH, isblinkM;
extern bool isTchange;

// 함수 선언
void displayTime(int h, int m);
int updateHour(int h);
int updateMin(int m);
void printled(int n);
void ledclear();
void changeTimeSW();
void changeLedSW();
void changeModeSW();
void changeAlarmSW();
void longTimeSW();
void longLedSW();
byte decToBcd(byte val);
void set3231Date();
void get3231Date();
float get3231Temp();
void blinkHM();
void showSerialTime();

#endif