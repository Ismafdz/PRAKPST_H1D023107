# Modul 1 Perulangan

## Jawaban Pertanyaan

### 1. Gambarkan rangkaian schematic 5 LED running yang digunakan pada percobaan!
Rangkaian terdiri dari 5 buah LED yang disusun secara paralel pada breadboard dan dihubungkan ke unit mikrokontroler Arduino Uno. Setiap LED dilengkapi dengan resistor pembatas arus untuk menjaga keamanan komponen sesuai standar yang ditetapkan dalam modul.

![Wiring Diagram](https://github.com/user-attachments/assets/8e6dc985-5dc1-49aa-bcf0-5488e460f86c)

Berikut adalah detail koneksi yang diimplementasikan pada rangkaian tersebut:
| Komponen | Pin Arduino | Keterangan |
| :--- | :--- | :--- |
| LED 1 (Merah) | Pin Digital 2 | Terhubung melalui Resistor 220 Ohm |
| LED 2 (Kuning) | Pin Digital 3 | Terhubung melalui Resistor 220 Ohm |
| LED 3 (Hijau) | Pin Digital 4 | Terhubung melalui Resistor 220 Ohm |
| LED 4 (Oranye) | Pin Digital 5 | Terhubung melalui Resistor 220 Ohm |
| LED 5 (Biru) | Pin Digital 6 | Terhubung melalui Resistor 220 Ohm |
| Common Rail | GND | Semua Katoda LED terhubung ke pin Ground |

Rangkaian ini telah menggunakan resistor 220 Ohm pada setiap jalur anoda LED. Hal ini berfungsi sebagai pembatas arus (current limiter) agar LED tidak rusak akibat tegangan berlebih dari pin digital Arduino.
Konfigurasi pin 2 hingga 6 ini sepenuhnya sinkron dengan struktur perulangan `for (int ledPin = 2; ledPin < 8; ledPin++)` yang diberikan oleh asisten praktikum.

### 2. Jelaskan bagaimana program membuat efek LED berjalan dari kiri ke kanan!
Efek LED berjalan dari kiri ke kanan (maju) dihasilkan melalui pemanfaatan struktur perulangan `for` yang mengiterasi nomor pin secara berurutan dari nilai terendah ke nilai tertinggi.
Program menggunakan variable kontrol `ledPin` yang dimulai dari angka 2. Perulangan akan terus berjalan selama nilai `ledPin` kurang dari 7 (atau 8) yang mencakup seluruh pin LED yang terpasang pada rangkaian.
Mikrokontroler memberikan tegangan tinggi pada pin aktif untuk menyalan LED. Setelah jeda berakhir, pin tersebut diubah menjadi tegangan rendah (LOW) untuk mematikan LED sebelum berpindah ke pin berikutnya

```cpp
delay(timer)
```
berfungsi memberikan jeda waktu agar transisi cahaya dapat tertangkap oleh mata manusia. 
Opetor `ledPin` memastikan bahwa pada setiap iterasi, target pin bergeser satu posisi ke arah kanan pada breadboard.

### 3. Jelaskan bagaimana program membuat LED kembali dari kanan ke kiri
Efek LED berjalan kembali dari kanan ke kiri dihasilkan dengan menggunakan struktur perulangan `for` yang melakukan hitung mundur (decrement) pada nomor pin digital.
Program menetapkan nilai awal variabel `ledPin` pada angka 7 (atau 6 sesuai batas fisik LED terakhir).

```cpp
ledPin >= 2
```
Perulangan akan terus dieksekusi selama nilai ledPin lebih besar dari atau sama dengan 2.

Mekanisme Dekremen: Operator `--` (decrement) digunakan untuk mengurangi nilai variabel `ledPin` sebesar 1 angka pada setiap akhir iterasi. Hal ini menyebabkan urutan pin yang aktif berpindah dari posisi fisik kanan (Pin tinggi) menuju ke kiri (Pin rendah).
Siklus Nyala-Mati: Sama seperti pada pergerakan maju, setiap pin akan diberikan instruksi HIGH, ditunda sejenak dengan `delay(timer)`, lalu diberikan instruksi LOW sebelum berpindah ke iterasi berikutnya.

### 4. Buatkan program agar LED menyala tiga LED kanan dan tiga LED kiri secara bergantian dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

Kode Program 3 LED kiri dan 3 LED kanan Bergantian
```cpp
// Definisi jeda waktu pergantian
int timer = 500; 

void setup() { 
  // Inisialisasi Pin 2 sampai 7 sebagai OUTPUT menggunakan perulangan for
  for (int ledPin = 2; ledPin < 8; ledPin++) { 
    pinMode(ledPin, OUTPUT);
  } 
} 

void loop() { 
  // FASE 1: MENYALAKAN 3 LED KIRI ---
  // Pin 2, 3, 4 HIGH | Pin 5, 6, 7 LOW
  for (int i = 2; i <= 4; i++) digitalWrite(i, HIGH);
  for (int i = 5; i <= 7; i++) digitalWrite(i, LOW);
  delay(timer); 

  // FASE 2: MENYALAKAN 3 LED KANAN ---
  // Pin 2, 3, 4 LOW | Pin 5, 6, 7 HIGH
  for (int i = 2; i <= 4; i++) digitalWrite(i, LOW);
  for (int i = 5; i <= 7; i++) digitalWrite(i, HIGH);
  delay(timer); 
}
```
Penjelasan Kode:
```cpp
int timer = 500;
```
Menentukan variabel durasi jeda sebesar 500 milidetik agar perpindahan antar kelompok LED terlihat jelas oleh mata.
```cpp
for (int ledPin = 2; ledPin < 8; ledPin++)
```
Menggunakan struktur perulangan `for` di dalam `setup()` untuk mengonfigurasi enam pin digital sekaligus sebagai OUTPUT secara efisien.
```cpp
for (int i = 2; i <= 4; i++) digitalWrite(i, HIGH);
```
Perulangan ini secara khusus menyasar kelompok LED kiri (Pin 2, 3, dan 4) untuk diberikan tegangan tinggi sehingga menyala bersamaan.
```cpp
for (int i = 5; i <= 7; i++) digitalWrite(i, LOW);
```
Memastikan kelompok LED kanan (Pin 5, 6, dan 7) dalam kondisi mati saat kelompok kiri menyala agar tercipta efek bergantian yang kontras.
```cpp
delay(timer);
```
Memberikan instruksi kepada mikrokontroler untuk menunda eksekusi baris kode berikutnya, sehingga status nyala LED tertahan selama durasi variabel `timer`.
```cpp
loop()
```
Mengulangi proses pertukaran kondisi antara kelompok kiri dan kanan secara terus-menerus selama Arduino mendapatkan daya listrik.
