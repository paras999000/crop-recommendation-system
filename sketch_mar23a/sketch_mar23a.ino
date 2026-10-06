#include <ESP8266WiFi.h>

// WiFi
const char* ssid = "Divyani's iPhone";
const char* password = "divapani";

// ThingSpeak
const char* server = "api.thingspeak.com";
String apiKey = "B9LJ6LGL7FZ5B8U3";

// Variables
float temp, humidity;
int soil, rain;
String crop;

void setup() {
  Serial.begin(9600);

  // Random seed (important)
  randomSeed(analogRead(0));

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected ✅");
}

void loop() {

  // 🔥 SIMULATED SENSOR DATA (DYNAMIC)
  temp = random(20, 36);        // 20–35 °C
  humidity = random(40, 91);    // 40–90 %
  soil = random(20, 91);        // 20–90 %
  rain = random(0, 2);          // 0 or 1

  // 🌾 CROP LOGIC (ADVANCED)
  if (temp > 25 && humidity > 70 && soil > 70 && rain == 0) {
    crop = "Rice 🌾";
  }
  else if (temp >= 20 && temp <= 30 && soil > 50 && humidity > 60) {
    crop = "Sugarcane 🍬";
  }
  else if (temp >= 15 && temp <= 25 && soil > 40 && soil <= 70) {
    crop = "Wheat 🌿";
  }
  else if (temp > 30 && soil < 40 && humidity < 50) {
    crop = "Cotton 🌱";
  }
  else if (humidity > 60 && soil > 30) {
    crop = "Maize 🌽";
  }
  else if (temp > 20 && humidity > 50 && rain == 0) {
    crop = "Paddy 🌾";
  }
  else if (temp > 18 && soil > 35 && humidity > 65) {
    crop = "Barley 🌾";
  }
  else if (temp > 25 && humidity < 60 && soil > 30) {
    crop = "Millet 🌾";
  }
  else if (temp > 22 && humidity > 55 && soil > 45) {
    crop = "Soybean 🌿";
  }
  else {
    crop = "No suitable crop ❌";
  }

  // 📟 SERIAL OUTPUT
  Serial.println("------ DATA ------");
  Serial.print("Temp: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("Soil: "); Serial.println(soil);
  Serial.print("Rain: "); Serial.println(rain);
  Serial.print("Crop: "); Serial.println(crop);

  // 🌐 SEND TO THINGSPEAK
  WiFiClient client;

  if (client.connect(server, 80)) {
    String url = "/update?api_key=" + apiKey +
                 "&field1=" + String(temp) +
                 "&field2=" + String(humidity) +
                 "&field3=" + String(soil) +
                 "&field4=" + String(rain) +
                 "&field5=" + crop;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + server + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Data sent to ThingSpeak ✅");
  } else {
    Serial.println("Connection failed ❌");
  }

  Serial.println("------------------\n");

  delay(15000); // ThingSpeak limit
}