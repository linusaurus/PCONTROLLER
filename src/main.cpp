#include <Arduino.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <UIPEthernet.h>

#define BUF_SIZE 512

byte mac[] = { 0x00, 0x04, 0xA3, 0x21, 0xCA, 0x38 };   // Nanode MAC address.
byte server[] = { 192,168, 10, 3 };                    // Set MQTT broker server IP address
byte ip[]     = { 192,168, 10, 117 };                  // Set the IP address for the arduino nano mcu 



String msgString;                                          // Initialize msgString variable

void callback(char* topic, byte* payload, unsigned int length) {
  
  // Handle message arrived upon a publish from a MQTT client
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
 for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  
      

 if ((char)payload[0] == '0') {
    
    Serial.println("MQTT_STOP");
  
  } 

  if ((char)payload[0] == '1') {
   
    
    Serial.println("MQTT_CLOSING");
    
   
  } 

}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);     // Initialize a MQTT client instance

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


  void setup()
{
  Serial.begin(57600);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}



void loop()
{
 if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
