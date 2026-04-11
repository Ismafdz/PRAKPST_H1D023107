# Modul 2 Pemrograman GPIO

## Jawaban Pertanyaan

### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
(Gambar Wiring Diagram Seven Segment)

![Wiring Diagram Seven Segment](https://github.com/user-attachments/assets/7a24bf71-94c9-4f8f-9633-66f72969f86a)


Rangkaian terdiri dari satu unit Seven Segment Display yang disusun pada breadboard dan dihubungkan ke unit mikrokontroler Arduino Uno. Setiap segmen (a-g dan dp) dilengkapi dengan resistor pembatas arus 220Ohm untuk menjaga keamanan komponen sesuai standar yang ditetapkan dalam modul. Berbeda dengan rangkaian standar, pada percobaan ini digunakan konfigurasi Common Anode, di mana jalur common (pin tengah) dihubungkan ke pin 5V Arduino.
Berikut adalah detail koneksi yang diimplementasikan pada rangkaian tersebut:
| Komponen     | Pin Arduino  | Keterangan                                      |
|--------------|-------------|-------------------------------------------------|
| Segmen a     | Pin Digital 7  | Terhubung melalui Resistor 220 Ohm             |
| Segmen b     | Pin Digital 6  | Terhubung melalui Resistor 220 Ohm             |
| Segmen c     | Pin Digital 5  | Terhubung melalui Resistor 220 Ohm             |
| Segmen d     | Pin Digital 11 | Terhubung melalui Resistor 220 Ohm             |
| Segmen e     | Pin Digital 10 | Terhubung melalui Resistor 220 Ohm             |
| Segmen f     | Pin Digital 8  | Terhubung melalui Resistor 220 Ohm             |
| Segmen g     | Pin Digital 9  | Terhubung melalui Resistor 220 Ohm             |
| Segmen dp    | Pin Digital 4  | Terhubung melalui Resistor 220 Ohm             |
| Common Rail  | 5V            | Semua Anoda LED terhubung ke sumber tegangan 5V |

Rangkaian ini telah menggunakan resistor 220 Ohm pada setiap jalur segmen. Hal ini berfungsi sebagai pembatas arus (current limiter) agar LED tidak rusak akibat tegangan berlebih dari pin digital Arduino. Penggunaan konfigurasi Common Anode ini mengharuskan logika program menjadi Active Low, di mana segmen akan menyala ketika pin digital memberikan logika LOW (0V) sehingga terjadi perbedaan potensial dengan sumber 5V pada common rail.

### 2. Apa yang terjadi jika nilai num lebih dari 15?
Dalam program ini, kita membuat daftar "resep" pola angka (0 sampai F) di dalam variabel digitPattern yang ukurannya hanya untuk 16 data. Artinya, komputer cuma menyediakan kursi dari nomor 0 sampai 15.Jika kita memasukkan nilai num lebih dari 15 (misalnya 16, 17, dst), program akan mengalami kondisi yang disebut index out of bounds. Sederhananya, program mencoba mengambil data di "kursi" yang sebenarnya tidak ada atau milik variabel lain. Karena data yang diambil adalah data acak atau "sampah memori", maka tampilan pada Seven Segment akan menjadi ngaco (eror), muncul garis-garis tidak jelas (glitch), atau bahkan lampunya mati total.Itulah alasan kenapa kita menggunakan perulangan `for(int i=0; i<16; i++)` agar urutan angkanya selalu terkontrol dan tidak melompat keluar dari daftar yang sudah kita buat.

### 3. Apakah program ini menggunakan common cathode atau common anode? Jelaskan alasanya!
Berdasarkan rangkaian yang telah dibuat, sistem ini menggunakan jenis Common Anode. Hal ini dapat dibuktikan melalui dua alasan teknis berikut:

Koneksi Fisik (Hardware): Jalur common (pin tengah) pada seven segment dihubungkan langsung ke pin 5V Arduino, bukan ke GND. Ini artinya semua anoda (kutub positif) LED di dalam seven segment digabungkan ke satu sumber tegangan.

Logika Program (Software): Karena common-nya sudah mendapat tegangan 5V, maka untuk menyalakan lampu, pin digital Arduino harus mengeluarkan logika LOW (0V) agar terjadi perbedaan potensial. Logika "nyala saat LOW" ini adalah ciri khas dari konfigurasi Common Anode.

Sederhananya, jika pada common cathode kita memberikan tegangan untuk menyalakan lampu, pada Common Anode kita justru "menarik" tegangan tersebut ke arah 0V agar arus bisa mengalir dan LED menyala.

### 4. Modifikasi program agar tampilan berjalan dari F ke 0 dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
Untuk mengubah urutan tampilan dari maju (0-F) menjadi mundur (F-0),maka perlu mengubah logika perulangan pada fungsi `loop()` . Jika sebelumnya kita menggunakan increment `(i++)` , maka sekarang kita menggunakan decrement `(i--)` dengan titik awal dari indeks tertinggi.
Kode Program Modifikasi:
```cpp
#include <Arduino.h>

// 7-Segment Display (Efficient Version)
// Modifikasi Tampilan: F - 0 (Mundur)

// Pemetaan pin Arduino ke segmen a, b, c, d, e, f, g, dp
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4}; 

// Pola karakter untuk 0-F (Logika Common Anode)
// 0 = ON, 1 = OFF (Karena common rail ke 5V)
byte digitPattern[16][8] = {
  {0,0,0,0,0,0,1,1}, // 0
  {1,0,0,1,1,1,1,1}, // 1
  {0,0,1,0,0,1,0,1}, // 2
  {0,0,0,0,1,1,0,1}, // 3
  {1,0,0,1,1,0,0,1}, // 4
  {0,1,0,0,1,0,0,1}, // 5
  {0,1,0,0,0,0,0,1}, // 6
  {0,0,0,1,1,1,1,1}, // 7
  {0,0,0,0,0,0,0,1}, // 8
  {0,0,0,0,1,0,0,1}, // 9
  {0,0,0,1,0,0,0,1}, // A
  {1,1,0,0,0,0,0,1}, // b
  {0,1,1,0,0,0,1,1}, // C
  {1,0,0,0,0,1,0,1}, // d
  {0,1,1,0,0,0,0,1}, // E
  {0,1,1,1,0,0,0,1}  // F
};

// Fungsi untuk mengirimkan data ke pin Seven Segment
void displayDigit(int num) {
  for(int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT
  for(int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
    // Matikan semua segmen di awal (HIGH untuk Common Anode)
    digitalWrite(segmentPins[i], HIGH);
  }
}

void loop() {
  // PERULANGAN MODIFIKASI: Berjalan mundur dari indeks 15 (F) ke 0 (0)
  for(int i = 15; i >= 0; i--) {
    displayDigit(i); // Tampilkan karakter berdasarkan indeks i
    delay(1000);     // Jeda 1 detik setiap angka
  }
}
```
Penjelasan:
```cpp
for(int i = 15; i >= 0; i--)
```
Baris ini mengatur alur perulangan.
```cpp
int i = 15
```
Inisialisasi variabel i dimulai dari 15, yang merupakan indeks untuk karakter 'F' pada array digitPattern.
```cpp
i >= 0:
```
Kondisi ini memastikan perulangan terus berjalan selama nilai i belum melewati angka 0.
```cpp
i--
```
Operator decrement yang berfungsi mengurangi nilai i sebesar 1 di setiap akhir putaran.
```cpp
displayDigit(i)
```
Fungsi ini mengirimkan pola bit dari array ke pin-pin Seven Segment sesuai dengan nilai i saat itu.
```cpp
delay(1000)
```
Menghentikan eksekusi program sementara selama 1000 milidetik (1 detik) sebelum berpindah ke angka berikutnya.
