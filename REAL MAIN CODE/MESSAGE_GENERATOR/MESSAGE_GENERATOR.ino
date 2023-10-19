#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <DHT.h>

char ssid[] = "Me";
char pass[] = "12345678";

//firebase keys--------
char firebaseHost[] = "echo-tech-default-rtdb.asia-southeast1.firebasedatabase.app";
String apiKey = "AIzaSyD9X96aRDLGD_52bcds9pcNC5ybzGtzfys";
//ifttt-------------
char HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME = "/trigger/email_to_me/with/key/fKsf7Mh8Re2PNyWCQIK5iQoSvjnCa1epfNBFvUe_wbA";

WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, firebaseHost, 443);

// Create a DHT sensor object 
#define DHTPIN 2       // Connect the DATA pin of your DHT22 sensor to pin 2 of Arduino
#define DHTTYPE DHT22  //THE TYPE OF THE DHT22 WE ARE USING 
DHT dht(DHTPIN, DHTTYPE);

//SENDING THE EMIAL ------------ IFTT
void sendEmail(String subject, float any) {
  if (client.connect(HOST_NAME, 80)) {
    String queryString = "?value1=" + subject + "&value2=" + String(any);
    client.print("GET " + PATH_NAME + queryString + " HTTP/1.1\r\n");
    client.print("Host: " + String(HOST_NAME) + "\r\n");
    client.print("Connection: close\r\n");
    client.print("\r\n");
    Serial.println("Request sent");
    Serial.println("Email sent");
  } else {
    Serial.println("Connection to IFTTT server failed");
  }

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  }
}

//FIREBASE SENDING FUCNTION
void sendDataToFirebase(float temperature, float humidity, float CO2, float light, float moisture_percentage) {
  //message generator fetcher -----
  String message = String_message_of_temp(temperature);
  String message_humi = String_message_of_humidity(humidity);
  String message_CO2 = String_message_of_CO2(CO2);
  String message_light = String_message_of_lightintensity(light);
  String messg_moit = String_message_of_soilmoisture(moisture_percentage);

  if (client.connect(firebaseHost, 443)) {  // Use client.connect() with host and port
    // Construct the Firebase URL with the path where you want to store data
    String firebasePath = "/sensorData.json";  // Replace with your desired path
    String url = "/" + firebasePath + "?auth=" + apiKey;

    // Send the data to Firebase using HTTP PUT or POST
    String dataToSend = "{\"temperature\": \"" + String(temperature, 2) + "\",\"humidity\": \"" + String(humidity, 2) + "\",\"CO2\": \"" + String(CO2, 2) + "\",\"light\": \"" + String(light, 2) + "\",\"Moisture\": \"" + String(moisture_percentage, 2) + "\",\"tempmessage\": \"" + String(message) + "\",\"humimessage\": \"" + String(message_humi) + "\",\"message_CO2\": \"" + String(message_CO2) + "\",\"message_light\": \"" + String(message_light) + "\",\"msg_moist\": \"" + String(messg_moit) + "\"}";
    client.put(url, "application/json", dataToSend);

    // Check for the server's response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("HTTP Status Code: ");
    Serial.println(statusCode);
    Serial.print("Server Response: ");
    Serial.println(response);
    Serial.println(dataToSend);
    client.stop();
  } else {
    Serial.println("DATA NOT SENT");
  }
}


//temp message generator ------
String String_message_of_temp(float temp) {
  String message;
  if (temp <= 5) {
    message = "not suitabel For Both hot and cold plants";
  }
  if (temp > 5 || temp < 10) {
    message = "Cold: Ideal for cold-loving plants";
  }
  if (temp = 10 || temp < 15) {
    message = " Moderately Cool: Suitable for cold-loving plants but Suitable for some  hot plants";
  }
  if (temp = 15 || temp < 24) {
    message = "Moderate: Suitable for Both hot and cold most common plants";
  }
  if (temp = 24 || temp < 32) {
    message = "Warm: Ideal for many tropical plants. ";
  }
  if (temp = 32) {
    message = "Too Hot: Some plants may suffer from heat stress. ";
  }
  return message;
}
//humidity messgae generator------
String String_message_of_humidity(float humidity) {
  String message;
  if (humidity < 40) {
    message = "Suitabel for plants like :Desert Plants, Succulents  ";
  }
  if (humidity >= 40 || humidity <= 60) {
    message = "suitable for Most Common Greenhouse Plants  ";
  }
  if (humidity >= 60 || humidity <= 80) {
    message = "Suitabel for plants like :Tropical Plants, Orchids, Ferns     ";
  }
  if (humidity > 80) {
    message = " Suitable for Humidity-Loving Plants and Air Plants ";
  }
  return message;
}

//co2 message generator ----------
String String_message_of_CO2(float CO2) {
  String message;
  if (CO2 < 800) {
    message = "Low CO2 levels, can limit plant growth.";
  }
  if (CO2 >= 800 || CO2 < 1200) {
    message = "Ideal range for maximizing plant growth and productivity.";
  }
  if (CO2 > 1200) {
    message = "CO2 levels significantly above the ideal range, which may not provide additional benefits and can potentially be detrimental to plant growth.";
  }
  return message;
}
//lightintensity message generator -----
String String_message_of_lightintensity(float light) {
  String message;

  if (light < 20) {
    message = "Extremely Low Light";
  } else if (light >= 20 && light < 40) {
    message = "Low Light";
  } else if (light >= 40 && light < 60) {
    message = "Moderate Light";
    sendEmail(message, light);
  } else if (light >= 60 && light < 80) {
    message = "Good Light for Most Plants";
    sendEmail(message, light);
  } else if (light >= 80 && light < 90) {
    message = "Bright Light, Ideal for Sun-Loving Plants";
    sendEmail(message, light);
  } else if (light >= 90) {
    message = "Extremely Bright Light, May Be Harmful to Some Plants";
    //SENDING THE EMIAL ------------ IFTT
    sendEmail(message, light);
  }

  return message;
}

//soilmoisture message generator -------
String String_message_of_soilmoisture(float moisture_percentage) {
  String message;

  if (moisture_percentage < 20) {
    message = "Very Dry: Water your plants immediately.";
  } else if (moisture_percentage >= 20 && moisture_percentage < 40) {
    message = "Dry: Water your plants soon.";
  } else if (moisture_percentage >= 40 && moisture_percentage < 60) {
    message = "Moderate Moisture: Soil is adequately moist for most plants.";
  } else if (moisture_percentage >= 60 && moisture_percentage < 80) {
    message = "Good Moisture: Ideal for many plants.";
  } else if (moisture_percentage >= 80) {
    message = "Very Wet: Be cautious, as excessive moisture can harm plants.";
  }
  return message;
}

void setup() {
  pinMode(A3, OUTPUT);  //soil moisture sensor
  pinMode(A0, OUTPUT);  //LDR PIN
  pinMode(A4, OUTPUT);  //C02 PIN DEFINE
  Serial.begin(9600);
  while (!Serial)
    ;
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
  float CO2 = analogRead(A4);
  float light = analogRead(A0);
  //calculating the analog value
  float sensor_analog = analogRead(A3);
  //now converting  the bit value range from 0 to 1023 to percent
  float moisture_percentage = (100 - ((sensor_analog / 1023.00) * 100));
  // Send temperature and humidity data to Firebase
  sendDataToFirebase(temperature, humidity, CO2, light, moisture_percentage);

  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(CO2);
  Serial.println("light");
  Serial.println(light);
  Serial.println("Moist");
  Serial.println(moisture_percentage);

  delay(5000);  // Send data every 5 seconds
}