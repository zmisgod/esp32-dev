

int led_pin = 2;

void setup() {
  // put your setup code here, to run once:
  // pinMode(led_pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(led_pin, HIGH);
  // delay(1000);
  // digitalWrite(led_pin, LOW);
  // delay(1000);
  for (int i=0; i <= 255; i++){
    analogWrite(led_pin, i);
    delay(10);
  }
}
