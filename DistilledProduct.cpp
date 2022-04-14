#include "DistilledProduct.h"
#include <NewPing.h>

DistilledProduct::DistilledProduct(String title, unsigned short lowerTemp, unsigned short upperTemp, NewPing &tank){
  this->lowerTemp = lowerTemp;
  this->upperTemp = upperTemp;
  averageLevel = 0;
  this->tank = &tank;
  this->title = title;
}

String DistilledProduct:: getTitle(){
  return this->title;
  }

unsigned short DistilledProduct:: getLowerTemp(){
  return this->lowerTemp;
  }

unsigned short DistilledProduct:: getUpperTemp(){
  return this->upperTemp;
  }

unsigned short DistilledProduct::getLevel(){
  return tank->ping_cm();
  }

/*
 Calculates the average level of product in the tank

 args:
  sampleCount(byte): number of samples to be taken

 returns:
  averageLevel: average level of fluid in cm.
*/
unsigned short DistilledProduct::getAverageLevel(byte sampleCount){
   unsigned long sum = 0; 
   for(int count = 0; count < sampleCount; count++){
    sum += tank->ping_cm();
    delay(10);
    }
  averageLevel = sum / sampleCount;
  return averageLevel;
  }

