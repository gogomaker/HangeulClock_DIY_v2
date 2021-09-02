/*
 Name:		han_clock.ino
 Created:	2021-06-23 오후 3:341:22
 Author:	gogomaker
*/

/*
 이 프로그램은 한글시계 v2를 돌리는 프로그램이다.
 정말이지 v1과 프로그램은 비슷할 예정이지만
 온습도 측정 및 표시가 난관이 아닐 까 싶다.
 항상 파이팅!
*/


/* 헤더파일 선언 */
#include "src\functions\functions.h"

/* 실제 변수 및 객체 선언 */

// 색상표 선언
byte color[12][3]
{
  {255, 0, 0},		// 붉은색
  {255, 94, 0},		// 선홍색
  {255, 187, 0},    // 주황색
  {255, 228, 0},	// 노란색
  {171, 242, 0},    // 연두색
  {29, 219, 22},    // 초록색
  {0, 84, 255},     // 하늘색
  {1, 0, 255},		// 파란색
  {95, 0, 255},     // 보라색
  {255, 0, 221},    // 분홍색
  {255, 0, 100},     // 분홍과 빨강 사이 어딘가
  {255, 255, 255}	// 하얀색
};

// 네오픽셀 객체 선언
Adafruit_NeoPixel strip(LED_COUNT, NEOPIN, NEO_GRBW + NEO_KHZ800);

//네오픽셀 관련 변수
byte r = 255; //네오픽셀 Red
byte g = 255; //네오픽셀 Green
byte b = 255; //네오픽셀 Blue
byte bright = 180;  //255가 최대, 0이 최소, 7단계 제어
byte ledmode = 0; //12개의 색

//버튼 관련 변수
unsigned long bu_led_w = 0;
bool bu_reading[3] = { HIGH, HIGH, HIGH};  //버튼의 실제 상태를 표현하는 변수
bool bu_state[3] = { HIGH, HIGH, HIGH};  //체터링을 거른 버튼의 상태를 표현하는 변수
bool last_bu_state[3] = { HIGH, HIGH , HIGH}; //마지막 버튼 상태
unsigned long LastDebounceTime[3] = { 0, 0, 0};
unsigned long bu_t_w, last_bu_t_w = 0;  //시간 버튼이 언제 눌렸는가
unsigned long wait_t, wait_m = 0; //LED깜박일 때 사용
bool timeCheck = false;
bool ledCheck = false;

// RTC모듈 관련 변수
byte tMSB, tLSB;  // RTC온도관련
float temp3231;

//시간 관련 변수
extern volatile unsigned long timer0_millis; // millis 오버플로우 대비 초기화
unsigned long time = 0;
byte sec, lastSec = 0;
byte hourPlus, minPlus = 0; // 버튼이 눌릴 때 이곳에 값이 임시저장 됨
byte minRtc, hourRtc = 0; 	// RTC의 시간값이 저장됨
byte min, hour = 0;   		// 최종적으로 출력되는 시간값이 저장됨
bool isResetMillis = false;
long randomBase = 0;
bool isblinkH, isblinkM = false;
byte tchange = 0;

// 온습도 관련 변수

// 알람 관련 변수

// 아날로그 밝기 제어 관련 변수

//플리커 관련 변수

void setup() {
	#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
		clock_prescale_set(clock_div_1);
	#endif
	// I2C 통신시작
	Wire.begin();

	// 시리얼 모니터
	Serial.begin(115200);
	Serial.println("Hangeul Clock has turn ON");
	Serial.println("Clock start");
	
	// 네오픽셀 초기설정
	strip.begin();
	strip.setBrightness(bright);
	strip.show();
	
	// 버튼입력설정
	pinMode(MODE_SW, INPUT_PULLUP);
	pinMode(LED_SW, INPUT_PULLUP);
	pinMode(TIME_SW, INPUT_PULLUP);
	pinMode(ALARM_SW, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);
	
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
			bu_led_w = 0;
			LastDebounceTime[0] = 0;
			LastDebounceTime[1] = 0;
			bu_t_w = 0;
			last_bu_t_w = 0;
			wait_t = 0;
			wait_m = 0;
			time = 0;
			timer0_millis = 0;
			isResetMillis = false;
		}
	}
	//매 초마다
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

	//버튼 눌림 감지 함수
	changeTimeButton();
	changeLedButton();
	//버튼 길게 눌림 감지 함수
	longTimeButton();
	longLedButton();
	//시간 변경 시 LED깜박임
	blinkHM();
}
