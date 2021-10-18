/*
 Name:		han_clock.ino
 Created:	2021-06-23 오후 3:341:22
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
byte color[COLOR_CNT][4]
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

// 객체 선언
Adafruit_NeoPixel strip(LED_CNT, NEOPIN, NEO_GRBW + NEO_KHZ800);
DHT dht(DHTPIN, DHTTYPE);


//네오픽셀 관련 변수
byte r, g, b, w = 255;	// 네오픽셀 LED
byte bright = 180; 		// 255가 최대, 0이 최소, 7단계 제어
byte ledmode = 0;		// 12개의 색 - 다채로운 30개 목표

//버튼 관련 변수
bool sw_reading[4] = { HIGH, HIGH, HIGH, HIGH};	// 버튼의 실제 상태를 표현하는 변수
bool sw_stat[4] = { HIGH, HIGH, HIGH, HIGH};	// 체터링을 거른 버튼의 상태를 표현하는 변수
bool l_sw_stat[4] = { HIGH, HIGH , HIGH, HIGH};	// 마지막 버튼 상태
bool sw_check[4] = {false, false, false, false};// 버튼의 짧고 긺을 측정할 때 사용하는 변수
unsigned long l_deb_tme[4] = { 0, 0, 0, 0};		// 디바운스 시간 체킹
unsigned long sw_w[4] = {0, 0, 0, 0};			// 시간 버튼이 언제 눌렸는가
unsigned long wait_t = 0;						// LED깜박일 때 사용

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
bool isblinkLED = false;		// LED가 깜박여야 하는가 - 시간수정모드 진입시
bool isTchange = false;			// 시간수정모드 여부

// 온습도 관련 변수
float temp, humi, f;	//온도, 습도, 화씨

// 알람 관련 변수     시 분 초
byte alarm_time[3] = {0, 0, 0};	// 시 분 초
bool isonAlarm = false;			// 알람기능이 켜져 있는가
bool isAchange = false;			// 알람수정모드 여부

// 아날로그 밝기 제어 관련 변수
int ext_bri = map(analogRead(EXT_BRIGHT), 0, 1023, 255, 0);	// 외부 밝기값

//플리커 관련 변수
byte flick_bri = 0;		// 플리커 밝기 변수

//전체 모드 제어
byte clock_mode = 0;	//0은 시계, 1은 온도, 2는 습도

void setup() {
	#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
		clock_prescale_set(clock_div_1);
	#endif
	// I2C 통신시작
	Wire.begin();

	// 시리얼 모니터
	Serial.begin(115200);
	Serial.println("Hangeul Clock v2 has turn ON");
	Serial.println("Clock start");
	
	// 네오픽셀 초기설정
	strip.begin();
	strip.setBrightness(bright);
	strip.show();
	
	// 온습도 센서 초기설정
	dht.begin();
	
	// 버튼입력설정
	pinMode(MODE_SW, INPUT_PULLUP);
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

void loop() {
	time = millis();
	get3231Date();
	if (!sec && !minRtc && !hourRtc) {	//millis 초기화
		if(!timer0_millis) isResetMillis = true;
		if (isResetMillis == true) {
			//Serial.println("Timer reset");
			l_deb_tme[ALM_INDEX] = 0;
			l_deb_tme[LED_INDEX] = 0;
			l_deb_tme[TME_INDEX] = 0;
			l_deb_tme[MOD_INDEX] = 0;
			sw_w[0] = 0;
			sw_w[1] = 0;
			sw_w[2] = 0;
			sw_w[3] = 0;
			wait_t = 0;
			time = 0;
			timer0_millis = 0;
			isResetMillis = false;
		}
	}
	
	if(clock_mode == 0) { // 시간 표시 모드
		//매 초마다 시계 기능 작동
		if (sec != lastSec) {
			hour = (hourRtc + hourPlus) % 24;
			min = (minRtc + minPlus) % 60;
			if (!sec) {	//매 0초마다(1분 간격으로)
				if (hourPlus || minPlus) {
					//Serial.println("RTC set");
					set3231Date();
				}
				displayTime(hour, min);
				//Serial.println("updated");
			}
			lastSec = sec;
		}
		
		/* 모든 스위치는 시계 모드에서만 작동 */
		//버튼 눌림 감지 함수
		changeModeSW();
		changeLedSW();
		changeTimeSW();
		changeAlarmSW();
		//버튼 길게 눌림 감지 함수
		longTimeSW();
		longLedSW();
		//시간 변경 시 LED깜박임
		blinkAllLED();
	}
	else if(clock_mode == 1) { // 온도 표시 모드

	}
	else { // 습도 표시 모드

	}
}
