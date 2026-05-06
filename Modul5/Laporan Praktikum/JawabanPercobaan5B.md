# Modul 5 Real-Time Operating System

## Jawaban Pertanyaan Percobaan 5B: Komunikasi Task

### 1. Apakah kedua task berjalan secara bersamaan atau bergantian? Jelaskan mekanismenya!
Kedua task pada Percobaan 5B ini sebenarnya berjalan secara bergantian, namun dilakukan dengan transisi yang sangat cepat sehingga terlihat seolah-olah berjalan bersamaan (concurrent).
- Sistem Satu Inti (Single Core): Karena menggunakan Arduino Uno, hanya ada satu task yang bisa berada dalam Running State pada satu waktu.
- Pengelolaan oleh Scheduler: Kernel Scheduler bertugas membagi waktu prosesor di antara Task `read_data` (pengirim) dan Task `display` (penerima).
- Sinkronisasi melalui Queue: Mekanisme pada percobaan ini melibatkan Queue sebagai media komunikasi. Task `read_data` mengisi data ke dalam struct, lalu menggunakan fungsi `xQueueSend()` untuk mengirimkan data tersebut ke antrean. Setelah mengirim, task ini masuk ke Blocked State melalui `vTaskDelay(100)`.  Task display menggunakan fungsi `xQueueReceive()` untuk mengambil data dari antrean. Jika antrean kosong, task ini akan menunggu (Blocked State) hingga ada data yang masuk.  
- Perpindahan State: Saat salah satu task sedang menunggu (karena delay atau menunggu antrean), scheduler secara otomatis memindahkan kendali prosesor ke task lainnya yang berada dalam Ready State. Hal ini memastikan pemrosesan data terjadi secara langsung tanpa adanya penundaan yang merusak alur kerja sistem (Real-Time).

Secara teknis, kedua tugas tersebut berjalan secara bergantian namun terkoordinasi melalui media komunikasi Queue. Mekanisme ini dikelola oleh Kernel Scheduler yang membagi waktu prosesor di antara kedua tugas tersebut. Tugas pengirim (Task `read_data`) akan mengisi data ke dalam antrean menggunakan fungsi `xQueueSend()`, kemudian masuk ke Blocked State untuk memberikan kesempatan kepada tugas penerima (Task display) untuk mengambil data tersebut melalui fungsi `xQueueReceive()`. Dengan adanya sistem antrean ini, perpindahan tugas terjadi secara otomatis dan sinkron tanpa ada data yang hilang atau bertabrakan, sehingga proses pengiriman informasi tetap bersifat real-time.

### 2. Apakah program ini berpotensi mengalami race condition? Jelaskan!
Berdasarkan struktur kode pada Percobaan 5B, program ini tidak berpotensi mengalami race condition yang berbahaya karena:
- Penggunaan Queue: Program ini menggunakan fitur Queue dari FreeRTOS untuk berkomunikasi antar tugas. Queue secara internal sudah memiliki mekanisme perlindungan atau thread-safe. Artinya, FreeRTOS secara otomatis mengatur agar hanya satu tugas yang bisa mengakses (menulis atau membaca) data di dalam antrean tersebut pada satu waktu.
- Isolasi Data: Data sensor tidak disimpan dalam variabel global yang bisa diakses bebas oleh semua tugas, melainkan dikirim sebagai salinan ke dalam antrean. Hal ini mencegah terjadinya tabrakan data di mana satu tugas mencoba membaca data sementara tugas lain sedang mengubahnya di waktu yang bersamaan.
- Sinkronisasi Otomatis: Fungsi  `xQueueSend` dan `xQueueReceive` memiliki mekanisme tunggu (blocking) yang memastikan tugas akan berhenti sementara jika antrean penuh atau kosong, sehingga sinkronisasi antar tugas tetap terjaga dengan aman.
  
### 3. Modifikasilah program dengan menggunakan sensor DHT sesungguhnya sehingga informasi yang ditampilkan dinamis. Bagaimana hasilnya? Jelaskan program pada file README.md.
