![Wiring Diagram](https://github.com/user-attachments/assets/c839112a-c880-45f6-b5d8-9deb636c982d)

https://github.com/user-attachments/assets/e8d9cc06-c1e0-49b5-ab0c-23810a9a0236

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
