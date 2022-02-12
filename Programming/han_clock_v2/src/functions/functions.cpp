#include "functions.h"

/* Function for Displaying Time */

// 시간을 출력하는 함수
void displayTime(int h, int m)
{
	ledclear();
	updateHour(h);
	updateMin(m);
	strip.show();
}
// 시간을 업데이트, 즉 시단위를 표시하는 함수이다
int updateHour(int h)
{
	printled(3);
	//오전 오후 출력
	if(h < 12)	{ printled(29); printled(30); }
	else		{ printled(29), printled(28); }
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
// 분을 업데이트, 즉 분단위를 표시하는 함수이다
int updateMin(int m)
{
	if(m) printled(2);
	int ten = m / 10;
	switch (ten) {
		case 1:	printled(23); printled(2); break;
		case 2:	printled(23); printled(32); break;
		case 3:	printled(23); printled(33); break;
		case 4:	printled(23); printled(25); break;
		case 5:	printled(23); printled(24); break;
	}

	m -= ten * 10;
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
// 실제로 시간표시 LED에 구동명령을 내리는 함수
void printled(int n)
{
	if (ledmode == 0) {
		rSeed++;
		randomSeed(rSeed);
		byte rand = random(0, COLOR_CNT);
		r = color[rand][0];
		g = color[rand][1];
		b = color[rand][2];
		w = color[rand][3];
	}
	strip.setPixelColor(n, r, g, b, w);
}
// 시간표시 LED를 전체 다 초기화시키는. 꺼 버리는 함수
void ledclear()
{
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0, 0, 0, 0);
	}
	strip.show();
}

/* Function for temperature and humidity */

/* Function for Sensing Switch */

//스위치 센싱 함수
int sensingSW(int index)
{
	bool reading = digitalRead(swpin[index]);
	Serial.print(reading);
	Serial.print("-");
	if (reading != l_sw_stat[index])
		l_deb_tme[index] = time;
	if ((time - l_deb_tme[index]) < DEB_DLY) {
    l_sw_stat[index] = reading;
		return NONE; 
	}
	if (reading == sw_org_stat[index]) {
    l_sw_stat[index] = reading;
		return NONE;
	}
	sw_org_stat[index] = reading;

	if (sw_org_stat[index] == LOW) {
		Serial.println("switch_pressed");
		sw_w[index] = time;
    l_sw_stat[index] = reading;
		return NONE;
	} else {
		Serial.println("switch_released");
    l_sw_stat[index] = reading;
		return sw_w[index] > time - SW_INTERVAL ?  SHORT : LONG;
	}
}

/* Function for controlling LED */

// LED밝기 제어하는 함수
void changeLEDbright()
{
	bright = bright > MAX_BRI ? INCREASE_BRI : +INCREASE_BRI;
	strip.setBrightness(bright);
	displayTime(hour, min);
	Serial.print("change brightness: ");
	Serial.println(bright);
}
// LED색깔 제어하는 함수
void changeLEDcolor()
{
	Serial.println("change color");
	ledmode++;
	if (ledmode > COLOR_CNT) ledmode = 0;
	if (ledmode > 0) {
		r = color[ledmode][0];
		g = color[ledmode][1];
		b = color[ledmode][2];
		w = color[ledmode][3];
	}
	displayTime(hour, min);
}

/* Function for alarm */

// 알람 시 값을 증가시키는 함수
void increasingAlmHour()
{
	Serial.println("Alarm hour plus");
	almHour = almHour > 23 ? 0 : +1;
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 알람 분 값을 증가시키는 함수
void increasingAlmMin()
{
	Serial.println("Alarm min plus");
	almMin = almMin > 59 ? 0 : +1;
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 알람 편집모드 시작시키는 함수
void startAchange()
{
	Serial.println("Start Alarm change");
	isAchange = true;
	displayTime(almHour, almMin);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 알람 편집모드 종료시키는 함수
void endAchange()
{
	Serial.println("End Alarm change");
	isAchange = false;
	displayTime(hour, min);
}
// 알람의 현재 상태를 바꾸고 상태를 보여주는 함수
void changeAlmStat()
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
// 알람 구동하는 코드
void alarmMotion()
{
	if(almHour == hour && almMin == min) {
		while(digitalRead(MOD_SW)) {
			for(int i = 0; i < strip.numPixels(); i++) {
				strip.setPixelColor(i, 255, 0, 0, 255);
			}
			strip.show();

			digitalWrite(7, HIGH);
			delay(180);
			digitalWrite(7, LOW);
			delay(20);
			digitalWrite(7, HIGH);
			delay(180);
			digitalWrite(7, LOW);
			delay(20);

			ledclear();
			delay(400);
		}
	}
}


/* Function for controlling time */

// 시 값을 증가시키는 함수
void increasingHour()
{
	Serial.println("hour plus");
	hourPlus = hourPlus > 23 ? 0 : +1;
	hour = (hourRtc + hourPlus) % 24;
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 분 값을 증가시키는 함수
void increasingMin()
{
	Serial.println("min plus");
	minPlus = minPlus > 59 ? 0 : +1;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 시간 편집모드 시작시키는 함수
void startTchange()
{
	Serial.println("start time change");
	isTchange = true;
	ledclear();
	printled(34);
	displayTime(hour, min);
	strip.setPixelColor(34, 0, 0, 0, 230);
	strip.show();
}
// 시간 편집모드 종료시키는 함수
void endTchange()
{
	Serial.println("end time change");
	isAchange = false;
	displayTime(hour, min);
}

/* Function for RTC */

// 십진수를 이진화된 십진수로 바꿔주는 함수
byte decToBcd(byte val)
{
	return ((val / 10 * 16) + (val % 10));
}
// RTC모듈의 시간값을 세팅하는 함수
void set3231Date()
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
// RTC모듈에서 시간값을 받는 함수
void get3231Date()
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
// RTC모듈의 온도를 받는 함수
// 다만, 프로그래머인 나의 입장에서는 사용할 이유가 없으나,하드웨어 설계 및 
// 유지보수시의 RTC의 온도를 체크해야 할 일이 있을 때는 요긴하게 씀.
float get3231Temp()
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

// 시간을 시리얼 창에 표시할 때 사용하는 함수
// 프로그래밍 시에 테스트 용으로만 사용하는 코드이다.
void showSerialTime()
{
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

void showClock()
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

float showTnH(float org, bool isWhat)
{
	if(tempshow + 2000 < time) {
		Serial.println("now showing temp and humi");
		int data = (int)org;
		int seatOne = data % 10;
		int seatTen = (data - seatOne) / 10;
		ledclear();
		showSEGnum(10, seatTen, isWhat);
		showSEGnum( 1, seatOne, isWhat);
		strip.show();
		tempshow = time;
	}
}

int showSEGnum(int a, int num, bool w) {
	int rrr, bbb;
	if(w) {
		rrr = 255;
		bbb = 0;
	} else {
		rrr = 0;
		bbb = 255;
	}
	if(a = 10) {
		for(int i = 0; i < 13; i++) {
			strip.setPixelColor(numberTEN_segment[num][i], rrr, 0, bbb, 0);
		}
	} else {
		for(int i = 0; i < 13; i++) {
			strip.setPixelColor(numberONE_segment[num][i], rrr, 0, bbb, 0);
		}
	}
	strip.setPixelColor(0, 0, 0, 0, 0);
}