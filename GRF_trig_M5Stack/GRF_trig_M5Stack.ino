/*
 Programed by Akito Kosugi
 ver 1.0 03.10.2026
*/ 
#include <M5Stack.h>
#include <time.h>

int checkIdx(void);
void writeData(char *filename, char *buffStr);

#define pin_ai     36
#define pin_dio    16
#define pin_switch 26

bool bSwitchOn;
int count = 0;
int readData;
int readData_prev;
int th = 2260;
float startTime; 
float time_from_start;
const long delay_ms = 10;
const long dulation_ms = 100;
unsigned long countTime_prev = 0;
const long countInterval_ms = 200;

void trig_on(int on_time){
  M5.Lcd.setCursor(10, 200);
  M5.Lcd.fillRect(10, 200, 150, 30, BLACK);
  M5.Lcd.println("Trig ON");
  count += 1;
  digitalWrite(pin_dio,HIGH);
  delay(on_time);
  digitalWrite(pin_dio,LOW);
  M5.Lcd.setCursor(10, 200);
  M5.Lcd.fillRect(10, 200, 150, 30, BLACK);
}

void setup() {  
  
  //Initialization
  M5.begin();
  pinMode(pin_dio, OUTPUT);
  pinMode(pin_ai, INPUT);
  pinMode(pin_switch, INPUT_PULLUP);

  //Display
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(40,10);
  M5.Lcd.print("M5Stack analog input");
  M5.Lcd.setCursor(0,70);
  M5.Lcd.print("Value:");  
  M5.Lcd.setCursor(0,100);
  M5.Lcd.print("Th:");   
  M5.Lcd.setCursor(0,130);
  M5.Lcd.print("Count:");   
   
  Serial.begin(115200);
  
  startTime = millis(); 
}
 
void loop(){ 
  //Definition
  char buff[128];

  //Update
  M5.update(); 

  //Data aquisition
  readData = analogRead(pin_ai);
  bSwitchOn = digitalRead(pin_switch);
  time_from_start = (millis()-startTime)/1000;
  
  //TTL output
  if(M5.BtnC.wasPressed()) {
    trig_on(200);
  }
  if(bSwitchOn == false){
    if(millis() > countTime_prev+countInterval_ms){
      trig_on(200);
      countTime_prev = millis();
    }
  }
  if(readData > th && readData_prev < th){
    if(millis() > countTime_prev+countInterval_ms){
      trig_on(200);
      countTime_prev = millis();
    }
  }
  
  //Setup
  if(M5.BtnA.wasPressed()){
    th += 10;
    if (th > 4096){
      th = 4096;
    }
  }
  if(M5.BtnB.wasPressed()){
    th -= 10;
    if (th < 0){
      th = 0;
    }
  }  
  
  //Display
  M5.Lcd.setCursor(150,70);
  M5.Lcd.fillRect(150,70,100,20,BLACK);
  M5.Lcd.print(readData);
  M5.Lcd.setCursor(150,100);
  M5.Lcd.fillRect(150,100,100,20,BLACK);
  M5.Lcd.print(th);
  M5.Lcd.setCursor(150,130);
  M5.Lcd.fillRect(150,130,100,20,BLACK);
  M5.Lcd.print(count);

  //Serial.print(String(time_from_start) + ", " + S1 + "\n");

  readData_prev = readData;
  delay(delay_ms);
}