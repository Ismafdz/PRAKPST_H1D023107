# Modul 3 Protokol Komunikasi

## Jawaban Pertanyaan Percobaan 3B: Inter-Integrated Circuit (I2C)

### 1. Jelaskan bagaimana cara kerja komunikasi I2C antara Arduino dan LCD pada rangkaian tersebut!
Secara teknis, proses komunikasi ini mengikuti alur sistem Master-Slave yang berlangsung melalui dua jalur utama:

a. **Inisialisasi Jalur (SDA & SCL)**  
   Komunikasi terjadi melalui dua kabel, yaitu SDA (Serial Data) untuk mengirimkan informasi data, dan SCL (Serial Clock) untuk mengatur detak waktu agar pengiriman data sinkron. Arduino bertindak sebagai Master (supir bus) yang memegang kendali penuh atas kapan data harus dikirimkan.

b. **Pemanggilan Alamat (I2C Address)**  
   Karena jalur I2C bisa menghubungkan banyak perangkat sekaligus, Arduino akan mengirimkan Nomor Kursi atau alamat unik (seperti `0x27` atau `0x3F`) ke seluruh jalur bus. Meskipun semua perangkat di jalur tersebut menerima sinyalnya, hanya LCD yang memiliki alamat tersebut yang akan merespons (menyahut), sementara perangkat lain akan mengabaikannya.

c. **Pengiriman Instruksi dan Data**  
   Setelah koneksi dengan LCD terjalin, Arduino mulai mengirimkan rangkaian instruksi. Dalam rangkaian ini, Arduino mengirimkan data nilai analog dari potensiometer yang telah diproses. Data ini dikirimkan bit demi bit melalui kabel SDA dengan sinkronisasi dari kabel SCL.

d. **Visualisasi pada LCD**  
   Modul I2C Backpack yang menempel pada LCD menerima bit-bit tersebut dan menerjemahkannya kembali menjadi karakter teks atau grafik batang (bar level) yang muncul pada layar. Proses ini terjadi sangat cepat secara berulang sehingga perubahan pada potensiometer bisa terlihat hampir seketika di layar.


### 2. Apakah pin potensiometer harus seperti itu? Jelaskan yang terjadi apabila pin kiri dan pin kanan tertukar!
Secara teknis, kaki tengah (wiper) wajib dihubungkan ke pin analog Arduino (seperti pin A0) karena kaki inilah yang mengambil nilai tegangan variabel untuk diproses oleh ADC (Analog to Digital Converter). Sementara itu, kaki kiri dan kanan berfungsi sebagai penentu kutub tegangan (5V dan GND). Selama kaki tengah tetap berada di posisi yang benar, rangkaian tidak akan mengalami kerusakan atau korsleting.

Jika Pin Kiri dan Kanan Tertukar: kabel 5V dan GND pada kaki kiri dan kanan, sistem akan tetap bekerja, namun dengan logika yang terbalik.

- **Kondisi Normal:**  
  Saat potensiometer diputar searah jarum jam (ke kanan), nilai ADC pada layar LCD akan bertambah dari 0 menuju 1023.

- **Kondisi Tertukar:**  
  Saat potensiometer diputar ke kanan, nilai ADC justru akan mengecil menuju 0. Sebaliknya, saat diputar ke kiri, nilai ADC akan membesar menuju 1023.

Penukaran ini tidak akan merusak Arduino maupun potensiometer. Namun, hal ini akan mengubah pengalaman pengguna (*user experience*) karena arah putaran tidak lagi sesuai dengan standar pembacaan nilai yang biasanya bertambah saat diputar ke arah kanan (searah jarum jam).

### 3. Modifikasi program dengan menggabungkan antara UART dan I2C (keduanya sebagai output) sehingga:

**- Data tidak hanya ditampilkan di LCD tetapi juga di Serial Monitor**

**- Adapun data yang ditampilkan pada Serial Monitor sesuai dengan tabel berikut:**

| ADC | Volt  | Persen |
|-----|-------|--------|
| 0   | 0.00V | 0%     |

**Tampilan jika potensiometer dalam kondisi diputar paling kiri:**

**- `ADC: 0 0%`**

**- `setCursor(0, 0)` dan Bar (level) `setCursor(0, 1)`**

**- Berikan penjelasan di setiap baris kode nya dalam bentuk README.md**

Kode Program Gabungan UART dan I2C
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
<img width="1296" height="648" alt="Wiring Diagram no 3 UART I2C" src="https://github.com/user-attachments/assets/267f0f62-821b-4f9e-9791-b36cbd87023b" />

Secara garis besar, rangkaian ini merupakan sistem kontrol dan pemantauan terintegrasi yang menggabungkan dua protokol komunikasi utama pada Arduino. Protokol I2C digunakan untuk efisiensi pengabelan pada LCD melalui pin A4 (SDA) dan A5 (SCL), di mana layar menampilkan interpretasi data dari Potensiometer yang dibaca melalui pin analog A0 dalam bentuk angka mentah (ADC), persentase, dan grafik batang visual. Sementara itu, protokol UART berperan dalam komunikasi dua arah antara Arduino dan komputer, memungkinkan pengiriman data sensor ke Serial Monitor sekaligus menerima perintah dari keyboard untuk mengontrol status LED pada pin 12, baik untuk menyala statis maupun berkedip. Seluruh komponen didukung oleh distribusi daya 5V dan Ground melalui breadboard, di mana resistor tetap digunakan sebagai pengaman arus untuk mencegah kerusakan pada LED.

### 4. Lengkapi table berikut berdasarkan pengamatan pada Serial Monitor
Untuk mengisi tabel tersebut, menggunakan rumus konversi yang sudah ditanamkan di dalam kode program Arduino sebelumnya. Karena Arduino Uno menggunakan ADC 10-bit, maka rentang nilainya adalah 0 hingga 1023 untuk tegangan 0 hingga *5V*.
Rumus yang Digunakan

- **Volt (V)**:  
  `Volt = ADC × (5.0 / 1023.0)`

- **Persen (%)**:  
  `Persen = (ADC / 1023.0) × 100%`  
  *(dibulatkan sesuai fungsi `map` di Arduino)*

---

## Tabel Hasil Konversi Data

| ADC | Volt (V) | Persen (%) |
|-----|---------|------------|
| 1   | 0.005 V | 0%         |
| 21  | 0.103 V | 2%         |
| 49  | 0.240 V | 5%         |
| 74  | 0.362 V | 7%         |
| 96  | 0.469 V | 9%         |

Penjelasan Singkat Perhitungan
- ADC 1:
Tegangannya sangat kecil (sekitar 5 milivolt), sehingga pada pembulatan persentase di Arduino, nilainya masih dianggap 0%.
- ADC 96:
Nilai ini hampir mencapai 10% dari kapasitas maksimal ADC (1023). Jika potensiometer diputar lebih jauh ke kanan, angka-angka ini akan terus meningkat hingga maksimal *5.00V* dan 100\%.
