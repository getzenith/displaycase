// Define pin numbers for buttons and lights
const int buttonAPin = 7;
const int buttonBPin = 6;
const int buttonCPin = 8;
const int lightAPin = 13;
const int lightBPin = 2;
const int lightCPin = 3;

bool inputEnabled = true;

void setup() {
  // Initialize buttons as inputs
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonBPin, INPUT_PULLUP);
  pinMode(buttonCPin, INPUT_PULLUP);
  
  // Initialize lights as outputs
  pinMode(lightAPin, OUTPUT);
  pinMode(lightBPin, OUTPUT);
  pinMode(lightCPin, OUTPUT);
  
  Serial.begin(9600);
  
  blinkAllLEDs(5); // Blink all LEDs 5 times
}

void loop() {
  if (inputEnabled) {
    if (digitalRead(buttonAPin) == LOW) {
      situationA();
    } else if (digitalRead(buttonBPin) == LOW) {
      situationB();
    } else if (digitalRead(buttonCPin) == LOW) {
      situationC();
    }
  }
}

void blinkAllLEDs(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(lightAPin, HIGH);
    digitalWrite(lightBPin, HIGH);
    digitalWrite(lightCPin, HIGH);
    delay(500);
    digitalWrite(lightAPin, LOW);
    digitalWrite(lightBPin, LOW);
    digitalWrite(lightCPin, LOW);
    delay(500);
  }
}

void situationA() {
  disableInputs();
  digitalWrite(lightAPin, HIGH);
  Serial.println("[Zenith Display] Situation A Started");
  delay(1000); // Adjust delay time as needed
  Serial.println("[Zenith Display] Situation A Ended");
  digitalWrite(lightAPin, LOW);
  enableInputs();
  blinkAllLEDs(5); // Blink all LEDs 5 times
}

void situationB() {
  disableInputs();
  digitalWrite(lightBPin, HIGH);
  Serial.println("[Zenith Display] Situation B Started");
  delay(1000); // Adjust delay time as needed
  Serial.println("[Zenith Display] Situation B Ended");
  digitalWrite(lightBPin, LOW);
  enableInputs();
  blinkAllLEDs(5); // Blink all LEDs 5 times
}

void situationC() {
  disableInputs();
  digitalWrite(lightCPin, HIGH);
  Serial.println("[Zenith Display] Situation C Started");
  delay(1000); // Adjust delay time as needed
  Serial.println("[Zenith Display] Situation C Ended");
  digitalWrite(lightCPin, LOW);
  enableInputs();
  blinkAllLEDs(5); // Blink all LEDs 5 times
}

void disableInputs() {
  inputEnabled = false;
}

void enableInputs() {
  inputEnabled = true;
}
