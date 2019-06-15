#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>

// libraries
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;
char *api_server = "api.pushingbox.com";
char *deviceId = "v45AF84D04313AE0";


const char* ssid = "Ranju";    //Network   Ranju
const char* password = "ranju144";   //Network Password ranju144
WiFiClient client;

unsigned long myChannelNumber = 778414; // ThingSpeak channel number
const char * myWriteAPIKey = "FCZBG9RRAIZJMX91"; // ThingSpeak API write key
// objects
SoftwareSerial mySerial(13, 15); // RX, TX
String inString = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  WiFi.begin(ssid, password);
  delay(100);
  ThingSpeak.begin(client);
  delay(100);
  mySerial.begin(9600); 
}
 
void loop() {  
  while (mySerial.available() > 0) {
  int inChar = mySerial.read();
    if (inChar != '\n') { 
      inString += (char)inChar;
    } 
    else {
      Serial.println(inString);

      int idxV = inString.indexOf('V') ;      
      String strV = inString.substring(0, idxV);
      float valV = strV.toFloat();
      
      int idxA = inString.indexOf('A') ;
      String strA = inString.substring(idxV + 3, idxA);
      float valA = strA.toFloat();

      int idxW = inString.indexOf('L') ;
      String strW = inString.substring(idxA + 3, idxW);
      float valW = strW.toFloat();

      int idxWh = inString.indexOf('T') ;
      String strWh = inString.substring(idxW + 3, idxWh);
      float valWh = strWh.toFloat();

      ThingSpeak.writeField(myChannelNumber, 1,valV, myWriteAPIKey); 
      ThingSpeak.writeField(myChannelNumber, 2,valA, myWriteAPIKey);
      ThingSpeak.writeField(myChannelNumber, 3,valW, myWriteAPIKey); 
      ThingSpeak.writeField(myChannelNumber, 4,valWh, myWriteAPIKey);
      delay(15000);
      if(valV<=1.00){
        get_http(String(valV));
        sendNotification(valV,valA);
        delay(15000);
      }
      inString = "";
    }
  }
}
int get_http(String state)
{
   HTTPClient http;
   int ret = 0;
   Serial.print("[HTTP] begin...\n");
   // configure ifttt server and url  should be HTTP only..not https!!!  (http://)   
   // http.begin("http://maker.ifttt.com/trigger/door/with/key/cJWsJdDX83R_qmUL5s7Y4B"); //HTTP
   //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line  
   http.begin("http://maker.ifttt.com/trigger/solar/with/key/cuua7hd7q3dhTEIeGHDeKuH5GD8GAgwhAsKnLy5dPht"); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}  


void sendNotification(float temp, float pressure) {
    Serial.println("Sending notification to " + String(api_server));
    if (client.connect(api_server, 80)) {
        Serial.println("Connected to the server");
        String message = "devid=" + String(deviceId) +
            "&temp=" + String(temp) +
            "&press=" + String(pressure) +
            "\r\n\r\n";
        client.print("POST /pushingbox HTTP/1.1\n");
        client.print("Host: api.pushingbox.com\n");
        client.print("Connection: close\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(message.length());
        client.print("\n\n");
        client.print(message);
    }
    client.stop();
    Serial.println("Notification sent!");
}
 
