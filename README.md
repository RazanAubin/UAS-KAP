Kelompok:
Fayed yamani (2022071017)
daffa bintang pratama (2022077002)
razan aubin farras (2022071043)

# Smart Home Monitoring System dengan ESP32

## a. Judul & Ringkasan Cara Kerja Sistem

### **Judul**
**Sistem IoT Smart Home untuk Monitoring dan Kontrol Perangkat Listrik Berbasis ESP32**

### **Ringkasan Cara Kerja Sistem**
Sistem ini dirancang untuk memantau dan mengontrol perangkat listrik rumah secara **real-time** dan **remote** menggunakan teknologi **Internet of Things (IoT)**.

ESP32 berfungsi sebagai node utama yang:
- Membaca intensitas cahaya dari **BH1750**
- Mendeteksi gerakan menggunakan **sensor PIR**
- Mengontrol perangkat listrik melalui **modul relay**

Komunikasi data dilakukan menggunakan **protokol MQTT**, sehingga data sensor dan perintah kontrol dapat dikirimkan secara efisien melalui jaringan internet.

**Alur kerja sistem:**
Sensor → ESP32 → MQTT Broker → Dashboard Web
Dashboard Web → MQTT Broker → ESP32 → Relay → Perangkat


---

## b. Hardware dan Software/Tools yang Digunakan

### **Hardware**
- ESP32-WROOM-32U
- Sensor Cahaya BH1750 (I2C)
- Sensor Gerak PIR (HC-SR501)
- 2x Modul Relay 2 Channel (5V)
- Mesin Cuci (simulasi)
- Pompa Air (simulasi)
- Lampu
- Power Supply 5V & 3.3V
- Kabel Jumper

### **Software & Tools**
- Arduino IDE v2.3.2
- Visual Studio Code + PlatformIO
- Library:
  - WiFi (ESP32)
  - Wire (I2C)
  - BH1750
  - PubSubClient
- Web:
  - HTML, CSS, JavaScript
  - MQTT.js
- Testing:
  - MQTT Explorer
  - Serial Monitor
- MQTT Broker:
  - `test.mosquitto.org` (Port 1883)

---

## c. Gambar Rangkaian (Wiring Diagram)

> 📌 Simpan gambar wiring di folder `/assets`


### **Konfigurasi Pin ESP32**

**BH1750**
- VCC → 3.3V  
- GND → GND  
- SDA → GPIO21  
- SCL → GPIO22  

**PIR Sensor**
- VCC → 5V  
- GND → GND  
- OUT → GPIO13  

**Relay Module 1**
- IN1 → GPIO12 (Mesin Cuci)
- IN2 → GPIO14 (Pompa Air)

**Relay Module 2**
- IN1 → GPIO27 (Lampu)
- IN2 → GPIO26 (Spare)

---

## d. Gambar Arsitektur Aplikasi & Jaringan Komputer


**Penjelasan Arsitektur:**
- ESP32 terhubung ke jaringan WiFi (STA Mode)
- Data dikirim ke MQTT Broker
- Dashboard Web melakukan subscribe & publish topik MQTT
- Relay mengontrol perangkat listrik berdasarkan perintah MQTT

---

## e. Topik & Payload MQTT

### **Publish (ESP32 → Broker)**

| Topik | Payload | Keterangan |
|-----|--------|-----------|
| `smart_home/sensor/light` | `156.25` | Intensitas cahaya (lux) |
| `smart_home/sensor/motion` | `Motion detected` | Status PIR |
| `smart_home/sensor/occupancy` | `Occupied` | Status hunian |
| `smart_home/device/main_light/status` | `ON` | Status lampu |
| `smart_home/system/status` | `Connected` | Status sistem |

### **Subscribe (ESP32 ← Broker)**

| Topik | Payload | Fungsi |
|-----|--------|-------|
| `smart_home/device/main_light` | `ON / OFF` | Kontrol lampu |
| `smart_home/device/water_pump` | `ON / OFF` | Kontrol pompa |
| `smart_home/device/washing_machine` | `ON / OFF` | Kontrol mesin |
| `smart_home/system/command` | `status` | Request status |

### **Contoh Payload JSON**
```json
{
  "light": 156.25,
  "motion": "Yes",
  "occupancy": "Occupied",
  "devices": {
    "washing_machine": "ON",
    "water_pump": "OFF",
    "main_light": "ON"
  },
  "timestamp": "2024-01-15T14:30:00Z"
}
```
## f. Cara Pengoperasian dan Pengujian

### **Pengoperasian**
- Hubungkan ESP32 ke WiFi
- Jalankan MQTT Broker
- Buka dashboard web
- Pantau data sensor secara real-time
- Kontrol perangkat melalui dashboard

### **Pengujian**
- Uji sensor PIR dengan gerakan
- Tutup sensor BH1750 untuk perubahan lux
- Kirim payload ON/OFF via MQTT Explorer
- Pastikan relay bekerja sesuai perintah

Dashboard
![eab546a5-4f8b-4c11-9503-a2fd39880faf](https://github.com/user-attachments/assets/74c33d86-7a6d-4e5c-85d3-e357f8054960)

Rangkaian
![080bd33a-fcbe-4500-a7ca-55cfa0ff710d](https://github.com/user-attachments/assets/7689505f-da96-4627-bbbf-d69a5edfb005)
