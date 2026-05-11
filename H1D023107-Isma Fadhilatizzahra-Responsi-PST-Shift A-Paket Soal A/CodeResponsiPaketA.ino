const int segmentPins[8] = {7, 6, 5, 3, 10, 8, 9, 4}; 
const int btnRequest = 2; 
const int ledGreen = 11;
const int ledYellow = 12;
const int ledRed = 13;

byte digitPattern[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

bool lastBtnState = HIGH;

void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void clearDisplay() {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], LOW);
  }
}

void setup() {
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(btnRequest, INPUT_PULLUP);

  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
  clearDisplay();
}

void loop() {
  bool currentBtnState = digitalRead(btnRequest);

  if(lastBtnState == HIGH && currentBtnState == LOW) {
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, HIGH);
    delay(2000); 

    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, HIGH);

    for(int i = 9; i >= 0; i--) {
      displayDigit(i);
      delay(1000); 
    }

  lastBtnState = currentBtnState;
}