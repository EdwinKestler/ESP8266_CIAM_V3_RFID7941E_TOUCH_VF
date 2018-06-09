#include <SoftwareSerial.h>
#include "settings.h"
#include "BlinkRGB.h"
#include "TouchPadButton.h"

TouchPadButton btn(D1);
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;

BlinkRGB Azul (D6);
BlinkRGB Verde (D7);
BlinkRGB Rojo (D8);


const int beep = D5;
unsigned long RetardoLectura;


#define DataLenght 10
#define TypesofCards 13

SoftwareSerial RFIDReader(D2, D3, false, 256);
byte incomingdata;
String inputString;
char SearchSValue;
byte tagID[DataLenght];
char charBuff[DataLenght];
boolean readedTag = false;
unsigned int count = 0;

char CardMatrix[TypesofCards][18] = {
  {"EM4100"},
  {"MIFRAME1K"},
  {"MIFRAME4K"},
  {"HIDCARD"},
  {"T5567"},
  {"2ndCARD"},
  {"ISO14443B"},
  {"FELICA"},
  {"15693LABEL"},
  {"CPUCARD"},
  {"SECTORINFORMATION"},
  {"KEYBOARDDATA"},
  {"NOREGCARDTYPE"}
};

void setup() {
  pinMode(beep, OUTPUT);
  digitalWrite(beep, LOW);
  Serial.begin(115200);
  RFIDReader.begin(9600);
  Serial.println (F("\nSoftware serial test started"));
}
//----------------------------------------------------------------------denifinir el sonido de bocina
void buzzer() {
  digitalWrite(beep, HIGH);
  delay(300);
  digitalWrite(beep, LOW);
  delay(100);
}

void clearBufferArray()              // function to clear buffer array
{
  inputString = "";
  for (int i = 0; i < count; i++)
  {
    tagID[i] = NULL; // clear all index of array with command NULL
  }
}

void readTag() {
  if (RFIDReader.available()) {
    if (RFIDReader.available() == 0x02) { //if data header is present.
      while (RFIDReader.available() > 0) { // If data available from reader
        incomingdata = RFIDReader.read();
        /*Serial.print(count);
           Serial.print(F(":HEX:"));
           Serial.print(incomingdata,HEX);
           Serial.print(F(":DEC:"));
           Serial.print(incomingdata,DEC);
           Serial.println(F(""));*/
        tagID[count] = incomingdata;
        if (count > 3 && count < 8) {
          inputString += incomingdata;
        }
        delay(10);
        if (count == 12) break;
        count++ ;
      }
      Serial.print(F("RFID CARD ID IS: "));
      Serial.println(inputString);
      Verde.Flash();
      buzzer();
      inputString = "";
      count = 0;
      readedTag = !readedTag;
    }
  }
  return;
}


//-----------------------------------------------------------------------parsing tag
void ParseTag() {
  if (readedTag == true) {
    for (int index = 0; index < sizeof(tagID); index++) {
      //Serial.print(tagID[index], HEX);
      if (index == 0 && tagID[index] == 0x02) {
        Serial.println (F("there is valid data in card"));
      }
      if (index == 1 && tagID[index] == 0x0A) {
        Serial.println (F("data lenght is 10 bytes"));
      }
      if (index == 2) {
        int thiscardtype = 0;
        if ( tagID[index] == 0x02) thiscardtype = 0;
        if ( tagID[index] == 0x01) thiscardtype = 1;
        if ( tagID[index] == 0x03) thiscardtype = 2;
        if ( tagID[index] == 0x10) thiscardtype = 3;
        if ( tagID[index] == 0x11) thiscardtype = 4;
        if ( tagID[index] == 0x20) thiscardtype = 5;
        if ( tagID[index] == 0x21) thiscardtype = 6;
        if ( tagID[index] == 0x22) thiscardtype = 7;
        if ( tagID[index] == 0x30) thiscardtype = 8;
        if ( tagID[index] == 0x50) thiscardtype = 9;
        if ( tagID[index] == 0x51) thiscardtype = 10;
        if ( tagID[index] == 0x0FF) thiscardtype = 11;
        //else thiscardtype = 12;
        Serial.print(F("Type of card is: ")); Serial.println(CardMatrix[thiscardtype]);
      }
      if (index == 3) {
        Serial.print (F("Card Hidden card Number is :"));
        Serial.println (tagID[index], DEC);
      }
      if (index == 8) {
        Serial.print (F("Card CRC is:"));
        Serial.println (tagID[index], HEX);
      }
      if (index == 9 && tagID[index] == 0x03) {
        Serial.println (F("Data ended Successfully"));
      }
    }
    delay(100);
    clearBufferArray();
    readedTag = !readedTag;
  }
  return;
}

void readBtn(){
 if(btn.check()){
  if((millis() - lastDebounceTime) > debounceDelay){
     Serial.println("Pressed");
     Azul.Flash();
     buzzer();
     lastDebounceTime = millis();
  }
 }
}

void loop() {
 readTag();
 readBtn();
 //ParseTag();
 yield();
}
