#include <Arduino.h>

// --- Pengaturan Pin ---
const int potPin = A0;   // Input dari potensiometer
const int ledPin = 9;    // Output ke LED (pin PWM)

// --- Variabel Simpan Data ---
int nilaiADC = 0;  // Untuk menyimpan angka dari sensor (0-1023)
int pwm = 0;       // Untuk menyimpan hasil konversi (0-255)

void setup() {
  pinMode(ledPin, OUTPUT);  // Set LED sebagai output
  Serial.begin(9600);       // Mulai komunikasi ke laptop
}

void loop() {
  // 1. Ambil data mentah dari potensiometer
  nilaiADC = analogRead(potPin); 

  // 2. Ubah skala 0-1023 jadi 0-255 agar cocok untuk PWM
  pwm = map(nilaiADC, 0, 1023, 0, 255); 

  // 3. Logika filter: Hanya menyala jika di rentang 50 sampai 200
  if (pwm >= 50 && pwm <= 200) {
    // Kalau masuk rentang, diberi tenaga ke LED sesuai putaran
    analogWrite(ledPin, pwm); 
  } 
  else {
    // Kalau di bawah 50 atau di atas 200, memaksa LED mati
    analogWrite(ledPin, 0); 
  }

  // 4. Pantau angka di Serial Monitor
  Serial.print("ADC: ");
  Serial.print(nilaiADC);
  Serial.print(" | PWM: ");
  Serial.println(pwm);

  delay(50); // memberi jeda
}
