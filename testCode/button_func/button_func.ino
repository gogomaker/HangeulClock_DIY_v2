int swpin[4] = {2, 9, 10, 11};  //단순히 핀 매핑해 놓은 것 뿐
bool sw_org_stat[4] = {HIGH, HIGH, HIGH, HIGH};  // 체터링을 거른 버튼의 상태를 표현하는 변수
bool l_sw_stat[4]   = {HIGH, HIGH , HIGH, HIGH}; // 마지막 버튼 상태
int sw_prcs_val[4]  = {0, 0, 0, 0};       // 버튼을 프로그램에서 처리할 때 사용하는 값
unsigned long l_deb_tme[4] = {0, 0, 0, 0};    // 디바운스 시간 체킹
unsigned long sw_w[4] = {0, 0, 0, 0};     // 시간 버튼이 언제 눌렸는가

unsigned long times = millis();

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  Serial.begin(1000000);
}

void loop() {
  times = millis();/*
  Serial.print(digitalRead(2));
  Serial.print(" / ");
  Serial.print(digitalRead(9));
  Serial.print(" / ");
  Serial.print(digitalRead(10));
  Serial.print(" / ");
  Serial.print(digitalRead(11));
  Serial.println("");*/

  Serial.print(sensingSW(0));
  Serial.print(" / ");
  Serial.print(sensingSW(1));
  Serial.print(" / ");
  Serial.print(sensingSW(2));
  Serial.print(" / ");
  Serial.print(sensingSW(3));
  Serial.println("");
}


int sensingSW(int index)
{
  bool reading = digitalRead(swpin[index]);
  Serial.print(reading);
  Serial.print("-");
  if (reading != l_sw_stat[index])
    l_deb_tme[index] = times;
  if ((times - l_deb_tme[index]) < 50) { 
    Serial.print("Debouncing-");
    l_sw_stat[index] = reading;
    return 0;
  }
  if (reading == sw_org_stat[index]) { 
    Serial.print("Not changed-");
    l_sw_stat[index] = reading;
    return 0; 
  }
  sw_org_stat[index] = reading;

  if (sw_org_stat[index] == LOW) {
    sw_w[index] = times;
    Serial.print("pushed-");
    l_sw_stat[index] = reading;
    return 0;
  } else {
    l_sw_stat[index] = reading;
    return sw_w[index] > times - 1200 ?  2 : 1;
  }
}
