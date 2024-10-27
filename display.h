#include <LiquidCrystal.h>    // include Arduino LCD library
//#include "utils.h"
// string for each row on LCD
String rowOneText = "";
String rowTwoText = "";
String rowThreeText = "";
String rowFourText = "";

void displayValues(
  LiquidCrystal lcd, 
  uint16_t v,
  uint16_t current1,
  uint16_t level1,
  uint16_t temp1, 
  uint16_t cont1,
  uint16_t v2,
  uint16_t current2,
  uint16_t level2,
  uint16_t temp2, 
  uint16_t cont2
  ){
  
  /*
   * 
  ________________________
  |  |  C  | T | L | OC  |
  |T1|2.3  |23 |50 | H   |
  |T2|3.3  |34 |60 | L   |
  |V1: 1200 | V2: 1000   |
  ________________________
  
  */
  // row one text
  rowOneText = "   ";
  rowOneText += "|";
  rowOneText += " C ";
  rowOneText += "|";
  rowOneText += " T ";
  rowOneText += "|";
  rowOneText += " L ";
  rowOneText += "|";
  rowOneText += " OC";
  
  lcd.setCursor(0, 0);
  lcd.print(rowOneText);

  // row two text: Tranformer 1 parameters:
  rowTwoText = "T1";
  rowTwoText += " |";
  lcd.setCursor(0, 1);
  lcd.print(rowTwoText);

  // current 1
  lcd.setCursor(4, 1);
  lcd.print(formatValue(current1, 3));
  lcd.setCursor(7, 1);
  lcd.print("|");
  
  // temperature 1
  lcd.setCursor(8, 1);
  lcd.print(formatValue(temp1, 3));
  lcd.setCursor(11, 1);
  lcd.print("|");

    // level 1
  lcd.setCursor(12, 1);
  lcd.print(formatValue(level1, 3));
  lcd.setCursor(15, 1);
  lcd.print("|");

    // oil contamination 1
  lcd.setCursor(16, 1);
  lcd.print(formatValue(cont1, 3));
  lcd.setCursor(19, 1);
  lcd.print("%");

  // row three text: Transformer 2 parameters:
  rowTwoText = "T2";
  rowTwoText += " |";
  lcd.setCursor(0, 2);
  lcd.print(rowTwoText);

  // current 2
  lcd.setCursor(4, 2);
  lcd.print(formatValue(current2, 3));
  lcd.setCursor(7, 2);
  lcd.print("|");
  
  // temperature 2
  lcd.setCursor(8, 2);
  lcd.print(formatValue(temp2, 3));
  lcd.setCursor(11, 2);
  lcd.print("|");

  // level 2
  lcd.setCursor(12, 2);
  lcd.print(formatValue(level2, 3));
  lcd.setCursor(15, 2);
  lcd.print("|");

  // oil contamination 2
  lcd.setCursor(16, 2);
  lcd.print(formatValue(cont2, 3));
  lcd.setCursor(19, 2);
  lcd.print("%");


  // row four text: voltage for both transformers
  lcd.setCursor(0, 3);
  lcd.print("[V1:");
  lcd.setCursor(4, 3);
  lcd.print(formatValue(v, 4));

  lcd.setCursor(9, 3);
  lcd.print("][");

  lcd.setCursor(11, 3);
  lcd.print("V2:");
  lcd.setCursor(15, 3);
  lcd.print(formatValue(v2, 4));
  lcd.setCursor(19, 3);
  lcd.print("]");
}
