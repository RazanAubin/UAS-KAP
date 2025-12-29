#include <Wire.h>
#include <BH1750.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Pin Configuration untuk ESP32
#define LIGHT_SENSOR_SDA 21      // ESP32 SDA default
#define LIGHT_SENSOR_SCL 22      // ESP32 SCL default
#define PIR_SENSOR 13            // Contoh: GPIO 13
#define RELAY1_CH1 12            // Contoh: GPIO 12
#define RELAY1_CH2 14            // Contoh: GPIO 14
#define RELAY2_CH1 27            // Contoh: GPIO 27
#define RELAY2_CH2 26            // Contoh: GPIO 26

// WiFi Credentials - SESUAIKAN DENGAN JARINGAN ANDA
const char* ssid = "dosen";
const char* password = "dosenjaya";

// MQTT Configuration - Menggunakan test.mosquitto.org
const char* mqtt_server = "test.mosquitto.org"; // Broker publik mosquitto.org
const char* mqtt_client_id = "smart_home_esp32_"; // Akan ditambahkan random number
const int mqtt_port = 1883; // Port non-TLS

// MQTT Topics (tetap sama)
const char* topic_light = "smart_home/sensor/light";
const char* topic_pir = "smart_home/sensor/motion";
const char* topic_washing_machine = "smart_home/device/washing_machine";
const char* topic_water_pump = "smart_home/device/water_pump";
const char* topic_main_light = "smart_home/device/main_light";
const char* topic_status = "smart_home/system/status";

WiFiClient espClient;
PubSubClient client(espClient);
BH1750 lightMeter;

// Variables
unsigned long lastMsg = 0;
unsigned long lastPirCheck = 0;
unsigned long motionStartTime = 0;
bool motionDetected = false;
bool pirState = false;
bool lastPirState = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(PIR_SENSOR, INPUT);
  pinMode(RELAY1_CH1, OUTPUT);
  pinMode(RELAY1_CH2, OUTPUT);
  pinMode(RELAY2_CH1, OUTPUT);
  pinMode(RELAY2_CH2, OUTPUT);
  
  // Initialize semua relay ke OFF (HIGH untuk relay aktif LOW)
  digitalWrite(RELAY1_CH1, HIGH);
  digitalWrite(RELAY1_CH2, HIGH);
  digitalWrite(RELAY2_CH1, HIGH);
  digitalWrite(RELAY2_CH2, HIGH);
  
  // Initialize I2C dan BH1750
  Wire.begin(LIGHT_SENSOR_SDA, LIGHT_SENSOR_SCL);
  if (!lightMeter.begin()) {
    Serial.println("BH1750 initialization failed!");
  } else {
    Serial.println("BH1750 initialized successfully");
  }
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  // Delay untuk PIR sensor warm-up (60 detik)
  Serial.println("PIR sensor warming up...");
  for (int i = 0; i < 60; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nPIR ready!");
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed!");
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);
  
  // Kontrol Mesin Cuci
  if (String(topic) == topic_washing_machine) {
    if (message == "ON") {
      digitalWrite(RELAY1_CH1, LOW);
      client.publish("smart_home/device/washing_machine/status", "ON");
      Serial.println("Washing Machine: ON");
    } else if (message == "OFF") {
      digitalWrite(RELAY1_CH1, HIGH);
      client.publish("smart_home/device/washing_machine/status", "OFF");
      Serial.println("Washing Machine: OFF");
    }
  }
  
  // Kontrol Pompa Air
  if (String(topic) == topic_water_pump) {
    if (message == "ON") {
      digitalWrite(RELAY1_CH2, LOW);
      client.publish("smart_home/device/water_pump/status", "ON");
      Serial.println("Water Pump: ON");
    } else if (message == "OFF") {
      digitalWrite(RELAY1_CH2, HIGH);
      client.publish("smart_home/device/water_pump/status", "OFF");
      Serial.println("Water Pump: OFF");
    }
  }
  
  // Kontrol Lampu Utama (jika diperlukan)
  if (String(topic) == topic_main_light) {
    if (message == "ON") {
      digitalWrite(RELAY2_CH1, LOW);
      client.publish("smart_home/device/main_light/status", "ON");
      Serial.println("Main Light: ON");
    } else if (message == "OFF") {
      digitalWrite(RELAY2_CH1, HIGH);
      client.publish("smart_home/device/main_light/status", "OFF");
      Serial.println("Main Light: OFF");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = String(mqtt_client_id) + String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      
      // Subscribe ke topik kontrol
      client.subscribe(topic_washing_machine);
      client.subscribe(topic_water_pump);
      client.subscribe(topic_main_light);
      
      // Publish status koneksi
      client.publish(topic_status, "System connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long now = millis();
  
  // Baca sensor setiap 2 detik
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    // Baca intensitas cahaya dari BH1750
    float lux = lightMeter.readLightLevel();
    
    // Publish data intensitas cahaya
    char luxStr[10];
    dtostrf(lux, 4, 2, luxStr);
    client.publish(topic_light, luxStr);
    
    // Log ke serial monitor
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lux");
    
    // Cek apakah lampu menyala saat tidak ada orang
    check_light_usage(lux);
  }
  
  // Baca PIR sensor setiap 500ms
  if (now - lastPirCheck > 500) {
    lastPirCheck = now;
    
    int pirValue = digitalRead(PIR_SENSOR);
    pirState = (pirValue == HIGH);
    
    // Deteksi perubahan state PIR
    if (pirState != lastPirState) {
      if (pirState) {
        motionDetected = true;
        motionStartTime = now;
        client.publish(topic_pir, "Motion detected");
        Serial.println("Motion detected!");
      } else {
        client.publish(topic_pir, "No motion");
        Serial.println("Motion ended");
        
        // Set timeout untuk motion (jika tidak ada gerakan selama 30 detik)
        if (motionDetected && (now - motionStartTime > 30000)) {
          motionDetected = false;
          Serial.println("House appears to be empty");
          client.publish("smart_home/sensor/occupancy", "Empty");
        }
      }
      lastPirState = pirState;
    }
    
    // Jika motion terdeteksi, reset timer
    if (motionDetected && pirState) {
      motionStartTime = now;
      client.publish("smart_home/sensor/occupancy", "Occupied");
    }
  }
}

void check_light_usage(float lux) {
  static unsigned long lastAlertTime = 0;
  unsigned long now = millis();
  
  // Jika intensitas cahaya tinggi (lampu menyala) dan tidak ada gerakan
  if (lux > 100.0 && !motionDetected && (now - motionStartTime > 60000)) {
    // Kirim alert maksimal setiap 5 menit
    if (now - lastAlertTime > 300000) {
      client.publish("smart_home/alert/light", "Light is ON but no motion detected");
      Serial.println("ALERT: Light is ON but no motion detected!");
      lastAlertTime = now;
    }
  }
}