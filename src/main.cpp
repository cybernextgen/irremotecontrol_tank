#include <Arduino.h>
#include <IRremote.h>
#include <EEPROM.h>

#define IR_RECEIVER_PIN 5
#define BUZZER_PIN 4
#define M1_DIR_PIN 12
#define M2_DIR_PIN 13
#define M1_PWM_PIN 10
#define M2_PWM_PIN 11
#define MAX_PWM 255
#define COMMAND_TIMEOUT_INTERVAL 120
#define SHORT_BEEP_DURATION 20

IRrecv irrecv(IR_RECEIVER_PIN);  
unsigned long lastCommandReceivedMillis = 0;
bool isCommandReceived = false;

unsigned long lastBeepEnabledMillis = 0;
bool isBeepEnabled = false;

unsigned short currentSpeed =  0;
int8_t trimmer = 0;

decode_results results;

void handleShortBeep(){
  if(isBeepEnabled){
    if(millis() - lastBeepEnabledMillis >= SHORT_BEEP_DURATION){
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}

void shortBeep(){
  if(!isBeepEnabled){
    isBeepEnabled = true;
    lastBeepEnabledMillis = millis();
    digitalWrite(BUZZER_PIN, HIGH);
  }
}

void setSpeed(unsigned short speed){
  currentSpeed = map(speed, 0, 100, 0, MAX_PWM);
  shortBeep();
}

void forward(){
  digitalWrite(M1_DIR_PIN, HIGH);
  digitalWrite(M2_DIR_PIN, HIGH);
  analogWrite(M1_PWM_PIN, currentSpeed);
  analogWrite(M2_PWM_PIN, currentSpeed);
}

void backward(){
  digitalWrite(M1_DIR_PIN, LOW);
  digitalWrite(M2_DIR_PIN, LOW);
  analogWrite(M1_PWM_PIN, currentSpeed);
  analogWrite(M2_PWM_PIN, currentSpeed);
}

void left(){
  digitalWrite(M1_DIR_PIN, LOW);
  digitalWrite(M2_DIR_PIN, HIGH);
  analogWrite(M1_PWM_PIN, currentSpeed);
  analogWrite(M2_PWM_PIN, currentSpeed);
}

void right(){
  digitalWrite(M1_DIR_PIN, HIGH);
  digitalWrite(M2_DIR_PIN, LOW);
  analogWrite(M1_PWM_PIN, currentSpeed);
  analogWrite(M2_PWM_PIN, currentSpeed);
}

void stop(){
  digitalWrite(M1_DIR_PIN, HIGH);
  digitalWrite(M2_DIR_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  analogWrite(M1_PWM_PIN, 0);
  analogWrite(M2_PWM_PIN, 0);

}

void longBeep(){
  digitalWrite(BUZZER_PIN, HIGH);
}

void readTrimmerValue(){
  EEPROM.get(0, trimmer);
}

void writeTrimmerValue(){
  EEPROM.put(0, trimmer);  
}

void changeTrimmer(bool isIncriment){
  if(isIncriment){
    if(trimmer < 100) trimmer += 1;
  }else{
    if(trimmer > -100) trimmer -= 1;
  };
  shortBeep();
  writeTrimmerValue();
}

void handleIR(){
  if (irrecv.decode())
  {
    uint16_t decodedCommand = irrecv.decodedIRData.command;
    lastCommandReceivedMillis = millis();
    isCommandReceived = true;
    irrecv.resume();
    //Serial.println(decodedCommand);
    if(decodedCommand == 6){
      forward();  
    }else if(decodedCommand == 90){
      left();
    }else if(decodedCommand == 27){
      right();
    }else if(decodedCommand == 22){
      backward();
    }else if(decodedCommand == 82){
      setSpeed(10);
    }else if(decodedCommand == 80){
      setSpeed(20);
    }else if(decodedCommand == 16){
      setSpeed(30);
    }else if(decodedCommand == 86){
      setSpeed(40);
    }else if(decodedCommand == 84){
      setSpeed(50);
    }else if(decodedCommand == 20){
      setSpeed(60);
    }else if(decodedCommand == 78){
      setSpeed(70);
    }else if(decodedCommand == 76){
      setSpeed(80);
    }else if(decodedCommand == 12){
      setSpeed(90);
    }else if(decodedCommand == 15){
      setSpeed(100);
    }else if(decodedCommand == 26){
      longBeep();
    }else if(decodedCommand == 77){
      changeTrimmer(false);
    }else if(decodedCommand == 13){
      changeTrimmer(true);
    };
    
  }

  if(isCommandReceived && millis() - lastCommandReceivedMillis > COMMAND_TIMEOUT_INTERVAL){
    stop();
    isCommandReceived = false;
    isBeepEnabled = false;
  }
}
 
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  setSpeed(50);
  readTrimmerValue();
}
 
void loop() {
  handleIR();
  handleShortBeep();
}



