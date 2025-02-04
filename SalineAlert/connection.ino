#define BLYNK_TEMPLATE_ID "TMPL3NV2uD1GQ"
#define BLYNK_TEMPLATE_NAME "saline alert"
#define BLYNK_AUTH_TOKEN "_BEYUHK826C9xefsrexcshZd0uqjyxKs"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
 
char auth[] = "_BEYUHK826C9xefsrexcshZd0uqjyxKs";    // authuntifaction id
char ssid[] = "project01";   //wifi name
char pass[] = "project01";   //wifi password

String status=" ";
int weight;

WidgetLED indicator(V0);
WidgetLED alert(V3);
 
void setup()
{ 
  Serial.begin(9600);

  pinMode(2,OUTPUT); 
  digitalWrite(2,HIGH);
  delay(50);
  
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi ..");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  
  Blynk.config(auth);
  Serial.println("READY");
}
void loop()
{ 
  if(Serial.available())
  {
    status=" ";
    digitalWrite(2,HIGH);
    status=Serial.readString();
    Serial.print("status : ");Serial.println(status);
    delay(500);
    digitalWrite(2,LOW);
    delay(500);
  }
  weight=status.toInt();
  Serial.print("weight : ");Serial.println(weight);
  Blynk.virtualWrite(V1, weight);
  
  digitalWrite(2,HIGH);indicator.on();delay(500);digitalWrite(2,LOW);indicator.off();delay(500);
  Blynk.run(); // Initiates Blynk
}
