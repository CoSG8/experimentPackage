/*
  Programmed by Akito Kosugi
  for reaching_marmo_operant.py
  ver.1.4 01.06.2025
*/

const int pin_speaker = 8;
const int pin_button = 7;
const int pin_solenoid = 9;
const int pin_LED1 = 2;
const int pin_LED2 = 4;
//const int pin_stimTrig = 4;
const int pin_successTrig = 5;
const int pin_successTrig_toArduino = 3;

bool bSwitch_on;
bool bTask;
bool bTaskReady;
bool value;

int toneTimeTh1 = 150;
int toneTimeTh2 = 400;
int toneTimeTh3 = 600;

unsigned int cmd;

void setup() {

  pinMode(pin_button, INPUT);
  pinMode(pin_LED1, OUTPUT);
  pinMode(pin_LED2, OUTPUT);
  //pinMode(pin_stimTrig, OUTPUT);
  pinMode(pin_successTrig, OUTPUT);
  pinMode(pin_successTrig_toArduino, OUTPUT);
  pinMode(pin_speaker, OUTPUT);
  pinMode(pin_solenoid, OUTPUT);

  bSwitch_on = false;
  bTaskReady = true;
  bTask = false;

  Serial.begin(9600);
}

void loop() {

  unsigned int ser_avail = Serial.available();

  while (ser_avail > 0) {
    value = digitalRead(pin_button);
    if (value == LOW) {
      digitalWrite(pin_LED1, LOW);
      digitalWrite(pin_LED2, LOW);
      //digitalWrite(pin_stimTrig, LOW);
      digitalWrite(pin_solenoid, LOW);
      bTask = false;
      bTaskReady = true;
    }

    unsigned int cmd = Serial.read();
    
    if (cmd == 'L') {
      if (bTaskReady == true) {
        digitalWrite(pin_LED1, HIGH);
        //digitalWrite(pin_stimTrig, HIGH);
        tone(pin_speaker, 500, toneTimeTh3);
        delay(500);
        digitalWrite(pin_LED2, HIGH);
        delay(500);
        digitalWrite(pin_solenoid, HIGH);
        // bTaskReady = false;
        bTask = true;
      }
    }
    // else if (cmd == 'C') {
    //   // if (bTaskReady == false){
    //   if (bTask == false){
    //     bTask = true;
    //     delay(500);
    //   }
    //   // }
    // }
    else if (cmd == 'R') {
      if (bTask == true) {
        bTaskReady = false;
        //digitalWrite(pin_stimTrig, LOW);        
        digitalWrite(pin_LED2, LOW);
        digitalWrite(pin_successTrig, HIGH);
        digitalWrite(pin_successTrig_toArduino, HIGH);
        tone(pin_speaker, 987, toneTimeTh1);
        delay(toneTimeTh1);
        tone(pin_speaker, 1318, toneTimeTh2);
        digitalWrite(pin_successTrig, LOW);
        digitalWrite(pin_successTrig_toArduino, LOW);        
        delay(700);
        // delay(2650);
        digitalWrite(pin_LED1, LOW);
         digitalWrite(pin_solenoid, LOW);
        bTask = false;
        bTaskReady = true;
        while (Serial.available() > 0) {
          unsigned int cmd_temp = Serial.read();
        }
      }
    }

    // else if (cmd == 'B') {
    //   if(bTaskReady == false){
    //     digitalWrite(pin_LED, LOW);
    //     if (bSwitch_on == false){
    //       delay(3000);
    //       digitalWrite(pin_solenoid, LOW);
    //     }
    //     bTaskReady = true;
    //   }
    // }
    else if (cmd == 'S') {
      digitalWrite(pin_solenoid, HIGH);
    }
    // Serial.println(bTaskReady);
  }
}