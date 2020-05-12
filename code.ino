#include<LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);    //A0-RS,A1-E,A2-D4,A3-D5,A4-D6,A5-D7
SoftwareSerial gsmSerial(2,3);
static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
const int buttonPin = 12;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin
int buttonState = 0;         // variable for reading the pushbutton status
SoftwareSerial ss(RXPin, TXPin);
void setup()
{
  Serial.begin(9600);
  gsmSerial.begin(9600);
  ss.begin(9600);
  lcd.begin(16,2);
  pinMode(ledPin, OUTPUT);    
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  lcd.clear(); //clear dispay
  lcd.setCursor(0,0); //set in 1st ine
  lcd.print(" Women Safety"); //lcd print
  lcd.setCursor(0,1); //set in  line
  lcd.print(" & security"); 
  delay(5000); // 5 sec delay
  lcd.clear();
 }
void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
     displayInfo1();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    lcd.print(F("No GPS detected"));
    while(true);
  }
   buttonState = digitalRead(buttonPin);             // read the state of the pushbutton value:
  if (buttonState == HIGH)
   {
    digitalWrite(ledPin, HIGH);
    lcd.setCursor(0,0);
    lcd.print("I AM IN DANGER ");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("MY LOCATION IS");
    delay(2000);
    lcd.clear();
    displayInfo();
    gsmSerial.println("AT+CMGF=1"); 
    delay(5000);
    gsmSerial.println("AT + CMGS = \"+919030140062\"\r");// recipient's mobile number with country code:  
    delay(5000);
    gsmSerial.println(" I AM IN DANGER,I AM AT LOCATION  \r");
    gsmSerial.println("LATITUDE: ");
    gsmSerial.println(gps.location.lat(), 6);
    gsmSerial.println("LONGITUDE: ");
    gsmSerial.print(gps.location.lng(), 6);
    gsmSerial.println((char)26);
    delay(5000);
  }
  else
  {
   digitalWrite(ledPin, LOW);
   lcd.setCursor(0,0);
   lcd.print(" I AM SAFE");
   lcd.setCursor(0,1);
   lcd.print("My Last Loca:");
   delay(3000);
   displayInfo();
   lcd.clear();
   gsmSerial.print("AT+CMGF=1"); 
   delay(5000);
   gsmSerial.println("AT + CMGS = \"+919030140062+\"\r");// recipient's mobile number with       country code
   delay(5000);
   gsmSerial.println(" I AM SAFE");
   gsmSerial.println("MY LAST LOCATION: ");
   gsmSerial.println("LATITUDE: ");
   gsmSerial.println(gps.location.lat(), 6);
   gsmSerial.println("LONGITUDE: ");
   gsmSerial.print(gps.location.lng(), 6);
   gsmSerial.println((char)26);
   delay(5000);
       }
}
void displayInfo()
{   
  if (gps.location.isValid())
  {
    lcd.setCursor(0,0);
    lcd.print("Lat=");
    lcd.print(gps.location.lat(), 6);
    lcd.setCursor(0,1);
    lcd.print("Lng=");
    lcd.print(gps.location.lng(), 6);  
    delay(5000);  
  }
  else
  {
   lcd.print("INVALID");
  } 
}  
void displayInfo1()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    delay(1000);
  }
  else
  {
    Serial.print(F("INVALID"));
  }  
  Serial.println();
  delay(1000);
}
