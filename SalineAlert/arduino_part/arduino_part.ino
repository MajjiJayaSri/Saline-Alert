#include<SoftwareSerial.h>
SoftwareSerial node(7,8);

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

int a=1;
String status=" ";
 
#define DT 2
#define SCK 3
#define sw 4

#define buzzer 5

#define sw1 6
int sw1_sense;
int s=0;

int w;
long sample=0;
float val=0;
long count=0;

unsigned long readCount(void)
{
unsigned long Count;
unsigned char i;
pinMode(DT, OUTPUT);
digitalWrite(DT,HIGH);
digitalWrite(SCK,LOW);
Count=0;
pinMode(DT, INPUT);
while(digitalRead(DT));
for (i=0;i<24;i++)
{
digitalWrite(SCK,HIGH);
Count=Count<<1;
digitalWrite(SCK,LOW);
if(digitalRead(DT))
Count++;
}
digitalWrite(SCK,HIGH);
Count=Count^0x800000;
digitalWrite(SCK,LOW);
return(Count);
}
 
void setup()
{
  Serial.begin(9600);
  node.begin(9600);

  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);

  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);

  pinMode(sw1,INPUT_PULLUP);
  
  Serial.println("READY");
  
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  
  lcd.print(" AUTOMATIC   SALINE ");
  lcd.setCursor(0,1);
  lcd.print("   MONITOR  SYSTEM   ");
  delay(2000);
  lcd.clear();
  calibrate();
}
void beep()
{
  digitalWrite(buzzer,HIGH);delay(500);digitalWrite(buzzer,LOW);delay(500);
  digitalWrite(buzzer,HIGH);delay(500);digitalWrite(buzzer,LOW);delay(500);
}
void alarm()
{
  sw1_sense=digitalRead(sw1);
  if(sw1_sense==0)
  {
    s=s+1;
  }
  if(s>=3)
    {
      digitalWrite(buzzer,LOW);
    }
    else
    {
      Serial.print("alert : ");Serial.println(s);
      beep();
    }
}
void loop()
{ 
  count= readCount();
  w=(((count-sample)/val)-2*((count-sample)/val));
  lcd.clear();
  lcd.print("WEIGHT : ");lcd.print(w);
  lcd.setCursor(0,1);
  lcd.print("LEVEL : ");lcd.print(status);
  
  node.println(w);
  Serial.print("weight : ");Serial.println(w);

  if(w <= 20)
  {
    status="EMPTY";
    alarm();
  }
  if(w > 20 && w <=50)
  {
    s=0;
    status="MEDIUM";
  }
  if(w > 50)
  {
    s=0;
    status="NORMAL";
  }
   
  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
  digitalWrite(13,HIGH);delay(1000);digitalWrite(13,LOW);delay(500);
}
 
void calibrate()
{
  lcd.clear();
  lcd.print("Calibrating...");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
  }
  sample/=100;
  lcd.clear();
  lcd.print("Put 100g & wait");
  count=0;
  while(count<1000)
  {
    count=readCount();
    count=sample-count;
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    count=readCount();
    val+=sample-count;
  }
  val=val/100.0;
  val=val/100.0; // put here your calibrating weight
  lcd.clear();
}
