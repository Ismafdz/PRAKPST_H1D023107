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

<img width="784" height="507" alt="image" src="https://github.com/user-attachments/assets/b732732d-93cf-42ca-b62e-3dc84bb6506f" />

Pada awalnya, modifikasi dilakukan menggunakan Arduino Uno. Namun, sistem mengalami kendala di mana sensor DHT sering kali tidak terdeteksi atau data tidak muncul di Serial Monitor. Hal ini disebabkan oleh beberapa faktor teknis:
- Keterbatasan Memori (RAM): Arduino Uno hanya memiliki RAM sebesar 2KB. Penggunaan library FreeRTOS bersamaan dengan library DHT dan mekanisme Queue mengonsumsi memori yang sangat besar. Hal ini menyebabkan terjadinya Stack Overflow, sehingga scheduler RTOS tidak mampu menjalankan tugas pembacaan sensor secara stabil.
- Sensitivitas Timing: Sensor DHT membutuhkan protokol komunikasi yang sangat presisi. Dalam lingkungan RTOS di Arduino Uno yang single-core, interupsi dari kernel scheduler sering kali mengganggu proses pembacaan data sensor, sehingga menyebabkan error "Failed to read from DHT sensor".


Sebagai solusi, proyek dipindahkan menggunakan ESP32. Penggunaan ESP32 memberikan hasil yang berhasil dan sangat stabil. Informasi yang ditampilkan menjadi dinamis dan berubah secara real-time sesuai kondisi lingkungan. Hal ini dimungkinkan karena:
<img width="941" height="684" alt="image" src="https://github.com/user-attachments/assets/f909fc9c-e73d-4a1e-8417-7cbc3432366a" />

- Arsitektur Dual-Core: ESP32 memungkinkan penerapan True Multitasking. Tugas pembacaan sensor (TaskReadData) dijalankan pada Core 0, sementara tugas penampilan data (TaskDisplay) dan indikator LED berjalan pada Core 1. Pemisahan beban kerja secara fisik ini membuat pembacaan sensor tidak terganggu oleh tugas lainnya.
- Kapasitas Memori Luas: Dengan RAM sebesar 520KB, ESP32 dapat menangani library sensor dan antrean data (Queue) tanpa risiko kehabisan memori.
- Native RTOS Support: ESP32 dikembangkan berbasis FreeRTOS secara asli (native), sehingga manajemen komunikasi antar task melalui Queue berjalan lebih efisien dan responsif.

<img width="793" height="462" alt="image" src="https://github.com/user-attachments/assets/08b1a809-8d67-4f8a-8eef-932e61221147" />

Modifikasi ini dilakukan dengan mengintegrasikan sensor DHT sesungguhnya (DHT22) ke dalam sistem komunikasi antar tugas (task communication) berbasis Queue pada FreeRTOS. Bagian utama yang dimodifikasi adalah fungsi read_data, di mana variabel suhu dan kelembapan yang sebelumnya diisi dengan nilai statis  diganti dengan pembacaan data dinamis dari sensor menggunakan perintah dht.readTemperature() dan dht.readHumidity(). Proses modifikasinya melibatkan penggunaan library DHT.h untuk inisialisasi sensor pada pin digital tertentu (misalnya Pin 4) dan pengiriman data tersebut ke dalam antrean (queue) agar dapat diakses oleh tugas tampilan (display task) secara real-time.

Perbedaan utama antara rangkaian ini dengan rangkaian tanpa sensor sesungguhnya terletak pada dinamika dan integritas data. Pada rangkaian sebelumnya, informasi yang ditampilkan pada Serial Monitor bersifat konstan dan tidak mencerminkan kondisi lingkungan. Namun, dengan sensor DHT sesungguhnya, data yang ditampilkan menjadi sangat dinamis dan berubah secara otomatis saat terjadi perubahan suhu atau kelembapan di sekitar sensor. Selain itu, penggunaan platform ESP32 sebagai alternatif dari Arduino Uno dalam modifikasi ini memberikan kestabilan yang lebih tinggi berkat arsitektur Dual-Core, yang memungkinkan pembacaan sensor dan penampilan data berjalan secara paralel tanpa risiko stack overflow atau kegagalan timing yang sering terjadi pada mikrokontroler single-core dengan memori terbatas. Bisa terlihat dalam code hasil modifikasi berikut:
```cpp
#include "DHT.h"

// Definisi Pin
#define DHTPIN 4       // Pin Data DHT22 ke GPIO 4
#define DHTTYPE DHT22  // Sensor DHT22
#define LED_MERAH 13   // LED Merah ke GPIO 13
#define LED_KUNING 12  // LED Kuning ke GPIO 12

// Inisialisasi Sensor
DHT dht(DHTPIN, DHTTYPE);

// Struktur data untuk Queue
struct readings {
  float temp;
  float h;
};

// Handle untuk Queue
QueueHandle_t structQueue;

void setup() {
  // ESP32 biasanya menggunakan baud rate 115200
  Serial.begin(115200);
  dht.begin();
  
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);

  // Membuat Queue untuk menampung 5 data struct
  structQueue = xQueueCreate(5, sizeof(struct readings));

  if (structQueue != NULL) {
    // Membuat TaskRead dan diletakkan di Core 0
    xTaskCreatePinnedToCore(
      TaskReadData,   // Fungsi task
      "ReadData",     // Nama task
      2048,           // Stack size (ESP32 butuh lebih besar dari Uno)
      NULL,           // Parameter
      1,              // Prioritas
      NULL,           // Task handle
      0               // Core 0
    );

    // Membuat TaskDisplay dan diletakkan di Core 1
    xTaskCreatePinnedToCore(
      TaskDisplay,
      "Display",
      2048,
      NULL,
      1,
      NULL,
      1               // Core 1
    );

    // Membuat TaskBlink dan diletakkan di Core 1
    xTaskCreatePinnedToCore(
      TaskBlink,
      "Blink",
      2048,
      NULL,
      1,
      NULL,
      1               // Core 1
    );
  }
}

void loop() {
  // Kosong, semua diatur oleh FreeRTOS
}

// --- Implementasi Task ---

void TaskReadData(void *pvParameters) {
  for (;;) {
    struct readings sensorData;
    sensorData.temp = dht.readTemperature();
    sensorData.h = dht.readHumidity();

    if (isnan(sensorData.temp) || isnan(sensorData.h)) {
      Serial.println("Gagal membaca sensor DHT!");
    } else {
      // Mengirim data ke Queue
      xQueueSend(structQueue, &sensorData, portMAX_DELAY);
    }
    // Delay 2 detik karena DHT22 lambat
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void TaskDisplay(void *pvParameters) {
  struct readings receivedData;
  for (;;) {
    // Menunggu data dari Queue
    if (xQueueReceive(structQueue, &receivedData, portMAX_DELAY) == pdPASS) {
      Serial.print("[CORE ");
      Serial.print(xPortGetCoreID()); // Menampilkan Core yang sedang bekerja
      Serial.print("] Suhu: ");
      Serial.print(receivedData.temp);
      Serial.print("C | Kelembapan: ");
      Serial.print(receivedData.h);
      Serial.println("%");
    }
  }
}

void TaskBlink(void *pvParameters) {
  for (;;) {
    // Mengedipkan kedua LED bergantian
    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(LED_KUNING, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(LED_KUNING, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
```


