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

Penjelasan kode:
1. Tahap Inisialisasi (Setup)

Pada bagian setup(), sistem mendaftarkan dua unit tugas yaitu TaskBlinkMerah dan TaskBlinkKuning. Keduanya diberikan prioritas yang sama, yang berarti Kernel Scheduler akan membagi waktu penggunaan prosesor secara adil di antara keduanya. Fungsi loop() dibiarkan kosong karena kendali penuh eksekusi telah diambil alih oleh sistem penjadwalan FreeRTOS.

2. Mekanisme Task LED Merah (Kontrol Dinamis)

TaskBlinkMerah bekerja dengan cara membaca input analog dari potensiometer pada pin A0. Nilai mentah (0-1023) tersebut dikonversi menggunakan fungsi map() menjadi rentang waktu milidetik (100ms - 2000ms). Nilai hasil konversi ini kemudian digunakan sebagai parameter dalam fungsi vTaskDelay(). Dampaknya kecepatan kedipan LED Merah akan berubah secara langsung (dinamis) mengikuti posisi tuas potensiometer.

3. Mekanisme Task LED kuning (Kontrol Statis)

Berbeda dengan tugas pertama, TaskBlinkKuning menjalankan instruksi dengan nilai jeda yang tetap (hardcoded) sebesar 500ms. Tugas ini tidak melakukan pembacaan sensor apa pun, sehingga ritme kedipannya akan selalu konsisten.

4. Interaksi Antar Tugas

Meskipun hanya menggunakan satu inti prosesor (single core), kedua tugas ini terlihat berjalan bersamaan. Hal ini terjadi karena saat satu tugas menjalankan vTaskDelay(), tugas tersebut memasuki Blocked State (istirahat) dan secara otomatis memberikan kesempatan kepada tugas lain untuk menggunakan prosesor (Running State).

