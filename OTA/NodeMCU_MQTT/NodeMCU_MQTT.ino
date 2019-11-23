#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SLEEP_DELAY_IN_SECONDS  2
char temperatureString[6];
int boardLED = 16;

const char* ssid = "KNIGHTS";
const char* password = "A185R5@@";

//const char*  mqtt_server   = "io.adafruit.com";
//const char*  mqtt_username = "fdffditt";
//const char*  mqtt_password = "2c1f1941c5aa4903a85ddc8e47197673";
//const uint8_t  mqtt_port     = "1883";
//const char*  mqtt_topic    = "sagor/feeds/newTopic";
//const char*  mqtt_rd_topic = "sagor/feeds/Light";

const char*  mqtt_server   = "m11.cloudmqtt.com";
const char*  mqtt_username = "fdffditt";
const char*  mqtt_password = "gJpCf2V1O4WK";
const uint16_t  mqtt_port  = 10902;
const char*  mqtt_topic_pub = "541610203040/feedback";
const char*  mqtt_topic_sub = "541610203040/ihexdata";

uint32_t msg_count = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi();
void callback(char* topic, byte* payload, unsigned int length);
void testESP_OnBoardLED();

void setup() {
  Serial.begin(115200);
  pinMode(boardLED, OUTPUT);
  // setup WiFi
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial1.begin(9600);
}

void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    Serial1.print((char)payload[i]);
  }
  Serial.println();
  msg_count++;
  Serial.print(msg_count);
  Serial.print("  ");
  Serial.println(length);
//  if((char)payload[0]=='1')
//  { Serial.println("Button : ON");
//    digitalWrite(boardLED, LOW);}
//  else {Serial.println("Button : OFF");
//   digitalWrite(boardLED, HIGH);}
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("541610203040/ihexdata");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      // Wait 5 seconds before retrying
      delay(3000);
    }
  }
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temperature = (float)(rand()/10000.0);
  //dtostrf(temperature, 2, 2, temperatureString);
  //Serial.print("Sending temperature: ");
  //Serial.println(temperatureString);
  //client.publish(mqtt_topic_pub, temperatureString);


//  Serial.println("Closing MQTT connection...");
//  client.disconnect();
//
//  Serial.println("Closing WiFi connection...");
//  WiFi.disconnect();

//  delay(1000);


//  Serial << "Entering deep sleep mode for " << SLEEP_DELAY_IN_SECONDS << " seconds..." << endl;
//  ESP.deepSleep(SLEEP_DELAY_IN_SECONDS * 1000000, WAKE_RF_DEFAULT);
//  //ESP.deepSleep(10 * 1000, WAKE_NO_RFCAL);
//  delay(500);   // wait for deep sleep to happen
//
}

void testESP_OnBoardLED()
{
   digitalWrite(boardLED, HIGH);
   delay(50);
   digitalWrite(boardLED, LOW);
   delay(50);
}

