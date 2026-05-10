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

Penjelasan code:
1. Definisi Struktur dan Komunikasi data
- Struct `readings`: Digunakan untuk membungkus dua data sekaligus (suhu dan kelembapan) ke dalam satu paket. Hal ini memudahkan pengiriman data yang kompleks antar tugas.
- Queue (`structQueue`): Berfungsi sebagai "kotak surat" digital. Antrean ini dibuat dengan kapasitas 5 slot data. Fungsinya adalah untuk mengirimkan data dari tugas pembaca sensor ke tugas tampilan secara aman tanpa risiko tabrakan data (race condition).
2. Inisialisasi Sistem (setup)
- `xTaskCreatePinnedToCore`: Ini adalah fitur khusus ESP32. Kita membagi tugas ke dua inti prosesor yang berbeda: Core 0: Dikhususkan untuk TaskReadData agar pembacaan sensor tidak terganggu oleh proses lain., Core 1: Digunakan untuk TaskDisplay dan TaskBlink.
- Pemisahan ini memastikan bahwa meskipun sensor DHT22 membutuhkan waktu lama untuk merespons, kedipan LED dan tampilan Serial tidak akan mengalami lag atau macet.
3. Mekanisme Kerja Task
- TaskReadData (Penghasil Data/Producer): Tugas ini membaca suhu dan kelembapan dari pin GPIO 4 setiap 2 detik. Jika data valid, tugas ini akan mengirimkan paket data ke dalam antrean menggunakan fungsi xQueueSend. Penggunaan vTaskDelay(2000) memberikan waktu bagi sensor untuk menstabilkan pembacaan tanpa menghentikan kerja prosesor secara keseluruhan.
- TaskDisplay (Penerima Data/Consumer): Tugas ini dalam kondisi "istirahat" (Blocked State) hingga ada data masuk ke dalam antrean. Begitu xQueueReceive mendeteksi data baru, tugas ini segera terbangun dan mencetak hasilnya ke Serial Monitor. Fungsi xPortGetCoreID() ditambahkan untuk membuktikan bahwa tugas ini berjalan secara paralel di inti prosesor yang berbeda.
- TaskBlink (Indikator Independen): Tugas ini berfungsi mengedipkan LED Merah dan Kuning secara bergantian setiap 0,5 detik. Tugas ini membuktikan konsep multitasking yang sesungguhnya; LED tetap berkedip dengan presisi tinggi meskipun tugas lain sedang sibuk menunggu data dari sensor.


Secara keseluruhan, program ini mendemonstrasikan sinkronisasi antar tugas. Data mengalir secara satu arah: dari sensor melalui Core 0, masuk ke Queue, lalu diambil dan diproses oleh Core 1. Mekanisme ini menjamin bahwa sistem tetap responsif (Real-Time) meskipun menangani komponen yang memiliki kecepatan respon berbeda-beda.
