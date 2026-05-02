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

Fungsi map(val, 0, 1023, 30, 150) adalah instruksi utama yang menentukan batasan gerak. Meskipun potensiometer memberikan input penuh (0 hingga 5 Volt yang dikonversi menjadi 0 hingga 1023 digital), fungsi ini secara matematis menghitung ulang agar outputnya tidak pernah kurang dari 30 atau lebih dari 150

Sesuai dengan skema pada gambar image_03ddfa.png, pin A0 digunakan sebagai gerbang masuk data analog (ADC), sedangkan pin 9 yang memiliki simbol tilde (~) digunakan sebagai gerbang keluar sinyal PWM untuk mengontrol posisi servo

Dengan menempatkan proses pembacaan, pemetaan, dan penulisan di dalam fungsi loop(), mikrokontroler akan terus-menerus memperbarui posisi servo setiap kali potensiometer diputar, namun tetap patuh pada batasan sudut yang telah ditetapkan.


