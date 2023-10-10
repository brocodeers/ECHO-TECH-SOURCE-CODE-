#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <DHT.h> // Include the DHT library

char ssid[] = "Moulik";
char pass[] = "12345678";
char firebaseHost[] = "echo-tech-default-rtdb.asia-southeast1.firebasedatabase.app";
; // Replace with your Firebase project's database URL (without "https://")
String apiKey = "AIzaSyD9X96aRDLGD_52bcds9pcNC5ybzGtzfys"; // Replace with your Firebase project's API key

WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, firebaseHost,443);

// Create a DHT sensor object (DHT22 in this case)
#define DHTPIN 2 // Connect the DATA pin of your DHT22 sensor to pin 2 of Arduino
#define DHTTYPE DHT22 // Change to DHT11 if you're using a DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void sendDataToFirebase(float temperature, float humidity) 
{
  if (client.connect(firebaseHost,443)) { // Use client.connect() with host and port
    // Construct the Firebase URL with the path where you want to store data
    String firebasePath = "/sensorData.json"; // Replace with your desired path
    String url = "/" + firebasePath + "?auth=" + apiKey;

    // Send the data to Firebase using HTTP PUT or POST
    String dataToSend = "{\"temperature\": " + String(temperature, 2) + ", \"humidity\": " + String(humidity, 2) + "}";
    client.put(url, "application/json", dataToSend);

    // Check for the server's response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("HTTP Status Code: ");
    Serial.println(statusCode);
    Serial.print("Server Response: ");
    Serial.println(response);

    client.stop();
  } else {
    Serial.println("DATA NOT SENT");
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to Wi-Fi");

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT22 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
   
  // Send temperature and humidity data to Firebase
  sendDataToFirebase(temperature, humidity);
  Serial.println(temperature);
  Serial.println(humidity);
  delay(5000); // Send data every 5 seconds (adjust as needed)
}