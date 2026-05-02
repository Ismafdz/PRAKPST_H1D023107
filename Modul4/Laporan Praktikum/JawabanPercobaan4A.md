# Modul 4 ADC PWM

## Jawaban Pertanyaan Percobaan 4A : Analog To Digital Converter (ADC)

### 1. Apa fungsi perintah analogRead() pada rangkaian praktikum ini?

Perintah `analogRead()` berfungsi untuk membaca sinyal tegangan analog yang masuk melalui pin input analog (dalam hal ini pin yang terhubung ke potensiometer) dan mengubahnya menjadi nilai digital yang dapat diproses oleh mikrokontroler. Karena Arduino Uno memiliki ADC dengan resolusi 10 bit, fungsi ini akan mengonversi rentang tegangan 0 hingga 5 Volt menjadi angka digital dalam rentang 0 hingga 1023.

Dalam konteks rangkaian praktikum ini, `analogRead()` digunakan untuk menangkap perubahan posisi atau putaran poros potensiometer. Ketika potensiometer diputar, tegangan yang dikirim ke pin analog akan berubah-ubah; nilai inilah yang dibaca oleh `analogRead()` . Nilai mentah (0–1023) tersebut kemudian menjadi basis data bagi mikrokontroler untuk menentukan seberapa besar sudut yang harus dibentuk oleh motor servo setelah melalui proses konversi menggunakan fungsi `map()` . Tanpa perintah ini, mikrokontroler tidak akan bisa menerima input variabel dari dunia luar untuk mengontrol perangkat output secara dinamis.

### 2. Mengapa diperlukan fungsi map() dalam program tersebut?
Fungsi map() sangat diperlukan dalam program tersebut karena adanya perbedaan rentang nilai (skala) antara input yang dibaca dari sensor dan output yang dibutuhkan oleh perangkat target.
Alasannya yaitu:
- Penyelarasan Skala Data: Perintah analogRead() menghasilkan nilai digital dengan rentang 0 hingga 1023 (berdasarkan resolusi ADC 10-bit). Di sisi lain, motor servo hanya dapat menerima instruksi posisi sudut dalam rentang 0 hingga 180 derajat. Tanpa fungsi map(), jika langsung memasukkan nilai ADC ke servo, nilai tersebut akan melebihi batas maksimal yang bisa diterima oleh library servo, sehingga gerakan servo menjadi tidak akurat atau bahkan tidak bergerak sama sekali.

- Konversi Matematis Otomatis: Fungsi ini bekerja dengan melakukan perhitungan matematis untuk memetakan satu rentang nilai ke rentang nilai lainnya secara proporsional. Dalam praktikum ini, map(val, 0, 1023, 0, 180) secara otomatis menghitung setiap perubahan nilai pada potensiometer agar sebanding dengan perubahan sudut pada servo. Misalnya, jika potensiometer berada di tengah (nilai ADC 511), maka fungsi map() akan menghasilkan nilai 90, yang merupakan titik tengah dari sudut servo.

- Kemudahan Pemrograman: Tanpa fungsi map(), pemrogram harus membuat rumus matematikanya sendiri (seperti: (nilai_adc / 1023.0) * 180). Fungsi map() menyederhanakan proses ini sehingga kode menjadi lebih bersih, mudah dibaca, dan meminimalisir kesalahan logika dalam perhitungan konversi data sensor ke parameter kontrol perangkat output.

### 3. Modifikasi program berikut agar servo hanya bergerak dalam rentang 30° hingga 150°, meskipun potensiometer tetap memiliki rentang ADC 0–1023. Jelaskan program pada file README.md
```cpp
#include <Servo.h> // Memasukkan library untuk mengontrol servo motor

Servo myservo; // Membuat objek servo

// ===================== PIN SETUP =====================
const int potensioPin = A0; // Pin analog A0 untuk pembacaan potensiometer
const int servoPin = 9;     // Pin digital 9 (PWM) untuk output ke servo

// ===================== VARIABEL =====================
int pos = 0; // Variabel untuk menyimpan hasil konversi sudut (0-180)[cite: 1]
int val = 0; // Variabel untuk menyimpan nilai mentah ADC (0-1023)[cite: 1]

void setup() {
  // Menghubungkan objek servo ke pin yang ditentukan (pin 9)[cite: 1]
  myservo.attach(servoPin); 

  // Mengaktifkan komunikasi serial dengan baud rate 9600[cite: 1]
  Serial.begin(9600); 
}

void loop() {
  // ===================== PEMBACAAN ADC =====================
  // Membaca tegangan dari potensiometer (hasilnya angka 0-1023)[cite: 1]
  val = analogRead(potensioPin); 

  // ===================== KONVERSI DATA =====================
  // MODIFIKASI: Memetakan nilai ADC (0-1023) ke rentang sudut baru (30-150)[cite: 1]
  // Format: map(nilai, dariRendah, dariTinggi, keRendah, keTinggi)[cite: 1]
  pos = map(val, 0, 1023, 30, 150); 

  // ===================== OUTPUT SERVO =====================
  // Menggerakkan poros servo ke sudut yang sudah dibatasi (30-150 derajat)[cite: 1]
  myservo.write(pos); 

  // ===================== MONITORING DATA =====================
  // Menampilkan data pembacaan dan hasil konversi ke Serial Monitor[cite: 1]
  Serial.print("ADC Potensio: ");
  Serial.print(val); 

  Serial.print(" | Sudut Servo: ");
  Serial.println(pos); 

  // ===================== STABILISASI =====================
  // Jeda waktu agar gerakan mekanis servo lebih stabil dan tidak bergetar[cite: 1]
  delay(15); 
}
```
