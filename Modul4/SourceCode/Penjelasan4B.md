```cpp
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
```

Untuk menentukan agar motor servo hanya bergerak dalam rentang terbatas 30° hingga 150°, bagian yang dimodifikasi bukanlah pada struktur if, melainkan pada parameter fungsi map().

Berikut adalah penjelasan bagian kodenya:

Modifikasi pada Fungsi Scaling (map)
Dalam aturan pemrograman Arduino, fungsi map() digunakan untuk menyelaraskan rentang nilai input ke rentang nilai output yang diinginkan. Untuk membatasi gerak servo, maka bisa dengan mengubah parameter tujuan pada fungsi tersebut menjadi:

```cpp
// Sebelum: pos = map(val, 0, 1023, 0, 180);
pos = map(val, 0, 1023, 30, 150);
```

(nilai_input, min_input, max_input, min_output, max_output). Dengan mengganti angka 0 menjadi 30 dan 180 menjadi 150, kita secara otomatis memerintahkan sistem untuk "memadatkan" seluruh jangkauan putaran potensiometer hanya ke dalam ruang gerak 120 derajat saja.

Meskipun potensiometer diputar maksimal dari ujung ke ujung (ADC 0–1023), secara matematis sistem akan menghitung ulang nilainya agar titik terendah (0) menghasilkan sudut 30° dan titik tertinggi (1023) menghasilkan sudut 150°. Hal ini memastikan pergerakan servo tetap halus dan proporsional terhadap putaran sensor.
