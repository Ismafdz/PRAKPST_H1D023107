# Modul 4 ADC PWM

## Jawaban Pertanyaan Percobaan 4B : Pulse Width Modulation (PWM)

### 1. Jelaskan mengapa LED dapat diatur kecerahannya menggunakan fungsi analogWrite()!
LED dapat diatur kecerahannya menggunakan fungsi analogWrite() karena fungsi tersebut menerapkan teknik Pulse Width Modulation (PWM) atau Modulasi Lebar Pulsa.

Meskipun pin digital Arduino secara fisik hanya bisa mengeluarkan tegangan 0V (LOW) atau 5V (HIGH), teknik PWM memungkinkan kita untuk mensimulasikan tegangan analog di antara rentang tersebut.
Mikrokontroler sebenarnya tidak bisa mengeluarkan tegangan "setengah-setengah" (misalnya 2,5 Volt). Ia hanya bisa mengeluarkan 0 Volt (mati) atau 5 Volt (nyala penuh). Agar LED tampak redup, Arduino mematikan dan menyalakan LED tersebut ribuan kali dalam satu detik. Karena gerakannya sangat cepat, mata manusia tidak bisa melihat kedipannya; yang kita lihat hanyalah LED yang tampak lebih redup atau lebih terang.
Fungsi analogWrite(pin, nilai) juga mengatur berapa lama lampu harus nyala dibanding lama lampu harus mati dalam satu putaran waktu yang singkat:
- Jika nilai kecil (misal 25): Lampu lebih banyak mati daripada nyala. Hasilnya, LED terlihat sangat redup.
- Jika nilai sedang (misal 127): Lampu nyala dan mati dengan durasi yang sama (setengah-setengah). Hasilnya, LED terlihat remang-remang.
- Jika nilai maksimal (255): Lampu nyala terus tanpa mati. Hasilnya, LED terlihat sangat terang.

Kenapa menggunakan angka 0 - 255? hal ini karena:
- Angka 0 berarti sakelar mati total.  
- Angka 255 berarti sakelar nyala total.

Sehingga, LED tidak benar-benar meredup karena tegangannya turun, tapi karena ia "diperintahkan" untuk menyala dalam durasi waktu yang sangat singkat secara berulang-ulang hingga mata kita tertipu dan menganggapnya redup.

### 2. Apa hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255)?
Hubungan antara nilai ADC (0–1023) dan nilai PWM (0–255) pada dasarnya adalah hubungan skala perbandingan atau proporsionalitas yang memungkinkan input dari sensor dikonversi menjadi output yang sesuai. Hal ini terjadi karena adanya perbedaan resolusi pada mikrokontroler Arduino Uno, di mana ADC memiliki resolusi 10-bit yang membagi rentang tegangan 0–5V menjadi 1024 tingkatan (0 hingga 1023), sedangkan PWM memiliki resolusi 8-bit yang hanya membagi siklus kerja menjadi 256 tingkatan (0 hingga 255).  

Secara matematis, perbedaan rentang ini menciptakan rasio tepat 4 banding 1, karena nilai maksimal ADC (1024) adalah empat kali lipat dari nilai maksimal PWM (256). Artinya, setiap kenaikan 4 angka pada nilai pembacaan ADC akan setara dengan kenaikan 1 angka pada nilai keluaran PWM. Sebagai contoh, jika potensiometer memberikan nilai ADC maksimal 1023, maka setelah dikonversi nilai PWM-nya akan menjadi 255, begitu pula jika ADC berada di titik tengah 511, maka nilai PWM akan berada di kisaran 127.  

Untuk menghubungkan kedua rentang yang berbeda ini di dalam kode program secara praktis, digunakanlah fungsi konversi yang disebut map(). Fungsi ini bekerja dengan cara memetakan nilai dari skala besar ADC ke skala yang lebih kecil pada PWM secara otomatis, sehingga pergerakan sensor tetap sinkron dengan respon perangkat output. Meskipun secara manual kita bisa menggunakan rumus sederhana dengan membagi nilai ADC dengan empat, fungsi map(nilaiADC, 0, 1023, 0, 255) jauh lebih efisien untuk menjaga kebersihan kode dan meminimalisir kesalahan logika.  

Tujuan utama dari terciptanya hubungan ini adalah agar pengguna dapat mengontrol perangkat output, seperti kecerahan LED atau kecepatan motor, secara presisi menggunakan input analog dari dunia nyata. Penyesuaian skala ini sangat krusial karena tanpa adanya sinkronisasi antara input 10-bit dan output 8-bit, nilai ADC yang terlalu besar akan menyebabkan sistem PWM mengalami kebingungan atau kesalahan logika. Hal ini dikarenakan nilai output yang diminta akan melebihi batas maksimal yang mampu dihasilkan oleh pin digital Arduino, sehingga konversi yang tepat memastikan sistem tetap bekerja secara stabil.

### 3. Modifikasilah program berikut agar LED hanya menyala pada rentang kecerahan sedang, yaitu hanya ketika nilai PWM berada pada rentang 50 sampai 200. Jelaskan program pada file README.md.
