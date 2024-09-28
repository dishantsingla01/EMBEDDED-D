#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "dishantt";
const char* password = "dishant111";
const char* mqtt_server = "test.mosquitto.org";  // You can replace this with another broker if needed
const int mqtt_port = 1883;
const char* mqtt_topic = "SIT210/wave";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int ledPin = 2;  // Pin for the LED

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  // Set LED pin as output
  
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  // Set the callback function to handle received messages
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// This function is called whenever a message is received from the subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Flash the LED 3 times
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}

void connectWiFi() {
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ArduinoReceiver")) {
      Serial.println("Connected to MQTT");
      client.subscribe(mqtt_topic);  // Subscribe to the MQTT topic
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(5000);
}
}
}