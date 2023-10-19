//HERE IS THE DATA TRANSFER USING THE THREE LIBRARIES
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <DHT.h> 
//ADD HERE YOUR IFIF PASSWORD AND NAME 
char ssid[] = "Moulik";
char pass[] = "12345678";

//CREDENTIALS OF THE FIREBASE ADD THE REALTIME DATABAE LINK AND BELOW IS THE API KEY OF THE FIREBASE
char firebaseHost[] = "echo-tech-default-rtdb.asia-southeast1.firebasedatabase.app";
String apiKey = "AIzaSyD9X96aRDLGD_52bcds9pcNC5ybzGtzfys"; 

//FIREBASE AND WIFI OBJECT 
WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, firebaseHost,443);

//DHT SENSORS REDETIALS 
#define DHTPIN 2 //MENTIONING TO WHICH PIN IT IS CONNECTED 
#define DHTTYPE DHT22  //TYPE OF DHT22 WE ARE USING 
//PASSING ALL THE PARAMETERS
DHT dht(DHTPIN, DHTTYPE);

//CREATING THE FUNCTION WHICH SENDS THE DATA TO THE FIREBASE 
void sendDataToFirebase(float temperature, float humidity, float CO2, float light, float moisture_percentage) 
{
  String message = String_message_of_temp(temperature);
  String message_humi = String_message_of_humidity(humidity);

  if (client.connect(firebaseHost,443)) { // Use client.connect() with host and port
    // Construct the Firebase URL with the path where you want to store data
    String firebasePath = "/sensorData.json"; // Replace with your desired path
    String url = "/" + firebasePath + "?auth=" + apiKey;

    // Send the data to Firebase using HTTP PUT or POST
String dataToSend = "{\"temperature\": \"" + String(temperature, 2) + "\", \"humidity\": \"" + String(humidity, 2) + "\", \"CO2\": \"" + String(CO2, 2) + "\", \"light\": \"" + String(light, 2) + "\", \"Moisture\": \"" + String(moisture_percentage, 2) + "\", \"tempmessage\": \"" + String(message) +"\", \"humimessage\": \"" + String(message_humi) +"\"}";
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
  if(temp <= 5)
  {
    message = "not suitabel For Both hot and cold plants";
  }
  if (temp >5|| temp <10) 
  {
    message = "Cold: Ideal for cold-loving plants";
  } 
  if(temp = 10|| temp <15)
  {
     message = " Moderately Cool: Suitable for cold-loving plants but Suitable for some  hot plants";
  }
  if(temp =15 || temp < 24)
  {
     message = "Moderate: Suitable for Both hot and cold most common plants";
  }
  if(temp = 24|| temp <32 )
  {
    message = "Warm: Ideal for many tropical plants. ";
  }
  if(temp = 32)
  {
    message = "Too Hot: Some plants may suffer from heat stress. ";
  }
  return message;
}
//humidity messgae generator------
String String_message_of_humidity(float humidity)
{
  String message;
  if(humidity <40)
  {
    message = "Suitabel for plants like :Desert Plants, Succulents  ";
  }
  if(humidity >=40 || humidity <=60 )
  {
     message = "suitable for Most Common Greenhouse Plants  ";
  }
  if(humidity >=60 || humidity <=80 )
  {
     message = "Suitabel for plants like :Tropical Plants, Orchids, Ferns     ";
  }
  if(humidity > 80)
  {
    message = " Suitable for Humidity-Loving Plants and Air Plants ";
  }
  return message;
}


//DECLEARING ALL THE PIN OF TH OTHER SESORS 
void setup() {
  pinMode(A1,OUTPUT);//soil moisture sensor 
  pinMode(A0,OUTPUT);//LDR PIN 
  pinMode(A4,OUTPUT);//C02 PIN DEFINE 
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

//HERE IS THE LOOP WHICH CONTINUE TO ITERATE 
void loop() {
  // Read temperature and humidity from DHT22 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float CO2= analogRead(A4);
  float light = analogRead(A0);
  //calculating the analog value 
  float sensor_analog = analogRead(A3);//SOIL MOISTURE PIN DECLARTION
  //now converting  the bit value range from 0 to 1023 to percent
  float moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  // Send temperature and humidity data to Firebase
  sendDataToFirebase(temperature, humidity,CO2,light,moisture_percentage);
  //PRINTNG THE VALUES TO CHECK WEATHER TEH DATA IS PRINTED BY THE SENSOR AND ALSO CONNECTIVITY
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(CO2);
  Serial.println("light");
  Serial.println(light);  
  Serial.println("Moist");
  Serial.println(moisture_percentage);
 
  delay(5000); //SENDING THE DATA IN EVERY 5 SECONDS YOU CAN ALSO ADJUUST IT TO YOUR NEEDS
}