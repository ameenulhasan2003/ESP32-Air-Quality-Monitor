#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>

DHT dht(14, DHT22);

// WOKWI default WIFI
char* ssid = "Wokwi-GUEST";
char* password = "";

unsigned long myChannelNumber = 3142270;
const char* myWriteAPIKey   = "O1EFTOO9MWL8BCL7";

WiFiClient client;

// MQ Analog input
int MQ_PIN = 34;

float Temperature;
float Humidity;
int AirQualityRaw;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(MQ_PIN, INPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  ThingSpeak.begin(client);
}

void loop() {

  Temperature = dht.readTemperature();
  Humidity    = dht.readHumidity();
  AirQualityRaw = analogRead(MQ_PIN);

  if (isnan(Temperature) || isnan(Humidity)) {
    Serial.println("Sensor Error!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(Temperature);
  Serial.println("°C");

  Serial.print("Humidity: ");
  Serial.print(Humidity);
  Serial.println("%");

  Serial.print("AirQualityRaw: ");
  Serial.println(AirQualityRaw);

  ThingSpeak.setField(1, Temperature);
  ThingSpeak.setField(2, Humidity);
  ThingSpeak.setField(3, AirQualityRaw);

  int code = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (code == 200) {
    Serial.println("Uploaded OK!");
  } else {
    Serial.print("Upload Failed, error code = ");
    Serial.println(code);
  }

  delay(15000);
}
