#include <Arduino.h> 

// ===================== PIN SETUP =====================
const int potPin = A0;   // Pin analog untuk baca potensiometer
const int ledPin = 9;    // Pin digital PWM untuk kontrol LED

// ===================== VARIABEL =====================
int nilaiADC = 0;  // Tempat simpan data mentah (0-1023)
int pwm = 0;       // Tempat simpan hasil konversi (0-255)[cite: 1]

void setup() {
  // Atur pin LED sebagai output arus[cite: 1]
  pinMode(ledPin, OUTPUT); 

  // Mulai komunikasi ke Serial Monitor (kecepatan 9600)[cite: 1]
  Serial.begin(9600); 
}

void loop() {
  // 1. Baca nilai analog dari potensiometer[cite: 1]
  nilaiADC = analogRead(potPin); 

  // 2. Skalasi: Ubah rentang ADC (0-1023) jadi rentang PWM (0-255)[cite: 1]
  pwm = map(nilaiADC, 0, 1023, 0, 255); 

  // 3. LOGIKA MODIFIKASI: Filter rentang 50 sampai 200[cite: 1]
  if (pwm >= 50 && pwm <= 200) {
    // Kalau masuk rentang sedang, LED nyala sesuai putaran[cite: 1]
    analogWrite(ledPin, pwm); 
  } 
  else {
    // Kalau di luar rentang, paksa LED mati (0)[cite: 1]
    analogWrite(ledPin, 0); 
  }

  // 4. Monitoring data ke layar laptop[cite: 1]
  Serial.print("ADC: ");
  Serial.print(nilaiADC); 

  Serial.print(" | PWM: ");
  Serial.println(pwm); 

  // Jeda dikit supaya pembacaan data stabil[cite: 1]
  delay(50); 
}
