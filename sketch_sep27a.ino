#include <WiFiNINA.h>
#include <PubSubClient.h>

const char* ssid = "dishantt";
const char* password = "dishant111";
const char* mqtt_server = "test.mosquitto.org";  // You can replace this with another broker if needed
const int mqtt_port = 1883;
const char* mqtt_topic = "SIT210/wave";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int trigPin = 9;
int echoPin = 10;
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // Ultrasonic sensor reading
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Detect a wave if the distance is less than 10 cm
  if (distance < 400 && distance > 0) {  // Filter invalid readings
    if (distance < 10) {
      Serial.println("Helllllooo");
      client.publish(mqtt_topic, "DISHANT SINGLA");
    } else {
      Serial.println("Error");
    }
  } else {
    Serial.println("Invalid reading");
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
    if (client.connect("ArduinoSender")) {
      Serial.println("Connected to MQTT");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(5000);
}
}
}