#include <Arduino.h>
#include<ESP8266WiFi.h>

//wifi setup
const char* ssid = "JRL";
const char* password = "godisgood";

//Host setup
const char* host="jacobmahto.000webhostapp.com";

void setup() {
  Serial.begin(9600);
  delay(10);

  //Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print('.');
  }
  Serial.println("WiFi Connected.");
  Serial.print("IP Address : ");
  Serial.print(WiFi.localIP());


}

void loop() {
  void contactHost(String mid);
  contactHost("1");
  delay(2000);
}

void contactHost(String mid){
  //Connecting to Host Website
  WiFiClient client;
  const int httpPort=80;
  if(!client.connect(host,httpPort)){
    Serial.println("Connection Failed");
    return;
  }
  String url="/Automation/mLog.php";
  String data="mid="+mid;
  String postRequest="POST "+url+" HTTP/1.1\r\n"
  "Host: mkpsindia.000webhostapp.com\r\n"+
  "User-Agent: TechRes\r\n"+
  "Headers: false\r\n"+
  "Content-Type: application/x-www-form-urlencoded\r\n"+
  "Content-Length: "+data.length()+"\n\n"+
  data+"\r\n"
  "Connection: close\r\n";

  // Serial.println(postRequest);
  client.print(postRequest);
  unsigned long timeout=millis();
  while(client.available()==0){
    if(millis()-timeout>3000){
      Serial.println(">>>Client Timeout");
      client.stop();
      return;
    }
  }
  while(client.available()){
    String line=client.readStringUntil('\r');
    Serial.print(line);
  }
client.stop();
  //close Connection
  Serial.println();
  Serial.println("**Closing Connection.**");
}
