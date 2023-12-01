#include "DistilledProduct.h"

DistilledProduct::DistilledProduct(String title, uint16_t lowerTemp, uint16_t upperTemp, NewPing &tank, 
  byte valvePin){
  this->lowerTemp = lowerTemp;
  this->upperTemp = upperTemp;
  averageLevel = 0;
  this->tank = &tank;
  this->title = title;
  this->valvePin = valvePin;
  MAX_TANK_HEIGHT = 50;
  
}

void DistilledProduct::configureValvePin(){
  pinMode(valvePin, OUTPUT);
  }
String DistilledProduct:: getTitle(){
  return this->title;
  }

uint16_t DistilledProduct:: getLowerTemp(){
  return this->lowerTemp;
  }

uint16_t DistilledProduct:: getUpperTemp(){
  return this->upperTemp;
  }

uint16_t DistilledProduct::getLevel(){
  return tank->ping_cm();
  }

/*
 Calculates the average level of product in the tank

 args:
  sampleCount(byte): number of samples to be taken

 returns:
  averageLevel: average level of fluid in cm.
*/
uint16_t DistilledProduct::getAverageLevel(byte sampleCount){
   unsigned long sum = 0; 
   for(int count = 0; count < sampleCount; count++){
    sum += tank->ping_cm();
    delay(10);
    }
  averageLevel = sum / sampleCount;
  return averageLevel;
  }
  
byte DistilledProduct::getValvePin(){
  return this->valvePin;
 }

void DistilledProduct::setValveState(String state){
  valveState = state;
  }

void DistilledProduct::openValve(){
  digitalWrite(valvePin, LOW);
  setValveState("open");
  }

void DistilledProduct::closeValve(){
  digitalWrite(valvePin, HIGH);
  setValveState("close");
  }
  
String DistilledProduct::getValveState(){
  return valveState;
  }

uint16_t DistilledProduct::cmToPercentageLevel(){
  // converts level in cm to percentage
  return (100 - 100*(tank->ping_cm())/MAX_TANK_HEIGHT); // MAX_TANK_HEIGHT = 50
}
