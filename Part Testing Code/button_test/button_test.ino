void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if(digitalRead(2) == LOW)
    Serial.println("MODE_SW_PUSH");
  if(digitalRead(9) == LOW)
    Serial.println("LED_SW_PUSH");
  if(digitalRead(10) == LOW)
    Serial.println("TIME_SW_PUSH");
  if(digitalRead(11) == LOW)
    Serial.println("ALARM_SW_PUSH");
}
