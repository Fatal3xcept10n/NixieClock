/*
  ShiftRegister74HC595 - Library for simplified control of 74HC595 shift registers.
  Developed and maintained by Timo Denk and contributers, since Nov 2014.
  Additional information is available at https://timodenk.com/blog/shift-register-arduino-library/
  Released into the public domain.
*/

// create a global shift register object
// parameters: <number of shift registers> (data pin, clock pin, latch pin)

#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

int dataPin = 4, clockPin = 6, latchPin = 2, state = 0, fetPin = 3;
int numerals[] = {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9};
unsigned long operation, data, finalInt;
uint8_t hourOff = 23, hourOn = 6, minuteOff = 30, minuteOn = 30;
uint8_t hou, minu, seco, dayy, monthh, offsetSecond = 7;
uint16_t yearr;
bool getTime = false, getDate = false;
bool sleep = false;
String fin;

void draw(uint8_t h, uint8_t m, uint8_t s) {
  uint8_t h_r = h / 10;
  uint8_t h_l = h % 10;
  uint8_t h_b = (h_l << 4) | h_r;
  
  uint8_t m_r = m / 10;
  uint8_t m_l = m % 10;
  uint8_t m_b = (m_l << 4) | m_r;
  
  uint8_t s_r = s / 10;
  uint8_t s_l = s % 10;
  uint8_t s_b = (s_l << 4) | s_r;

  shiftOut(s_b);
  shiftOut(m_b);
  shiftOut(h_b);

  digitalWrite(latchPin, LOW);
  digitalWrite(latchPin, HIGH);
   
}

void turnoff() {
  digitalWrite(fetPin, LOW);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) ; // wait for serial
  delay(200);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(fetPin, OUTPUT);
  digitalWrite(fetPin, HIGH);
}

void shiftOut(uint8_t val) {
  uint8_t i;
  for (i=0; i<8; i++) {
    digitalWrite(dataPin, (val & 128) != 0);
    val <<= 1;
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

void cathodePoison(){
  for(int i = 0; i < 30; i++){
    for(int x = 0; x <= 9; x++){
      draw(combine(numerals[x], numerals[x]),combine(numerals[x], numerals[x]),combine(numerals[x], numerals[x]));
      delay(70);
    }
  }
}

int combine(int a, int b) {
   int times = 10;
   while (times <= b)
      times *= 10;
   return a*times + b;
}

void loop() {
  tmElements_t tm;
  if (RTC.read(tm)) {
    if(Serial.available() > 0){
      byte buffer[64]; // create a buffer array to store the response
      int bytesRead = Serial.readBytesUntil('\n', buffer, sizeof(buffer)); // read the response into the buffer array
      buffer[bytesRead] = '\0'; // add a null terminator to the end of the buffer array
      // convert the buffer array to a string and print it
      fin = String((char*)buffer); // convert the buffer array to a string
      fin.replace("\r", "");
      finalInt = fin.toInt();
      operation = finalInt / 1000000U;
      data = finalInt % 1000000U;
      switch(operation){
        case 1:
          hou = data / 10000U;
          minu = (data / 100U) % 100U;
          seco = data % 100U;
          getTime = true;
          break;
        case 2:
          dayy = data / 10000U;
          monthh = (data / 100U) % 100U;
          yearr = 2000 + (data % 100U);
          getDate = true;
          break;  
        case 3:
          hourOff = data / 10000U;
          minuteOff = (data / 100U) % 100U;
          break;
        case 4:
          hourOn = data / 10000U;
          minuteOn = (data / 100U) % 100U;
          break;
        case 5:
          offsetSecond = data / 10000U;
          break;
        default:
          break;
      }
    }

    if(getTime && getDate){
      setTime(hou,minu,seco,dayy,monthh,yearr);
      RTC.set(now());
      getTime = false;
      getDate = false;
    }

    if(tm.Hour == hourOff && tm.Minute == minuteOff && sleep != true){
      sleep = true;
      for(int x=0;x<2;x++){
        cathodePoison();
      }
      RTC.read(tm);
      uint8_t tempH = tm.Hour, tempM = tm.Minute, tempS = tm.Second, tempD = tm.Day, tempMon = tm.Month, tempY = tm.Year;
      Serial.println(tempH);
      Serial.println(tempM);
      Serial.println(tempS-offsetSecond);
      setTime(tempH, tempM, tempS-offsetSecond, tempD, tempMon, tempY);
      RTC.set(now());
    }else if(tm.Hour == hourOn && tm.Minute == minuteOn && sleep != false){
      sleep = false;
    }

    if (sleep){
      turnoff();
    }else{
      digitalWrite(fetPin, HIGH);
      draw(tm.Hour, tm.Minute, tm.Second);
      if(((tm.Minute % 10) == 0) && (tm.Second == 20)){
        cathodePoison();
      }
    }
  }
}