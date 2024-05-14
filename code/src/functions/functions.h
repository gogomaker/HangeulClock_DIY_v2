// This .h file's name is "functions.h"
#pragma once

// 헤더파일 정의
#ifndef __FUNCTOINS_H__
#define __FUNCTOINS_H__

// 헤더파일 선언
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <AM2302-Sensor.h>
#ifdef __AVR__
#endif
#include <Wire.h>

/* 상수 선언 */
// 핀번호 지정
#define BT_RX 4 // 조정 필요
#define BT_TX 5 // 조정 필요
#define FLICKER_PIN 3
#define NEO_PIN 6
#define BUZZER_PIN 7
#define DHT_PIN 8
#define MOD_SW_PIN 2
#define LED_SW_PIN 9
#define TIME_SW_PIN 10
#define ALARM_SW_PIN 11
#define EXT_BRIGHT A1
// 기타 상수
#define DHTTYPE DHT22           // DHT 22 (AM2302), AM2321
#define LED_CNT 35              // 네오픽셀 개수
#define COMMA 34                // 쉼표의 LED주소
#define DS3231_I2C_ADDRESS 104  // RTC주소
#define COLOR_CNT 14            // 시계에서 지원하는 컬러개수+1(랜덤포함)
#define INCREASE_BRI 30         // LED의 밝기값 증가폭
#define MAX_BRI 240             // 시간 LED의 최대값
// 스위치 상수 
#define DEB_DLY 50              // 스위치 디바운싱의 기준값
#define SW_INTERVAL 1200        // 스위치 길게 누름의 기준값(ms)
#define BLINK_INTERVAL 150      // 모드 교체 시 LED깜박이는 타이밍
#define NONE 0                  // 스위치 리턴값 없음
#define SHORT 1                 // 스위치 짧게 눌림
#define LONG 2                  // 스위치 길게 눌림
#define MOD_SW 0                // 단순 인덱싱
#define LED_SW 1                // 단순 인덱싱
#define TIME_SW 2                // 단순 인덱싱
#define ALARM_SW 3                // 단순 인덱싱
#define M_CLOCK 0               // 시계 모드 함수
#define M_TEMP  1               // 시계 모드 함수
#define M_HUMID 2               // 시계 모드 함수

/* 변수 및 객체 선언 */
// 객체선언
extern Adafruit_NeoPixel strip;
//형상에 대한 배열 선언
extern const byte shapeO[8], shapeX[8];
extern const byte numberTEN_segment[10][13], numberONE_segment[10][13];
// LED 관련
extern const byte color[COLOR_CNT][4];
extern long rSeed;
extern byte r, g, b, w, bright, ledmode;
// 시간 관련
extern unsigned long time;
extern byte sec, lastSec, lastClockSec, min, hour;
extern bool isResetMillis;
extern bool isClockChange;			// 시간 수정여부
// 스위치 관련
extern bool sw_org_stat[4], l_sw_stat[4];
extern unsigned long l_deb_tme[4], sw_w[4];
extern int swpin[4], sw_prcs_val[4];
// 온습도 관련
extern float celsius, humidity;	    //섭씨, 습도 
extern unsigned long dhtshowTime;
extern bool isEnableShowDht;
extern unsigned long l_tempshow, tempshow;
// 알람 관련
extern byte almHour, almMin, almSec;	// 시 분 초
extern bool isonAlarm;			// 알람기능이 켜져 있는가
extern bool isAlarmChange;				// 알람 수정여부
extern unsigned long l_showAstat_Time;		//알람 상태를 보여주기 시작한 시간 기록 변수
// 플리커 관련
extern byte flick_bri;
// 모드 관련
extern byte clock_mode;
extern bool isChangeMode;

/* 함수 선언 */
// Function for Displaying Time
void displayTime(int h, int m);
void updateHour(int h);
void updateMin(int m);
// Function for Sensing Switch
int sensingSW(int index);
// Function for controlling LED
void changeLEDbright();
void changeLEDcolor();
void printled(int n);
void ledclear();
void startMotion();
void blink();
// Function for alarm
void increasingAlmHour();
void increasingAlmMin();
void changeAlmStat();
void showAlmStat(bool t);
void alarmMotion();
// Function for controlling time
void increasingHour();
void increasingMin();
// ETC code
void showSerialTime();
void showSEGnum(int digit, int num, int isON);
// Core Function
void showClock();
void showTnH(int mode, float c, float h);
#endif