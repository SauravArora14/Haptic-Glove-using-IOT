#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Saurav's Galaxy M32";         // Replace with your network SSID
const char* password = "gryo9219"; // Replace with your network password

const char* server = "http://api.thingspeak.com/update";
String apiKey = "UYI4MA4QQAATBE63";         // Replace with your ThingSpeak API key

// Define the pins for flex sensors
#define FLEX1_PIN 34 // Analog pin for Flex Sensor 1
#define FLEX2_PIN 35 // Analog pin for Flex Sensor 2

void setup() {
  Serial.begin(115200);
  pinMode(FLEX1_PIN, INPUT);
  pinMode(FLEX2_PIN, INPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  int flex1Value = analogRead(FLEX1_PIN); // Read Flex Sensor 1
  int flex2Value = analogRead(FLEX2_PIN); // Read Flex Sensor 2

  // Thresholds to detect bending
  int threshold = 500; // Adjust based on your flex sensor readings

  if (flex1Value > threshold) {
    Serial.println("Request: Water");
  }

  if (flex2Value > threshold) {
    Serial.println("Request: Food");
  }

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Construct the URL for ThingSpeak with field updates
    String url = String(server) + "?api_key=" + apiKey + "&field1=" + String(flex1Value) + "&field2=" + String(flex2Value);

    http.begin(url);
    int httpCode = http.GET();  // Send the request

    // Check the returning code
    if (httpCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
    }

    http.end();  // Close connection
  }

  // Wait for 15 seconds to send the next data
  delay(3000);
}
