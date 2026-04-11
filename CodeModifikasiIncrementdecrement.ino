// Definisi Pin Seven Segment (a, b, c, d, e, f, g, dp)
const int segmentPins[8] = {7, 6, 5, 11, 10, 8, 9, 4};

// Definisi Pin untuk Tombol
const int btnUp = 2;   // Tombol untuk menambah angka
const int btnDown = 3; // Tombol untuk mengurangi angka

// Pola Karakter 0-F (1 = ON, 0 = OFF untuk Common Cathode)
byte digitPattern[16][8] = {
  {1,1,1,1,1,1,0,0}, {0,1,1,0,0,0,0,0}, {1,1,0,1,1,0,1,0}, {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0}, {1,0,1,1,0,1,1,0}, {1,0,1,1,1,1,1,0}, {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0}, {1,1,1,1,0,1,1,0}, {1,1,1,0,1,1,1,0}, {0,0,1,1,1,1,1,0},
  {1,0,0,1,1,1,0,0}, {0,1,1,1,1,0,1,0}, {1,0,0,1,1,1,1,0}, {1,0,0,0,1,1,1,0}
};

int currentDigit = 0;      // Menyimpan nilai angka saat ini
bool lastUpState = HIGH;   // Menyimpan status terakhir tombol Up
bool lastDownState = HIGH; // Menyimpan status terakhir tombol Down

void displayDigit(int num) {
  for(int i=0; i<8; i++) {
    digitalWrite(segmentPins[i], digitPattern[num][i]);
  }
}

void setup() {
  // Mengatur semua pin segmen sebagai OUTPUT
  for(int i=0; i<8; i++) pinMode(segmentPins[i], OUTPUT);
  
  // Mengatur pin tombol sebagai INPUT dengan resistor internal
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  
  // Menampilkan angka awal (0)
  displayDigit(currentDigit);
}

void loop() {
  // Membaca status tombol saat ini
  bool upState = digitalRead(btnUp);
  bool downState = digitalRead(btnDown);

  // Logika Tombol Tambah (Up)
  if(lastUpState == HIGH && upState == LOW) {
    currentDigit++;
    if(currentDigit > 15) currentDigit = 0; // Kembali ke 0 jika lebih dari F
    displayDigit(currentDigit);
    delay(50); // Debouncing sederhana
  }
  lastUpState = upState;

  // Logika Tombol Kurang (Down)
  if(lastDownState == HIGH && downState == LOW) {
    currentDigit--;
    if(currentDigit < 0) currentDigit = 15; // Kembali ke F jika di bawah 0
    displayDigit(currentDigit);
    delay(50); // Debouncing sederhana
  }
  lastDownState = downState;
}
