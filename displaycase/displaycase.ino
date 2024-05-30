#include "Arduino.h"

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
const unsigned long situationDuration = 15000;  // 15 seconds
const unsigned long blinkInterval = 30000; // 30 seconds
const unsigned long blinkDelay = 500; // 500 milliseconds
const unsigned long flickerDelay = 250; // 250 milliseconds

unsigned long lastBlinkTime = 0;
unsigned long situationStartTime = 0;
unsigned long lastBlinkStepTime = 0;
unsigned long lastFlickerStepTime = 0;
int blinkStep = 0;
int flickerStep = 0;
bool inSituation = false;
bool blinking = false;
bool flickering = false;
Situation currentSituation;

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

  turnOnAllStreetLights(); // Ensure all street lights are on by default
  turnOnAllButtonLights(); // Ensure all button lights are on by default
  startBlinking(blinkCount); // Initial blink sequence
  lastBlinkTime = millis(); // Initialize the last blink time
}

void loop() {
  handleButtonPress();

  if (inSituation) {
    // If in a situation, check if the duration has passed
    if (millis() - situationStartTime >= situationDuration) {
      endSituation();
    }
  } else {
    // Check if it's time to blink the LEDs
    if (millis() - lastBlinkTime >= blinkInterval) {
      startBlinking(blinkCount);
      lastBlinkTime = millis();
    }
  }

  // Handle non-blocking blinking
  if (blinking) {
    handleBlinking();
  }

  // Handle non-blocking flickering
  if (flickering) {
    handleFlickering();
  }
}

void startBlinking(int times) {
  blinking = true;
  blinkStep = 0;
}

void handleBlinking() {
  if (millis() - lastBlinkStepTime >= blinkDelay) {
    if (blinkStep % 2 == 0) {
      digitalWrite(LightAPin, LOW);
      digitalWrite(LightBPin, LOW);
      digitalWrite(LightCPin, LOW);
    } else {
      digitalWrite(LightAPin, HIGH);
      digitalWrite(LightBPin, HIGH);
      digitalWrite(LightCPin, HIGH);
    }

    blinkStep++;
    lastBlinkStepTime = millis();

    if (blinkStep >= blinkCount * 2) {
      blinking = false;
      Serial.println("[Zenith Display] Ready");
    }
  }
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
  currentSituation = situation;
  inSituation = true;
  situationStartTime = millis();

  int pin;
  switch (situation) {
    case SituationA:
      pin = LightAPin;
      Serial.println("[Zenith Display] Situation A Started");
      break;
    case SituationB:
      pin = LightBPin;
      Serial.println("[Zenith Display] Situation B Started");
      break;
    case SituationC:
      pin = LightCPin;
      Serial.println("[Zenith Display] Situation C Started");
      break;
  }

  // Turn off all button lights and turn on the respective button light instantly
  turnOffAllButtonLights();
  digitalWrite(pin, HIGH);

  // Start flickering all street lights before activating the specific one
  startFlickering();
}

void endSituation() {
  inSituation = false;
  turnOnAllStreetLights(); // Turn on all street lights after the situation ends
  turnOnAllButtonLights(); // Turn on all button lights after the situation ends
  Serial.println("[Zenith Display] Situation Ended");
}

void startFlickering() {
  flickering = true;
  flickerStep = 0;
}

void handleFlickering() {
  if (millis() - lastFlickerStepTime >= flickerDelay) {
    if (flickerStep % 2 == 0) {
      digitalWrite(StreetLightAPin, HIGH);
      digitalWrite(StreetLightBPin, HIGH);
      digitalWrite(StreetLightCPin, HIGH);
    } else {
      digitalWrite(StreetLightAPin, LOW);
      digitalWrite(StreetLightBPin, LOW);
      digitalWrite(StreetLightCPin, LOW);
    }

    flickerStep++;
    lastFlickerStepTime = millis();

    if (flickerStep >= 6) {
      flickering = false;
      turnOnSituationStreetLight();
    }
  }
}

void turnOnSituationStreetLight() {
  // Ensure all street lights are off
  digitalWrite(StreetLightAPin, LOW);
  digitalWrite(StreetLightBPin, LOW);
  digitalWrite(StreetLightCPin, LOW);

  // Turn on the specific street light for the current situation
  if (currentSituation == SituationA) digitalWrite(StreetLightAPin, HIGH);
  if (currentSituation == SituationB) digitalWrite(StreetLightBPin, HIGH);
  if (currentSituation == SituationC) digitalWrite(StreetLightCPin, HIGH);
}

void turnOnAllButtonLights() {
  digitalWrite(LightAPin, HIGH);
  digitalWrite(LightBPin, HIGH);
  digitalWrite(LightCPin, HIGH);
}

void turnOffAllButtonLights() {
  digitalWrite(LightAPin, LOW);
  digitalWrite(LightBPin, LOW);
  digitalWrite(LightCPin, LOW);
}

void turnOnAllStreetLights() {
  digitalWrite(StreetLightAPin, HIGH);
  digitalWrite(StreetLightBPin, HIGH);
  digitalWrite(StreetLightCPin, HIGH);
}
