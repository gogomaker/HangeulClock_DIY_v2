/*
 Name:		han_clock.ino
 Created:	2021-06-23 오후 3:34:22
 Author:	gogomaker
*/

/*
 이 프로그램은 한글시계 v2를 돌리는 프로그램이다.
 정말이지 v1과 프로그램은 비슷할 예정이지만
 온습도 측정 및 표시가 난관이 아닐까 싶다.
 항상 파이팅!
*/

/* 헤더파일 선언 */
#include "src\functions\functions.h"

/* 실제 변수 및 객체 선언 */

// 색상표 선언 - 수정해야 함(30가지 색)
const byte color[COLOR_CNT][4]
{//R  , G  , B  , W  
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
  {255, 0  , 100, 0  }		// 분홍과 빨강 사이 어딘가
};

// 형상에 대한 배열 선언
const byte shapeO[8] = {7, 8, 12, 21, 25, 26, 18, 15};
const byte shapeX[8] = {27, 19, 13, 9, 24, 20, 14, 6};
const byte numberTEN_segment[10][13]
{
	{31, 30, 29, 28, 17, 16,  5,  6,  7, 14, 19, 26,  0},	// 0
	{30, 27, 18, 15,  6,  0,  0,  0,  0,  0,  0,  0,  0},	// 1
	{29, 30, 31, 26, 19, 18, 17, 16,  5,  6,  7,  0,  0},	// 2
	{29, 30, 31, 26, 19, 14,  7,  6,  5,  0,  0,  0,  0},	// 3
	{29, 28, 17, 18, 19, 31, 26, 14,  7,  0,  0,  0,  0},	// 4
	{31, 30, 29, 28, 17, 18, 19, 14,  7,  6,  5,  0,  0},	// 5
	{31, 30, 29, 28, 17, 16,  5,  6,  7, 14, 19, 18,  0}, // 6
	{17, 28, 29, 30, 31, 26, 19, 14,  7,  0,  0,  0,  0},	// 7
	{26, 31, 30, 29, 28, 17, 18, 19, 14,  7,  6,  5, 16},	// 8
	{18, 17, 28, 29, 30, 31, 26, 19, 14,  7,  6,  5,  0}	// 9
};
const byte numberONE_segment[10][13]
{
	{32, 33, 34, 23, 22, 11, 10,  9,  8, 13, 20, 25,  0},	// 0
	{33, 24, 21, 12,  9,  0,  0,  0,  0,  0,  0,  0,  0},	// 1
	{32, 33, 34, 23, 22, 21, 20, 13,  8,  9, 10,  0,  0},	// 2
	{32, 33, 34, 23, 22, 21, 20, 11, 10,  9,  8,  0,  0},	// 3
	{32, 25, 20, 21, 34, 23, 22, 11, 10,  0,  0,  0,  0},	// 4
	{34, 33, 32, 25, 20, 21, 22, 11, 10,  9,  8,  0,  0},	// 5
	{34, 33, 32, 25, 20, 13,  8,  9, 10, 11, 22, 21,  0},	// 6
	{20, 25, 32, 33, 34, 23, 22, 11, 10,  0,  0,  0,  0},	// 7
	{23, 34, 33, 32, 25, 20, 21, 22, 11, 10,  9,  8, 13},	// 8
	{21, 20, 25, 32, 33, 34, 23, 22, 11, 10,  9,  8,  0}	// 9
};

// 객체 선언
Adafruit_NeoPixel strip(LED_CNT, NEOPIN, NEO_GRBW + NEO_KHZ800);

//네오픽셀 관련 변수
byte r, g, b, w = 255;	// 네오픽셀 LED
byte bright = 180; 		// 255가 최대, 0이 최소, 7단계 제어
byte ledmode = 0;		// 13개의 색 - 다채로운 30개 목표

//버튼 관련 변수
int swpin[4] = {MOD_SW, LED_SW, TIME_SW, ALARM_SW};	//단순히 핀 매핑해 놓은 것 뿐
bool sw_org_stat[4] = {HIGH, HIGH, HIGH, HIGH};  // 체터링을 거른 버튼의 상태를 표현하는 변수
bool l_sw_stat[4]   = {HIGH, HIGH , HIGH, HIGH}; // 마지막 버튼 상태
int sw_prcs_val[4]  = {0, 0, 0, 0};				// 버튼을 프로그램에서 처리할 때 사용하는 값
unsigned long l_deb_tme[4] = {0, 0, 0, 0};		// 디바운스 시간 체킹
unsigned long sw_w[4] = {0, 0, 0, 0};			// 시간 버튼이 언제 눌렸는가

// RTC모듈 관련 변수
byte tMSB, tLSB;  // RTC온도관련
float temp3231;

//시간 관련 변수
extern volatile unsigned long timer0_millis;	// millis 오버플로우 대비 초기화
unsigned long time = 0;			// 아두이노 내부 밀리초 값
byte sec, lastSec = 0;			// 매 초 실행되는 기능을 위해
byte hourPlus, minPlus = 0;		// 시간 변경 시 임시적으로 값을 저장
byte minRtc, hourRtc = 0; 		// RTC의 시간값
byte min, hour = 0;   			// 실제 시간값
bool isResetMillis = false;		// millis오버플로우 초기화 여부
long rSeed = 0;					// LED랜덤색상 설정 시드값
bool isTchange = false;			// 시간수정모드 여부

// 온습도 관련 변수
float temp, humi, f;	//온도, 습도, 화씨
unsigned long dhtshowTime = 0;
bool isEnableShowDht = false;
unsigned long l_tempshow = 0;
unsigned long tempshow = 0;


// 알람 관련 변수     시 분 초
byte almHour, almMin, almSec = 0;	// 시 분 초
bool isonAlarm = false;			// 알람기능이 켜져 있는가
bool isAchange = false;			// 알람수정모드 여부
unsigned long l_showAstat_Time = 0;		//알람 상태를 보여주기 시작한 시간 기록 변수


// 아날로그 밝기 제어 관련 변수
int ext_bri = map(analogRead(EXT_BRIGHT), 0, 1023, 255, 0);	// 외부 밝기값

//플리커 관련 변수
byte flick_bri = 0;		// 플리커 밝기 변수

//전체 모드 제어
byte clock_mode = 0;	//0은 시계, 1은 온도, 2는 습도

void setup() 
{
	#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
		clock_prescale_set(clock_div_1);
	#endif
	// I2C 통신시작
	Wire.begin();

	// 시리얼 모니터
	Serial.begin(9600);
	Serial.println("Hangeul Clock v2 has turn ON");
	Serial.println("Clock start");
	
	// 네오픽셀 초기설정
	strip.begin();
	strip.setBrightness(bright);
	strip.show();
	
	// 온습도 센서 초기설정
	//dht.begin();
	
	// 버튼입력설정
	pinMode(MOD_SW, INPUT_PULLUP);
	pinMode(LED_SW, INPUT_PULLUP);
	pinMode(TIME_SW, INPUT_PULLUP);
	pinMode(ALARM_SW, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);
	pinMode(FLICKER, OUTPUT);
	
	// 시계에 시간 출력
	time = millis();
	get3231Date();
	hour = (hourRtc + hourPlus) % 24;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
}

void loop() 
{
	/* 사전설정 */ 
	time = millis();
	get3231Date();
	if(dhtshowTime < time) {
		//float humi = dht.readHumidity();
		//float temp = dht.readTemperature();
		if (isnan(temp) || isnan(humi)) {
			// 값 읽기 실패시 시리얼 모니터 출력
			Serial.println("Failed to read from DHT");
		}
		dhtshowTime += 2000;
	}
	if (!sec && !minRtc && !hourRtc) {	//millis 초기화
		if(!timer0_millis) isResetMillis = true;
		if (isResetMillis == true) {
			//Serial.println("Timer reset");
			for(int i = 0; i < 4; i++) {
				l_deb_tme[i] = 0;
				sw_w[i] = 0;
			}
			sw_w[3] = 0;
			time = 0;
			timer0_millis = 0;
			isResetMillis = false;
		}
	}
	/* 시계구동코드 */
	// 스위치 기반 작동 코드(LED밝기 및 색상, 시간 알람 편집 등)
	for (int i = 0; i < 4; i++) { 
		sw_prcs_val[i] = sensingSW(i); 
		Serial.print(sw_prcs_val[i]);
		Serial.print(" / ");
		}
		Serial.println();
	if (sw_prcs_val[LED_SW] == SHORT) { changeLEDbright(); }
	if (clock_mode == M_CLOCK) {
		if(sw_prcs_val[LED_SW] == LONG) { changeLEDcolor(); }
		if(isTchange) {
			if(sw_prcs_val[TME_SW] == SHORT) { increasingHour(); }	//시단위 추가
			if(sw_prcs_val[ALM_SW] == SHORT) { increasingMin();  }	//분단위 추가
			if(sw_prcs_val[TME_SW] == LONG) { endTchange(); }	//시간편집모드 종료
		}
		else if(isAchange) {
			if(sw_prcs_val[TME_SW] == SHORT) { increasingAlmHour(); }	//시단위 추가
			if(sw_prcs_val[ALM_SW] == SHORT) { increasingAlmMin();  }	//분단위 추가
			if(sw_prcs_val[TME_SW] == LONG) { endAchange(); }	//알람편집모드 종료
		}
		else {
			if(sw_prcs_val[ALM_SW] == SHORT) { changeAlmStat(); }	//알람 활성화 및 비활성화
			if(sw_prcs_val[TME_SW] == LONG) { startTchange(); }	//시간편집모드 시작
			if(sw_prcs_val[TME_SW] == LONG) { startAchange(); }	//알람편집모드 시작
		}
	}
	// 알람구동코드
	if(isonAlarm) {	//알람 활성화라면
		alarmMotion();
	}
	// 플리커 구동코드
	digitalWrite(FLICKER, sec%2);	// 원리를 보자면 짝수에는 false, 꺼지고 홀수에는 true, 켜진다.

	// 모드기반 기능구동코드(시간표시, 온도표시, 습도표시)를 처리할 수 있게 하는 코드
	if(!isAchange) {
		if (sw_prcs_val[MOD_SW]) { clock_mode = clock_mode > 1 ? 0 : + 1; }
		switch (clock_mode)
		{
		case 0:	// 시계모드
			showClock();
			break;
		
		case 1:	// 온도모드
			showTnH(temp, 0);
			break;
		
		case 2:	// 습도모드
			showTnH(humi, 1);
			break;
		
		default:
			break;
		}
	}
	// 알람설정기능은 시간표시기능에서만 작동, 알람설정기능이 활성화라면 모드버튼 비활성화 됨.
	// 또한, 시간수정모드가 활성화 된 상태에서 모드전환이 이루어진다면 값은 자동저장 됨.
	// 그리고 시간수정모드는 비활성화 됨.
}
