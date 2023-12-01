unsigned long lastDebounceTime = 0;
unsigned long debounceDelayTime = 50;
unsigned long count = 0;

String createZeros(byte count){
  /*
  Creates a string with number of zeros(0) determine by "count argument"

  args:
    count: Number of spaces to be created

  returns:
    Created string
  */
  String zeros = "";
  
  for(byte i = 0; i < count; i++){
    zeros += "0";
    }
    
  return zeros;
  }

  

uint16_t conditionValue(uint16_t oriValue, uint16_t maxValue){
  /*
   Condition value to certain limit
  */

  if(oriValue > maxValue){
    return 0;
    }else{
    return oriValue;
    }
  }



String formatValue(uint16_t value, byte maxDigitCount){
  /*
    Prepare values for display on LCD.

    it prepends "0" appropriately 
    to the value to be displayed
  */
  String formatedValue = "";
  byte digit = 1;
  if(value < 10){ // unit value: 1 digit
      
      formatedValue += createZeros(maxDigitCount - digit); //prepends 
      formatedValue += String(value);
    }else if(value >= 10 and value <= 100){
      digit = 2;
      formatedValue += createZeros(maxDigitCount - digit); //prepends 
      formatedValue += String(value);
    }else if(value >= 100 and value <= 1000){
      digit = 3;
      formatedValue += createZeros(maxDigitCount - digit); //prepends 
      formatedValue += String(value);
    }else if(value >= 1000 and value <= 10000){
      digit = 4;
      formatedValue += createZeros(maxDigitCount - digit); //prepends 
      formatedValue += String(value);
    }else{
      formatedValue += String(value);
     }
    return formatedValue;
  }

bool startStop(byte startButtonPin, byte stopButtonPin){
  /*
   Set boolean flag to either false or true to indicate whether 
   process is started or stopped. It returns boolean true when
   startButton is pressed and boolean false when stopButton is
   pressed.

   returns:
      bool
  */ 
  bool flag = false; 
  int startReading = digitalRead(startButtonPin); // state of the stop pin
  int stopReading = digitalRead(stopButtonPin); // state of the stop pin
  
  if(startReading == HIGH){
    delay(300);
    return true;
    }
  if(stopReading == HIGH){
    delay(300);
    return false;
    }
}


bool readButtonState(byte buttonPin){
  /*
   Set boolean flag to either false or true to indicate whether 
   process is started or stopped. It returns boolean true when
   startButton is pressed and boolean false when stopButton is
   pressed.

   returns:
      bool
  */ 
  int reading = digitalRead(buttonPin); // state of the button pin
  
  if(reading == LOW){
    delay(300);
    return true;
    }else{
      return false;
   }

}


unsigned long buttonPressCount(byte buttonPin, byte padding){
  /*
   Set boolean flag to either false or true to indicate whether 
   process is started or stopped. It returns boolean true when
   startButton is pressed and boolean false when stopButton is
   pressed.

   returns:
      bool
  */ 
  int reading = digitalRead(buttonPin); // state of the button pin
  
  if(reading == LOW){
    delay(300);
    count +=1;
    }
  return count + padding;
}

bool onOff(unsigned long pressCount){
  // returns true if count is even otherwise false.
  if(pressCount % 2 == 1){
    return true;
    }else{
    return false;
    }
}

void onOffBeep(byte pin, byte beepCount){
  for(byte i = 0; i < beepCount; i++){
    digitalWrite(pin, HIGH);
    delay(200);
    digitalWrite(pin, LOW);
    delay(300);
    }
 }
