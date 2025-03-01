// 温度传感器
volatile int value;//定义热敏传感器模拟值变量
int ledPin = 2;  //定义数字口13
int inputPin = 13; //定义数字口3
void setup(){
  value = 10;
  Serial.begin(9600);//设置波特率
  pinMode(inputPin, INPUT);//定义A0为输入引脚
  pinMode(ledPin, OUTPUT);//定义6为输出引脚
}
 void loop(){
  value = analogRead(inputPin);//获取热敏电压模拟值
  Serial.print("Thermal Data：");
  Serial.println(value);//打印热敏电压模拟值
  if (analogRead(inputPin) > 1250) {//判断热敏值是否大于380
    digitalWrite(ledPin, HIGH); // LED亮起
  }
  delay(200);
  digitalWrite(ledPin, LOW); // LED变暗
}