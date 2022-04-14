#include "arduino.h"
#include <NewPing.h>
#ifndef DISTILLEDPRODUCT_H
#define DISTILLEDPRODUCT_H
class DistilledProduct{
  private:
    String title; // product name
    NewPing *tank; // where product is stored 
    unsigned short lowerTemp; // distilling temperature lower limit  
    unsigned short upperTemp; // distilling temperature upper limit  
    unsigned short averageLevel; 
  
  DistilledProduct::DistilledProduct(String title, unsigned short lowerTemp, unsigned short upperTemp, NewPing &tank);
  
  public:
    String getTitle();
    unsigned short getLowerTemp();
    unsigned short getUpperTemp();
    unsigned short getLevel();
    unsigned short getAverageLevel(byte sampleCount); 
  };

  
 #endif
