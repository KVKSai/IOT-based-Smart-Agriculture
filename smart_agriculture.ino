#include <LiquidCrystal.h>
const int rs =2, en =3, d4 =4, d5 =5, d6 =6, d7 =7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11,12);
int mot=9;
int mot1=10;
int t,h;
int ms=A0;
int ldr=A2;
#include "DHT.h"
#define DHTPIN A1 
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
int buz=13;
void setup() {
lcd.begin (16,2);
lcd.setCursor(4,0);
lcd.print("WELCOME:");
lcd.setCursor(0,1);
lcd.print("SMART AGRICULTURE:");
delay(2000);
lcd.clear();
mySerial.begin(115200);
dht.begin();
pinMode(buz,OUTPUT);
pinMode(ms,INPUT);
pinMode(ldr,INPUT);
pinMode(mot,OUTPUT);
pinMode(mot1,OUTPUT);
 wifi_init();
}
void loop() {
int h = dht.readHumidity();
int t = dht.readTemperature();
int mval=analogRead(ms);
//int rval=digitalRead(Rs);
int lval=analogRead(ldr);
upload_iot(t,h,mval,lval);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("T:");
lcd.setCursor(2,0);
lcd.print(t);
lcd.setCursor(5,0);
lcd.print("H:");
lcd.setCursor(7,0);
lcd.print(h);
lcd.setCursor(10,0);
lcd.print("L:");
lcd.setCursor(12,0);
lcd.print(lval);
lcd.setCursor(0,1);
lcd.print("M:");
lcd.setCursor(2,1);
lcd.print(mval);
if(mval>400)
{
  digitalWrite(mot,1);
  digitalWrite(mot1,0);
  
}
else
{
  digitalWrite(mot,0);
  digitalWrite(mot1,0);
}
if((t>40)||(h>90)||(lval<100))
{
  digitalWrite(buz,1);
  delay(1000);
  digitalWrite(buz,0);
}
else
{
 digitalWrite(buz,0);
}
}
void wifi_init()
 { 
   mySerial.println("AT+RST");
  delay(4000);
  mySerial.println("AT+CWMODE=3");
  delay(4000);
  mySerial.print("AT+CWJAP=");
  mySerial.write('"');
  mySerial.print("KVKSai"); // ssid/user name
  mySerial.write('"');
  mySerial.write(',');
  mySerial.write('"'); 
  mySerial.print("internet"); //password
  mySerial.write('"');
  mySerial.println();
  delay(1000);
 }
void upload_iot(int x, int y,int z,int p) //ldr copied int to - x   and gas copied into -y
{
  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  mySerial.println(cmd);
  delay(1500);
 String getStr ="GET /update?api_key=02PB4Z0MX6MUSVNP&field1=";
  getStr += String(x);
  getStr +="&field2=";
  getStr += String(y);
  getStr +="&field3=";
  getStr += String(z);
  getStr +="&field4=";
  getStr += String(p);
  getStr += "\r\n\r\n";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  mySerial.println(cmd);
  delay(1500);
  mySerial.println(getStr);
  delay(1500);
}
