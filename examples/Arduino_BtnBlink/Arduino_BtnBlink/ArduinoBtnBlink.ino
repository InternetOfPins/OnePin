
void setup() {
  pinMode(13,OUTPUT);
  pinMode(4,INPUT_PULLUP);
}

void loop() {
  if (digitalRead(4)) {//converts to bool => reads the pin!
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
  } else {
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
    delay(90);
  }
}
