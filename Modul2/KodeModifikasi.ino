const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

const int btnUp = 2;   
const int btnDown = 3; 

byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, {0,1,1,0,0,0,0,0}, {1,1,0,1,1,0,1,0}, {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0}, {1,0,1,1,0,1,1,0}, {1,0,1,1,1,1,1,0}, {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0}, {1,1,1,1,0,1,1,0}, {1,1,1,0,1,1,1,0}, {0,0,1,1,1,1,1,0},
  {1,0,0,1,1,1,0,0}, {0,1,1,1,1,0,1,0}, {1,0,0,1,1,1,1,0}, {1,0,0,0,1,1,1,0}
};

int currentDigit = 0;      
bool lastUpState = HIGH;   
bool lastDownState = HIGH; 

void displayDigit(int num) {
  for(int i=0; i<8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  for(int i=0; i<8; i++) pinMode(segmentPins[i], OUTPUT);

  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  
  displayDigit(currentDigit);
}

void loop() {
  bool upState = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  if(lastUpState == HIGH && upState == LOW) {
    currentDigit++;
    if(currentDigit > 15) currentDigit = 0; 
    displayDigit(currentDigit);
    delay(50); 
  }
  lastUpState = upState;

  if(lastDownState == HIGH && downState == LOW) {
    currentDigit--;
    if(currentDigit < 0) currentDigit = 15; 
    displayDigit(currentDigit);
    delay(50);
  }
  lastDownState = downState;
}
