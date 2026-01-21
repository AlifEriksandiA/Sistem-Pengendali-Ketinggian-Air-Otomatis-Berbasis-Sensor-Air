# Smart Water Pump Controller (Low Power & Watchdog Timer) 💧⚡

Proyek ini adalah sistem kontrol pompa air otomatis berbasis Arduino yang dirancang untuk **efisiensi daya tinggi** dan **keandalan sistem (anti-hang)**. 

Sistem ini tidak hanya menyalakan/mematikan pompa berdasarkan level air, tetapi juga tidur (*deep sleep*) saat tidak bekerja dan diawasi oleh *Watchdog Timer* untuk mencegah error jangka panjang.

## 🌟 Fitur Unggulan

* **🔋 Ultra Low Power**: Menggunakan library `LowPower`, Arduino akan "tidur" total selama 1 detik di setiap siklus loop, menghemat baterai/daya secara signifikan.
* **🛡️ Watchdog Timer (WDT)**: Fitur keamanan hardware. Jika sistem macet (*freeze/hang*) selama lebih dari 2 detik, sistem akan me-restart dirinya sendiri secara otomatis.
* **🌊 Hysteresis Logic**: Mencegah pompa hidup-mati terus menerus (*short cycling*). Pompa akan menyala saat habis, dan **terus menyala** hingga benar-benar penuh (Sensor Atas basah).
* **🔘 Manual Override**: Tombol fisik untuk memaksa pengisian air jika level air berada di tengah-tengah (belum habis, tapi ingin dipenuhkan).
* **🖥️ Flicker-Free LCD**: Tampilan LCD I2C dioptimalkan agar hanya diperbarui saat teks berubah, mencegah layar berkedip dan menghemat waktu proses CPU.

## 🛠️ Perangkat Keras (Hardware)

* **Mikrokontroler**: Arduino Uno / Nano / Pro Mini.
* **Actuator**: Modul Relay 1 Channel (5V).
* **Display**: LCD 16x2 dengan modul I2C Backpack.
* **Input**:
    * 2x Sensor Level Air (Bisa menggunakan kawat tembaga sederhana, float switch, atau sensor water level).
    * 1x Push Button (Tactile switch).
* **Power**: Power supply 5V atau Baterai.

## 🔌 Konfigurasi Pin (Wiring)

| Komponen | Pin Arduino | Keterangan |
| :--- | :--- | :--- |
| **Sensor Bawah** | Pin A2 | Mendeteksi jika air habis |
| **Sensor Atas** | Pin A1 | Mendeteksi jika air penuh |
| **Relay Pompa** | Pin D2 | Output ke Pompa (Active Low) |
| **Tombol Manual** | Pin D3 | Push Button ke Ground (Pull-up) |
| **LCD SDA** | Pin A4 | Jalur Data I2C |
| **LCD SCL** | Pin A5 | Jalur Clock I2C |

*> **Catatan**: Sensor air analog bekerja dengan prinsip konduktivitas. Hubungkan satu kutub sensor ke Pin Analog (A1/A2) dan kutub lainnya ke VCC (5V).*

## 💾 Instalasi Software

1.  Buka **Arduino IDE**.
2.  Pastikan Library berikut sudah terinstal (Menu **Sketch** > **Include Library** > **Manage Libraries**):
    * `LiquidCrystal_I2C` by Frank de Brabander.
    * `LowPower` by Rocket Scream Electronics.
3.  Buka file `.ino` proyek ini.
4.  Upload ke board Arduino Anda.

## ⚙️ Logika Operasi (Cara Kerja)

Sistem bekerja berdasarkan "State Machine" sederhana:

1.  **Kondisi KOSONG**: 
    * Sensor Bawah `OFF`, Sensor Atas `OFF`.
    * **Aksi**: Pompa **NYALA**. Status LCD: "Air: HABIS".
2.  **Kondisi MENGISI**:
    * Air menyentuh Sensor Bawah (`ON`), tapi belum menyentuh Sensor Atas (`OFF`).
    * **Aksi**: Pompa **TETAP NYALA** sampai penuh. Status LCD: "Air: MENGISI...".
3.  **Kondisi PENUH**:
    * Sensor Bawah `ON`, Sensor Atas `ON`.
    * **Aksi**: Pompa **MATI**. Status LCD: "Air: PENUH".
4.  **Tombol Manual**:
    * Jika tombol ditekan saat air di posisi tengah (Sensor Bawah `ON`, Sensor Atas `OFF`), sistem dipaksa masuk mode **MENGISI** sampai penuh.

## ⚠️ Troubleshooting

* **Pompa Terbalik (Nyala saat penuh)**:
    * Cek spesifikasi Relay Anda (Active Low atau Active High). Kode ini disetting untuk **Active Low** (LOW = Nyala). Jika relay Anda Active High, ubah logika `relayState` di dalam kode.
* **Sensor Tidak Mendeteksi**:
    * Sesuaikan nilai `THRESHOLD_BAWAH` dan `THRESHOLD_ATAS` di kode (default 500 & 450) sesuai dengan konduktivitas air di tempat Anda.

## 👤 Author

1. Alif Eriksandi Agustino
2. Muhammad Farkhan Fadillah
3. Arbi Yusuf Ramanda
4. Maulana Ihsan Maggio
5. Alya Teghar Lestari
6. Bakhitah Cinta Syahirah
   
* Teknik Komputer - Universitas Brawijaya
