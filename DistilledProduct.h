#include "arduino.h"
#include <NewPing.h>
#ifndef DISTILLEDPRODUCT_H
#define DISTILLEDPRODUCT_H


class DistilledProduct{
  private:
    String title; // product name
    NewPing *tank; // where product is stored 
    uint16_t lowerTemp; // distilling temperature lower limit  
    uint16_t upperTemp; // distilling temperature upper limit  
    uint16_t averageLevel; 
    byte valvePin; 
    String valveState;
    byte MAX_TANK_HEIGHT;

  
  public:
    DistilledProduct::DistilledProduct(
      String title, 
      uint16_t lowerTemp, 
      uint16_t upperTemp, 
      NewPing &tank, 
      byte valvePin
     );
     
    String getTitle();
    uint16_t getLowerTemp();
    uint16_t getUpperTemp();
    uint16_t getLevel();
    uint16_t getAverageLevel(byte sampleCount); 
    byte getValvePin();
    String getValveState();
    void setValveState(String state);
    void closeValve();
    void openValve();
    uint16_t cmToPercentageLevel();
    void configureValvePin();
  };

  
 #endif
