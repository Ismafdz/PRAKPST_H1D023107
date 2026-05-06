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
