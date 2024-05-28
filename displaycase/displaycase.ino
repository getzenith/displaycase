#include "Arduino.h"
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

enum PinConstants {
  ButtonAPin = 2,
  ButtonBPin = 4,
  ButtonCPin = 6,
  LightAPin = 3,
  LightBPin = 5,
  LightCPin = 7,
  StreetLightAPin = 11,
  StreetLightBPin = 12,
  StreetLightCPin = 13
};

enum Situation {
  SituationA,
  SituationB,
  SituationC
};

const int blinkCount = 5;
const unsigned long trackDuration = 15000;  // 15 seconds
const unsigned long blinkInterval = 30000; // 30 seconds

bool inputEnabled = true;
unsigned long lastBlinkTime = 0;

SoftwareSerial softSerial(9, 8);
DFPlayerMini_Fast myMP3;

void setup() {
  pinMode(ButtonAPin, INPUT_PULLUP);
  pinMode(ButtonBPin, INPUT_PULLUP);
  pinMode(ButtonCPin, INPUT_PULLUP);
  pinMode(LightAPin, OUTPUT);
  pinMode(LightBPin, OUTPUT);
  pinMode(LightCPin, OUTPUT);
  pinMode(StreetLightAPin, OUTPUT);
  pinMode(StreetLightBPin, OUTPUT);
  pinMode(StreetLightCPin, OUTPUT);
  delay(100);
  Serial.begin(115200);

#if !defined(UBRR1H)
  softSerial.begin(9600);
  myMP3.begin(softSerial, true);
#else
  Serial1.begin(9600);
  myMP3.begin(Serial1, true);
#endif
  myMP3.volume(30);

  turnOnAllStreetLights(); // Ensure all street lights are on by default
  blinkAllLEDs(blinkCount); // Initial blink sequence
  lastBlinkTime = millis(); // Initialize the last blink time
}

void loop() {
  if (inputEnabled) {
    handleButtonPress();
    // Check if it's time to blink the LEDs
    if (millis() - lastBlinkTime >= blinkInterval) {
      blinkAllLEDs(blinkCount);
      lastBlinkTime = millis();
    }
  }
}

void blinkAllLEDs(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LightAPin, HIGH);
    digitalWrite(LightBPin, HIGH);
    digitalWrite(LightCPin, HIGH);
    delay(500);
    digitalWrite(LightAPin, LOW);
    digitalWrite(LightBPin, LOW);
    digitalWrite(LightCPin, LOW);
    delay(500);
  }
  Serial.println("[Zenith Display] Ready");
}

void handleButtonPress() {
  if (digitalRead(ButtonAPin) == LOW) {
    startSituation(SituationA);
  } else if (digitalRead(ButtonBPin) == LOW) {
    startSituation(SituationB);
  } else if (digitalRead(ButtonCPin) == LOW) {
    startSituation(SituationC);
  }
}

void startSituation(Situation situation) {
  disableInputs();
  int pin;
  uint16_t trackNum;
  switch (situation) {
    case SituationA:
      pin = LightAPin;
      Serial.println("[Zenith Display] Situation A Started");
      trackNum = 1; // Adjust track number as needed
      myMP3.play(trackNum);
      digitalWrite(StreetLightAPin, HIGH);
      digitalWrite(StreetLightBPin, LOW);
      digitalWrite(StreetLightCPin, LOW);
      break;
    case SituationB:
      pin = LightBPin;
      Serial.println("[Zenith Display] Situation B Started");
      trackNum = 2; // Adjust track number as needed
      myMP3.play(trackNum);
      digitalWrite(StreetLightAPin, LOW);
      digitalWrite(StreetLightBPin, HIGH);
      digitalWrite(StreetLightCPin, LOW);
      break;
    case SituationC:
      pin = LightCPin;
      Serial.println("[Zenith Display] Situation C Started");
      trackNum = 3; // Adjust track number as needed
      myMP3.play(trackNum);
      digitalWrite(StreetLightAPin, LOW);
      digitalWrite(StreetLightBPin, LOW);
      digitalWrite(StreetLightCPin, HIGH);
      break;
  }
  digitalWrite(pin, HIGH);
  unsigned long startTime = millis();
  while (millis() - startTime < trackDuration) {
    // No need to check DFPlayer's state here
    delay(100);
  }
  digitalWrite(pin, LOW);
  turnOnAllStreetLights(); // Turn on all street lights after the situation ends
  enableInputs();
  lastBlinkTime = millis(); // Reset blink timer after a situation ends
}

void disableInputs() {
  inputEnabled = false;
}

void enableInputs() {
  inputEnabled = true;
}

void turnOnAllStreetLights() {
  digitalWrite(StreetLightAPin, HIGH);
  digitalWrite(StreetLightBPin, HIGH);
  digitalWrite(StreetLightCPin, HIGH);
}
