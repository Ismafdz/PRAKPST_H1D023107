# Modul 5 Real-Time Operating System

## Jawaban Pertanyaan Percobaan 5A : Multitasking

### 1. Apakah ketiga task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Ketiga task tersebut sebenarnya berjalan secara bergantian, namun karena proses perpindahannya terjadi sangat cepat, bagi penglihatan manusia seolah-olah ketiganya berjalan secara bersamaan (concurrent).
Mekanisme Kerjanya:
- Single Core Processor: Arduino Uno hanya memiliki satu inti prosesor (single core), sehingga secara fisik ia hanya mampu mengeksekusi satu instruksi atau satu task pada satu waktu.
- Kernel Scheduler: FreeRTOS menggunakan komponen yang disebut Kernel Scheduler untuk mengatur pembagian waktu eksekusi prosesor. Scheduler ini memberikan "slot waktu" kepada setiap task secara bergantian.
- Manajemen State: Running yaitu Task yang sedang memegang kendali prosesor berada dalam state ini.  Blocked yaitu Ketika sebuah task memanggil fungsi vTaskDelay(), task tersebut akan masuk ke Blocked State. Pada saat inilah scheduler secara otomatis memindahkan kendali prosesor ke task lain yang sudah siap (Ready State).  Ready yaitu Task yang sudah menyelesaikan waktu tunggunya akan mengantre di Ready State untuk dijadwalkan kembali oleh prosesor
- Deterministik: Mekanisme ini memastikan bahwa setiap task mendapatkan giliran secara konsisten dan teratur, sehingga sistem tetap responsif dalam memproses data secara langsung tanpa penundaan yang terasa oleh pengguna.
  
Kesimpulannya, ketiga tugas tersebut sebenarnya bekerja secara bergantian dengan sangat cepat di bawah kendali scheduler FreeRTOS, namun karena proses perpindahannya terjadi dalam hitungan milidetik, mereka terlihat seolah-olah berjalan secara bersamaan. Mekanisme ini dimungkinkan karena saat satu tugas sedang beristirahat melalui perintah `vTaskDelay()` , tugas tersebut akan memberikan jalan bagi tugas lainnya untuk menggunakan prosesor, sehingga sistem tetap stabil, efisien, dan responsif dalam menyelesaikan berbagai pekerjaan secara multitasking.

### 2. Bagaimana cara menambahkan task keempat? Jelaskan langkahnya!
Langkah-Langkah Menambahkan Task Keempat:
- Membuat prototipe fungsi untuk task keempat di bagian atas kode (sebelum fungsi setup) agar program mengenali nama fungsi tersebut. contoh: `void TaskBaru(void *pvParameters);`
- Membuat fungsi `xTaskCreate()` di dalam `void setup()` untuk mendaftarkan task keempat ke dalam Kernel Scheduler. setelahnya harus menentukan nama task, alokasi memori (stack size), dan prioritasnya. contoh: `xTaskCreate(TaskBaru, "Task4", 128, NULL, 1, NULL);`
- Membuat fungsi task di bagian bawah kode dengan menggunakan perulangan tak terbatas seperti `while(1)` atau `for(;;)` . Di dalam loop ini, wajib menyertakan fungsi `vTaskDelay()` agar task tersebut dapat berpindah ke Blocked State dan memberikan kesempatan bagi task lain untuk berjalan secara bergantian.

Dengan menambahkan task keempat, beban kerja Kernel Scheduler akan meningkat dalam melakukan context switching atau perpindahan antar tugas. namun total alokasi memori (stack) dari keempat task tersebut tidak melebihi kapasitas RAM Arduino Uno yang terbatas agar sistem tidak mengalami kegagalan atau crash. Secara mekanis, task keempat ini akan mengantre di Ready State dan menunggu giliran dari scheduler untuk masuk ke Running State secara sinkron dengan tiga task sebelumnya.

### 3. Modifikasilah program dengan menambah sensor (misalnya potensiometer), lalu gunakan nilainya untuk mengontrol kecepatan LED! Bagaimana hasilnya? Jelaskan program pada file README.md.
```cpp
#include <Arduino_FreeRTOS.h>

// Deklarasi fungsi Task
void TaskBlinkMerah(void *pvParameters);
void TaskBlinkKuning(void *pvParameters);

void setup() {
  // Inisialisasi Serial Monitor untuk memantau nilai ADC
  Serial.begin(9600);
  
  // Konfigurasi Pin Output
  pinMode(13, OUTPUT); // LED Merah
  pinMode(11, OUTPUT); // LED Kuning
  
  // Pin A0 otomatis menjadi INPUT saat menggunakan analogRead()

  // Membuat Task untuk LED Merah
  xTaskCreate(
    TaskBlinkMerah,   // Nama fungsi task
    "Merah",          // Nama task (untuk debug)
    128,              // Stack size
    NULL,             // Parameter
    1,                // Prioritas
    NULL              // Task handle
  );

  // Membuat Task untuk LED Kuning
  xTaskCreate(
    TaskBlinkKuning, 
    "Kuning", 
    128, 
    NULL, 
    1, 
    NULL
  );
}

void loop() {
  // Loop dibiarkan kosong karena sistem dikelola oleh Scheduler RTOS
}

// Implementasi Task LED Merah (Kecepatan dikontrol Potensiometer)
void TaskBlinkMerah(void *pvParameters) {
  for (;;) {
    // Membaca nilai potensiometer (0 - 1023)
    int sensorValue = analogRead(A0);
    
    // Memberikan batas minimal delay agar LED tidak terlihat menyala terus
    int delayTime = map(sensorValue, 0, 1023, 100, 2000); 

    digitalWrite(13, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS); // Delay dinamis
    digitalWrite(13, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    
    Serial.print("Delay Merah: ");
    Serial.println(delayTime);
  }
}

// Implementasi Task LED Kuning (Kecepatan Tetap)
void TaskBlinkKuning(void *pvParameters) {
  for (;;) {
    digitalWrite(11, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS); // Kedip statis setiap 0.5 detik
    digitalWrite(11, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```

Modifikasi dilakukan pada struktur task dengan mengubah nilai delay yang sebelumnya bersifat statis (angka tetap) menjadi dinamis (berdasarkan variabel). Berikut adalah bagian penting yang dimodifikasi:
- Penambahan Pembacaan Analog: Di dalam TaskBlinkMerah, ditambahkan baris int sensorValue = analogRead(A0); untuk mengambil data dari potensiometer setiap kali siklus task berjalan.
- Penggunaan Fungsi map(): Nilai sensor (0-1023) diubah menjadi rentang waktu milidetik yang proporsional (misalnya 100ms hingga 2000ms) menggunakan fungsi map(). Hal ini bertujuan agar kontrol kecepatan LED lebih terasa halus dan tidak mati terlalu lama.
- Dinamisasi vTaskDelay(): Fungsi vTaskDelay(delayTime / portTICK_PERIOD_MS) sekarang menggunakan variabel hasil pembacaan sensor, bukan lagi angka tetap seperti pada percobaan awal.

<img width="665" height="450" alt="5a 3" src="https://github.com/user-attachments/assets/56d36aaa-5e7a-4deb-a6ea-af34839a1c54" />


Rangkaian ini menggunakan Arduino Uno sebagai pusat kendali dengan tiga komponen input/output utama: LED Merah (Pin 13), LED Kuning (Pin 11), dan Potensiometer (Pin A0). Alur kerjanya adalah sebagai berikut:
- Input: Potensiometer berfungsi sebagai pembagi tegangan variabel. Saat tuas diputar, tegangan yang masuk ke Pin A0 akan berubah (0V hingga 5V), yang kemudian dikonversi oleh ADC Arduino menjadi nilai digital antara 0 sampai 1023.
- Proses: RTOS menjalankan dua tugas secara independen. Tugas pertama membaca nilai potensiometer untuk menentukan durasi jeda (delay), sementara tugas kedua menjalankan ritme kedipan yang konstan.
- Output: Hasil akhirnya adalah LED Merah yang kecepatan kedipnya bisa diatur secara manual melalui potensiometer, sementara LED Kuning tetap berkedip stabil setiap 0,5 detik tanpa terpengaruh oleh perubahan pada LED Merah.


Dampak dari modifikasi ini menunjukkan dua fenomena utama dalam sistem tertanam:
- Kontrol Real-Time: Sistem mampu merespons perubahan input fisik (potensiometer) secara langsung dan mengubah perilaku output (LED) secara instan.
- Independensi Task (Multitasking): Terlihat jelas bahwa meskipun TaskBlinkMerah mengalami perubahan beban waktu delay yang drastis, TaskBlinkKuning sama sekali tidak terganggu. Hal ini membuktikan keunggulan RTOS dibandingkan metode delay() biasa pada Arduino, di mana dalam RTOS satu tugas yang sedang "istirahat" (Blocked State) tidak akan menghentikan eksekusi tugas lainnya.
