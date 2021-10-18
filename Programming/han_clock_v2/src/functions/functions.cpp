#include "functions.h"

void displayTime(int h, int m) {
	ledclear();
	updateHour(h);
	updateMin(m);
	strip.show();
}

int updateHour(int h) {
	printled(3);
	//오전 오후 출력
	if(h < 12) { printled(29); printled(30); }
	else { printled(29), printled(28); }
	//시간 출력
	if (h == 0 || h == 12)       { printled(31); printled(26); } // 열두시
	else if (h == 1 || h == 13)  { printled(27); }				 // 한  시
	else if (h == 2 || h == 14)  { printled(26); }				 // 두  시
	else if (h == 3 || h == 15)  { printled(17); }				 // 세  시
	else if (h == 4 || h == 16)  { printled(19); }				 // 네  시
	else if (h == 5 || h == 17)  { printled(18); printled(15); } // 다섯시
	else if (h == 6 || h == 18)  { printled(16); printled(15); } // 여섯시
	else if (h == 7 || h == 19)  { printled(14); printled( 7); } // 일곱시
	else if (h == 8 || h == 20)  { printled(16); printled( 5); } // 여덟시
	else if (h == 9 || h == 21)  { printled( 6); printled( 4); } // 아홉시
	else if (h == 10 || h == 22) { printled(31); }				 // 열  시
	else if (h == 11 || h == 23) { printled(31); printled(27); } // 열한시
}

int updateMin(int m) {
	if(m) printled(0);
	int ten = m / 10;
	if      (ten == 1) { printled(12); printled(0); }
	else if (ten == 2) { printled(12); printled(16); }
	else if (ten == 3) { printled(12); printled(15); }
	else if (ten == 4) { printled(12); printled(14); }
	else if (ten == 5) { printled(12); printled(13); }

	m -= ten * 10;
	if      (m == 1) printled(7);
	else if (m == 2) printled(8);
	else if (m == 3) printled(9);
	else if (m == 4) printled(10);
	else if (m == 5) printled(4);
	else if (m == 6) printled(11);
	else if (m == 7) printled(3);
	else if (m == 8) printled(2);
	else if (m == 9) printled(1);
}

void printled(int n) {
	if (ledmode == 0) {
		rSeed++;
		randomSeed(rSeed);
		byte rand = random(0, COLOR_CNT);
		r = color[rand][0];
		g = color[rand][1];
		b = color[rand][2];
		w = 0;
	}
	strip.setPixelColor(n, r, g, b, w);
}

void ledclear() {
	for (int i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, 0, 0, 0);
	}
	strip.show();
}

void blinkAllLED() {
	if (isblinkLED == true) {
		if (time >= wait_t + BLINK_INTERVAL) {
			//Serial.println("blink!");
			displayTime(hour, min);
			isblinkLED = false;
		}
	}
}

void changeModeSW() {
	
}

void changeAlarmSW() {
	bool reading = digitalRead(ALARM_SW);		// 버튼 상태 읽기
	if (reading != l_deb_tme[ALM_INDEX]) {	// 버튼 바운싱 걸러냄
		l_deb_tme[ALM_INDEX] = time;
	}
	if ((time - l_deb_tme[ALM_INDEX]) > DEB_DLY) {	// 디바운스가 아닌 진짜 버튼 눌림이면
		if (reading != sw_stat[ALM_INDEX]) {	//저장되 있는 버튼 값과 현재 버튼의 상태가 다르다면
			sw_stat[ALM_INDEX] = reading;
			if (sw_stat[ALM_INDEX] == LOW) {	//버튼이 눌렸다면
				//Serial.println("Time_pressed");
				sw_w[ALM_INDEX] = time;
				sw_check[ALM_INDEX] = true;
			}
			else {	//버튼이 때졌다면 / 버튼이 때 졌을 때를 기준으로 작동이 일어나기에 코드가 복잡함
				if (time < sw_w[ALM_INDEX] + SW_INTERVAL) {		//short pressed
					if(isTchange == true) {	//readme 기능 파트 참조
						//Serial.println("hour plus");
						hourPlus++;
						if (hourPlus > 23) hourPlus = 0;
						hour = (hourRtc + hourPlus) % 24;
						displayTime(hour, min);
					}
					if(isAchange == true) {
						alarm_time[0]++;
					}
					sw_check[ALM_INDEX] = false;
				}
			}
		}
	}
	l_sw_stat[TME_INDEX] = reading;
}

void changeTimeSW() {
	bool reading = digitalRead(TIME_SW);		// 버튼 상태 읽기
	if (reading != l_deb_tme[TME_INDEX]) {	// 버튼 바운싱 걸러냄
		l_deb_tme[TME_INDEX] = time;
	}
	if ((time - l_deb_tme[TME_INDEX]) > DEB_DLY) {	// 디바운스가 아닌 진짜 버튼 눌림이면
		if (reading != sw_stat[TME_INDEX]) {	//저장되 있는 버튼 값과 현재 버튼의 상태가 다르다면
			sw_stat[TME_INDEX] = reading;
			if (sw_stat[TME_INDEX] == LOW) {	//버튼이 눌렸다면
				//Serial.println("Time_pressed");
				sw_w[TME_INDEX] = time;
				sw_check[TME_INDEX] = true;
			}
			else {	//버튼이 때졌다면 / 버튼이 때 졌을 때를 기준으로 작동이 일어나기에 코드가 복잡함
				if (time < sw_w[TME_INDEX] + SW_INTERVAL) {		//short pressed
					if(isTchange == true) {	//readme 기능 파트 참조
						//Serial.println("hour plus");
						hourPlus++;
						if (hourPlus > 23) hourPlus = 0;
						hour = (hourRtc + hourPlus) % 24;
						displayTime(hour, min);
					}
					if(isAchange == true) {
						alarm_time[0]++;
						if(alarm_time[0] > 23) alarm_time[0] = 0;
					}
					sw_check[TME_INDEX] = false;
				}
			}
		}
	}
	l_sw_stat[TME_INDEX] = reading;
}

void changeLedSW() {
	bool reading = digitalRead(LED_SW);
	if (reading != l_sw_stat[LED_INDEX]) {
		l_deb_tme[LED_INDEX] = time;
	}
	if ((time - l_deb_tme[LED_INDEX]) > DEB_DLY) {
		if (reading != sw_stat[LED_INDEX]) {
			sw_stat[LED_INDEX] = reading;
			if (sw_stat[LED_INDEX] == LOW) {
				//Serial.println("LED_pressed");
				sw_w[LED_INDEX] = time;
				sw_check[LED_INDEX] = true;
			}
			else {
				if (time < sw_w[LED_INDEX] + SW_INTERVAL) {
					bright += INCREASE_BRI;
					if (bright > MAX_BRI) bright = INCREASE_BRI;
					strip.setBrightness(bright);
					displayTime(hour, min);
					//Serial.print("change brightness: ");
					//Serial.println(bright);
					sw_check[LED_INDEX] = false;
				}
			}
		}
	}
	l_sw_stat[LED_INDEX] = reading;
}

void longTimeSW() {
	if (sw_check[TME_INDEX] == true) {
		if ((time - sw_w[TME_INDEX]) >= SW_INTERVAL) {
			if(isAchange == false) {	//알람 설정 모드가 아닐 경우에만
				if(isTchange == true) {
					isTchange = false;
					isblinkLED = true;
					ledclear();	//LED 소등
					blinkAllLED();
				}
				else {
					isTchange == true;
					blinkAllLED();
				}
			}
		}
	}
}

void longLedSW() {
	if (sw_check[LED_INDEX] == true) {
		if ((time - sw_w[LED_INDEX]) >= SW_INTERVAL) {
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
			sw_check[LED_INDEX] = false;
		}
	}
}

byte decToBcd(byte val) {
	return ((val / 10 * 16) + (val % 10));
}

void set3231Date() {
	//year = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//month = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//date = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//hour = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//min = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//sec = (byte)((Serial.read() - 48) * 10 + (Serial.read() - 48));
	//day = (byte)(Serial.read() - 48);
	if (!min) hour++;
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0x00);
	Wire.write(sec);
	Wire.write(decToBcd(min));
	Wire.write(decToBcd(hour));
	//Wire.write(decToBcd(day));
	//Wire.write(decToBcd(date));
	//Wire.write(decToBcd(month));
	//Wire.write(decToBcd(year));
	Wire.endTransmission();

	minPlus = hourPlus = 0;
}

void get3231Date() {
	// send request to receive data starting at register 0
	Wire.beginTransmission(DS3231_I2C_ADDRESS); // 104 is DS3231 device address
	Wire.write(0x00); // start at register 0
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7); // request seven bytes

	if (Wire.available()) {
		sec = Wire.read(); // get seconds
		minRtc = Wire.read(); // get minutes
		hourRtc = Wire.read();   // get hours
	//	day = Wire.read();
	//	date = Wire.read();
	//	month = Wire.read(); //temp month
	//	year = Wire.read();

		sec = (((sec & B11110000) >> 4) * 10 + (sec & B00001111)); // convert BCD to decimal
		minRtc = (((minRtc & B11110000) >> 4) * 10 + (minRtc & B00001111)); // convert BCD to decimal
		hourRtc = (((hourRtc & B00110000) >> 4) * 10 + (hourRtc & B00001111)); // convert BCD to decimal (assume 24 hour mode)
	//	day = (day & B00000111); // 1-7
	//	date = (((date & B00110000) >> 4) * 10 + (date & B00001111)); // 1-31
	//	month = (((month & B00010000) >> 4) * 10 + (month & B00001111)); //msb7 is century overflow
	//	year = (((year & B11110000) >> 4) * 10 + (year & B00001111));
	}
	else {
		//oh noes, no data!
	}

	/*switch (day) {
	case 1:
		strcpy(weekDay, "Sun");
		break;
	case 2:
		strcpy(weekDay, "Mon");
		break;
	case 3:
		strcpy(weekDay, "Tue");
		break;
	case 4:
		strcpy(weekDay, "Wed");
		break;
	case 5:
		strcpy(weekDay, "Thu");
		break;
	case 6:
		strcpy(weekDay, "Fri");
		break;
	case 7:
		strcpy(weekDay, "Sat");
		break;
	}*/
}

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
