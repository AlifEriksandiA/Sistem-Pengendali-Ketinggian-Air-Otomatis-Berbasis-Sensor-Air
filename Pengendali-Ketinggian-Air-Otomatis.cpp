#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LowPower.h>
#include <avr/wdt.h>
 
 
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
// Pin analog sensor
const int sensorBawahPin = A2;
const int sensorAtasPin  = A1;
const int relayPompaPin  = 2;
const int tombolManualPin = 3;
 
// Threshold deteksi air
const int thresholdAirbawah = 500;
const int thresholdAiratas = 450;
 
bool pompaNyala = false;
bool modeIsiUlang = false;
bool lastButtonState = HIGH;
 
// Untuk log Serial rate-limited
unsigned long lastLogTime = 0;
const unsigned long intervalLog = 1000;
 
void setup() {
  if (MCUSR & _BV(WDRF)) {
    MCUSR = 0;
  }
  pinMode(relayPompaPin, OUTPUT);
  digitalWrite(relayPompaPin, HIGH);
 
  pinMode(tombolManualPin, INPUT_PULLUP);
 
  lcd.begin(16, 2);
  lcd.backlight();
 
  Serial.begin(9600);
  Serial.println("Sistem Pompa Air Dimulai");
 
  lcd.setCursor(0, 0);
  lcd.print(" Sistem Pompa Air ");
  delay(2000);
  lcd.clear();
  wdt_enable(WDTO_2S);
}
 
void loop() {
  // Masuk sleep selama 1 detik untuk hemat daya
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
 
  // Bangun dan jalankan tugas utama
  bacaSensorDanKontrol();
}
 
void bacaSensorDanKontrol() {
  int nilaiSensorBawah = analogRead(sensorBawahPin);
  int nilaiSensorAtas  = analogRead(sensorAtasPin);
  bool sensorBawah = nilaiSensorBawah > thresholdAirbawah;
  bool sensorAtas  = nilaiSensorAtas  > thresholdAiratas;
 
  bool tombolState = digitalRead(tombolManualPin);
 
  // Deteksi tombol ditekan
  if (lastButtonState == HIGH && tombolState == LOW) {
    if (sensorBawah && !sensorAtas) {
      modeIsiUlang = true;
      Serial.println("-> Tombol ditekan, memulai isi ulang manual");
    } else if (sensorBawah && sensorAtas) {
      Serial.println("-> Air sudah penuh, tombol diabaikan");
    } else {
      Serial.println("-> Air LOW, mode isi ulang sudah aktif");
    }
  }
  lastButtonState = tombolState;
 
  // --- Logika kontrol pompa ---
  String statusAir = "";
  String statusPompa = "";
 
  if (!sensorBawah && !sensorAtas) {
    modeIsiUlang = true;
    nyalakanPompa();
    statusAir = "Air: LOW ";
    statusPompa = "Pompa: ON ";
  }
  else if (modeIsiUlang && (sensorBawah && !sensorAtas)) {
    nyalakanPompa();
    statusAir = "Mengisi...";
    statusPompa = "Pompa: ON ";
  }
  else if (modeIsiUlang && (sensorBawah && sensorAtas)) {
    modeIsiUlang = false;
    matikanPompa();
    statusAir = "Air: HIGH";
    statusPompa = "Pompa: OFF";
  }
  else {
    matikanPompa();
    if (sensorBawah && !sensorAtas) {
      statusAir = "Air: MID ";
      statusPompa = "Pompa: OFF";
    } else if (sensorBawah && sensorAtas) {
      statusAir = "Air: HIGH";
      statusPompa = "Pompa: OFF";
    } else {
      statusAir = "Sensor Error";
      statusPompa = "Pompa: OFF";
    }
  }
 
  tampilkanStatus(statusAir, statusPompa);
 
  // Serial log setiap bangun dari sleep (per detik)
  Serial.print("Sensor Bawah: ");
  Serial.print(nilaiSensorBawah);
  Serial.print(sensorBawah ? " (ON)" : " (OFF)");
 
  Serial.print(" | Sensor Atas: ");
  Serial.print(nilaiSensorAtas);
  Serial.print(sensorAtas ? " (ON)" : " (OFF)");
 
  Serial.print(" | Tombol: ");
  Serial.print(tombolState == LOW ? "TEKAN" : "LEPAS");
 
  Serial.print(" -> ");
  Serial.print(statusAir);
  Serial.print(", ");
  Serial.println(statusPompa);
 
  Serial.flush();
  wdt_reset();
 
}
