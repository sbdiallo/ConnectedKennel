#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>

int lightPin = A0;  
int ledPin=A2;     
            
int temperaturePin=A1;

Servo door;

const byte TRIGGER_PIN = 2;
const byte ECHO_PIN = 3;    
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
const float SOUND_SPEED = 340.0 / 1000;

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x19 };   // Be sure this address is unique in your network
//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
char DEVID1[] = "v11305D35D6ABD20";

EthernetClient client;
char serverName[] = "api.pushingbox.com";
boolean DEBUG = true;


void setup() {
  door.attach(4);
  door.write(90);

  Serial.begin(115200);

  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); 
  pinMode(ECHO_PIN, INPUT);

  pinMode( ledPin, OUTPUT ); 
/*
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.println("Ethernet ready");
    // print the Ethernet board/shield's IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
*/ 
}
void loop() {

Basket();

Door();

light();

}

void Basket()
{
int reading = analogRead(temperaturePin);  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 // now print out the temperature
 float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
                                               //to degrees ((voltage - 500mV) times 100)
 Serial.print(temperatureC); Serial.println(" degrees C");

 if (temperatureC > 28)
 {
  Serial.println("No need to heat up the basket");
 }
 if(temperatureC>22 && temperatureC<28)
 {
  Serial.println("the basket is heating up to 30°C");
 }
 if(temperatureC == -50)
 {
  Serial.println("No dog is seated on the basket");
 }
 
 delay(1000);          
}

void Door(){
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT);
 
  float distance_mm = measure / 2.0 * SOUND_SPEED;

  if (distance_mm < 50)
  {
    door.write(0);
    //sendToPushingBox(DEVID1);
  }

  else{
    door.write(0);
  }

  delay(500);
 
}

void light(){
  int read = analogRead(lightPin);
 
  analogWrite(ledPin, 300 - (analogRead(lightPin)/4));
  delay(1000);
}

/*
void sendToPushingBox(char devid[]){
  client.stop();
  if(DEBUG){Serial.println("connecting...");}

  if (client.connect(serverName, 80)) {
    if(DEBUG){Serial.println("connected");}

    if(DEBUG){Serial.println("sendind request");}
    client.print("GET /pushingbox?devid=");
    client.print(devid);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
  */



