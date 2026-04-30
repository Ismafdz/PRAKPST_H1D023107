# Modul 3 ADC PWM

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
