# Praktikum Sistem Tertanam - Modul 1 Percabangan

## Jawaban Pertanyaan Praktikum

### 1. Pada kondisi apa program masuk ke blok if?

Program masuk ke blok `if` ketika nilai:

```cpp
timeDelay <= 100
```
pernyataan di dalam blok tersebut hanya akan diproses jika kondisi yang diseleksi terpenuhi atau bernilai benar (true) yaitu nilai `timeDelay` (yang terus dikurangi 100 di blok `else` pada setiap siklus) telah mencapai angka 100 atau di bawahnya, sistem akan menjalankan aksi di dalam blok `if`, yaitu mereset `timeDelay` kembali ke 1000 dan memberikan jeda selama 3000 ms.

### 2. Pada kondisi apa program masuk ke blok else?

Program masuk ke blok `else` ketika nilai:

```cpp
timeDelay > 100
```
Dalam struktur kontrol `if-else`, blok `else` berfungsi sebagai jalur alternatif yang hanya dieksekusi jika syarat utama dalam `if` tidak terpenuhi. Selama berada di blok `else`, program akan terus menjalankan perintah ```timeDelay -= 100;``` Hal ini mengakibatkan durasi jeda kedipan LED semakin singkat (cepat) pada setiap siklusnya, hingga akhirnya nilai tersebut mencapai titik di mana kondisi `if` menjadi benar.

### 3. Apa fungsi dari perintah delay(timeDelay)?

Perintah berfungsi untuk menginstruksikan mikrokontroler agar menghentikan (menunda) eksekusi program selama durasi waktu tertentu yang ditentukan oleh variabel `timeDelay` dalam satuan milidetik.
```cpp
digitalWrite(ledPin, HIGH); 
delay(timeDelay);           
digitalWrite(ledPin, LOW); 
delay(timeDelay);
```
Dalam percobaan ini, `delay` digunakan untuk mempertahankan status logika pin (HIGH atau LOW) dalam jangka waktu tertentu, sehingga mata manusia dapat menangkap fenomena LED yang menyala atau mati. Karena nilai di dalam kurung menggunakan variabel  `timeDelay`, durasi jeda ini dapat berubah secara dinamis sesuai dengan logika percabangan yang menghasilkan efek kedipan yang semakin cepat atau lambat.

### 4. Jika program yang dibuat memiliki alur mati → lambat → cepat → reset (mati), ubah menjadi LED tidak langsung reset → tetapi berubah dari cepat → sedang → mati dan berikan penjelasan disetiap baris kode nya dalam bentuk README.md!

Kode Program Modifikasi:
```cpp
const int ledPin = 6;     // Menentukan pin 6 sebagai output LED 
int timeDelay = 100;      // Mulai dari delay cepat (100ms) 
bool arahMelambat = true; // Flag untuk mengatur transisi dari cepat ke sedang

void setup() {
  pinMode(ledPin, OUTPUT); // Mengatur ledPin sebagai OUTPUT
}

void loop() {
  // 1. Logika Menyalakan dan Mematikan LED
  digitalWrite(ledPin, HIGH); // LED Menyala 
  delay(timeDelay);           // Jeda sesuai nilai timeDelay saat ini 
  digitalWrite(ledPin, LOW);  // LED Mati 
  delay(timeDelay);           // Jeda sesuai nilai timeDelay saat ini 

  // 2. Logika Percabangan untuk Perubahan Fase (Cepat -> Sedang -> Mati)
  if (arahMelambat) {
    timeDelay += 450;         // Menambah delay agar ritme melambat (100 -> 550 -> 1000)
    
    if (timeDelay > 1000) {   // Jika sudah mencapai fase sangat lambat/mati
      timeDelay = 1000;       // Mengunci nilai delay di titik terlambat
      arahMelambat = false;   // Menghentikan penambahan delay
    }
  } else {
    // Fase Akhir: LED tetap mati atau berhenti (Sistem Berhenti/Standby)
    digitalWrite(ledPin, LOW); 
    while(true);              // Menghentikan program (simulasi fase "Mati" permanen)
  }
}
```

```cpp
int timeDelay = 100;
```
Inisialisasi variabel dimulai dari angka kecil agar LED berkedip dengan frekuensi tinggi (Fase Cepat).

```cpp
timeDelay += 450;
```
Menggunakan operator penugasan penjumlahan untuk meningkatkan nilai delay secara bertahap, tujuannya mengubah ritme kedipan dari cepat menjadi menengah (Fase Sedang).

```cpp
if (timeDelay > 1000)
```
Struktur kontrol `if` digunakan untuk mendeteksi kapan nilai delay telah mencapai ambang batas maksimal (paling lambat).

```cpp
arahMelambat = aflse;
```
Mengubah nilai boolean sebagai penanda bahwa perubahan kecepatan telah selesai.

```cpp
while(true);
```
Perintah perulangan `while` dengan kondisi yang selalu benar akan menciptakan "loop tak terhingga" yang kosong. Sehingga sapat membuat mikrokontroler berhenti mengeksekusi instruksi baru, sehingga LED tetap dalam kondisi terakhir (Fase Mati/Selesai).
