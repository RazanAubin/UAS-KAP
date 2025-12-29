Kelompok:
Fayed yamani (2022071017)
daffa bintang pratama (2022077002)
razan aubin farras (2022071043)

# Smart Home Monitoring System dengan ESP32

## a. Judul & Ringkasan Cara Kerja Sistem

### **Judul:**
**Sistem IoT Smart Home untuk Monitoring dan Kontrol Perangkat Listrik Berbasis ESP32**

### **Ringkasan Cara Kerja Sistem:**
Sistem ini dirancang untuk memantau dan mengontrol perangkat listrik di rumah secara remote menggunakan teknologi IoT. Sistem terdiri dari:

1. **Node Sensor-Aktor (ESP32)**: 
   - Membaca intensitas cahaya menggunakan sensor BH1750
   - Mendeteksi keberadaan penghuni menggunakan sensor PIR
   - Mengontrol perangkat listrik (mesin cuci, pompa air, lampu) melalui relay

2. **Broker MQTT (test.mosquitto.org)**:
   - Sebagai penghubung komunikasi antara ESP32 dan dashboard
   - Menerima dan mendistribusikan pesan dari/tuju semua komponen

3. **Dashboard Web**:
   - Antarmuka pengguna untuk monitoring real-time
   - Kontrol perangkat dari jarak jauh
   - Menampilkan notifikasi dan alert

**Alur Kerja:**
Sensor BH1750 → ESP32 → MQTT Broker → Dashboard Web
Sensor PIR → ESP32 → MQTT Broker → Dashboard Web
Dashboard Web → MQTT Broker → ESP32 → Relay → Perangkat



## b. Hardware dan Software/Tools yang Digunakan

### **Hardware:**
1. **Mikrokontroler**: ESP32-WROOM-32U
2. **Sensor Cahaya**: BH1750 (I2C Digital Light Sensor)
3. **Sensor Gerak**: PIR Motion Sensor (HC-SR501)
4. **Modul Relay**: 2x Relay Module Double Channel (4 relay)
5. **Perangkat Listrik**: 
   - Mesin cuci (simulasi dengan lampu)
   - Pompa air (simulasi dengan kipas)
   - Lampu utama
6. **Power Supply**: 
   - 5V untuk relay dan sensor
   - 3.3V untuk ESP32
7. **Kabel jumper**: Male-to-female dan male-to-male

### **Software/Tools:**
1. **Pengembangan**:
   - Arduino IDE v2.3.2
   - Visual Studio Code
   - PlatformIO Extension

2. **Library Arduino**:
   - WiFi (built-in ESP32)
   - Wire (I2C communication)
   - BH1750 by Christopher Laws
   - PubSubClient by Nick O'Leary

3. **Web Development**:
   - HTML5, CSS3, JavaScript
   - MQTT.js library
   - Google Fonts (Poppins)

4. **Testing Tools**:
   - MQTT Explorer
   - Serial Monitor (115200 baud)
   - Chrome Developer Tools

5. **Broker MQTT**: test.mosquitto.org (public broker)

## c. Gambar Rangkaian (Wiring Diagram)

### **Pin Configuration:**

## ESP32-WROOM-32U:
├── BH1750:
│ ├── VCC → 3.3V
│ ├── GND → GND
│ ├── SDA → GPIO21
│ └── SCL → GPIO22
├── PIR Sensor:
│ ├── VCC → 5V
│ ├── GND → GND
│ └── OUT → GPIO13
├── Relay Module 1:
│ ├── VCC → 5V
│ ├── GND → GND
│ ├── IN1 → GPIO12 (Mesin Cuci)
│ └── IN2 → GPIO14 (Pompa Air)
└── Relay Module 2:
├── VCC → 5V
├── GND → GND
├── IN1 → GPIO27 (Lampu Utama)
└── IN2 → GPIO26 (Spare)

Dashboard
![eab546a5-4f8b-4c11-9503-a2fd39880faf](https://github.com/user-attachments/assets/74c33d86-7a6d-4e5c-85d3-e357f8054960)

Rangkaian
![080bd33a-fcbe-4500-a7ca-55cfa0ff710d](https://github.com/user-attachments/assets/7689505f-da96-4627-bbbf-d69a5edfb005)
