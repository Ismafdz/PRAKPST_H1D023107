```cpp
#include <Arduino.h> // Memasukkan pustaka inti Arduino untuk mengenali fungsi standar.

const int PIN_LED = 12; // Mendefinisikan nomor pin digital yang terhubung ke LED.
char modeAktif = '0';   // Variabel "state" untuk menyimpan perintah mode terakhir yang dikirim pengguna.

void setup() {
  Serial.begin(9600); // Membuka jalur komunikasi serial dengan kecepatan 9600 bit per detik (bps).
  pinMode(PIN_LED, OUTPUT); // Mengonfigurasi Pin 12 sebagai output untuk menyalakan/mematikan LED.
  Serial.println("Kontrol LED: 1=ON, 0=OFF, 2=BLINK"); // Menampilkan menu instruksi di Serial Monitor.
}

void loop() {
  // 1. Mengecek apakah ada data perintah masuk dari Serial Monitor komputer.
  if (Serial.available() > 0) { 
    char dataMasuk = Serial.read(); // Membaca satu karakter bita data dari buffer serial.
    
    // Menyaring karakter 'Enter' (\n atau \r) agar tidak merusak perubahan status mode.
    if (dataMasuk != '\n' && dataMasuk != '\r') { 
      modeAktif = dataMasuk; // Memperbarui status mode berdasarkan input keyboard terbaru.
      
      // Mengirim konfirmasi balik ke layar komputer sesuai mode yang dipilih.
      if (modeAktif == '1') Serial.println("Mode: LED ON"); // Konfirmasi LED menyala terus.
      else if (modeAktif == '0') Serial.println("Mode: LED OFF"); // Konfirmasi LED mati total.
      else if (modeAktif == '2') Serial.println("Mode: LED BLINKING"); // Konfirmasi masuk ke mode berkedip.
      else Serial.println("Perintah tidak dikenal"); // Pesan jika input selain 0, 1, atau 2.
    }
  }

  // 2. Menjalankan tugas secara kontinu berdasarkan isi variabel modeAktif saat ini.
  if (modeAktif == '1') {
    digitalWrite(PIN_LED, HIGH); // Memberikan tegangan 5V pada pin agar LED tetap menyala.
  } 
  else if (modeAktif == '0') {
    digitalWrite(PIN_LED, LOW); // Memutus aliran tegangan agar LED mati.
  } 
  else if (modeAktif == '2') {
    // Logika Berkedip (Blink) - LED akan terus bergantian status selama modeAktif masih '2'.
    digitalWrite(PIN_LED, HIGH); // Menyalakan LED (memberi tegangan 5V).
    delay(500);                  // Menghentikan proses selama 500ms agar kedipan terlihat.
    digitalWrite(PIN_LED, LOW);  // Mematikan LED (memutus tegangan).
    delay(500);                  // Menghentikan proses selama 500ms agar fase mati terlihat.
  }
}
```
