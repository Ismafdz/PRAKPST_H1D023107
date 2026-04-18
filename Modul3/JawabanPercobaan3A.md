# Modul 3 Protokol Komunikasi

## Jawaban Pertanyaan Percobaan 3A : Komunikasi Serial

### 1. Jelaskan proses dari input keyboard hingga LED menyala/mati!
Proses kendali LED melalui input keyboard diawali ketika pengguna mengetikkan karakter pada Serial Monitor dan menekan Enter, yang menyebabkan data tersebut dikirim dalam bentuk rangkaian bit melalui kabel USB menuju Arduino. Data yang masuk kemudian diterima oleh modul perangkat keras UART (Universal Asynchronous Receiver-Transmitter) dan dikumpulkan di dalam sebuah Buffer, yakni tempat penyimpanan data sementara di memori mikrokontroler agar informasi tidak hilang sebelum diproses. Di dalam kode program, fungsi `Serial.available()` akan secara kontinu mengecek ketersediaan data di dalam buffer tersebut; jika terdeteksi adanya data, maka fungsi `Serial.read()` akan membaca karakter tersebut dan menyimpannya ke dalam sebuah variabel. Selanjutnya, mikrokontroler menjalankan logika percabangan untuk mencocokkan karakter yang diterima; apabila karakter tersebut adalah '1', maka perintah `digitalWrite` akan mengubah status pin digital menjadi HIGH atau mengeluarkan tegangan 5V yang menyebabkan arus mengalir sehingga LED menyala. Sebaliknya, jika karakter '0' yang terbaca, status pin akan diubah menjadi LOW (0V) yang mengakibatkan LED mati. Sebagai penutup alur, Arduino mengirimkan pesan umpan balik melalui perintah `Serial.println` untuk menampilkan status kondisi LED saat itu kembali ke layar monitor komputer.

### 2. Mengapa digunakan `Serial.available()` sebelum membaca data? Apa yang terjadi jika baris tersebut dihilangkan?
Penggunaan fungsi `Serial.available()` sangat krusial dalam komunikasi serial karena fungsi ini bertugas untuk memantau apakah ada data yang telah masuk dan tersimpan di dalam buffer penerimaan Arduino. Secara teknis, fungsi ini akan mengembalikan nilai berupa jumlah bita (bytes) data yang tersedia untuk dibaca. Dengan melakukan pengecekan ini terlebih dahulu, program memastikan bahwa fungsi pembacaan hanya akan dieksekusi ketika data benar-benar sudah siap di memori sementara.

Apabila baris `Serial.available()` dihilangkan, maka fungsi `Serial.read()` akan terus dipaksa berjalan pada setiap perulangan (looping) program tanpa mempedulikan ada atau tidaknya kiriman data dari komputer. Hal ini akan menyebabkan variabel penampung menerima nilai -1 (atau karakter kosong dalam beberapa kasus) secara berulang-ulang karena program mencoba membaca data dari gudang penyimpanan yang sebenarnya sedang kosong. Dampaknya, logika program seperti perintah menyalakan LED bisa terganggu atau tidak responsif karena mikrokontroler sibuk memproses data "kosong" tersebut, yang pada akhirnya dapat memicu perilaku sistem yang tidak stabil atau munculnya pesan kesalahan yang tidak diinginkan pada Serial Monitor.

### 3. Modifikasi program agar LED berkedip (blink) ketika menerima input '2' dengan kondisi jika ‘2’ aktif maka LED akan terus berkedip sampai perintah selanjutnya diberikan dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!
```cpp
#include <Arduino.h>

const int PIN_LED = 12; 
char modeAktif = '0';   // Variabel untuk menyimpan status mode saat ini

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT); 
  Serial.println("Kontrol LED: 1=ON, 0=OFF, 2=BLINK"); 
}

void loop() {
  // 1. mengecek apakah ada kiriman perintah baru dari Serial Monitor
  if (Serial.available() > 0) { 
    char dataMasuk = Serial.read(); 
    
    if (dataMasuk != '\n' && dataMasuk != '\r') { 
      modeAktif = dataMasuk; // Update mode berdasarkan input terbaru
      
      // memberikan umpan balik ke Serial Monitor
      if (modeAktif == '1') Serial.println("Mode: LED ON");
      else if (modeAktif == '0') Serial.println("Mode: LED OFF"); 
      else if (modeAktif == '2') Serial.println("Mode: LED BLINKING");
      else Serial.println("Perintah tidak dikenal");
    }
  }

  // 2. Menjalankan aksi berdasarkan mode yang sedang aktif
  if (modeAktif == '1') {
    digitalWrite(PIN_LED, HIGH); 
  } 
  else if (modeAktif == '0') {
    digitalWrite(PIN_LED, LOW); 
  } 
  else if (modeAktif == '2') {
    // Logika Berkedip (Blink)
    digitalWrite(PIN_LED, HIGH);
    delay(500); // Tunggu 500ms
    digitalWrite(PIN_LED, LOW);
    delay(500); // Tunggu 500ms
  }
}
```

Program memungkinkan pengguna mengontrol LED melalui Serial Monitor dengan tiga instruksi:
Input '1': LED menyala terus-menerus.
Input '0': LED mati.
Input '2': LED masuk ke mode blink (berkedip) secara kontinu hingga menerima perintah lain.

**Penjelasan Kode**
```cpp
char modeAktif = '0';
```
Membuat variabel "status" untuk menyimpan perintah terakhir yang diterima.

```cpp
if (Serial.available() > 0)
```
Memastikan pembacaan hanya dilakukan jika ada data yang benar-benar masuk ke buffer UART.

```cpp
modeAktif = dataMasuk;
```
Ini kunci perubahannya. Tidak langsung mematikan/menyalakan LED di dalam blok ini, melainkan hanya mengubah status modenya saja.

```cpp
if (modeAktif == '2')
```
Jika status saat ini adalah '2', maka Arduino akan menjalankan siklus `digitalWrite` HIGH dan LOW secara berulang di dalam fungsi `loop()` .

```cpp
delay(500)
```
Memberikan jeda waktu agar kedipan LED dapat terlihat oleh mata manusia.

### 4. Tentukan apakah menggunakan delay() atau milis()! Jelaskan pengaruhnya terhadap sistem
**Penggunaan delay() (Metode Blocking)**

Fungsi delay() bekerja dengan cara menghentikan seluruh aktivitas prosesor selama waktu yang ditentukan
Pengaruh terhadap sistem: Selama delay() berjalan, Arduino menjadi "lumpuh" atau tidak bisa melakukan pekerjaan lain, termasuk membaca input baru dari Serial Monitor.
Masalah: Jika menggunakan delay(500), lalu di tengah-tengah waktu tunggu tersebut mengetik angka '0' untuk mematikan LED, Arduino tidak akan langsung merespons. Arduino baru akan membaca perintah setelah waktu delay() selesai. Hal ini membuat sistem terasa lambat atau tidak responsif.

**Penggunaan millis() (Metode Non-Blocking)**

Fungsi millis() memberikan nilai waktu dalam milidetik yang telah berlalu sejak Arduino mulai dinyalakan. Digunakan untuk menghitung selisih waktu tanpa menghentikan program.
Pengaruh terhadap sistem: Program tetap berjalan dengan kecepatan penuh pada setiap perulangan (loop). Arduino bisa terus-menerus mengecek apakah ada data baru di `Serial.available()` sambil secara bersamaan menghitung kapan waktunya LED harus berubah status (nyala ke mati atau sebaliknya).
Keuntungan: Sistem menjadi sangat responsif. Begitu mengirimkan perintah baru via keyboard, Arduino akan langsung membacanya saat itu juga tanpa menunggu siklus kedipan LED selesai.

Sehingga dapat ditarik kesimpulan untuk sistem komunikasi serial yang interaktif seperti pada modul ini, millis() sangat direkomendasikan. Penggunaan delay() hanya cocok untuk program yang sangat sederhana dan tidak membutuhkan reaksi cepat terhadap input mendadak. Dengan millis(), mikrokontroler bisa melakukan tugas majemuk (multitasking) secara semu, yang merupakan praktik terbaik dalam pemrograman sistem tertanam.

