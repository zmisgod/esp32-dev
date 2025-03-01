int pressure_pin = 33;
int jdq_pin = 12;
#define preesureTimeInterval 300
unsigned long pressureTimes =0;
int numAverage[30];
int sum = 0;
int average = 0;
int pressureVal = 0;
int diffVal = 0;
int maxPressure = 0;

void setup() {
  pinMode(jdq_pin, OUTPUT);
  //put your setup code here, to run once:
  Serial.begin(19500);
  maxPressure = analogRead(pressure_pin);
  for(int i = 0;i<30;i++) {
    numAverage[i] =  analogRead(pressure_pin);
    sum += numAverage[i];
  }
  average = sum /30;
}

void loop() {
  getPress();
}

void getPress() {
  if(millis() - pressureTimes >= preesureTimeInterval) {
    pressureTimes = millis();

    pressureVal = analogRead(pressure_pin);

    diffVal = abs(average - pressureVal);
    if(maxPressure - pressureVal > 0 ) {
      Serial.printf("\n average: %d", average);
      Serial.printf("\n pressureVal: %d", pressureVal);
      Serial.printf("\n diffVal: %d", diffVal);
      digitalWrite(jdq_pin, HIGH);
    }else{
      digitalWrite(jdq_pin, LOW);
    }
  }
}
