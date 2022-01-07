#include "functions.h"

/* Function for Displaying Time */

// 시간을 출력하는 함수
void displayTime(int h, int m) {
	ledclear();
	updateHour(h);
	updateMin(m);
	strip.show();
}

// 시간을 업데이트, 즉 시단위를 표시하는 함수이다
int updateHour(int h) {
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
int updateMin(int m) {
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
		case 3:	printled(22);	break;
		case 4:	printled(11);	break;
		case 5: printled(22);	break;
		case 6:	printled(9);	break;
		case 7:	printled(12);	break;
		case 8:	printled(10);	break;
		case 9:	printled(13);	break;
	}
}

// 실제로 시간표시 LED에 구동명령을 내리는 함수
void printled(int n) {
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
void ledclear() {
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0, 0, 0, 0);
	}
	strip.show();
}

// 모든 시간표시 LED를 깜박이게 하는 함수, 다만 사용여부는 모르겠다(지울예정)
void blinkAllLED() {
	if (isblinkLED == true) {
		if (time >= wait_t + BLINK_INTERVAL) {
			//Serial.println("blink!");
			displayTime(hour, min);
			isblinkLED = false;
		}
	}
}

/* Function for Sensing Switch */

//모드스위치 센싱 함수, 다른 함수 안 사용함
int sensingSW(int index) {
	bool reading = digitalRead(swpin[index]);
	if (reading != l_sw_stat[index])
		l_deb_tme[index] = time;
	if ((time - l_deb_tme[index]) <= DEB_DLY) { return NONE; }
	if (reading == sw_org_stat[index]) { return NONE; }
	sw_org_stat[index] = reading;

	if (sw_org_stat[index] == LOW) {
		//Serial.println("LED_pressed");
		sw_w[index] = time;
	} else {
		return sw_w[index] > time - SW_INTERVAL ?  LONG : SHORT;
	}
}

/* Function for temperature and humidity */


/* Function for alarm */


/* Function for RTC */

// 십진수를 이진화된 십진수로 바꿔주는 함수
byte decToBcd(byte val) {
	return ((val / 10 * 16) + (val % 10));
}

// RTC모듈의 시간값을 세팅하는 함수
void set3231Date() {
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
void get3231Date() {
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
float get3231Temp() {
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
void showSerialTime() {

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

void changeLEDbright() {
	bright = bright > MAX_BRI ? INCREASE_BRI : +INCREASE_BRI;
	strip.setBrightness(bright);
	displayTime(hour, min);
	//Serial.print("change brightness: ");
	//Serial.println(bright);
}

void changeLEDcolor() {
	//Serial.println("change color");
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

void increasingHour() {
	//Serial.println("hour plus");
	hourPlus = hourPlus > 23 ? 0 : +1;
	hour = (hourRtc + hourPlus) % 24;
	displayTime(hour, min);
}

void increasingMin() {
	//Serial.println("min plus");
	minPlus = minPlus > 59 ? 0 : +1;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
}

void increasingAlmHour() {
	//Serial.println("Alarm hour plus");
	hourPlus = hourPlus > 23 ? 0 : +1;
	hour = (hourRtc + hourPlus) % 24;
	displayTime(hour, min);
}

void increasingAlmMin() {
	//Serial.println("Alarm min plus");
	minPlus = minPlus > 59 ? 0 : +1;
	min = (minRtc + minPlus) % 60;
	displayTime(hour, min);
}
