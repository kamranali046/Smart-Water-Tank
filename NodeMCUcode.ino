#include <OneWire.h>
#include <DallasTemperature.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define rainDigital D3
#define ONE_WIRE_BUS D2
#define RelayPin D0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Set these to run example.
#define FIREBASE_HOST "sensors-readings-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "IlyqxL34VdzN2umdTZQNwwSbNXbzuQN1e5ecMnkS"
#define WIFI_SSID "Punial House"
#define WIFI_PASSWORD "aabb1122"

 float Celcius=0;
 int rainDigitalVal=1;
int n = 0;
void setup()
{
  pinMode(RelayPin, OUTPUT);
  Serial.begin(57600);
  pinMode(rainDigital,INPUT);
  
  sensors.begin();
  
   // connect to wifi.
   pinMode(D1,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("Motor", 0);
  Firebase.set("WATER", 1);
  Firebase.setFloat("Temperature",0.00);
}

void loop()
{ 
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  rainDigitalVal = digitalRead(rainDigital);
  n=Firebase.getInt("Motor");
    
  if (n==1) {
      Serial.print("Motor ON ");
      Serial.print("\t");
      digitalWrite(D1,HIGH);  
      digitalWrite(RelayPin, LOW);
  }
  else {
    Serial.print("Motor OFF");
    Serial.print("\t");
    digitalWrite(D1,LOW); 
    digitalWrite(RelayPin, HIGH);
  }  
  Serial.print("\t");
  Serial.print(rainDigitalVal);
  Serial.print("\t");
  Serial.println(Celcius);
  Firebase.setInt("WATER", rainDigitalVal);
  Firebase.setFloat("Temperature", Celcius);
  
  Serial.println("\t");
  
  if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.print("\t");
        Serial.println(Firebase.error());  
        return;
    }
  delay(500);
}
