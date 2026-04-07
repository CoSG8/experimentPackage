/*
 Programed by Akito Kosugi
 ver 1.1 03.19.2026
*/ 
#include <M5Stack.h>
#include <time.h>

int checkIdx(void);
void writeData(char *filename, char *buffStr);

#define pin_ai  36
#define pin_do  16
#define pin_di  26

bool bSwitchOn;
bool bTrig;
bool bTrigReady;
int count = 0;
int readData;
int readData_prev;
int th = 2;
int freq = 1;
float startTime; 
float time_from_start;
const long delay_ms = 10;
const long dulation_ms = 100;
unsigned long countTime_prev = 0;
unsigned long switchTime_prev = 0;
const long countInterval_ms = 1000;

void trig_on(int on_time){
  M5.Lcd.setCursor(10, 220);
  M5.Lcd.fillRect(10, 220, 150, 30, BLACK);
  M5.Lcd.println("Trig ON");
  digitalWrite(pin_do,HIGH);
  delay(on_time);
  digitalWrite(pin_do,LOW);
  M5.Lcd.setCursor(10, 220);
  M5.Lcd.fillRect(10, 220, 150, 30, BLACK);
}

void setup() {  
  
  //Initialization
  M5.begin();
  pinMode(pin_do, OUTPUT);
  pinMode(pin_ai, INPUT);
  pinMode(pin_di, INPUT_PULLUP);
  
  bTrig = true;
  bTrigReady = true;

  //Display
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(40,10);
  M5.Lcd.print("M5Stack EMG trigger");
  M5.Lcd.setCursor(0,70);
  M5.Lcd.print("Value:");
  M5.Lcd.setCursor(0,100);
  M5.Lcd.print("Th:");
  M5.Lcd.setCursor(0,130);
  M5.Lcd.print("Count:");
  M5.Lcd.setCursor(0,160);
  M5.Lcd.print("Switch:");
  M5.Lcd.setCursor(0,190);
  M5.Lcd.print("Status:");

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
  bSwitchOn = digitalRead(pin_di);
  time_from_start = (millis()-startTime)/1000;
  
  //TTL output
  if(readData > th && readData_prev < th){
    if(millis() > countTime_prev+countInterval_ms){
      countTime_prev = millis();
      count += 1;
      if(bTrigReady){
        trig_on(200);
        bTrigReady = false;
      }
    }
  }

  //Switch
  if(bSwitchOn == false){
    if(millis() > switchTime_prev+countInterval_ms){
      switchTime_prev = millis();
      bTrig =! bTrig;
    }
  }

  if(bTrig){
    if (count%freq == 0){
      bTrigReady = true;
    }
    else{
      bTrigReady = false;    
    }
  }
  else{
    bTrigReady = false;
  }

  //Setup
  if(M5.BtnA.wasPressed()){
    th += 1;
    if (th > 4096){
      th = 4096;
    }
  }
  if(M5.BtnB.wasPressed()){
    th -= 1;
    if (th < 0){
      th = 0;
    }
  }  
  if(M5.BtnC.wasPressed()){
    bTrig =! bTrig;
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
  M5.Lcd.setCursor(150,160);
  M5.Lcd.fillRect(150,160,100,20,BLACK);
  M5.Lcd.print(bSwitchOn);
  M5.Lcd.setCursor(150,190);
  M5.Lcd.fillRect(150,190,100,20,BLACK);
  M5.Lcd.print(bTrig);
  Serial.print(String(readData) + "\n");

  readData_prev = readData;
  delay(delay_ms);
}