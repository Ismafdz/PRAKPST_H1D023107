![Wiring Diagram](https://github.com/user-attachments/assets/c839112a-c880-45f6-b5d8-9deb636c982d)

Code:
```cpp
int timer = 100;

void setup() { 
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    pinMode(ledPin, OUTPUT); 
  } 
} 

void loop() { 
  for (int ledPin = 2; ledPin < 7; ledPin++) { 
    digitalWrite(ledPin, HIGH);
    delay(timer);               
    digitalWrite(ledPin, LOW);
  }

  for (int ledPin = 6; ledPin >= 2; ledPin--) { 
    digitalWrite(ledPin, HIGH); 
    delay(timer);               
    digitalWrite(ledPin, LOW);  
  } 
}
```
