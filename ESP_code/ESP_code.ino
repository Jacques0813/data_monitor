#include <WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <ArduinoJson.h>

// Details to connect to wifi network.
char* ssid = "VDWALTS";
char* password = "Lvdw1208";

// Details needed to connect to MQTT server.
// char* mqtt_server = "41.193.5.154";
// int mqtt_port = 22500;

// Details to connect to time server.
char* time_ntpServer = "pool.ntp.org";
long  time_gmtOffset_sec = 0;
int   time_daylightOffset_sec = 3600;

// Details to connect to MQTT server.
char* mqtt_server = "broker.emqx.io";
int mqtt_port = 1883;

// Set the MQTT client as a wifi client.
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(9600);
  
  connect_To_Wifi();

  connect_To_MQTT(mqtt_server, mqtt_port);

  get_Time(time_ntpServer, time_gmtOffset_sec, time_daylightOffset_sec);
}

void loop() {
  // Measures potentiometer value.
  int val1 = random(30);
  int val2 = random(30);
  int val3 = random(30);
  int val4 = random(30);
  int val5 = random(30);
  int val6 = random(30);
  int val7 = random(30);
  int val8 = random(30);
  int val9 = random(30);
  int val10 = random(30);
  int val11 = random(30);
  int val12 = random(30);
  int val13 = random(30);
  int val14 = random(30);

  publish_To_MQTT(val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14);
  delay(500);
}

// Function to connect to wifi.
void connect_To_Wifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
}

void connect_To_MQTT(char* server, int port){
  // Sets the MQTT server.
  client.setServer(server, port);
  // Connects to MQTT server.
  client.connect("arduinoClient");
}

void publish_To_MQTT(int value1, int value2, int value3, int value4, int value5, int value6, int value7, int value8, int value9, int value10, int value11, int value12, int value13, int value14){
  // Continually checks if the client is connected to the arduino.
  if (!client.connected()) {
    client.connect("arduinoClient");
  }


  // Converts the number to a string to send to MQTT server.
  char value_payload[200];
  sprintf(value_payload, "%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d", value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11, value12, value13, value14);
  // sprintf(value_payload, "%d#%d#%d", value1, value2, value3);
  // Serial.println(value_payload);
  // Publishes to MQTT Server
  client.publish("esp32/Data", value_payload);
  // client.publish("esp32/Flow", value_payload);
}

void get_Time(char* ntpServer, long  gmtOffset_sec, int daylightOffset_sec){
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  // Stores the time in timeinfo.
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  // Converts the time to the correct format.
  char time[30];
  strftime(time, 30, "%d-%B-%Y_%H-%M-%S", &timeinfo);
  Serial.println(time);

    // Continually checks if the client is connected to the arduino.
  if (!client.connected()) {
    client.connect("arduinoClient");
  }

  // Sends the time.
  client.publish("esp32/time", time);
  delay(2000);
}
