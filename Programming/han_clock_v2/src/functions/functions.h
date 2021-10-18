// This .h file's name is "functions.h"
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
#define BT_RX 4 // 조정 필요
#define BT_TX 5 // 조정 필요

#define FLICKER 3
#define NEOPIN 6
#define BUZZER 7
#define DHTPIN 8
#define MODE_SW 2
#define LED_SW 9
#define TIME_SW 10
#define ALARM_SW 11
#define EXT_BRIGHT A1
// 한글시계 핀

// 기타 상수
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define LED_CNT 35
#define DS3231_I2C_ADDRESS 104
#define COLOR_CNT 13
#define INCREASE_BRI 30
#define MAX_BRI 210
// 스위치 상수
#define DEB_DLY 50
#define SW_INTERVAL 1200
#define BLINK_INTERVAL 150
#define TME_INDEX 0
#define LED_INDEX 1
#define ALM_INDEX 2
#define MOD_INDEX 3

/* 변수 및 객체 선언 */
// 객체선언
extern Adafruit_NeoPixel strip;
extern DHT dht;
// LED 관련
extern byte color[13][4];
extern long rSeed;
extern byte r, g, b, w, bright, ledmode;
extern bool isblinkLED;
// 시간 관련
extern unsigned long time;
extern byte sec, lastSec, hourPlus, minPlus, min, hour;
extern byte minRtc, hourRtc;
extern bool isResetMillis;
// RTC기능 관련
extern byte tMSB, tLSB;
extern float temp3231;
// 스위치 관련
extern bool sw_reading[4], sw_stat[4], l_sw_stat[4], sw_check[4];
extern unsigned long l_deb_tme[4], sw_w[4];
extern unsigned long wait_t;
extern bool isTchange, isAchange;
// 온습도 관련
extern float temp, humi, f;
// 알람 관련
extern byte alarm_time[3];
// 아날로그 밝기 제어 관련
extern int ext_bri;
// 플리커 관련
extern byte flick_bri;
// 모드 관련
extern byte clock_mode;

/* 함수 선언 */
void displayTime(int h, int m);
int updateHour(int h);
int updateMin(int m);
void printled(int n);
void ledclear();
void blinkAllLED();

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

void showSerialTime();

#endif