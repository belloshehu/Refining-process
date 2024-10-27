#include "arduino.h"

#define TRIG_PIN_KERO  1  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_KERO  0  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIG_PIN_NAPH  3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_NAPH  2  // Arduino pin tied to echo pin on the ultrasonic sensor.

// SENSOR 3
#define TRIG_PIN_AGO  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_AGO  8  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIG_PIN_BIT  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_BIT  10  // Arduino pin tied to echo pin on the ultrasonic sensor.

#define MAX_LEVEL 50 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Newping instance for kerosene
NewPing keroTank(TRIG_PIN_KERO, ECHO_PIN_KERO, MAX_LEVEL); 

// Newping instance for Diesel
NewPing agoTank(TRIG_PIN_AGO, ECHO_PIN_AGO, MAX_LEVEL);

// Newping instance for Bitumen
NewPing bitTank(TRIG_PIN_BIT, ECHO_PIN_BIT, MAX_LEVEL); 

// Newping instance for Naphthalene
NewPing naphTank(TRIG_PIN_NAPH, ECHO_PIN_NAPH, MAX_LEVEL); 

unsigned short averageLevel(NewPing &tankInstance, byte iterationCount){
  unsigned long average = 0;
  for(int count = 0; count < iterationCount; count++){
    average += tankInstance.ping_cm();
    delay(10);
    }
  return average / iterationCount;
  }
  
float cmToPercentageLevel(int cmLevel){
  // converts level in cm to percentage
  return (100 - 100*cmLevel/MAX_LEVEL);
}
