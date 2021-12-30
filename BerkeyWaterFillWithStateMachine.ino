#include <StateMachine.h>

const int STATE_DELAY = 1000;

#define upperWaterSensorPin A0
#define lowerWaterSensorPin A1
#define fillWaterPumpPin A7
#define ledPin 13      // select the pin for the LED
int upperSensorValue = 0;
int upperPercentageValue = 0;
int lowerSensorValue = 0;
int lowerPercentageValue = 0;
int fillCycleCount = 0;

StateMachine machine = StateMachine();

State* WaitForStabilize = machine.addState(&startupStabilize); 
State* WaitForLowerEmpty = machine.addState(&waitForLowerEmpty);
State* FillOneSecond = machine.addState(&fillOneSecond);
State* TooMuchFilling = machine.addState(&tooMuchFilling);

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  pinMode(upperWaterSensorPin, INPUT);
  pinMode(lowerWaterSensorPin, INPUT);
  pinMode(fillWaterPumpPin, OUTPUT);

  WaitForStabilize->addTransition(&transitionStartupDone,WaitForLowerEmpty);
  WaitForLowerEmpty->addTransition(&transitionStartFilling,FillOneSecond);
  FillOneSecond->addTransition(&transitionErrorFilling,TooMuchFilling);
  FillOneSecond->addTransition(&transitionDoneFilling,WaitForLowerEmpty);
  FillOneSecond->addTransition(&transitionContinueFilling,FillOneSecond);
}

//
// WaitForStabilize
//

void startupStabilize(){
  Serial.println("WaitForStabilize");
  delay(4000); // Sensors to stabilize
}


//
// State Operations
//

void waitForLowerEmpty() {
  Serial.println("WaitForLowerEmpty");
  fillCycleCount = 0;
}

void fillOneSecond() {
  Serial.println("FillingForOneSecond");
  openValve();
  delay(1000);
  closeValve();
  fillCycleCount += 1;
  delay(3000); // For the sensor to stabilize
}

// Error
void tooMuchFilling() {
  Serial.println("TooMuchFilling");

  // Flash forever
  closeValve(); // Just to be sure
  
  while(true) {
    ledOn();
    delay(250);
    ledOff();
    delay(250);
  }
}

//
// Transition Checks
//

bool transitionStartupDone(){
  return true; // We know five seconds has passed
}

bool transitionStartFilling(){
  updateSensorValues();
  if ((lowerPercentageValue < 30) && (upperPercentageValue <  30)) {
    return true;
  }
  
 return false;
}

bool transitionErrorFilling() {
  updateSensorValues();
  if (fillCycleCount > 20) {
    return true;
  }
  return false;
}

bool transitionContinueFilling() {
  updateSensorValues();

  if (upperPercentageValue < 60) {
    return true;
  }

  return false;
}

bool transitionDoneFilling() {
  updateSensorValues();

  if (upperPercentageValue >= 60) {
    return true;
  }
  return false;
}

// Utilities

void ledOn() {
  digitalWrite(ledPin, HIGH);
}

void ledOff() {
  digitalWrite(ledPin, LOW);
}

void openValve() {
  digitalWrite(fillWaterPumpPin, HIGH);
}

void closeValve() {
  digitalWrite(fillWaterPumpPin, LOW);
}

int percentageFullSensorOne(int rawValue) {
  return (rawValue-500)*100L/(735-500);
}

int percentageFullSensorTwo(int rawValue) {
  return (rawValue-525)*100L/(670-525);
}

void updateSensorValues() {
  // read the value from the sensor:
  upperSensorValue = analogRead(upperWaterSensorPin);
  lowerSensorValue = analogRead(lowerWaterSensorPin);
  upperPercentageValue = percentageFullSensorOne(upperSensorValue); // < 25 not reliable
  lowerPercentageValue = percentageFullSensorTwo(lowerSensorValue); // < 25 not reliable
  // https://forum.arduino.cc/t/baffled-by-ranging-an-analog-value-into-a-percentage/364834/2
  Serial.println(upperPercentageValue);
  Serial.println(lowerPercentageValue);
  Serial.println("\n");
}

void loop() {
  machine.run();
  delay(STATE_DELAY);
}
