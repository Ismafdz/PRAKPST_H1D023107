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
