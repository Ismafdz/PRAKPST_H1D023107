```cpp
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
```

Pada kode asli, nilai PWM langsung dikirim ke LED menggunakan analogWrite(ledPin, pwm) tanpa filter. Dalam modifikasi, struktur kontrol if-else untuk memeriksa nilai variabel pwm sebelum dikirim ke pin output.
```cpp
if (pwm >= 50 && pwm <= 200) { 
    // Bagian ini adalah 'filter' untuk rentang sedang
    analogWrite(ledPin, pwm); 
} else {
    // Bagian ini untuk memastikan LED mati di luar rentang
    analogWrite(ledPin, 0); 
}
```

Bagian yang paling krusial dalam modifikasi ini adalah penggunaan operator &&. Operator ini menetapkan aturan bahwa LED hanya akan menyala jika dua syarat terpenuhi secara bersamaan:
- Nilai PWM harus lebih besar atau sama dengan 50.  
- Nilai PWM harus lebih kecil atau sama dengan 200.

Jika nilai PWM adalah 40 (terlalu rendah) atau 210 (terlalu tinggi), maka pernyataan tersebut bernilai salah (false), sehingga kode di dalam if dilewati.

Modifikasi juga terletak pada bagian else. Di sini, memberikan perintah analogWrite(ledPin, 0). Aturan ini sangat penting karena tanpa instruksi else, LED tidak akan padam secara otomatis saat nilai PWM keluar dari rentang 50-200; LED justru akan tetap menyala pada tingkat kecerahan terakhirnya.

**Alur Kerja Program Hasil Modifikasi:**
1. Pembacaan: Arduino membaca nilai potensiometer (0–1023) melalui analogRead().  
2. Konversi: Nilai tersebut diubah menjadi skala PWM (0–255) melalui fungsi map().  
3. Validasi (Modifikasi Utama): Sistem mengecek apakah hasil konversi berada di angka 50 sampai 200.  
4. Eksekusi: Jika masuk rentang, LED menyala sesuai tingkat kecerahan dari potensiometer. Jika tidak masuk rentang, LED dipaksa mati (0).
