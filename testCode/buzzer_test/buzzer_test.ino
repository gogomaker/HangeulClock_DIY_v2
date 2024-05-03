void setup() {
  pinMode(7, OUTPUT);
}

void loop() {
  digitalWrite(7, HIGH);
  delay(180);
  digitalWrite(7, LOW);
  delay(20);
  digitalWrite(7, HIGH);
  delay(180);
  digitalWrite(7, LOW);
  delay(20);
  delay(400);
}
