#include <Arduino.h>
#include<ESP8266WiFi.h>

//Average runtime=2 seconds , highest observed runtime=4seconds

//wifi setup
const char* ssid = "JRL";
const char* password = "godisgood";
String mid="";
int debug=0;
int postError=0;
int statusLed=D5;

//Host setup
const char* host="mkpsindia.000webhostapp.com";

//function prototype
void contactHost(String mid);
int getVal();


void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(statusLed,OUTPUT);
  //Connecting to WiFi network
  if(debug==1){
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
  }

  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    if(debug==1){
      Serial.print('.');
    }
  }
  if(debug==1){
    Serial.println("WiFi Connected.");
    Serial.print("IP Address : ");
    Serial.print(WiFi.localIP());
  }
  digitalWrite(statusLed,HIGH);
}

void loop() {
  mid="";
  if(getVal()==1){
    if(debug==1){
      Serial.println("FOUND MID-"+mid);
    }
    contactHost(mid);
  }
}

void contactHost(String mid){
  //Connecting to Host Website
  WiFiClient client;
  const int httpPort=80;
  unsigned long clock=millis();//used for time measurement for debugging
  if(!client.connect(host,httpPort)){
    if(debug==1){
      Serial.println("Connection Failed");
    }
    postError=1;
    digitalWrite(statusLed,LOW);
    return;
  }
  else{
    digitalWrite(statusLed,HIGH);
  }

  String url="/Automation/mLog.php";
  String data="mid="+mid;
  String postRequest="POST "+url+" HTTP/1.1\r\n"
  "Host: "+host+"\r\n"+
  //"User-Agent: TechRes\r\n"+
  //"Headers: false\r\n"+
  "Content-Type: application/x-www-form-urlencoded\r\n"+
  "Content-Length: "+data.length()+"\n\n"+
  data+"\r\n"
  "Connection: close\r\n";

  // Serial.println(postRequest);
  client.print(postRequest);

  //Timeout checking block(i have commented it out to save 3 seconds)
  unsigned long timeout=millis();
  while(client.available()==0){
    if(millis()-timeout>3000){
      if(debug==1){
        Serial.println(">>>Client Timeout");
      }
      postError=1;
      client.stop();
      return;
    }
  }

  //THIS BLOCK SHOW THE SERVER RESPONSE. This block has been commented out , because it takes more than 6 seconds to complete
  // while(client.available()){
  //   String line=client.readStringUntil('\r');
  //   if(debug==1){
  //     Serial.print(line);
  //   }
  //   postError=0;
  // }
  client.stop();

  //close Connection
  if(debug==1){
    Serial.println(millis()-clock);
    Serial.println("**Closing Connection.**");
  }

  //Confirmation to be sent to Arduino
  if(postError==1){
    Serial.println("no");
  }
  else{
    Serial.println("ok");
  }

}

int getVal(){
  if(Serial.available()>0){
    //delay(1000);//2000 is optimal
    // while(Serial.available()>0){
    //   //command[i++]=(char)Serial.read();
    //   mid=Serial.readString();
    // }
    mid=Serial.readString();
    return 1;
  }
  return 0;
}
