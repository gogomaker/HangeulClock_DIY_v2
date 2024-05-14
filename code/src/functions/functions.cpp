#include "functions.h"

/* Function for Displaying Time */
void displayTime(int h, int m)	//시간 출력하는 함수
{
	ledclear();
	updateHour(h);
	updateMin(m);
	strip.show();
}
void updateHour(int h)			//시단위를 표시하는 함수
{
	printled(3);
	//오전 오후 출력
	if(h < 12)	{	printled(29);	printled(30);}
	else		{	printled(29);	printled(28);}
	//시간 출력
	switch(h) {
		case 0:		printled(31);	printled(26);	break;
		case 1:		printled(27);					break;
		case 2:		printled(26);					break;
		case 3:		printled(17);					break;
		case 4:		printled(19);					break;
		case 5:		printled(18);	printled(15);	break;
		case 6:		printled(16);	printled(15);	break;
		case 7:		printled(14);	printled(7);	break;
		case 8:		printled(16);	printled(5);	break;
		case 9:		printled(6);	printled(4);	break;
		case 10:	printled(31);					break;
		case 11:	printled(31);	printled(27);	break;
		case 12:	printled(31);	printled(26);	break;
		case 13:	printled(27);					break;
		case 14:	printled(26);					break;
		case 15:	printled(17);					break;
		case 16:	printled(19);					break;
		case 17:	printled(18);	printled(15);	break;
		case 18:	printled(16);	printled(15);	break;
		case 19:	printled(14);	printled(7);	break;
		case 20:	printled(16);	printled(5);	break;
		case 21:	printled(6);	printled(4);	break;
		case 22:	printled(31);					break;
		case 23:	printled(31);	printled(27);	break;
	}
}
void updateMin(int m)			//분단위를 표시하는 함수
{
	if(m) printled(2);
	int ten = m/10;
	switch (ten) {
		case 1:	printled(23); printled(2); break;
		case 2:	printled(23); printled(32); break;
		case 3:	printled(23); printled(33); break;
		case 4:	printled(23); printled(25); break;
		case 5:	printled(23); printled(24); break;
	}
	m-=ten*10;
	switch (m) {
		case 1:	printled(20);	break;
		case 2:	printled(21);	break;
		case 3:	printled(8);	break;
		case 4:	printled(11);	break;
		case 5: printled(22);	break;
		case 6:	printled(9);	break;
		case 7:	printled(12);	break;
		case 8:	printled(10);	break;
		case 9:	printled(13);	break;
	}
}

/* Function for Sensing Switch */
int sensingSW(int index)		//스위치 센싱 함수
{
	bool reading = digitalRead(swpin[index]);
	if (reading != l_sw_stat[index]) 
		l_deb_tme[index] = time;	// 스위치 값을 처음 읽고는, 바운싱인지 아닌지 확인하는 코드
	if ((time - l_deb_tme[index]) < DEB_DLY) {		// 스위치가 바운싱되었을때는 아무것도 안 함
    	l_sw_stat[index] = reading;
		return NONE; 
	}
	if (reading == sw_org_stat[index]) {			// 스위치를 읽은 값이 스위치 상태와 같다면
    	l_sw_stat[index] = reading;
		return NONE;
	}
	sw_org_stat[index] = reading;
	//아래 코드는 디바운싱된 스위치의 상태애서, 스위치의 상태가 변경되었을 때 작동하는 코드이다. 
	if (sw_org_stat[index] == LOW) {				// 스위치가 눌렸다면
		Serial.println("switch_pressed");
		sw_w[index] = time;
    	l_sw_stat[index] = reading;
		if(sw_w[index] < time-SW_INTERVAL) {
			Serial.println("Switch is released by system.");
	    	l_sw_stat[index] = reading;
			return LONG;
		} else { return NONE; }
	} else {										// 스위치가 때졌다면
		Serial.println("switch_released");
    	l_sw_stat[index] = reading;
		return (sw_w[index] < time-SW_INTERVAL) ?  LONG : SHORT;
	}
}

/* Function for controlling LED */
void changeLEDbright()			//LED밝기 제어하는 함수
{
	bright = (bright >= MAX_BRI) ? INCREASE_BRI : bright+INCREASE_BRI;
	strip.setBrightness(bright);
	Serial.print("Changed brightness: ");
	Serial.println(bright);
	if(isChangeMode) {
		showTnH(clock_mode, celsius, humidity);
		isChangeMode = false;
	}
	else
		displayTime(hour, min);
}
void changeLEDcolor()			//LED색깔 제어하는 함수
{
	Serial.println("change color");
	ledmode++;
	if (ledmode > COLOR_CNT) ledmode = 0;
	if(ledmode) {
		r = color[ledmode][0];
		g = color[ledmode][1];
		b = color[ledmode][2];
		w = color[ledmode][3];
	}
	displayTime(hour, min);
}
void printled(int n)			//LED에 색상 할당하는 함수
{
	if (ledmode == 0) {
		rSeed++;
		randomSeed(rSeed);
		byte rand = random(1, COLOR_CNT);
		r = color[rand][0];
		g = color[rand][1];
		b = color[rand][2];
		w = color[rand][3];
	}
	strip.setPixelColor(n, r, g, b, w);
}
void ledclear()					//LED색상 초기화하는 함수
{
	for (int i = 0; i < strip.numPixels(); i++)
		strip.setPixelColor(i, 0, 0, 0, 0);
	strip.show();
}
void startMotion() {			// 시계가 처음 시작될 때 대각선으로 불 켜지는 함수
	strip.setBrightness(255);
	int gb[12] = {0, -50, -100, -150, -200, -250, -300, -350, -400, -450, -500, 0};	//해당하는 그룹의 밝기
	int delta[12] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 2};
	ledclear();
	for(int i = 0; i < 240; i++) {
		for(int j = 0; j < 11; j++) {
			if(gb[j]>=0) {	//밝기 값이 유효할 때만
				//해당 그룹의 W.W.LED에 밝기값을 할당한다. 
				switch(j) {
				case 0:
					strip.setPixelColor(0, 0, 0, 0, gb[j]);
					break;
				case 1:
					strip.setPixelColor(1, 0, 0, 0, gb[j]);
					strip.setPixelColor(10, 0, 0, 0, gb[j]);
					break;
				case 2:
					strip.setPixelColor(2, 0, 0, 0, gb[j]);
					strip.setPixelColor(9, 0, 0, 0, gb[j]);
					strip.setPixelColor(11, 0, 0, 0, gb[j]);
					break;
				case 3:
					strip.setPixelColor(3, 0, 0, 0, gb[j]);
					strip.setPixelColor(8, 0, 0, 0, gb[j]);
					strip.setPixelColor(12, 0, 0, 0, gb[j]);
					strip.setPixelColor(22, 0, 0, 0, gb[j]);
					break;
				case 4:
					strip.setPixelColor(4, 0, 0, 0, gb[j]);
					strip.setPixelColor(7, 0, 0, 0, gb[j]);
					strip.setPixelColor(13, 0, 0, 0, gb[j]);
					strip.setPixelColor(21, 0, 0, 0, gb[j]);
					strip.setPixelColor(23, 0, 0, 0, gb[j]);
					break;
				case 5:
					strip.setPixelColor(6, 0, 0, 0, gb[j]);
					strip.setPixelColor(14, 0, 0, 0, gb[j]);
					strip.setPixelColor(20, 0, 0, 0, gb[j]);
					strip.setPixelColor(24, 0, 0, 0, gb[j]);
					strip.setPixelColor(34, 0, 0, 0, gb[j]);
					break;
				case 6:
					strip.setPixelColor(5, 0, 0, 0, gb[j]);
					strip.setPixelColor(15, 0, 0, 0, gb[j]);
					strip.setPixelColor(19, 0, 0, 0, gb[j]);
					strip.setPixelColor(25, 0, 0, 0, gb[j]);
					strip.setPixelColor(33, 0, 0, 0, gb[j]);
					break;
				case 7:
					strip.setPixelColor(16, 0, 0, 0, gb[j]);
					strip.setPixelColor(18, 0, 0, 0, gb[j]);
					strip.setPixelColor(26, 0, 0, 0, gb[j]);
					strip.setPixelColor(32, 0, 0, 0, gb[j]);
					break;
				case 8:
					strip.setPixelColor(17, 0, 0, 0, gb[j]);
					strip.setPixelColor(27, 0, 0, 0, gb[j]);
					strip.setPixelColor(31, 0, 0, 0, gb[j]);
					break;
				case 9:
					strip.setPixelColor(28, 0, 0, 0, gb[j]);
					strip.setPixelColor(30, 0, 0, 0, gb[j]);
					break;
				case 10:
					strip.setPixelColor(29, 0, 0, 0, gb[j]);
					break;
				}
			}
		}
		strip.show();
		for(int j = 0; j < 11; j++) {	//밝기값 재설정
			if(gb[j] == 250) {
				delta[j] = -5;
			}
			gb[j] += delta[j];
		}
		analogWrite(FLICKER_PIN, gb[11]);
		if(gb[11] == 250) delta[11] = -2;
		gb[11] += delta[11];
		delay(10);
	}
	strip.setBrightness(MAX_BRI);
}
void blink() {
	if(sec%2) {
		if(isClockChange || isAlarmChange) {
			strip.setPixelColor(2, 0, 0, 0, MAX_BRI);
			strip.setPixelColor(3, 0, 0, 0, MAX_BRI);
			strip.show();
		}
		digitalWrite(FLICKER_PIN, HIGH);
	} else {
		if(isClockChange || isAlarmChange) {
			strip.setPixelColor(2, 0, 0, 0, 0);
			strip.setPixelColor(3, 0, 0, 0, 0);
			strip.show();
		}
		digitalWrite(FLICKER_PIN, LOW);
	}
}
/* Function for alarm */
void increasingAlmHour()		//알람 '시' 값을 증가시키는 함수
{
	Serial.println("Alarm hour plus");
	almHour = (almHour>=23) ? 0 : almHour+1;
	displayTime(almHour, almMin);
	strip.setPixelColor(COMMA, 0, MAX_BRI, 0, 0);
	strip.show();
}
void increasingAlmMin()			//알람 분 값을 증가시키는 함수
{
	Serial.println("Alarm min plus");
	almMin = (almMin>=59) ? 0 : almMin+1;
	displayTime(almHour, almMin);
	strip.setPixelColor(COMMA, 0, MAX_BRI, 0, 0);
	strip.show();
}
void changeAlmStat()			//알람의 현재 상태를 바꾸고 상태를 보여주는 함수
{
	Serial.println("Changed alarm status");
	isonAlarm = !isonAlarm;
	showAlmStat(isonAlarm);
}
void showAlmStat(bool t) {
	ledclear();
	if(t) {
		for(int i = 0; i < 8; i++) 
			strip.setPixelColor(shapeO[i], 0, 160, 0, 0);		// 녹색
	}
	else {
		for(int i = 0; i < 8; i++) 
			strip.setPixelColor(shapeX[i], 250, 30, 19, 0);	// 주황
	}
	strip.show();
	// 여기에 유일하게 딜레이를 넣은 이유는 간단하다. 
	// 알람의 상태를 보여줄 때는 굳이 다른 동작들을 해야 할 이유가 없는 것.
	delay(1500);
	ledclear();
	displayTime(hour, min);
}
void alarmMotion()				//알람 구동하는 코드
{
	if((almHour == hour && almMin == min) && (!isAlarmChange || !isClockChange)) {
		while(digitalRead(MOD_SW_PIN)) {
			for(int i = 0; i < LED_CNT; i++) {
				strip.setPixelColor(i, MAX_BRI, 0, 0, MAX_BRI);
			}
			strip.show();
			digitalWrite(BUZZER_PIN, HIGH);
			delay(180);
			digitalWrite(BUZZER_PIN, LOW);
			delay(20);
			ledclear();
			delay(400);
		}
		isonAlarm = false;
		clock_mode = 0;
	}
}

/* Function for controlling time */
void increasingHour()			//시 값을 증가시키는 함수
{
	Serial.println("hour plus");
	//hourPlus = (hourPlus>23) ? 0 : hourPlus+1;//RTC기반 동작일 때 필요한 코드
	//hour = (hourRtc + hourPlus) % 60;			//RTC기반 동작일 때 필요한 코드
	hour = (hour>=23) ? 0 : hour+1;		// 아두이노 내부 millis 기반일 때 필요한 코드
	displayTime(hour, min);
	strip.setPixelColor(COMMA, 0, 0, MAX_BRI, 0);
	strip.show();
}
void increasingMin()			//분 값을 증가시키는 함수
{
	Serial.println("min plus");
	//minPlus = (minPlus>59) ? 0 : minPlus+1;	//RTC기반 동작일 때 필요한 코드
	//min = (minRtc + minPlus) % 60;			//RTC기반 동작일 때 필요한 코드
	min = (min>=59) ? 0 : min+1;	// 아두이노 내부 millis 기반일 때 필요한 코드
	displayTime(hour, min);
	strip.setPixelColor(COMMA, 0, 0, MAX_BRI, 0);
	strip.show();
}

/* Function for RTC */
byte decToBcd(byte val)			// 십진수를 이진화된 십진수로 바꿔주는 함수
{
	return ((val / 10 * 16) + (val % 10));
}
void set3231Date()				// RTC모듈의 시간을 설정하는 함수
{
	if (!min) hour++;
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x00);
	Wire.write(sec);
	Wire.write(decToBcd(min));
	Wire.write(decToBcd(hour));
	Wire.endTransmission();
	minPlus = hourPlus = 0;
}
void get3231Date()				// RTC모듈에서 시간값을 받는 함수
{
	// send request to receive data starting at register 0
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x00); // start at register 0
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes
	if (Wire.available()) {
		sec = Wire.read(); // get seconds
		minRtc = Wire.read(); // get minutes
		hourRtc = Wire.read();   // get hours
		sec = (((sec & B11110000) >> 4) * 10 + (sec & B00001111)); // convert BCD to decimal
		minRtc = (((minRtc & B11110000) >> 4) * 10 + (minRtc & B00001111)); // convert BCD to decimal
		hourRtc = (((hourRtc & B00110000) >> 4) * 10 + (hourRtc & B00001111)); // convert BCD to decimal (assume 24 hour mode)
	}
}
float get3231Temp()				// RTC모듈의 온도를 받는 함수
{
	//temp registers (11h-12h) get updated automatically every 64s
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x11);
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 2);
	if (Wire.available()) {
		tMSB = Wire.read(); //2's complement int portion
		tLSB = Wire.read(); //fraction portion
		temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
		temp3231 += ((tLSB >> 6) * 0.25); //only care about bits 7 & 8
	}
	return temp3231;
}

/* ETC code */
void showSerialTime()			// 시간을 시리얼 창에 표시할 때 사용하는 함수
{
	Serial.print(hour);
	Serial.print(":");
	Serial.print(min);
	Serial.print(":");
	Serial.println(sec);
}
void showSEGnum(int digit, int num, int isON) {	//세그먼트 형태로 숫자를 출력하는 함수
	int red, blue;
	if(isON){red = 0;	blue = 255;	} 
	else	{red = 255;	blue = 0;	}
	if(digit == 10) {
		for(int i = 0; i < 13; i++) {
			strip.setPixelColor(numberTEN_segment[num][i], red, 0, blue, 0);
		}
	} else {
		for(int i = 0; i < 13; i++) {
			strip.setPixelColor(numberONE_segment[num][i], red, 0, blue, 0);
		}
	}
	strip.setPixelColor(0, 0, 0, 0, 0);
}

/* Core Function */
void showClock()					// 시계모드 함수
{
	// LED 버튼 처리
	if 		(sw_prcs_val[LED_SW] == SHORT) 	changeLEDbright();
	else if (sw_prcs_val[LED_SW] == LONG)	changeLEDcolor();

	// 시간 버튼 처리
	if(sw_prcs_val[TIME_SW]==SHORT && sw_prcs_val[ALARM_SW]==SHORT) {
		changeAlmStat();
	}
	if(sw_prcs_val[TIME_SW] == LONG) { 
		if(isClockChange) {					//시간 수정모드 해제
			Serial.println("end time change");
			strip.setPixelColor(COMMA, 0, 0, 0, 0);
			strip.show();
			//hour = (hourRtc + hourPlus) % 24;		//RTC기반 동작일 때 필요한 코드
			//min = (minRtc + minPlus) % 60;		//RTC기반 동작일 때 필요한 코드
			displayTime(hour, min);
			isClockChange = false;
		} else {
			if(!isAlarmChange) {
				Serial.println("start time change");
				strip.setPixelColor(COMMA, 0, 0, MAX_BRI, 0);
				strip.show();
				isClockChange = true;
			}
		}
	} else if (sw_prcs_val[TIME_SW]==SHORT && !(sw_prcs_val[ALARM_SW]==SHORT)) {
		if(isClockChange) increasingHour();
		else if(isAlarmChange) increasingAlmHour();
	}
	if(sw_prcs_val[ALARM_SW] == LONG) {
		if(isAlarmChange) {
			Serial.println("End alarm change");
			isAlarmChange = false;
			isonAlarm = true;
			showAlmStat(isonAlarm);
			strip.setPixelColor(COMMA, 0, 0, 0, 0);
			strip.show();
		} else {
			if(!isClockChange) {
				Serial.println("start alarm change");
				displayTime(almHour, almMin);
				strip.setPixelColor(COMMA, 0, MAX_BRI, 0, 0);
				strip.show();
				isAlarmChange = true;
			}
		}
	} else if (sw_prcs_val[ALARM_SW]==SHORT && !(sw_prcs_val[TIME_SW]==SHORT)) {
		if(isClockChange) increasingMin();
		else if(isAlarmChange) increasingAlmMin();
		else if(!isAlarmChange && !isClockChange) changeAlmStat();
	}

	if(isChangeMode) {
		displayTime(hour, min);
		isChangeMode = false;
	}
	if (sec != lastClockSec) {		//매 초마다 시계 기능 작동
		//hour = (hourRtc + hourPlus) % 24;	//RTC기반 동작일 때 필요한 코드
		//min = (minRtc + minPlus) % 60;	//RTC기반 동작일 때 필요한 코드
		if (!sec) {	//매 0초마다(1분 간격으로)
			if (hourPlus || minPlus) {
				Serial.println("RTC set");
				set3231Date();
			}
			if(isAlarmChange)	displayTime(almHour, almMin);
			else				displayTime(hour, min);
			if(isClockChange) {
				strip.setPixelColor(COMMA, 0, 0, MAX_BRI, 0);
				strip.show();
			}
			else if(isAlarmChange) {
				strip.setPixelColor(COMMA, 0, MAX_BRI, 0, 0);
				strip.show();
			}
			Serial.println("Updated freauently");
		}
		lastClockSec = sec;
		showSerialTime();
	}
}
void showTnH(int mode, float c, float h)	//온습도 모드
{
	if(sw_prcs_val[LED_SW] == SHORT) {
		isChangeMode = true;
		changeLEDbright();
	}
	if(tempshow+2000<time || isChangeMode) {	//2초마다 실행되도록
		if(isChangeMode) isChangeMode = false;
		Serial.print("now showing temp and humi: ");
		Serial.print(c);
		Serial.print(" / ");
		Serial.println(h);
		int data = (mode == M_TEMP)? c : h;
		int seatOne = data % 10;
		int seatTen = (data - seatOne) / 10;
		ledclear();
		showSEGnum(10, seatTen, mode-1);
		showSEGnum( 1, seatOne, mode-1);
		if(mode == M_TEMP)	strip.setPixelColor(1, 255, 0, 0, 0);
		else 				strip.setPixelColor(0, 0, 0, 255, 0);
		strip.show();
		tempshow = time;
	}
}
