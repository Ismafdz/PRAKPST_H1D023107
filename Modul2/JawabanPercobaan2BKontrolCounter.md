# Modul 2 Pemrograman GPIO

## Jawaban Pertanyaan 2B Kontrol Counter dengan Push Button

### 1. Gambarkan rangkaian schematic yang digunakan pada percobaan!
![Wiring Diagram Kontrol Counter](https://github.com/user-attachments/assets/16d48aaa-4182-4c27-b35b-a41814357e7b)

Rangkaian ini merupakan pengembangan dari sistem counter sebelumnya dengan menambahkan sebuah tombol tekan (push button) sebagai pengendali input digital. Sistem ini dirancang untuk membaca sinyal dari tombol guna mengubah tampilan angka pada Seven Segment secara manual melalui logika pemrograman Arduino.
Berikut adalah detail koneksi yang diimplementasikan pada rangkaian tersebut:
| Komponen      | Pin Arduino    | Keterangan                                                     |
|---------------|----------------|----------------------------------------------------------------|
| Push Button   | Pin Digital 3  | Terhubung ke GND (Menggunakan INPUT_PULLUP internal)          |
| Segmen a      | Pin Digital 7  | Jalur data untuk segmen atas                                  |
| Segmen b      | Pin Digital 6  | Jalur data untuk segmen kanan atas                            |
| Segmen c      | Pin Digital 5  | Jalur data untuk segmen kanan bawah                           |
| Segmen d      | Pin Digital 11 | Jalur data untuk segmen bawah                                 |
| Segmen e      | Pin Digital 10 | Jalur data untuk segmen kiri bawah                            |
| Segmen f      | Pin Digital 8  | Jalur data untuk segmen kiri atas                             |
| Segmen g      | Pin Digital 9  | Jalur data untuk segmen tengah                                |
| Segmen dp     | Pin Digital 4  | Jalur data untuk titik desimal                                |
| Common Rail   | GND            | Jalur bersama untuk jenis Common Cathode                      |

Rangkaian ini telah menggunakan resistor 220 $\Omega$ pada setiap jalur segmen sebagai pembatas arus (current limiter) agar LED tidak rusak. Penggunaan konfigurasi Common Cathode ini sepenuhnya sinkron dengan kode program yang menggunakan logika Active High, di mana segmen akan menyala ketika pin digital memberikan logika HIGH (5V). Penambahan tombol pada Pin 2 berfungsi sebagai pemicu untuk menambah nilai variabel currentDigit setiap kali terjadi transisi sinyal dari HIGH ke LOW (falling edge).

### 2. Mengapa pada push button digunakan mode INPUT_PULLUP pada Arduino Uno?  Apa keuntungannya dibandingkan rangkaian biasa?
Penggunaan mode INPUT_PULLUP pada Arduino Uno bertujuan untuk mengaktifkan resistor internal (internal resistor) yang sudah tertanam di dalam chip mikrokontroler. Dalam mode ini, Pin 2 yang dihubungkan ke tombol akan ditarik ke tegangan tinggi (logic high) secara otomatis oleh sistem saat tombol tidak ditekan.

Keuntungannya dibandingkan rangkaian biasa:

* Efisiensi Komponen (Component Efficiency): Kita tidak perlu memasang resistor fisik tambahan (external resistor) pada papan percobaan (breadboard). Hal ini membuat rangkaian menjadi lebih ringkas dan meminimalisir kesalahan perakitan.

* Menghindari Sinyal Mengambang (Floating Signal): Tanpa mode ini, pin input bisa berada dalam kondisi tidak pasti yang sensitif terhadap gangguan listrik statis di sekitar. INPUT_PULLUP memastikan status pin selalu stabil di posisi HIGH saat tombol dilepas.

* Penyederhanaan Jalur Kabel (Wiring Simplification): Kita hanya perlu menghubungkan satu kaki tombol ke pin Arduino dan kaki lainnya langsung ke jalur negatif (ground). Saat tombol ditekan, arus akan mengalir ke ground dan sistem membaca sinyal LOW, yang menjadi penanda jelas bagi program untuk mengeksekusi perintah.

### 3. Jika salah satu LED segmen tidak menyala, apa saja kemungkinan penyebabnya dari sisi hardware maupun software?
Jika terjadi kendala di mana salah satu bagian lampu (segment) pada Seven Segment tidak menyala, terdapat beberapa faktor penyebab yang dapat diidentifikasi baik dari segi fisik maupun kode program:

**Sisi Perangkat Keras (Hardware):**
- Kabel Penghubung (Jumper Wire) Longgar atau Putus:  
  Kabel yang menghubungkan pin digital Arduino ke kaki segmen tersebut tidak tertancap dengan sempurna pada papan percobaan (*breadboard*).

- Kerusakan Komponen LED:  
  Salah satu dioda pemancar cahaya (LED) di dalam unit Seven Segment sudah mengalami kerusakan atau terbakar akibat penggunaan sebelumnya.

- Masalah pada Hambatan (Resistor):  
  Resistor pembatas arus yang terhubung ke segmen tersebut mungkin mengalami kegagalan fungsi atau tidak terpasang di jalur yang benar.

- Kesalahan Jalur Pin (Wiring Error):  
  Kabel jumper tertancap pada lubang *breadboard* yang salah, sehingga tidak terhubung ke pin Arduino yang seharusnya mengontrol segmen tersebut.

**Sisi Perangkat Lunak (Software):**

- Kesalahan Data pada Matriks Pola (Array Index Error):  
  Terdapat kekeliruan dalam penulisan angka `0` atau `1` pada variabel `digitPattern`, sehingga pin tersebut tidak pernah mendapat perintah untuk menyala pada angka tertentu.

- Pin Belum Dikonfigurasi Sebagai Output:  
  Pin yang bersangkutan lupa didaftarkan di dalam fungsi `setup()` menggunakan perintah `pinMode()`, sehingga Arduino tidak bisa mengirimkan tegangan ke pin tersebut.

- Ketidaksesuaian Definisi Pin (Pin Mapping Mismatch):  
  Nomor pin yang didefinisikan pada variabel `segmentPins` di dalam kode program tidak sesuai dengan posisi fisik kabel yang dicolokkan ke papan Arduino.

### 4. Modifikasi rangkaian dan program dengan dua push button yang berfungsi sebagai penambahan (increment) dan pengurangan (decrement) pada sistem counter dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
Berikut adalah rangkaian yang sudah dimodifikasi:
![Modifikasi incremenrt decrement](https://github.com/user-attachments/assets/a49e3669-1b28-4535-9ec9-4e8011a3955d)

ini adalah tabel koneksinya:

| Komponen        | Pin Arduino       | Fungsi                                      |
|-----------------|------------------|---------------------------------------------|
| Push Button 1   | Pin Digital 2    | Penambahan angka (*Increment*)              |
| Push Button 2   | Pin Digital 3    | Pengurangan angka (*Decrement*)             |
| Segmen a – dp   | Pin Digital 4–11 | Output tampilan karakter 0 – F              |
| Common Pin      | GND              | Jalur negatif bersama (*Common Cathode*)    |

Tombol Tambah (Up): Tetap terhubung ke Pin Digital 2 dan GND.

Tombol Kurang (Down): Dihubungkan ke Pin Digital 3 dan GND.

Seven Segment: Tetap menggunakan konfigurasi, Common Cathode yang terhubung ke GND agar sesuai dengan logika kode program.

**Kode Program hasil Modifikasi:**
```cpp
// Definisi Pin Seven Segment (a, b, c, d, e, f, g, dp)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Definisi Pin untuk Tombol
const int btnUp = 2;   // Tombol untuk menambah angka
const int btnDown = 3; // Tombol untuk mengurangi angka

// Pola Karakter 0-F (1 = ON, 0 = OFF untuk Common Cathode)
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, {0,1,1,0,0,0,0,0}, {1,1,0,1,1,0,1,0}, {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0}, {1,0,1,1,0,1,1,0}, {1,0,1,1,1,1,1,0}, {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0}, {1,1,1,1,0,1,1,0}, {1,1,1,0,1,1,1,0}, {0,0,1,1,1,1,1,0},
  {1,0,0,1,1,1,0,0}, {0,1,1,1,1,0,1,0}, {1,0,0,1,1,1,1,0}, {1,0,0,0,1,1,1,0}
};

int currentDigit = 0;      // Menyimpan nilai angka saat ini
bool lastUpState = HIGH;   // Menyimpan status terakhir tombol Up
bool lastDownState = HIGH; // Menyimpan status terakhir tombol Down

void displayDigit(int num) {
  for(int i=0; i<8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT
  for(int i=0; i<8; i++) pinMode(segmentPins[i], OUTPUT);
  
  // Mengatur pin tombol sebagai INPUT dengan resistor internal
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  
  // Menampilkan angka awal (0)
  displayDigit(currentDigit);
}

void loop() {
  // Membaca status tombol saat ini
  bool upState = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Logika Tombol Tambah (Up)
  if(lastUpState == HIGH && upState == LOW) {
    currentDigit++;
    if(currentDigit > 15) currentDigit = 0; // Kembali ke 0 jika lebih dari F
    displayDigit(currentDigit);
    delay(50); // Debouncing sederhana
  }
  lastUpState = upState;

  // Logika Tombol Kurang (Down)
  if(lastDownState == HIGH && downState == LOW) {
    currentDigit--;
    if(currentDigit < 0) currentDigit = 15; // Kembali ke F jika di bawah 0
    displayDigit(currentDigit);
    delay(50); // Debouncing sederhana
  }
  lastDownState = downState;
}
```

**Penjelasan Code:**
```cpp
pinMode(btnDown, INPUT_PULLUP);
```
Mengonfigurasi Pin 3 sebagai masukan dengan mengaktifkan resistor penarik (pull-up) internal.
```cpp
if(lastDownState == HIGH && downState == LOW)
```
Melakukan deteksi tepi jatuh (falling edge detection) untuk memastikan angka hanya berkurang satu kali setiap tombol ditekan.
```cpp
if(currentDigit < 0) currentDigit = 15;
```
Logika perlindungan agar variabel tidak bernilai negatif; jika angka di bawah 0, maka tampilan akan melompat ke karakter tertinggi (F).
```cpp
lastDownState = downState;
```
Memperbarui status tombol untuk digunakan pada putaran perulangan (loop) berikutnya.
