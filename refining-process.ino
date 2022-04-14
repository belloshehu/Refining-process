/*
 * TITLE: Refining process control system
 * AUTHOR: Shehu Bello
 * DATE: 15/01/2022
*/

#include "max6675.h"
#include "level.h"
#include "customCharacters.h"
#include <LCD_I2C.h>

LCD_I2C lcd(0x27);

int thermoDO = 5;
int thermoCS = 6;
int thermoCLK = 7;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// valve config:
byte naphthaValvePin = 0;
byte dieselValvePin = 1;
byte keroValvePin = 2;
byte residueValvePin = 3;

// start/stop buttons
byte startButtonPin = 11;
byte stopButtonPin = 12;

// column span 
byte colSpan = 4; 

byte buzzerPin = 8;

#define products[] {"Kero", "Naphtha", "Diesel", "Bitumen"}
// temperature set points:
#define keroLowTemp 0 //kerosine lower temperature
#define keroHighTemp 0 //kerosine upper temperature

#define dieselLowTemp 0 //diesel lower temperature
#define dieselHighTemp 0 //diesel upper temperature

#define naphthaLowTemp 0 //naphtha lower temperature
#define naphthaHighTemp 0 //naphtha upper temperature

#define residueLowTemp 0 //residue lower temperature
#define residueHighTemp 0 //residue upper temperature

// flags: for indicating various states in the process
bool started = false;

String lastProductName = "";

void setup() {
  // put your setup code here, to run once:
  lcd.begin(0X27);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("   WELCOME   ");
  lcd.setCursor(0, 1); // Or setting the cursor in the desired position.
  lcd.print("Crude refining");
  lcd.setCursor(0, 2); // Or setting the cursor in the desired position.
  lcd.print("system");

  // create the customi characters
  lcd.createChar(0, openIcon);
  lcd.createChar(1, closeIcon);
  lcd.createChar(2, downArrow);
  
  pinMode(naphthaValvePin, OUTPUT);
  pinMode(dieselValvePin, OUTPUT);
  pinMode(keroValvePin, OUTPUT);
  pinMode(residueValvePin, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);
}

float readTemperatureValue(){
//  float temperatureValue = thermocouple.readCelsius();
//  return temperatureValue;
}

void closeAllValves(){
  digitalWrite(dieselValvePin, LOW);
  digitalWrite(keroValvePin, LOW);
  digitalWrite(naphthaValvePin, LOW);
  digitalWrite(residueValvePin, LOW);
  }

void openValve(byte valvePin){
   closeAllValves();
   digitalWrite(valvePin, HIGH);
  }

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
void displayDistillingProduct(String productName, unsigned short level, unsigned short temp){
  
  String productText = productName;

  // row 1 text
  productText += " ";
  productText += "at";
  productText += " ";
  productText += String(temp);
  lcd.setCursor(0, 0);
  lcd.print(productText);
  // display open icon
  lcd.setCursor(19, 0);
  lcd.write(0);
  
  //row 2 text:
  String productText2 = "";
  productText2 += level;
  productText2 += " ";
  productText2 += "litres";
  lcd.setCursor(1, 0);
  lcd.print(productText2);
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


void displayProduct(String valveState, unsigned short level, String productName){
  byte colNumber;
  if(lastProductName != productName){
      lcd.clear();
    }
  if(productName.equalsIgnoreCase("D")){
    colNumber = 0; // place DPK in row 1
    }
  else if(productName.equalsIgnoreCase("A")){
    colNumber = 1* colSpan; // place AGO in row 2
    }
  else if(productName.equalsIgnoreCase("N")){
    colNumber = 2 * colSpan; // place napthalene in row 3
    }
  else if(productName.equalsIgnoreCase("B")){
     colNumber = 3 * colSpan; // place Bitumen in row 1
   }
 
    lcd.setCursor(colNumber + 20, 0);
    lcd.print(productName);
    lcd.setCursor(productName.length() + 20, 0); // place icon after product name 
    if(valveState.equalsIgnoreCase("open")){
      lcd.write(0);
      lcd.write(2);
      }else{
       lcd.write(1);
     }

    // print level on the last row
    lcd.setCursor(colNumber+ 20, 1);
    lcd.print(String(level) + " |");
    
    lastProductName = productName;
}

/*
  Displays non distiling products
*/
void nonDistillingProducts(String product1, String product2, String product3){
  // Ago
        unsigned short agoLevel = averageLevel(agoTank, 10);
        displayProduct("close", agoLevel, "Ago" );

        // Naphtha
        unsigned short naphthaLevel = averageLevel(naphTank, 10);
        displayProduct("close", naphthaLevel, "Naphtha" );

        // Bitumen
        unsigned short bitLevel = averageLevel(bitTank, 10);
        displayProduct("close", bitLevel, "Bitumen" );
 }
 
void process(){
  float tempVal = thermocouple.readCelsius();
  unsigned short level = 0;
  // when process is started
  if(started){
      
      if((tempVal <= keroHighTemp) && (tempVal >= keroLowTemp) ){
        // open valve for kero if temperature is with the range
        openValve(keroValvePin);  
        level = averageLevel(keroTank, 10);
        
        displayDistillingProduct("Kerosine", level, tempVal );
        
        // display non distilling products

      }
        
      if((tempVal <= dieselHighTemp) && (tempVal >= dieselLowTemp) ){
        // open valve for diesel if temperature is with the range
        openValve(dieselValvePin);
        level = averageLevel(agoTank, 10);  
        displayProduct("open", level, "Diesel");
        }

      if((tempVal <= naphthaHighTemp) && (tempVal >= naphthaLowTemp) ){
        // open valve for naphtha if temperature is with the range
        openValve(naphthaValvePin); 
        level = averageLevel(naphTank, 10); 
        displayProduct("open", level, "Naphtha");
        }

      if((tempVal <= residueHighTemp) && (tempVal >= residueLowTemp)){
        // open valve for residue if temperature is with the range
        openValve(residueValvePin);  
        level = averageLevel(bitTank, 10);
        displayProduct("open", level, "Residue");
        }
        
       else{
          // no product matches temperature range
          closeAllValves();
          //displayProcess(temp, "none", "none");
        }
    }
    else{
      return;
     }
  }


void loop() {
  // put your main code here, to run repeatedly:
 if(digitalRead(startButtonPin) == LOW){
    delay(150);
    started = true;
  }
 if(digitalRead(stopButtonPin) == LOW){
    delay(150);
    started = false;
  }

  process();
}


