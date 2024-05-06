/*
	Name:		code.ino
	Created:	2024-05-04(Sat), 16:28:48
	Author:		Calub Son
*/

/* 헤더파일 선언 */
#include "src\functions\functions.h"

/* 실제 변수 및 객체 선언 */
const byte color[COLOR_CNT][4] {//R  , G  , B  , W
	{0  , 0  , 0  , 0  },		// 랜덤모드라서 값이 없음
	{0  , 0  , 0  , 255},		// 하얀색(warm white)
	{200, 200, 200, 0  },		// 하얀색(cool white)
	{255, 0  , 0  , 0  },		// 붉은색
	{255, 94 , 0  , 0  },		// 선홍색
	{255, 187, 0  , 0  },		// 주황색
	{255, 228, 0  , 0  },		// 노란색
	{171, 242, 0  , 0  },		// 연두색
	{29 , 219, 22 , 0  },		// 초록색
	{0  , 84 , 255, 0  },		// 하늘색
	{1  , 0  , 255, 0  },		// 파란색
	{95 , 0  , 255, 0  },		// 보라색
	{255, 0  , 221, 0  },		// 분홍색
	{255, 0  , 100, 0  }};		// 분홍과 빨강 사이 어딘가

// 형상에 대한 배열 선언
const byte shapeO[8] = {7, 8, 12, 21, 25, 26, 18, 15};		// O
const byte shapeX[8] = {27, 19, 13, 9, 24, 20, 14, 6};		// X
const byte numberTEN_segment[10][13] {
	{31, 30, 29, 28, 17, 16,  5,  6,  7, 14, 19, 26,  0},	// 0
	{30, 27, 18, 15,  6,  0,  0,  0,  0,  0,  0,  0,  0},	// 1
	{29, 30, 31, 26, 19, 18, 17, 16,  5,  6,  7,  0,  0},	// 2
	{29, 30, 31, 26, 19, 14,  7,  6,  5,  0,  0,  0,  0},	// 3
	{29, 28, 17, 18, 19, 31, 26, 14,  7,  0,  0,  0,  0},	// 4
	{31, 30, 29, 28, 17, 18, 19, 14,  7,  6,  5,  0,  0},	// 5
	{31, 30, 29, 28, 17, 16,  5,  6,  7, 14, 19, 18,  0}, 	// 6
	{17, 28, 29, 30, 31, 26, 19, 14,  7,  0,  0,  0,  0},	// 7
	{26, 31, 30, 29, 28, 17, 18, 19, 14,  7,  6,  5, 16},	// 8
	{18, 17, 28, 29, 30, 31, 26, 19, 14,  7,  6,  5,  0}};	// 9
const byte numberONE_segment[10][13] {
	{32, 33, 34, 23, 22, 11, 10,  9,  8, 13, 20, 25,  0},	// 0
	{33, 24, 21, 12,  9,  0,  0,  0,  0,  0,  0,  0,  0},	// 1
	{32, 33, 34, 23, 22, 21, 20, 13,  8,  9, 10,  0,  0},	// 2
	{32, 33, 34, 23, 22, 21, 20, 11, 10,  9,  8,  0,  0},	// 3
	{32, 25, 20, 21, 34, 23, 22, 11, 10,  0,  0,  0,  0},	// 4
	{34, 33, 32, 25, 20, 21, 22, 11, 10,  9,  8,  0,  0},	// 5
	{34, 33, 32, 25, 20, 13,  8,  9, 10, 11, 22, 21,  0},	// 6
	{20, 25, 32, 33, 34, 23, 22, 11, 10,  0,  0,  0,  0},	// 7
	{23, 34, 33, 32, 25, 20, 21, 22, 11, 10,  9,  8, 13},	// 8
	{21, 20, 25, 32, 33, 34, 23, 22, 11, 10,  9,  8,  0}};	// 9

// 객체 선언
Adafruit_NeoPixel strip(LED_CNT, NEO_PIN, NEO_GRBW + NEO_KHZ800);
AM2302::AM2302_Sensor dht{DHT_PIN};

//네오픽셀 관련 변수
byte r, g, b, w = 0;	// 네오픽셀 LED
byte bright = 240; 		// 240이 최대, 30이 최소, 8단계 제어
byte ledmode = 0;		// 13개의 색
long rSeed = 0;			// LED랜덤색상 설정 시드값

//버튼 관련 변수
int swpin[4] = {MOD_SW_PIN, LED_SW_PIN, TIME_SW_PIN, ALARM_SW_PIN};	//단순히 핀 매핑해 놓은 것 뿐
bool sw_org_stat[4] = {HIGH, HIGH, HIGH, HIGH};		// 체터링을 거른 버튼의 상태를 표현하는 변수
bool l_sw_stat[4]   = {HIGH, HIGH , HIGH, HIGH};	// 마지막 버튼 상태
int sw_prcs_val[4]  = {0, 0, 0, 0};					// 버튼을 프로그램에서 처리할 때 사용하는 값
unsigned long l_deb_tme[4] = {0, 0, 0, 0};			// 디바운스 시간 체킹
unsigned long sw_w[4] = {0, 0, 0, 0};				// 시간 버튼이 언제 눌렸는가

// RTC모듈 관련 변수
byte tMSB, tLSB;  // RTC온도관련
float temp3231;

//시간 관련 변수
extern volatile unsigned long timer0_millis;	// millis 오버플로우 대비 초기화
unsigned long time = 0;			// 아두이노 내부 밀리초 값
byte sec, lastSec, lastClockSec = 0;	// 매 초 실행되는 기능을 위해
byte hourPlus, minPlus = 0;		// 시간 변경 시 임시적으로 값을 저장
byte minRtc, hourRtc = 0; 		// RTC의 시간값
byte min, hour = 0;   			// 실제 시간값
bool isResetMillis = false;		// millis오버플로우 초기화 여부
bool isClockChange = false;			// 시간 수정여부

// 온습도 관련 변수
int8_t dhtStatus;
float celsius, humidity = 0;	//섭씨, 습도 
unsigned long dhtshowTime = 0;
bool isEnableShowDht = false;
unsigned long tempshow = 0;

// 알람 관련 변수
byte almHour, almMin, almSec = 0;	// 시 분 초
bool isonAlarm = false;				// 알람기능이 켜져 있는가
bool isAlarmChange = false;				// 알람 시간 수정여부
unsigned long l_showAstat_Time = 0;	//알람 상태를 보여주기 시작한 시간 기록 변수

//플리커 관련 변수
byte flick_bri = 0;	// 플리커 밝기 변수
//전체 모드 제어
byte clock_mode = 0;//0은 시계, 1은 온도, 2는 습도
bool isChangeMode = false;	//모드가 바뀔 때는 화면도 업데이트 되어야지

void setup() 
{
	#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
		clock_prescale_set(clock_div_1);
	#endif
	// I2C 통신시작
	Wire.begin();
	// 입출력설정
	pinMode(MOD_SW_PIN, INPUT_PULLUP);
	pinMode(LED_SW_PIN, INPUT_PULLUP);
	pinMode(TIME_SW_PIN, INPUT_PULLUP);
	pinMode(ALARM_SW_PIN, INPUT_PULLUP);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(FLICKER_PIN, OUTPUT);
	// 시리얼 모니터
	Serial.begin(9600);
	Serial.println("Hangeul Clock v2 is turned ON");
	Serial.println("Clock start");
	// 네오픽셀 초기설정
	strip.begin();
	strip.setBrightness(bright);
	strip.show();
	// 온습도 센서 초기설정
	if (dht.begin()) {
		Serial.println("DHTsensor is correct.");
		isEnableShowDht = true;
	} else {
		Serial.println("DHTsensor is not correct. Please check again");
		isEnableShowDht = false;
	}
	// 시계켜짐모션, DHT센서 특성상 3초가 필요해 해당 함수는 여기 들어간다.  
	startMotion();
	// 시계에 시간 출력
	time = millis();
	get3231Date();
	hour = (hourRtc + hourPlus) % 24;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
}

void loop() 
{
	// 사전설정
	time = millis();
	sec = int(time/1000)%60;
	if(sec == 60) sec = 0;
	//get3231Date();
	if (!sec && !minRtc && !hourRtc) {	//millis 초기화
		if(!timer0_millis) isResetMillis = true;
		if (isResetMillis == true) {
			for(int i = 0; i < 4; i++) {
				l_deb_tme[i] = 0;
				sw_w[i] = 0;
			}
			time = 0;
			timer0_millis = 0;
			isResetMillis = false;
		}
	}

	// 알람구동코드
	if(isonAlarm) alarmMotion();
	// 스위치 센싱
	for (int i = 0; i < 4; i++) { //4개의 스위치를 순차적으로 센싱함.
		sw_prcs_val[i] = sensingSW(i); 
		}
	
	//모드 변경
	if(!(isAlarmChange || isClockChange) && sw_prcs_val[MOD_SW]) {
		clock_mode = (clock_mode == 2) ? 0 : clock_mode+1;
		isChangeMode = true;
		Serial.print("Now mode is ");
		Serial.println(clock_mode);
	}
	//기능실행
	if(clock_mode == M_CLOCK) showClock();
	else showTnH(clock_mode ,celsius, humidity);
	// 1초마다 실행되는 주기적인 코드들.
	if (sec != lastSec) {
		blink();			// Flickering
		dhtStatus = dht.read();	//TnH sensing
		celsius = dht.get_Temperature();
		humidity = dht.get_Humidity();
		lastSec = sec;
	}
}
