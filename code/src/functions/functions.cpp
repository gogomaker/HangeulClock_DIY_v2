#include "functions.h"

/* Function for Displaying Time */
void displayTime(int h, int m)	//시간 출력하는 함수
{
	ledclear();
	updateHour(h);
	updateMin(m);
	strip.show();
}
int updateHour(int h)			//시단위를 표시하는 함수
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
int updateMin(int m)			//분단위를 표시하는 함수
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

/* Function for temperature and humidity */
// 코드개발하세요:

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
		return NONE;
	} else {										// 스위치가 때졌다면
		Serial.println("switch_released");
    	l_sw_stat[index] = reading;
		return (sw_w[index] > time-SW_INTERVAL) ?  LONG : SHORT;
	}
}

/* Function for controlling LED */
void changeLEDbright()			//LED밝기 제어하는 함수
{
	bright = (bright >= MAX_BRI) ? INCREASE_BRI : +INCREASE_BRI;
	strip.setBrightness(bright);
	displayTime(hour, min);
	Serial.print("change brightness: ");
	Serial.println(bright);
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

/* Function for alarm */
void increasingAlmHour()		//알람 '시' 값을 증가시키는 함수
{
	Serial.println("Alarm hour plus");
	almHour = almHour > 23 ? 0 : +1;
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, MAX_BRI);
	strip.show();
}
void increasingAlmMin()			//알람 분 값을 증가시키는 함수
{
	Serial.println("Alarm min plus");
	almMin = almMin > 59 ? 0 : +1;
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, MAX_BRI);
	strip.show();
}
void startAchange()				//알람 편집모드 시작시키는 함수
{
	Serial.println("Start Alarm change");
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, MAX_BRI);
	strip.show();
}
void endAchange()				//알람 편집모드 종료시키는 함수
{
	Serial.println("End Alarm change");
	displayTime(hour, min);
}
void changeAlmStat()			//알람의 현재 상태를 바꾸고 상태를 보여주는 함수
{
	Serial.println("Changed alarm status");
	isonAlarm = !isonAlarm;
	ledclear();
	if(isonAlarm) {
		for(int i = 0; i < 8; i++) {
			strip.setPixelColor(shapeO[i], 0, 160, 0, 0);			//녹색
		}
	}
	else {
		for(int i = 0; i < 8; i++) {
			strip.setPixelColor(shapeX[i], 230, 127, 19, 0);	//주황
		}
	}
	strip.show();
	// 여기에 유일하게 딜레이를 넣은 이유는 간단하다. 
	// 알람의 상태를 보여줄 때는 굳이 다른 동작들을 해야 할 이유가 없는 것.
	delay(1500);	//시계 구동에 문제있을 시 1000의 배수로 전환(ex.1000ms or 2000ms)
	ledclear();
	displayTime(hour, min);
}
void alarmMotion()				//알람 구동하는 코드
{
	if(almHour == hour && almMin == min) {
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
		clock_mode = 0;
	}
}


/* Function for controlling time */
void increasingHour()			//시 값을 증가시키는 함수
{
	Serial.println("hour plus");
	hourPlus = hourPlus > 23 ? 0 : +1;
	hour = (hourRtc + hourPlus) % 24;
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
void increasingMin()			//분 값을 증가시키는 함수
{
	Serial.println("min plus");
	minPlus = minPlus > 59 ? 0 : +1;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
void startTchange()				// 시간 편집모드 시작시키는 함수
{
	Serial.println("start time change");
	ledclear();
	printled(34);
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, MAX_BRI);
	strip.show();
}
void endTchange()				// 시간 편집모드 종료시키는 함수
{
	Serial.println("end time change");
	displayTime(hour, min);
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
	else {
		//oh noes, no data!
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
	else {
		//error! no data!
	}
	return temp3231;
}

/* ETC code */
void showSerialTime()			// 시간을 시리얼 창에 표시할 때 사용하는 함수
{
	// 프로그래밍 시에 테스트 용으로만 사용하는 코드이다.
	Serial.print(hour, DEC);
	Serial.print(":");
	Serial.print(min, DEC);
	Serial.print(":");
	Serial.print(sec, DEC);
	Serial.print(" - Temp: ");
	Serial.println(get3231Temp());

	Serial.print(hourRtc);
	Serial.print(", ");
	Serial.print(hourPlus);
	Serial.print(" ; ");
	Serial.print(minRtc);
	Serial.print(",");
	Serial.println(minPlus);
	Serial.println();
}
int showSEGnum(int digit, int num, bool isON) {	//세그먼트 형태로 숫자를 출력하는 함수
	int red, blue;
	if(isON){red = 0;	blue = 255;	} 
	else	{red = 255;	blue = 0;	}
	if(digit = 10) {
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

/*메인 펑션*/
void showClock()					// 시계모드 함수
{
	//매 초마다 시계 기능 작동
	if (sec != lastSec) {
		hour = (hourRtc + hourPlus) % 24;
		min = (minRtc + minPlus) % 60;
		if (!sec) {	//매 0초마다(1분 간격으로)
			if (hourPlus || minPlus) {
				Serial.println("RTC set");
				set3231Date();
			}
			displayTime(hour, min);
			Serial.println("updated");
		}
		lastSec = sec;
		showSerialTime();
	}
}
void showTnH(int mode, float c, float h)	//온습도 모드
{
	if(tempshow + 2000 < time) {
		Serial.println("now showing temp and humi");
		int data;
		if(mode == 1)	data = int(c);	//온도모드라면
		else 			data = int(h);	//습도모드라면
		int seatOne = data % 10;
		int seatTen = (data - seatOne) / 10;
		ledclear();
		showSEGnum(10, seatTen, mode-1);
		showSEGnum( 1, seatOne, mode-1);
		strip.show();
		tempshow = time;
	}
}

void startMotion() {			// 시계가 처음 시작될 때 대각선으로 불 켜지는 함수
	
}

void blink() {
	if(sec%2) {
		if(isHourChange || isAlarmHourChange) {
			//시LED를 켬
		} else if (isMinChange || isAlarmMinChange) {
			//분LED를 켬
		}
		digitalWrite(FLICKER_PIN, HIGH);
	} else {
		if(isHourChange || isAlarmHourChange) {
			//시LED를 끔
		} else if (isMinChange || isAlarmMinChange) {
			//분LED를 끔
		}
		digitalWrite(FLICKER_PIN, LOW);
	}
}