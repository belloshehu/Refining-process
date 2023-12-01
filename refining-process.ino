/*
 * TITLE: Refining process control system
 * AUTHOR: Shehu Bello
 * DATE: 15/01/2022
*/

#include "max6675.h"
#include "customCharacters.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DistilledProduct.h"
#include "level.h"
#include "utils.h"


// valve config:
byte naphthaValvePin = 14;
byte dieselValvePin = 15;
byte keroValvePin = 16;
byte residueValvePin = 17;
byte lastIndex = 0;
// time values
unsigned long dischargeDuration = 120000;  //2*60*1000 (2 minutes)
unsigned short tempMargin = 2; // margin between each temperature range

// create instances of DistilledProduct to represent
// products being distilled

DistilledProduct naphtha("Naphtha", 40, 55, naphTank, naphthaValvePin); // real Values: lower = 40, upper = 170
DistilledProduct kerosene("Kerosene", 57, 70, keroTank, keroValvePin); // real Values: lower = 170, upper = 250
DistilledProduct diesel("Diesel", 72, 85, agoTank, dieselValvePin); // real Values: lower = 250, upper = 350
DistilledProduct residue("Residue", 87, 100, bitTank, residueValvePin); // real Values: lower = 350, upper = beyond 350

LiquidCrystal_I2C lcd(0x27, 20, 4);

int thermoDO = 5;
int thermoCS = 6;
int thermoCLK = 7;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// start/stop buttons
byte startButtonPin = 12;
byte stopButtonPin = 13;

// column span 
byte colSpan = 5; 
bool entryFlag = false;
bool firstIteration = false;

byte buzzerPin = 4;

DistilledProduct prod[4] = {naphtha, kerosene, diesel, residue};
//#define productTitles[] {"Naphtha", "Kero", "Diesel", "Residue"}

String lastProductName = "";

void setup() {
  // put your setup code here, to run once:
  // configure pins

  kerosene.configureValvePin();
  naphtha.configureValvePin();
  diesel.configureValvePin();
  residue.configureValvePin();
  
//  lcd.backlight();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("PTI Crude oil");
  lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
  lcd.print("Batch Distillation");
  lcd.setCursor(7, 2); // Or setting the cursor in the desired position.
  lcd.print("Process");

  // create the custom/icons characters
  lcd.createChar(0, openIcon);
  lcd.createChar(1, closeIcon);
  lcd.createChar(2, downArrow);
  lcd.createChar(3, degreeIcon);
  
  for(byte i = 0; i < 4; i++){
      pinMode(prod[i].getValvePin(), OUTPUT);
    }
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);
  
  onOffBeep(buzzerPin, 2);
  // clear display
  delay(4000);
  lcd.clear();

  // run tests
  //  tests();
  //  delay(2000);
  //  lcd.clear();
  
}

void closeAllValves(){
  // Turns all valve off
  for(byte i = 0; i < sizeof(prod); i++){
      prod[i].closeValve();
    }
  }


void displayDistillingProduct(DistilledProduct &product){
  /*
 * Displays the product being distilled on the first and second row
 * 
 * args:
 *  productName(String): Name of product being distilled
 *  level(unsigned short ): level of the product in the tank
 *  temp(unsigned short): temperature of the process
 *  
 * returns: none
 *  
*/
  String productText = product.getTitle();

  // row 1 text
  productText += " ";
  productText += "at";
  productText += " ";
  productText += String(formatValue(thermocouple.readCelsius(), 3));
  lcd.setCursor(0, 0);
  lcd.print(productText);
  // display degree icon
  lcd.setCursor(productText.length(), 0);
  lcd.write(3);
  lcd.setCursor(productText.length()+1, 0);
  lcd.print("C");
  
  //row 2 text:
  String productText2 = "";
  productText2 += String(formatValue(product.getAverageLevel(10), 3));
//  productText2 += " ";
//  productText2 += "%";

  // display open icon 
  lcd.setCursor(0, 1);
  lcd.write(0);
  
  lcd.setCursor(14, 1);
  lcd.print(productText2);

  lcd.setCursor(17, 1);
  lcd.print("%");
  }


void displayResidueDischarge(DistilledProduct &product, unsigned long timeElapsed){
  /*
 * Displays residue when discharging. 
 * 
 * args:
 *  product object reference
 *  
 * returns: none
 *  
*/
  String productText = product.getTitle();

  // row 1 text
  productText += " ";
  productText += "discharging";
  lcd.setCursor(0, 0);
  lcd.print(productText);
  
  productText = "at";
  productText += " ";
  productText += String(formatValue(thermocouple.readCelsius(), 3));
  lcd.setCursor(0, 1);
  lcd.print(productText);

  // degree icon
  lcd.setCursor(productText.length() + 1, 1);
  lcd.write(3);
  lcd.setCursor(productText.length()+2, 1);  
  lcd.print("C");
  //row 2 text:
  // display open icon 
  lcd.setCursor(18, 1);
  lcd.write(0);


  lcd.setCursor(0, 2);
  lcd.print("Please wait ....");
  lcd.setCursor(0, 3);
  // time is converted from milli second to minute. 
  // there 60 * 1000 milli seconds in a minute
  String timingText = String(timeElapsed / 1000);
  timingText += " of ";
  timingText += String(dischargeDuration / 1000);
  timingText += " secs (";
  timingText += String(dischargeDuration / 60000);
  timingText += " mins)";
  lcd.print(timingText);
  }

/*
 
  ---- DISPLAY FORMAT ---
   ----------------------
  | D: 000 | 00 | open   |
  | A: 000 | 00 | close  |
  | N: 000 | 00 | close  |
  | B: 000 | 00 | close  |
   ----------------------

     ---- DISPLAY FORMAT ---
   ----------------------
  | Diesel at 300C       |
  | 200 liters           |
  | A  |  K  | N   | D   | 
  | 000| 000 | 000 | 000 |
   ----------------------
*/  
void displayNormal(){
  // Displays the process temperature when no product is being distilled
  
  String text = "Temperature: " + formatValue(thermocouple.readCelsius(), 3);
  lcd.setCursor(0, 1);
  lcd.print(text);
  
  lcd.setCursor(text.length(), 1);
  lcd.write(3);
  
  lcd.setCursor(text.length() + 1, 1);
  lcd.print("C");
}

void displayProduct(DistilledProduct &product, byte pos){
  byte colNumber;
  String productName = product.getTitle();
    colNumber = pos * colSpan;
    lcd.setCursor(colNumber, 2);
    lcd.print(product.getTitle()[0]);
   
    if(product.getValveState().equalsIgnoreCase("open")){
      lcd.setCursor(colNumber + 3, 2); // place open icon after product name 
      lcd.write(0);
      lcd.setCursor(colNumber + 2, 2); // place down arrow icon after product name 
      lcd.write(2);
      }else{
       lcd.setCursor(colNumber + 3, 2); // place close icon after product name 
       lcd.write(1);
     }
    lcd.setCursor(colNumber+4, 2);
    lcd.print("|");
     
    // print level on the last row
    lcd.setCursor(colNumber, 3);
    lcd.print(formatValue(product.getLevel(), 3));
    lcd.setCursor(colNumber + 4, 3);
    lcd.print("|");
    
    lastProductName = productName;
}

 
byte process(){
  float tempVal = thermocouple.readCelsius();
  firstIteration = false;
  unsigned short level = 0;
  byte distillingIndex = 0; // index of the product distilling
  
  // when process is started
 
  lcd.setCursor(0, 0);
  lcd.print("Refining stopped..");
  displayNormal();
  for(byte index=0; index < 4; index++){
        displayProduct(prod[index], index);
        prod[index].closeValve();
   }
  bool started = onOff(buttonPressCount(startButtonPin, 0));
  while(started == true){    
    // clear LCD on first iteration:
    if(firstIteration == false){
      lcd.setCursor(0, 0);
      lcd.print("Refining started..");
      onOffBeep(buzzerPin, 3);
      lcd.clear();
      firstIteration = true;
     }
     
    tempVal = thermocouple.readCelsius();
    bool found = false; // indicates if distilling product is found
    
    // loop through all products and display each 
    for(byte index=0; index < 4; index++){
      uint16_t productLowerTemp = prod[index].getLowerTemp();
      uint16_t productUpperTemp = prod[index].getUpperTemp();
      if((tempVal >= productLowerTemp) && (tempVal <= productUpperTemp) && index < 3){
         // clear screen when in new range
        if(lastIndex != index){
          lcd.clear();
          delay(1500);
         }
        displayDistillingProduct(prod[index]);
        prod[index].openValve();
        found = true;
        distillingIndex = index; 
        lastIndex = index;
      }else if((tempVal >= productLowerTemp) && index == 3){ 
        // wait till discharge time;
        lcd.clear();
        unsigned long dischargeStartTime = millis();
        unsigned long timeElapsed = 0;
        while(((millis() - dischargeStartTime) <= dischargeDuration) && index == 3){
          displayResidueDischarge(prod[index], timeElapsed);
          prod[index].openValve();
          found = true;
          distillingIndex = index;
          timeElapsed = millis() - dischargeStartTime;  
          } 
          
          // indicate that process is completed
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Process Completed...");
          delay(2000); 
          lcd.clear(); 
          started = onOff(buttonPressCount(startButtonPin, 1));
          return 0; 
        }
        
    }

    // close all valve 
    for(byte index=0; index < 4; index++){
        displayProduct(prod[index], index);
        if(index != distillingIndex){
          prod[index].closeValve();
         }
      }
      
    if(found == false){
      displayNormal();
      delay(1000);
      }
    started = onOff(buttonPressCount(startButtonPin, 0));
    if(started == false){
      onOffBeep(buzzerPin, 1);
     }
  }
  delay(2000);
  lcd.clear();
 }


void loop() {
  // put your main code here, to run repeatedly:
  process();  
}
