```cpp
#include <Adafruit_LiquidCrystal.h> // Memasukkan library LCD Adafruit untuk simulasi Tinkercad 

// Inisialisasi LCD dengan alamat 0 (standar default I2C di Tinkercad) 
Adafruit_LiquidCrystal lcd(0); 

const int pinPot = A0;   // Pin tengah potensiometer terhubung ke Analog 0
const int PIN_LED = 12;  // Pin digital 12 untuk kontrol LED via UART 
char modeAktif = '0';

void setup() {
  Serial.begin(9600);    // Memulai komunikasi serial dengan kecepatan 9600 bps 
  
  lcd.begin(16, 2);      // Menyiapkan LCD berukuran 16 kolom dan 2 baris 
  lcd.setBacklight(1);   // Menyalakan lampu latar LCD agar teks terlihat 

  pinMode(PIN_LED, OUTPUT); // Mengatur pin 12 sebagai output untuk LED 
  
  // Membuat header tabel di Serial Monitor agar tampilan data rapi 
  Serial.println("ADC\tVolt\tPersen"); 
}

void loop() {
  // Membaca nilai tegangan dari potensiometer (rentang 0 - 1023) 
  int nilaiADC = analogRead(pinPot); 
  
  // Mengonversi nilai ADC menjadi satuan Volt (0V - 5V) 
  float tegangan = nilaiADC * (5.0 / 1023.0); 
  
  // Mengonversi nilai ADC menjadi persentase (0% - 100%) 
  int persen = map(nilaiADC, 0, 1023, 0, 100); 
  
  // Mengonversi nilai ADC ke skala panjang bar LCD (0 - 16 karakter) 
  int panjangBar = map(nilaiADC, 0, 1023, 0, 16); 

  // --- OUTPUT KE SERIAL MONITOR (UART) --- 
  Serial.print(nilaiADC);    // Menampilkan angka mentah ADC 
  Serial.print("\t");        // Memberikan spasi tab agar sejajar 
  Serial.print(tegangan);    // Menampilkan nilai voltase 
  Serial.print("V\t");
  Serial.print(persen);      // Menampilkan nilai persen 
  Serial.println("%");

  // --- OUTPUT KE LCD (I2C) ---
  // Menampilkan teks dan persentase di baris pertama (baris 0) 
  lcd.setCursor(0, 0); 
  lcd.print("ADC:");
  lcd.print(nilaiADC);
  lcd.print(" ");
  lcd.print(persen);
  lcd.print("%   "); // Spasi tambahan untuk menghapus sisa angka jika nilai berkurang

  // Menampilkan grafik batang (Bar Level) di baris kedua (baris 1) 
  lcd.setCursor(0, 1); 
  for (int i = 0; i < 16; i++) {
    if (i < panjangBar) {
      lcd.write(0xFF); // Menampilkan balok penuh menggunakan kode ASCII 255 
    } else {
      lcd.print(" ");  // Menampilkan spasi kosong jika level belum mencapai posisi tersebut 
    }
  }

  delay(200); // Memberi jeda 0,2 detik agar tampilan stabil dan tidak flicker 
}
```
